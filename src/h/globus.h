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

#ifndef __GLOBUS_H
#define __GLOBUS_H

#include "cttypes.h"    // portable basic types
#include "err16.h"      // common use error values

/*** Modifiers ***/
/*
   Here we define next call modifiers:
   CLA_IMPO    -  class imported from DLL
   CLA_EXPO    -  class exported from DLL
   FUN_IMPO( ret_type ) - function imported from DLL
   FUN_EXPO( ret_type ) - function exported from DLL
      ret_type - function return type

   Usage of modifiers:
      // imported from DLL class in context of class user:
      class CLA_IMPO SomeClass {...};
      // exported from DLL class in context of it's definition:
      class CLA_EXPO SomeClass {...};

      // exported from DLL functions in context of definition one:
      FUN_EXPO( char* ) SomeFunc1() {...};
      FUN_EXPO( char* ) PASCAL SomeFunc2() {...};
      // imported from DLL functions prototypes in context of usage:
      FUN_IMPO( char* ) SomeFunc1();
      FUN_IMPO( void  ) FAR PASCAL SomeFunc2();
*/

#if defined( _MSC_VER )        /* Microsoft C/C++ compilers ******/
#if _MSC_VER > 800    /* MSVC 2.0 and later */
#define CLA_IMPO    __declspec( dllimport )
#define CLA_EXPO    __declspec( dllexport )
#define FUN_IMPO__  __declspec( dllimport )
#define __FUN_IMPO
#define FUN_EXPO__  __declspec( dllexport )
#define __FUN_EXPO
#endif
#elif __MINGW32__
#define CLA_IMPO
#define CLA_EXPO
#define FUN_IMPO__
#define __FUN_IMPO
#define FUN_EXPO__
#define __FUN_EXPO
#elif __GNUC__ >= 4 /* GNU Compiler Suite ******/
#define CLA_IMPO
#define CLA_EXPO __attribute__ ((visibility("default")))
#define FUN_IMPO__
#define __FUN_IMPO
#define FUN_EXPO__ __attribute__ ((visibility("default")))
#define __FUN_EXPO
#else /* unknown compiler ******************************************/
#define CLA_IMPO
#define CLA_EXPO
#define FUN_IMPO__
#define __FUN_IMPO
#define FUN_EXPO__
#define __FUN_EXPO
#endif
/*********************************************************/
#define FUN_IMPO( ret_type )  FUN_IMPO__ ret_type __FUN_IMPO
#define FUN_EXPO( ret_type )  FUN_EXPO__ ret_type __FUN_EXPO
/*********************************************************/

#endif /* __GLOBUS_H */
