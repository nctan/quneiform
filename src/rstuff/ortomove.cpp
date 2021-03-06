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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "cimage/ctiimage.h"
#include "cpage/cpage.h"
#include "lns/lns.h"
#include "rline/rline.h"
#include "dpuma.h"
#include "common/ctdib.h"
#include "rsfunc.h"
#include "puma/pumadef.h"
#include "cline/cline.h"

#include "cfcompat.h"

static int move;
extern Handle OrtMove;
void CopyMove(uchar* newpmasp, uchar* oldpmasp, int newbytewide,
		int oldbytewide, int num_str, int move);
void MasCopy(uchar* oldpmasp, uchar* pmasp, int oldbytewide, int num_str);
void CleanImage(uchar* pmasp, int bytewide, int num_str, int wide);

Bool32 OrtoMove(PRSPreProcessImage Image) {
	if (!LDPUMA_Skip(OrtMove))
		return TRUE;

	Handle hCPage = Image->hCPAGE;
	PAGEINFO info;
	char OldImage[CPAGE_MAXNAME];
	GetPageInfo(hCPage, &info);

	move = info.SkewLocVerLin2048;
	if (!move)
		return TRUE;

	for (int i = 0; i < CPAGE_MAXNAME; i++)
		OldImage[i] = info.szImageName[i];

	const char* ImageName = PUMA_IMAGE_ORTOMOVE;

    cf::BitmapPtr lpDIB;
    if (!CIMAGE_ReadDIB(OldImage, &lpDIB))
		return FALSE;

    cf::CTDIB* olddib = new cf::CTDIB;
	if (!(olddib->setBitmap(lpDIB))) {
		delete olddib;
		return FALSE;
	}
	int oldbytewide = olddib->lineWidthInBytes();
	int num_str = olddib->linesNumber();
    uchar* pmasp = (uchar*) (olddib->imageData());

	int oldwide = (int) (olddib->lineWidth());

	int max_move = abs((num_str * move) / 2048);
	if (!max_move) {
		olddib->reset();
		delete olddib;
		return TRUE;
	}
    cf::CTDIB * newdib = new cf::CTDIB;
	int newwide = oldwide + max_move;

    lpDIB = newdib->createBegin(newwide, num_str, olddib->bpp());

    if (!lpDIB) {
		olddib->reset();
		delete olddib;
		delete newdib;
		return TRUE;
	}
    uint X_DPM = 0;
    uint Y_DPM = 0;
	olddib->resolutionDotsPerMeter(&X_DPM, &Y_DPM);
	newdib->setResolutionDotsPerInch(info.DPIX, info.DPIY);
	newdib->setResolutionDotsPerMeter(X_DPM, Y_DPM);
	if (!(newdib->createEnd())) {
		olddib->reset();
		delete olddib;
		delete newdib;
		return TRUE;
	}
	int newbytewide = newdib->lineWidthInBytes();
    uchar* newpmasp = (uchar*) (newdib->imageData());
	if (newwide > (int) (newdib->lineWidth())) {
		olddib->reset();
		delete olddib;
		delete newdib;
		return TRUE;
	}
	CleanImage(pmasp, oldbytewide, num_str, (int) (olddib->lineWidth()));
	CopyMove(newpmasp, pmasp, newbytewide, oldbytewide, num_str, move);

    if (CIMAGE_AddImageCopy(ImageName, (cf::BitmapPtr) lpDIB)) {
        cf::BitmapPtr lp = NULL;
        if (CIMAGE_ReadDIB(ImageName, &lp)) {
			info.Images |= IMAGE_ORTOMOVE;
			strcpy((char*) info.szImageName, PUMA_IMAGE_ORTOMOVE);
			SetPageInfo(hCPage, info);
		} else {
			olddib->reset();
			delete olddib;
			delete newdib;
			return FALSE;
		}

		olddib->reset();
		delete olddib;
		delete newdib;

		//снова выделим компоненты
		if (!ExtractComponents(FALSE, NULL, PUMA_IMAGE_ORTOMOVE, Image))
			return FALSE;
		//выделим линии
		CLINE_Reset();
		if (!SearchLines(Image))
			return FALSE;
		//найдём угол наклона
		if (!CalcIncline(Image))
			return FALSE;
	} else {
		olddib->reset();
		delete olddib;
		delete newdib;
		return FALSE;
	}

	return TRUE;
}

void CopyMove(uchar* newpmasp, uchar* oldpmasp, int newbytewide,
		int oldbytewide, int num_str, int move) {
	int max_move = abs((num_str * move) / 2048);
	int spusk = (num_str + max_move - 1) / max_move;
	int nowmove = 0;
	int realspusk = spusk >> 1;
	int bytemove = 0;
	int newnowbyte;
	int oldnowbyte;
	int stopbyte;
	int newstopbyte;

	if (move > 0) {
		stopbyte = num_str * oldbytewide - 1;
		newstopbyte = num_str * newbytewide - 1;
		oldnowbyte = stopbyte - oldbytewide + 1;
		newnowbyte = newstopbyte - newbytewide + 1;
		for (int i = num_str - 1; i >= 0; i--) {
			for (int k = 0; k < bytemove; k++) {
				newpmasp[newnowbyte] = 255;
				newnowbyte++;
			}

			newpmasp[newnowbyte] = ~(255 >> nowmove);

			while ((oldnowbyte < stopbyte) && (newnowbyte < newstopbyte - 1)) {
				newpmasp[newnowbyte] |= (oldpmasp[oldnowbyte] >> nowmove);
				newnowbyte++;
				newpmasp[newnowbyte] = (oldpmasp[oldnowbyte] << (7 - nowmove));
				oldnowbyte++;
			}
			newpmasp[newnowbyte] |= (255 >> nowmove);
			newnowbyte++;
			while (newnowbyte < newstopbyte) {
				newpmasp[newnowbyte] = 255;
				newnowbyte++;
			}

			stopbyte -= oldbytewide;
			newstopbyte -= newbytewide;
			oldnowbyte = stopbyte - oldbytewide + 1;
			newnowbyte = newstopbyte - newbytewide + 1;
			realspusk--;
			if (!realspusk) {
				realspusk = spusk;
				nowmove++;
				if (nowmove == 8) {
					nowmove = 0;
					bytemove++;
				}
			}
		}
	} else {
		stopbyte = oldbytewide - 1;
		newstopbyte = newbytewide - 1;
		oldnowbyte = 0;
		newnowbyte = 0;
		for (int i = 0; i < num_str; i++) {
			for (int k = 0; k < bytemove; k++) {
				newpmasp[newnowbyte] = 255;
				newnowbyte++;
			}

			newpmasp[newnowbyte] = ~(255 >> nowmove);

			while ((oldnowbyte < stopbyte) && (newnowbyte < newstopbyte - 1)) {
				newpmasp[newnowbyte] |= (oldpmasp[oldnowbyte] >> nowmove);
				newnowbyte++;
				newpmasp[newnowbyte] = (oldpmasp[oldnowbyte] << (7 - nowmove));
				oldnowbyte++;
			}
			newpmasp[newnowbyte] |= (255 >> nowmove);
			newnowbyte++;
			while (newnowbyte < newstopbyte) {
				newpmasp[newnowbyte] = 255;
				newnowbyte++;
			}

			stopbyte += oldbytewide;
			newstopbyte += newbytewide;
			oldnowbyte = stopbyte - oldbytewide + 1;
			newnowbyte = newstopbyte - newbytewide + 1;
			realspusk--;
			if (!realspusk) {
				realspusk = spusk;
				nowmove++;
				if (nowmove == 8) {
					nowmove = 0;
					bytemove++;
				}
			}
		}
	}
}

void CleanImage(uchar* pmasp, int bytewide, int num_str, int wide) {
	int realbytewide = ((wide + 7) >> 3);
	int stop = num_str * bytewide;
	int nowbyte = bytewide - 1;
	int nowbyte1 = bytewide - 2;
	int nowbyte2 = bytewide - 3;
	switch (bytewide - realbytewide) {
	case 0:
		break;
	case 1:
		while (nowbyte < stop) {
			pmasp[nowbyte] = 255;
			nowbyte += bytewide;
		}
		break;
	case 2:
		while (nowbyte1 < stop) {
			pmasp[nowbyte] = 255;
			pmasp[nowbyte1] = 255;
			nowbyte += bytewide;
			nowbyte1 += bytewide;
		}
		break;
	case 3:
		while (nowbyte2 < stop) {
			pmasp[nowbyte] = 255;
			nowbyte += bytewide;
			pmasp[nowbyte1] = 255;
			nowbyte1 += bytewide;
			pmasp[nowbyte2] = 255;
			nowbyte2 += bytewide;
		}
	}
	int move = (realbytewide << 3) - wide;
	if (!move)
		return;
	move = 7 - move;
	nowbyte = realbytewide - 1;
	while (nowbyte < stop) {
		pmasp[nowbyte] |= 255 >> move;
		nowbyte += bytewide;
	}
}

void MasCopy(uchar* oldpmasp, uchar* pmasp, int oldbytewide, int num_str) {
	int j = oldbytewide * num_str;
	for (int i = 0; i < j; i++)
		oldpmasp[i] = pmasp[i];
}
