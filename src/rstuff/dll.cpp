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
/*  Файл       :  'dll.cpp'                                                   */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/

#include "resource.h"
#include "rstuff_local.h"
#include "rsfunc.h"
#include "rsmemory.h"
#include "dpuma.h"
#include "dsnap.h"
#include "puma/pumadef.h"
#include "smetric/smetric.h"
#include "rline/rline.h"

#define __RSTUFF_CPP__
#include "rsglobaldata.h"
#undef __RSTUFF_CPP__

#include "compat_defs.h"
//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
struct tagRC16 {
	uint16_t gwLowRC;
	uint16_t gwHeightRC;
};
union RCode {
	tagRC16 RC16;
	uint32_t gwRC;
};

static RCode RC = { 0 };
static uint16_t wHighErrCode = 0;

Handle ObvKillLines;
Handle OKL;
Handle Zone;
Handle OrtMove;
Handle hNewLine;
Handle hDotLine;
Handle hAngles;
Handle hCalcMuchSkew;
Handle hTalkMuchSkew;
static Handle MainDebug;
Handle InsideKill;
Handle hNotTestAlik;
Handle NotKillPointed;

static Handle hMainTime;
static Handle hPrep;
Handle hContBigComp;
Handle hSearchLine;
Handle hCalcIncline;
Handle hOrto;
Handle hVerOrNewLine;
Handle hKillLine;
Handle hPrep2;
Handle hKillLineAfter;
static Handle hEnd;
Handle hDebugAutoTemplate;
Handle hAutoTemplate1;
Handle hAutoTemplateBC;
Handle hAutoTemplateBCShow;
Handle hAutoTemplateMar;
Handle hAutoTemplateWrite;
Handle hWndTurn = NULL;
Handle hDebugPreprocess;
Handle hDebugPrintResolution;

int KVO = 1;

Bool32 RSTUFF_RSNormVerify(PRSPreProcessImage Image)
{
    SetReturnCode_rstuff(0);
    return VerifyN(Image);
}

Bool32 RSTUFF_RSNormRemoveLines(PRSPreProcessImage Image)
{
    SetReturnCode_rstuff(0);
    return KillLinesN( Image );
}

Bool32 RSTUFF_RSLayout( PRSPreProcessImage Image )
{
    SetReturnCode_rstuff((uint16_t)0);
    return Layout(Image);
}

void SetReturnCode_rstuff(int rc) {
    RC.gwRC = rc;
}

void DebugInit(void) {
    hWndTurn = NULL;
    LDPUMA_Registry(&hMainTime, SNAP_ROOT_MAIN_TIME_CONTROL, NULL);

    LDPUMA_Registry(&hPrep, "Подготовка.", hMainTime);
    LDPUMA_RegistryHelp(hPrep,
			"Автоповорот.Выделение компонент.Формирование изображения.", FALSE);
    LDPUMA_Registry(&hSearchLine, "LNS.Поиск линий.", hMainTime);
    LDPUMA_Registry(&hCalcIncline, "Вычисление углов наклона..", hMainTime);
    LDPUMA_Registry(&hOrto, "Ортосдвиг страницы.", hMainTime);
    LDPUMA_Registry(&hContBigComp, "Заполнение контейнера BigComp", hMainTime);
    LDPUMA_Registry(&hVerOrNewLine, "Верификация или второй проход.", hMainTime);
    LDPUMA_Registry(&hKillLine, "Снятие линий.", hMainTime);
    LDPUMA_Registry(&hPrep2, "После снятия.", hMainTime);
    LDPUMA_RegistryHelp(hPrep2,
			"Выделение компонент.Формирование изображения.", FALSE);
    LDPUMA_Registry(&hKillLineAfter, "Доубитие линий.", hMainTime);
    LDPUMA_Registry(&hEnd, "От RSTUFF до RMARKER", hMainTime);

    LDPUMA_Registry(&MainDebug, SNAP_ROOT_MAIN_DEBUG, NULL);

    LDPUMA_Registry(&hDebugRoot, SNAP_ROOT_STUFF, NULL);

    LDPUMA_Registry(&hDebugPreprocess, SNAP_STUFF_BINARIZE, hDebugRoot);
    LDPUMA_Registry(&hDebugPrintResolution, "Печать нового разрешения",
                    hDebugPreprocess);
    LDPUMA_RegistryHelp(
                hDebugPrintResolution,
                "Вывести на консоль разрешение, установленноге по компонентам, в случае его отличия от исходного",
                FALSE);

    LDPUMA_Registry(&hDebugKillLines, SNAP_STUFF_KILL_LINES, hDebugRoot);
    LDPUMA_RegistryHelp(
                hDebugKillLines,
                "<Ответственный: A.Коноплев> Поиск и удаление линий и прилежащих к ним компонент.",
                FALSE);
    LDPUMA_RegVariable(hDebugKillLines, "Убить найденышей", &gKillComponents,
                       "unsigned");
    LDPUMA_RegVariable(hDebugKillLines, "Зона поражения (в пикселах)",
                       &gKillZone, "unsigned");
    LDPUMA_RegVariable(hDebugKillLines, "Покрытие (0 - 255)", &gKillRate,
                       "unsigned");

    LDPUMA_Registry(&hDebugKillLinesStep, SNAP_KILL_LINES_STEP, hDebugKillLines);
    LDPUMA_RegistryHelp(hDebugKillLinesStep, "Пошаговый проход.", FALSE);

    LDPUMA_Registry(&hDebugKillLinesData, SNAP_KILL_LINES_DATA, hDebugKillLines);
    LDPUMA_RegistryHelp(hDebugKillLinesData, "Сама по себе не функционирует",
			FALSE);

    LDPUMA_Registry(&hDebugKillLinesShowComponentsBefore,
                    SNAP_KILL_LINES_SHOW_BEFORE, hDebugKillLines);
    LDPUMA_RegistryHelp(hDebugKillLinesData,
			"Показывает компоненты до удаления всех линий", FALSE);
    LDPUMA_Registry(&hDebugKillLinesShowComponentsAfter,
                    SNAP_KILL_LINES_SHOW_AFTER, hDebugKillLines);
    LDPUMA_RegistryHelp(hDebugKillLinesData,
			"Показывает компоненты после удаления всех линий", FALSE);

    LDPUMA_Registry(&OKL, "Тривиальное удаление линий", hDebugRoot);
    LDPUMA_RegistryHelp(
                OKL,
                "<Ответственный тов. Степаненков> \
                \n Корневая вершина отладки тривиального удаления линий\
                                                                          Активизация вершины приведёт именно к этому способу их удаления.",
                                                                                                                                         FALSE);

    LDPUMA_Registry(&Zone, "Расширить КВО", OKL);
    LDPUMA_RegistryHelp(Zone, "Увеличить ширину линии на 2*...", FALSE);
    LDPUMA_RegVariable(Zone, "Height+", &KVO, "int");
    LDPUMA_Registry(&hNotTestAlik, "Не проверять Алика", OKL);
    LDPUMA_RegistryHelp(hNotTestAlik, "Не проверять Алика", FALSE);

    //	LDPUMA_Registry(&InsideKill,"Удаление линий Алика" ,MainDebug);
    //	LDPUMA_RegistryHelp(InsideKill,"Разрешить удаление линий Алика",FALSE);
    LDPUMA_Registry(&NotKillPointed, "Не удалять точечные линии", MainDebug);
    LDPUMA_RegistryHelp(NotKillPointed, "Не удалять точечные линии", FALSE);
    LDPUMA_Registry(&OrtMove, "Этап ортогонализующего сдвига.", NULL);
    LDPUMA_Registry(&ObvKillLines, "Тривиальное удаление линий.", MainDebug);
    LDPUMA_RegistryHelp(ObvKillLines, "Разрешить тривиальное удаление линий.",
			FALSE);

    LDPUMA_Registry(&hDotLine, "Поиск точечных линий.", MainDebug);
    LDPUMA_RegistryHelp(hDotLine,
			"Включить поиск точечных линий библиотекой RShellLines", FALSE);

    //    LDPUMA_RegVariable(OrtMove,"Сдвиг",&move,"int");

    LDPUMA_Registry(&hNewLine, "Верификация линий.", MainDebug);
    LDPUMA_RegistryHelp(hNewLine,
			"Веривикация линий вместо второго прохода Алика", FALSE);
    //	LDPUMA_Registry (&hUseCLine,"Работа с контейнером линий",NULL);

    LDPUMA_Registry(&hAngles, "Вычисление углов.", hDebugRoot);
    LDPUMA_RegistryHelp(hAngles, "Вычисление углов.", FALSE);
    LDPUMA_Registry(&hCalcMuchSkew, "Вычислить разные углы.", hAngles);
    LDPUMA_RegistryHelp(hCalcMuchSkew, "Вычислить разные углы.", FALSE);
    LDPUMA_Registry(&hTalkMuchSkew, "Рассказать о разных углах.", hAngles);
    LDPUMA_RegistryHelp(hTalkMuchSkew, "Рассказать о разных углах.", FALSE);
    /*-Andrey: moved to RNORM
  //-----------------------
  LDPUMA_Registry(&hDebugAutoTemplate,"Автоматическое ограничение области распознавания.",hMainTime);
  LDPUMA_RegistryHelp(hDebugAutoTemplate,"Автоматическое ограничение области распознавания.",FALSE);
  LDPUMA_Registry(&hAutoTemplate1,"Подробнее.",hDebugAutoTemplate);
  LDPUMA_Registry(&hAutoTemplateWrite,"Отписать в файл.",hDebugAutoTemplate);
  LDPUMA_RegistryHelp(hAutoTemplateWrite,"Отписать в файл.",FALSE);
  LDPUMA_Registry(&hAutoTemplateBC,"Большие компоненты.",hDebugAutoTemplate);
  LDPUMA_RegistryHelp(hAutoTemplateBC,"Большие компоненты.",FALSE);
  LDPUMA_Registry(&hAutoTemplateBCShow,"Прорисовка больших компонент.",hDebugAutoTemplate);
  LDPUMA_RegistryHelp(hAutoTemplateBCShow,"Прорисовка больших компонент.",FALSE);
  LDPUMA_Registry(&hAutoTemplateMar,"Поля.",hDebugAutoTemplate);
  LDPUMA_RegistryHelp(hAutoTemplateMar,"Поля.",FALSE);
  -*/

}
