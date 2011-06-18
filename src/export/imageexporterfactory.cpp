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

#include "imageexporterfactory.h"
#include "config-user.h"
#include "bmpimageexporter.h"

#if defined(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) || defined(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK)
#include "magickimageexporter.h"
#endif

namespace cf
{

ImageExporterPtr ImageExporterFactoryImpl::make() {
#if defined(CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK) || defined(CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK)
    return ImageExporterPtr(new MagickImageExporter);
#else
    return ImageExporterPtr(new BmpImageExporter);
#endif
}

}
