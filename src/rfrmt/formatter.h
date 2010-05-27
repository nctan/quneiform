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

#ifndef FORMATTER_H_
#define FORMATTER_H_

#include "globus.h"
#include "formatoptions.h"

namespace CIF
{

class CEDPage;

class CLA_EXPO Formatter
{
    public:
        Formatter(const FormatOptions& opt = FormatOptions());
        /**
         * Formats file
         * @return pointer to created CEDPage document
         * @note caller should free return value
         */
        CEDPage * format(const std::string& fileName) const;

        /**
         * Returns format options
         */
        FormatOptions options() const;

        /**
         * Sets format options
         */
        void setOptions(const FormatOptions& opts);
    private:
        void setInnerOptions() const;
    private:
        FormatOptions opts_;
};

}

#endif /* FORMATTER_H_ */