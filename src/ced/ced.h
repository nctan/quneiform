/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Cognitive Technologies nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __CED_H
#define __CED_H

#include "globus.h"
#include "lang_def.h"
#include "common/size.h"
#include "common/rect.h"
#include "common/letter.h"
#include "common/color.h"
#include "common/align.h"

#ifdef __CED__
#define CED_FUNC  FUN_EXPO
#else
#define CED_FUNC  FUN_IMPO
#endif

#define FUN_IMPO_VOID  FUN_IMPO__  __FUN_IMPO
#define FUN_EXPO_VOID  FUN_EXPO__  __FUN_EXPO

#pragma pack (push,8)

#include "edfile.h"

typedef struct edBox
{
        int x, w, y, h;
} EDBOX;

typedef struct edSize
{
        int32_t cx, cy;
} EDSIZE;

typedef struct edCol
{
        int32_t width, space;
} EDCOL;

#ifndef _WINGDI_

#define DEFAULT_PITCH           0
#define FIXED_PITCH             1
#define VARIABLE_PITCH          2
#define MONO_FONT               8

/* Font Families */
enum ced_font_t
{
    /* Don't care or don't know. */
    FF_DONTCARE = (0 << 4),
    /* Variable stroke width, serifed. */
    /* Times Roman, Century Schoolbook, etc. */
    FF_ROMAN = (1 << 4),
    /* Variable stroke width, sans-serifed. */
    /* Helvetica, Swiss, etc. */
    FF_SWISS = (2 << 4),
    /* Constant stroke width, serifed or sans-serifed. */
    /* Pica, Elite, Courier, etc. */
    FF_MODERN = (3 << 4),
    /* Cursive, etc. */
    FF_SCRIPT = (4 << 4),
    /* Old English, etc. */
    FF_DECORATIVE = (5 << 4)
};
#endif

//Для CreateFrame
#define HPOS_PAGE       (0<<4)
#define HPOS_MRG        (1<<4)
#define HPOS_COL        (2<<4)

#define VPOS_PAGE       0
#define VPOS_MRG        1
#define VPOS_PARA       2

//Для разрыва раздела
#define ED_SECT_NEW_PAGE       1
#define ED_SECT_CONTINUE       0
/* orientation selections */
#define ED_ORIENT_PORTRAIT   0
#define ED_ORIENT_LANDSCAPE  1

//Положение фрейма
#define ED_POSXR    (0x7fffffff-1)

//Стили символов
enum ed_char_style_t
{
    ED_ULINED = 1,
    ED_HIDDEN = 2,
    ED_BOLD = 4,
    ED_ITALIC = 16,
    ED_PROTECT = 32,
    ED_ULINE = 64,
    ED_STRIKE = 128,
    ED_SUPSCR = 256,
    ED_SUBSCR = 512,
    ED_FNOTE = 1024

};
//для абзацев
#define ED_PARA_KEEP    1
#define ED_PARA_KEEP_NEXT   3
//- Positions the paragraph to the right within the reference frame.
#define ED_POSXC    (0x7fffffff-2)
//- Centers the frame horizontally within the reference frame.
//Слияние ячеек
#define ED_CELL_MRGF    1
// - ячейка первая в объединяемой группе
#define ED_CELL_MRG     2
//- ячейка очередная в объедияемой группе, т.е.
#define ED_DROPCAP      1
// - Фрейм является буквицей
#define ED_TDIR_UP      2
#define ED_TDIR_DOWN    4
// - направление текста в ячейке таблицы или фрейме

//Типы границ ячейки, абзаца и др.
enum ed_border_t
{
    ED_BRDR_NONE = 0,
    ED_BRDR_SINGLE,
    ED_BRDR_SHADOWED,
    ED_BRDR_DOTTED,
    ED_BRDR_DASHED,
    ED_BRDR_DOUBLE
};

typedef void (*FNRDProc)(uchar* ptr, uint32_t lth);

//Для CreatePicture
enum ed_align_t
{
    ED_ALIGN_BOT = 0, // align picture bottom to base line
    ED_ALIGN_MIDDLE = 1, // center picture vertically to baseline
    ED_ALIGN_TOP = 2
// align picture top to base line
};

namespace CIF
{
class CEDPage;
class CEDLine;
class CEDParagraph;
class CEDSection;
class CEDColumn;
class BlockElement;
class CEDFrame;
}
CED_FUNC(CIF::CEDSection*) CED_CreateSection(CIF::CEDPage* page, const CIF::Rect& border,
        int colInterval, int numOfCols, EDCOL* colInfo, char sectionBreak, int width, int height,
        char orientation, int headerY, int footerY);
CED_FUNC(CIF::CEDParagraph*) CED_CreateParagraph(CIF::CEDSection * sect, CIF::BlockElement * cont,
        CIF::align_t align, const CIF::Rect& indent, int UserNum, int FlagBorder, EDSIZE interval,
        const CIF::Rect& layout, const CIF::Color& color, const CIF::Color& backgr,
        int spaceBetweenLines, char spcBtwLnsMult, char keep);

#pragma pack (pop)

#endif
