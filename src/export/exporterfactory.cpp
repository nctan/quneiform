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

#include "exporterfactory.h"
#include "htmlexporter.h"
#include "debugexporter.h"
#include "rtfexporter.h"
#include "edexporter.h"
#include "textexporter.h"
#include "hocrexporter.h"
#include "puma/pumadef.h"
#include "ced/cedpage.h"
#include "common/outputformat.h"
#include "config-user.h" // for CF_USE_ODF

#ifdef CF_USE_ODF
#include "odfexporter.h"
#endif

namespace CIF
{

ExporterFactoryImpl::ExporterFactoryImpl() :
    page_(NULL) {
}

void ExporterFactoryImpl::setFormatOptions(const FormatOptions& opts) {
    format_options_ = opts;
}

void ExporterFactoryImpl::setPage(CEDPage * page) {
    page_ = page;
}

ExporterPtr ExporterFactoryImpl::make(format_t format) {
    ExporterPtr exp;
    switch (format) {
    case FORMAT_DEBUG:
        exp.reset(new DebugExporter(format_options_));
        break;
    case FORMAT_RTF:
        exp.reset(new RtfExporter(page_));
        break;
    case FORMAT_EDNATIVE:
        exp.reset(new EdExporter(page_));
        break;
    case FORMAT_HOCR:
        exp.reset(new HocrExporter(page_, format_options_));
        break;
    case FORMAT_HTML:
        exp.reset(new HtmlExporter(page_, format_options_));
    case FORMAT_SMARTTEXT:
        format_options_.setPreserveLineBreaks(true);
    case FORMAT_TEXT:
        exp.reset(new TextExporter(page_, format_options_));
        break;
#ifdef CF_USE_ODF
        case FORMAT_ODF:
        exp.reset(new OdfExporter(page_, format_options_));
        break;
#endif
    default:
        throw Exception("[ExporterFactoryImpl::make] Unsupported export format: "
                + OutputFormat::name(format), format);
    }
    return exp;
}

}
