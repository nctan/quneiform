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

#include "polish.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new PolishAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_POLISH, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_POLISH,
        AlphabetFactory::AlphabetTables("rec1cen.dat", "rec2cen.dat"));
}

namespace cf
{

PolishAlphabet::PolishAlphabet()
{
    addSymbol(POLISH_SS);
    addSymbol(POLISH_s);
    addSymbol(POLISH_ZZR);
    addSymbol(POLISH_zr);
    addSymbol(POLISH_ZZD);
    addSymbol(POLISH_zd);
    addSymbol(POLISH_CC);
    addSymbol(POLISH_c);
    addSymbol(POLISH_EE);
    addSymbol(POLISH_e);
    addSymbol(POLISH_NN);
    addSymbol(POLISH_n);
    addSymbol(POLISH_OO);
    addSymbol(POLISH_o);
    addSymbol(POLISH_LL);
    addSymbol(POLISH_l);
}

language_t PolishAlphabet::language() const
{
    return LANGUAGE_POLISH;
}

}
