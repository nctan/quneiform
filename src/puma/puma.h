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

#ifndef __PUMA_H
#define __PUMA_H

#include <string>

#include "globus.h"
#include "cttypes.h"
#include "singleton.h"

#ifdef __PUMA__
#define PUMA_FUNC  FUN_EXPO__
#else
#define PUMA_FUNC  FUN_IMPO__
#endif

namespace CIF {
class PumaImpl;
typedef Singleton<PumaImpl, CreateUsingNewSmartPtr> Puma;
}

typedef void (*FNPUMA_ProgressStart)(void);
typedef void (*FNPUMA_ProgressFinish)(void);
typedef bool (*FNPUMA_ProgressStep)(uint32_t step, const char* name,
		uint32_t percent);
typedef struct {
	uint16_t wImageHeight;
	uint16_t wImageWidth;
	uint16_t wImageByteWidth;
	uint16_t wImageDisplacement;
	uint16_t wResolutionX;
	uint16_t wResolutionY;
	uchar bFotoMetrics;
	uchar bUnused;
	uint16_t wAddX;
	uint16_t wAddY;
} PUMA_ImageInfo;

typedef Bool16 (*PUMA_Callback_ImageOpen)(PUMA_ImageInfo *);
typedef uint16_t (*PUMA_Callback_ImageRead)(pchar, uint16_t);
typedef Bool16 (*PUMA_Callback_ImageClose)(void);

typedef struct {
	PUMA_Callback_ImageOpen CIMAGE_ImageOpen;
	PUMA_Callback_ImageRead CIMAGE_ImageRead;
	PUMA_Callback_ImageClose CIMAGE_ImageClose;
} PUMAIMAGECALLBACK;

enum PUMA_EXPORT_ENTRIES {
	PUMA_FNPUMA_EnumLanguages = 1,
	PUMA_FNPUMA_EnumFormats,
	PUMA_FNPUMA_EnumCodes,
	PUMA_pchar_Version,
	PUMA_FNPUMA_EnumFormatMode,
	PUMA_FNPUMA_EnumTable,
	PUMA_FNPUMA_EnumPicture,
	PUMA_Word8_Format,
	PUMA_Point32_PageSize,
	PUMA_FNPUMA_XSetTemplate,
	PUMA_Bool32_PreserveLineBreaks,
	PUMA_LPPUMAENTRY_CED,
	PUMA_LPPUMAENTRY_ROUT,
	PUMA_FNPUMA_GetSpecialBuffer,
	PUMA_FNPUMA_SetSpecialProject,
	PUMA_FNPUMA_XGetTemplate
};

#define DEC_FUN(a,b,c) typedef a (*FNPUMA_##b)c;

// Module functions
DEC_FUN(int, GetReturnCode,())
PUMA_FUNC int PUMA_GetReturnCode();

PUMA_FUNC void PUMA_GetSpecialBuffer(char * szResult, int32_t *nResultLength);
PUMA_FUNC bool PUMA_SetSpecialProject(uchar nSpecPrj);

#undef DEC_FUN

#endif
