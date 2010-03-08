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

#ifndef MAGICKIMAGEEXPORTER_H_
#define MAGICKIMAGEEXPORTER_H_

#include "imageexporter.h"
#include "rdib/imageformats.h"

namespace CIF
{

class MagickImageExporter: public ImageExporter
{
    public:
        MagickImageExporter(image_format_t format = FORMAT_PNG);
        std::string mime() const;
        void save(void * data, size_t dataSize, std::ostream& os);
        FormatList supportedFormats() const;
};

}

#endif /* MAGICKIMAGEEXPORTER_H_ */