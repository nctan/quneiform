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

#ifndef CRTFPAGE_H_
#define CRTFPAGE_H_

#include <vector>

#include "globus.h"
#include "common/size.h"
#include "cfcompat.h"
#include "crtfstruct.h"
#include "common/font.h"

namespace cf
{
class CEDPage;
class CRtfFragment;
class CRtfSector;
class CRtfPage;
class FormatOptions;

class CLA_EXPO CRtfPage
{
    public:
        CRtfPage(const std::string& imageName);
        ~CRtfPage();

        /**
         * Adds fragment to page
         */
        void addFragment(CRtfFragment * frag);

        void addPictures();

        void addTables();

        /**
         * Returns page bounds
         */
        Rect bRect() const;

        /**
         * Removes fragments
         */
        void clearFragments();

        /**
         * Removes sectors
         */
        void clearSectors();

        /**
         * Returns page size
         */
        Size pageSize() const;

        /**
         * Reads data from format file
         */
        void readInternalFile(FILE * file);

        /**
         * Sets formatting options
         */
        void setFormatOptions(const FormatOptions& opts);

        /**
         * Sets image name
         */
        void setImageName(const std::string& name);

        /**
         * Sets symbol that inserted instead of unrecognized characters
         */
        void setUnrecognizedChar(char ch);

        /**
         * Sorts page fragments by user numbers
         */
        void sortByUserNumber();

        /**
         * Exports CRtfPage to ced
         * @return pointer to CED page
         * @note caller should free result
         */
        CEDPage * Write();

        void setFragmentsInColumn(const CRtfFragment * cur_fragm);

        Bool FindPageTree(FILE *FileNameIn);
        void CorrectKegl(void);
        void ChangeKegl(void);
        void AddNewKegl(int16_t OldKegl, int16_t NewKegl);
        int16_t GetNewKegl(int16_t OldKegl);
        int16_t GetMinKegl(int16_t OldKegl);
        void AddLines(void);
        void ToPlacePicturesAndTables(void);
        void SetPaperSize(int32_t LeftPos, int32_t RightPos, int32_t TopPos, int32_t BottomPos,
                Size& size, int32_t* MargL, int32_t* MargR, int32_t* MargT, int32_t* MargB);

        typedef std::vector<CRtfFragment*> FragmentList;
        FragmentList m_arFragments;
        typedef std::vector<CRtfSector*> SectorList;
        SectorList m_arSectors;

        RtfPageElementCount Count;
        uint16_t m_wDpi;
        RECT m_rect;
        int32_t MargL;
        int32_t MargR;
        int32_t MargT;
        int32_t MargB;

        int32_t InitMargL;
        int32_t InitMargR;
        int32_t InitMargT;
        int32_t InitMargB;

        int m_nCurSectorNumber;
    private:
        CRtfPage(const CRtfPage&);
        void calcPageSize();
        void calcPageSizeCommon();
        void calcPageSizeFrames();
        void calcPageSizeNone();
        int freeSpaceBetweenSectors(CRtfSector* first, CRtfSector* second);
        /* Ишется следуюший фрагмент по пользовательскому номеру */
        int getFlagAndNumberFragment(int * FragmentType, int * CurrentSectorNumber);
        void initCedPage();
        int maxFragmentWidth() const;
        void writeSectorsHeader(int i);
        /* fragments written by user numbers */
        void writeUsingNone();
        void writeUsingFrames();
        void writeUsingFramesAndColumns();
    private:
        std::string image_name_;
        char unrecognized_char_;
        language_t language_;
        Size page_size_;
        CEDPage * ced_page_;
        bool bad_column_;
        std::vector<KEGL> arKegls;
};
}

#endif /* CRTFPAGE_H_ */
