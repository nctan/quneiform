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
#include "testbmp.h"
#define private public
#include <rdib/bmpimageloader.h>
#include <rdib/bmp.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestBmpLoader);
using namespace CIF;

void TestBmpLoader::testInit() {
    std::auto_ptr<ImageLoader> loader(new BmpImageLoader);
    ImagePtr image = loader->load(LOADER_TEST_IMAGE_DIR + std::string("test.bmp"));
    CPPUNIT_ASSERT_THROW(loader->load(LOADER_TEST_IMAGE_DIR + std::string("test_not_exists.bmp")), ImageLoader::Exception);
    //    test empty stream
    std::stringstream is;
    CPPUNIT_ASSERT_THROW(loader->load(is), CIF::ImageLoader::Exception);
}

void TestBmpLoader::testMagick() {
    std::auto_ptr<BmpImageLoader> loader(new BmpImageLoader);
    {
        std::stringstream is;
        // test empty
        CPPUNIT_ASSERT_THROW(loader->readBmpMagick(is), CIF::ImageLoader::Exception);
        // test wrong magick
        is.clear();
        is << "GB";
        CPPUNIT_ASSERT_THROW(loader->readBmpMagick(is), CIF::ImageLoader::Exception);
    }

    // test valid magick
    {
        std::stringstream is;
        is << "BM";
        CPPUNIT_ASSERT_NO_THROW(loader->readBmpMagick(is));
    }

    // too short magick
    {
        std::stringstream is;
        is << "B";
        CPPUNIT_ASSERT_THROW(loader->readBmpMagick(is), CIF::ImageLoader::Exception);
    }
}

void TestBmpLoader::testReadBmpHeader() {
    std::auto_ptr<BmpImageLoader> loader(new BmpImageLoader);
    // test empty
    {
        std::stringstream is;
        CPPUNIT_ASSERT_THROW(loader->readBmpFileHeader(is), CIF::ImageLoader::Exception);
    }

    // test offsetbits
    {
        std::stringstream is;
        char data[50] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        is.write(data, 10);
        const uint32_t number = 0x87654321;
        is.write((char*) &number, sizeof(number));
        is.write(data, 10);
        CPPUNIT_ASSERT_NO_THROW(loader->readBmpFileHeader(is));
        CPPUNIT_ASSERT(loader->file_header_.iSize == 20 + sizeof(int32_t));
        CPPUNIT_ASSERT(loader->file_header_.iOffBits == number);
    }
}

void TestBmpLoader::testReadBmpInfoHeader() {
    std::auto_ptr<BmpImageLoader> loader(new BmpImageLoader);
    // test empty
    {
        std::stringstream is;
        CPPUNIT_ASSERT_THROW(loader->readBmpInfoHeader(is), CIF::ImageLoader::Exception);
    }

    // test wrong BMP version
    {
        std::stringstream is;
        char data[14] = { 0 };
        is.write(data, 14);
        uint32_t sz = BIH_WIN4SIZE + 1;
        is.write((char*) &sz, sizeof(sz));
        CPPUNIT_ASSERT_THROW(loader->readBmpInfoHeader(is), CIF::ImageLoader::Exception);
    }

    {
        std::stringstream is;
        char data[14] = { 0 };
        is.write(data, 14);
        uint32_t sz = BIH_WIN4SIZE;
        is.write((char*) &sz, sizeof(sz));
        CPPUNIT_ASSERT_THROW(loader->readBmpInfoHeader(is), ImageLoader::Exception);
    }

    // test version 3
    {
        std::stringstream is;
        char data[100] = { 0 };
        is.write(data, BFH_SIZE);
        const uint32_t sz = BIH_WIN4SIZE;
        is.write((char*) &sz, sizeof(sz));
        is.write(data, 100);
        CPPUNIT_ASSERT_NO_THROW(loader->readBmpInfoHeader(is));
        CPPUNIT_ASSERT_EQUAL(loader->bmp_type, BMPT_WIN4);
    }

    // test version 4
    {
        std::stringstream is;
        char data[100] = { 0 };
        is.write(data, BFH_SIZE);
        const uint32_t sz = BIH_VER4SIZE;
        is.write((char*) &sz, sizeof(sz));
        is.write(data, 100);
        CPPUNIT_ASSERT_NO_THROW(loader->readBmpInfoHeader(is));
        CPPUNIT_ASSERT_EQUAL(loader->bmp_type, BMPT_WIN5);
    }

    // test version 5
    {
        std::stringstream is;
        char data[100] = { 0 };
        is.write(data, BFH_SIZE);
        const uint32_t sz = BIH_VER5SIZE;
        is.write((char*) &sz, sizeof(sz));
        is.write(data, 100);
        CPPUNIT_ASSERT_NO_THROW(loader->readBmpInfoHeader(is));
        CPPUNIT_ASSERT_EQUAL(loader->bmp_type, BMPT_WIN5);
    }
}

