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

#ifndef SIZE_H_
#define SIZE_H_

namespace CIF
{

template<class T>
class SizeImpl
{
    public:
        SizeImpl() :
                width_(0), height_(0) {
        }

        SizeImpl(T width, T height) :
                width_(width), height_(height) {
        }

        bool isValid() const {
            return width >= 0 && height_ >= 0;
        }

        T height() const {
            return height_;
        }

        template<class U>
        bool operator==(const SizeImpl<U>& sz) {
            return width_ == sz.width() && height_ == sz.height();
        }

        template<class U>
        bool operator!=(const SizeImpl<U>& sz) {
            return width_ != sz.width() || height_ != sz.height();
        }

        template<class U>
        void operator=(const SizeImpl<U>& sz) {
            setWidth(sz.width());
            setHeight(sz.height());
        }

        T& rheight() {
            return height_;
        }

        T& rwidth() {
            return width_;
        }

        SizeImpl& scale(double factor) {
            width_ *= factor;
            height_ *= factor;
            return *this;
        }

        void setHeight(T h) {
            height_ = h;
        }

        void setWidth(T w) {
            width_ = w;
        }

        void set(T width, T height) {
            height_ = height;
            width_ = width;
        }

        T width() const {
            return width_;
        }

    private:
        T width_, height_;
};

template<class T, class U>
bool SizeContentEqual(const SizeImpl<T>& s0, const SizeImpl<U>& s1)
{
    return s0.height() * s0.width() == s1.height() * s1.width();
}

typedef SizeImpl<int> Size;

}

#endif /* SIZE_H_ */