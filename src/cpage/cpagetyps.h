/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Cognitive Technologies nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __CPAGETYPS_H__
#define __CPAGETYPS_H__

#include "table.h"
#include "polyblock.h"

// PAGES
#define PT_EMPTY    CPAGE_GetInternalType("__Page__")
#define PT_PAGEINFO CPAGE_GetInternalType("__PageInfo__")

class PAGEINFO
{
    public:
        PAGEINFO() :
            Page(0), DPIX(0), DPIY(0), BitPerPixel(0), X(0), Y(0), Width(0), Height(0),
                    Incline2048(0), Angle(0), SkewLocVerLin2048(0), Images(0), status(0) {
            szImageName[0] = 0;
        }

        void addFlag(int flag) {
            Images |= flag;
        }

        bool hasFlag(int flag) const {
            return Images & flag;
        }

        /**
          * Sets page turn angle
          * @note allowed values: 0, 90, 180, 270
          * @see turnAngle()
          */
        void setTurnAngle(int angle)
        {
            Angle = angle % 360;
        }

        /**
          * Returns page turn angle
          * @see setTurnAngle()
          */
        int turnAngle() const {
            return Angle;
        }

        char szImageName[CPAGE_MAXNAME];// имя блока распознаваемого изображения в контейнере RIMAGE
        uint32_t Page; // номер страницы
        uint32_t DPIX;
        uint32_t DPIY;
        uint32_t BitPerPixel;
        int32_t X; // смещение относительно глобальных координат
        int32_t Y;
        uint32_t Width; // размеры в пиксеалх
        uint32_t Height;
        int32_t Incline2048; // наклон страницы в долях от 2048
        uint32_t Angle;// угол поворота страницы 0, 90, 180, 270
        int32_t SkewLocVerLin2048;//вертикальное отклонение в долях 2048
#define IMAGE_USER      1
#define IMAGE_BINARIZE  2
#define IMAGE_DELLINE   4
#define IMAGE_ROTATE    8
#define IMAGE_ORTOMOVE  16
#define IMAGE_TURN      32
        uint16_t Images;//существующие изображения
        uint32_t status;
#define PINFO_USERTEMPLATE  1  // темплейт установлен пользователем
#define PINFO_AUTOTEMPLATE  2  // темплейт установлен автоматически
};

CPageHandle CreateEmptyPage();

// BLOCKS
#define TYPE_CPAGE_TABLE    CPAGE_GetInternalType("TYPE_CPAGE_TABLE")
#define TYPE_CPAGE_PICTURE  CPAGE_GetInternalType("TYPE_CPAGE_PICTURE")
#define TYPE_TEXT         CPAGE_GetInternalType("TYPE_TEXT")
#define TYPE_IMAGE        CPAGE_GetInternalType("TYPE_IMAGE")
#define TYPE_TABLE        CPAGE_GetInternalType("TYPE_TABLE")
#define TYPE_DESC         CPAGE_GetInternalType("TYPE_DESC")
#define TYPE_DESK         TYPE_DESC
#define TYPE_PICTURE      CPAGE_GetInternalType("TYPE_PICTURE")

#endif
