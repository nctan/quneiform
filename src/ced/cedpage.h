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

#ifndef CEDPAGE_H_
#define CEDPAGE_H_

#include "globus.h"
#include "ced_struct.h"
#include <common/size.h>

namespace CIF
{

class FUN_EXPO__ CEDPage
{
    public:
        /**
         * Returns image dpi
         */
        Size imageDpi() const;

        /**
         * Returns size of the original image in pixels
         */
        Size imageSize() const;

        /**
         * Number of Pages (= 0 not in batch mode)
         */
        int pageNumber() const;

        /**
         * Sets image dpi
         */
        void setImageDpi(const Size& dpi);

        /**
         * Sets image size in pixels
         */
        void setImageSize(const Size& size);

        /**
         * Sets page number
         * @see pageNumber
         */
        void setPageNumber(int number);

        /**
         * Sets page turn
         * @see turn()
         * @param angle  - tangent angle on the vertical images * 2048
         */
        void setTurn(int angle);

        /**
         * Returns tangent angle on the vertical images * 2048
         */
        int turn() const;

        //picture data
        char* imageName; // Filename image. If the path is not specified, is searched in one
        // Directory with the file ed

        EDSIZE pageSizeInTwips; // The width of the page in twip (1dyuym = 1440tvipov) for text editor
        EDRECT pageBordersInTwips;
        char unrecogChar;
        char recogLang;
        Bool32 resizeToFit;

        int fontsUsed; //РљThe number of fonts used in table
        int fontsCreated; //РљThe number of fonts created in the table
        fontEntry* fontTable; // Pointer to the table fonts
        int picsUsed; //РљNumber of images used in table
        int picsCreated; //РљNumber of images created in the table
        pictEntry* picsTable; // pointer to a table of images

        CEDPage();
        ~CEDPage();

        Bool32 FormattedWriteRtf(const char * fileName, Bool merge);

        CEDSection * GetSection(int _num);
        CEDParagraph * GetParagraph(int _num);
        CEDLine * GetLine(int _num);
        CEDChar * GetChar(int _num);

        Bool32 GoToNextSection();
        Bool32 GoToNextParagraph(Bool32 NonFictiveOnly);
        Bool32 GoToNextLine();
        Bool32 GoToNextChar();

        int GetNumberOfSections();
        int GetNumberOfParagraphs();
        int GetNumberOfLines();
        int GetNumberOfChars();

        Bool32 CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,
                char* fontName);
        Bool32 GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily,
                uchar* fontCharset, char** fontName);

        int GetFontByNum(uchar fontNumber);

        Bool32 CreatePicture(int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal,
                int pictAlign, int type, void * data, int len);

        CEDSection * InsertSection(); //inserts new section after current one. inserted one becomes current
        //sets pointer to the inserted one
        //  CEDSection * DeleteSection(Bool32 _deleteSubItems); //deletes current section. previous one becomes current
        //return it
        //_deleteSubItems - either delete all daughter elements or attach it to previous object
        CEDSection * SetCurSection(CEDSection* _sect);//sets new value of current section
        CEDSection * SetCurSection(int _number);//sets new value of current section

        CEDSection * GetCurSection(); //returns current section
        int GetNumOfCurSection(); //returns current section

        CEDSection * NextSection(); //returns next section, 0 if last
        CEDSection * PrevSection(); //return previous section, 0 if first

        int NumberOfSections, NumberOfParagraphs, NumberOfLines, NumberOfChars;

        CEDSection * sections; //connected list of sections
        CEDSection * curSect; //current section
    private:
        Size image_size_;
        Size image_dpi_;
        int turn_;
        int page_number_;
};

}

#endif /* CEDPAGE_H_ */
