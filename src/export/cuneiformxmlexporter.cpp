/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "cuneiformxmlexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedcolumn.h"
#include "ced/cedframe.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/blockelement.h"
#include "ced/element.h"
#include "ced/cedarchive.h"

namespace cf {

CuneiformXmlExporter::CuneiformXmlExporter(CEDPagePtr p, const FormatOptions& opts) :
    Exporter(p, opts)
{
}

void CuneiformXmlExporter::doExport(std::ostream& os) {
    if(!page()) {
        std::cerr << "empty page given";
        std::cerr.flush();
        return;
    }

    if(formatOptions().isTestOutput())
        page()->setImageName("");

    try {
        cf::CEDXmlOutputArchive ar(os);
        CEDPage * p = page().get();
        ar << boost::serialization::make_nvp("cedpage", p);
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr.flush();
    }
}

}
