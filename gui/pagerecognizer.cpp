/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <QImage>
#include <QDebug>
#include <sstream>

#include "pagerecognizer.h"
#include "page.h"
#include "cuneiform.h"
#include "rdib/qtimageloader.h"
#include "common/lang_def.h"
#include "quneiform_debug.h"
#include "rectexporter.h"

PageRecognizer::PageRecognizer(Page * p, QObject * parent)
    : QObject(parent),
    page_(p),
    language_(LANGUAGE_ENGLISH),
    paused_(false),
    aborted_(false) {
}

void PageRecognizer::abort() {
    aborted_ = true;
    CF_INFO("aborted")
}

void PageRecognizer::doRecognize() {
    cf::Puma::instance().recognize();
    emit recognized();
}

void PageRecognizer::formatResult() {
    Q_CHECK_PTR(page_);

    cf::Puma::instance().formatResult();
    cf::RectExporter exporter(cf::Puma::instance().cedPage());
    exporter.collect();
//    qDebug() << exporter.paragraphs();
//    qDebug() << exporter.pictures();

    page_->setRects(exporter.pictures(), Page::PICTURE);
    emit formatted();
}

bool PageRecognizer::isPaused() const {
    return paused_;
}

QImage PageRecognizer::loadImage() const {
    Q_CHECK_PTR(page_);

    QImage img(page_->imagePath());

    // select page area
    if(page_->pageArea().isValid())
        img = img.copy(page_->pageArea());

    // rotate
    if(page_->angle() != 0) {
        QTransform t;
        t.rotate(page_->angle());
        img = img.transformed(t);
    }


    return img.convertToFormat(QImage::Format_RGB888);
}

Page * PageRecognizer::page() {
    return page_;
}

void PageRecognizer::pause() {
    if(paused_)
        return;

    paused_ = true;
    pause_.lock();
    CF_INFO("paused")
}

void PageRecognizer::resume() {
    if(!paused_)
        return;

    paused_ = false;
    pause_.unlock();
    CF_INFO("resumed")
}

void PageRecognizer::start() {
    recognize();
}

void PageRecognizer::recognize() {
    Q_CHECK_PTR(page_);

    try {       
        setRecognizeOptions();
        cf::QtImageLoader loader;
        QImage img = loadImage();
        cf::ImagePtr image = loader.load(&img);
        if (!image)
            throw Page::Exception("[PageRecognizer::openImage] can't open image");

        cf::Puma::instance().open(image);
        doRecognize();
        formatResult();
        saveOcrText();
    }
    catch(std::exception& e) {
        page_->setFlag(Page::RECOGNITION_FAILED);
        emit failed(e.what());
    }
}

void PageRecognizer::saveOcrText() {
    Q_CHECK_PTR(page_);

    std::ostringstream buf;
    cf::Puma::instance().save(buf, cf::FORMAT_TEXT);
    page_->setOcrText(QString::fromUtf8(buf.str().c_str()));
}

void PageRecognizer::setLanguage(int language) {
    language_ = language;
}

void PageRecognizer::setPage(Page * p) {
    Q_CHECK_PTR(p);

    page_ = p;
}

void PageRecognizer::setRecognizeOptions() {
    Q_CHECK_PTR(page_);

    cf::RecognizeOptions recognize_opts;

    RecognitionSettings settings = page_->recognitionSettings();
    recognize_opts.setFax(settings.fax());
    recognize_opts.setDotMatrix(settings.dotMatrix());
    recognize_opts.setOneColumn(settings.oneColumn());
    recognize_opts.setPictureSearch(settings.picturesSearch());
    recognize_opts.setSpellCorrection(settings.spelling());

    recognize_opts.setLanguage(static_cast<language_t>(language_));

    cf::Puma::instance().setRecognizeOptions(recognize_opts);
    cf::Config::instance().setDebug(false);
}

