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

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPixmap>
#include <QtGui/QLayout>
#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtGui/QMenu>

#include "thumbnailwidget.h"
#include "thumbnaillist.h"
#include "imagecache.h"
#include "page.h"

static const int THUMB_IMAGE_WIDTH = 90;
static const int THUMB_IMAGE_MARGIN = 5;
static const int THUMB_WIDTH = 150;
static const int THUMB_HEIGHT = 150;

ThumbnailWidget::ThumbnailWidget(Page * page, ThumbnailList * parent) :
    QFrame(parent), page_(page), layout_(NULL), thumb_(NULL), checked_(NULL) {

    setupFrame();
    setupLayout();
    setupPixmap();
    setupLabel();
    setupToolTip();
    setupCheckBox();

    connect(checked_, SIGNAL(toggled(bool)), SLOT(selectPage(bool)));
    connect(this, SIGNAL(contextMenuCreated(QMenu*)), parent, SLOT(setupContextMenu(QMenu*)));
    connect(this, SIGNAL(invalidImage(const QString&)), parent, SLOT(handleInvalidImage(const QString&)));
    connect(page, SIGNAL(rotated(int)), SLOT(rotate(int)));
}

void ThumbnailWidget::contextMenuEvent(QContextMenuEvent * event) {
    QMenu * menu = new QMenu(this);
    emit contextMenuCreated(menu);
    menu->addAction(QIcon(":/list_recognize.png"), tr("Recognize"), this, SLOT(recognizeThumb()));
    menu->addAction(QIcon(":/list_remove.png"), tr("Delete"), this, SLOT(removePage()));
    menu->addAction(QIcon(":/img/oxygen/22x22/document_properties.png"), tr("Properties"), this, SLOT(showProperties()));

//    menu->setupActions();
    menu->exec(event->globalPos());
    delete menu;
}

void ThumbnailWidget::selectPage(bool value) {
    qDebug() << "[ThumbnailWidget::selectPage]" << value;
    page_->setSelected(value);
}

void ThumbnailWidget::showProperties() {
    QMessageBox::information(NULL, tr("Properties"), pageProperties());
}

void ThumbnailWidget::highlight(bool value) {
    if (value)
        setBackgroundRole(QPalette::Highlight);
    else
        setBackgroundRole(QPalette::Background);
}

bool ThumbnailWidget::isChecked() const {
    Q_CHECK_PTR(checked_);
    return checked_->isChecked();
}

void ThumbnailWidget::mousePressEvent(QMouseEvent * event) {
    Q_CHECK_PTR(event);

    switch (event->button()) {
    case Qt::LeftButton:
        emit clicked();
        break;
    case Qt::RightButton:
        break;
    default:
        qDebug("Unhandled mouse event");
    }
}

Page * ThumbnailWidget::page() const {
    return page_;
}

QString ThumbnailWidget::pageProperties() const {
    Q_CHECK_PTR(page_);

    QString res = tr("Filename: \"%1\"\n").arg(page_->imagePath());
    res += tr("Size: %1x%2\n").arg(page_->imageSize().width()).arg(page_->imageSize().height());
//    res += tr("Rotation: %1\n").arg(page_->r);
    return res;
}

void ThumbnailWidget::recognizeThumb() {
	emit recognize(page_);
}

void ThumbnailWidget::rotate(int angle) {
	Q_CHECK_PTR(page_);

	QTransform t;
	QPixmap new_pixmap = thumb_->pixmap()->transformed(t.rotate(angle));
	thumb_->setPixmap(new_pixmap);
}


void ThumbnailWidget::removePage() {
    emit removed(page_);
}

void ThumbnailWidget::setChecked(bool value) {
    Q_CHECK_PTR(checked_);

    checked_->setChecked(value);
}

void ThumbnailWidget::setName(const QString& name) {
    Q_CHECK_PTR(checked_);

    checked_->setText(name);
}

void ThumbnailWidget::setupCheckBox() {
    Q_CHECK_PTR(layout_);
    Q_CHECK_PTR(page_);

    checked_ = new QCheckBox;
    checked_->setChecked(page_->isSelected());
    layout_->addWidget(checked_, 0, Qt::AlignHCenter);
}

void ThumbnailWidget::setupFrame() {
    setAutoFillBackground(true);
    //setFrameShape(QFrame::Box);
}

void ThumbnailWidget::setupLabel() {
    Q_CHECK_PTR(page_);
    Q_CHECK_PTR(layout_);

    //page_name_ = new QLabel;
    //page_name_->setTextFormat(Qt::PlainText);
    //layout_->addWidget(page_name_, 0, Qt::AlignHCenter);
}

void ThumbnailWidget::setupLayout() {
    layout_ = new QVBoxLayout(this);
    layout_->setSpacing(0);
    layout_->setMargin(0);
    setFixedWidth(THUMB_WIDTH);
    setFixedHeight(THUMB_HEIGHT);
}

void ThumbnailWidget::setupPixmap() {
    Q_CHECK_PTR(page_);
    Q_CHECK_PTR(layout_);

    QPixmap image;
    if(!ImageCache::load(page_->imagePath(), &image)) {
        qDebug() << "[ThumbnailWidget::setupPixmap] can't load pixmap";
        return;
    }

    thumb_ = new QLabel;
    thumb_->setMargin(THUMB_IMAGE_MARGIN);
    thumb_->setPixmap(image.scaledToWidth(THUMB_IMAGE_WIDTH));
    // stretch image
    static const int STRETCH_KOEF = 4;
    layout_->addWidget(thumb_, STRETCH_KOEF, Qt::AlignHCenter);
}

void ThumbnailWidget::setupToolTip() {
    Q_CHECK_PTR(page_);

    setToolTip(page_->imagePath());
}

void ThumbnailWidget::toggleSelection() {
    Q_CHECK_PTR(checked_);

    checked_->toggle();
}
