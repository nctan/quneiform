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

#ifndef SANESCANNER_H
#define SANESCANNER_H

#include <vector>
#include <string>

#include "globus.h"
#include "iscanner.h"
#include "common/image.h"

namespace cf {

class CLA_EXPO SaneScanner : public IScanner
{
public:
    SaneScanner();
    ~SaneScanner();

    bool close();
    DeviceList listDevices() const;
    bool open(const std::string& device);
    ImagePtr start();

    Rect scanArea() const;
    bool setScanArea(const Rect& area);
protected:
    bool setBackendOption(const std::string& name, bool value);
    bool setBackendOption(const std::string& name, int value);
    bool setBackendOption(const std::string& name, float value);
    bool setBackendOption(const std::string& name, const std::string& value);
private:
    void addOption(const void * d, int idx);
    void fillDeviceOptions();
    ImagePtr handScannerScan(int format, int width, int lineByteWidth, uint depth);
    ImagePtr normalScannerScan(int format, int width, int height, int lineByteWidth, uint depth);
    bool isOpened() const;
    bool isOptionSettable(int idx) const;
    int optionCount() const;
    int optionIndex(const std::string& name) const;
    bool readLine(uchar * buffer, size_t maxSize);
    bool setValueOption(const void * descr, int idx, ScanOptionValue * value);
private:
    void * scanner_;
};

}

#endif // SANESCANNER_H
