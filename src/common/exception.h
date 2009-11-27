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

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <stdexcept>

namespace CIF
{

template<class T>
class RuntimeExceptionImpl: public std::runtime_error
{
    public:
        RuntimeExceptionImpl(const std::string& msg, int code = 0);
        int code() const;
    private:
        int code_;
};

template<class T>
RuntimeExceptionImpl<T>::RuntimeExceptionImpl(const std::string& msg, int code) :
        std::runtime_error(msg), code_(code)
{
}

template<class T>
int RuntimeExceptionImpl<T>::code() const
{
    return code_;
}

}

#endif /* EXCEPTION_H_ */