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

//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//                                                                              //
//                    Cognitive Technologies Ltd.                               //
//                                                                              //
//                    Data Storage Container                                    //
//                                                                              //
//                                                                              //
//                                                                              //
//                                                                              //
//                    started at 25 may 1998                                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
#define __CFIO__
#include "control.h"
#include "compat_defs.h"
#include "resource.h"

namespace CIF {
namespace CFIO {

//GLOBAL
void SetReturnCode_cfio(uint16_t rc);
//GLOBAL VARIABLES
static uint16_t wHeightRC = 0;
static uint16_t wLowRC = 0;
static Handle hInst = NULL;

Control * Control_ctc = NULL;

static int32_t InitCount = 0;
static Bool32 InitDone = FALSE;
//////////////////////////////////////////////////////////////////////////////////
//
Bool APIENTRY DllMain(Handle hModule, uint32_t ul_reason_for_call, pvoid /*lpReserved*/) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		hInst = hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

Bool32 CFIO_Init(uint16_t wHeightCode, Handle /*hStorage*/) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (!Control_ctc) {
		Control_ctc = new Control;
		wHeightRC = wHeightCode;
	}

	if (Control_ctc) {
		InitCount++;
		return TRUE;
	}

	SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
	return FALSE;
}

Bool32 CFIO_Done() {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc) {
		if (--InitCount == 0) {
			delete Control_ctc;
			Control_ctc = NULL;
		}
		return InitDone = TRUE;
	} else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
	}

	return FALSE;
}

uint32_t CFIO_GetReturnCode() {
	return (wHeightRC << 16) | (wLowRC - IDS_ERR_MIN);
}

char * CFIO_GetReturnString(uint32_t dwError) {
	if (dwError >> 16 != wHeightRC)
		wLowRC = IDS_ERR_NOTIMPLEMENT;

	return NULL;
}

void SetReturnCode_cfio(uint16_t rc) {
	if (rc == IDS_CFIO_ERR_NO || wLowRC == IDS_CFIO_ERR_NO)
		wLowRC = rc;
}

uint16_t GetReturnCode_cfio() {
	return wLowRC;
}

#define CASE_FUNCTION(a)	case CFIO_FN##a:	*(FNCFIO##a *)pData = CFIO_##a; break;
//
//Bool32 CFIO_GetExportData(uint32_t dwType, void * pData) {
//	Bool32 rc = TRUE;
//
//	SetReturnCode_cfio(IDS_CFIO_ERR_NO);
//
//	wLowRC = 0;
//	switch (dwType) {
//	case CFIO_PCHAR_TEMPORARY_FOLDER:
//		rc = Control_ctc->GetFolder(CFIO_TEMP_FOLDER, (char *) pData);
//		break;
//
//	case CFIO_PCHAR_STORAGE_FOLDER:
//		rc = Control_ctc->GetFolder(CFIO_FILE_FOLDER, (char *) pData);
//		break;
//
//	case CFIO_PCHAR_FILE_FOLDER:
//		rc = Control_ctc->GetFolder(CFIO_STORAGE_FOLDER, (char *) pData);
//		break;
//
//	CASE_FUNCTION(OpenStorage)
//	CASE_FUNCTION(CloseStorage)
//	CASE_FUNCTION(DeleteStorage)
//	CASE_FUNCTION(WriteFileToStorage)
//	CASE_FUNCTION(ReadFileFromStorage)
//	CASE_FUNCTION(OpenFreeFile)
//	CASE_FUNCTION(CloseFreeFile)
//	CASE_FUNCTION(WriteToFile)
//	CASE_FUNCTION(ReadFromFile)
//	CASE_FUNCTION(SeekFilePointer)
//	CASE_FUNCTION(TellFilePointer)
//	CASE_FUNCTION(FlushFile)
//	CASE_FUNCTION(AllocMemory)
//	CASE_FUNCTION(DAllocMemory)
//	CASE_FUNCTION(ReAllocMemory)
//	CASE_FUNCTION(FreeMemory)
//	CASE_FUNCTION(LockMemory)
//	CASE_FUNCTION(UnlockMemory)
//	CASE_FUNCTION(WriteMemoryToFile)
//	CASE_FUNCTION(ReadMemoryFromFile)
//	CASE_FUNCTION(WriteMemoryToStorage)
//	CASE_FUNCTION(ReadMemoryFromStorage)
//	default:
//		*(char **) pData = NULL;
//		wLowRC = IDS_ERR_NOTIMPLEMENT;
//		rc = FALSE;
//	}
//	return rc;
//}

Bool32 CFIO_SetImportData(uint32_t dwType, void * pData) {
	uint32_t Folder;

	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	wLowRC = 0;
	switch (dwType) {
	case CFIO_PCHAR_TEMPORARY_FOLDER:
		Folder = CFIO_TEMP_FOLDER;
		break;

	case CFIO_PCHAR_STORAGE_FOLDER:
		Folder = CFIO_FILE_FOLDER;
		break;

	case CFIO_PCHAR_FILE_FOLDER:
		Folder = CFIO_STORAGE_FOLDER;
		break;

	default:
		wLowRC = IDS_ERR_NOTIMPLEMENT;
		return FALSE;
	}
	return Control_ctc->SetFolder(Folder, (char *) pData);
}

Handle CFIO_OpenStorage(const char * Name, uint32_t Types) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->OpenStorage(Name, Types);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

bool CFIO_CloseStorage(Handle hStorage, uint Flag) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->CloseStorage(hStorage, Flag);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return false;
	}
}

bool CFIO_DeleteStorage(const char *Name) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->DeleteStorage(Name);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return false;
	}
}

uint32_t CFIO_WriteFileToStorage(Handle hStorage, Handle hFile,
		const char * NameInStorage) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->WriteFileToStorage(hStorage, hFile, NameInStorage);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Handle CFIO_ReadFileFromStorage(Handle hStorage, char* lpName) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->ReadFileFromStorage(hStorage, lpName);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Handle CFIO_OpenFreeFile(Handle hStorage, const char * Name, uint Flag) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->OpenFile(hStorage, Name, Flag);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

bool CFIO_CloseFreeFile(Handle hFile, uint32_t dwFlag) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->CloseFile(hFile, dwFlag);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return false;
	}
}

uint32_t CFIO_WriteToFile(Handle hFile, char * Data, uint32_t Size) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->WriteFile(hFile, Data, Size);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_ReadFromFile(Handle hFile, char * Data, uint32_t Size) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->ReadFromFile(hFile, Data, Size);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_SeekFilePointer(Handle hFile, uint32_t dwBytes, uint32_t dwFrom) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Seek(hFile, dwBytes, dwFrom);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_TellFilePointer(Handle hFile) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Tell(hFile);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Bool32 CFIO_FlushFile(Handle hFile) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Flush(hFile);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Handle CFIO_AllocMemory(uint32_t dwSize, uint32_t dwFlag) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Alloc(dwSize, dwFlag, "CFIO general memory block",
				"no comment");
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Handle CFIO_DAllocMemory(uint32_t dwSize, uint32_t dwFlag, const char* Owner,
		const char* Comment) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Alloc(dwSize, dwFlag, Owner, Comment);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Handle CFIO_ReAllocMemory(Handle hMemory, uint32_t dwSize, uint32_t dwFlag) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->ReAlloc(hMemory, dwSize, dwFlag);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Bool32 CFIO_FreeMemory(Handle hMem) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Free(hMem);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Handle CFIO_LockMemory(Handle hMem) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Lock(hMem);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

Bool32 CFIO_UnlockMemory(Handle hMem) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->Unlock(hMem);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_WriteMemoryToFile(Handle hMem, const char * lpName) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->WriteMemToFile(hMem, lpName);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_ReadMemoryFromFile(const char * lpName, Handle * phMem) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->ReadMemFromFile(lpName, phMem);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_WriteMemoryToStorage(Handle hMem, Handle hStorage,
		const char * lpName) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->WriteMemToStorage(hMem, hStorage, lpName);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}

uint32_t CFIO_ReadMemoryFromStorage(Handle hStorage, char * lpName,
		Handle * phMem) {
	SetReturnCode_cfio(IDS_CFIO_ERR_NO);

	if (Control_ctc)
		return Control_ctc->ReadMemFromStorage(hStorage, lpName, phMem);
	else {
		if (InitDone)
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_DONE);
		else
			SetReturnCode_cfio(IDS_CFIO_ERR_CONTAINER_NOT_INITIALIZED);
		return FALSE;
	}
}
}
}