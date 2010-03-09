/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef PUMAIMPL_H_
#define PUMAIMPL_H_

#include <string>
#include <iosfwd>
#include <memory>

#include "pumadef.h"
#include "cfcompat.h"
#include "rfrmt/formatoptions.h"
#include "specprj.h"
#include "common/rect.h"
#include "common/exception.h"
#include "common/memorybuffer.h"
#include "common/outputformat.h"
#include "cimage/imageinfo.h"
#include "export/exporter.h"
#include "rdib/image.h"
#include "globus.h"

class CTIControl;
struct CCOM_cont;

namespace CIF
{

class RMarker;
class ComponentExtractor;
class CEDPage;

Bool32 IsUpdate(uint32_t flg);
void SetUpdate(uint32_t flgAdd, uint32_t flgRemove);

class CLA_EXPO PumaImpl
{
    public:
        PumaImpl();
        ~PumaImpl();

        /**
         * Append recognition results to specified file
         * @note not all formats support this type of saving
         * @throw PumaException on error
         */
        void append(const std::string& filename, format_t format) const;

        /**
         * Make some cleanup after recognition
         */
        void close();

        /**
         * Returns format options
         */
        FormatOptions formatOptions() const;

        /**
         * Opens image for recognition
         */
        void open(ImagePtr img);

        /**
         * Returns image working area
         */
        Rect pageTemplate() const;

        /**
         * Recognizes image
         */
        void recognize();

        /**
         * Saves to file recognition result in specified format
         * @param filename - file to save
         * @param format - document format
         */
        void save(const std::string& filename, format_t format) const;

        /**
         * Saves to stream recognition result in specified format
         * @note not all export abilities are available in this mode
         * (such as saving pictures) it depends from format
         */
        void save(std::ostream& os, format_t format) const;

        /**
         * Sets format options
         */
        void setFormatOptions(const FormatOptions& opt);
        void setOptionAutoRotate(bool val);
        void setOptionDotMatrix(bool val);
        void setOptionFax100(bool val);
        void setOptionFormatMode(puma_format_mode_t format);

        /**
         * Sets recognition language
         */
        void setOptionLanguage(language_t language);

        /**
         * Sets one column layout
         */
        void setOptionOneColumn(bool val);
        void setOptionPictures(puma_picture_t type);
        void setOptionTable(puma_table_t mode);
        void setOptionUserDictionaryName(const char * name);

        /**
         * Sets use speller after recognition process
         */
        void setOptionUseSpeller(bool value = true);

        /**
         * Sets working region on input image
         */
        void setPageTemplate(const Rect& r);
        void setSpecialProject(special_project_t SpecialProject);
    public:
        static unsigned char * mainBuffer();
        static unsigned char * workBuffer();
        static const size_t MainBufferSize = 500000;
        static const size_t WorkBufferSize = 180000;
    private:
        void binarizeImage();
        void clearAll();
        void extractComponents();
        void extractStrings();
        void formatResult();
        void getImageInfo(const std::string& image_name);
        void layout();
        void loadLayoutFromFile(const std::string& fname);
        ExporterPtr makeExporter(format_t format) const;
        void modulesDone();
        void modulesInit();
        const char * modulePath() const;
        const char * moduleTmpPath() const;
        void normalize();
        void pass1();
        void pass2();
        void pass2special();
        void preOpenInitialize();
        void preprocessImage();
        void printRecognizeOptions();
        void printResult(std::ostream& os);
        void printResultLine(std::ostream& os, size_t lineNumber);
        void postOpenInitialize();
        void recognizeCorrection();
        void recognizePass1();
        void recognizePass2();
        void recognizeSetup();
        void recognizeSpecial();
        void rotate(void * dib, Point * p);
        void saveCSTR(int pass);
        void saveLayoutToFile(const std::string& fname);
        void saveToText(std::ostream& os) const;
        void saveToText(const std::string& filename) const;
        void spellCorrection();
    private:
        static FixedBuffer<unsigned char, MainBufferSize> main_buffer_;
        static FixedBuffer<unsigned char, WorkBufferSize> work_buffer_;
    private:
        std::auto_ptr<CTIControl> cimage_;
        std::auto_ptr<RMarker> rmarker_;
        std::auto_ptr<ComponentExtractor> comp_extractor_;
        BitmapInfoHeader info_;
        Rect rect_template_;
        bool do_spell_corretion_;
        bool fax100_;
        bool one_column_;
        bool dot_matrix_;
        bool auto_rotate_;
        language_t language_;
        std::string user_dict_name_;
        std::string input_filename_;
        std::string layout_filename_;
        FormatOptions format_options_;
        puma_picture_t pictures_;
        puma_table_t tables_;
        void * input_dib_;
        void * recog_dib_;
        int tables_num_;
        CCOM_cont * ccom_;
        Handle cpage_;
        Handle lines_ccom_;
        Handle cline_;
        CEDPage * ed_page_;
        Bool32 rc_line_;
        Bool32 kill_vsl_components_;
        Bool32 need_clean_line_;
        const char * recog_name_;
        special_project_t special_project_;
};

typedef RuntimeExceptionImpl<PumaImpl> PumaException;

}

#endif /* PUMAIMPL_H_ */
