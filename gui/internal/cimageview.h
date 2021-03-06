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

#ifndef CIMAGEVIEW_H
#define CIMAGEVIEW_H

#include <QDialog>

class QListWidget;
class QLabel;
class QListWidgetItem;
class CImageStorage;
class QScrollArea;

class CImageView : public QDialog
{
    Q_OBJECT
public:
    explicit CImageView(QWidget * parent = 0);
    ~CImageView();
private:
    void init();
private slots:
    void showImage(QListWidgetItem* item);
private:
    QListWidget * image_list_;
    QLabel * image_;
    CImageStorage * storage_;
    QScrollArea * scroll_;
};

#endif // CIMAGEVIEW_H
