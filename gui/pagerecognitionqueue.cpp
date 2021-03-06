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

#include <QDebug>
#include <QMutexLocker>
#include <QCoreApplication>

#include "language.h"
#include "pagerecognitionqueue.h"
#include "pagerecognizer.h"
#include "page.h"
#include "packet.h"
#include "quneiform_debug.h"

PageRecognitionQueue::PageRecognitionQueue(QObject * parent) :
        QObject(parent),
        page_count_(0),
        abort_(false),
        page_error_num_(0)
{
    recognizer_ = new PageRecognizer(this, PageRecognizer::RUNTIME);

    connect(recognizer_, SIGNAL(percentsDone(int)), SLOT(handlePagePercents(int)));
    connect(recognizer_, SIGNAL(failed(QString)), SLOT(handleFail(QString)));
}

void PageRecognitionQueue::add(Packet * packet) {
    Q_CHECK_PTR(packet);

    for(int i = 0; i < packet->pageCount(); i++)
        add(packet->pageAt(i));
}

void PageRecognitionQueue::add(Page * p)
{
    if(!p) {
        qCritical() << Q_FUNC_INFO << "NULL page given";
        return;
    }

    QMutexLocker l(&queue_lock_);

    if(!pages_.contains(p))
        pages_.enqueue(p);
}

void PageRecognitionQueue::abort() {
    QMutexLocker l(&abort_lock_);
    abort_ = true;
    recognizer_->abort();
}

void PageRecognitionQueue::clearPageFault(const QString& page) {
     page_errors_.remove(page);
}

QString PageRecognitionQueue::pageError(const QString& imagePath) const {
    return page_errors_.value(imagePath, "");
}

bool PageRecognitionQueue::isEmpty() const {
    return pages_.isEmpty();
}

bool PageRecognitionQueue::isFailed(const QString& path) const {
    return page_errors_.contains(path);
}

void PageRecognitionQueue::handleFail(const QString& msg) {
    setPageFault(recognizer_->pagePath(), msg);
    handlePagePercents(100);
    page_error_num_++;
    emit failed(msg);
}

void PageRecognitionQueue::handlePagePercents(int perc) {
    qreal page_percent = 100.0 / page_count_;
    int page_done = page_count_ - pageCount() - 1;
    qreal percent_done = ((page_done * 100 + perc) * page_percent) / 100.0;
    emit percentDone(static_cast<int>(percent_done));
    QCoreApplication::processEvents();
}

PageRecognizer * PageRecognitionQueue::recognizer() {
    return recognizer_;
}

void PageRecognitionQueue::start()
{
    QMutexLocker l(&queue_lock_);
    emit started();
    page_count_ = pages_.count();
    page_error_num_ = 0;
    int pages_done = 0;

    if(abort_)
        abort_ = false;

    while(!pages_.empty()) {
        {
            QMutexLocker l(&abort_lock_);
            if(abort_) {
                pages_.clear();
                abort_ = false;
                break;
            }
        }

        Page * p = pages_.dequeue();
        clearPageFault(p->imagePath());
        emit pageStarted(p->name());
        recognizer_->setPage(p);
        bool ok = recognizer_->recognize();

        if(ok)
            pages_done++;
    }

    if(abort_)
        abort_ = false;

    emit finished(pages_done);
}

void PageRecognitionQueue::startSegmentation()
{
    QMutexLocker l(&queue_lock_);
    emit started();
    page_count_ = pages_.count();
    page_error_num_ = 0;
    int pages_done = 0;

    if(abort_)
        abort_ = false;

    while(!pages_.empty()) {
        {
            QMutexLocker l(&abort_lock_);
            if(abort_) {
                pages_.clear();
                abort_ = false;
                break;
            }
        }

        Page * p = pages_.dequeue();
        clearPageFault(p->imagePath());
        emit pageStarted(p->name());
        recognizer_->setPage(p);
        bool ok = recognizer_->analyze();

        if(ok) {
            pages_done++;
        }
        else {
            qWarning() << Q_FUNC_INFO << "page analyze failed";
        }
    }

    if(abort_)
        abort_ = false;

    emit finished(pages_done);
}

int PageRecognitionQueue::pageCount() const {
    return pages_.count();
}

int PageRecognitionQueue::pageErrorNum() const {
    return page_error_num_;
}

void PageRecognitionQueue::setPageFault(const QString& page, const QString& msg) {
    page_errors_[page] = msg;
}
