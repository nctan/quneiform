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

#include "rfrmtoptions.h"
#include "crtffragment.h"
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfsector.h"
#include "crtfpage.h"
#include "crtfword.h"
#include "creatertf.h"
#include "crtffunc.h"
#include "frmtpict.h"

#include "ced/ced.h"
#include "cstr/cstrdefs.h"
#include "common/cifconfig.h"
#include "common/debug.h"
#include "minmax.h"

void Rtf_CED_CreateChar(CIF::Rect* slayout, CIF::Letter* letter, CIF::CRtfChar* pRtfChar);
Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int shad,
        int LenthStringInTwips, int LengthFragmInTwips);

namespace CIF
{

const int SMALL_FONT_SIZE = 5;
const int MEDIUM_FONT_SIZE = 10;
const int BIG_FONT_SIZE = 14;
const int PENALTY_FOR_SMALL_FONT_SIZE = 1;
const int PENALTY_FOR_MEDIUM_FONT_SIZE = 2;
const int PENALTY_FOR_BIG_FONT_SIZE = 4;
const int DEFAULT_MAX_CHAR_DISTANCE = 10;

CRtfFragment::CRtfFragment() :
    parent_(NULL), left_border_(0), right_border_(0), max_char_distance_(0) {
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountLeftRightEqual = 0;
    m_CountCentreEqual = 0;
    SetRect(&m_rect, 32000, 32000, 0, 0);
    m_bOutPutType = FOT_FRAME;
    m_LeftOffsetFragmentFromVerticalColumn = 0;
    m_RightOffsetFragmentFromVerticalColumn = 0;
    m_WidthVerticalColumn = 0;
    m_bFlagObjectInColumn = 0;
    m_wOffsetFromPrevTextFragment = 0;
    m_bFlagUsed = 0;
    m_Attr = 0;
    m_FlagCarry = 0;
    m_Flag = 0;
}

CRtfFragment::~CRtfFragment() {
    clearStrings();
}

void CRtfFragment::addString(CRtfString * str) {
    strings_.push_back(str);
}

void CRtfFragment::adjustParagraph(int topMargin) {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        CRtfString * str = *it;

        if (it == strings_.begin()) {
            str->setTopMargin(topMargin);
            str->setParagraphBegin(true);
        } else {
            str->setTopMargin(0);
            str->setParagraphBegin(false);
        }

        if (str->isLineCarryNeeded())
            str->setLineCarry(true);
    }
}

void CRtfFragment::adjustStringIndents() {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        CRtfString * str = *it;
        str->setLeftIndent(str->leftBorder() - left_border_);
        str->setRightIndent(right_border_ - str->rightBorder());

        assert(0 <= str->leftIndent());
        assert(0 <= str->rightIndent());
    }
}

void CRtfFragment::calcFragmentBorders() {
    if (strings_.empty())
        return;

    left_border_ = minStringLeftBorder();
    right_border_ = maxStringRightBorder();

    assert(left_border_ <= right_border_);
}

void CRtfFragment::calcMaxCharDistance() {
    int char_count = charCount();

    if (char_count)
        max_char_distance_ = charTotalLength() / char_count;
    else
        max_char_distance_ = DEFAULT_MAX_CHAR_DISTANCE;
}

inline bool diffHasEqualSign(int first, int second) {
    return (first <= 0 && second <= 0) || (0 < first && 0 < second);
}

inline bool diffThreshold(int first, int second, int threshold) {
    return (abs(first) > threshold) && (abs(second) > threshold);
}

inline bool symmetricDiffChange(int first, int second, int delta) {
    return diffHasEqualSign(first, second) && diffThreshold(first, second, delta);
}

inline bool isAccuratelyCentered(int left_diff, int right_diff, int delta) {
    // center_diff == (right_diff - left_diff) / 2;
    // (abs(center_diff) < delta) equal to:
    // ((abs(right_diff - left_diff) / 2 ) < delta) equal to:
    // (abs(right_diff - left_diff) < 2 * delta)
    return (abs(right_diff - left_diff) < (2 * delta)) //
            && symmetricDiffChange(left_diff, right_diff, delta / 2);
}

inline bool isFreelyCentered(int left_diff, int right_diff, int max_delta) {
    return ((abs(right_diff + left_diff) / 2 < 2 * max_delta) //
            && (abs(right_diff - left_diff) < 3 * max_delta) //
            && diffThreshold(left_diff, right_diff, 5 * max_delta));
}

inline bool isStringCentered(int left_diff, int right_diff, int max_delta) {
    return isAccuratelyCentered(left_diff, right_diff, max_delta) //
            || isFreelyCentered(left_diff, right_diff, max_delta);
}

inline bool leftAligned(const CRtfString * first, const CRtfString * second, int max_delta) {
    return abs(first->leftIndent() - second->leftIndent()) <= max_delta;
}

inline bool rightAligned(const CRtfString * first, const CRtfString * second, int max_delta) {
    return abs(first->rightIndent() - second->rightIndent()) <= max_delta;
}

inline bool centered(const CRtfString * first, const CRtfString * second, int max_delta) {
    return isStringCentered(first->leftIndent() - second->leftIndent(), //
            first->rightIndent() - second->rightIndent(), max_delta);
}

void CRtfFragment::calcStringEndsEqual() {
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountCentreEqual = 0;
    m_CountLeftRightEqual = 0;

    for (size_t ns = 1; ns < strings_.size(); ns++) {
        updateStringPairAlignment(strings_[ns], strings_[ns - 1]);

        if (ns == 1)
            updateFirstStringPairAlignment();
    }
}

size_t CRtfFragment::charCount() const {
    size_t result = 0;
    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it)
        result += (*it)->charCount();
    return result;
}

int CRtfFragment::charTotalLength() const {
    int result = 0;
    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it)
        result += (*it)->charTotalLength();
    return result;
}

void CRtfFragment::clearStrings() {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it)
        delete *it;
    strings_.clear();
}

void CRtfFragment::correctParagraphIndents(StringIterator begin, StringIterator end) {
    int min_left_indent = minParagraphLeftIndent(begin, end);

    for (StringIterator it = begin; it != end; ++it) {
        if ((*it)->isParagraphBegin()) {
            if (abs((*it)->leftIndent() - min_left_indent) < max_char_distance_)
                (*it)->setLeftIndent(min_left_indent);
        }
    }
}

CRtfString * CRtfFragment::firstString() {
    return strings_.at(0);
}

const CRtfString * CRtfFragment::firstString() const {
    return strings_.at(0);
}

void CRtfFragment::initFragmentFonts(int fragment_count) {
    CRtfWord* first_word = firstString()->firstWord();
    firstString()->setFontSizePenalty(SMALL_FONT_SIZE, fontSizePenalty(fragment_count));
    m_wprev_font_name = fontName(first_word->fontNumber());
    m_wprev_font_size = first_word->realFontSize();
}

CRtfString * CRtfFragment::lastString() {
    return const_cast<CRtfString*> (const_cast<const CRtfFragment*> (this)->lastString());
}

const CRtfString * CRtfFragment::lastString() const {
    if (strings_.empty())
        throw std::out_of_range("[CRtfFragment::lastString] fragment is empty");
    return strings_.back();
}

int CRtfFragment::minParagraphLeftIndent(StringIteratorConst begin, StringIteratorConst end) const {
    int min_left_indent = 0;
    for (StringIteratorConst it = begin; it != end; ++it) {
        if (it == begin) {
            min_left_indent = (*it)->leftIndent();
            continue;
        }

        if ((*it)->isParagraphBegin()) {
            min_left_indent = MIN((*it)->leftIndent(), min_left_indent);
        }
    }
    return min_left_indent;
}

inline bool stringLeftBorderCompare(const CRtfString * first, const CRtfString * second) {
    return first->leftBorder() < second->leftBorder();
}

int CRtfFragment::minStringLeftBorder() const {
    if (strings_.empty())
        throw std::out_of_range("[CRtfFragment::minStringLeftBorder] fragment is empty");

    StringIteratorConst it = std::min_element(strings_.begin(), strings_.end(),
            stringLeftBorderCompare);

    return (*it)->leftBorder();
}

inline bool stringRightBorderCompare(const CRtfString * first, const CRtfString * second) {
    return first->rightBorder() < second->rightBorder();
}

int CRtfFragment::maxStringRightBorder() const {
    if (strings_.empty())
        throw std::out_of_range("[CRtfFragment::maxStringRightBorder] fragment is empty");

    StringIteratorConst it = std::max_element(strings_.begin(), strings_.end(),
            stringRightBorderCompare);
    return (*it)->rightBorder();
}

CRtfFragment::StringIteratorConst CRtfFragment::findNextFragment(StringIteratorConst begin) const {
    if (begin == strings_.end())
        return strings_.end();

    for (StringIteratorConst it = begin + 1, end = strings_.end(); it != end; ++it) {
        if ((*it)->hasAttributes())
            return it;
    }
    return strings_.end();
}

CRtfFragment::StringIterator CRtfFragment::findParagraph(StringIterator begin, StringIterator end) {
    for (StringIterator it = begin; it != end; ++it) {
        if ((*it)->isParagraphBegin())
            return it;
    }
    return strings_.end();
}

void CRtfFragment::updateFirstStringPairAlignment() {
    assert(strings_.size() > 1);
    CRtfString * current = strings_[1];
    CRtfString * previous = strings_.front();

    if (current->isEqualLeft()) {
        previous->setEqualLeft(true);
        m_CountLeftEqual++;
    } else if (current->isEqualRight()) {
        previous->setEqualRight(true);
        m_CountLeftRightEqual++;
        m_CountRightEqual++;
    } else if (current->isEqualCenter()) {
        previous->setEqualCenter(true);
        m_CountCentreEqual++;
    } else {
        Debug() << "[CRtfFragment::updateFirstStringPairAlignment] no string alignment\n";
    }
}

void CRtfFragment::updateStringPairAlignment(CRtfString * current, CRtfString * previous) {
    if (leftAligned(current, previous, max_char_distance_)) {
        current->setEqualLeft(true);
        m_CountLeftEqual++;
    }

    if (rightAligned(current, previous, max_char_distance_)) {
        current->setEqualRight(true);
        m_CountRightEqual++;

        if (current->isEqualLeft())
            m_CountLeftRightEqual++;
    }

    if (centered(current, previous, max_char_distance_)) {
        current->setEqualCenter(true);
        m_CountCentreEqual++;
    }
}

void CRtfFragment::processingUseNoneMode() {
    for (size_t i = 0; i < strings_.size(); i++) {
        CRtfString * str = strings_[i];

        if (i == 0)
            str->setParagraphBegin(true);
        else
            str->setParagraphBegin(false);

        if (i == stringCount() - 1)
            str->setLineTransfer(false);
        else
            str->setLineTransfer(true);

        str->setAlign(RTF_TP_LEFT_ALLIGN);
        str->setLeftIndent(0);
        str->setRightIndent(0);
        str->setFirstIndent(0);
    }
}

void CRtfFragment::setFragmentAlignment(RtfSectorInfo* SectorInfo) {
    if (RfrmtOptions::useNone()) {
        processingUseNoneMode();
        return;
    }

    Init(SectorInfo);

    if (SectorInfo->FlagOverLayed) {
        return processingOverlayed();
    }

    if (DeterminationOfMixedFragment(SectorInfo) == FALSE) {
        if (DeterminationOfLeftRightJustification(0, stringCount()) == FALSE) {
            if (DeterminationOfListType(0, stringCount()) == FALSE) {
                if (DeterminationOfLeftJustification(0, stringCount(), 0) == FALSE) {
                    if (DeterminationOfCentreJustification(0, stringCount()) == FALSE) {
                        if (DeterminationOfRightJustification(0, stringCount()) == FALSE) {
                            DeterminationOfLeftJustification(0, stringCount(), 1);
                        }
                    }
                }
            }
        }
    }

    Done();
}

void CRtfFragment::setLineTransfer(StringIterator begin, StringIterator end) {
    for (StringIterator it = begin; it != end; ++it)
        (*it)->setLineTransfer(true);
}

void CRtfFragment::setParent(CRtfPage * page) {
    parent_ = page;
}

CRtfString * CRtfFragment::stringAt(size_t pos) {
    return strings_.at(pos);
}

const CRtfString * CRtfFragment::stringAt(size_t pos) const {
    return strings_.at(pos);
}

size_t CRtfFragment::stringCount() const {
    return strings_.size();
}

std::string CRtfFragment::toString() const {
    std::string result;
    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it) {
        result.append((*it)->toString());
        if (it != strings_.begin())
            result += "\n";
    }
    return result;
}

Bool CRtfFragment::FWriteText(int NumberCurrentFragment, RtfSectorInfo *SectorInfo, Bool OutPutType) {
    CRtfWord* pRtfWord;
    CRtfString* pRtfString;
    CRtfChar* pRtfChar;
    uint16_t CountWords;
    int16_t flag_end_word_with_hiphen;
    int16_t tmp_font_name;
    Bool boPrevNega, boNega;
#ifdef EdWrite
    int EDFontAttribs, EDFontPointSize;
    Handle hParagraph = NULL;
    Handle hString = NULL;
    CIF::Rect slayout;
    CIF::Letter Letter[REC_MAX_VERS];
    int shading = -1;
#endif
    initFragment(SectorInfo);
    //--- Цикл по строкам
    boPrevNega = false; //NEGA_STR

    for (int ns = 0; ns < strings_.size(); ns++) {
        pRtfString = strings_[ns];
        pRtfWord = pRtfString->firstWord();
        pRtfChar = pRtfWord->firstChar();

        if (pRtfChar->isDropCap()) { //заносим буквицы во frame
            if (RfrmtOptions::useFrames() || OutPutType)
                pRtfChar->setDropCap(false);
            else
                pRtfString->setParagraphBegin(true);
        }

        // чтобы не смешивать в одном абзаце негатив. и позитив. строки, при смене
        // цвета стартуем новый абзац
        boNega = pRtfString->hasFlag(CSTR_STR_NEGATIVE); //NEGA_STR

        if (boNega != boPrevNega)
            pRtfString->setParagraphBegin(true);

        if (pRtfString->isParagraphBegin()) {
            if (pRtfString->align() == RTF_TP_TYPE_LINE) {
                pRtfString->setAlign(RTF_TP_LEFT_AND_RIGHT_ALLIGN);
                m_fi = -pRtfString->firstIndent();
            } else
                m_fi = pRtfString->firstIndent();

            m_wvid_parag = pRtfString->align();
            m_li = pRtfString->leftIndent();
            m_ri = pRtfString->rightIndent();
            m_sb = pRtfString->marginTop();
#ifdef EdWrite

            if (!RtfWriteMode && OutPutType)
                m_sb = 0;

#endif
            m_wprev_font_size = pRtfWord->realFontSize();

            if (CIF::RfrmtOptions::useFramesAndColumns()) {
                if (SectorInfo->FlagOneString == TRUE) {
                    m_li = 0;
                    m_fi = MAX(0, (int16_t)(m_rect.left - SectorInfo->MargL));
                    m_ri = 0;
                }
            }

#ifdef EdWrite

            if (!RtfWriteMode) {
                pRtfWord = pRtfString->firstWord();
                pRtfChar = pRtfWord->firstChar();
                int colWidth = 0;

                if (parent_ && !RfrmtOptions::useNone()) {
                    CRtfSector* curSect =
                            (CRtfSector*) parent_->m_arSectors[parent_->m_nCurSectorNumber];

                    //Если пишем с форматированием и однострочная колонка
                    if (RfrmtOptions::useFramesAndColumns() && curSect->SectorInfo.FlagOneString
                            == TRUE)
                        colWidth = SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR);

                    //Если пишем в колонку

                    else if (SectorInfo->hColumn == SectorInfo->hObject) {
                        colWidth
                                = curSect->m_arWidthTerminalColumns[curSect->m_VTerminalColumnNumber
                                        - 1];
                    }
                }

                //Если ничего не помогло
                if (!colWidth)
                    colWidth = m_rectReal.right - m_rectReal.left;

                if (!pRtfChar->isDropCap())
                    hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb, SectorInfo,
                            m_wvid_parag, pRtfString->flags(), pRtfString->realLength(), colWidth); //NEGA_STR
            }

#endif
        }

#ifdef EdWrite

        if (!RtfWriteMode && !pRtfChar->isDropCap()) {
#ifdef CHEREDOV
            hString = CED_CreateLine(hParagraph, pRtfString->line_break_, (int)((pRtfWord->real_font_size_ - 1) * 2));
#else

            if (!RfrmtOptions::useSize() && !RfrmtOptions::useFrames()) {
                hString = CED_CreateLine(hParagraph, pRtfString->lineTransfer(), DefFontSize); //line is text line
            }

            else {
                hString = CED_CreateLine(hParagraph, pRtfString->lineTransfer(),
                        pRtfWord->realFontSize() * 2);
            }

#endif
        }

#endif
        //--- Цикл по словам
        CountWords = pRtfString->wordCount();

        for (int nw = 0; nw < CountWords; nw++) {
            pRtfWord = pRtfString->wordAt(nw);
            pRtfChar = pRtfWord->firstChar();
            tmp_font_name = fontName(pRtfWord->fontNumber());
#ifdef EdWrite
            EDFontAttribs = 0;
#endif

            if (pRtfWord->fontNumber() & TG_EDW_UNDERLINE) {
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_UNDERLINE;
#endif
            }

            if (RfrmtOptions::useBold() && (pRtfWord->fontNumber() & TG_EDW_BOLD)) {
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_BOLD;
#endif
            }

            if (RfrmtOptions::useItalic() && (pRtfWord->fontNumber() & TG_EDW_ITALIC)) {
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_ITALIC;
#endif
            }

            flag_end_word_with_hiphen = 0;
            pRtfWord->calcCoordinatesAndProbability();
            //--- Цикл по буквам

            for (int nz = 0, total = pRtfWord->charCount(); nz < total; nz++) {
                pRtfChar = pRtfWord->charAt(nz);
#ifdef EdWrite

                if (!pRtfWord->isSpelled())
                    pRtfChar->first().setProbability(0);

                if (nw == 0 && nz == 0 && pRtfChar->isDropCap())
#ifdef CHEREDOV
                    EDFontPointSize = (int)((pRtfChar->font_size_ - 1) * 2);

#else

                    if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                        EDFontPointSize = DefFontSize;
                    else
                        EDFontPointSize = pRtfChar->fontSize() * 2;

#endif
                else
#ifdef CHEREDOV
                EDFontPointSize = (int)((pRtfWord->real_font_size_ - 1) * 2);

#else

                if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                    EDFontPointSize = DefFontSize;
                else
                    EDFontPointSize = pRtfWord->realFontSize() * 2;

#endif
#endif
                flag_end_word_with_hiphen = 0;

                if (nw == (total - 1) && nz == (total - 1) && pRtfChar->first().getChar() == '-')
                    flag_end_word_with_hiphen = 1;

                if (pRtfChar->first().getChar()) {
                    if (pRtfString->lineTransfer()) {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            if (nw == 0 && nz == 0 && pRtfChar->isDropCap()) {
                                slayout = CIF::Rect();
                                EDBOX playout__ = { 0, 0, 0, 0 };
                                Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                        SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                                CED_SetFrameFlag(hObject__, ED_DROPCAP);
                                EDSIZE interval__ = { 0, 0 };

                                //                          if(m_Flag & CSTR_STR_NEGATIVE) //nega
                                if (pRtfString->hasFlag(CSTR_STR_NEGATIVE)) //NEGA_STR
                                    shading = 10000;

                                Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                        hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                        playout__, -1, shading, -1, -1, FALSE);
                                Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
                                        -1, -1);
                                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                        SectorInfo, m_wvid_parag, pRtfString->flags(),
                                        pRtfString->realLength(), m_rectReal.right
                                                - m_rectReal.left); //NEGA_STR
#ifdef CHEREDOV
                                hString = CED_CreateLine(hParagraph, pRtfString->line_break_, (int)((pRtfWord->real_font_size_ - 1) * 2));
#else

                                if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->lineTransfer(), DefFontSize);

                                else
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->lineTransfer(), pRtfWord->realFontSize()
                                                    * 2);

#endif
                            }

                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
                                        -1, -1);
                            }
                        }

#endif

                        if (nw == 0 && nz == 0 && pRtfChar->isDropCap())
                            WriteCupDrop(pRtfChar, tmp_font_name);

                        else
                            PutChar(pRtfChar->first().getChar());
                    }

                    else if (!((m_wvid_parag == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag
                            == RTF_TP_LEFT_ALLIGN) && flag_end_word_with_hiphen
                            && pRtfChar->m_bFlg_spell_nocarrying)) {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            if (nw == 0 && nz == 0 && pRtfChar->isDropCap()) {
                                slayout = CIF::Rect();
                                EDBOX playout__ = { 0, 0, 0, 0 };
                                Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                        SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                                CED_SetFrameFlag(hObject__, ED_DROPCAP);
                                EDSIZE interval__ = { 0, 0 };

                                Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                        hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                        playout__, -1, -1, -1, -1, FALSE);
                                Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
                                        -1, -1);
                                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                        SectorInfo, m_wvid_parag, pRtfString->flags(),
                                        pRtfString->realLength(), m_rectReal.right
                                                - m_rectReal.left); //NEGA_STR
#ifdef CHEREDOV
                                hString = CED_CreateLine(hParagraph, pRtfString->line_break_, (int)((pRtfWord->real_font_size_ - 1) * 2));
#else

                                if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->lineTransfer(), DefFontSize);

                                else
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->lineTransfer(), pRtfWord->realFontSize()
                                                    * 2);

#endif
                            }

                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
                                        -1, -1);
                            }
                        }

#endif

                        if (nw == 0 && nz == 0 && pRtfChar->isDropCap())
                            WriteCupDrop(pRtfChar, tmp_font_name);

                        else
                            PutChar(pRtfChar->first().getChar());
                    }

                    else {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            EDFontAttribs = EDFontAttribs | 0x02;
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1,
                                    -1);
                        }

#endif
                    }
                }
            }

            //--- Конец цикла по буквам
            if (nw < CountWords - 1) {
#ifdef EdWrite

                if (!RtfWriteMode && !pRtfChar->isDropCap()) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->real_font_size_ - 1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else

                    if (!CIF::RfrmtOptions::useSize() && CIF::RfrmtOptions::useFrames())
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

                    else
                        CED_CreateChar(hString, slayout, Letter, pRtfWord->realFontSize() * 2,
                                (int) tmp_font_name, EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

#endif
                }

#endif
            }

            else if ((ns < stringCount() - 1) && (nw == CountWords - 1) && (m_wvid_parag
                    == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag == RTF_TP_LEFT_ALLIGN)
                    && !flag_end_word_with_hiphen) {
#ifdef EdWrite

                if (!RtfWriteMode) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->real_font_size_ - 1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else

                    if (!RfrmtOptions::useSize() && RfrmtOptions::useFrames())
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

                    else
                        CED_CreateChar(hString, slayout, Letter, pRtfWord->realFontSize() * 2,
                                (int) tmp_font_name, EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

#endif
                }

#endif
            }

        }

        //--- Конец цикла по словам
        boPrevNega = boNega; //NEGA_STR
    }

    //--- Конец цикла по строкам
    return TRUE;
}

int CRtfFragment::fontSizePenalty(int fragment_count) const {
    if (strings_.size() != 1 || fragment_count <= 1)
        return 0;

    int sz = strings_.front()->firstWord()->realFontSize();

    if (SMALL_FONT_SIZE < sz && sz <= MEDIUM_FONT_SIZE)
        return PENALTY_FOR_SMALL_FONT_SIZE;
    else if (MEDIUM_FONT_SIZE < sz && sz <= BIG_FONT_SIZE)
        return PENALTY_FOR_MEDIUM_FONT_SIZE;
    else if (BIG_FONT_SIZE < sz)
        return PENALTY_FOR_BIG_FONT_SIZE;
    else
        return 0;
}

void CRtfFragment::initFragment(RtfSectorInfo* SectorInfo) {
    assert(SectorInfo);
    initFragmentFonts(SectorInfo->CountFragments);
    m_wprev_lang = 1024;
    setFragmentAlignment(SectorInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWritePicture                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfFragment::FWritePicture(int16_t NumberCurrentFragment, RtfSectorInfo *SectorInfo,
        Bool OutPutType) {
    //  CString  PictString;
    //  uint32_t   Pindex;
    //  uint32_t  CountPictElem;
    //  char     Psym;
    WritePict((uint32_t) NumberCurrentFragment, SectorInfo,/* &PictString,*/
    OutPutType);
    /*  if(RtfWriteMode)
     {
     CountPictElem = PictString.GetLength();
     for( Pindex=0;  Pindex<CountPictElem; Pindex++ )
     {
     Psym=(char)PictString.GetAt(Pindex);
     if(Psym)
     PutC(Psym);
     }
     }
     */
    return TRUE;
}

void CRtfFragment::Init(RtfSectorInfo* SectorInfo) {
    assert(SectorInfo);

    adjustParagraph(SectorInfo->VerticalOffsetFragmentInColumn);

    //  Поиск:       Левой(m_l_fragment) и правой(m_r_fragment) границ фрагмента
    calcFragmentBorders();

    //  Вычисление:  m_max_dist, котороя используется при поиске начала абзаца
    calcMaxCharDistance();

    // Вычисляется отступ (leftIndent, rightIndent) строки от краев фрагмента
    adjustStringIndents();

    // Присваиваются признаки равенства концов и середины соседних строк
    calcStringEndsEqual();

    if (Config::instance().debugHigh())
        printResult(std::cerr, "\n ================== Init ==================");
}

// если колонки остались несепарабельными, то они все будут отписаны как frames
void CRtfFragment::processingOverlayed() {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfStringNext;
    CRtfString *pRtfString;

    for (size_t i = 0; i < stringCount(); i++) {
        CRtfString * str = strings_[i];
        str->setAlign(RTF_TP_LEFT_AND_RIGHT_ALLIGN);
        str->setParagraphBegin(false);
        str->setRightIndent(0);
    }

    for (size_t ns = 0; ns < stringCount(); ns++) {
        pRtfString = strings_[ns];

        if (ns == 0) {
            if (pRtfString->leftIndent() > max_char_distance_ / 2)
                pRtfString->setFirstIndent(max_char_distance_ * getTwips());
            else
                pRtfString->setFirstIndent(0);

            pRtfString->setLeftIndent(0);
            pRtfString->setParagraphBegin(true);
            continue;
        }

        // ns >= 1
        pRtfStringPrev = strings_[ns - 1];

        if (ns == stringCount() - 1) {
            if ((pRtfString->leftIndent() - pRtfStringPrev->leftIndent())
                    > (max_char_distance_ / 2)) {
                pRtfString->setLeftIndent(0);
                pRtfString->setFirstIndent(max_char_distance_ * getTwips());
                pRtfString->setParagraphBegin(true);
            }
        } else {
            pRtfStringNext = strings_[ns + 1];

            if (((pRtfString->leftIndent() - pRtfStringPrev->leftIndent()) > (max_char_distance_
                    / 2)) && ((pRtfString->leftIndent() - pRtfStringNext->leftIndent())
                    > (max_char_distance_ / 2))) {
                pRtfString->setLeftIndent(0);
                pRtfString->setFirstIndent(max_char_distance_ * getTwips());
                pRtfString->setParagraphBegin(true);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfLeftRightJustification

Bool CRtfFragment::DeterminationOfLeftRightJustification(int beg, int end) {
    if (!CheckLeftRightJustification(beg, end))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    SetFlagBeginParagraphForLeftRightJustification(beg, end);
    correctParagraphIndents(strings_.begin() + beg, strings_.begin() + end);

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== DeterminationOfLeftRightJustification ==================");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckLeftRightJustification

Bool CRtfFragment::CheckLeftRightJustification(int beg, int end) {
    CRtfString *pRtfString;
    int Count = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;
    uint16_t CountLeftEqual = 0;
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);
    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);

    if (CountCentreEqual == (end - beg))
        return FALSE;

    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    m_FlagCarry = GetFlagCarry(beg, end);
    m_FlagLeft = GetFlagLeft(beg, end);
    m_FlagStrongLeft = GetFlagStrongLeft(beg, end);
    m_FlagRight = GetFlagRight(beg, end);
    m_FlagBigSpace = GetFlagBigSpace(beg, end);

    if (m_FlagStrongLeft == TRUE)
        return FALSE;

    if (m_FlagCarry == FALSE && end - beg <= 2 && CountLeftRightEqual == 0)
        return FALSE;

    if (m_FlagCarry == FALSE && m_FlagBigSpace == FALSE && (CountLeftRightEqual < (end - beg) / 3))
        return FALSE;

    if (m_FlagRight == TRUE && end - beg <= 4)
        return FALSE;

    if ((CountLeftEqual >= (end - beg - 1)) && m_FlagBigSpace == FALSE) {
        if (((end - beg) < 5) && (CountLeftEqual > CountLeftRightEqual))
            return FALSE;

        if (CountLeftRightEqual <= (end - beg) / 2)
            return FALSE;
    }

    for (int ns = beg + 1; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->isEqualLeft() && pRtfString->isEqualRight())
            continue;

        if (CheckStringForLeftRightJustification(ns))
            Count++;
    }

    if (m_FlagLeft && ((Count + CountLeftRightEqual) < 4 * (end - beg - 1) / 5))
        return FALSE;

    if (m_FlagCarry) {
        if ((Count + CountLeftRightEqual) < (end - beg - 1) / 3)
            return FALSE;
    }

    else {
        if ((Count + CountLeftRightEqual) < 4 * (end - beg - 1) / 5)
            return FALSE;

        GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

        if ((Count + CountLeftRightEqual) < CountCentreEqual)
            return FALSE;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetCountEqual

void CRtfFragment::GetCountEqual(int beg, int end, uint16_t* Count, int AlignType) {
    CRtfString *pRtfString;

    for (int i = beg; i < end; i++) {
        pRtfString = (CRtfString*) strings_[i];

        switch (AlignType) {
        case RTF_TP_LEFT_ALLIGN:

            if (pRtfString->isEqualLeft())
                (*Count)++;

            break;
        case RTF_TP_RIGHT_ALLIGN:

            if (pRtfString->isEqualRight())
                (*Count)++;

            break;
        case RTF_TP_LEFT_AND_RIGHT_ALLIGN:

            if (pRtfString->isEqualLeft() && pRtfString->isEqualRight())
                (*Count)++;

            break;
        case RTF_TP_CENTER:

            if (pRtfString->isEqualCenter())
                (*Count)++;

            break;
        default:
            break;
        }
    }

    if (*Count == 1)
        (*Count)++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                       CheckStringForLeftRightJustification

Bool CRtfFragment::CheckStringForLeftRightJustification(int ns) {
    CRtfString *pRtfString;
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfStringNext;
    int16_t LeftFragm, RightFragm;
    int16_t LeftDif, RightDif;

    if (m_Attr) {
        LeftFragm = m_l_fragmentLocal;
        RightFragm = m_r_fragmentLocal;
        LeftDif = m_l_fragmentLocal - left_border_;
        RightDif = right_border_ - m_r_fragmentLocal;
    }

    else {
        LeftFragm = left_border_;
        RightFragm = right_border_;
        LeftDif = 0;
        RightDif = 0;
    }

    pRtfString = (CRtfString*) strings_[ns];

    if ((pRtfString->leftIndent() - LeftDif) > max_char_distance_ && ns < stringCount() - 1) {
        pRtfStringNext = (CRtfString*) strings_[ns + 1];

        if (((pRtfString->leftIndent() - LeftDif) < (RightFragm - LeftFragm) / 2)
                && ((pRtfString->rightIndent() - RightDif) < max_char_distance_)
                && ((pRtfStringNext->leftIndent() - LeftDif) < max_char_distance_))
            return TRUE;
    }

    if ((pRtfString->leftIndent() - LeftDif) < max_char_distance_ && ns > 1) {
        pRtfStringPrev = (CRtfString*) strings_[ns - 1];

        if ((pRtfStringPrev->rightIndent() - RightDif) < max_char_distance_)
            return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagBeginParagraphForLeftRightJustification

void CRtfFragment::SetFlagBeginParagraphForLeftRightJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (ns == beg) {
            pRtfString->setParagraphBegin(true);
            continue;
        }

        pRtfStringPrev = (CRtfString*) strings_[ns - 1];

        if (((pRtfString->leftIndent() > 2 * max_char_distance_) && (abs(pRtfString->leftIndent()
                - pRtfStringPrev->leftIndent()) > max_char_distance_))
                || (pRtfStringPrev->rightIndent() > 10 * max_char_distance_)
                || ((pRtfStringPrev->rightIndent() > 5 * max_char_distance_)
                        && (pRtfStringPrev->endsWith(';') || pRtfStringPrev->endsWith('.')))
                || (pRtfString->startsWithDigit() && (pRtfStringPrev->endsWith(';')
                        || pRtfStringPrev->endsWith('.'))) || ((pRtfString->leftIndent() > 3
                * max_char_distance_ / 2) && (pRtfStringPrev->endsWith('.')
                || pRtfString->startsWithDash())) || (pRtfStringPrev->endsWith('.')
                && pRtfString->startsWithDash()) || (pRtfStringPrev->endsWith('?')
                && pRtfString->startsWithDash()) || (pRtfStringPrev->endsWith(':')
                && pRtfString->startsWithDash()) || (pRtfStringPrev->rightIndent() > 2
                * max_char_distance_ && pRtfString->startsWithDash()))
            pRtfString->setParagraphBegin(true);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfLeftJustification

Bool CRtfFragment::DeterminationOfLeftJustification(int beg, int end, Bool direct) {
    if (!direct && !CheckLeftJustification(beg, end))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_LEFT_ALLIGN);

    if (GetFlagCarry(beg, end) == FALSE || GetFlagLeft(beg, end))
        setLineTransfer(strings_.begin() + beg, strings_.begin() + end - 1);

    SetFlagBeginParagraphForLeftJustification(beg, end);
    correctParagraphIndents(strings_.begin() + beg, strings_.begin() + end);

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== DeterminationOfLeftJustification ==================");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckLeftJustification

Bool CRtfFragment::CheckLeftJustification(int beg, int end) {
    uint16_t CountLeftEqual = 0;
    uint16_t CountRightEqual = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;
    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);
    GetCountEqual(beg, end, &CountRightEqual, RTF_TP_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if ((CountLeftEqual < (end - beg) / 2 || CountLeftEqual < CountRightEqual || CountLeftEqual
            < CountLeftRightEqual || CountLeftEqual < CountCentreEqual) && (CountRightEqual
            + CountLeftRightEqual + CountCentreEqual > 0))
        return FALSE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagBeginParagraphForLeftJustification

void CRtfFragment::SetFlagBeginParagraphForLeftJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;
    uchar FlagStringParagraph = FALSE;
    uchar FlagStringParagraphSoft = FALSE;
    uint16_t Count = 0;
    int16_t LeftFragm, RightFragm;
    int16_t LeftDif, RightDif;

    if (m_Attr) {
        LeftFragm = m_l_fragmentLocal;
        RightFragm = m_r_fragmentLocal;
        LeftDif = m_l_fragmentLocal - left_border_;
        RightDif = right_border_ - m_r_fragmentLocal;
    }

    else {
        LeftFragm = left_border_;
        RightFragm = right_border_;
        LeftDif = 0;
        RightDif = 0;
    }

    int ns(0);

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->endsWith('.'))
            Count++;
    }

    if (Count > (4 * (end - beg) / 5))
        FlagStringParagraph = TRUE;

    if (Count >= ((end - beg) / 3))
        FlagStringParagraphSoft = TRUE;

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (ns == beg) {
            pRtfString->setParagraphBegin(true);
            continue;
        }

        pRtfStringPrev = (CRtfString*) strings_[ns - 1];

        if (((pRtfString->leftIndent() - LeftDif) > 2 * max_char_distance_)
                || ((pRtfStringPrev->rightIndent() - RightDif) > (RightFragm - LeftFragm) / 3)
                || ((pRtfString->leftIndent() > max_char_distance_) && pRtfString->startsWithDash())
                || (pRtfString->startsWithDigit() && (pRtfStringPrev->endsWith(';')
                        || pRtfStringPrev->endsWith('.'))) || (pRtfStringPrev->endsWith('.')
                && FlagStringParagraphSoft == TRUE && (pRtfStringPrev->rightIndent() - RightDif)
                > 5 * max_char_distance_) || (pRtfStringPrev->endsWith('.') && FlagStringParagraph
                == TRUE)) {
            pRtfStringPrev->setLineTransfer(false);
            pRtfString->setParagraphBegin(true);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfCentreJustification

Bool CRtfFragment::DeterminationOfCentreJustification(int beg, int end) {
    CRtfString *pRtfString;
    uint16_t CountLeftEqual = 0;
    uint16_t CountRightEqual = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;
    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);
    GetCountEqual(beg, end, &CountRightEqual, RTF_TP_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if ((CountCentreEqual < (end - beg) / 2 || CountCentreEqual < CountRightEqual
            || CountCentreEqual < CountLeftRightEqual || CountCentreEqual < CountLeftEqual)
            && (CountRightEqual + CountLeftRightEqual + CountLeftEqual > 0))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_CENTER);
    setLineTransfer(strings_.begin() + beg, strings_.begin() + end);
    pRtfString = (CRtfString*) strings_[beg];
    pRtfString->setParagraphBegin(true);

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== DeterminationOfCentreJustification ==================");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfRightJustification

Bool CRtfFragment::DeterminationOfRightJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;
    uint16_t CountLeftEqual = 0;
    uint16_t CountRightEqual = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;
    m_FlagCarry = GetFlagCarry(beg, end);

    if (m_FlagCarry && m_FlagRight == FALSE)
        return FALSE;

    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);
    GetCountEqual(beg, end, &CountRightEqual, RTF_TP_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if (CountRightEqual < (end - beg) / 2)
        return FALSE;

    if ((CountRightEqual < CountCentreEqual || CountRightEqual < CountLeftRightEqual
            || CountRightEqual < CountLeftEqual) && (CountCentreEqual + CountLeftRightEqual
            + CountLeftEqual > 0))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_RIGHT_ALLIGN);

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (ns == beg) {
            pRtfString->setParagraphBegin(true);
            pRtfString->setLineTransfer(true);
            continue;
        }

        pRtfString->setLineTransfer(true);
        pRtfStringPrev = (CRtfString*) strings_[ns - 1];

        if (pRtfStringPrev->endsWith('.')) {
            pRtfString->setParagraphBegin(true);
            pRtfStringPrev->setLineTransfer(false);
        }
    }

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== DeterminationOfRightJustification ==================");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfListType

Bool CRtfFragment::DeterminationOfListType(int beg, int end) {
    CRtfString *pRtfString;
    uchar FlagListParagraph = 0;
    int32_t MinLeft, MaxLeft, MaxRight;
    int32_t CountMinLeft = 0, CountMaxLeft = 0, CountMaxRight = 0;
    uint16_t CountCentreEqual = 0;
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if (CountCentreEqual == (end - beg))
        return FALSE;

    pRtfString = (CRtfString*) strings_[0];
    MinLeft = MaxLeft = pRtfString->leftBorder();
    MaxRight = pRtfString->rightBorder();
    int ns(0);

    //поиск тела списка
    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];
        MinLeft = MIN(MinLeft, pRtfString->leftBorder());
        MaxLeft = MAX(MaxLeft, pRtfString->leftBorder());
        MaxRight = MAX(MaxRight, pRtfString->rightBorder());
    }

    if ((MaxLeft - MinLeft) > (MaxRight - MaxLeft) / 2)
        return FALSE;

    if ((MaxLeft - MinLeft) < max_char_distance_)
        return FALSE;

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if ((abs(MinLeft - pRtfString->leftBorder()) > 5 * max_char_distance_) && (abs(MaxLeft
                - pRtfString->leftBorder()) > 5 * max_char_distance_))
            return FALSE;

        if (abs(MinLeft - pRtfString->leftBorder()) < max_char_distance_)
            CountMinLeft++;

        if (abs(MaxLeft - pRtfString->leftBorder()) < max_char_distance_)
            CountMaxLeft++;

        if (abs(MaxRight - pRtfString->rightBorder()) < max_char_distance_)
            CountMaxRight++;
    }

    if ((CountMinLeft > CountMaxLeft) || ((CountMinLeft + CountMaxLeft) < 4 * (end - beg) / 5)
            || (CountMaxRight < (end - beg) / 2))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_TYPE_LINE);

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if ((ns == beg) || abs(MinLeft - pRtfString->leftBorder()) < max_char_distance_)
            pRtfString->setParagraphBegin(true);
    }

    if (Config::instance().debugHigh())
        printResult(std::cerr, "\n ================== DeterminationOfListType ==================");
    return TRUE;
}

Bool CRtfFragment::DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo) {
    int32_t beg = 0, end;
    Bool Flag = TRUE;

    if (m_Attr == FALSE)
        return FALSE;

    while (Flag) {
        GetNextFragmentBegEnd(&beg, &end, &Flag);
        ReInit(SectorInfo, beg, end);

        if (DeterminationOfLeftRightJustification(beg, end) == FALSE) {
            if (DeterminationOfListType(beg, end) == FALSE) {
                if (DeterminationOfLeftJustification(beg, end, 0) == FALSE) {
                    if (DeterminationOfCentreJustification(beg, end) == FALSE) {
                        if (DeterminationOfRightJustification(beg, end) == FALSE) {
                            DeterminationOfLeftJustification(beg, end, 1);
                        }
                    }
                }
            }
        }

        beg = end;
    }

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== DeterminationOfMixedFragment ==================");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            ReInit

void CRtfFragment::ReInit(RtfSectorInfo* SectorInfo, int beg, int end) {
    int LeftDif, RightDif, CentreDif, top, bottom;
    CRtfString *pRtfStringPrev, *pRtfString;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfCharFirst, *pRtfCharLast;
    int ns(0);
    m_l_fragmentLocal = 32000;
    m_r_fragmentLocal = 0;
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountLeftRightEqual = 0;
    m_CountCentreEqual = 0;

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];
        pRtfString->setEqualLeft(false);
        pRtfString->setEqualRight(false);
        pRtfString->setEqualCenter(false);
    }

    //  I. Поиск:       Левой(m_l_fragmentLocal) и правой(m_r_fragmentLocal) границ фрагмента
    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (ns == beg) {
            if (!ns)
                pRtfString->setTopMargin(SectorInfo->VerticalOffsetFragmentInColumn);

            else {
                pRtfStringPrev = (CRtfString*) strings_[ns - 1];
                pRtfWord = pRtfStringPrev->firstWord();
                pRtfCharFirst = pRtfWord->firstChar();
                top = pRtfCharFirst->idealRect().bottom();
                pRtfWord = pRtfString->firstWord();
                pRtfCharFirst = pRtfWord->firstChar();
                bottom = pRtfCharFirst->idealRect().top();
                pRtfString->setTopMargin(bottom - top);
            }

            pRtfString->setParagraphBegin(true);
        }

        else
            pRtfString->setTopMargin(0);

        pRtfWord = pRtfString->firstWord();
        pRtfCharFirst = pRtfWord->firstChar();
        pRtfWord = pRtfString->lastWord();
        pRtfCharLast = pRtfWord->lastChar();
        m_l_fragmentLocal = MIN(m_l_fragmentLocal, (int16_t)pRtfCharFirst->idealRect().left());
        m_r_fragmentLocal = MAX(m_r_fragmentLocal, (int16_t)pRtfCharLast->idealRect().right());
    }

    // Присваиваются признаки равенства концов и середины соседних строк
    for (ns = beg + 1; ns < end; ns++) {
        pRtfStringPrev = (CRtfString*) strings_[ns - 1];
        pRtfString = (CRtfString*) strings_[ns];
        LeftDif = pRtfString->leftIndent() - pRtfStringPrev->leftIndent();
        RightDif = pRtfString->rightIndent() - pRtfStringPrev->rightIndent();
        CentreDif = pRtfString->center() - pRtfStringPrev->center();

        if (abs(LeftDif) <= max_char_distance_) {
            pRtfString->setEqualLeft(true);
            m_CountLeftEqual++;

            if (ns == beg) {
                pRtfStringPrev->setEqualLeft(true);
                m_CountLeftEqual++;
            }
        }

        if (abs(RightDif) <= max_char_distance_) {
            pRtfString->setEqualRight(true);
            m_CountRightEqual++;

            if (pRtfString->isEqualRight())
                m_CountLeftRightEqual++;

            if (ns == beg) {
                pRtfStringPrev->setEqualRight(true);
                m_CountRightEqual++;
                m_CountLeftRightEqual++;
            }
        }

        if ((abs(CentreDif) < max_char_distance_) && ((LeftDif <= 0 && RightDif <= 0) || (LeftDif
                > 0 && RightDif > 0)) && (abs(LeftDif) > max_char_distance_ / 2) && (abs(RightDif)
                > max_char_distance_ / 2)) {
            pRtfString->setEqualCenter(true);
            m_CountCentreEqual++;

            if (ns == beg) {
                pRtfStringPrev->setEqualCenter(true);
                m_CountCentreEqual++;
            }
        }
    }

    if (Config::instance().debugHigh())
        printResult(std::cerr, "\n ================== ReInit ==================");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetNextFragmentBegEnd

void CRtfFragment::GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag) {
    CRtfString *pRtfString;
    int i;
    *end = *beg + 1;

    for (i = *end; i < stringCount(); i++) {
        pRtfString = (CRtfString*) strings_[i];

        if (pRtfString->hasAttributes()) {
            *end = i;
            break;
        }
    }

    if ((*end >= (int32_t) stringCount()) || (i >= (int32_t) stringCount())) {
        *end = (int32_t) stringCount();
        *Flag = FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Done

void CRtfFragment::Done(void) {
    CheckOnceAgainImportancesFlagBeginParagraph();
    SetFirstLeftAndRightIndentOfParagraph();
    defineLineTransfer();
}

//-------------------------------------------------------------------------------------------------
//---------------------------------  HELPER FUNCTIONS  --------------------------------------------
//-------------------------------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////
//                 CheckOnceAgainImportancesFlagBeginParagraph

void CRtfFragment::CheckOnceAgainImportancesFlagBeginParagraph() {
    CRtfString *pRtfString;
    CRtfString *pRtfStringPrev;
    CRtfWord *pRtfWordPrev;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfChar;
    uint16_t CountWords;
    uint16_t CountChars;
    int ns(0);

    for (ns = 1; ns < stringCount(); ns++) {
        pRtfStringPrev = (CRtfString*) strings_[ns - 1];
        pRtfString = (CRtfString*) strings_[ns];

        // если выр. другая, то необходимо начать новый параграф
        if (pRtfString->align() != pRtfStringPrev->align()) {
            pRtfStringPrev->setLineTransfer(false);
            pRtfString->setParagraphBegin(true);
        }
    }

    for (ns = 1; ns < stringCount(); ns++) {
        pRtfStringPrev = (CRtfString*) strings_[ns - 1];
        pRtfString = (CRtfString*) strings_[ns];
        pRtfWordPrev = pRtfStringPrev->firstWord();
        pRtfWord = pRtfString->firstWord();

        if (pRtfString->align() != RTF_TP_CENTER && abs(pRtfWord->realFontSize()
                - pRtfWordPrev->realFontSize()) > 1) {
            pRtfStringPrev->setLineTransfer(false);
            pRtfString->setParagraphBegin(true);
        }
    }

    for (ns = 1; ns < stringCount(); ns++) {
        pRtfStringPrev = (CRtfString*) strings_[ns - 1];
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->isParagraphBegin()) {
            CountWords = pRtfStringPrev->wordCount();
            pRtfWord = pRtfStringPrev->lastWord();
            pRtfChar = pRtfWord->lastChar();

            if (pRtfChar->first().isHyphen() && pRtfChar->m_bFlg_spell_nocarrying) {
                if (pRtfString->align() == pRtfStringPrev->align())
                    pRtfString->setParagraphBegin(false);

                else if (pRtfStringPrev->align() == RTF_TP_LEFT_AND_RIGHT_ALLIGN
                        && pRtfString->align() == RTF_TP_LEFT_ALLIGN) {
                    pRtfString->setAlign(RTF_TP_LEFT_AND_RIGHT_ALLIGN);
                    pRtfStringPrev->setLineTransfer(false);
                    pRtfString->setParagraphBegin(false);
                }
            }
        }
    }

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== CheckOnceAgainImportancesFlagBeginParagraph ==================");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                          SetFirstLeftAndRightIndentOfParagraph

void CRtfFragment::SetFirstLeftAndRightIndentOfParagraph() {
    CRtfString *pRtfString;
    CRtfString *pRtfStringNext;
    int16_t MinLeftIndent;
    int16_t MinRightIndent;
    int i;
    int ns(0);
    int16_t twp_dist;
    int16_t Dif = 0;
    twp_dist = (int16_t) (3 * max_char_distance_ * getTwips());

    for (ns = 0; ns < stringCount(); ns++) {
        pRtfString = (CRtfString*) strings_[ns];
        pRtfString->calcRealLength();
        pRtfString->setLeftIndent(pRtfString->leftIndent() * getTwips()
                + m_LeftOffsetFragmentFromVerticalColumn);
        pRtfString->setRightIndent(pRtfString->rightIndent() * getTwips()
                + m_RightOffsetFragmentFromVerticalColumn);
        pRtfString->setRightIndent(
                MIN(pRtfString->rightIndent(),
                        m_WidthVerticalColumn - (pRtfString->realLength() + pRtfString->leftIndent() + pRtfString->rightIndent())));
    }

    for (ns = 0; ns < stringCount(); ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->isParagraphBegin()) {
            if (pRtfString->align() == RTF_TP_LEFT_ALLIGN) {
                Dif = MAX(0, m_WidthVerticalColumn - pRtfString->realLength());
                MinLeftIndent = pRtfString->leftIndent();

                for (i = ns + 1; i < stringCount(); i++) {
                    pRtfStringNext = (CRtfString*) strings_[i];

                    if (pRtfStringNext->isParagraphBegin() || pRtfStringNext->align()
                            != RTF_TP_LEFT_ALLIGN)
                        break;

                    MinLeftIndent = MIN(pRtfStringNext->leftIndent(), MinLeftIndent);
                }

                if (m_WidthVerticalColumn > pRtfString->lengthInTwips()) {
                    pRtfString->setFirstIndent(pRtfString->leftIndent() - MinLeftIndent);

                    if (pRtfString->firstIndent() < (twp_dist / 3))
                        pRtfString->setFirstIndent(0);

                    pRtfString->setLeftIndent(MIN(Dif, MinLeftIndent));
                }

                else {
                    pRtfString->setLeftIndent(0);
                    pRtfString->setFirstIndent(0);
                }

                pRtfString->setRightIndent(0);

                if (pRtfString->leftIndent() < (twp_dist / 2))
                    pRtfString->setLeftIndent(0);

                if (i == (ns + 1)) {
                    pRtfString->setFirstIndent(pRtfString->leftIndent());
                    pRtfString->setLeftIndent(0);
                }

                continue;
            }

            if (pRtfString->align() == RTF_TP_RIGHT_ALLIGN) {
                pRtfString->setLeftIndent(0);
                pRtfString->setFirstIndent(0);
                pRtfString->setRightIndent(0);
                continue;
            }

            if (pRtfString->align() == RTF_TP_TYPE_LINE) {
                if (ns + 1 < stringCount()) {
                    pRtfStringNext = (CRtfString*) strings_[ns + 1];

                    if (!pRtfStringNext->isParagraphBegin() && pRtfStringNext->align()
                            == RTF_TP_TYPE_LINE) {
                        pRtfString->setFirstIndent(pRtfStringNext->leftIndent()
                                - pRtfString->leftIndent());
                        pRtfString->setLeftIndent(pRtfStringNext->leftIndent());
                    }

                    else
                        pRtfString->setFirstIndent(0);
                }

                else
                    pRtfString->setFirstIndent(0);

                continue;
            }

            if (pRtfString->align() == RTF_TP_LEFT_AND_RIGHT_ALLIGN) {
                MinLeftIndent = pRtfString->leftIndent();
                MinRightIndent = pRtfString->rightIndent();

                for (i = ns + 1; i < stringCount(); i++) {
                    pRtfStringNext = (CRtfString*) strings_[i];

                    if (pRtfStringNext->isParagraphBegin() || pRtfStringNext->align()
                            != RTF_TP_LEFT_AND_RIGHT_ALLIGN)
                        break;

                    MinLeftIndent = MIN(pRtfStringNext->leftIndent(), MinLeftIndent);
                    MinRightIndent = MIN(pRtfStringNext->rightIndent(), MinRightIndent);
                }

                if (MinLeftIndent < (twp_dist / 3))
                    MinLeftIndent = 0;

                pRtfString->setFirstIndent(pRtfString->leftIndent() - MinLeftIndent);

                if (pRtfString->firstIndent() < (twp_dist / 3))
                    pRtfString->setFirstIndent(0);

                if (MinLeftIndent < twp_dist)
                    pRtfString->setLeftIndent(0);

                else
                    pRtfString->setLeftIndent(MinLeftIndent);

                if (MinRightIndent < twp_dist)
                    pRtfString->setRightIndent(0);

                if (i == (ns + 1)) {
                    if (MinLeftIndent > ((2 * twp_dist) / 3))
                        pRtfString->setLeftIndent(MinLeftIndent);

                    pRtfString->setFirstIndent(pRtfString->leftIndent());
                    pRtfString->setLeftIndent(0);
                    pRtfString->setRightIndent(0);
                }

                continue;
            }

            if (pRtfString->align() == RTF_TP_CENTER) {
                MinLeftIndent = pRtfString->leftIndent();
                MinRightIndent = pRtfString->rightIndent();

                for (i = ns; i < stringCount(); i++) {
                    pRtfStringNext = (CRtfString*) strings_[i];
                    MinLeftIndent = MIN(pRtfStringNext->leftIndent(), MinLeftIndent);
                    MinRightIndent = MIN(pRtfStringNext->rightIndent(), MinRightIndent);

                    if (pRtfString->align() != RTF_TP_CENTER)
                        break;
                }

                if (abs(MinLeftIndent - MinRightIndent) < (2 * max_char_distance_))
                    MinLeftIndent = MinRightIndent = 0;

                pRtfString->setFirstIndent(0);
                pRtfString->setLeftIndent(MinLeftIndent);
                pRtfString->setRightIndent(MinRightIndent);
            }
        }
    }

    if (Config::instance().debugHigh())
        printResult(std::cerr,
                "\n ================== SetFirstLeftAndRightIndentOfParagraph ==================");
}

void CRtfFragment::defineLineTransfer() {
    for (StringIterator it = strings_.begin(), end = strings_.end(); it != end; ++it) {
        if ((*it)->isParagraphBegin() && (RfrmtOptions::lineTransfer() || (*it)->align()
                == RTF_TP_CENTER)) {
            setLineTransfer(it, findParagraph(it, end));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagLeft

Bool CRtfFragment::GetFlagLeft(int beg, int end) {
    CRtfString* pRtfString;
    int Count = 0;
    uint16_t CountLeftEqual = 0;
    Bool PriznakLeft = FALSE;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->endsWith(',')) {
            Count++;

            if (pRtfString->rightIndent() > (pRtfString->width()) / 4)
                PriznakLeft = TRUE;
        }
    }

    if (Count > 1 && PriznakLeft)
        return TRUE;

    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);

    if (CountLeftEqual == (end - beg))
        return TRUE;

    m_FlagBigSpace = GetFlagBigSpace(beg, end);
    m_FlagCarry = GetFlagCarry(beg, end);

    if (!m_FlagBigSpace && !m_FlagCarry)
        return TRUE;

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagStrongLeft

Bool CRtfFragment::GetFlagStrongLeft(int beg, int end) {
    int Count = 0;
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->endsWith('.') || pRtfString->endsWith(','))
            Count++;
    }

    if (Count == end - beg)
        return TRUE;

    if ((end - beg < 5) && Count > 2)
        return TRUE;

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagRight

Bool CRtfFragment::GetFlagRight(int beg, int end) {
    CRtfString *pRtfString;
    int ns(0);

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->rightIndent() > max_char_distance_)
            return FALSE;
    }

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->leftIndent() > ((right_border_ - left_border_) / 2))
            return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagCarry

Bool CRtfFragment::GetFlagCarry(int beg, int end) {
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        if (pRtfString->lineCarry())
            return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagBigSpace

Bool CRtfFragment::GetFlagBigSpace(int beg, int end) {
    CRtfString* pRtfString;
    CRtfWord* pRtfWordPrev, *pRtfWordCur;
    CRtfChar* pRtfCharPrev, *pRtfCharCur;
    int FlagBigSpase = 0;
    int CountCharInPrevWord;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings_[ns];

        for (int i = 1; i < pRtfString->wordCount(); i++) {
            pRtfWordPrev = pRtfString->wordAt(i - 1);
            pRtfWordCur = pRtfString->wordAt(i);
            pRtfCharPrev = pRtfWordPrev->lastChar();
            pRtfCharCur = pRtfWordCur->firstChar();

            if ((pRtfCharCur->idealRect().left() - pRtfCharPrev->idealRect().right()) > 2
                    * max_char_distance_)
                FlagBigSpase = 1;
        }
    }

    return FlagBigSpase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   SetParagraphAlignment

void CRtfFragment::SetParagraphAlignment(int beg, int end, rtf_align_t AlignType) {
    CRtfString *pRtfString;
    int i;
    beg = MAX(0, beg);

    for (i = beg; i < end; i++) {
        pRtfString = (CRtfString*) strings_[i];
        pRtfString->setAlign(AlignType);
    }
}

void CRtfFragment::printResult(std::ostream& os, const char * header_str) const {
    os << header_str << std::endl;

    os << "max char distance: " << max_char_distance_ << "\n";

    for (StringIteratorConst it = strings_.begin(), e = strings_.end(); it != e; ++it) {
        os << "String #" << std::distance(it, strings_.begin()) << "\n";
        (*it)->print(os);
        os << std::endl;
    }
}

}
