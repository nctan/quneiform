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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "cedsection.h"
#include "cedcolumn.h"
#include "cedparagraph.h"

namespace CIF
{

CEDSection::CEDSection() {
    sectionBreak = 0;
    width = 0;
    height = 0;
    orientation = 0;
    headerY = 0;
    footerY = 0;
    numSnakeCols = 0;
    lineBetCol = 0;
    colInfo = 0;
    columnsEnd = 0;
}

CEDSection::~CEDSection() {
    if (colInfo)
        delete[] colInfo;
}

void CEDSection::addColumn(CEDColumn * col) {
    addElement(col);
}

CEDColumn * CEDSection::columnAt(size_t pos) {
    return static_cast<CEDColumn*> (elementAt(pos));
}

size_t CEDSection::columnCount() const {
    return elementCount();
}

CEDColumn * CEDSection::createColumn() {
    CEDColumn * col = new CEDColumn;
    addElement(col);
    return col;
}

CEDParagraph * CEDSection::CreateFrame(CEDParagraph* hObject, edBox rect, char position,
        int borderSpace, int dxfrtextx, int dxfrtexty) {
    if (hObject->type != COLUMN_BEGIN) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt of frame creation outside of table";
#endif
        return 0;
    }

    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);
#ifdef _DEBUG

    if (colde == 0) {
        std::cerr << "CED error: Attempt of frame creation in ordinary paragraph\n(not in column)";
        return 0;
    }

#endif

    //start frame
    CEDParagraph * para = new CEDParagraph;
    para->type = FRAME_BEGIN;
    edFrameDescr* framinf = (EDFRAMEDESCR *) malloc(sizeof(EDFRAMEDESCR));
    framinf->rec.x = rect.x;
    framinf->rec.y = rect.y;
    framinf->rec.w = rect.w;
    framinf->rec.h = rect.h;
    framinf->position = position;
    framinf->borderSpace = borderSpace;
    framinf->dxfrtextx = dxfrtextx;
    framinf->dxfrtexty = dxfrtexty;
    framinf->flag = 0;
    para->descriptor = framinf;
    columnAt(0)->addElement(para);
    //finish frame
    CEDParagraph * para1 = new CEDParagraph;
    columnAt(0)->addElement(para1);
    para1->type = FRAME_END;
    ((EDFRAMEDESCR *) (para->descriptor))->last = para1;
    return para;
}

CEDParagraph * CEDSection::CreateParagraph(CEDParagraph * hObject, align_t align,
        const Rect& indent, int UserNum, int FlagBorder, EDSIZE interval, edBox layout,
        const Color& color, const Color& shading, int spaceBetweenLines, char spcBtwLnsMult,
        char keep) {
    CEDParagraph * para = new CEDParagraph;
    columnAt(0)->addElement(para);
    para->setAlign(align);
    para->setIndent(indent);
    para->userNumber = UserNum;
    para->border = FlagBorder;
    para->setColor(color);
    para->setBackgroundColor(shading);
    para->setLineSpace(spaceBetweenLines);
    para->spcBtwLnsMult = spcBtwLnsMult;
    para->keep = keep;
    para->interval = interval;
    para->layout = layout;
    return para;
}

}
