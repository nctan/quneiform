/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <iostream>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QPixmapCache>
#include <QComboBox>
#include <qdebug.h>
#include <cmath>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "document.h"
#include "languageselect.h"
#include "page.h"
#include "thumbnailwidget.h"

static const char * ORGANIZATION = "openocr.org";
static const char * APPLICATION = "Cuneiform OCR";
static const int PIXMAP_CACHE_SIZE = 1024 * 1024 * 6;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui_(new Ui::MainWindow), doc_(new Document(this)) {
	QPixmapCache::setCacheLimit(PIXMAP_CACHE_SIZE);
	setupUi();
	createActions();
	readSettings();
}

MainWindow::~MainWindow() {
}

void MainWindow::about() {
	QMessageBox::about(0, "About", "Cuneiform GUI");
}

void MainWindow::clearScene() {
	foreach(QGraphicsItem * item, scene_.items()) {
		scene_.removeItem(item);
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	writeSettings();
	event->accept();
}

void MainWindow::createActions() {
	Q_ASSERT(!ui_.isNull());
	connect(ui_->actionAbout, SIGNAL(triggered()), SLOT(about()));
	connect(ui_->actionOpen, SIGNAL(triggered()), SLOT(openImages()));
	connect(ui_->thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(showPageImage(Page*)));
	connect(ui_->thumbs_, SIGNAL(thumbSelected(Page*)), SLOT(showPageText(Page*)));
	connect(ui_->actionZoom_In, SIGNAL(triggered()), ui_->image_view_, SLOT(zoomIn()));
	connect(ui_->actionZoom_Out, SIGNAL(triggered()), ui_->image_view_, SLOT(zoomOut()));
	connect(ui_->actionFitWidth, SIGNAL(triggered()), ui_->image_view_, SLOT(fitWidth()));
	connect(ui_->actionFitPage, SIGNAL(triggered()), ui_->image_view_, SLOT(fitPage()));
	connect(ui_->actionRecognizeAll, SIGNAL(triggered()), this, SLOT(recognizeAll()));
	connect(ui_->thumbs_, SIGNAL(thumbRecognize(Page*)), SLOT(recognizePage(Page*)));
	connect(ui_->actionRotateLeft, SIGNAL(triggered()), SLOT(rotateLeft()));
	connect(ui_->actionRotateRight, SIGNAL(triggered()), SLOT(rotateRight()));
}

void MainWindow::openImage(const QString& path) {
	Q_CHECK_PTR( doc_);

	QFileInfo info(path);
	if(!info.exists()) {
		QMessageBox::critical(NULL, tr("Error"), QString(tr("File \"%1\" not exists")).arg(path));
		return;
	}

	statusBar()->showMessage(QString(tr("Opening \"%1\"")).arg(path));

	Page * p = new Page(0, path);
	doc_->append(p);

	statusBar()->showMessage("");

	//	first page
	if(doc_->pageCount() == 1) {
		ui_->actionRecognizeAll->setEnabled(true);
	}
}

void MainWindow::openImages() {
	QStringList files = QFileDialog::getOpenFileNames(NULL, "Open Dialog", "",
			"Images (*.gif *.png *.xpm *.jpg *.tif *.bmp)");
	qDebug() << "[openImages()]";
	foreach(QString image_path, files) {
		openImage(image_path);
	}
}

void MainWindow::readSettings() {
	QSettings settings(ORGANIZATION, APPLICATION);
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(650, 450)).toSize();
	resize(size);
	move(pos);
}

void MainWindow::recognizeAll() {
	Q_CHECK_PTR(doc_);

	if(!doc_->countSelected()) {
		QMessageBox::warning(NULL, tr("Warning"), tr("No page selected"));
		return;
	}

	for(int i = 0, total = doc_->pageCount(); i < total; i++) {
		if(doc_->page(i)->isSelected()) {
			doc_->page(i)->setLanguage(lang_select_->currentLanguageCode());
			recognizePage(doc_->page(i));
		}
	}
}

void MainWindow::recognizePage(Page * page) {
	Q_CHECK_PTR(page);

	if(page->isRecognized()) {
		if(QMessageBox::Ok != QMessageBox::question(NULL, tr("Warning"), tr("Page already recognized. Do you want do rerecognize it?"))) {
			return;
		}
	}

	page->recognize();
	showPageText(page);
}

void MainWindow::rotate(int factor) {
	Page * p = ui_->thumbs_->currentPage();
	if (!p) {
		qDebug() << "No page selected";
		return;
	}

	p->rotate(factor);
}

void MainWindow::rotateLeft() {
	rotate(-90);
}

void MainWindow::rotateRight() {
	rotate(90);
}

void MainWindow::setupUi() {
	ui_->setupUi(this);
	setUnifiedTitleAndToolBarOnMac(true);
	ui_->thumbs_->setDocument(doc_);
	ui_->image_view_->setScene(&scene_);
	setZoomEnabled(false);
//	lang_select_ = new LanguageSelect(this);
//	ui_->mainToolBar->addWidget(lang_select_);
	QMenu * lang_menu  = ui_->menuRecognition->addMenu(tr("Language"));
}

void MainWindow::setZoomEnabled(bool value) {
	ui_->actionZoom_In->setEnabled(value);
	ui_->actionZoom_Out->setEnabled(value);
	ui_->actionFitWidth->setEnabled(value);
	ui_->actionFitPage->setEnabled(value);
	ui_->actionRotateLeft->setEnabled(value);
	ui_->actionRotateRight->setEnabled(value);
}

void MainWindow::showPageImage(Page * page) {
	Q_CHECK_PTR(page);
	statusBar()->showMessage(QFileInfo(page->imagePath()).fileName());
	clearScene();

	QPixmap image;
	if (!QPixmapCache::find(page->imagePath(), &image)) {
		image.load(page->imagePath());
		QPixmapCache::insert(page->imagePath(), image);
	}

	scene_.addPixmap(image);
	scene_.setSceneRect(image.rect());
	setZoomEnabled(true);
	ui_->image_view_->setDragMode(QGraphicsView::ScrollHandDrag);
	ui_->image_view_->setPage(page);
}

void MainWindow::showPageText(Page * page) {
	Q_CHECK_PTR(page);
	Q_CHECK_PTR(ui_->text_view_);
	ui_->text_view_->document()->setHtml(page->ocrText());
}

void MainWindow::writeSettings() {
	QSettings settings(ORGANIZATION, APPLICATION);
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}
