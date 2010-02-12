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

#ifndef GENERICEXPORTER_H_
#define GENERICEXPORTER_H_

#include "exporter.h"

class CEDPage;
class CEDSection;
class CEDParagraph;
class CEDLine;
class CEDChar;

namespace CIF
{

class GenericExporter: public Exporter
{
    public:
        GenericExporter(CEDPage * page, const FormatOptions& opts);
    private:
        void doExport(std::ostream& os);
        void exportChar(CEDChar * chr);
        void exportColumn(CEDParagraph * col);
        void exportFrame(CEDParagraph * frame);
        void exportLine(CEDLine * line);
        void exportPage();
        void exportParagraph(CEDParagraph* par);
        void exportPicture(CEDChar * picture);
        void exportSection(CEDSection * sect);
        void exportTable(CEDParagraph * table);
    private:
        virtual void writeFooter(std::ostream& os);
        virtual void writeHeader(std::ostream& os);
        virtual void writeParagraph(std::ostream& os, CEDParagraph * par);
        virtual void writeSection(std::ostream& os, CEDSection * sect);
    private:
        CEDPage * page_;
        bool first_paragraph_;
        std::ostream * os_;
        int num_chars_;
        int num_columns_;
        int num_frames_;
        int num_lines_;
        int num_paragraphs_;
        int num_pictures_;
        int num_sections_;
};

}

#endif /* GENERICEXPORTER_H_ */
