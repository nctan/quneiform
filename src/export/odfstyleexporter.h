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

#ifndef ODFSTYLEEXPORTER_H_
#define ODFSTYLEEXPORTER_H_

#include <map>
#include <boost/shared_ptr.hpp>
#include "styleexporter.h"

namespace cf
{

class OdfStyleExporter: public StyleExporter
{
    public:
        OdfStyleExporter(CEDPagePtr page, const FormatOptions& opts);
        std::string makeStyle(const CEDChar& chr);
        std::string makeStyle(const CEDParagraph& par);
        std::string makeStyle(const CEDSection& s);
        void writePageEnd(CEDPage& page);
    private:
        int fontSize2odf(int value) const;
        std::string makeStyleContent(const CEDParagraph& par, const std::string& name);
        std::string makeStyleContent(const CEDChar& chr, const std::string& name);
        std::string makeStyleContent(const CEDSection& s, const std::string& name);
        void writeTextProperties(std::ostream& os, const CEDChar& c);
    private:
        typedef std::map<std::string, std::string> StyleMap;
        StyleMap styles_;
        float font_koef_;
};

}

#endif /* ODFSTYLEEXPORTER_H_ */
