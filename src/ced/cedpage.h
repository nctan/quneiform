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

#include <string>
#include <vector>
#include "globus.h"
#include "ced_struct.h"
#include "common/size.h"

namespace CIF
{

class FUN_EXPO__ CEDPage
{
    public:
        /**
         * Removes all pictures from page
         */
        void clearPictures();

        void clearSections();

        /**
         * Returns pointer to picture by given picture number
         * @return NULL if nothing found
         */
        PictureEntry * findPictureByNumber(int number) const;

        /**
         * Returns image dpi
         * @see setImageDpi()
         */
        Size imageDpi() const;

        /**
         * Returns image name
         * @see setImageName()
         */
        std::string imageName() const;

        /**
         * Returns size of the original image in pixels
         * @see setImageSize()
         */
        Size imageSize() const;

        /**
         * Returns page language
         * @see setLanguage
         */
        language_t language() const;

        /**
         * Returns page borders in twips
         * @see pageSize(), setPageBorder()
         */
        Rect pageBorder() const;

        /**
         * Number of Pages (= 0 not in batch mode)
         * @see setPageNumber()
         */
        int pageNumber() const;

        /**
         * Returns page size in twips
         * @note 1 inch == 1440 twips for text editor
         * @see setPageSize()
         */
        Size pageSize() const;

        /**
         * Returns pointer to picture with given number
         */
        PictureEntry * pictureAt(size_t pos) const;

        /**
         * Returns count of pictures
         */
        size_t pictureCount() const;

        /**
         * Returns pointer to section with given number
         */
        CEDSection * section(int num);

        /**
         * Returns number of sections
         */
        int sectionCount() const;

        /**
         * sets new value of current section
         */
        CEDSection * setCurrentSection(CEDSection* sect);

        /**
         * sets new value of current section
         */
        CEDSection * setCurrentSection(int number);

        /**
         * Sets image dpi
         * @see imageDpi()
         */
        void setImageDpi(const Size& dpi);

        /**
         * Sets image path
         * If the path is not specified, is searched in one
         * Directory with the file ed
         */
        void setImageName(const std::string& filename);

        /**
         * Sets image size in pixels
         * @see imageSize()
         */
        void setImageSize(const Size& size);

        /**
         * Sets page language
         * @see language()
         */
        void setLanguage(language_t language);

        /**
         * Sets page borders in twips
         * @see pageBorder()
         */
        void setPageBorder(const Rect& borders);

        /**
         * Sets page number
         * @see pageNumber()
         */
        void setPageNumber(int number);

        /**
         * Sets page size in twips
         * @see pageSize()
         */
        void setPageSize(const Size& size);

        /**
         * Sets page turn
         * @see turn()
         * @param angle  - tangent angle on the vertical images * 2048
         */
        void setTurn(int angle);

        /**
         * Sets unrecognized char
         * @see unrecongizedChar()
         */
        void setUnrecognizedChar(char chr);

        /**
         * Returns tangent angle on the vertical images * 2048
         * @see setTurn()
         */
        int turn() const;

        /**
         * Returns unrecognized char
         * @see setUnrecognizedChar()
         */
        char unrecognizedChar() const;

        int fontsUsed; //РљThe number of fonts used in table
        int fontsCreated; //РљThe number of fonts created in the table
        fontEntry* fontTable; // Pointer to the table fonts

        CEDPage();
        ~CEDPage();

        Bool32 FormattedWriteRtf(const char * fileName, Bool merge);

        CEDParagraph * GetParagraph(int _num);
        CEDLine * GetLine(int _num);
        CEDChar * GetChar(int _num);

        Bool32 GoToNextSection();
        Bool32 GoToNextParagraph(Bool32 NonFictiveOnly);
        Bool32 GoToNextLine();
        Bool32 GoToNextChar();

        int GetNumberOfParagraphs();
        int GetNumberOfLines();
        int GetNumberOfChars();

        Bool32 CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,
                const char* fontName);
        Bool32 GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily,
                uchar* fontCharset, char** fontName);

        int GetFontByNum(uchar fontNumber);

        Bool32 CreatePicture(int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal,
                int pictAlign, int type, void * data, int len);

        CEDSection * InsertSection(); //inserts new section after current one. inserted one becomes current
        //sets pointer to the inserted one

        CEDSection * GetCurSection(); //returns current section
        int GetNumOfCurSection(); //returns current section

        CEDSection * NextSection(); //returns next section, 0 if last
        CEDSection * PrevSection(); //return previous section, 0 if first

        int NumberOfParagraphs, NumberOfLines, NumberOfChars;
        int section_num_;
    private:
        Size image_size_;
        Size image_dpi_;
        Size page_size_;
        Rect page_borders_;
        std::string image_name_;
        int turn_;
        int page_number_;
        language_t language_;
        char unrecognized_char_;
        CEDSection * sections; //connected list of sections
        CEDSection * curSect; //current section

        typedef std::vector<PictureEntry*> PictureList;
        PictureList pictures_;
};

}

#endif /* CEDPAGE_H_ */
