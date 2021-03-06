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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cttypes.h"
#include "func.h"
#include "ccom/ccom.h"
#include "exc/exc.h"
#include "loc/loc.h"

#define CREATE_STATUS
#include "status.h"

#include "dpuma.h"
#include "newfunc.h"

#include "layout.h"

#include "new_c.h" /* FIXME: to compile in MS VC++, Handle exthCCOM */
#include "rootlist.h"

extern int nIncline;

uint16_t run_options = 0;

uint32_t progress_set_percent(uint32_t volume) {
	uint32_t rc = 0;
	if (fnProgressStep_rsel)
		rc = !fnProgressStep_rsel(volume);
	return rc;
}

void progress_finish(void) {
	if (fnProgressFinish_rsel)
		fnProgressFinish_rsel();
}

#undef malloc
#undef realloc
#undef free

void * DebugMalloc(size_t size) {
	char * buf = (char*) malloc(size);
	return buf;
}

void * DebugRealloc(void * old_blk, size_t size) {
	char *buf = (char*) realloc(old_blk, size);
	return buf;
}

void DebugFree(void * p) {
	free(p);
}

extern uchar work_raster[], work_raster_1[];

MN * c_locomp(puchar raster, int16_t bw, int16_t h, int16_t upper, int16_t left)
{
	return LOC_CLocomp(raster, bw, h, upper, left);
}

puchar make_raster_CCOM(CCOM_comp *cmp) {
	int16_t h, d, dd, k, i, ii;
	RecRaster rs;

	memset(work_raster, 0, cmp->rw * cmp->h);
	CCOM_GetRaster(cmp, &rs);
	h = rs.lnPixHeight;
	d = REC_GW_WORD8(rs.lnPixWidth); // align to 8 bytes in RecRaster
	dd = (rs.lnPixWidth + 7) / 8; // aling to 1 byte  in standart
	for (k = ii = i = 0; k < h; k++, i += d, ii += dd) {
		memcpy(&work_raster[ii], &rs.Raster[i], dd);
	}

	return work_raster;
}

puchar make_extended_raster_CCOM(CCOM_comp *cmp) {
	int16_t h, d, dd, k, i, ii;
	RecRaster rs;

	memset(work_raster, 0, cmp->rw * cmp->h);
	CCOM_GetExtRaster(cmp, &rs);
	h = rs.lnPixHeight;
	d = REC_GW_WORD8(rs.lnPixWidth); // align to 8 bytes in RecRaster
	dd = (rs.lnPixWidth + 7) / 8; // aling to 1 byte  in standart
	for (k = ii = i = 0; k < h; k++, i += d, ii += dd) {
		memcpy(&work_raster[ii], &rs.Raster[i], dd);
	}

	return work_raster;
}

CCOM_comp * get_CCOM_comp(ROOT * r) {
    return r->pComp;
}

Bool save_MN(MN *mn) {
	CCOM_comp * p = REXC_MN2CCOM((Handle) exthCCOM, (Handle) mn);
	if (!p)
		return FALSE;

	if (!AddRoot(p, FALSE))
		return FALSE;

    if(cf::Roots::count() < 1)
        return FALSE;

    BlockAccountRoot(pCurrentBlock, cf::Roots::last());
	return TRUE;
}
