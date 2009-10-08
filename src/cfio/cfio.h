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

#ifndef __CFIO_H__
#define __CFIO_H__

#include "globus.h"

#ifdef __CFIO__
#define CFIO_FUNC  FUN_EXPO__
#else
#define CFIO_FUNC  FUN_IMPO__
#endif

namespace CIF {
namespace CFIO {

enum MaxValues {
	CFIO_MAX_PATH = 256, CFIO_MAX_COMMENT = 48
};

CFIO_FUNC Bool32 CFIO_Init(uint16_t HeightCode, Handle hStorage);
CFIO_FUNC Bool32 CFIO_Done();
CFIO_FUNC uint32_t CFIO_GetReturnCode();
CFIO_FUNC char * CFIO_GetReturnString(uint32_t Error);
CFIO_FUNC Bool32 CFIO_GetExportData(uint32_t Type, void * pData);
CFIO_FUNC Bool32 CFIO_SetImportData(uint32_t Type, void * pData);

enum Parameters {
	CFIO_PCHAR_TEMPORARY_FOLDER = 1,
	CFIO_PCHAR_STORAGE_FOLDER,
	CFIO_PCHAR_FILE_FOLDER,
	CFIO_FNOpenStorage,
	CFIO_FNCloseStorage,
	CFIO_FNDeleteStorage,
	CFIO_FNWriteFileToStorage,
	CFIO_FNReadFileFromStorage,
	CFIO_FNOpenFreeFile,
	CFIO_FNCloseFreeFile,
	CFIO_FNWriteToFile,
	CFIO_FNReadFromFile,
	CFIO_FNSeekFilePointer,
	CFIO_FNTellFilePointer,
	CFIO_FNFlushFile,
	CFIO_FNAllocMemory,
	CFIO_FNDAllocMemory,
	CFIO_FNReAllocMemory,
	CFIO_FNFreeMemory,
	CFIO_FNLockMemory,
	CFIO_FNUnlockMemory,
	CFIO_FNWriteMemoryToFile,
	CFIO_FNReadMemoryFromFile,
	CFIO_FNWriteMemoryToStorage,
	CFIO_FNReadMemoryFromStorage
};

//Folders
enum CFIOFolders {
	CFIO_TEMP_FOLDER = 1, CFIO_FILE_FOLDER, CFIO_STORAGE_FOLDER
};

#define DEC_FUN(a,b,c) typedef a (*FNCFIO##b)c; CFIO_FUNC a CFIO_##b c;

//Open storage
enum OpenFlag {
	OS_CREATE = 0x01, OS_OPEN = 0x02
};

CFIO_FUNC Handle CFIO_OpenStorage(const char * Name, uint Flags);

//Close Storage
enum CloseFlag {
	CS_WITHOUT_SAVE = 0x01, // Only close
	CS_DELETE = 0x02, // Delete storage
	CS_FILE_DELETE = 0x04, // Delete all attached files from disk
	CS_SAVE = 0x08, // Save storage at current state
	CS_FILE_SAVE = 0x10, // Save all attached files
	CS_ALL = 0x20
// Close all open storages
};
CFIO_FUNC bool CFIO_CloseStorage(Handle, uint Flags);

//Delete storage from disk (don't need to be opened)
CFIO_FUNC bool CFIO_DeleteStorage(const char * Name);

//Files
//Write file to storage
CFIO_FUNC uint32_t CFIO_WriteFileToStorage(Handle, Handle, const char * Name);
//Read file from storage
CFIO_FUNC Handle CFIO_ReadFileFromStorage(Handle, char*);

//Open file
enum cfio_osf_t {
	OSF_CREATE = 0x01,
	OSF_OPEN = 0x02,
	OSF_READ = 0x04,
	OSF_WRITE = 0x08,
	OSF_BINARY = 0x10,
	OSF_IN_MEMORY = 0x20,
	OSF_TEMPORARY = 0x40
};

CFIO_FUNC Handle CFIO_OpenFreeFile(Handle, const char * Name, uint Flag);

//Close file
enum cfio_csf_t {
	CSF_SAVEDISK = 0x01,
	CSF_SAVESTORAGE = 0x02,
	CSF_DELETE = 0x04,
	CSF_WRITE = 0x08
};

CFIO_FUNC bool CFIO_CloseFreeFile(Handle, uint32_t);

//Write data to file
CFIO_FUNC uint32_t CFIO_WriteToFile(Handle, char *, uint32_t);
// Read data from file
DEC_FUN(uint32_t, ReadFromFile, (Handle, char *, uint32_t))

//Seek pointer
enum cfio_fs_t {
	FS_END = 0x01, FS_BEGIN = 0x02, FS_CUR = 0x04
};
DEC_FUN(uint32_t, SeekFilePointer, (Handle, uint32_t, uint32_t))
//Tell pointer
DEC_FUN(uint32_t, TellFilePointer, (Handle))
//Flash data from buffer
DEC_FUN(Bool32, FlushFile, (Handle))
//Memory

// Alloc memory
enum cfio_maf_t {
	MAF_GPTR = 0x0001,
	MAF_GNHD = 0x0002,
	MAF_GALL_GMEM_FIXED = 0x0004,
	MAF_GALL_GMEM_MOVEABLE = 0x0008,
	MAF_GALL_GPTR = 0x0010,
	MAF_GALL_GHND = 0x0020,
	MAF_GALL_GMEM_DDESHARE = 0x0040,
	MAF_GALL_GMEM_DISCARDABLE = 0x0080,
	MAF_GALL_GMEM_LOWER = 0x0100,
	MAF_GALL_GMEM_NOCOMPACT = 0x0200,
	MAF_GALL_GMEM_NODISCARD = 0x0400,
	MAF_GALL_GMEM_NOT_BANKED = 0x0800,
	MAF_GALL_GMEM_NOTIFY = 0x1000,
	MAF_GALL_GMEM_SHARE = 0x2000,
	MAF_GALL_GMEM_ZEROINIT = 0x4000,
	MAF_GALL_GMEM_RESERVED = 0x8000
};

// ReAlloc memory
enum cfio_mrc_t {
	MRF_NEW_MEMORY = 0x0000,
	MRF_GALL_GMEM_DISCARDABLEGPTR = 0x0001,
	MRF_GALL_GMEM_MOVEABLE = 0x0002,
	MRF_GALL_GMEM_NOCOMPACT = 0x0004,
	MRF_GALL_GMEM_ZEROINIT = 0x0008
};

DEC_FUN(Handle, AllocMemory, (uint32_t, uint32_t))
DEC_FUN(Handle, DAllocMemory, (uint32_t, uint32_t, const char*, const char*))
DEC_FUN(Handle, ReAllocMemory, (Handle, uint32_t, uint32_t))
//Free memory
DEC_FUN(Bool32, FreeMemory, (Handle))
//Lock memory
DEC_FUN(Handle, LockMemory, (Handle))
//Unlock memory
DEC_FUN(Bool32, UnlockMemory, (Handle))
// Write from memory to disk
DEC_FUN(uint32_t, WriteMemoryToFile, (Handle, const char *))
//Read data from disk to memory
DEC_FUN(uint32_t, ReadMemoryFromFile, (const char *, Handle *))
//Write data from memory to storage
DEC_FUN(uint32_t, WriteMemoryToStorage, (Handle, Handle, const char *))
//Read data from storage to memory
DEC_FUN(uint32_t, ReadMemoryFromStorage, (Handle, pchar, Handle *))

#undef DEC_FUN

}
}

#endif  //__CFIO_H__
