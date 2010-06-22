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
#include <iostream>
#include <fstream>
#include "testcedchar.h"
#include <common/tostring.h>
#include <ced/cedchar.h>
#include <ced/cedarchive.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDChar);

using namespace CIF;

void TestCEDChar::testInit() {
}

void TestCEDChar::testSerialize() {
#ifdef CF_SERIALIZE
    CEDChar chr;
    chr.addAlternative(Letter('a', 128));
    chr.setFontHeight(11);
    chr.setBackgroundColor(Color(255, 0, 0));
    chr.setColor(Color(0, 0, 255));
    chr.setBoundingRect(Rect(Point(1, 2), Point(3, 4)));

    // save data to archive
    {
        std::ofstream ofs("serialize_cedchar.txt");
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << chr;
    }

    CEDChar new_chr;
    {
        // create and open an archive for input
        std::ifstream ifs("serialize_cedchar.txt");
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_chr;

        CPPUNIT_ASSERT_EQUAL(chr.alternativeCount(), new_chr.alternativeCount());
        CPPUNIT_ASSERT_EQUAL(chr.fontHeight(), new_chr.fontHeight());
        CPPUNIT_ASSERT_EQUAL(chr.backgroundColor(), new_chr.backgroundColor());
        CPPUNIT_ASSERT_EQUAL(chr.color(), new_chr.color());
        CPPUNIT_ASSERT_EQUAL(chr.boundingRect(), new_chr.boundingRect());
    }

#endif

}
