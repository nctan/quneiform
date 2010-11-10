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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Document;
class LanguageSelect;
class Page;
class ImageWidget;
class ThumbnailList;
class QSignalMapper;
class QProgressDialog;
class QTextEdit;
class QHBoxLayout;

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);
public slots:
    void about();
    void changeDocumentLanguage(int lang);
    void documentChange();
    void documentSave();
    void imageDuplication(const QString& path);
    void openImages();
    void openImages(const QStringList& paths);
    void openPacket();
    void openPacket(const QString& path);
    void recognizeAll();
    void recognizePage(Page * page);
    void rotate(int factor);
    void rotateLeft();
    void rotateRight();
    void savePacket();
    void savePacket(const QString& path);
    void savePage(Page * page);
    void showPageImage(Page * page);
    void showPageText(Page * page);
private slots:
    void disableViewActions();
    void openRecent();
private:
    void addRecentFile(const QString& path);
    void addRecentFileMenuAction(const QString& path);
    void connectActions();
    void connectThumbs();
    void enablePageActions(bool value);
    void mapLanguageActions(const QList<QAction*>& actions);
    void mapLanguageMenuActions();
    void mapLanguageToolButtonActions();
    bool openImage(const QString& path, bool allowDuplication = false);
    void populateRecentFiles();
    void readSettings();
    void selectLanguage(int lang);
    void setupDocument();
    void setupImageView();
    void setupLanguageUi();
    void setupShortcuts();
    void setupTextView();
    void setupThumbs();
    void setupUi();
    void setupUiLayout();
    void writeSettings();
private:
    Ui::MainWindow * ui_;
    Document * doc_;
    LanguageSelect * lang_select_;
    QSignalMapper * lang_mapper_;
    QMenu * lang_menu_;
    QProgressDialog * progress_;
    ImageWidget * image_widget_;
    QTextEdit * text_view_;
    ThumbnailList * thumbs_;
    QHBoxLayout * main_layout_;
    QStringList recent_files_;
};

#endif // MAINWINDOW_H
