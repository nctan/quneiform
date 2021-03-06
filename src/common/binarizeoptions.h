/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef BINARIZEOPTIONS_H
#define BINARIZEOPTIONS_H

#include <map>
#include <string>
#include <boost/variant.hpp>

#include "globus.h"
#include "common/exception.h"
#include "common/binarizatordef.h"

namespace cf {

class CLA_EXPO BinarizeOptions
{
public:
    typedef RuntimeExceptionImpl<BinarizeOptions> Exception;
public:
    BinarizeOptions();
    BinarizeOptions(binarizator_t t);

    bool hasOption(const std::string& key) const;

    bool optionBool(const std::string& key, bool fallback) const;
    float optionFloat(const std::string& key, float fallback) const;
    int optionInt(const std::string& key, int fallback) const;
    std::string optionString(const std::string& key, const std::string& fallback) const;

    void setOption(const std::string& key, bool value);
    void setOption(const std::string& key, float value);
    void setOption(const std::string& key, int value);
    void setOption(const std::string&key, const char * value);
    void setOption(const std::string&key, const std::string& value);

    binarizator_t binarizator() const;
    void setBinarizator(binarizator_t t);
private:
    typedef boost::variant<bool, int, float, std::string> Value;
    typedef std::map<std::string, Value> OptionMap;
private:
    binarizator_t binarizator_;
    OptionMap options_;
};

}

#endif // BINARIZEOPTIONS_H
