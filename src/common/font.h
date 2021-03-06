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

#ifndef CIF_FONT_H_
#define CIF_FONT_H_

#include "globus.h"
#include "common/serialize.h"

namespace cf
{

typedef short font_number;

const int MaxFontSize = 72;
const int DefFontSize = 24;
const int ChangedKeglSize = 6;
const unsigned char TIRE = 0x97; //'-'

enum format_align_t
{
    //- выравниваение по левому краю
    FORMAT_ALIGN_LEFT = 0,
    //- выравнивание по правому краю
    FORMAT_ALIGN_RIGHT = 1,
    //- выравнивание по ширине
    FORMAT_ALIGN_JUSTIFY = 2,
    //- выравнивание по центру
    FORMAT_ALIGN_CENTER = 3,
    //- каждая строка состоит из одного слова, выравнивание левому краю
    FORMAT_ALIGN_ONE = 4,
    //- список
    FORMAT_ALIGN_LIST = 5
};

// Font bits
enum
{
    FORMAT_FONT_SERIF = 1,
    FORMAT_FONT_SANS = 2,
    FORMAT_FONT_BOLD = 4,
    FORMAT_FONT_ITALIC = 16,
    FORMAT_FONT_UNDERLINE = 64,
    // user defined
    FORMAT_FONT_NARROW = 128
};

class KEGL
{
    public:
        short OldKegl;
        short NewKegl;
        short Count;
};

class CLA_EXPO FontEntry
{
    public:
        FontEntry() :
            fontNumber(0), fontPitchAndFamily(0), charset_(0) {
        }

        FontEntry(uchar number, uchar family, uchar charset, const std::string& name) :
            fontNumber(number), fontPitchAndFamily(family), charset_(charset), name_(name) {
        }
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar & fontNumber;
            ar & fontPitchAndFamily;
            ar & charset_;
            ar & name_;
        }
#endif
    private:
        uchar fontNumber;
        uchar fontPitchAndFamily;
        uchar charset_;
        std::string name_;
};

}

#endif
