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

#include <Magick++.h>
#include <cstring>
#include <boost/scoped_array.hpp>

#include "magickimageloader.h"
#include "imageloaderfactory.h"
#include "common/cifconfig.h"

namespace
{

CIF::ImageLoader * create() {
    return new CIF::MagickImageLoader;
}

bool unkwnown = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_UNKNOWN, 100,
        create);
bool bmp = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_BMP, 100, create);
bool gif = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_GIF, 100, create);
bool jpg = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_JPEG, 100, create);
bool pnm = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_PNM, 100, create);
bool png = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_PNG, 100, create);
bool tif = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_TIFF, 100, create);
bool xpm = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_XPM, 100, create);
}

namespace CIF
{

const int MIN_DPI_FOR_VECTOR_FORMAT = 300;

MagickImageLoader::MagickImageLoader() :
    dpi_(0) {
}

void MagickImageLoader::convertColorSpace(Magick::Image& image) {
    switch (image.type()) {
    case Magick::BilevelType:
    case Magick::TrueColorType:
        break;
    default:
        image.type(Magick::TrueColorType);
    }
}

void MagickImageLoader::convertImageToDib(Magick::Image& image, Magick::Blob& blob) {
    image.magick("DIB");
    image.write(&blob);
}

void MagickImageLoader::convertImageDpi(Magick::Image& image) {
    if (image.magick() == "PDF" or image.magick() == "SVG" or image.magick() == "DJVU") {
        //change from default 72 dpi
        image.density(Magick::Geometry(MIN_DPI_FOR_VECTOR_FORMAT, MIN_DPI_FOR_VECTOR_FORMAT));
    }
}

ImagePtr MagickImageLoader::load(Magick::Image& image) {
    Magick::Blob blob;
    convertColorSpace(image);
    convertImageToDib(image, blob);
    ImagePtr res = makeImage(blob);
    res->setSize(Size(image.size().width(), image.size().height()));
    return res;
}

ImagePtr MagickImageLoader::load(std::istream& stream) {
    if (stream.fail())
        throw Exception("[MagickImageLoader::load] invalid stream given");

    long int stream_size = streamSize(stream);
    if (stream_size < 1)
        throw Exception("[MagickImageLoader::load] empty stream given");

    boost::scoped_array<char> tmp(new char[stream_size]);
    Magick::Blob blob;
    blob.updateNoCopy(tmp.get(), stream_size);
    try {
        Magick::Image image;
        image.density("10");
        image.ping(blob);
        convertImageDpi(image);
        image.read(blob);

        return load(image);
    } catch (Magick::Exception &e) {
        std::cerr << e.what() << "\n";
        throw Exception("[MagickImageLoader::load] failed");
    }
}

ImagePtr MagickImageLoader::load(const std::string& fname) {
    try {
        Magick::Image image;
        image.density("10");
        image.ping(fname);
        convertImageDpi(image);
        image.read(fname);
        return load(image);
    } catch (Magick::Exception &e) {
        std::cerr << e.what() << "\n";
        throw Exception("[MagickImageLoader::load] faild");
    }
}

ImagePtr MagickImageLoader::makeImage(Magick::Blob& blob) {
    uchar * new_data = new uchar[blob.length()];
    memcpy(new_data, blob.data(), blob.length());
    return ImagePtr(new Image(new_data, blob.length(), Image::AllocatorNew));
}

}

