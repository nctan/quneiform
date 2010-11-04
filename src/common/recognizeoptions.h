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

#ifndef RECOGNIZEOPTIONS_H_
#define RECOGNIZEOPTIONS_H_

#include <iostream>
#include <string>
#include "lang_def.h"
#include "globus.h"

namespace cf {

class CLA_EXPO RecognizeOptions
{
    public:
        RecognizeOptions();

        enum table_mode_t
        {
            TABLE_NONE = 0,
            TABLE_DEFAULT,
            TABLE_ONLY_LINE,
            TABLE_ONLY_TEXT,
            TABLE_LINE_TEXT
        };

        bool autoRotate() const;
        bool dotMatrix() const;
        bool fax() const;
        bool pictureSearch() const;
        table_mode_t tableMode() const;
        const std::string& userDict() const;

        /**
         * Sets recognition language
         */
        language_t language() const;
        bool oneColumn() const;
        bool spellCorection() const;

        void setAutoRotate(bool value);
        void setFax(bool value);
        void setDotMatrix(bool value);
        void setLanguage(language_t language);

        /**
         * Sets one column layout
         */
        void setOneColumn(bool value);
        void setPictureSearch(bool value);
        void setSpellCorrection(bool value);
        void setTableMode(table_mode_t mode);
        void setUserDict(const std::string& user_dict);
    private:
        language_t language_;
        bool auto_rotate_;
        bool dot_matrix_;
        bool do_spell_correction_;
        bool fax100_;
        bool one_column_;
        bool find_pictures_;
        table_mode_t table_mode_;
        std::string user_dict_name_;
};

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const RecognizeOptions& opts);

}

#endif /* RECOGNIZEOPTIONS_H_ */