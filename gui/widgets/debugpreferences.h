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

#ifndef DEBUGPREFERENCES_H
#define DEBUGPREFERENCES_H

#include "preferenceswidget.h"

class QVBoxLayout;

class DebugPreferences : public PreferencesWidget
{
    Q_OBJECT
public:
    explicit DebugPreferences(QWidget * parent = 0);
private:
    void setupLayout();
    void setupUI();
    void setupUIFormatBBoxes();
private:
    QVBoxLayout * layout_;
};

#endif // DEBUGPREFERENCES_H
