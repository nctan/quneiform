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

#ifndef TESTCRTFSTRING_H_
#define TESTCRTFSTRING_H_

#include <cppunit/extensions/HelperMacros.h>

class TestCRtfString: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCRtfString);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testClearWords);
    CPPUNIT_TEST(testToString);
    CPPUNIT_TEST(testRealLength);
    CPPUNIT_TEST(testFirstChar);
    CPPUNIT_TEST(testStartsWith);
    CPPUNIT_TEST(testStartsWithDigit);
    CPPUNIT_TEST(testStartWithDash);
    CPPUNIT_TEST(testCharCount);
    CPPUNIT_TEST(testLeftBorder);
    CPPUNIT_TEST(testRightBorder);
    CPPUNIT_TEST(testBorders);
    CPPUNIT_TEST(testCharTotalLength);
    CPPUNIT_TEST(testEndsWith);
    CPPUNIT_TEST(testIsEndOfSentence);
    CPPUNIT_TEST(testHasChars);
    CPPUNIT_TEST(testHasBigSpace);
    CPPUNIT_TEST(testSetDrawCallback);
    CPPUNIT_TEST_SUITE_END();
public:
        void testBorders();
        void testCharCount();
        void testCharTotalLength();
        void testClearWords();
        void testEndsWith();
        void testFirstChar();
        void testHasBigSpace();
        void testHasChars();
        void testInit();
        void testIsEndOfSentence();
        void testLeftBorder();
        void testRealLength();
        void testRightBorder();
        void testStartsWith();
        void testStartsWithDigit();
        void testStartWithDash();
        void testToString();
        void testSetDrawCallback();
};

#endif /* TESTCRTFSTRING_H_ */
