/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef TESTLOCALRECOGNITIONSERVER_H
#define TESTLOCALRECOGNITIONSERVER_H

#include <cppunit/extensions/HelperMacros.h>

class TestLocalRecognitionServer : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLocalRecognitionServer);
    CPPUNIT_TEST(testRecognizeImage);
    CPPUNIT_TEST(testRecognizeRotated);
    CPPUNIT_TEST(testRecognizeArea);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testBinarize);
    CPPUNIT_TEST(testAnalyze);
    CPPUNIT_TEST(testRecognize);
    CPPUNIT_TEST(testFormat);
    CPPUNIT_TEST_SUITE_END();
public:
    void testRecognizeImage();
    void testRecognizeRotated();
    void testRecognizeArea();
    void testOpen();
    void testBinarize();
    void testAnalyze();
    void testRecognize();
    void testFormat();
};

#endif // TESTLOCALRECOGNITIONSERVER_H
