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

#ifndef ICONVIMPL_H_
#define ICONVIMPL_H_

#include <string>
#include <iconv.h>

namespace cf
{

class IconvImpl
{
    public:
        IconvImpl();
        IconvImpl(const std::string& from, const std::string& to);
        ~IconvImpl();

        bool close();
        std::string convert(const std::string& src);
        std::string convert(unsigned char chr);
        bool isOpened() const;
        bool open(const std::string& from, const std::string& to);
    private:
        size_t convert(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
        iconv_t iconv_;
};

}

#endif /* ICONV_IMPL_H_ */
