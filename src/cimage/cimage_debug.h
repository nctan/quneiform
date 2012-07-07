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

#ifndef CIMAGE_DEBUG_H
#define CIMAGE_DEBUG_H

#include <boost/current_function.hpp>

#include "common/tostring.h"
#include "common/debug.h"

#define CIMAGE_ERROR Debug() << "[CIMAGE] ERROR "
#define CIMAGE_ERROR_FUNC Debug() << "[CIMAGE] ERROR " << BOOST_CURRENT_FUNCTION << ' '

#define CIMAGE_DEBUG Debug() << "[CIMAGE] "
#define CIMAGE_DEBUG_FUNC Debug() << "[CIMAGE] " << BOOST_CURRENT_FUNCTION << ' '

#define CIMAGE_WARNING Debug() << "[CIMAGE] WARNING "
#define CIMAGE_WARNING_FUNC Debug() << "[CIMAGE] WARNING " << BOOST_CURRENT_FUNCTION << ' '

#endif // CIMAGE_DEBUG_H