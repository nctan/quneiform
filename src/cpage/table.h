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

#ifndef __TABLE_H__
#define __TABLE_H__

#include "cttypes.h"
#include "commondata.h"

enum {
    MaxTableUsers = 5,
    MaxHorLines = 99,
    MaxVerLines = 50,
    MaxCells = (MaxHorLines-1)*(MaxVerLines-1)
};

enum NAMES_TABLE_USER {
    TABLE_USER_COMMON = 0,
    TABLE_USER_RLTABLE,
    TABLE_USER_RBLOCK,
    TABLE_USER_CPAGE,
    TABLE_USER_RBTABLE
};

struct TABLE_LINE_DESC {
    enum {
        TLT_UnKnown = 0x00, //- ничего не означает
        TLT_Rough = 0x01,   //- правильная линия, не пытаться сократить
        TLT_UnSure = 0x02   //- ничего пока не означает
    };

    int32_t   Level;
    char    Type;              // need types will be described
    char    reserv[3];
};

struct TABLE_LEVEL_DESC {
    char    Type;              // need types will be described
    char    reserv[3];
};

enum SIDES_TABLE_CELL {
    TABLE_CELL_LEF = 0,  // Beg_X
    TABLE_CELL_RIG,      // End_X
    TABLE_CELL_TOP,      // Beg_Y
    TABLE_CELL_BOT       // End_Y
};

struct TABLE_CELL_DESC {
    char    TypeOwn;           // need types will be described
    char    TypeBound[4];      // это - избыточная информация
    char    reserv[3];
};

struct TABLE_SPECIAL_PROPERTIES {//сейчас использует только 'RLTABLE'
    char    nSpecVertex;   // кол-во особых узлов
    char    Hori[5];       // номер табличной гориз.линии
    char    Vert[5];       // номер табличной гориз.линии
    char    Type[5];       // характер особенности узла
    enum {
        NON_CELL_VERTEX = 0x01
    };
};

struct TABLE_DESC {
    /*  Как искали таблицу  */
    Rect32  RectFieldSearch;
    int32_t   SkewFieldSearch;   // at what units?
    char    TaskTableSearch;   // need types will be described
    /*  Пользователи таблицы  */
    /*  Key[USER] - ключ пользователя "USER" от хранилища его связанных  */
    /*  с этой таблицей данных. Если равно 0 - хранилище не заведено.    */
    uint32_t  Key[MaxTableUsers];
    /*  Общие свойства таблицы  */
    Rect32  RectTable;         // это - избыточная информация
    int32_t   SkewTable;         // at what units?
    char    LifeTable;         // need types will be described
    int     nHorLines;
    int     nVerLines;
    int     nCell;             // это - избыточная информация [(nH-1)*(nV-1)]
    /*  Свойства крупных структурных элементов таблицы  */
    TABLE_LINE_DESC    HorLine [MaxHorLines];
    TABLE_LINE_DESC    VerLine [MaxVerLines];
    TABLE_LEVEL_DESC   HorLevel[MaxHorLines-1];
    TABLE_LEVEL_DESC   VerLevel[MaxVerLines-1];
    /*  Свойства мелких структурных элементов таблицы  */
    enum {
        CELL_BOUND_PSEVDO = 0x01
    };
    char    TypeHorBound[MaxHorLines*MaxVerLines];
    char    TypeVerBound[MaxHorLines*MaxVerLines];
    TABLE_CELL_DESC       Cell [MaxCells];
    /*  Прочее  */
    TABLE_SPECIAL_PROPERTIES   SpecProp;
    char    iSet; //номер сета, которому
    enum {
        TTD_None         = 0x00, //- никакой, в сете она одна
        TTD_AsInImage    = 0x01, //- простая
        TTD_ByLogic      = 0x02, //- средняя
        TTD_Maximal      = 0x03 //- максимально подробная
    };
    char    TypeTablDetail; // тип детализации
    char    Active; // 0 - пассивная, 1 - активная в сете
    char    reserv[253];
};

namespace cf {
namespace cpage {

class TABLE : public CommonData
{
    int32_t num_colons;//число колонок
    int32_t num_rows;//число строк
    int32_t LineY[MaxHorLines/*MaxHorLines*/-1];//координаты линий (нулевая совпадает с первой линией, верхняя крышка таблицы не участвует)
    int32_t LineX[MaxVerLines-1];//координаты колонок (нулевая совпадает с первой левой колонкой)
    Bool16 Visible[MaxHorLines][MaxVerLines][2];//видима-невидима плюс флаги: сплошная, пунктирная, штрих
    Bool16 TypeCell[MaxHorLines][MaxVerLines];//тип ячейки
    int32_t Skew;
    Bool16 Negative[MaxHorLines][MaxVerLines];//Негатив = 1, Позитив = 0;//     01.01.01 Логинов
    int16_t Orient[MaxHorLines][MaxVerLines];//TYPE_UPDOWN- Сверху вниз и т.д. см константы выше

    char    iSet; //номер сета, которому
    char    TypeTablDetail;
    char    Active; // 0 - пассивная, 1 - активная в сете
    char    reserv[3];
    //int16_t SetNum;//Нумерация в сете
    //Bool16 bActive;//для маркировки активной таблицы в сете
    //int16_t GlobNum;//Внутренняя нумерация
};

}
}

struct CellInfo
{
    int32_t Number; // Номер ячейки физической таблицы ( начиная с 1 )
    cf::Point32 PhCoord;// Координаты привязки к физической ячейке
    int32_t Block; // Номер фрагмента
    int32_t GeCount;// число геометрических ячеек, входящих в физическую
    uint32_t wFlags[16];// флажки
    int32_t reserv[48];
};

struct CPAGE_TABLE
{
    TABLE_DESC prop; // основная информация о таблице
    CellInfo cell[MaxHorLines - 1][MaxVerLines - 1];// номер соответсвующего
    uint32_t PhNumber; // число физических ячеек
    uint32_t wFlags[16];// флажки
};

#endif
