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
#include <stdlib.h>

#include "resource.h"
#include "mpuma.h"

// Для использования без CFIO.DLL
#ifndef _DEBUG
	#define _DEBUG
#endif


//####################################
void *	myAlloc(size_t stAllocateBlock)
{
	char * mem = NULL;

//#ifdef _DEBUG

	mem = (char *)malloc(stAllocateBlock);
	if(!mem)
		SetReturnCode_puma(IDS_ERR_NO_MEMORY);
//#endif

	return mem;
}
//####################################
void	myFree(void * mem)
{
//#ifdef _DEBUG
	free(mem);
//#endif
}

Handle  myOpenSave(char * lpName)
{
	Handle rc = NULL;
//#ifdef _DEBUG
	rc = (Handle)fopen(lpName,"wb");
//#endif
	return rc;
}
Handle  myOpenRestore(char * lpName)
{
	Handle rc = NULL;
//#ifdef _DEBUG
	rc = (Handle)fopen(lpName,"rb");
//#endif
	return rc;
}
unsigned int  myWrite(Handle h,void * lpdata,unsigned int size)
{
	uint32_t rc = 0;
//#ifdef _DEBUG
	rc = fwrite(lpdata,1,size,(FILE*)h);
//#endif
	return rc;
}
unsigned int  myRead(Handle h,void * lpdata,unsigned int size)
{
	uint32_t rc = 0;
//#ifdef _DEBUG
	rc = fread(lpdata,1,size,(FILE *)h);
//#endif
	return rc;
}
void    myClose(Handle h)
{
//#ifdef _DEBUG
	fclose((FILE*)h);
//#endif
}


static uchar* Buffer;
static uchar* WorkMem;

void GiveMainBuff (void **vvBuff, int *Size)
{
	*vvBuff = Buffer;
	*Size = BufferSize;
}

void GiveWorkBuff (void **vvBuff, int *Size)
{
	*vvBuff = WorkMem;
	*Size = WorkMemSize;
}

Bool32 InitMem()
{
	Buffer=NULL;
	Buffer=(uchar*)myAlloc(BufferSize);
	if(!Buffer)
		return FALSE;
	WorkMem=NULL;
	WorkMem=(uchar*)myAlloc(WorkMemSize);
	if(!WorkMem)
		return FALSE;
	return TRUE;
}

void DoneMem()
{
	if(Buffer)
	  myFree(Buffer);
	Buffer=NULL;
	if(WorkMem)
	  myFree(WorkMem);
	WorkMem=NULL;
}
