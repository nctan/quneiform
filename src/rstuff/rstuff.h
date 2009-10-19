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
/*  Автор      :  Александр Михайлов                                          */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :  Алексей Коноплев                                            */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'RSTUFF.H'                                                */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
//Almi 16.06.00 //Last edit ........
#ifndef __RSTUFF_H
#define __RSTUFF_H

#include "globus.h"
#include "rect.h"
#include "memorybuffer.h"
#include "puma/layoutoptions.h"

#ifdef __RSTUFF__
#define RSTUFF_FUNC  FUN_EXPO__
#else
#define RSTUFF_FUNC  FUN_IMPO__
#endif

class RSPreProcessImage
{
public:
    puchar *pgpRecogDIB;
    Bool32 gbAutoRotate;
    Bool32 gbDotMatrix;
    Bool32 gbFax100;
    uint32_t gnLanguage;
    uint32_t gnTables;
    Handle hCPAGE;
    Handle hDebugCancelSearchPictures;
    Handle hDebugCancelComponent;
    Handle hDebugCancelTurn;
    Handle hDebugCancelSearchLines;
    Handle hDebugCancelVerifyLines;
    Handle hDebugCancelSearchDotLines;
    Handle hDebugCancelRemoveLines;
    Handle hDebugCancelSearchTables;
    Handle hDebugCancelAutoTemplate;
    Handle hDebugEnableSearchSegment;
    const char * pglpRecogName;
    Handle* phCCOM;
    void * pinfo;
    Handle* phLinesCCOM;
    void * phCLINE;
    PBool32 pgneed_clean_line;
    int32_t * pgnNumberTables;
    uint32_t gnPictures;
    Bool32* pgrc_line;
    CIF::Rect gRectTemplate;
    const char *szLayoutFileName;
};

typedef RSPreProcessImage * PRSPreProcessImage;

namespace CIF {
class RStuff
{
public:
    RStuff();
    ~RStuff();
    void binarize();
    void layout();
    void normalize();
    void removeLines();
    void setImageData(RSPreProcessImage& data);
    void setSpecialProject(uchar NoSpecPrj);
    void verifyNormalization();
private:
    RSPreProcessImage * image_;
    LayoutOptions layout_opts_;
private:
    static const size_t MainBufferSize = 500000;
    static const size_t WorkBufferSize = 180000;
    static FixedBuffer<unsigned char, MainBufferSize> main_buffer_;
    static FixedBuffer<unsigned char, WorkBufferSize> work_buffer_;
};
}

typedef struct tagRSCBProgressPoints
{
    void * pGetModulePath;
    void * pSetUpdate;
} RSCBProgressPoints, *PRSCBProgressPoints;
RSTUFF_FUNC void RSTUFF_Init();
RSTUFF_FUNC void RSTUFF_Done();
RSTUFF_FUNC uint32_t RSTUFF_GetReturnCode();
RSTUFF_FUNC char * RSTUFF_GetReturnString(uint32_t dwError);
RSTUFF_FUNC Bool32 RSTUFF_GetExportData(uint32_t dwType, void * pData);
RSTUFF_FUNC Bool32 RSTUFF_SetImportData(uint32_t dwType, void * pData);

enum RSTUFF_EXPORT_ENTRIES
{
    RSTUFF_FN_RSBinarise = 1,
    RSTUFF_FN_RSNormalise,
    RSTUFF_FN_RSLayout,
    RSTUFF_FN_RSSetSpecPrj
};

enum RSTUFF_IMPORT_ENTRIES
{
    RSTUFF_FN_SetProgresspoints = 128,
    RSTUFF_FN_SetProgressStart,
    RSTUFF_FN_SetProgressStep,
    RSTUFF_FN_SetProgressFinish,
    RSTUFF_FN_SetInitPRGTIME,
    RSTUFF_FN_SetDPumaSkipComponent
};
/*  Описание функций  */
Bool32 RSTUFF_RSBinarise();
Bool32 RSTUFF_RSNormalise(PRSPreProcessImage, void* vBuff, int Size, void* vWork, int SizeWork);
Bool32 RSTUFF_RSNormVerify(PRSPreProcessImage);
Bool32 RSTUFF_RSNormRemoveLines(PRSPreProcessImage);
Bool32 RSTUFF_RSLayout(PRSPreProcessImage);
Bool32 RSTUFF_RSSetSpecPrj(uchar NoSpecPrj);
#undef DEC_FUN

#endif