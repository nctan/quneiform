/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef FORMATSETTINGSDIALOG_H
#define FORMATSETTINGSDIALOG_H

#include <QDialog>
#include <QPixmap>
#include "formatsettings.h"

namespace Ui {
    class FormatSettingsDialog;
}

class FormatSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FormatSettingsDialog(const FormatSettings& settings);
    ~FormatSettingsDialog();

    const FormatSettings& settings() const;
private slots:
    void save();
    void showAlternativeColorDialog();
    void load();
private:
    Ui::FormatSettingsDialog *ui;
    FormatSettings settings_;
    QPixmap pixmap_;
};

#endif // FORMATSETTINGSDIALOG_H
