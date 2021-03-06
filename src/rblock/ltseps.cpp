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

/****************************************************************************
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTBLOCKS.C - Blocks forming                                 *
 *                                                                          *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "tech.h"
#include "cpage/cpage.h"
#include "layout.h"
#include "my_mem.h"
#include "cline/cline.h"

#include "dpuma.h"
#include "minmax.h"

#ifndef LT_STAND_ALONE
#include "struct.h"
extern FRAME frames[];
extern int16_t nf;
extern STRLN lines[];
extern int16_t nl;
# endif

SEPARATOR *pSeps = NULL;
int nSeps = 0;

# ifdef LT_STAND_ALONE
Bool32 DeleteSeps (int n)
{
    int32_t i;

    for (i = n; i < nSeps - 1; i++) {
        pSeps[i] = pSeps[i+1];
    }

    nSeps--;
    return TRUE;
}

void SeparatorsGet (void)
{
#define ABS1 10
#define ABS2 40
    PAGEINFO pInfo;
    Handle pPage;
    uint32_t ResolutionCoeff;
    uint32_t i, j;
    Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));
    Handle hBlock;
    uint32_t key;
    uint32_t color;
    int32_t nPics;
    POLY_ *pPics;
    uint32_t size_line_com = sizeof(LINE_COM);
    CLINE_handle hline;
    extern CLINE_handle HCLINE;
    pPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));
    CPAGE_GetPageData( pPage, PT_PAGEINFO, (void*)&pInfo, sizeof(pInfo));
    ResolutionCoeff = pInfo.DPIY / 2;
    SeparatorsFreeData ();
    hline = CLINE_GetFirstLine(HCLINE);

    if (!hline)
    return;

    while (hline) {
        CPDLine cpdata = CLINE_GetLineData(hline);

        if (!cpdata)
        hline = CLINE_GetNextLine(hline);

        else {
            nSeps++;
            pSeps = static_cast<SEPARATOR*>(realloc (pSeps, nSeps * sizeof(SEPARATOR)));
            pSeps [nSeps-1].xBegin = cpdata->Line.Beg_X;
            pSeps [nSeps-1].yBegin = cpdata->Line.Beg_Y;
            pSeps [nSeps-1].xEnd = cpdata->Line.End_X;
            pSeps [nSeps-1].yEnd = cpdata->Line.End_Y;
            pSeps [nSeps-1].nWidth = cpdata->Line.Wid10 / 10;
            pSeps [nSeps-1].Type = SEP_NULL;
            pSeps [nSeps-1].uFlags = 0;

            if (cpdata->Dir == LD_Horiz)
            pSeps [nSeps-1].Type = SEP_HORZ;

            else
            pSeps [nSeps-1].Type = SEP_VERT;

            hline = CLINE_GetNextLine(hline);
        }
    }

    color = 200;
    key = 111;

    /* Deleting short separators */
    for (i = 0; i < nSeps; i++) {
        if (pSeps [i].Type == SEP_VERT) {
            if (pSeps[i].yEnd - pSeps[i].yBegin < ResolutionCoeff) {
                DeleteSeps(i);
                i--;
            }
        }

        else {
            if (pSeps[i].xEnd - pSeps[i].xBegin < ResolutionCoeff) {
                DeleteSeps(i);
                i--;
            }
        }
    }

    /* Deleting separators from pictures */
    nPics = 0;
    pPics = NULL;
# define PICS_QUANTUM           128

    for (hBlock = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE);
            hBlock != NULL;
            hBlock = CPAGE_GetBlockNext(hPage, hBlock, TYPE_IMAGE)) {
        if (nPics % PICS_QUANTUM == 0) {
            pPics = static_cast<POLY_*>(realloc (pPics,
                            (size_t) ((nPics / PICS_QUANTUM + 1)
                                    * PICS_QUANTUM * sizeof (POLY_))));
        }

        CPAGE_GetBlockData(hPage, hBlock, TYPE_IMAGE, &pPics[nPics++], sizeof(POLY_));
    }

    for (i = 0; i < nPics; i++) {
        for (j = 0; j < nSeps; j++) {
            if ( (pSeps[j].xBegin > pPics[i].com.Vertex[0].x() - 10) &&
                    (pSeps[j].yBegin > pPics[i].com.Vertex[0].y() - 10) &&
                    (pSeps[j].xEnd < pPics[i].com.Vertex[1].x() + 10) &&
                    (pSeps[j].yEnd < pPics[i].com.Vertex[2].y() + 10)) {
                DeleteSeps(j);
                j--;
            }
        }
    }

    if (pPics != NULL) {
        free (pPics);
        pPics = NULL;
    }

    nPics = 0;

    /* Удаление близколежащих сепараторов */
    for (i = 0; i < nSeps; i++) {
        for (j = 0; j < nSeps; j++) {
            if (pSeps [i].Type == SEP_VERT && pSeps [j].Type == SEP_VERT) {
                if ((uint32_t)(abs(pSeps[i].xBegin - pSeps[j].xEnd ) < ResolutionCoeff / 2) &&
                        (uint32_t)(abs(pSeps[i].xEnd - pSeps[j].xBegin) < ResolutionCoeff / 2) &&
                        (pSeps[i].yBegin < pSeps[j].yBegin) &&
                        (pSeps[i].yEnd > pSeps[j].yEnd)) {
                    DeleteSeps(j);
                    j--;
                }
            }
        }
    }

    /* Объединение сепараторов */
    for (i = 0; i < nSeps; i++) {
        for (j = 0; j < nSeps; j++) {
            if (pSeps [i].Type == SEP_VERT && pSeps [j].Type == SEP_VERT) {
                if ((abs(pSeps[i].xBegin - pSeps[j].xEnd) < ABS1) &&
                        (pSeps[i].yBegin > pSeps[j].yEnd) &&
                        (uint32_t)(pSeps[i].yBegin - pSeps[j].yEnd) < ResolutionCoeff / 2) {
                    pSeps[i].xBegin = MIN(pSeps[i].xBegin, pSeps[j].xBegin);
                    pSeps[i].xEnd = MAX(pSeps[i].xEnd, pSeps[j].xEnd);
                    pSeps[i].yBegin = pSeps[j].yBegin;
                    DeleteSeps(j);
                    j--;
                }
            }

            if (pSeps [i].Type == SEP_HORZ && pSeps [j].Type == SEP_HORZ) {
                if ((abs(pSeps[i].yBegin - pSeps[j].yEnd) < ABS1) &&
                        (pSeps[i].xBegin > pSeps[j].xEnd) &&
                        (pSeps[i].xBegin - pSeps[j].xEnd) < ABS2) {
                    pSeps[i].yBegin = MIN(pSeps[i].yBegin, pSeps[j].yBegin);
                    pSeps[i].yEnd = MAX(pSeps[i].yEnd, pSeps[j].yEnd);
                    pSeps[i].xBegin = pSeps[j].xBegin;
                    DeleteSeps(j);
                    j--;
                }
            }
        }
    }

    /*
     for(i = 0; i < nSeps; i++)
     {
     color = 200;
     key = 111;
     p_start.x = pSeps[i].xBegin;
     p_start.y = pSeps[i].yBegin;
     p_end.x   = pSeps[i].xEnd;
     p_end.y   = pSeps[i].yEnd;

     LDPUMA_DrawLine(NULL, &p_start, &p_end, 0, color, 4, key);
     }
     */
    //LDPUMA_WaitUserInput(NULL, NULL);
}

# else
static int SepComp(const SEPARATOR *p, const SEPARATOR *q) //AK 04.03.97
{
    return (MAX(abs(q -> xEnd - q -> xBegin), abs(q -> yEnd - q -> yBegin))
            - MAX(abs(p -> xEnd - p -> xBegin), abs(p -> yEnd - p -> yBegin)));
}

void SeparatorsGet(void)
{
    int j = 0;
    SeparatorsFreeData();

    if (nl == 0)
        return;

    nSeps = nf + nl;
    pSeps = (SEPARATOR*) malloc(nSeps * sizeof(SEPARATOR));

    if (pSeps == NULL)
        ErrorNoEnoughMemory("in LTSEPS.C,SeparatorsGet,part 1");

    for (int i = 0, j = 0; i < nf; i++, j++) {
        pSeps[j].Type = SEP_RECT;
        pSeps[j].uFlags = SEPF_NULL;
        pSeps[j].xBegin = frames[i].topleft.x();
        pSeps[j].yBegin = frames[i].topleft.y();
        pSeps[j].xEnd = frames[i].botright.x();
        pSeps[j].yEnd = frames[i].botright.y();
        pSeps[j].nWidth = 1;
    }

    for (int i = 0; i < nl; i++, j++) {
        if (lines[i].type & UNDRLN)
            pSeps[j].Type = SEP_NULL;

        else if (lines[i].type & VERT_LN)
            pSeps[j].Type = SEP_VERT;

        else if (lines[i].type & HOR_LN)
            pSeps[j].Type = SEP_HORZ;

        else
            pSeps[j].Type = SEP_NULL;

        pSeps[j].uFlags = lines[i].type & FRM_LN ? SEPF_IS_PART : SEPF_NULL;
        pSeps[j].xBegin = lines[i].beg.x();
        pSeps[j].yBegin = lines[i].beg.y();
        pSeps[j].xEnd = lines[i].end.x();
        pSeps[j].yEnd = lines[i].end.y();
        pSeps[j].nWidth = lines[i].width;
    }

    qsort((char *) pSeps, nSeps, sizeof(SEPARATOR), (int(*)(const void*, const void*)) SepComp); //AK 04.03.97
}
# endif

void BlocksAddVirtualSeparatorsBlocks(void)
{
    int BlockType = 0;

    for (int i = 0; i < nSeps; i++) {
        if (pSeps[i].uFlags & SEPF_IS_PART)
            continue;

        switch (pSeps[i].Type) {
        case SEP_HORZ:
            BlockType = BLOCK_HORZ_SEPARATOR;
            break;
        case SEP_VERT:
            BlockType = BLOCK_VERT_SEPARATOR;
            break;
        case SEP_RECT:
            BlockType = BLOCK_RECT_SEPARATOR;
            break;
        default:
            continue;
        }

        BLOCK * p = BlocksAddDescriptor();
        p->nNumber = ++nNextBlockNumber;
        p->Type = BlockType;
        p->Rect.xLeft = pSeps[i].xBegin;
        p->Rect.yTop = pSeps[i].yBegin;
        p->Rect.xRight = pSeps[i].xEnd;
        p->Rect.yBottom = pSeps[i].yEnd;
    }
}

void SeparatorsFreeData(void)
{
    if (pSeps != NULL)
        free(pSeps);

    pSeps = NULL;
    nSeps = 0;
}
