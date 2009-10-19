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

/**********  Заголовок  *******************************************************/
/*  Автор,                                                                    */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :  Алексей Коноплев                                            */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'RSTGlobalData.h'                                           */
/*  Содержание :  Описание и объявление глобальных переменных                 */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
#ifndef _RSGLOBALDATA_H_
#define _RSGLOBALDATA_H_

////////////////////////////////////////////////////////////////////////////////
#include "globus.h"
#include "rsdefines.h"
#include "lns32/lnsdefs.h"

// predefined
#ifdef __RSTUFF_CPP__
	#define EXTERN
	#define VAL(a)		    = a
    #define VALM(a)		    = {a}
	#define VAL2(a,b)	    = { a,b }
	#define VAL4(a,b,c,d)	= { a,b,c,d }
#else
	#define EXTERN		extern
	#define VAL(a)
	#define VALM(a)
	#define VAL2(a,b)
	#define VAL4(a,b,c,d)
#endif

////////////////////////////////////////////////

EXTERN CIF::RSCBProgressPoints   ProgressPoints      VALM(NULL);
EXTERN LinesTotalInfo       *gLTInfo            VAL(NULL);

EXTERN RSLinesBuffer        gLinesData          VALM(NULL);

EXTERN uint32_t               gKillComponents     VAL(1);
EXTERN uint32_t               gKillZone           VAL(1);
EXTERN uint32_t               gKillRate           VAL(127);
EXTERN int32_t                gComponentRange     VAL(0);
EXTERN int32_t                bShowLineDebug      VAL(FALSE);
EXTERN int32_t                bShowStepLineDebug  VAL(FALSE);
EXTERN int32_t                bShowLineDebugData  VAL(FALSE);

EXTERN Handle               hLineKillerWindow                   VAL(NULL);
EXTERN Handle               hDebugRoot                          VAL(NULL);
EXTERN Handle               hDebugKillLines                     VAL(NULL);
EXTERN Handle               hDebugKillLinesStep                 VAL(NULL);
EXTERN Handle               hDebugKillLinesData                 VAL(NULL);
EXTERN Handle               hDebugKillLinesShowComponentsBefore VAL(NULL);
EXTERN Handle               hDebugKillLinesShowComponentsAfter  VAL(NULL);
EXTERN	uchar				db_spec_prj			VAL(0);
#endif //_RSGLOBALDATA_H_
////////////////////////////////////////////////////////////////////////////////////
//end of file
