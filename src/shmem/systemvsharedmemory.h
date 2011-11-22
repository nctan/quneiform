/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef SYSTEMVSHAREDMEMORY_H
#define SYSTEMVSHAREDMEMORY_H

#include "sharedmemoryholderprivate.h"

namespace cf {

class SystemVSharedMemory : public SharedMemoryHolderPrivate
{
public:
    SystemVSharedMemory();
    void close(void * mem);
    void * create(size_t key, size_t size);
    void * open(size_t key, size_t size);
    bool remove();
private:
    int id_;
};

}

#endif // SYSTEMVSHAREDMEMORY_H
