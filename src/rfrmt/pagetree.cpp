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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 18.10.98
//
// PageTree.cpp
//
// ============================================================================

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <algorithm>

#include "aldebug.h"
#include "creatertf.h"
#include "crtffragment.h"
#include "crtfsector.h"
#include "crtfstring.h"
#include "crtfpage.h"
#include "crtfchar.h"
#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "crtfword.h"
#include "crtffunc.h"
#include "formatdebug.h"

#include "cpage/cpage.h"
#include "cpage/cpagetyps.h"
#include "common/debug.h"

#include "lst3_win.h"
#include "ful_txt.h"
#include "consmess.h"

#include "minmax.h"

#ifdef alDebug
short FlagGraphic1 = 0, Graphic1Color = 0;
std::vector<RECT> pTheGeomStep;
std::vector<RECT> pTheGeomStep1;
std::vector<RECT> pTheGeomStep2;
std::vector<RECT> pTheGeomTemp;
VectorWord pFragRectColor;

void MyDrawForDebug(void) {
}
uint16_t CountRect;
#else

#endif

typedef void(*FMyDraw)(void);
typedef struct tagSETUP_GENERATE_TREE
{
        int16_t size_x, size_y; //standard sizes of symbols in page
} SETUP_GENERATE_TREE;

int16_t FlagOdinSectorOdnaColonka = FALSE;
int16_t MaxLev, NumColTrue, RegimExt;
Rect16 *RectFragm;
FRAME *ArrFrm;

extern SUB_ALLOC SubZn;

int SizeYGlob;
uint16_t ScanResolution;
float MulScanRes;//Разрешения сканера и нормир.множитель

/*
 util     - memory alloc
 util_lst - lists manipulation , etc.
 sort_int - fast sorting
 util_spl:
 OpenFullOutTiger  - RTF
 CalcStatTiger     - статистика об интервалах (внутри- и меж- словные и т.п.)
 */
Bool PageTree(FILE *InFileName, cf::CRtfPage* RtfPage);
short OpenFullOutTiger(FILE *FileName);
Bool Alik_sort_function(const void *a, const void *b);
int CalcStatTiger(void);
int GenerateTreeByFragm(Rect16 *RectFragm, int16_t NumFragm, SETUP_GENERATE_TREE *setup,
        FRAME ***Frm1, INF_TREE *Inf);

/**
 * Создать дерево стр-ры Text Plain
 *
 * @param BndTxt - page boundaries
 * @param LineV - вертикальные линии найденные на листе
 * @param NumLV - число вертикальных линий найденных на листе
 * @param LineH - горизонтальные линии найденные на листе
 * @param NumLH - число горизонтальных линий найденных на листе
 * @param frm - рамки букв (фрагментов в CunieForm) листа
 * @param NumFrm - число рамок букв листа
 * @param Inf - результирующая структура хранения дерева колонок листа
 * @param size_x - стандарт. размер букв листа
 * @param size_y - стандарт. размер букв листа
 * @return 0 - OK
 */
int CreateTreePlainTxt1(BOUND BndTxt, STRET *LineV, int16_t NumLV, STRET *LineH, int16_t NumLH,
        FRAME **frm, int16_t NumFrm, INF_TREE *Inf, int16_t size_x, int16_t size_y);
int AddLine1(LINE_KNOT **Line1, int16_t *nCurr, int16_t *nMax, int16_t Coor, int16_t Thres);
int16_t SearchColHist1(FRAME **frm, int16_t k_frm, BOUND *bnd, int16_t ave_x, int16_t ave_y,
        int16_t reg, int16_t *k_int, int16_t **intr1, int16_t **begI, int16_t **endI,
        int16_t *NumMax);
int16_t
SearchInterval1(FRAME **frm, int16_t k_frm, int16_t **beg1, int16_t **end1, int16_t *k_int1,
        BOUND *bnd, int16_t ave_dir, int16_t ave_ort, int16_t reg, int16_t *NumMax);
KNOTT* IncKnot(KNOTT *up, KNOTT *after, KNOTT **free);
void FillFieldKNOTT1(KNOTT *ptr, int16_t Left, int16_t Right, int16_t Top, int16_t Bottom,
        int16_t InBegFrm, int16_t NumFrm, int16_t InColA, uint OrderChild, uint Type,
        uint AllowOCR, uint JustH, uint JustV, char *Name);
int16_t SortHorLine1(LINE_KNOT *LineHK, int16_t NumH, LINE_KNOT *LineVK, int16_t NumV, KNOTT *Root,
        KNOTT ***colt1, int16_t *k_colt1, FRAME **frm);
int16_t Check_IsItFalseHorLine(int16_t recalc, int16_t reg, FRAME **frm, int16_t *his, int16_t pos,
        int16_t len, int16_t maxh, int16_t sum, int16_t len_group, int16_t *his_first_group,
        int16_t *his_second_group, BOUND *bnd, int16_t k_frm);
int check_white_int(int16_t beg_white_int, int16_t end_white_int, int16_t maxh,
        int16_t *his_second_group);
void Get_all_term_fragms(KNOTT *ptr, int16_t *Colt, int16_t *iv, int16_t NumCol, FRAME **frm);
void Get_all_term_fragms1(KNOTT* ptr, int16_t* Colt, int16_t* iv, int16_t NumCol, FRAME **frm);
cf::Rect toRect(const SRECT& src);
cf::Rect toRect(const tagRECT& src);
void RtfUnionRect_CRect_SRect(tagRECT *s1, SRECT *s2);

////////////// functions, which are moved from other modules //////////////

int ConsMess(const char* str, ...) {
    if (str == NULL)
        return 0;

    char strbuf[4096]; // string to be put
    va_list list;
    va_start(list, str);
    int count = vsnprintf(strbuf, sizeof(strbuf), str, list);
    va_end(list);
    strcat(strbuf, "\r\n");
    fputs(strbuf, stderr);
    return count;
}

bool pageTreeFrameSortFunction(FRAME * a, FRAME * b) {
    if (a->up < b->up)
        return true;

    if ((a->up == b->up) && (a->down < b->down))
        return true;

    return false;
}

int MaxArr(int *x, int n, int *PosExtr) {
    int in = 0;

    for (int i = 1; i <= n; ++i) {
        if (x[i] > x[in])
            in = i;
    }

    *PosExtr = in;
    return x[in];
}

int MinArr(int *x, int n, int *PosExtr) {
    int in = 0;

    for (int i = 1; i <= n; ++i) {
        if (x[i] < x[in])
            in = i;
    }

    *PosExtr = in;
    return x[in];
}

void bound_frm(FRAME **frm, int k_frm, BOUND *bnd) {
    int ymin = 32000, ymax = -32000, xmin = 32000, xmax = -32000;
    for (int i = 0; i <= k_frm; ++i) {
        ymin = std::min(ymin, frm[i]->up);
        ymax = std::max(ymax, frm[i]->down);
        xmin = std::min(xmin, frm[i]->left);
        xmax = std::max(xmax, frm[i]->right);
    }
    bnd->left = xmin;
    bnd->right = xmax;
    bnd->up = ymin;
    bnd->down = ymax;
}

void TestKNOTT1(KNOTT *ptr, LINE_KNOT *LineVK, LINE_KNOT *LineHK, int16_t NumFrm, int16_t NumVK,
        int16_t NumHK) {
    RECT r1 = ptr->Rect;
    KNOTT *up = ptr->up;

    if (r1.left >= r1.right || r1.top == r1.bottom || r1.left < 0 || r1.left >= NumVK || r1.right
            < 0 || r1.right >= NumVK || r1.top < 0 || r1.top >= NumHK || r1.bottom < 0 || r1.bottom
            >= NumHK || LineVK[r1.left].beg >= LineVK[r1.right].beg || LineHK[r1.top].beg
            >= LineHK[r1.bottom].beg || ptr->InBegFrm < (up ? up->InBegFrm : 0) || ptr->InBegFrm
            + ptr->NumFrm > (up ? up->InBegFrm + up->NumFrm : NumFrm))
        std::cerr << "Error #1: TestKNOTT1\n";
}

int Statist(int *arr, int n, int *ave, int *sig, int *med, int *mod, int regim) {
    return n > 0 ? statis1(arr, n - 1, ave, sig, med, mod, regim) : -1;
}

/*если regim > 0, усекаем края выборки вместо [0,n] - [n/regim,n-n/regim]*/
int statis1(TYPE *arr, int n, TYPE *ave1, TYPE *sig1, TYPE *med, TYPE *mod, int regim) {
    long ave = 0, sig = 0, work;

    if (*med) {
        u4sort(arr, n + 1, sizeof(TYPE), (COMP_FUN) comp1);

        if (regim) { /*обрезаем края выборки*/
            int beg = n / regim;

            if ((n -= 2 * beg) < 0) {
                n += 2 * beg;
                goto m1;
            }

            if (beg) {
                for (int i = 0; i <= n; ++i)
                    arr[i] = arr[i + beg];
            }
        }

        m1: *med = arr[n >> 1];
    }

    for (int i = 0; i <= n; ++i) {
        ave += arr[i];
    }

    ave /= (long) (n + 1);

    if (*sig1) {
        for (int i = 0; i <= n; ++i) {
            work = arr[i] - ave;
            sig += work * work;
        }
        sig = (long) sqrt(sig / (float) (n + 1));
        *sig1 = (TYPE) sig;
    }

    *ave1 = (TYPE) ave;
    return 0;
}

//Память под beg_as[][] захват-тся внутри, память под списк.пр-во KNOT3 снаружи
//==Генерация плоской АС на двусвязных списках
int GenAS(FRAME **frm, int k_frm, int dx, int dy, BOUND *bnd, KNOT3 *beg_free, int value, AS *As,
        RECT Rect) {
    int xmin, xmax, ymin, ymax, kx, ky, nx, ny, delx, dely;
    KNOT3 ***beg, *ptr;
    FRAME *f;
    /*оконтуривание решетки пустой полосой*/
    xmin = bnd->left;
    xmax = bnd->right;
    ymin = bnd->up;
    ymax = bnd->down;
    xmin -= (dx + 1);
    kx = ((xmax - xmin + 1) / dx) + 1;
    xmax = xmin + (kx + 1) * dx;
    ymin -= (dy + 1);
    ky = ((ymax - ymin + 1) / dy) + 1;
    ymax = ymin + (ky + 1) * dy;

    if ((beg = (KNOT3***) malloc((ky + 1) * sizeof(KNOT3**))) == NULL)
        return -3;

    for (int i = 0; i <= ky; ++i) {
        if ((beg[i] = (KNOT3**) malloc((kx + 1) * sizeof(KNOT3*))) == NULL)
            return NOT_ALLOC;

        memset(beg[i], 0, (kx + 1) * sizeof(PTR));
    }

    if (value != INDEF) { //заполнение ассоциат. стр-ры
        for (int i = 0; i <= k_frm; ++i) {
            f = frm[i];
            ny = (((f->up + f->down) >> 1) - ymin) / dy;
            nx = (((f->left + f->right) >> 1) - xmin) / dx;

            if (ny < 0 || nx < 0 || ny > ky || nx > kx)
                return -1;

            if ((ptr = (KNOT3*) inc_lst((KNOT**) &beg[ny][nx], (KNOT**) &beg_free)) == NULL)
                return -4;

            ptr->f = f;
            ptr->cl = value;
        }
    } else {
        for (int i = 0; i <= k_frm; ++i) {
            f = frm[i];
            ny = (((f->up + f->down) >> 1) - ymin) / dy;
            nx = (((f->left + f->right) >> 1) - xmin) / dx;

            if ((ptr = (KNOT3*) inc_lst((KNOT**) &beg[ny][nx], (KNOT**) &beg_free)) == NULL)
                return -4;

            ptr->f = f;
            delx = f->right - f->left;
            dely = f->down - f->up;
            ptr->cl = (delx >= Rect.left && delx <= Rect.right && dely >= Rect.top && dely
                    <= Rect.bottom) ? 1 : value;
        }
    }

    As->bnd = bnd;
    As->kx = kx;
    As->ky = ky;
    As->dx = dx;
    As->dy = dy;
    As->beg_free = beg_free;
    As->beg_as = beg;
    As->xmin = xmin;
    As->ymin = ymin;
    return 0;
}

#ifdef alDebug
//===if ColFrm < 0, no draw frm
//================================================================================
//=================================   ImageKnot1  ================================
//================================================================================
void ImageKnot1(KNOTT *ptr, LINE_KNOT *LineVK, LINE_KNOT *LineHK, int16_t col, int16_t line_style,
        int16_t fill, int16_t ColFrm, FRAME **f, int16_t NumFrm, int16_t NumVK, int16_t NumHK) {
    RECT r, r1 = ptr->Rect;
    int16_t fl = (ColFrm != INDEF) ? 0 : 1;

    if (ColFrm == INDEF)
    ColFrm = col;

    r.left = LineVK[r1.left].beg;
    r.right = LineVK[r1.right].beg;
    r.top = LineHK[r1.top].beg;
    r.bottom = LineHK[r1.bottom].beg;

    if (ColFrm >= 0 && ptr->InBegFrm >= 0 && ptr->NumFrm >= 0) {
        TestKNOTT1(ptr, LineVK, LineHK, NumFrm, NumVK, NumHK);
    }
}

//проход по дереву с отображением
//================================================================================
//=================================   ImageTree1  ================================
//================================================================================
int16_t ImageTree1(KNOTT *Root, LINE_KNOT *LineVK, LINE_KNOT *LineHK, FRAME **frm, int16_t NumFrm,
        int16_t NumVK, int16_t NumHK) {
    STACK St;
    int16_t DepthTree = 20, col, ColFrm, i = 1;
    KNOTT *Curr;
    char *err = "ImageTree1";

    if (NewStack(DepthTree, &St))
    return NOT_ALLOC;

    Curr = Root;
    col = 0;

    while (Curr != NULL) {
        if (++col > 15)
        col = 1;

        ColFrm = col;

        //ImageKnot1(Curr,LineVK,LineHK,col,0xFFFF,_GBORDER,ColFrm,frm,NumFrm,NumVK,NumHK);
        //--Рисуем по перекрестным ссылкам терминал. H-графы,списки их V-ссылок и им обратные
        //if(Curr->InBegFrm == IN_NO && Curr->down == NULL) //Терм. H-узел
        cf::FMT_DBG("i=%2d   InBegFrm=%2d  NumFrm=%2d  InColA=%6d  OrderChild=%2d", i,
                Curr->InBegFrm, Curr->NumFrm, Curr->InColA, Curr->OrderChild);
        ++i;

        ImageKnot1(Curr, LineVK, LineHK, col, (int16_t) 0xFFFF, _GBORDER, (int16_t) -1, frm,
                NumFrm, NumVK, NumHK);
        Curr = NextKnot(Curr, &St);

        if (OverflowStack(&St))
        return NOT_ALLOC;
    }

    DelStack(&St);
    return 0;
}

void init_font(void) {
    ;
}
#endif /*DRAW*/

static int16_t Realloc2(KNOTT*** colt, KNOTT*** colnt, KNOTT*** colnt1, int16_t **begI,
        int16_t **endI, int16_t **intr, int16_t nOld, int16_t nNew) {
    size_t newS = nNew * sizeof(PTR);
    size_t newS1 = nNew * sizeof(int16_t);

    if ((colt && ((*colt = (KNOTT**) realloc(*colt, newS)) == NULL)) || (colnt && ((*colnt
            = (KNOTT**) realloc(*colnt, newS)) == NULL)) || (colnt1 && ((*colnt1
            = (KNOTT**) realloc(*colnt1, newS)) == NULL)) || (begI && ((*begI = (int16_t*) realloc(
            *begI, newS1)) == NULL)) || (endI && ((*endI = (int16_t*) realloc(*endI, newS1))
            == NULL)) || (intr && ((*intr = (int16_t*) realloc(*intr, newS1)) == NULL)))
        return NOT_ALLOC;

    return 0;
}

void ConvertRect16ToBnd(Rect16 *r, SRECT *b) {
    b->left = r->left;
    b->right = r->right;
    b->top = r->top;
    b->bottom = r->bottom;
}

#ifdef alDebug
void show_frm(int16_t NumFragm, FRAME **frm) {
    int16_t i;

    for (i = 0; i < NumFragm; ++i) {
        cf::FMT_DBG(" i=%d start_pos=%d l=%d r=%d t=%d b=%d", i, frm[i]->start_pos, frm[i]->left,
                frm[i]->right, frm[i]->up, frm[i]->down);
    }
}
#endif

//==Generate Columns Tree by Cunei-array of fragments
//tree generation:
//  input
//      RectFragm[NumFragm],
//      setup - input parameters
//  output - Inf
int GenerateTreeByFragm(Rect16 *RectFragm, int16_t NumFragm, SETUP_GENERATE_TREE *setup,
        FRAME ***Frm1, INF_TREE *Inf) {
    FRAME **frm;
    BOUND BndAll;
    ArrFrm = (FRAME*) malloc(NumFragm * sizeof(FRAME));
    frm = (FRAME**) malloc(NumFragm * sizeof(PTR));

    if (!ArrFrm || !frm)
        return NOT_ALLOC;

    *Frm1 = frm;
    //--BndAll is all page boundaries--
    BndAll.left = 32000;
    BndAll.right = -32000;
    BndAll.up = 32000;
    BndAll.down = -32000;

    //--convert from RectFragm[NumFragm] to frm[NumFragm]--
    for (int i = 0; i < NumFragm; ++i) {
        frm[i] = &ArrFrm[i];
        //"i" is initial index for text information extraction from fragment arrays Zn,TitleWord,TitleStr,etc.
        ArrFrm[i].start_pos = i;
        ArrFrm[i].left = RectFragm[i].left;
        ArrFrm[i].right = RectFragm[i].right;
        ArrFrm[i].up = RectFragm[i].top;
        ArrFrm[i].down = RectFragm[i].bottom;
#ifdef alDebug

        cf::FMT_DBG("i=%d,l=%d,r=%d,u=%d,d=%d", i, RectFragm[i].left, RectFragm[i].right,
                RectFragm[i].top, RectFragm[i].bottom);

#endif
        BndAll.left = MIN(BndAll.left, RectFragm[i].left);
        BndAll.right = MAX(BndAll.right, RectFragm[i].right);
        BndAll.up = MIN(BndAll.up, RectFragm[i].top);
        BndAll.down = MAX(BndAll.down, RectFragm[i].bottom);
    }

    //--calling internal function for tree generation--
    if (CreateTreePlainTxt1(BndAll, NULL, 0, NULL, 0, frm, NumFragm, Inf, setup->size_x,
            setup->size_y))
        return -200;

    return 0;
}

int CreateTreePlainTxt1(BOUND BndTxt, STRET *LineV, int16_t NumLV, STRET *LineH, int16_t NumLH,
        FRAME **frm, int16_t NumFrm, INF_TREE *Inf, int16_t size_x, int16_t size_y) {
    int16_t i, nVmax = 3 * MAX_COL, nHmax = 3 * MAX_COL, nV, nH, j;
    int16_t fl, tmp, InBegFrm, NumF, ThresX, ThresY;
    int16_t left, right, top, bottom, NumMax = 3 * MAX_COL, MaxOld;
    int16_t *intr, *begI, *endI;
    int16_t k_colnt, k_colnt1, k_colt, minz;
    int16_t fl_beg/*Признак первого расщепления на колонки*/;
    int16_t order/*Тип искомой упорядоченности:HOR - горизонт. или VER - вертикал.*/;
    int16_t kf, kcol, del, del1;
    TREE2 Tree;
    BOUND bndc;
    KNOTT *PrevChild, *Child;
    KNOTT **colt, **colnt, **colnt1, *ptr;
    LINE_KNOT *LineVK, *LineHK;
    STAT_CELL *StatCell = (STAT_CELL*) malloc(sizeof(STAT_CELL));

    cf::FMT_DBG("===  CreateTreePlainTxt1  ===");

    //LineVK, LineHK - виртуальные либо натуральные линии разграфки,
    //   сепарирующие найденные колонки, с помощью этих линий производитс
    //   горизонт. и вертикал. выравнивание колонок
    LineVK = (LINE_KNOT*) malloc((nVmax + 2) * sizeof(LINE_KNOT));
    LineHK = (LINE_KNOT*) malloc((nHmax + 2) * sizeof(LINE_KNOT));
    intr = (int16_t*) malloc(NumMax * sizeof(int16_t));
    begI = (int16_t*) malloc(NumMax * sizeof(int16_t));
    endI = (int16_t*) malloc(NumMax * sizeof(int16_t));
    //Заносим в разграфку натурал. линии после их ортогонализации
    //-- Замыкание системы H- и V-линий с краев по рамке листа --
    LineVK[0].beg = BndTxt.left;
    LineVK[1].beg = BndTxt.right;
    LineHK[0].beg = BndTxt.up;
    LineHK[1].beg = BndTxt.down;
    LineVK[0].Thick = LineVK[1].Thick = LineHK[0].Thick = LineHK[1].Thick = 0;
    LineVK[0].TypeLine = LineVK[1].TypeLine = LineHK[0].TypeLine = LineHK[1].TypeLine = HIDDEN_LINE;
    nV = nH = 2;
    colt = (KNOTT**) malloc(NumMax * sizeof(PTR));
    colnt = (KNOTT**) malloc(NumMax * sizeof(PTR));
    colnt1 = (KNOTT**) malloc(NumMax * sizeof(PTR));

    if (colt == NULL || colnt == NULL || colnt1 == NULL) {
        cf::FMT_DBG("CreateTreePlainTxt1(): NOT_ALLOC at %d", __LINE__);
        return NOT_ALLOC;
    }

    //--Создание спискового простр-ва и корня дерева--
    Tree.NumKnot = NumMax * 2;
    Tree.NumSeg = -1;

    if ((fl
            = init_lst(&Tree.ArrSeg, &Tree.NumSeg, Tree.NumKnot, (KNOT**) &Tree.free, sizeof(KNOTT)))
            != 0) {
        cf::FMT_DBG("CreateTreePlainTxt1(): NOT_ALLOC at %d", __LINE__);
        return -90 - fl;
    }

    if ((Tree.Root = IncKnot(NULL, NULL, &Tree.free)) == NULL) {
        cf::FMT_DBG("CreateTreePlainTxt1(): NOT_ALLOC at %d", __LINE__);
        return -6;
    }

    FillFieldKNOTT1(Tree.Root, 0, 1, 0, 1, 0, NumFrm, IN_NO, UNSORT, ROOT, TRUE, ALG_BEG, ALG_BEG,
            NULL);
    colnt[k_colnt = 0] = Tree.Root;
    k_colt = -1;
    fl_beg = 1;
    order = VER;//Так удобнее для RTF-converter
    ThresX = ThresY = (int16_t) (1.5 * size_y);
    int16_t MaxAllowLev, flTerm;
    MaxAllowLev = 6;
    MaxLev = 0;
    flTerm = FALSE;
    //вначале считаем, что все фрагменты образуют одну колонку,
    //внешний цикл колонизации - на каждой итерации MaxLev производитс
    //попытка разделить те узлы-колонки, которые есть на этом уровне
    //и для тех узлов-колонок, которые поделились, строим дочерние
    //узлы-подколонки на следующем уровне MaxLev+1
    //colnt[k_colnt]   - массив узлов текущего уровня MaxLev
    //colnt1[k_colnt1] - массив узлов следующего уровня MaxLev+1
    //colt[k_colt] - массив терминальных колонок
    //признак конца итераций - на очередном уровне нет узлов, т.е. ни один
    //из узлов предыдущего уровня не удалось раздробить

    while (k_colnt >= 0) {
        if (++MaxLev > MaxAllowLev) //ограничение Word-a на глубину дерева
            flTerm = TRUE;

        k_colnt1 = -1; /*Число нетерминал. колонок след. уровня*/
        //--цикл попытки дробления всех узлов текущего уровня--
        cf::FMT_DBG("while....................  k_colnt=%d", k_colnt);
        for (i = 0; i <= k_colnt; i++) {
            cf::FMT_DBG("beg......................1");

            ptr = colnt[i]; //текущий узел
            left = ptr->InBegFrm; //индекс первой рамки (фрагмента) узла
            kf = ptr->NumFrm - 1; //число рамок(фрагментов)узла
            ptr->OrderChild = UNSORT;//вначале считаем, что рамки узла неупорядочены
            //если же узел удастся разбить, то порядок
            //узла определяется упорядоченностью его дочерних
            //узлов (V- или H-порядок)

            cf::FMT_DBG("i=%d, (index first fragm) left=%d, (count fragm) kf=%d,", i, left, kf);

            //bndc - рамка узла
            if (!fl_beg) //рамка узла есть рамка входящих в узел рамок фрагментов
                bound_frm(&frm[left], kf, &bndc);
            else
                //вначале рамка узла-корня есть рамка всего листа
                bndc = BndTxt;

            cf::FMT_DBG(" fl_beg=%d, bndc : left=%d, right=%d, up=%d, down=%d", fl_beg, bndc.left,
                    bndc.right, bndc.up, bndc.down);

            //---
            MaxOld = NumMax;

            //-- попытка разбиение узла путем построения соответствующего --
            //   профиля: на ось Ox для order=HOR или на ось Oy для order=VER
            //   и поиска на нем межколонных зазоров
            if (flTerm) {
                fl = kcol = 0;
            } else {
                if ((fl = SearchColHist1(&frm[left], kf, &bndc, size_x, size_y, order, &kcol,
                        &intr, &begI, &endI, &NumMax)) < 0) {
                    cf::FMT_DBG("at line %d. !!!!! SearchColHist1 fl=%d", __LINE__, fl);
                    return fl - 2000;
                }

                cf::FMT_DBG("after SearchColHist1 MaxOld=%d, NumMax=%d", MaxOld, NumMax);
            }

            if (MaxOld != NumMax && Realloc2(&colt, &colnt, &colnt1, NULL, NULL, NULL, MaxOld,
                    NumMax) == NOT_ALLOC) {
                cf::FMT_DBG(" NOT_ALLOC at %d", __LINE__);
                return NOT_ALLOC;
            }

            if (!fl && fl_beg) {
                //В I-ый раз пробуем ортогональное направления разбиения,
                //если не удалось разбиение в прямом направлении
                //вообще-то лучше сделать в корневом узле не предопределенный выбор
                //направления первого разбиения, если допустимы оба направления,
                //а выбор того направления, на котором между колонками больший зазор
                order = order == HOR ? VER : HOR;
                MaxOld = NumMax;

                if ((fl = SearchColHist1(&frm[left], kf, &bndc, size_x, size_y, order, &kcol,
                        &intr, &begI, &endI, &NumMax)) < 0) {
                    cf::FMT_DBG("fl-2000 at %d", __LINE__);
                    return fl - 2000;
                }

                if (MaxOld != NumMax && Realloc2(&colt, &colnt, &colnt1, NULL, NULL, NULL, MaxOld,
                        NumMax) == NOT_ALLOC) {
                    cf::FMT_DBG("NOT_ALLOC at %d", __LINE__);
                    return NOT_ALLOC;
                }
            }

            if (fl) //если разбиение удалось, присваиваем узлу порядок, определяемый направлением разбиени
                ptr->OrderChild = order;

            if (kcol > 0) {
                cf::FMT_DBG("beg------------------2");
                cf::FMT_DBG("kcol=%d", kcol);
                BOUND b;

                if (fl_beg && ptr->OrderChild == HOR)
                    --MaxAllowLev;

                //Разбиение удалось => заносим в дерево дочерний куст подколонок данного узла
                PrevChild = NULL;
                for (j = 0; j <= kcol; ++j) { //цикл внесению дочерей в дерево
                    //вставляем дочерний узел в дерево:
                    //      ptr - узел-родитель
                    //      PrevChild - предыдущее дит
                    //      Tree.free - свободное списковое пространство дерева
                    cf::FMT_DBG("beg------------------3");
                    cf::FMT_DBG("j=%d", j);

                    if ((Child = IncKnot(ptr, PrevChild, &Tree.free)) == NULL) {
                        cf::FMT_DBG("!!!!!! IncKnot return -7 at %d", __LINE__);
                        return -7;
                    }

                    //номер начального фрагмента InBegFrm и число фрагментов NumF данной колонки
                    tmp = j ? intr[j - 1] + 1 : 0;
                    InBegFrm = ptr->InBegFrm + tmp;
                    NumF = intr[j] - tmp + 1;
                    cf::FMT_DBG("InBegFrm=%d, NumF=%d", InBegFrm, NumF);
                    //--ищем близкие линии или генерим новые для границ колонки--
                    bound_frm(&frm[InBegFrm], NumF - 1, &b);
                    del = begI[j] - (j ? endI[j - 1] : 0);

                    if (order == HOR) {
                        cf::FMT_DBG("beg------------------4");
                        cf::FMT_DBG("order == HOR");
                        minz = MIN(ThresX, del);

                        if ((left = !j ? ptr->Rect.left : AddLine1(&LineVK, &nV, &nVmax,
                                (int16_t) endI[j - 1], minz)) < 0) {
                            cf::FMT_DBG("       left-100 at %d", __LINE__);
                            return left - 100;
                        }

                        if ((right = j == kcol ? ptr->Rect.right : AddLine1(&LineVK, &nV, &nVmax,
                                begI[j], minz)) < 0) {
                            cf::FMT_DBG("       right-200 at %d", __LINE__);
                            return right - 200;
                        }

                        //top=ptr->Rect.top; bottom=ptr->Rect.bottom;
                        //!!!для разновысоких колонок можно уточнять и верхи и низы!!!
                        //но тогда придется рассчитать рамки новых колонок
                        del1 = b.down - b.up;
                        minz = MIN(ThresY, del1);

                        if ((top = AddLine1(&LineHK, &nH, &nHmax, b.up, minz)) < 0) {
                            cf::FMT_DBG("       top-100 at %d", __LINE__);
                            return top - 100;
                        }

                        if ((bottom = AddLine1(&LineHK, &nH, &nHmax, b.down, minz)) < 0) {
                            cf::FMT_DBG("       bottom-100 at %d", __LINE__);
                            return bottom - 100;
                        }

                        cf::FMT_DBG("end------------------4");
                    } else {
                        cf::FMT_DBG("beg------------------5");
                        cf::FMT_DBG("order == VER");
                        minz = MIN(ThresY, del);

                        if ((top = !j ? ptr->Rect.top : AddLine1(&LineHK, &nH, &nHmax, endI[j - 1],
                                minz)) < 0) {
                            cf::FMT_DBG("       top-300 at %d", __LINE__);
                            return top - 300;
                        }

                        if ((bottom = j == kcol ? ptr->Rect.bottom : AddLine1(&LineHK, &nH, &nHmax,
                                begI[j], minz)) < 0) {
                            cf::FMT_DBG("       bottom-400 at %d", __LINE__);
                            return bottom - 400;
                        }

                        //left=ptr->Rect.left; right=ptr->Rect.right;
                        //!!!для разношироких колонок можно уточнять и боковые линии!!!
                        del1 = b.right - b.left;
                        minz = MIN(ThresX, del1);

                        if ((left = AddLine1(&LineVK, &nV, &nVmax, b.left, minz)) < 0) {
                            cf::FMT_DBG("       left-100 at %d", __LINE__);
                            return left - 100;
                        }

                        if ((right = AddLine1(&LineVK, &nV, &nVmax, b.right, minz)) < 0) {
                            cf::FMT_DBG("       right-200 at %d", __LINE__);
                            return right - 200;
                        }

                        cf::FMT_DBG("end------------------5");
                    }

                    //--разрешение противоречия равенства индексов начальной и конечной--
                    //  границ для малоразмерных колонок
                    if (left == right) { //узкая колонка
                        cf::FMT_DBG("left == right <<narrow column>>");

                        if (j != kcol || !fl) {
                            minz = fl ? (order == HOR ? begI[j] : b.right) : b.right;

                            if ((right = AddLine1(&LineVK, &nV, &nVmax, minz, -1)) < 0) {
                                cf::FMT_DBG("       right-200 at %d", __LINE__);
                                return right - 200;
                            }
                        } else {
                            minz = fl ? (order == HOR ? endI[j - 1] : b.left) : b.right;

                            if ((left = AddLine1(&LineVK, &nV, &nVmax, minz, -1)) < 0) {
                                cf::FMT_DBG("       right-200 at %d", __LINE__);
                                return right - 200;
                            }
                        }
                    }

                    if (top == bottom) { //низкая колонка
                        cf::FMT_DBG("top == bottom <<low column>>");

                        if (j != kcol || !fl) {
                            minz = fl ? (order == VER ? begI[j] : b.down) : b.down;

                            if ((bottom = AddLine1(&LineHK, &nH, &nHmax, minz, -1)) < 0) {
                                cf::FMT_DBG("       bottom-100 at %d", __LINE__);
                                return bottom - 100;
                            }
                        } else {
                            minz = order == VER ? endI[j - 1] : b.up;

                            if ((top = AddLine1(&LineHK, &nH, &nHmax, minz, -1)) < 0) {
                                cf::FMT_DBG("       top-100 at %d", __LINE__);
                                return top - 100;
                            }
                        }
                    }

                    if (LineVK[left].beg - 30 >= LineVK[right].beg || LineHK[top].beg - 10
                            >= LineHK[bottom].beg) //LineHK[top].beg >= LineHK[bottom].beg)
                        cf::FMT_DBG(" Неправильные координаты фрагмента!!!");

                    if (LineVK[left].beg - 30 >= LineVK[right].beg || LineHK[top].beg - 10
                            > LineHK[bottom].beg) { //LineHK[top].beg >= LineHK[bottom].beg)
                        cf::FMT_DBG(" return -6 at %d", __LINE__);
                        return -6;
                    }

                    //заполняем поля нового узла колонки
                    //!!! поля left,right,top,bottom имеют смысл индексов линий разграфки
                    FillFieldKNOTT1(Child, left, right, top, bottom, InBegFrm, NumF, IN_NO, UNSORT,
                            CELL, TRUE, ALG_BEG, ALG_BEG, NULL);

                    if (fl) {
                        if (++k_colnt1 >= NumMax) {
                            int16_t old = NumMax;
                            NumMax = (int16_t) (NumMax * 1.5);

                            if (Realloc2(&colt, &colnt, &colnt1, &begI, &endI, &intr, old, NumMax)
                                    == NOT_ALLOC) {
                                cf::FMT_DBG("NOT_ALLOC at %d", __LINE__);
                                return NOT_ALLOC;
                            }
                        }

                        colnt1[k_colnt1] = Child;//Запоминаем очередную нетерминал. колонку
                    }

                    PrevChild = Child;
                    cf::FMT_DBG("end------------------3");
                }
                cf::FMT_DBG("end------------------2");
            } else { /*Обнаружена терминал. колонка*/
                cf::FMT_DBG("Terminal column found");
                cf::FMT_DBG("beg------------------2.1");

                if (++k_colt >= NumMax) {
                    int16_t old = NumMax;
                    NumMax = (int16_t) (NumMax * 1.5);

                    if (Realloc2(&colt, &colnt, &colnt1, &begI, &endI, &intr, old, NumMax)
                            == NOT_ALLOC) {
                        cf::FMT_DBG("NOT_ALLOC at %d", __LINE__);
                        return NOT_ALLOC;
                    }
                }

                colt[k_colt] = ptr;
                //заполняем поля очередного терминал. узла
                ptr->OrderChild = TERM;
                colt[k_colt]->InColA = (int) frm[left]->start_pos;
                cf::FMT_DBG("end------------------2.1");
            }

            cf::FMT_DBG("end------------------1");
        }
        //переписываем узлы следующего уровня в текущий уровень для след.итерации
        for (i = 0; i <= k_colnt1; ++i)
            colnt[i] = colnt1[i];
        k_colnt = k_colnt1;
        fl_beg = 0;
        order = order == HOR ? VER : HOR;//меняем порядок на ортогональный для след.итерации

        if (flTerm == TRUE)
            break;
    } //end of while

    free(colt);
    free(colnt);
    free(colnt1);
    free(intr);
    free(begI);
    free(endI);
    tmp = k_colt;

    //сортируем линии разграфки и перенумеруем их в узлах дерева
    if ((fl = SortHorLine1(LineHK, nH, LineVK, nV, Tree.Root, &colt, &k_colt, frm))) {
        cf::FMT_DBG("fl-260 at %d", __LINE__);
        return fl - 260;
    }

    if (tmp != k_colt) {
        cf::FMT_DBG("fl-11 at %d", __LINE__);
        return fl - 11;
    }

#ifdef alDebug
    pTheGeomStep = pTheGeomStep2;

    for (i = 0; i <= k_colt; ++i)
    ImageKnot1(colt[i], LineVK, LineHK, 14, (int16_t) 0xFFFF, _GBORDER, (int16_t) -1, frm,
            NumFrm, nV, nH);

    pTheGeomStep = pTheGeomStep1;
    FlagGraphic1 = 1;
    Graphic1Color = 0;
    fl = ImageTree1(Tree.Root, LineVK, LineHK, frm, NumFrm, nV, nH);
    FlagGraphic1 = 0;
#endif
    //заносим в выходную "деревянную" структуру дерево и его причандалы
    Inf->Tree = Tree; //дерево
    Inf->StatCell = StatCell; //
    Inf->LineVK = LineVK; //V-разграфка дерева LineVK[nV]
    Inf->nV = nV; //
    Inf->LineHK = LineHK; //H-разграфка дерева LineHK[nH]
    Inf->nH = nH; //
    Inf->ColT = colt; //терминал. колонки дерева colt[NumT]
    Inf->NumT = k_colt; //

    if (!(Inf->bnd_col = (BOUND*) malloc((k_colt + 1) * sizeof(BOUND)))) {
        cf::FMT_DBG("NOT_ALLOC at %d", __LINE__);
        return NOT_ALLOC;
    }

    for (i = 0; i <= k_colt; ++i) {
        colt[i]->AllowOCR = 1;
        Inf->bnd_col[i].left = LineVK[colt[i]->Rect.left].beg;
        Inf->bnd_col[i].right = LineVK[colt[i]->Rect.right].beg;
        Inf->bnd_col[i].up = LineHK[colt[i]->Rect.top].beg;
        Inf->bnd_col[i].down = LineHK[colt[i]->Rect.bottom].beg;
    }

    cf::FMT_DBG("==  !!!! end  CreateTreePlainTxt1   == ");
    free((KNOT**) StatCell);
    return 0;
}

//Return:
// >= 0 - среди существующих линий найдена близкая в смысле Thres (ее индекс)
//        или сгенерирована новая (ее индекс)
// < 0 - ERR
//Вход:               Coor - x-коор-та для V-линии или y-коор-та для H-линии
//                                      Thres - порог грубости отождествления линий
//Обменные параметры: Line1[nCurr] - система линий
//                    nMax - максимальное число линий
int AddLine1(LINE_KNOT **Line1, int16_t *nCurr, int16_t *nMax, int16_t Coor, int16_t Thres) {
    LINE_KNOT *Line = *Line1;

    for (int i = 0; i < *nCurr; ++i)
        if (abs(Line[i].beg - Coor) < Thres)
            return i;

    int n = *nCurr;//Add new Line

    if (n >= *nMax) { //обработка переполнение массива линий
        *nMax = (int16_t) (*nMax * 1.5);

        if ((Line = (LINE_KNOT*) realloc(Line, *nMax * sizeof(LINE_KNOT))) == NULL)
            return NOT_ALLOC;

        *Line1 = Line;
    }

    Line[n].beg = Coor;
    Line[n].Thick = 0;
    Line[(*nCurr)++].TypeLine = HIDDEN_LINE;
    return n;
}

/*Классификация рамок по колонкам после устранения перекосов рекурсивным
 делением ортогональных профилей
 Вход: frm[k_frm] - рамки компонент (фрагментов);
 bnd - габариты текущего узла;
 ave_x,ave_y - среднее габаритов рамки компоненты;
 NumMax - максимал.число колонок
 Выход:frm[k_frm]  - рамки переупорядочены поколонно, если колонки обнаружены;
 intr1[kcol] - правые границы найденных колонок в массиве frm,
 begI,endI   - геометрические границы межколон. интервалов
 Return:
 0 - если колонки не обнаружены,
 kcol, если обнаружены,
 < 0 - ERR*/
int16_t SearchColHist1(FRAME **frm, int16_t k_frm, BOUND *bnd, int16_t ave_x, int16_t ave_y,
        int16_t reg, int16_t *k_int, int16_t **intr1, int16_t **begI, int16_t **endI,
        int16_t *NumMax) {
    int16_t ave_dir, ave_ort, i, kcol, x, in, kf, fl, ki, MaxOld = *NumMax;
    int k_bloc;
    int16_t *intr = *intr1;
    KNOT4 *Free, **knot, **beg = (KNOT4**) malloc(*NumMax * sizeof(PTR)), *ptr;
    cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "===beg    SearchColHist1   ===");

    if (reg == HOR) {
        ave_dir = ave_x;
        ave_ort = ave_y;
        cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "reg==HOR,ave_dir=%d ; ave_ort=%d", ave_dir,
                ave_ort);
    } else {
        ave_dir = ave_y;
        ave_ort = ave_x;
        cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "reg==VER,ave_dir=%d ; ave_ort=%d", ave_dir,
                ave_ort);
    }

    if ((fl = SearchInterval1(frm, k_frm, begI, endI, k_int, bnd, ave_dir, ave_ort, reg, NumMax))
            <= 0) {
        cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "!!!!! SearchInterval1 return<0");
        free(beg);
        return fl;
    }

    if (MaxOld != *NumMax) {
        if ((intr = (int16_t*) realloc(intr, *NumMax * sizeof(int16_t))) == NULL || (beg
                = (KNOT4**) realloc(beg, *NumMax * sizeof(PTR))) == NULL)
            return NOT_ALLOC;
    }

    kcol = *k_int + 1;
    cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "kcol=%d", kcol);
    for (i = 0; i <= kcol; ++i)
        beg[i] = NULL;

    for (i = 0; i < kcol; ++i)
        intr[i] = ((*begI)[i] + (*endI)[i]) >> 1;

    k_bloc = -1;

    if ((fl = init_lst((KNOT***) &knot, &k_bloc, k_frm + 2, (KNOT**) &Free, sizeof(KNOT4))) != 0)
        return -fl - 50;

    cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "k_frm=%d", k_frm);
    for (int i = 0; i <= k_frm; ++i) { /*занесение рамок в списки колонок*/
        x = (reg == HOR) ? frm[i]->left : frm[i]->up;

        if (kcol < 2)
            in = (x < intr[0]) ? 0 : 1;
        else
            in = search_int((int*) intr, *k_int, x);

        ptr = (KNOT4*) inc_lst((KNOT**) &beg[in], (KNOT**) &Free);
        ptr->f = frm[i];
    }
    kf = -1;
    ki = -1;
    for (int i = 0; i <= kcol; ++i) {
        ptr = beg[i];

        if (ptr == NULL)
            continue;

        while (ptr != NULL) {
            frm[++kf] = ptr->f;
            ptr = ptr->next;
        }

        intr[++ki] = kf;
    }

    free_lst((KNOT**) knot, k_bloc);

    if (kf != k_frm) {
        cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "!!!!!!SearchColHist1: kf != k_frm;return -7");
        return -7;
    }

    *k_int = ki;
    free(beg);
    cf::FMT_DBG(cf::FormatDebug::HISTOGRAM, "===SearchColHist1: normal end; kcol=%d===", kcol);
    return kcol;
}

/*Поиск колонок по вертикали либо горизонтали - функция разбиения одного узла
 методом анализа профил
 Вход: frm[k_frm] - рамки компонент (фрагментов);
 bnd - габариты текущего узла;
 ave_dir,ave_ort - среднее размера буквы-компоненты в прямом и перпендикулярном напр.
 NumMax - максимал.число колонок
 reg - режим колонизации (HOR - горизонтально-упорядоченные колонки,
 VER - вертикально --//--).
 Выход:frm[k_frm]  - рамки переупорядочены поколонно, если колонки обнаружены;
 intr1[kcol] - правые границы найденных колонок в массиве frm,
 beg1,end1   - [k_int1] геометрические границы межколон. интервалов
 Выход:intr[k_int1]-середины найденных межколонных интервалов
 reg=HOR - ищем горизонт. упорядоченные колонки,VER - вертикально-упорядоченные
 Return:
 0 - разбиение не произошло,
 1 - разбиение произошло,
 < 0 - ERR*/
int16_t SearchInterval1(FRAME **frm, int16_t k_frm, int16_t **beg1, int16_t **end1,
        int16_t *k_int1, BOUND *bnd, int16_t ave_dir, int16_t ave_ort, int16_t reg, int16_t *NumMax) {
    int16_t k_int, pos, min_col, min_int, maxh, minh, midh, kstr, len, len_group, i, j;
    int16_t beg_int, end_int, sumh, ave_h, mi, ma, Home, Fin;
    int16_t *his, *his_first_group, *his_second_group, last_real_line, *Beg = *beg1, *End = *end1,
            tmp_pos;
    //--Расчет параметров одномерной колонизации--
    min_col = 1;//миним. длина колонки в пикселах
    min_int = ave_dir * 2;//миним. длина интервала в пикселах
    maxh = 0;//Макс.допустимая высота гистограммы в интервале-критерий обнаруж.начала интервала
    midh = 0;//Макс.допустимая средняя высота гист-мы в интервале
    minh = 1;//Мин.допустимая высота гистограммы в колонке-критерий обнаруж.начала колонки
    //после настоящего интервала и с левого края листа

    if (reg == HOR)
        min_int = 5;/*ave_dir * 1.5*/

    ; //~~~

    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "=== begin SearchInterval1===");
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "min_int= %d", min_int);

    if (reg == HOR) {
        Home = bnd->left;
        Fin = bnd->right;
        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "reg==HOR; Home=%d; Fin=%d;", Home, Fin);
    } else {
        Home = bnd->up;
        Fin = bnd->down;
        len_group = bnd->right - bnd->left + 2;
        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "reg==VER; Home=%d; Fin=%d;", Home, Fin);
    }

    //Оценка числа строк(HOR)листа /ширины листа в символах(VER)
    kstr = (int16_t) (reg == HOR ? (bnd->down - bnd->up) / (2 * ave_ort) : (bnd->right - bnd->left)
            / (1.2 * ave_ort));
    //--Вычисление профиля-гистограммы числа компонент, проецирующихся в данный пиксел--
    len = Fin - Home + 2;//длина гистограммы
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "kstr=%d  len_hist=%d", kstr, len);

    if ((his = (int16_t*) malloc(len * sizeof(int16_t))) == NULL)
        return NOT_ALLOC;

    memset(his, 0, len * sizeof(int16_t));
    --len;

    if (reg == VER) {
        if (len_group < 0) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "begin len_group=%d ", len_group);
            len_group = 0;
        }

        if ((his_first_group = (int16_t*) malloc((len_group + 4) * sizeof(int16_t))) == NULL)
            return NOT_ALLOC;

        if ((his_second_group = (int16_t*) malloc((len_group + 4) * sizeof(int16_t))) == NULL)
            return NOT_ALLOC;

        last_real_line = 0;
        --len_group;
    }

    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "k_frm=%d ", k_frm + 1);
#ifdef alDebug
    {
        pTheGeomTemp.clear();
        tagRECT rct;
        SetRect(&rct, bnd->left, bnd->up, bnd->right, bnd->down);
        pTheGeomTemp.push_back(rct);
    }
#endif
    for (int i = 0; i <= k_frm; ++i) {
#ifdef alDebug
        {
            tagRECT rct;
            SetRect(&rct, frm[i]->left, frm[i]->up, frm[i]->right, frm[i]->down);
            pTheGeomTemp.push_back(rct);
        }
#endif

        if (reg == HOR) {
            mi = frm[i]->left - Home;
            ma = frm[i]->right - Home;
            cf::FMT_DBG(cf::FormatDebug::INTERVAL,
                    "reg==HOR: frm[i]->left=%d,frm[i]->right=%d, mi=%d, ma=%d", frm[i]->left,
                    frm[i]->right, mi, ma);
        } else {
            mi = frm[i]->up - Home;
            ma = frm[i]->down - Home;
            cf::FMT_DBG(cf::FormatDebug::INTERVAL,
                    "reg==VER: frm[i]->up=%d,frm[i]->down=%d,mi=%d,ma=%d", frm[i]->up,
                    frm[i]->down, mi, ma);
        }

        if (mi < 0 || ma > len) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "!!!!!! mi < 0 || ma > len return -6");
            return -6;
        }

        for (int j = mi; j <= ma; ++j)
            ++his[j];
    }
#ifdef alDebug
    CountRect = pTheGeomTemp.size();
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "---Поиск межколон. интервалов---");
#endif
    //--Поиск межколон. интервалов--
    k_int = -1;
    pos = -1;

    while ((int16_t) his[++pos] < minh) { /*поиск I позиции текста*/
        if (pos >= len) {
            if (reg == VER) {
                free(his_first_group);
                free(his_second_group);
            }

            free(his);
            *k_int1 = -1;
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "===колонки не обнаружены===");
            return 0;
        } /*колонки не обнаружены*/
    }

    do { /*ищем I позицию интервала*/
        while (pos < len - min_int - min_col && (int16_t) his[++pos] > maxh)
            ;

        if (pos >= len - min_int - min_col) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "!!!!!! pos >= len-min_int-min_col");
            break;
        }

        beg_int = pos;

        /*ищем последовательность не менее чем min_int высотой < maxh*/
        while ((int16_t) his[++pos] <= maxh)
            if (pos >= len || pos - beg_int >= min_int)
                break;

        if (pos - beg_int < min_int && Check_IsItFalseHorLine(last_real_line, reg, frm, his, pos,
                len, maxh, min_int + min_col, len_group, his_first_group, his_second_group, bnd,
                k_frm))
            continue;

        if (pos >= len)
            break;

        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "beg_int=%d; pos=%d", beg_int, pos);
        tmp_pos = pos;
        sumh = 0;
        for (int i = beg_int; i <= pos; ++i)
            sumh += his[i];

        ave_h = sumh / (pos - beg_int + 1);
        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "ave_h=%d", ave_h);

        if (ave_h <= midh) { /*интервал найден, ищем его конец*/
            while ((int16_t) his[++pos] <= maxh) {
                sumh += his[pos];
                ave_h = sumh / (pos - beg_int + 1);

                if (ave_h > midh || pos >= len - min_col)
                    break; /*Обнаружен конец интервала*/
            }

            end_int = pos;
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "end_int=%d", end_int);
#ifdef alDebug

            if (reg == VER) {
                tagRECT rct;
                SetRect(&rct, bnd->left, tmp_pos + Home, bnd->right, tmp_pos + Home);
                pTheGeomTemp.push_back(rct);//~
            } else {
                tagRECT rct;
                SetRect(&rct, tmp_pos + Home, bnd->up, tmp_pos + Home, bnd->down);
                pTheGeomTemp.push_back(rct);
            }
#endif

            if (pos < len - min_col) { //не тривиальный интервал - т.е. еще не правая граница
                Beg[++k_int] = beg_int + Home;
                End[k_int] = end_int + Home;

                //Фиксируем интервал лишь в случае достаточной длины колонки
                if ((k_int > 0 && Beg[k_int] - End[k_int - 1] < min_col) || (Beg[k_int] - Home
                        < min_col)) {
                    --k_int;
                    cf::FMT_DBG(cf::FormatDebug::INTERVAL,
                            " колонка узкая,интервал не принимается! ");
                    continue;
                }

                last_real_line = end_int;

                if (k_int > *NumMax - 2) {
                    int16_t MaxOld = *NumMax;
                    *NumMax = (int16_t) (*NumMax * 1.5);

                    if ((Beg = (int16_t*) realloc(Beg, *NumMax * sizeof(int16_t))) == NULL || (End
                            = (int16_t*) realloc(End, *NumMax * sizeof(int16_t))) == NULL) {
                        free(his);

                        if (reg == VER) {
                            free(his_first_group);
                            free(his_second_group);
                        }

                        return -5;
                    }
                }
            }
        }

        pos += min_col; /*для попадания внутрь колонки,если end_pos - внутри интервала*/
    } while (pos < len - min_int - min_col);

    free(his);

    if (reg == VER) {
        free(his_first_group);
        free(his_second_group);
    }

    *beg1 = Beg;
    *end1 = End;
    *k_int1 = k_int;
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "k_int=%d", k_int);
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "=== end SearchInterval1===");
    return k_int >= 0 ? 1 : 0;
}

//////////////        Check_IsItFalseHorLine
int16_t Check_IsItFalseHorLine(int16_t last_real_line, int16_t reg, FRAME **frm, int16_t *his,
        int16_t pos, int16_t len, int16_t maxh, int16_t sum, int16_t len_group,
        int16_t *his_first_group, int16_t *his_second_group, BOUND *bnd, int16_t k_frm) {
    int16_t Home, Fin, old_pos, i, j, mi, ma, beg_white_int, end_white_int, k_frm_first,
            k_frm_second;
    std::vector<tagRECT> First_Group;
    std::vector<tagRECT> Second_Group;

    if (reg == HOR || len_group <= 0)
        goto end1;

    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "===begin Check_IsItFalseHorLine===");
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "pos=%d len=%d last_real_line=%d", pos, len,
            last_real_line);

    if (last_real_line > 0)
        --last_real_line;

    Home = bnd->left;
    Fin = bnd->right;
    ///////////////////        First_Group             /////////////////////////////////////
    for (int i = 0; i <= k_frm; ++i) {
        if (frm[i]->up >= last_real_line + bnd->up && frm[i]->down <= pos + bnd->up) {
            tagRECT rct;
            SetRect(&rct, frm[i]->left, frm[i]->up, frm[i]->right, frm[i]->down);
            First_Group.push_back(rct);
        }
    }
    k_frm_first = First_Group.size() - 1;
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, " new_count_frm-first=%d ", k_frm_first + 1);
    memset(his_first_group, 0, (len_group + 1) * sizeof(int16_t));
    for (int i = 0; i <= k_frm_first; ++i) {
        mi = First_Group[i].left - Home;
        ma = First_Group[i].right - Home;

        if (mi < 0 || ma > len_group) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "!!!!!! mi < 0 || ma > len First_Group ");
            goto end1;
        }

        for (int j = mi; j <= ma; ++j)
            ++his_first_group[j];
    }
    ////////////////////////// Second_Group   ////////////////////////////////////////////////
    old_pos = pos;

    while (pos <= len && (int16_t) his[++pos] > maxh)
        ;

    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "Second_Group new pos = %d", pos);
    for (i = 0; i <= k_frm; ++i) {
        if (frm[i]->up >= old_pos + bnd->up && frm[i]->down <= pos + bnd->up) {
            tagRECT rct;
            SetRect(&rct, frm[i]->left, frm[i]->up, frm[i]->right, frm[i]->down);
            Second_Group.push_back(rct);
        }
    }
    k_frm_second = Second_Group.size() - 1;
    cf::FMT_DBG(cf::FormatDebug::INTERVAL, "Second_Group new_count_frm=%d ", k_frm_second + 1);
    memset(his_second_group, 0, (len_group + 1) * sizeof(int16_t));
    for (i = 0; i <= k_frm_second; ++i) {
        mi = Second_Group[i].left - Home;
        ma = Second_Group[i].right - Home;

        if (mi < 0 || ma > len_group) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "!!!!!! mi < 0 || ma > len Second_Group");
            goto end1;
        }

        for (j = mi; j <= ma; ++j)
            ++his_second_group[j];
    }
    /////////  Compary white intervals //////////////////////////////
    for (i = 0; i <= len_group; ++i) {
        while (i <= len_group && (int16_t) his_first_group[++i] <= maxh)
            ;

        while (i <= len_group && (int16_t) his_first_group[++i] > maxh)
            ;

        if (i >= len_group)
            break;

        while (i <= len_group && (int16_t) his_first_group[++i] > maxh)
            ;

        beg_white_int = i;
        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "First Group beg_white_int=%d ", i);

        while (i <= len_group && (int16_t) his_first_group[++i] <= maxh)
            ;

        end_white_int = i;

        if (end_white_int >= len_group)
            break;

        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "First Group end_white_int=%d ", i);

        if (beg_white_int == end_white_int) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "beg_white_int==end_white_int i=%d ", i);
            goto end1;
        }

        if (!check_white_int(beg_white_int, end_white_int, maxh, his_second_group)) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "===Can't find white interval->First Group===");
            goto end0;
        }
    }
    for (i = 0; i <= len_group; ++i) {
        while (i <= len_group && (int16_t) his_second_group[++i] <= maxh)
            ;

        while (i <= len_group && (int16_t) his_second_group[++i] > maxh)
            ;

        if (i >= len_group)
            break;

        while (i <= len_group && (int16_t) his_second_group[++i] > maxh)
            ;

        beg_white_int = i;
        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "Second Group beg_white_int=%d ", i);

        while (i <= len_group && (int16_t) his_second_group[++i] <= maxh)
            ;

        end_white_int = i;

        if (end_white_int >= len_group)
            break;

        cf::FMT_DBG(cf::FormatDebug::INTERVAL, "Second Group end_white_int=%d ", i);

        if (beg_white_int == end_white_int) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL,
                    "Second Group beg_white_int==end_white_int i=%d ", i);
            goto end1;
        }

        if (!check_white_int(beg_white_int, end_white_int, maxh, his_first_group)) {
            cf::FMT_DBG(cf::FormatDebug::INTERVAL, "===Second Group Can't find white interval===");
            goto end0;
        }
    }
    end1: return 1;
    end0: return 0;
}

int check_white_int(int16_t beg_white_int, int16_t end_white_int, int16_t maxh,
        int16_t *his_second_group) {
    int count_white_pixels = 0;
    for (int i = beg_white_int; i <= end_white_int; ++i) {
        if (his_second_group[++i] <= maxh)
            ++count_white_pixels;
    }

    return (count_white_pixels >= 2) ? TRUE : FALSE;
}

/*Добавить узел в дерево после дочернего эл-та after узла-родителя up:
 если after=NULL, добавляем в начало списка дочерей узла up,
 если up=NULL, создаем корень дерева,
 up - узел-родитель по дереву,
 after - узел-дочь, после кот. следует вставить новый узел,
 free - голова общего списка свобод. памяти дерева*/
//============return: указатель на новый включен. в дерево узел или NULL
KNOTT* IncKnot(KNOTT *up, KNOTT *after, KNOTT **free) {
    KNOTT *New;

    if (up != NULL) {
        if (after == NULL)
            New = (KNOTT*) inc_lst((KNOT**) &up->down, (KNOT**) free);
        else
            New = (KNOTT*) inc_after_lst((KNOT*) after, (KNOT**) &up->down, (KNOT**) free);

        if (New == NULL)
            return New;
    } else {
        New = NULL;
        New = (KNOTT*) inc_lst((KNOT**) &New, (KNOT**) free);//корень
    }

    New->up = up;
    New->down = NULL;
    return New;
}
//===Заполнение полей узла KNOTT ортогонал. дерева TREE2
void FillFieldKNOTT1(KNOTT *ptr, int16_t Left, int16_t Right, int16_t Top, int16_t Bottom,
        int16_t InBegFrm, int16_t NumFrm, int16_t InColA, uint OrderChild, uint Type,
        uint AllowOCR, uint JustH, uint JustV, char *Name) {
    ptr->Rect.top = Top; //индекс линии - верхней границы
    ptr->Rect.bottom = Bottom; //--/-- нижней
    ptr->Rect.left = Left; //--/-- левой
    ptr->Rect.right = Right; //--/-- правой
    ptr->InBegFrm = InBegFrm; //индекс начал. фрагмента колонки
    ptr->NumFrm = NumFrm; //число фрагментов колонки (фрагменты одной колонки идут в массиве frm подряд)
    ptr->InColA = InColA; //индекс первого фрагмента данной колонки
    ptr->OrderChild = OrderChild; //порядок дочерей узла
    ptr->Type = Type; //
    ptr->AllowOCR = AllowOCR; //
    ptr->JustH = JustH; //
    ptr->JustV = JustV; //
    ptr->Name = Name; //
    ptr->RefOrt = ptr->RefH = NULL; //
}

int16_t compINDEX_SORT1(INDEX_SORT *a, INDEX_SORT *b) {
    return (a->value >= b->value ? 1 : -1);
}

int16_t compLINE_KNOT1(LINE_KNOT *a, LINE_KNOT *b) {
    return (a->beg >= b->beg ? 1 : -1);
}

//сортировка H-линий дерева по вертикали
//и попутное заполнение массива терминал. V-колонок (k_colt1 - индекс)
//если V-линии не упорядочены, они также сортируютс
int16_t SortHorLine1(LINE_KNOT *LineHK, int16_t NumH, LINE_KNOT *LineVK, int16_t NumV, KNOTT *Root,
        KNOTT ***colt1, int16_t *k_colt1, FRAME **frm) {
    INDEX_SORT *Ind = (INDEX_SORT *) malloc(MAX(NumH, NumV) * sizeof(INDEX_SORT));
    STAT_CELL StDefault;
    int16_t *Index = (int16_t *) malloc(NumH * sizeof(int16_t)), *IndexV = (int16_t *) malloc(NumV
            * sizeof(int16_t));
    int16_t i, DepthTree = 20, flV;
    STACK St;
    KNOTT *Curr, **colt;
    int16_t k_colt;
    StDefault.dyLow = NORM_SCAN(20);

    //--сортировка H-линий--
    for (i = 0; i < NumH; ++i) {
        Ind[i].ind = i;
        Ind[i].value = LineHK[i].beg;
    }

    u4sort(Ind, NumH, sizeof(INDEX_SORT), (COMP_FUN) compINDEX_SORT1);
    u4sort(LineHK, NumH, sizeof(LINE_KNOT), (COMP_FUN) compLINE_KNOT1);

    for (i = 0; i < NumH; ++i)
        Index[Ind[i].ind] = i;

    //--сортировка V-линий--
    flV = 0;

    for (i = 0; i < NumV; ++i) {
        Ind[i].ind = i;
        Ind[i].value = LineVK[i].beg;

        if (i && LineVK[i - 1].beg > LineVK[i].beg)
            flV = 1;
    }

    if (flV) { //V-линии неупорядочены
        u4sort(Ind, NumV, sizeof(INDEX_SORT), (COMP_FUN) compINDEX_SORT1);
        u4sort(LineVK, NumV, sizeof(LINE_KNOT), (COMP_FUN) compLINE_KNOT1);
    }

    for (i = 0; i < NumV; ++i)
        IndexV[Ind[i].ind] = i;

    free(Ind);

    if (NewStack(DepthTree, &St)) //создать стек для обхода дерева
        return NOT_ALLOC;

    Curr = Root;
    k_colt = 0;

    while (Curr != NULL) { //I проход TREE-перенумерация H-линий и расчет числа TERM Cell
        Curr->Rect.top = Index[Curr->Rect.top];
        Curr->Rect.bottom = Index[Curr->Rect.bottom];
        Curr->Rect.left = IndexV[Curr->Rect.left];
        Curr->Rect.right = IndexV[Curr->Rect.right];

        if (Curr->NumFrm > 0)
            ++k_colt;

        Curr = NextKnot(Curr, &St); //перейти к следующему узлу

        if (OverflowStack(&St)) //проверка переполнения стека
            return NOT_ALLOC;
    }

    if ((colt = (KNOTT**) malloc(k_colt * sizeof(KNOTT*))) == NULL)
        return NOT_ALLOC;

    ClearStack(&St);//Для повтор.прохода по дереву чистим стек
    Curr = Root;
    k_colt = -1;
#define VER_ORDER_QUE_STR
#ifdef VER_ORDER_QUE_STR

    while (Curr != NULL) { //II проход - заполнение массива TERM Cell
        if (Curr->down == NULL && Curr->AllowOCR && Curr->InBegFrm != IN_NO && Curr->NumFrm > 0)
            colt[++k_colt] = Curr;

        Curr = NextKnot(Curr, &St);
    }

#else

    while (Curr != NULL) {
        if (Curr->InBegFrm == IN_NO && Curr->down == NULL) { //Терм. H-узел
            while (CurrV) {
                if (CurrV->down == NULL && CurrV->AllowOCR && CurrV->InBegFrm != IN_NO &&
                        CurrV->NumFrm > 0) colt[++k_colt] = CurrV;

                CurrV = CurrV->RefH;
            }
        }

        Curr = NextKnot(Curr, &St);
    }

#endif
    DelStack(&St);
    *colt1 = colt;
    *k_colt1 = k_colt;
    free(Index);
    free(IndexV);
    return 0;
}

struct COLH
{
        SRECT bnd;/*рамка объединения*/
};

int16_t CalcNumDau(KNOTT *Knot)
{
    if (!Knot) {
        std::cerr << "Error #1: ED_RTF\n";
        return 0;
    }

    int16_t n = 0;

    for (KNOTT * ptr = Knot->down; ptr; ptr = ptr->next)
        n++;

    return n;
}

//Return:
//0 - OK.
// Режим работы USE_FRAME_AND_COLUMN
//=========Разбиение страницы на сектора
//         Секторы на горизантальные колонки
//         Горизантальные колонки на вертикальные колонки
//         Определение терминальности вертикальных колонок
//  Заполнение RtfPage.
Bool PageTree(FILE *InFileName, cf::CRtfPage* RtfPage) {
    int16_t nc, ns, nw, nz, fl, i, i_ns1, i_nsb, i_nse, j, ih, iv, iv1, kp, kp1, kp2, n_beg,
            flag_vse_term = 1, OldNumCol;
    int16_t FlagBadColumn;
    SRECT bnd;
    int16_t ***Colt, K_Sect, *K_Hor, **K_Ver_Flag_Term, **K_Ver_Add_On, **K_Ver_Offset, **K_Ver;
    COLH **ColH, **ColH_New;
    INF_TREE Inf;
    FRAME **frm;
    Bool FlagBadBad = FALSE;
    KNOTT *RootUdal = NULL;
    cf::CRtfSector* pRtfSector;
    cf::CRtfHorizontalColumn* pRtfHorizontalColumn;
    cf::CRtfVerticalColumn* pRtfVerticalColumn;
    cf::CRtfFragment* pRtfFragment;
    cf::CRtfString* pRtfString;
    cf::CRtfWord* pRtfWord;
    cf::CRtfChar* pRtfChar;
    SETUP_GENERATE_TREE setup;
    Inf.Tree.Root = (KNOTT*) malloc(sizeof(KNOTT));
    KNOTT *pRoot = Inf.Tree.Root, *ptr, *ptr1, *ptr2;
    RootUdal = pRoot;
    ArrFrm = NULL;
    frm = NULL;
    K_Hor = NULL;
    K_Ver = NULL;
    K_Ver_Flag_Term = NULL;
    K_Ver_Add_On = NULL;
    K_Ver_Offset = NULL;
    Colt = NULL;
    ColH = NULL;
    ColH_New = NULL;
    //DBG("Begin FileName=" << OutFileName);
#ifdef alDebug
    FlagGraphic1 = 0;
#endif
    //settings for GenFullTxtfromTree
    par_ful.AllowSpell = 0;
    par_ful.KodNoRecogOut = '~';
    Inf.TypeDoc = PLAIN;
    FlagOdinSectorOdnaColonka = TRUE;
    //--Инициализация системы:чтение internal-файла--
    FlagBadColumn = 0;

    if (!OpenFullOutTiger(InFileName)) {
#ifdef alDebug
        free((KNOT**) Inf.Tree.Root);

        cf::FMT_DBG("Formatter End ");
#endif
        return TRUE;
    }

    cf::FMT_DBG("OpenFullOutTiger");

    if (NumCol >= 0)
        fl = CalcStatTiger();
    else
        fl = 1;

    setup.size_x = setup.size_y = SizeYGlob;
    OldNumCol = NumCol;
    ++NumCol;

    // Форматирование текстовых фрагментов
    if (fl = GenerateTreeByFragm(RectFragm, NumCol, &setup, &frm, &Inf)) {
        --NumCol;
        FlagBadBad = TRUE;
        goto BadReturn;
    }

    cf::FMT_DBG("GenerateTreeByFragm ");

    //---объединяем результаты распознавания текстовых фрагментов в колонки---
    if (Inf.NumT)
        FlagOdinSectorOdnaColonka = FALSE;

    for (i = 0; i <= Inf.NumT; ++i) {
        int16_t num, inCol, nc, ns, r, l, t, b, TotalNumStr, m;
        l = Inf.bnd_col[i].left;
        r = Inf.bnd_col[i].right;
        t = Inf.bnd_col[i].up;
        b = Inf.bnd_col[i].down;
        n_beg = Inf.ColT[i]->InBegFrm;
        num = Inf.ColT[i]->NumFrm;
        Inf.ColT[i]->Type = 1;
        Inf.ColT[i]->InBegFrm = (int) frm[n_beg]->start_pos;
        inCol = Inf.ColT[i]->InBegFrm;

        if (num > 1) {
            std::sort((frm + n_beg), (frm + n_beg + num), pageTreeFrameSortFunction);

            for (j = 0; j < num - 1; ++j) {
                if (frm[n_beg + j]->down > frm[n_beg + j + 1]->up) {
                    FlagOdinSectorOdnaColonka = FALSE;
                    break;
                }
            }

            for (j = 0; j < num - 1; ++j) {
                if (frm[n_beg + j]->down - 30 > frm[n_beg + j + 1]->up) {
                    Inf.ColT[i]->Type = 0;
                    Inf.ColT[i]->InBegFrm = n_beg;
                    break;
                }
            }

            if (Inf.ColT[i]->Type)
                inCol = Inf.ColT[i]->InBegFrm = (int) frm[n_beg]->start_pos;
        }

        cf::FMT_DBG(cf::FormatDebug::FRAGMENT, "beg=%d num=%d inCol=%d", n_beg, num, (NumCol
                - inCol));
#ifdef alDebug

        if (num > 1 && !Inf.ColT[i]->Type) {
            cf::FMT_DBG(" Фрагмент не отсортирован !!! ");
            cf::FMT_DBG("********* end multiframe ********");
        }

#endif

        //многофрагментная колонка => объединяем строки фрагментов
        if (num > 1 && (Inf.ColT[i]->Type || (FlagOdinSectorOdnaColonka && !Inf.NumT))) {
            TotalNumStr = 0;
            m = 0;
            RectFragm[inCol].left = Inf.bnd_col[i].left;
            RectFragm[inCol].right = Inf.bnd_col[i].right;
            RectFragm[inCol].top = Inf.bnd_col[i].up;
            RectFragm[inCol].bottom = Inf.bnd_col[i].down;

            for (j = 0; j < num; ++j) {
                nc = (int16_t) frm[n_beg + j]->start_pos;
                TotalNumStr += NumStr[nc] + 1;
            }

            TITLE_STR *tS;
            TITLE_WORD **tW;
            ZN ***Z;
            tS = (TITLE_STR*) Submalloc(TotalNumStr * sizeof(TITLE_STR), &SubZn);
            tW = (TITLE_WORD**) Submalloc(TotalNumStr * sizeof(PTR), &SubZn);
            Z = (ZN***) Submalloc(TotalNumStr * sizeof(PTR), &SubZn);

            if (!tS || !tW || !Z)
                return NOT_ALLOC;

            for (j = 0; j < num; ++j) {
                nc = (int16_t) frm[n_beg + j]->start_pos; //прибавить строки колонки nc к inCol

                if (j)
                    TitleStr[nc][0].S_Attr = 1;

#ifdef alDebug
                cf::FMT_DBG("nc= %d", NumCol - nc);

#endif

                for (ns = 0; ns <= NumStr[nc]; ++ns) {
                    tS[m] = TitleStr[nc][ns];
                    tW[m] = TitleWord[nc][ns];
                    Z[m] = Zn[nc][ns];
                    m++;
                }

                NumStr[nc] = -1; //для неосвобождения повторно их памяти или уплотнени
            }

            if (FlagOdinSectorOdnaColonka)
                inCol = 0;

            TitleStr[inCol] = tS;
            TitleWord[inCol] = tW;
            Zn[inCol] = Z;
            NumStr[inCol] = TotalNumStr - 1;
            cf::FMT_DBG("********* end multiframe ********");
        }
    }

    pRoot = Inf.Tree.Root;
    Inf.StatCell = (STAT_CELL*) malloc(NumCol * sizeof(STAT_CELL));
    Inf.StatCell[0].HeiStr = 42;//~т.к. из ED-файла приходит неверные значени
    Inf.MonoSpaceTrue = 22;//~
    Inf.k_str = NumStr;
    //---Заполнение структуры колонок K_Sect,K_Hor,K_Ver,Colt,ColH---***********************
    k_lev = MaxLev - 1;
    --NumCol;

    if (FlagOdinSectorOdnaColonka) {
        OldNumCol = NumCol;
        NumCol = 0;
    }

    //converter from Inf, где лежит дерево, в трехуровневую uint16_t-схему
    //представления иерархии колонок:
    // K_Sect
    //      K_Hor[K_Sect]
    //          K_Ver[K_Sect][K_Hor[K_Sect]]
    // K_Ver_Flag_Term указывает свойства колонки :
    // 0----простая колонка; 1----простая колонка c терм. фрагментами; 2----- сложная колонка
    {
        k_col[1] = CalcNumDau(pRoot) - 1;

        /*****************     Начал. порядок - горизон. *********************************/
        if (pRoot->OrderChild == HOR) {
            cf::FMT_DBG(cf::FormatDebug::GEOMETRY, "Original horizontal order");
            cf::FMT_DBG(cf::FormatDebug::GEOMETRY, "Columns number =%d", k_col[1] + 1);
            K_Sect = 0;
            K_Hor = (int16_t*) malloc((K_Sect + 1) * sizeof(int16_t));
            K_Hor[0] = k_col[1];
            K_Ver = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
            K_Ver_Flag_Term = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
            Colt = (int16_t***) malloc((K_Sect + 1) * sizeof(int16_t**));

            if (K_Hor == NULL || K_Ver_Flag_Term == NULL || K_Ver == NULL || Colt == NULL)
                return NOT_ALLOC;

            if ((K_Ver[0] = (int16_t*) malloc((K_Hor[0] + 1) * sizeof(int16_t))) == NULL
                    || (K_Ver_Flag_Term[0] = (int16_t*) malloc((K_Hor[0] + 1) * sizeof(int16_t)))
                            == NULL || (Colt[0] = (int16_t**) malloc((K_Hor[0] + 1) * sizeof(PTR)))
                    == NULL)
                return NOT_ALLOC;

            for (ih = 0, ptr = pRoot->down; ih <= K_Hor[0]; ++ih, ptr = ptr->next) {
                K_Ver_Flag_Term[0][ih] = 0;
                iv = 0;
                kp = CalcNumDau(ptr) - 1;
                cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #Col=%d", ih + 1);

                if (kp < 0) { //Терминал.колонка
                    if (ptr->NumFrm > 1 && !ptr->Type) {
                        K_Ver[0][ih] = ptr->NumFrm - 1;
                        K_Ver_Flag_Term[0][ih] = 2;
                    } else
                        K_Ver[0][ih] = 0;

                    if ((Colt[0][ih] = (int16_t*) malloc((K_Ver[0][ih] + 1) * sizeof(int16_t)))
                            == NULL)
                        return NOT_ALLOC;

                    Get_all_term_fragms1(ptr, Colt[0][ih], &iv, NumCol, frm);
#ifdef alDebug

                    if (ptr->NumFrm > 1 && !ptr->Type)
                    cf::FMT_DBG("Колонка сложной структуры (фреймы) ");

#endif
                } else {
                    K_Ver[0][ih] = ptr->NumFrm - 1;

                    if ((Colt[0][ih] = (int16_t*) malloc((K_Ver[0][ih] + 1) * sizeof(int16_t)))
                            == NULL)
                        return NOT_ALLOC;

                    cf::FMT_DBG(cf::FormatDebug::GEOMETRY, "Выделяем память для %d term fragm",
                            ptr->NumFrm);

                    for (iv1 = 0, iv = 0, ptr1 = ptr->down; iv1 <= kp; ++iv1, ptr1 = ptr1->next) {
                        //Поиск терминальных фрагментов
                        kp1 = CalcNumDau(ptr1) - 1;

                        if (kp1 >= 0) {
                            Get_all_term_fragms(ptr1, Colt[0][ih], &iv, NumCol, frm);
                            K_Ver_Flag_Term[0][ih] = 2;
                        } else {
                            if (ptr1->NumFrm > 1 && !ptr1->Type) {
                                K_Ver_Flag_Term[0][ih] = 2;
                                cf::FMT_DBG(cf::FormatDebug::GEOMETRY,
                                        ">>> %d fragments not sorted", ptr1->NumFrm);
                                i_nse = ptr1->InBegFrm + ptr1->NumFrm;

                                for (i_nsb = ptr1->InBegFrm; i_nsb < i_nse; ++iv, ++i_nsb) {
                                    Colt[0][ih][iv] = (int16_t) frm[i_nsb]->start_pos;
                                    cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d", NumCol
                                            + 1 - Colt[0][ih][iv]);
                                }
                            } else {
                                if (!K_Ver_Flag_Term[0][ih])
                                    K_Ver_Flag_Term[0][ih] = 1;

                                Colt[0][ih][iv] = ptr1->InBegFrm;
                                cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d", NumCol + 1
                                        - Colt[0][ih][iv]);
                                iv++;
                            }
                        }
                    }

                    K_Ver[0][ih] = --iv;
#ifdef alDebug
                    cf::FMT_DBG("Кол-во терм. колонок=%d", (K_Ver[0][ih] + 1));
#endif
                }

#ifdef alDebug

                if (!K_Ver_Flag_Term[0][ih])
                cf::FMT_DBG("Column is simple");
                else if (K_Ver_Flag_Term[0][ih] == 1)
                cf::FMT_DBG("Column is simple and consist of terminal fragments");
                else
                cf::FMT_DBG("Column is complicated (frames)");

#endif
            }
        }

        /*****************     Начал. порядок - вертикал. *********************************/
        else if (pRoot->OrderChild == VER) {
            cf::FMT_DBG(cf::FormatDebug::GEOMETRY, "Original order - vertical");
            cf::FMT_DBG(cf::FormatDebug::GEOMETRY, "Section number =%d", k_col[1] + 1);
            K_Sect = k_col[1];
            K_Hor = (int16_t*) malloc((K_Sect + 1) * sizeof(int16_t));
            K_Ver = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
            K_Ver_Flag_Term = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
            Colt = (int16_t***) malloc((K_Sect + 1) * sizeof(int16_t**));

            if (K_Hor == NULL || K_Ver_Flag_Term == NULL || K_Ver == NULL || Colt == NULL)
                return NOT_ALLOC;

            //******  Цикл по секциям,внутри каждой - до 2-уровневой иерархии ***********
            for (i = 0, ptr = pRoot->down; i <= K_Sect; ++i, ptr = ptr->next) {
                kp = CalcNumDau(ptr) - 1;
                cf::FMT_DBG(cf::FormatDebug::GEOMETRY,
                        "***Section #%d - from %d horizontal columns", i + 1, kp < 0 ? 1 : kp + 1);

                //Секция - одна терминал. колонка
                if (kp < 0) {
                    K_Hor[i] = 0;
                    K_Ver[i] = (int16_t*) malloc((K_Hor[i] + 1) * sizeof(int16_t));
                    K_Ver_Flag_Term[i] = (int16_t*) malloc((K_Hor[i] + 1) * sizeof(int16_t));
                    Colt[i] = (int16_t**) malloc((K_Hor[i] + 1) * sizeof(int16_t*));

                    if (K_Ver[i] == NULL || K_Ver_Flag_Term[i] == NULL || Colt[i] == NULL)
                        return NOT_ALLOC;

                    if (ptr->NumFrm > 1 && !ptr->Type) {
                        K_Ver[i][0] = ptr->NumFrm - 1;
                        K_Ver_Flag_Term[i][0] = 2;
                    } else {
                        K_Ver[i][0] = 0;
                        K_Ver_Flag_Term[i][0] = 0;
                    }

                    if ((Colt[i][0] = (int16_t*) malloc((K_Ver[i][0] + 1) * sizeof(int16_t)))
                            == NULL)
                        return -3;

                    if (ptr->NumFrm > 1 && !ptr->Type) {
                        cf::FMT_DBG(cf::FormatDebug::GEOMETRY,
                                ">>> %d не отсортированных фрагмента", ptr->NumFrm);
                        i_nse = ptr->InBegFrm + ptr->NumFrm;

                        for (i_ns1 = 0, i_nsb = ptr->InBegFrm; i_nsb < i_nse; ++i_ns1, ++i_nsb) {
                            Colt[i][0][i_ns1] = (int16_t) frm[i_nsb]->start_pos;
                            cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d", (NumCol + 1
                                    - Colt[i][0][i_ns1]));
                        }

                        cf::FMT_DBG(cf::FormatDebug::GEOMETRY,
                                "Колонка сложной структуры (фреймы) ");
                    } else {
                        Colt[i][0][0] = ptr->InBegFrm;
                        cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d", NumCol + 1
                                - Colt[i][0][0]);
                    }
                }

                //Секция - из kp H-колонок
                else {
                    K_Hor[i] = kp;
                    K_Ver[i] = (int16_t*) malloc((K_Hor[i] + 1) * sizeof(int16_t));
                    K_Ver_Flag_Term[i] = (int16_t*) malloc((K_Hor[i] + 1) * sizeof(int16_t));
                    Colt[i] = (int16_t**) malloc((K_Hor[i] + 1) * sizeof(int16_t*));

                    if (K_Ver[i] == NULL || K_Ver_Flag_Term[i] == NULL || Colt[i] == NULL)
                        return NOT_ALLOC;

                    //************  цикл по H-дочерям секции ****************************
                    for (ih = 0, ptr1 = ptr->down; ih <= kp; ++ih, ptr1 = ptr1->next) {
                        kp1 = CalcNumDau(ptr1) - 1;//Число дочерей H-col
                        K_Ver_Flag_Term[i][ih] = 0;
                        cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #Col=%d", ih + 1);

                        if (kp1 < 0) { //Терм. H-col
                            if (ptr1->NumFrm > 1 && !ptr1->Type) {
                                K_Ver[i][ih] = ptr1->NumFrm - 1;
                                K_Ver_Flag_Term[i][ih] = 2;
                            } else {
                                K_Ver[i][ih] = 0;
                                K_Ver_Flag_Term[i][ih] = 0;
                            }

                            if ((Colt[i][ih] = (int16_t*) malloc((K_Ver[i][ih] + 1)
                                    * sizeof(int16_t))) == NULL)
                                return -3;

                            if (ptr1->NumFrm > 1 && !ptr1->Type) {
                                cf::FMT_DBG(cf::FormatDebug::GEOMETRY,
                                        ">>>  %d не отсортированных фрагмента", ptr1->NumFrm);

                                i_nse = ptr1->InBegFrm + ptr1->NumFrm;

                                for (i_ns1 = 0, i_nsb = ptr1->InBegFrm; i_nsb < i_nse; ++i_ns1, ++i_nsb) {
                                    Colt[i][ih][i_ns1] = (int16_t) frm[i_nsb]->start_pos;
                                    cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d", NumCol
                                            + 1 - Colt[i][ih][i_ns1]);
                                }
                            } else {
                                Colt[i][ih][0] = ptr1->InBegFrm;
                                cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=5d", (NumCol + 1
                                        - Colt[i][ih][0]));
                            }
                        } else {
                            K_Ver[i][ih] = ptr1->NumFrm - 1;

                            if ((Colt[i][ih] = (int16_t*) malloc((K_Ver[i][ih] + 1)
                                    * sizeof(int16_t))) == NULL)
                                return -3;

                            cf::FMT_DBG(" Выделяем память для %d  term fragm", ptr1->NumFrm);

                            for (iv1 = 0, iv = 0, ptr2 = ptr1->down; iv1 <= kp1; ++iv1, ptr2
                                    = ptr2->next) {
                                //Поиск терминальных фрагментов
                                kp2 = CalcNumDau(ptr2) - 1;

                                if (kp2 >= 0) {
                                    Get_all_term_fragms(ptr2, Colt[i][ih], &iv, NumCol, frm);
                                    K_Ver_Flag_Term[i][ih] = 2;
                                } else {
                                    if (ptr2->NumFrm > 1 && !ptr2->Type) {
                                        K_Ver_Flag_Term[i][ih] = 2;
                                        cf::FMT_DBG(cf::FormatDebug::GEOMETRY,
                                                ">>> %d не отсортированных фрагмента", ptr2->NumFrm);
                                        i_nse = ptr2->InBegFrm + ptr2->NumFrm;

                                        for (i_nsb = ptr2->InBegFrm; i_nsb < i_nse; ++iv, ++i_nsb) {
                                            Colt[i][ih][iv] = (int16_t) frm[i_nsb]->start_pos;
                                            cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d",
                                                    NumCol + 1 - Colt[i][ih][iv]);
                                        }
                                    } else {
                                        if (!K_Ver_Flag_Term[i][ih])
                                            K_Ver_Flag_Term[i][ih] = 1;

                                        Colt[i][ih][iv] = ptr2->InBegFrm;
                                        cf::FMT_DBG(cf::FormatDebug::GEOMETRY, " #term=%d",
                                                NumCol + 1 - Colt[i][ih][iv]);
                                        iv++;
                                    }
                                }
                            }

                            K_Ver[i][ih] = --iv;
#ifdef alDebug
                            cf::FMT_DBG("Кол-во терм. колонок=%d", K_Ver[i][ih] + 1);

#endif
                        }

#ifdef alDebug

                        if (!K_Ver_Flag_Term[i][ih])
                        cf::FMT_DBG("Колонка простая");
                        else if (K_Ver_Flag_Term[i][ih] == 1)
                        cf::FMT_DBG("Колонка простая и состоит из терм.фраг-тов");
                        else
                        cf::FMT_DBG("Колонка сложной структуры (фреймы) ");

#endif
                    }

                    //************ Конец цикла по H-дочерям секции ****************************
                }
            }
        } else {
            if (!NumCol) { //090899 update
                K_Sect = 0;
                K_Hor = (int16_t*) malloc((K_Sect + 1) * sizeof(int16_t));
                K_Hor[0] = 0;
                K_Ver = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
                K_Ver_Flag_Term = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
                Colt = (int16_t***) malloc((K_Sect + 1) * sizeof(int16_t**));

                if (K_Hor == NULL || K_Ver_Flag_Term == NULL || K_Ver == NULL || Colt == NULL)
                    return NOT_ALLOC;

                if ((K_Ver[0] = (int16_t*) malloc((K_Hor[0] + 1) * sizeof(int16_t))) == NULL
                        || (K_Ver_Flag_Term[0]
                                = (int16_t*) malloc((K_Hor[0] + 1) * sizeof(int16_t))) == NULL
                        || (Colt[0] = (int16_t**) malloc((K_Hor[0] + 1) * sizeof(PTR))) == NULL)
                    return NOT_ALLOC;

                K_Ver_Flag_Term[0][0] = 0;
                K_Ver[0][0] = 0;

                if ((Colt[0][0] = (int16_t*) malloc((K_Ver[0][0] + 1) * sizeof(int16_t))) == NULL)
                    return NOT_ALLOC;

                Colt[0][0][0] = 0;
            } else {
                BadReturn: FlagBadColumn = 1;
                K_Sect = 0;
                K_Hor = (int16_t*) malloc((K_Sect + 1) * sizeof(int16_t));
                K_Hor[0] = 0;
                K_Ver = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
                K_Ver_Flag_Term = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
                Colt = (int16_t***) malloc((K_Sect + 1) * sizeof(int16_t**));

                if (K_Hor == NULL || K_Ver_Flag_Term == NULL || K_Ver == NULL || Colt == NULL)
                    return NOT_ALLOC;

                if ((K_Ver[0] = (int16_t*) malloc((K_Hor[0] + 1) * sizeof(int16_t))) == NULL
                        || (K_Ver_Flag_Term[0]
                                = (int16_t*) malloc((K_Hor[0] + 1) * sizeof(int16_t))) == NULL
                        || (Colt[0] = (int16_t**) malloc((K_Hor[0] + 1) * sizeof(PTR))) == NULL)
                    return NOT_ALLOC;

                K_Ver_Flag_Term[0][0] = 2;
                K_Ver[0][0] = NumCol;

                if ((Colt[0][0] = (int16_t*) malloc((K_Ver[0][0] + 1) * sizeof(int16_t))) == NULL)
                    return NOT_ALLOC;

                for (i = 0; i <= NumCol; ++i) {
                    Colt[0][0][i] = i;
                }
            }
        }
    }
    for (i = 0; i <= K_Sect; ++i) {
        for (ih = 0; ih <= K_Hor[i]; ++ih) {
            for (iv = 0; iv <= K_Ver[i][ih]; ++iv) {
                nc = Colt[i][ih][iv];
                RectFragm[nc].left = RectFragm[nc].left;
                RectFragm[nc].right = RectFragm[nc].right;
                RectFragm[nc].top = RectFragm[nc].top;
                RectFragm[nc].bottom = RectFragm[nc].bottom;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //  Создание массива укрупненных колонок Colh--*****************************************//
    //  преобр-ние из 3-уровневой в 2-уровневую систему колонок  (ideal size)               //
    //////////////////////////////////////////////////////////////////////////////////////////
    if ((ColH = (COLH**) malloc((K_Sect + 1) * sizeof(COLH*))) == NULL)
        return -3;

    K_Ver_Add_On = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));
    K_Ver_Offset = (int16_t**) malloc((K_Sect + 1) * sizeof(int16_t*));

    if (K_Ver_Add_On == NULL || K_Ver_Offset == NULL)
        return NOT_ALLOC;

    for (i = 0; i <= K_Sect; ++i) {
        SRECT BndTmp;

        if ((ColH[i] = (COLH*) malloc((K_Hor[i] + 1) * sizeof(COLH))) == NULL)
            return -3;

        K_Ver_Add_On[i] = (int16_t*) malloc((K_Hor[i] + 1) * sizeof(int16_t));
        K_Ver_Offset[i] = (int16_t*) malloc((K_Hor[i] + 1) * sizeof(int16_t));

        if (K_Ver_Add_On[i] == NULL || K_Ver_Offset[i] == NULL)
            return NOT_ALLOC;

        for (ih = 0; ih <= K_Hor[i]; ++ih) {
            K_Ver_Add_On[i][ih] = 0;
            K_Ver_Offset[i][ih] = 0;

            if (K_Ver_Flag_Term[i][ih] >= 2)
                flag_vse_term = 0;

            for (iv = 0; iv <= K_Ver[i][ih]; ++iv) {
                nc = Colt[i][ih][iv];

                if (!iv)
                    ConvertRect16ToBnd(&RectFragm[nc], &bnd); //!! или берем по линиям с выравниванием

                else {
                    ConvertRect16ToBnd(&RectFragm[nc], &BndTmp);
                    MyUnionRect(&bnd, &BndTmp, &bnd);
                }
            }
            ColH[i][ih].bnd.left = bnd.left;
            ColH[i][ih].bnd.right = bnd.right;
            ColH[i][ih].bnd.top = bnd.top;
            ColH[i][ih].bnd.bottom = bnd.bottom;
        }
    }
    cf::FMT_DBG("Подсчет реалных размеров кеглей ");
    /////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                     //
    //                  Подсчет реалных размеров кеглей                                    //
    //                                                                                     //
    /////////////////////////////////////////////////////////////////////////////////////////
    for (i = 0; i <= K_Sect; ++i) { //sect begin
        int index_word;
        for (ih = 0; ih <= K_Hor[i]; ++ih) {//hor. col.  begin
            for (iv = 0; iv <= K_Ver[i][ih]; ++iv) { //vert. col.  begin
                nc = Colt[i][ih][iv];

                if (NumStr[nc] < 0)
                    continue;

                RtfPage->m_arFragments.push_back(new cf::CRtfFragment());
                RtfPage->Count.RtfTextFragments++;
                j = RtfPage->m_arFragments.size();
                pRtfFragment = RtfPage->m_arFragments[j - 1];
                pRtfFragment->setType(FT_TEXT);
                pRtfFragment->m_rect.left = RectFragm[nc].left;
                pRtfFragment->m_rect.right = RectFragm[nc].right;
                pRtfFragment->m_rect.top = RectFragm[nc].top;
                pRtfFragment->m_rect.bottom = RectFragm[nc].bottom;
                //pRtfFragment->m_Flag = FragFlag [nc]; //nega_str сделать цикл и занести в массив RtfString признаки негативности
                for (ns = 0; ns <= NumStr[nc]; ++ns) { //str. begin
                    if (TitleStr[nc][ns].S_Gen.S_NumWord <= 0)
                        continue;

                    pRtfFragment->addString(new cf::CRtfString);
                    pRtfString = pRtfFragment->stringAt(ns);
                    //nega_str добавить m_Flag в RtfString и занести туда признак NEGATIVE
                    pRtfString->setFlags(TitleStr[nc][ns].S_Flags); //NEGA_STR
                    for (nw = 0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw) {//word begin
                        if (TitleWord[nc][ns][nw].W_Gen.W_NumSym == 0) {
                            continue;
                        }

                        pRtfString->addWord(new cf::CRtfWord);
                        index_word = pRtfString->wordCount();
                        pRtfWord = pRtfString->wordAt(index_word - 1);
                        pRtfWord->setIdealFontSize(((TitleWord[nc][ns][nw]).W_Gen).FontSize);
                        pRtfWord->setFontNumber(((TitleWord[nc][ns][nw]).W_Gen).FontNumber);
                        for (nz = 0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz) { //char begin
                            pRtfWord->addChar(new cf::CRtfChar);
                            pRtfChar = pRtfWord->charAt(nz);

                            for (int alt = 0; alt < Zn[nc][ns][nw][nz].Title.Z_Num_Alt && alt
                                    < REC_MAX_VERS; alt++) {
                                cf::Letter vers(Zn[nc][ns][nw][nz].Alt[alt].a_Code,
                                        Zn[nc][ns][nw][nz].Alt[alt].a_Prob);
                                pRtfChar->addVersion(vers);
                            }

                            //~ не знак переноса, а дефис в слове (пр: красно-белый)
                            pRtfChar->setSpelledNoCarrying(
                                    Zn[nc][ns][nw][nz].Alt[0].a_SpellNoCarrying);
                            pRtfChar->setDropCap(Zn[nc][ns][nw][nz].Alt[0].a_FlagCupDrop);
                            pRtfChar->setLanguage((language_t) Zn[nc][ns][nw][nz].Alt[0].a_language);
                            pRtfChar->setSpelled(Zn[nc][ns][nw][nz].Alt[0].a_Spell);
                            pRtfChar->setFont(((TitleWord[nc][ns][nw]).W_Gen).FontNumber);
                            pRtfChar->setFontSize(((TitleWord[nc][ns][nw]).W_Gen).FontSize);
                            pRtfChar->setRealRect(toRect(Zn[nc][ns][nw][nz].Title.Z_RealRect));
                            pRtfChar->setIdealRect(toRect(Zn[nc][ns][nw][nz].Title.Z_Rect));
                        }//char end
                    }//word end
                }//str end
            }//vert.end
        }//hor.end
    }//sec.end
    RtfPage->CorrectKegl(); // actually it is calculation of the kegles
    RtfPage->ChangeKegl(); // it is properly writing
    //////////////////////////////////////////////////////////////////////////////////////////
    //Новые координаты гор.колонок после пересчета реал. размеров вер.кол , параг. и строк  //
    //////////////////////////////////////////////////////////////////////////////////////////
    if ((ColH_New = (COLH**) malloc((K_Sect + 1) * sizeof(COLH*))) == NULL)
        return -3;

    for (i = 0; i <= K_Sect; ++i) {
        if ((ColH_New[i] = (COLH*) malloc((K_Hor[i] + 1) * sizeof(COLH))) == NULL)
            return -3;

        for (ih = 0; ih <= K_Hor[i]; ++ih) {
            ColH_New[i][ih].bnd.left = ColH[i][ih].bnd.left;
            ColH_New[i][ih].bnd.right = ColH[i][ih].bnd.right;
            ColH_New[i][ih].bnd.top = ColH[i][ih].bnd.top;
            ColH_New[i][ih].bnd.bottom = ColH[i][ih].bnd.bottom;
        }
    }
    //                                                                                     //
    /////////////////////////////////////////////////////////////////////////////////////////
    // Добавляем отформатированные текстовые фрагменты (Page).
    RtfPage->Count.RtfSectors = K_Sect;
    for (i = 0; i <= K_Sect; ++i) { //sect begin
        int index_word;
        RtfPage->m_arSectors.push_back(new cf::CRtfSector);
        pRtfSector = RtfPage->m_arSectors[i];
        for (ih = 0; ih <= K_Hor[i]; ++ih) {//hor. col.  begin
            pRtfHorizontalColumn = new cf::CRtfHorizontalColumn;
            pRtfSector->addColumn(pRtfHorizontalColumn);
            RtfUnionRect_CRect_SRect(&pRtfHorizontalColumn->m_rect, &ColH[i][ih].bnd);
            RtfUnionRect_CRect_CRect(&pRtfSector->m_rect, &pRtfHorizontalColumn->m_rect);
            RtfUnionRect_CRect_CRect(&RtfPage->m_rect, &pRtfSector->m_rect);
            pRtfHorizontalColumn->setType(
                    static_cast<cf::CRtfHorizontalColumn::column_t> (K_Ver_Flag_Term[i][ih]));
            for (iv = 0; iv <= K_Ver[i][ih]; ++iv) { //vert. col.  begin
                nc = Colt[i][ih][iv];

                if (NumStr[nc] < 0)
                    continue;

                if (K_Hor[i] == 0 && K_Ver[i][ih] == 0 && NumStr[nc] == 0)
                    pRtfSector->m_FlagOneString = TRUE;

                pRtfVerticalColumn = new cf::CRtfVerticalColumn;
                pRtfHorizontalColumn->addColumn(pRtfVerticalColumn);
                pRtfVerticalColumn->addFragment(new cf::CRtfFragment);
                //nega ~? м.б. [iv] вместо [0]?
                // uliss FIXME
                // this is really strange
                pRtfFragment = pRtfVerticalColumn->fragmentAt(0);
                pRtfFragment->setType(FT_TEXT);
                RtfAssignRect_CRect_Rect16(&pRtfVerticalColumn->m_rect, &RectFragm[nc]);
                RtfAssignRect_CRect_Rect16(&pRtfVerticalColumn->m_rectReal, &RectFragm[nc]);
                RtfAssignRect_CRect_Rect16(&pRtfFragment->m_rect, &RectFragm[nc]);
                RtfAssignRect_CRect_Rect16(&pRtfFragment->m_rectReal, &RectFragm[nc]);
                //pRtfFragment->m_Flag = FragFlag[nc]; //nega
                for (ns = 0; ns <= NumStr[nc]; ++ns) { //str. begin
                    if (TitleStr[nc][ns].S_Gen.S_NumWord <= 0)
                        continue;

                    pRtfFragment->addString(new cf::CRtfString);
                    pRtfString = pRtfFragment->stringAt(ns);

                    if (TitleStr[nc][ns].S_Attr) {
                        pRtfFragment->setMixed(true);
                        pRtfString->setAttributes(true);
                    } else
                        pRtfString->setAttributes(false);

                    pRtfString->setFlags(TitleStr[nc][ns].S_Flags); //NEGA_STR
                    for (nw = 0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw) { //word begin
                        if (TitleWord[nc][ns][nw].W_Gen.W_NumSym == 0) {
                            continue;
                        }

                        pRtfString->addWord(new cf::CRtfWord);
                        index_word = pRtfString->wordCount();
                        pRtfWord = pRtfString->wordAt(index_word - 1);
                        pRtfWord->setFontNumber(((TitleWord[nc][ns][nw]).W_Gen).FontNumber);
                        pRtfWord->setIdealFontSize(((TitleWord[nc][ns][nw]).W_Gen).FontSize);

                        if (NumStr[nc] == 0 && TitleStr[nc][ns].S_Gen.S_NumWord == 1)
                            pRtfWord->setRealFontSize(
                                    RtfPage->GetMinKegl(pRtfWord->idealFontSize()));
                        else
                            pRtfWord->setRealFontSize(
                                    RtfPage->GetNewKegl(pRtfWord->idealFontSize()));

                        for (nz = 0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz) {
                            pRtfWord->addChar(new cf::CRtfChar);
                            pRtfChar = pRtfWord->charAt(nz);

                            for (int alt = 0; alt < Zn[nc][ns][nw][nz].Title.Z_Num_Alt && alt
                                    < REC_MAX_VERS; alt++) {
                                cf::Letter vers(Zn[nc][ns][nw][nz].Alt[alt].a_Code,
                                        Zn[nc][ns][nw][nz].Alt[alt].a_Prob);
                                pRtfChar->addVersion(vers);
                            }

                            pRtfChar->setSpelledNoCarrying(
                                    Zn[nc][ns][nw][nz].Alt[0].a_SpellNoCarrying);
                            pRtfChar->setDropCap(Zn[nc][ns][nw][nz].Alt[0].a_FlagCupDrop);
                            pRtfChar->setLanguage((language_t) Zn[nc][ns][nw][nz].Alt[0].a_language);
                            pRtfChar->setSpelled(Zn[nc][ns][nw][nz].Alt[0].a_Spell);
                            pRtfChar->setFont(((TitleWord[nc][ns][nw]).W_Gen).FontNumber);
                            pRtfChar->setFontSize(((TitleWord[nc][ns][nw]).W_Gen).FontSize);
                            pRtfChar->setRealRect(toRect(Zn[nc][ns][nw][nz].Title.Z_RealRect));
                            pRtfChar->setIdealRect(toRect(Zn[nc][ns][nw][nz].Title.Z_Rect));
                        }
                    }//word end
                }//str end
            }//vert.end
        }//hor.end
    }//sec.end

    cf::FMT_DBG("Formatter End ");

    for (i = 0; i <= K_Sect; ++i) {
        for (ih = 0; ih <= K_Hor[i]; ++ih)
            free(Colt[i][ih]);

        free(K_Ver[i]);
        free(K_Ver_Flag_Term[i]);
        free(K_Ver_Add_On[i]);
        free(K_Ver_Offset[i]);
        free(Colt[i]);
        free(ColH[i]);
        free(ColH_New[i]);
    }

    free(K_Hor);
    free(K_Ver);
    free(K_Ver_Flag_Term);
    free(K_Ver_Add_On);
    free(K_Ver_Offset);
    free(Colt);
    free(ColH);
    free(ColH_New);
    free((KNOT**) RootUdal);
    free(ArrFrm);
    free(frm);

    if (FlagOdinSectorOdnaColonka) {
        int16_t tmpNumCol;
        tmpNumCol = NumCol;
        NumCol = OldNumCol;

        if (NumStr)
            FreeStructFull();

        NumCol = tmpNumCol;
    } else if (NumStr)
        FreeStructFull();

        free(NumStr);
        free(UserNumber);
        free(FragFlag);
        free(RectFragm);

    if (FlagBadBad)
        return TRUE;

    free((KNOT**) Inf.Tree.Root);
    free((KNOT**) Inf.Tree.ArrSeg);
    free((KNOT**) Inf.LineVK);
    free((KNOT**) Inf.LineHK);
    free((KNOT**) Inf.bnd_col);
    free((KNOT**) Inf.StatCell);
    free((KNOT**) Inf.ColT);
    return TRUE;
}

void Get_all_term_fragms1(KNOTT* ptr, int16_t* Colt, int16_t* iv, int16_t NumCol, FRAME **frm) {
    int16_t i_nsb, i_nse;

    if (ptr->NumFrm > 1 && !ptr->Type) {
#ifdef alDebug
        cf::FMT_DBG(">>> %d не отсортированных фрагмента", ptr->NumFrm);
#endif
        i_nse = ptr->InBegFrm + ptr->NumFrm;

        for (i_nsb = ptr->InBegFrm; i_nsb < i_nse; ++*iv, ++i_nsb) {
            Colt[*iv] = (int16_t) frm[i_nsb]->start_pos;
#ifdef alDebug
            cf::FMT_DBG(" #term=%d", NumCol + 1 - Colt[*iv]);
#endif
        }
    } else {
        Colt[*iv] = ptr->InBegFrm;
        ++*iv;
#ifdef alDebug
        cf::FMT_DBG(" #term=%d", NumCol + 1 - ptr->InBegFrm);
#endif
    }
}

void Get_all_term_fragms(KNOTT* ptr, int16_t* Colt, int16_t* iv, int16_t NumCol, FRAME **frm) {
    int16_t i, i1, i2, i3, kp, kp1, kp2, kp3, kp4;
    KNOTT *ptr1, *ptr2, *ptr3, *ptr4;
    kp = CalcNumDau(ptr) - 1;

    for (i = 0, ptr1 = ptr->down; i <= kp; ++i, ptr1 = ptr1->next) {
        kp1 = CalcNumDau(ptr1) - 1;//Число дочерей col

        if (kp1 < 0) //Терм. col
            Get_all_term_fragms1(ptr1, Colt, iv, NumCol, frm);
        else {
            for (i1 = 0, ptr2 = ptr1->down; i1 <= kp1; ++i1, ptr2 = ptr2->next) {
                kp2 = CalcNumDau(ptr2) - 1;

                if (kp2 < 0) //Терм. col
                    Get_all_term_fragms1(ptr2, Colt, iv, NumCol, frm);
                else {
                    for (i2 = 0, ptr3 = ptr2->down; i2 <= kp2; ++i2, ptr3 = ptr3->next) {
                        kp3 = CalcNumDau(ptr3) - 1;

                        if (kp3 < 0) //Терм. col
                            Get_all_term_fragms1(ptr3, Colt, iv, NumCol, frm);
                        else {
                            for (i3 = 0, ptr4 = ptr3->down; i3 <= kp3; ++i3, ptr4 = ptr4->next) {
                                kp4 = CalcNumDau(ptr4) - 1;

                                if (kp4 < 0) //Терм. col
                                    Get_all_term_fragms1(ptr4, Colt, iv, NumCol, frm);

#ifdef alDebug
                                else
                                cf::FMT_DBG("   Ошибка !!!   ");
#endif
                            }
                        }
                    }
                }
            }
        }
    }
}

cf::Rect toRect(const SRECT& src) {
    return cf::Rect(cf::Point(src.left, src.top), cf::Point(src.right, src.bottom));
}

cf::Rect toRect(const tagRECT& src) {
    return cf::Rect(cf::Point(src.left, src.top), cf::Point(src.right, src.bottom));
}

void RtfUnionRect_CRect_SRect(tagRECT *s1, SRECT *s2) {
    s1->left = MIN(s1->left, s2->left);
    s1->right = MAX(s1->right, s2->right);
    s1->top = MIN(s1->top, s2->top);
    s1->bottom = MAX(s1->bottom, s2->bottom);
}

//==Объединение пары рамок
void MyUnionRect(SRECT *s1, SRECT *s2, SRECT *u)
//==
{
    u->left = MIN(s1->left, s2->left);
    u->right = MAX(s1->right, s2->right);
    u->top = MIN(s1->top, s2->top);
    u->bottom = MAX(s1->bottom, s2->bottom);
}

#ifdef alDebug
void clear(void) {
    ;
}
void pause_internal(void) {
    ;
}

void image_frm(FRAME *f, int col, int line_style, int fill) {
    RECT f1;
    f1.left = f->left;
    f1.right = f->right;
    f1.top = f->up;
    f1.bottom = f->down;
}

void image_bnd(BOUND *f, int col, int line_style, int fill) {
}

void image_frame(FRAME **frm, int k, int col, int line_style, int fill) {
    for (int i = 0; i <= k; i++)
    image_frm(frm[i], col, line_style, fill);
}

void bounds_frm(int ii, FRAME **frm, int nx) {
}

void BoundsRect(int ii, RECT *frm, int nx) {
}

void image_rect(RECT *f, int col, int line_style, int fill) {
    cf::FMT_DBG(" left=%d,  right=%d,  up=%d,  down=%d", f->left, f->right, f->top, f->bottom);

    if (pTheGeomStep == pTheGeomStep1) {
        pFragRectColor.push_back(Graphic1Color);

        if (Graphic1Color == 0) {
            f->left = MAX(0, f->left - 12);
            f->top = MAX(0, f->top - 12);
            f->right += 12;
            f->bottom += 12;
        }

        if (Graphic1Color == 1) {
            f->left = MAX(0, f->left - 8);
            f->top = MAX(0, f->top - 8);
            f->right += 8;
            f->bottom += 8;
        }

        if (Graphic1Color == 2) {
            f->left = MAX(0, f->left - 4);
            f->top = MAX(0, f->top - 4);
            f->right += 4;
            f->bottom += 4;
        }
    }

    tagRECT rct;
    SetRect(&rct, f->left, f->top, f->right, f->bottom);
    pTheGeomStep.push_back(rct);
}

#endif
