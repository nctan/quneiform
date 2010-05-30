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

#include <cstring>
#include "crtfchar.h"
#include "crtfsector.h"
#include "crtfword.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "crtffunc.h"
#include "crtfstring.h"
#include "creatertf.h"
#include "frmtpict.h"
#include "rfrmtoptions.h"
#include "formatoptions.h"

// common module
#include "common/debug.h"
// ced module
#include "ced/ced.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
// cpage module
#include "cpage/cpage.h"
// cfio
#include "cfio/cfio.h"

#include "lst3_win.h"

extern uint32_t CountPict;
uchar Frmt_CharSet = (uchar) 204;
int16_t K_TwipsInInch = 1440;
uint16_t FlagWriteRtfCoordinates = 1;
char WriteRtfPageNumber[CFIO_MAX_PATH] = "1";

namespace CIF
{

RfrmtDrawPageFunction CRtfPage::draw_func_;

CRtfPage::CRtfPage() {
    Count.RtfSectors = 0;
    Count.RtfTextFragments = 0;
    Count.RtfFrameTextFragments = 0;
    Count.RtfPictureFragments = 0;
    Count.RtfTableFragments = 0;
    Count.RtfStrings = 0;
    Count.RtfWords = 0;
    Count.RtfChars = 0;
    bad_column_ = false;
    ced_page_ = NULL;
    SetRect(&m_rect, 32000, 32000, 0, 0);
    SetRect(&m_rectReal, 32000, 32000, 0, 0);
}

CRtfPage::~CRtfPage() {
    clearSectors();
    clearFragments();
}

void CRtfPage::clearFragments() {
    for (FragmentList::iterator it = m_arFragments.begin(), e = m_arFragments.end(); it != e; ++it)
        delete *it;

    m_arFragments.clear();
}

void CRtfPage::clearSectors() {
    for (SectorList::iterator it = m_arSectors.begin(), e = m_arSectors.end(); it != e; ++it)
        delete *it;

    m_arSectors.clear();
}

void CRtfPage::drawLayout() const {
    if (!draw_func_.empty())
        draw_func_(this);
}

void CRtfPage::setDrawCallback(RfrmtDrawPageFunction f) {
    draw_func_ = f;
}

void CRtfPage::setFontMonospace(const std::string& name) {
    font_monospace_ = name;
}

void CRtfPage::setFontSans(const std::string& name) {
    font_sans_ = name;
}

void CRtfPage::setFontSerif(const std::string& name) {
    font_serif_ = name;
}

void CRtfPage::setFormatOptions(const FormatOptions& opts) {
    font_sans_ = opts.sansSerifName();
    font_serif_ = opts.serifName();
    font_monospace_ = opts.monospaceName();
    language_ = opts.language();
    unrecognized_char_ = (char) opts.unrecognizedChar();
}

void CRtfPage::setImageName(const std::string& name) {
    image_name_ = name;
}

void CRtfPage::setUnrecognizedChar(char ch) {
    unrecognized_char_ = ch;
}

void CRtfPage::setFragmentsInColumn(const CRtfFragment * cur_frag) {
    assert(cur_frag);

    for (size_t i = 0; i < m_arFragments.size(); i++) {
        CRtfFragment * frag = m_arFragments[i];

        if (frag->type() == cur_frag->type() && frag->rect() == cur_frag->rect())
            frag->setInColumn(true);
    }
}
void CRtfPage::initCedPage() {
    ced_page_ = new CEDPage;
    ced_page_->setImageName(image_name_);
    ced_page_->setUnrecognizedChar(unrecognized_char_);
    ced_page_->setLanguage(language_);
    ced_page_->setPageSize(page_size_);
    ced_page_->setPageBorder(Rect(Point(MargT, MargL), Point(MargB, MargR)));

    // setting page number
    int PageNumber = 1;
    PageNumber = atoi(WriteRtfPageNumber);
    ced_page_->setPageNumber(PageNumber);

    // setting page info
    PAGEINFO PageInfo;
    Handle hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    GetPageInfo(hCPAGE, &PageInfo);
    ced_page_->setTurn(PageInfo.Incline2048);
    ced_page_->setImageSize(Size(PageInfo.Width, PageInfo.Height));
    ced_page_->setImageDpi(Size(PageInfo.DPIX, PageInfo.DPIY));
    ced_page_->setResizeToFit(!RfrmtOptions::useNone());
}

CRtfFragment* CRtfPage::GetNextFragment() {
    m_arFragments.push_back(new CRtfFragment());
    return m_arFragments.back();
}

void CRtfPage::CloseOutputFile() {
    fclose(out);
}

//********* Чтение internal.vit
//*********  Частичное запольнения Page.
//*********  В Page записываютя:
//*********    текстовые фрагменты с содержимым
//*********    -- фреймы  ( врезки ),
//*********    -- обычные ( кандидаты на колонки )
//---------------------------------------
//*********    без содержимого
//*********    -- таблицы
//*********    -- картинки

Bool ReadInternalFileRelease(FILE *in, CRtfPage* RtfPage);

Bool CRtfPage::ReadInternalFile(FILE *fpFileNameIn) {
    if (ReadInternalFileRelease(fpFileNameIn, this))
        return TRUE;

    return FALSE;
}

Bool ReadInternalFileRelease(FILE *in, CRtfPage* RtfPage) {
    CRtfFragment* pRtfFragment;
    CRtfString* pRtfString;
    CRtfWord* pRtfWord;
    CRtfChar* pRtfChar;
    int16_t nc, ns, nw, nz, i;
    int16_t tmp;
    uint32_t wtmp;
    ::Rect16 RectFragm;
    ::Rect16 SRect;
    rewind(in);
    fread(&tmp, 2, 1, in);
    RtfPage->m_wDpi = tmp;

    if (tmp) {
        setTwips(((float) K_TwipsInInch) / tmp);
        setTwips((float) ((int) (getTwips() + 0.5)));
    }

    fread(&tmp, 2, 1, in);
    RtfPage->Count.RtfTextFragments = tmp;
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    RtfPage->Count.RtfFrameTextFragments = 0;

    for (nc = 0; nc < RtfPage->Count.RtfTextFragments; ++nc) {
        pRtfFragment = RtfPage->GetNextFragment();
        pRtfFragment->setParent(RtfPage);
        pRtfFragment->setType(FT_TEXT);
        fread(&RectFragm, 1, sizeof(Rect16), in);
        pRtfFragment->m_rect.left = (int32_t) (RectFragm.left * CIF::getTwips());
        pRtfFragment->m_rect.top = (int32_t) (RectFragm.top * CIF::getTwips());
        pRtfFragment->m_rect.right = (int32_t) (RectFragm.right * CIF::getTwips());
        pRtfFragment->m_rect.bottom = (int32_t) (RectFragm.bottom * CIF::getTwips());
        fread(&tmp, 2, 1, in);
        int str_count = tmp;
        fread(&wtmp, 4, 1, in);
        pRtfFragment->m_wUserNumber = wtmp;
        fread(&wtmp, 4, 1, in);

        for (ns = 0; ns < str_count; ++ns) {
            pRtfString = new CRtfString;
            pRtfFragment->addString(pRtfString);
            fread(&SRect, sizeof(Rect16), 1, in);
            //Реальные коор. строки!
            fread(&SRect, sizeof(Rect16), 1, in);
            fread(&tmp, 2, 1, in);
            int word_count = tmp;
            fread(&tmp, sizeof(uint32_t), 1, in);//NEGA_STR

            for (nw = 0; nw < word_count; ++nw) {
                pRtfWord = new CRtfWord;
                pRtfString->addWord(pRtfWord);
                fread(&tmp, 2, 1, in);
                int char_count = tmp;
                fread(&tmp, 2, 1, in);
                pRtfWord->setFontNumber(tmp);
                fread(&tmp, 2, 1, in);
                pRtfWord->setIdealFontSize(tmp);

                for (nz = 0; nz < char_count; ++nz) {
                    uint16_t num;
#pragma pack(1)
                    struct ALT_TIGER1
                    {
                            unsigned char let, prob;
                    } alt1;
                    struct ALT_TIGER2
                    {
                            unsigned char spellnocarrying, FlagCapDrop, spell, base;
                            language_t language;
                    } alt2;
#pragma pack()
                    pRtfChar = new CRtfChar;
                    pRtfWord->addChar(pRtfChar);
                    fread(&SRect, sizeof(Rect16), 1, in); //Ideal BOX
                    pRtfChar->setIdealRect(CIF::Rect(Point(SRect.left, SRect.top), Point(
                            SRect.right, SRect.bottom)));
                    fread(&SRect, sizeof(Rect16), 1, in); //Real BOX
                    pRtfChar->setRealRect(CIF::Rect(Point(SRect.left, SRect.top), Point(
                            SRect.right, SRect.bottom)));
                    fread(&num, sizeof(uint16_t), 1, in);
                    assert(num <= REC_MAX_VERS);
                    for (i = 0; i < num; i++) {
                        fread(&alt1, sizeof(struct ALT_TIGER1), 1, in);
                        pRtfChar->addVersion(Letter(alt1.let, alt1.prob));
                    }

                    fread(&alt2, sizeof(struct ALT_TIGER2), 1, in);
                    pRtfChar->setLanguage(alt2.language);
                    pRtfChar->setSpelledNoCarrying(alt2.spellnocarrying);
                    pRtfChar->setDropCap(alt2.FlagCapDrop);
                    pRtfChar->setSpelled(alt2.spell);
                    pRtfChar->setFont(pRtfWord->fontNumber());
                    pRtfChar->setFontSize(pRtfWord->idealFontSize());
                }
            }
        }
    }

    return TRUE;
}

void CRtfPage::SetTwips(void) {
    m_fTwips = getTwips();
}

void CRtfPage::AddTables() {
    //    CRtfFragment * pRtfFragment;
    //    Rect16 RectPict;
    //
    //    Count.RtfTableFragments = (uint16_t) CountTable;
    //    for (uint32_t i = 0; i < (int) CountTable; i++) {
    //        pRtfFragment = GetNextFragment();
    //        GetTableRect(i, &RectPict, (uint32_t*) &pRtfFragment->m_wUserNumber);
    //        RtfAssignRect_CRect_Rect16(&pRtfFragment->m_rect, &RectPict);
    //        RtfCalcRectSizeInTwips(&pRtfFragment->m_rect, CIF::getTwips());
    //        pRtfFragment->type_ = FT_TABLE;
    //        if (FlagMode & USE_FRAME_AND_COLUMN) {
    //            pRtfFragment->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumber;
    //            pRtfFragment->m_wUserNumber = i;
    //        }
    //    }
}

void CRtfPage::AddPictures() {
    CRtfFragment* pRtfFragment;
    ::Rect16 RectPict;
    Count.RtfPictureFragments = (uint16_t) CountPict;

    for (int i = 0; i < (int) CountPict; i++) {
        pRtfFragment = GetNextFragment();
        GetPictRect(i, &RectPict, (uint32_t*) &pRtfFragment->m_wUserNumber);
        RtfAssignRect_CRect_Rect16(&pRtfFragment->m_rect, &RectPict);
        RtfCalcRectSizeInTwips(&pRtfFragment->m_rect, CIF::getTwips());
        pRtfFragment->setType(FT_PICTURE);

        if (RfrmtOptions::useFramesAndColumns()) {
            pRtfFragment->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumber;
            pRtfFragment->m_wUserNumber = i;
        }
    }
}
//~ поиск межсекторных H_lines & межколонных V-Lines
void CRtfPage::AddLines() {
    CRtfSector* pRtfSector;
    CRtfSector* pRtfNextSector;
    RECT Rect;
    int16_t CountSectors = m_arSectors.size();

    for (int i = 0; i < CountSectors; i++) {
        if (i == 0) {
            pRtfSector = m_arSectors[i];
            pRtfSector->SectorInfo.PaperW = page_size_.width();
            Rect.left = 0;
            Rect.right = page_size_.width();
            Rect.top = 0;
            Rect.bottom = MAX(0, pRtfSector->m_rect.top);

            if (CheckLines(&Rect, FALSE, &pRtfSector->SectorInfo)) {
                pRtfSector = *m_arSectors.insert(m_arSectors.begin() + i, new CRtfSector());
                pRtfSector->m_bFlagLine = TRUE;
                pRtfSector->SectorInfo.PaperW = page_size_.width();
                CountSectors++;
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &Rect);
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &Rect);
            }
        } else
        //      if( i < CountSectors )
        {
            pRtfSector = m_arSectors[i - 1];
            pRtfSector->SectorInfo.PaperW = page_size_.width();
            pRtfNextSector = m_arSectors[i];
            Rect.left = 0;
            Rect.right = page_size_.width();
            Rect.top = pRtfSector->m_rect.bottom;
            Rect.bottom = pRtfNextSector->m_rect.top;

            if (CheckLines(&Rect, FALSE, &pRtfSector->SectorInfo)) {
                pRtfSector = *m_arSectors.insert(m_arSectors.begin() + i, new CRtfSector());
                pRtfSector->m_bFlagLine = TRUE;
                pRtfSector->SectorInfo.PaperW = page_size_.width();
                CountSectors++;
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &Rect);
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &Rect);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SortUserNumber                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::SortUserNumber(void) {
    CRtfFragment* pRtfFragment;
    uchar FlagChange;
    uint32_t mas[500], MinUserNumber = 32000;
    int indexMinUserNumber, i, j;
    int16_t CountFragments;
    CountFragments = Count.RtfFrameTextFragments + Count.RtfTextFragments + Count.RtfTableFragments
            + Count.RtfPictureFragments;

    for (i = 0; i < CountFragments; i++) {
        MinUserNumber = 32000;
        FlagChange = FALSE;

        for (j = 0; j < CountFragments; j++) {
            pRtfFragment = m_arFragments[j];

            if (pRtfFragment->m_wUserNumber < MinUserNumber) {
                MinUserNumber = pRtfFragment->m_wUserNumber;
                indexMinUserNumber = j;
                FlagChange = TRUE;
            }
        }

        if (FlagChange) {
            pRtfFragment = m_arFragments[indexMinUserNumber];
            pRtfFragment->m_wUserNumber = 32000;
            mas[indexMinUserNumber] = i + 1;
        }
    }

    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = m_arFragments[i];
        pRtfFragment->m_wUserNumber = mas[i];
    }
}

//Режим работы: USE_FRAME_AND_COLUMN
//Разбиение страницы на сектора
//Секторы на горизантальные колонки
//Горизантальные колонки на вертикальные колонки
//Определение терминальности вертикальных колонок
Bool CRtfPage::FindPageTree(FILE *fpFileNameIn, const char* FileNameOut) {
    return PageTree(fpFileNameIn, this, FileNameOut);
}

//////////////////////////////////////////////////////////////////////////////////////////
//           Вычисления ширины и высоты листа                                           //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::ReCalcPageWidthAndHeight(void) {
    int32_t LeftPos = 32000, TopPos = 32000, RightPos = -32000, BottomPos = -32000, Width = 0;
    CRtfSector* pRtfSector;

    if (RfrmtOptions::useNone()) {// Фрагменты отписываются по пользовательским номерам
        MargL = DefMargL;
        MargR = DefMargR;
        MargT = DefMargT;
        MargB = DefMargB;

        for (FragmentList::iterator ppRtfFragment = m_arFragments.begin(); ppRtfFragment
                != m_arFragments.end(); ppRtfFragment++) {
            Width = MAX(Width, (*ppRtfFragment)->m_rect.right - (*ppRtfFragment)->m_rect.left);
        }

        page_size_.rwidth()
                = MAX(DefaultWidthPage, (int32_t)(Width/** CIF::getTwips()*/) + MargL + MargR);
        page_size_.rheight() = DefaultHeightPage;
    } else if (RfrmtOptions::useFrames() || bad_column_) {// Все фрагменты фреймы
        m_arSectors.push_back(new CRtfSector());
        pRtfSector = m_arSectors.back();

        for (FragmentList::iterator ppRtfFragment = m_arFragments.begin(); ppRtfFragment
                != m_arFragments.end(); ppRtfFragment++) {
            LeftPos = MIN(LeftPos, (int16_t)(*ppRtfFragment)->m_rect.left);
            TopPos = MIN(TopPos, (int16_t)(*ppRtfFragment)->m_rect.top);
            RightPos = MAX(RightPos, (int16_t)(*ppRtfFragment)->m_rect.right);
            BottomPos = MAX(BottomPos, (int16_t)(*ppRtfFragment)->m_rect.bottom);
        }

        pRtfSector->m_rectReal.left = pRtfSector->m_rect.left = LeftPos;
        pRtfSector->m_rectReal.right = pRtfSector->m_rect.right = RightPos;
        pRtfSector->m_rectReal.top = pRtfSector->m_rect.top = TopPos;
        pRtfSector->m_rectReal.bottom = pRtfSector->m_rect.bottom = BottomPos;
        SetPaperSize(LeftPos, RightPos, TopPos, BottomPos, page_size_, &MargL, &MargR, &MargT,
                &MargB);
        InitMargL = MargL;
        InitMargR = MargR;
        InitMargT = MargT;
        InitMargB = MargB;
    } else {// Фрагменты отписываются после изучения структуры страницы
        for (FragmentList::iterator ppRtfFragment = m_arFragments.begin(); ppRtfFragment
                != m_arFragments.end(); ppRtfFragment++) {
            LeftPos = MIN(LeftPos, (int16_t)(*ppRtfFragment)->m_rect.left);
            TopPos = MIN(TopPos, (int16_t)(*ppRtfFragment)->m_rect.top);
            RightPos = MAX(RightPos, (int16_t)(*ppRtfFragment)->m_rect.right);
            BottomPos = MAX(BottomPos, (int16_t)(*ppRtfFragment)->m_rect.bottom);
        }

        SetPaperSize(LeftPos, RightPos, TopPos, BottomPos, page_size_, &MargL, &MargR, &MargT,
                &MargB);
        InitMargL = MargL;
        InitMargR = MargR;
        InitMargT = MargT;
        InitMargB = MargB;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//           Вычисления ширены и высоты листа                                           //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::SetPaperSize(int32_t LeftPos, int32_t RightPos, int32_t TopPos, int32_t BottomPos,
        Size& size, int32_t* MargL, int32_t* MargR, int32_t* MargT, int32_t* MargB) {
    int i, j;
    int32_t MargL2 = DefMargL / 2, MargL10 = DefMargL / 10, MargL0 = 0;
    int32_t MargR2 = DefMargR / 2, MargR10 = DefMargR / 10, MargR0 = 0;
    int32_t MargT2 = DefMargT / 2, MargT10 = DefMargT / 10, MargT0 = 0;
    int32_t MargB2 = DefMargB / 2, MargB10 = DefMargB / 10, MargB0 = 0;
    int32_t MasMargL[] = { DefMargL, MargL2, MargL10, MargL0 };
    int32_t MasMargR[] = { DefMargR, MargR2, MargR10, MargR0 };
    int32_t MasMargT[] = { DefMargT, MargT2, MargT10, MargT0 };
    int32_t MasMargB[] = { DefMargB, MargB2, MargB10, MargB0 };
    int32_t WidthPage[] = { 11906, 20163, 23812, 23812 }; // A4,Legal,A3,A2
    int32_t HeightPage[] = { 16838, 12242, 16838, 33676 }; // A4,Legal,A3,A2

    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            if (((RightPos + MasMargL[i] + MasMargR[i]) <= WidthPage[j]) && ((BottomPos
                    + MasMargT[i] + MasMargB[i]) <= HeightPage[j])) {
                *MargL = MasMargL[i];
                *MargR = MasMargR[i];

                if ((BottomPos + 2 * (MasMargT[i] + MasMargB[i])) >= HeightPage[j]) {
                    *MargT = MasMargT[3];
                    *MargB = MasMargB[3];
                } else {
                    *MargT = MasMargT[i];
                    *MargB = MasMargB[i];
                }

                size.setWidth(HeightPage[j]);
                size.setHeight(WidthPage[j]);
                return;
            }
        }

        for (i = 0; i < 4; i++) {
            if (((RightPos + MasMargL[i] + MasMargR[i]) <= HeightPage[j]) && ((BottomPos
                    + MasMargT[i] + MasMargB[i]) <= WidthPage[j])) {
                *MargL = MasMargL[i];
                *MargR = MasMargR[i];

                if ((BottomPos + 2 * (MasMargT[i] + MasMargB[i])) >= WidthPage[j]) {
                    *MargT = MasMargT[3];
                    *MargB = MasMargB[3];
                } else {
                    *MargT = MasMargT[i];
                    *MargB = MasMargB[i];
                }

                size.setWidth(HeightPage[j]);
                size.setHeight(WidthPage[j]);
                return;
            }
        }
    }

    *MargL = MargL2;
    *MargR = MargR2;
    *MargT = MargT2;
    *MargB = MargB2;
    size.setWidth(HeightPage[3]);
    size.setHeight(WidthPage[3]);
}

//////////////////////////////////////////////////////////////////////////////////////////
//            Моя коррекция кегля с учетом размера строки и заданного фонта             //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::CorrectKegl(void) {
    CRtfFragment* pRtfFragment;
    CRtfString* pRtfString;
    CRtfWord* pRtfWord;
    CRtfChar *pRtfChar, *pLastChar, *pFirstChar;
    uint16_t CountTextFragments;
    uint16_t CountStrings;
    uint16_t CountWords;
    uint16_t CountChars;
    int16_t LenghtStr, Real_Size_Kegl;
    char tmp_str[MAX_BUFFER_SIZE];
    CountTextFragments = Count.RtfTextFragments + Count.RtfFrameTextFragments;

    for (int i = 0; i < CountTextFragments; i++) {
        pRtfFragment = m_arFragments[i];
        CountStrings = pRtfFragment->stringCount();

        for (int ns = 0; ns < CountStrings; ns++) {
            pRtfString = pRtfFragment->stringAt(ns);
            CountWords = pRtfString->wordCount();
            //Считаем длину получившейся строки
            int len = 0;

            for (int w = 0; w < CountWords; w++)
                len += pRtfString->wordAt(w)->charCount() + 1;

            //Выделяем буфер под неё
            char* TmpString = new char[len + 1];
            TmpString[0] = 0;

            //Формируем полную строку
            for (int nw = 0; nw < CountWords; nw++) {
                int nz;
                pRtfWord = pRtfString->wordAt(nw);
                CountChars = pRtfWord->charCount();

                for (nz = 0; nz < CountChars; nz++) {
                    pRtfChar = pRtfWord->charAt(nz);
                    tmp_str[nz] = pRtfChar->first().getChar();

                    if (!nz)
                        pRtfChar->setFontSize(MIN(pRtfChar->fontSize(), MaxFontSize));
                }

                if (nw < CountWords - 1)
                    tmp_str[nz++] = ' ';

                tmp_str[nz] = 0;
                strcat(TmpString, tmp_str);
            }

            pRtfWord = pRtfString->firstWord();
            pFirstChar = pRtfWord->firstChar();
            pRtfWord = pRtfString->lastWord();
            CountChars = pRtfWord->charCount();
            pLastChar = pRtfWord->lastChar();
            LenghtStr = (int16_t) (pLastChar->idealRect().right() - pFirstChar->idealRect().left());
            // adjust kegl to the text line real width (Microsoft function)
            Real_Size_Kegl = GetRealSizeKegl(TmpString, LenghtStr, pFirstChar->fontSize(),
                    pFirstChar->font());
            AddNewKegl(pLastChar->fontSize(), Real_Size_Kegl);
            delete[] TmpString;
        }
    }
}

void CRtfPage::ChangeKegl(void) {
    CRtfFragment* pRtfFragment;
    CRtfString* pRtfString;
    CRtfWord* pRtfWord;
    uint16_t CountTextFragments;
    uint16_t CountStrings;
    uint16_t CountWords;
    CountTextFragments = Count.RtfTextFragments + Count.RtfFrameTextFragments;

    // по частоте встречаемости выбираем преобразование из реал. в идеал. кегль
    for (int i = 0; i < CountTextFragments; i++) {
        pRtfFragment = m_arFragments[i];
        CountStrings = pRtfFragment->stringCount();

        for (int ns = 0; ns < CountStrings; ns++) {
            pRtfString = pRtfFragment->stringAt(ns);
            CountWords = pRtfString->wordCount();

            for (int nw = 0; nw < CountWords; nw++) {
                pRtfWord = pRtfString->wordAt(nw);

                if (CountStrings == 1)
                    pRtfWord->setRealFontSize(GetMinKegl(pRtfWord->idealFontSize()));
                else
                    pRtfWord->setRealFontSize(GetNewKegl(pRtfWord->idealFontSize()));
            }
        }
    }
}

void CRtfPage::AddNewKegl(int16_t OldKegl, int16_t NewKegl) {
    KEGL tmp;

    for (std::vector<KEGL>::iterator kegl = arKegls.begin(); kegl < arKegls.end(); kegl++) {
        if (OldKegl == kegl->OldKegl && kegl->NewKegl == NewKegl) {
            kegl->Count++;
            return;
        }
    }

    tmp.OldKegl = OldKegl;
    tmp.NewKegl = NewKegl;
    tmp.Count = 1;
    arKegls.push_back(tmp);
}

int16_t CRtfPage::GetNewKegl(int16_t OldKegl) {
    int16_t FlagChange = 0, Count = 0, tmpKegl, FlagPenalty = 1;

    for (std::vector<KEGL>::iterator kegl = arKegls.begin(); kegl < arKegls.end(); kegl++) {
        if (OldKegl == kegl->OldKegl) {
            if (OldKegl <= 14) {
                if (Count < kegl->Count) {
                    FlagChange = 1;
                    Count = kegl->Count;
                    tmpKegl = kegl->NewKegl;
                }
            } else {
                FlagChange = 1;
                tmpKegl = MIN(kegl->NewKegl, OldKegl);
            }
        }
    }

    if (FlagChange) {
        for (std::vector<KEGL>::iterator __kegl = arKegls.begin(); __kegl < arKegls.end(); __kegl++) {
            if (OldKegl == __kegl->OldKegl) {
                if ((__kegl->Count > (Count / 4)) && __kegl->NewKegl < tmpKegl) {
                    tmpKegl = __kegl->NewKegl;
                    FlagPenalty = 0;
                } else if (FlagPenalty && (__kegl->Count > (Count / 10)) && __kegl->NewKegl
                        < tmpKegl) {
                    tmpKegl--;
                    tmpKegl = MAX(tmpKegl, 4);
                }
            }
        }

        OldKegl = tmpKegl;
    }

    OldKegl = MAX(4, OldKegl);
    return OldKegl;
}

int16_t CRtfPage::GetMinKegl(int16_t OldKegl) {
    int16_t FlagChange = 0, tmpKegl = 72;

    for (std::vector<KEGL>::iterator kegl = arKegls.begin(); kegl < arKegls.end(); kegl++) {
        if (OldKegl == kegl->OldKegl) {
            FlagChange = 1;
            tmpKegl = MIN(kegl->NewKegl, tmpKegl);
        }
    }

    if (FlagChange)
        OldKegl = tmpKegl;

    return OldKegl;
}

CEDPage * CRtfPage::Write() {
    if (RfrmtOptions::useNone()) { // Фрагменты отписываются по пользовательским номерам
        writeUsingNone();
    } else if (RfrmtOptions::useFrames() || bad_column_) { // Все фрагменты фреймы
        writeUsingFrames();
    } else {
        writeUsingFramesAndColumns(); // Фрагменты отписываются после изучения структуры страницы
    }

    return ced_page_;
}

// Фрагменты отписываются по пользовательским номерам
Bool CRtfPage::writeUsingNone() {
    ReCalcPageWidthAndHeight();
    initCedPage();

    if (!WriteHeaderRtf())
        throw std::runtime_error("[CRtfPage::Write] WriteHeaderRtf failed");

    int16_t NumberCurrentFragment, InGroupNumber;
    uchar FragmentType;
    CRtfFragment *pRtfFragment;
    CRtfSector *pRtfSector;
    int16_t CountSectors = Count.RtfFrameTextFragments + Count.RtfTextFragments
            + Count.RtfTableFragments + Count.RtfPictureFragments;

    for (int16_t i = 0; i < CountSectors; i++) {
        m_nCurSectorNumber = i;
        InGroupNumber = i;
        NumberCurrentFragment = GetFlagAndNumberFragment(&FragmentType, &InGroupNumber);
        m_arSectors.push_back(new CRtfSector());
        pRtfSector = m_arSectors.back();
        pRtfSector->SectorInfo.userNum = NumberCurrentFragment;
        pRtfSector->m_VTerminalColumnNumber = 1;
        WriteSectorsHeader(i);
        pRtfFragment = m_arFragments[NumberCurrentFragment];

        if (FragmentType == FT_TABLE) {
            //pRtfFragment->FWriteTable(InGroupNumber, &pRtfSector->SectorInfo, FOT_SINGLE);
        } else if (FragmentType == FT_PICTURE) {
            pRtfFragment->FWritePicture(InGroupNumber, &pRtfSector->SectorInfo, FOT_SINGLE);
        } else {
            pRtfFragment->setParent(this);
            pRtfFragment->writeText(&pRtfSector->SectorInfo);
        }
    }

    return TRUE;
}

// Все фрагменты фреймы
Bool CRtfPage::writeUsingFrames() {
    ReCalcPageWidthAndHeight();
    initCedPage();

    if (!WriteHeaderRtf())
        throw std::runtime_error("[CRtfPage::Write] WriteHeaderRtf failed");

    int16_t InGroupNumber;
    CRtfFragment* pRtfFragment;
    RtfSectorInfo* SectorInfo;
    CRtfSector *pRtfSector;
    int32_t shpleft, shptop, shpright, shpbottom;
    EDBOX EdFragmRect;
    Handle hParagraph = NULL;
    int align;
    CIF::Rect indent;
    EDSIZE interval;
    EDBOX playout;
    int16_t CountFragments = Count.RtfFrameTextFragments + Count.RtfTextFragments
            + Count.RtfTableFragments + Count.RtfPictureFragments;
    WriteSectorsHeader(0);
    pRtfSector = m_arSectors[0];
    SectorInfo = &pRtfSector->SectorInfo;
    SectorInfo->Offset.set(0, 0);

    if (CountFragments) {
        indent = CIF::Rect();
        interval.cx = 0;
        interval.cy = 0;
        playout.x = -1;
        playout.w = -1;
        playout.y = -1;
        playout.h = -1;
        align = TP_LEFT_ALLIGN;
        hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align, indent,
                SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
        CEDLine * hString = new CEDLine(false, 6);
        ((CEDParagraph*) hParagraph)->insertLine(hString);
    }

    for (int16_t i = 0; i < CountFragments; i++) {
        pRtfFragment = m_arFragments[i];

        if (pRtfFragment->type() == FT_TABLE) {
            InGroupNumber = i - (Count.RtfFrameTextFragments + Count.RtfTextFragments);
            //            pRtfFragment->FWriteTable(InGroupNumber, SectorInfo, FOT_FRAME);
        } else if (pRtfFragment->type() == FT_PICTURE) {
            InGroupNumber = i - (Count.RtfFrameTextFragments + Count.RtfTextFragments
                    + Count.RtfTableFragments);
            pRtfFragment->FWritePicture(InGroupNumber, SectorInfo, FOT_FRAME);
        } else {
            shpleft = pRtfFragment->m_rect.left;
            shptop = pRtfFragment->m_rect.top;
            shpbottom = pRtfFragment->m_rect.bottom - pRtfFragment->m_rect.top;
            shpright = pRtfFragment->m_rect.right - pRtfFragment->m_rect.left;

            EdFragmRect.x = pRtfFragment->m_rect.left;
            EdFragmRect.w = pRtfFragment->m_rect.right - pRtfFragment->m_rect.left;
            EdFragmRect.y = pRtfFragment->m_rect.top;
            EdFragmRect.h = pRtfFragment->m_rect.bottom - pRtfFragment->m_rect.top;
            SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn,
                    EdFragmRect, 0x22, -1, 86, 43);

            SectorInfo->FlagOverLayed = FALSE;
            pRtfFragment->setParent(this);
            pRtfFragment->writeText(SectorInfo);
        }
    }

    return TRUE;
}
//табл. помещаются в существующие сектора как frame (м.б.изменение размера колонки)
// или создаются для них новые сектора(терминал.колонка)
void CRtfPage::ToPlacePicturesAndTables(void) {
    CRtfSector* pRtfSector;
    int j, CountSectors;

    for (std::vector<CRtfFragment*>::iterator ppRtfFragment = m_arFragments.begin(); ppRtfFragment
            < m_arFragments.end(); ppRtfFragment++) {
        if ((*ppRtfFragment)->type() == FT_PICTURE || (*ppRtfFragment)->type() == FT_TABLE) {
            CountSectors = m_arSectors.size();

            //страница пустая:создается новый сектор для них
            if (CountSectors == 0) {
                m_arSectors.push_back(new CRtfSector());
                pRtfSector = m_arSectors.front();
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &(*ppRtfFragment)->m_rect);
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &(*ppRtfFragment)->m_rect);
                pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
                continue;
            }

            //картина или таблица ниже последного сектора:создается новый сектор для них
            pRtfSector = m_arSectors.back();

            if ((*ppRtfFragment)->m_rect.top >= pRtfSector->m_rectReal.bottom) {
                m_arSectors.push_back(new CRtfSector());
                pRtfSector = m_arSectors.back();
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &(*ppRtfFragment)->m_rect);
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &(*ppRtfFragment)->m_rect);
                pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
                continue;
            }

            for (j = 0; j < CountSectors; j++) {//картина или таблица между секторами:создается новый сектор для них
                pRtfSector = m_arSectors[j];

                if ((*ppRtfFragment)->m_rect.bottom <= pRtfSector->m_rectReal.top) {
                    pRtfSector = *m_arSectors.insert(m_arSectors.begin() + j, new CRtfSector());
                    RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &(*ppRtfFragment)->m_rect);
                    RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &(*ppRtfFragment)->m_rect);
                    pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
                    break;
                } else {//картина или таблица внутри сектора: добавляетс
                    if (((*ppRtfFragment)->m_rect.top < pRtfSector->m_rectReal.top
                            && (*ppRtfFragment)->m_rect.bottom > pRtfSector->m_rectReal.top)
                            || ((*ppRtfFragment)->m_rect.top >= pRtfSector->m_rectReal.top
                                    && (*ppRtfFragment)->m_rect.top < pRtfSector->m_rectReal.bottom)) {
                        pRtfSector->ToPlacePicturesAndTables((*ppRtfFragment));
                        break;
                    }
                }
            }
        }
    }
}

// Фрагменты отписываются после изучения структуры страницы
Bool CRtfPage::writeUsingFramesAndColumns() {
    ToPlacePicturesAndTables();
    ReCalcPageWidthAndHeight();
    initCedPage();

    if (!WriteHeaderRtf())
        throw std::runtime_error("[CRtfPage::Write] WriteHeaderRtf failed");

    AddLines();

    int i;
    CRtfSector* pRtfSector;
    CRtfSector* pRtfNextSector;
    int16_t CountSectors = m_arSectors.size();

    //Считаем расстояния между секциями (кроме последней)
    for (i = 0; i < CountSectors - 1; i++) {
        pRtfSector = m_arSectors[i];
        pRtfNextSector = m_arSectors[i + 1];
        pRtfSector->SectorInfo.InterSectorDist = GetFreeSpaceBetweenSectors(pRtfSector,
                pRtfNextSector);
    }

    //Отписываем каждую секцию
    for (i = 0; i < CountSectors; i++) {
        m_nCurSectorNumber = i;
        pRtfSector = m_arSectors[i];
        pRtfSector->setPage(this);

        if (pRtfSector->m_bFlagLine == FALSE)
            pRtfSector->calcSector();

        WriteSectorsHeader(i);
        pRtfSector->Write();
    }

    return TRUE;
}

// чтобы дать больший, чем default, вертикальный offset of the section
uint16_t CRtfPage::GetFreeSpaceBetweenSectors(CRtfSector* pRtfSector, CRtfSector* pRtfNextSector) {
    CRtfFragment *pRtfFragment;
    RECT FreePlace;
    int i, CountFragments;
    int32_t FreePlaceHeight;
    FreePlace.left = pRtfSector->m_rect.left;
    FreePlace.right = pRtfSector->m_rect.right;
    FreePlace.bottom = pRtfNextSector->m_rect.top - 1;
    FreePlace.top = pRtfSector->m_rect.bottom + 1;

    if (FreePlace.top >= FreePlace.bottom)
        return 0;

    CountFragments = m_arFragments.size(); //это множество фрагментов pRtfNextSector

    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = m_arFragments[i];

        if (pRtfFragment->m_rect.bottom <= FreePlace.top || pRtfFragment->m_rect.right
                <= FreePlace.left || pRtfFragment->m_rect.top >= FreePlace.bottom
                || pRtfFragment->m_rect.left >= FreePlace.right)
            continue;

        if (pRtfFragment->m_rect.top >= FreePlace.top && pRtfFragment->m_rect.top
                <= FreePlace.bottom)
            FreePlace.bottom = pRtfFragment->m_rect.top;
    }

    FreePlaceHeight = MAX(0, FreePlace.bottom - FreePlace.top);
    return (uint16_t) FreePlaceHeight;
}

Bool CRtfPage::WriteHeaderRtf() {
    typedef std::pair<int, std::string> FontEntry;
    typedef std::vector<FontEntry> FontList;
    FontList fonts;
    fonts.push_back(FontEntry(FF_SWISS, font_sans_));
    fonts.push_back(FontEntry(FF_ROMAN, font_serif_));
    fonts.push_back(FontEntry(FF_MODERN, font_monospace_));
    fonts.push_back(FontEntry(FF_SWISS, "Arial Narrow"));

    for (size_t i = 0; i < fonts.size(); i++) {
        CED_CreateFont(ced_page_, (uchar) i, fonts[i].first, (uchar) Frmt_CharSet,
                fonts[i].second.c_str());
    }

    return TRUE;
}

void CRtfPage::WriteSectorsHeader(int16_t i) {
    CRtfSector *pRtfSector;
    int CountHTerminalColumns;
    int j;
    int EDCountHTerminalColumns;
    CIF::Rect border;
    EDCOL *pEDColumnFirst, *pEDColumn;

    pRtfSector = m_arSectors[i];

    if (i > 0) //!!!Art
        pRtfSector->SectorInfo.Offset.ry() = m_arSectors[i - 1]->m_rectReal.bottom + m_arSectors[i
                - 1]->SectorInfo.InterSectorDist;//!!!Art
    else
        //!!!Art
        pRtfSector->SectorInfo.Offset.ry() = pRtfSector->m_rectReal.top;

    pRtfSector->SectorInfo.FlagOneString = FALSE;
    pRtfSector->SectorInfo.PaperW = page_size_.width();
    pRtfSector->SectorInfo.PaperH = page_size_.height();

    // m_bFlagLine (есть линии) => не пытаться сдвигать margL для красоты
    if (RfrmtOptions::useFramesAndColumns() && pRtfSector->m_bFlagLine == FALSE) {
        pRtfSector->GetCountAndRightBoundVTerminalColumns();
        CountHTerminalColumns = pRtfSector->m_arWidthTerminalColumns.size();

        if (CountHTerminalColumns) {
            if (pRtfSector->m_FlagOneString == FALSE)
                MargL = pRtfSector->m_arRightBoundTerminalColumns[0];
            else {
                MargL = MIN(InitMargL, pRtfSector->m_arRightBoundTerminalColumns[0]);
                pRtfSector->SectorInfo.FlagOneString = TRUE;
            }
        } else
            MargL = MAX(pRtfSector->m_rectReal.left, 0);
    }

    if (RfrmtOptions::useFramesAndColumns() && pRtfSector->m_bFlagLine == FALSE) {
        if (CountHTerminalColumns) {
            if (pRtfSector->m_FlagOneString == FALSE)
                MargR = page_size_.width()
                        - (pRtfSector->m_arRightBoundTerminalColumns[CountHTerminalColumns - 1]
                                + pRtfSector->m_arWidthTerminalColumns[CountHTerminalColumns - 1]);
            else
                MargR
                        = MIN(InitMargR, page_size_.width()
                                - (pRtfSector->m_arRightBoundTerminalColumns[CountHTerminalColumns - 1]
                                        + pRtfSector->m_arWidthTerminalColumns[CountHTerminalColumns - 1]));
        } else
            MargR = page_size_.width() - pRtfSector->m_rectReal.right;
    }

    pRtfSector->SectorInfo.Offset.rx() = MargL;
    pRtfSector->SectorInfo.MargL = MargL;
    pRtfSector->SectorInfo.MargR = MargR;

    border.setLeft(MargL);
    border.setTop(MargT);
    border.setRight(MargR);
    border.setBottom(MargB);

    if (RfrmtOptions::useFramesAndColumns())
        pRtfSector->SectorInfo.userNum = -1;
    else
        CountHTerminalColumns = 0;

    if (RfrmtOptions::useFramesAndColumns() && pRtfSector->m_bFlagLine == TRUE)
        CountHTerminalColumns = 0;

    EDCountHTerminalColumns = CountHTerminalColumns;

    if (!EDCountHTerminalColumns)
        EDCountHTerminalColumns = 1;

    pEDColumnFirst = pEDColumn = (EDCOL*) malloc(EDCountHTerminalColumns * sizeof(EDCOL));

    if (!pEDColumn)
        return;

    if (!CountHTerminalColumns) {
        pEDColumn->width = page_size_.width() - (MargL + MargR);
        pEDColumn->space = 0;
    }

    for (j = 0; j < CountHTerminalColumns; j++) {
        if (RfrmtOptions::useFramesAndColumns() && pRtfSector->SectorInfo.FlagOneString == TRUE)
            pEDColumn->width = page_size_.width() - (MargL + MargR);
        else
            pEDColumn->width = pRtfSector->m_arWidthTerminalColumns[j];

        if (j < CountHTerminalColumns - 1) {
            pEDColumn->space = pRtfSector->m_arRightBoundTerminalColumns[j + 1]
                    - (pRtfSector->m_arRightBoundTerminalColumns[j] + pEDColumn->width);
        } else
            pEDColumn->space = 0;

        pEDColumn++;
    }

    pRtfSector->m_hEDSector = CED_CreateSection(ced_page_, border, -1, EDCountHTerminalColumns,
            pEDColumnFirst, 0, page_size_.width(), page_size_.height(), 0, -1, -1);
    pRtfSector->SectorInfo.hEDSector = pRtfSector->m_hEDSector;
    pRtfSector->SectorInfo.hEDPage = ced_page_;
    pRtfSector->SectorInfo.hFirstColumn = CED_CreateColumn(pRtfSector->SectorInfo.hEDSector);
    pRtfSector->SectorInfo.hColumn = pRtfSector->SectorInfo.hFirstColumn;
    pRtfSector->SectorInfo.hObject = pRtfSector->SectorInfo.hFirstColumn;
    free(pEDColumnFirst);
}

//Ишется следуюший фрагмент по пользовательскому номеру
int16_t CRtfPage::GetFlagAndNumberFragment(uchar* FragmentType, int16_t* InGroupNumber) {
    int16_t j, i, CountT, CountTT, CountTTP;
    CRtfFragment* pRtfFragment;
    *FragmentType = FT_TEXT;
    i = *InGroupNumber;
    CountT = Count.RtfTextFragments + Count.RtfFrameTextFragments;
    CountTT = CountT + Count.RtfTableFragments;
    CountTTP = CountTT + Count.RtfPictureFragments;

    for (j = 0; j < CountTTP; j++) {
        pRtfFragment = m_arFragments[j];

        if ((uint32_t) (i + 1) == pRtfFragment->m_wUserNumber) {
            i = j;
            break;
        }
    }

    if (i < CountT) {
        *InGroupNumber = i;
        *FragmentType = FT_TEXT;
    } else {
        if (i < CountTT) {
            *InGroupNumber = i - CountT;
            *FragmentType = FT_TABLE;
        } else {
            *InGroupNumber = i - CountTT;
            *FragmentType = FT_PICTURE;
        }
    }

    return i;
}

}

