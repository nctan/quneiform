/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <QDebug>
#include <QImageReader>

#include "pagebinarizator.h"
#include "binarizationsettings.h"
#include "page.h"
#include "imageurl.h"
#include "imagecache.h"
#include "rimage/binarizatorfactory.h"
#include "rdib/qtimageloader.h"
#include "common/ctdib.h"
#include "common/binarizeoptions.h"

PageBinarizator::PageBinarizator(QObject *parent) :
    QObject(parent)
{
}

bool PageBinarizator::binarize(Page * page)
{
    if(!page) {
        qWarning() << Q_FUNC_INFO << "NULL page given";
        return false;
    }

    QImage img = binarize(page->imageURL(), page->binarizationSettings());
    if(img.isNull()) {
        qWarning() << Q_FUNC_INFO << "binarization failed:" << page->imageURL();
        return false;
    }

    page->setBinarizedImage(img);
    return true;
}

QImage PageBinarizator::binarize(const ImageURL& path, const BinarizationSettings& settings)
{
    using namespace cf;
    QImage src_image;
    if(!ImageCache::load(path, &src_image)) {
        qWarning() << Q_FUNC_INFO << "can't load image:" << path;
        return QImage();
    }

    QtImageLoader loader;
    ImagePtr raw_data = loader.fromQImage(src_image);
    if(!raw_data) {
        qWarning() << Q_FUNC_INFO << "can't load image in CF module:" << path;
        return QImage();
    }

    CTDIB dib;
    if(!dib.setBitmap(raw_data->data())) {
        qCritical() << Q_FUNC_INFO << "can't set bitmap data";
        return QImage();
    }

    BinarizatorPtr bptr = BinarizatorFactory::instance().make(settings.toCfOptions());
    bptr->setSource(&dib);
    CTDIB * bin_dib = bptr->binarize();

    if(!bin_dib) {
        qCritical() << Q_FUNC_INFO << "can't binarize DIB";
        return src_image;
    }

    BitmapPtr data = NULL;
    if(!bin_dib->bitmap(&data)) {
        qCritical() << Q_FUNC_INFO << "can't get DIB handle";
        return src_image;
    }

    QImage res;
    if(!res.loadFromData((uchar*) data, dib.dibSize(), "DIB")) {
        qCritical() << Q_FUNC_INFO << "can't load DIB";
        delete bin_dib;
        return src_image;
    }

    delete bin_dib;

    return res;
}
