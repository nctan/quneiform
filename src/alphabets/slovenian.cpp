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

#include "slovenian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new SlovenianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_SLOVENIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_SLOVENIAN,
        AlphabetFactory::AlphabetTables("rec1cen.dat", "rec2cen.dat"));
}

namespace cf
{

SlovenianAlphabet::SlovenianAlphabet()
{
    addSymbol(CC_inv_roof);
    addSymbol(c_inv_roof);
    addSymbol(SS_inv_roof_latin);
    addSymbol(s_inv_roof_latin);
    addSymbol(ZZ_inv_roof_latin);
    addSymbol(z_inv_roof_latin);
}

language_t SlovenianAlphabet::language() const
{
    return LANGUAGE_SLOVENIAN;
}

}
