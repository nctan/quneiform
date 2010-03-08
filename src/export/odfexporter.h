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

#ifndef ODTEXPORTER_H_
#define ODTEXPORTER_H_

#include <zip.h>
#include <vector>
#include <string>
#include <map>

#include "xmlexporter.h"

namespace CIF
{

class Tag;

class OdfExporter: public XmlExporter
{
    public:
        OdfExporter(CEDPage * page, const FormatOptions& opts);
        ~OdfExporter();

        void exportTo(const std::string& fname);
        void exportTo(std::ostream& os);
    protected:
        std::string fontStyleTag(int style) const;
        void writeFontStyleBegin(std::ostream& os, int style);
        void writeFontStyleEnd(std::ostream& os, int style);
        void writeLineBreak(std::ostream& os, CEDLine * line);
        void writeMeta(std::ostream& os);
        void writeMetaDate(std::ostream& os);
        void writeMetaStatistics(std::ostream& os);
        void writePageBegin(std::ostream& os, CEDPage * page);
        void writePageEnd(std::ostream& os, CEDPage * page);
        void writeParagraphBegin(std::ostream& os, CEDParagraph * par);
        void writeParagraphEnd(std::ostream& os, CEDParagraph * par);
        void writePicture(std::ostream& os, CEDChar * picture);
    private:
        void addOdfAutomaticStyles(std::ostream& os);
        void addOdfContent();
        void addOdfManifest();
        void addOdfManifestFile(const std::string& path, const std::string& type);
        void addOdfMeta();
        void addOdfMime();
        void addOdfSettings();
        void addOdfStyles();
        void odfClose();
        void odfOpen(const std::string& fname);
        void odfWrite(const std::string& fname, const std::string& data);
        void setCommonOdfNamespaces(Tag& tag) const;
    private:
        zip * zip_;
        typedef std::vector<std::string> BufList;
        typedef std::map<std::string, std::string> ManifestList;
        BufList buffers_;
        ManifestList files_;
};

}

#endif /* ODTEXPORTER_H_ */