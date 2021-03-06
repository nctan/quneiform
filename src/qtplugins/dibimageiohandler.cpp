/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <QImage>
#include <QDebug>
#include <QVariant>

#include <string.h>

#include "dibimageiohandler.h"
#include "cfcompat.h"
#include "common/ctdib.h"
#include "common/dib.h"
#include "common/bmp.h"

using namespace cf;

static bool readDIBHeader(QIODevice * device, BitmapInfoHeader& info)
{
    if(!device)
        return false;

    device->reset();

    qint64 read = device->peek((char*) &info, sizeof(BitmapInfoHeader));
    if(read != sizeof(BitmapInfoHeader))
        return false;

    return true;
}

static bool isValidDIBHeader(BitmapInfoHeader& h)
{
    if(h.biSize != DIB_VERSION_3_HEADER_SIZE &&
            h.biSize != DIB_VERSION_4_HEADER_SIZE &&
            h.biSize != DIB_VERSION_5_HEADER_SIZE) {
        qWarning() << Q_FUNC_INFO << "invalid dib header size:" << h.biSize;
        return false;
    }

    if(h.biPlanes != 1)
        return false;

    if(h.biBitCount != 0 &&
            h.biBitCount != 1 &&
            h.biBitCount != 4 &&
            h.biBitCount != 8 &&
            h.biBitCount != 16 &&
            h.biBitCount != 24 &&
            h.biBitCount != 32) {
        qWarning() << Q_FUNC_INFO << "invalid dib bit count:" << h.biBitCount;
        return false;
    }

    return true;
}

DIBImageIOHandler::DIBImageIOHandler()
{}

bool DIBImageIOHandler::canRead() const
{
    BitmapInfoHeader bitmap_info;
    if(!readDIBHeader(device(), bitmap_info))
        return false;

    return isValidDIBHeader(bitmap_info);
}

bool DIBImageIOHandler::read(QImage * image)
{
    if(!device()->isOpen()) {
        qDebug() << Q_FUNC_INFO << "device not opened";
        return false;
    }

    if(!device()->isReadable()) {
        qDebug() << Q_FUNC_INFO << "device is not readable";
        return false;
    }

    BitmapInfoHeader bitmap_info;
    if(!readDIBHeader(device(), bitmap_info)) {
        qDebug() << Q_FUNC_INFO << "can't read DIB header";
        return false;
    }

    if(!isValidDIBHeader(bitmap_info)) {
        qDebug() << Q_FUNC_INFO << "invalid DIB header";
        return false;
    }

    cf::CTDIB dib;
    dib.setBitmap(&bitmap_info);
    const size_t bitmap_size = dib.dibSize();

    BitmapFileHeader bmp_header;
    bmp_header.bfSize = (uint32_t) (BMP_FILE_HEADER_SIZE + bitmap_size);
    // fileheader + infoheader + palette
    bmp_header.bfOffBits = BMP_FILE_HEADER_SIZE +
            dib.headerSize() +
            dib.palleteSize();

    char * buf = new char[bmp_header.bfSize];
    char * buf_ptr = buf;
    memcpy(buf_ptr, &bmp_header, BMP_FILE_HEADER_SIZE);
    buf_ptr += BMP_FILE_HEADER_SIZE;

    if(!device()->read(buf_ptr, bitmap_size)) {
        delete[] buf;
        qDebug() << Q_FUNC_INFO << "read error";
        return false;
    }

    bool rc = image->loadFromData((uchar*) buf, (int) bmp_header.bfSize, "BMP");
    delete[] buf;
    return rc;
}

bool DIBImageIOHandler::write(const QImage&)
{
    return false;
}

QVariant DIBImageIOHandler::option(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case Size: {
        BitmapInfoHeader dib_info;
        if(!readDIBHeader(device(), dib_info))
            return QVariant();

        return QSize(qAbs(dib_info.biWidth), qAbs(dib_info.biHeight));
    }
    default:
        return QImageIOHandler::option(option);
    }
}

bool DIBImageIOHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case Size:
        return true;
    default:
        return QImageIOHandler::supportsOption(option);
    }
}
