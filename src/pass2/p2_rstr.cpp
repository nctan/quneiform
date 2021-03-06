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

#include "cstr/cstr.h"
#include "p2.h"

// распознать с разрезанием/склейкой кусок - от first до last,
// результат поместить в lineFon
int my_p2_RecogCutGlu(CSTR_rast first, CSTR_rast last, CSTR_line lineFon,
                      P2GLOBALS *p2glob)
{
    return 0;
}

// допустимые символы, перекодировка
void my_p2_SetP2Alphabet(int lang, char *alf)
{
    memset(alf, 0, 256);
}

uchar my_p2_GetCodePage(int lang)
{
    return 0;
}

void my_p2_DecodeCode(char *pCode, int let)
{
}

// снэр
Bool my_NoStopSnapLEO(void)
{
    return FALSE;
}

Bool my_snap_monitor_ori(CSTR_line *snap_line, int32_t num_lines)
{
    return FALSE;
}

Bool my_snap_activity(uchar a)
{
    return FALSE;
}

Bool my_snap_show_text(const char * /*txt*/)
{
    return FALSE;
}

Bool mySetupField(void *letInfo, int32_t nFont, void *info)
{
    return FALSE;
}

Bool mySetupPage(void *info)
{
    return FALSE;
}

Bool mySpecRecog(RecObject *ro)
{
    return FALSE;
}

// проверка по словарю
Bool my_p2_spell(pchar s, uchar lang)
{
    return FALSE;
}

// распознать с разрезанием/склейкой кусок - от first до last,
// результат поместить в lineFon
int (*RSTR_p2_RecogCutGlu)(CSTR_rast first, CSTR_rast last, CSTR_line lineFon,
                           P2GLOBALS *pglob) =
                               my_p2_RecogCutGlu;

// допустимые символы, перекодировка
void (*RSTR_p2_SetP2Alphabet)(int lang, char *alf) = my_p2_SetP2Alphabet;
uchar (*RSTR_p2_GetCodePage)(int lang) = my_p2_GetCodePage;
void (*RSTR_p2_DecodeCode)(char *pCode, int let) = my_p2_DecodeCode;

// снэр
Bool (*RSTR_p2_NoStopSnapLEO)(void) = my_NoStopSnapLEO;
Bool
(*RSTR_p2_snap_monitor_ori)(CSTR_line *snap_line, int32_t num_lines) = my_snap_monitor_ori;
Bool (*RSTR_p2_snap_activity)(uchar a) = my_snap_activity;
Bool (*RSTR_p2_snap_show_text)(const char *txt) = my_snap_show_text;

// проверка по словарю
Bool (*RSTR_p2_spell)(pchar s, uchar lang) = my_p2_spell;

// дополнительное распознавание (LEO)
Bool
(*ADDREC_SetupField)(void *letInfo, int32_t nFont, void* fontInfo) = mySetupField;
Bool (*ADDREC_SetupPage)(void *info) = mySetupPage;
Bool (*ADDREC_Recog)(RecObject* obj) = mySpecRecog;

void P2_SetRSTR(Handle RecogCutGlu, Handle setAlpha, Handle GetPage,
                Handle Decode, Handle NoStopSnapLEO, Handle monitor_ori,
                Handle activity, Handle show_text, Handle spell, Handle setupPage,
                Handle setupField, Handle specRecog)
{
    RSTR_p2_RecogCutGlu = (int(*)(strucCSTR_cell*, strucCSTR_cell*, void*,
                                  P2GLOBALS*)) RecogCutGlu;
    RSTR_p2_SetP2Alphabet = (void(*)(int, char*)) setAlpha;
    RSTR_p2_GetCodePage = (uchar(*)(int)) GetPage;
    RSTR_p2_DecodeCode = (void(*)(char*, int)) Decode;
    RSTR_p2_NoStopSnapLEO = (Bool(*)()) NoStopSnapLEO;
    RSTR_p2_snap_monitor_ori = (Bool(*)(void**, int32_t)) monitor_ori;
    RSTR_p2_snap_activity = (Bool(*)(uchar)) activity;
    RSTR_p2_snap_show_text = (Bool(*)(const char*)) show_text;
    RSTR_p2_spell = (Bool(*)(char*, uchar)) spell;

    if (setupPage)
        ADDREC_SetupPage = (Bool(*)(void*)) setupPage;

    if (setupField)
        ADDREC_SetupField = (Bool(*)(void*, int32_t, void*)) setupField;

    if (specRecog)
        ADDREC_Recog = (Bool(*)(RecObject*)) specRecog;
}
