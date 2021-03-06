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

#ifndef TESTCRTFHORIZONTALCOLUMN_H_
#define TESTCRTFHORIZONTALCOLUMN_H_

#include <cppunit/extensions/HelperMacros.h>

class TestCRtfHorizontalColumn: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCRtfHorizontalColumn);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testProcessSpaceByHist);
    CPPUNIT_TEST(testMarkSmallColumns);
    CPPUNIT_TEST(testMaxVColumnHeight);
    CPPUNIT_TEST(testMaxVColumnWidth);
    CPPUNIT_TEST(testAllTextToFrames);
    CPPUNIT_TEST(testFillSingleTerminalColumnIndex);
    CPPUNIT_TEST(testFillAllTerminalColumnIndex);
    CPPUNIT_TEST(testSortColumns);
    CPPUNIT_TEST(testSortColumnsInGroup);
    CPPUNIT_TEST(testFindHighestUnsortedColumn);
    CPPUNIT_TEST(testFindHighestUnsortedColumnInGroup);
    CPPUNIT_TEST(testSetDrawCallback);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testMarkSmallColumns();
    void testMaxVColumnHeight();
    void testMaxVColumnWidth();
    void testProcessSpaceByHist();
    void testAllTextToFrames();
    void testFillSingleTerminalColumnIndex();
    void testFillAllTerminalColumnIndex();
    void testSortColumns();
    void testSortColumnsInGroup();
    void testFindHighestUnsortedColumn();
    void testFindHighestUnsortedColumnInGroup();
    void testSetDrawCallback();
};

#endif /* TESTCRTFHORIZONTALCOLUMN_H_ */
