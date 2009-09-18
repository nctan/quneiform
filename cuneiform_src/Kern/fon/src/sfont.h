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

#include "c_types.h"
#include "cttypes.h"

#ifndef SFONT_H
#define SFONT_H

#define SINT signed short int
#define WR_MAX_HEIGHT   64
#define WR_MAX_WIDTH    128
#define WR_SMALL_LET    8

#define arrnum(a)       (sizeof(a)/sizeof(a[0]))

/*======= Data templates=============== */
#define NFIELDDWORD 2  // сколько двойных слов для полей
#define MAXFIELD   NFIELDDWORD*32
#define SetFields(dest,sour) {dest[0] =sour[0];dest[1]=sour[1];}
#define AddFields(dest,sour) {dest[0]|=sour[0];dest[1]|=sour[1];}
#define IntersectFields(sour1,sour2) ((sour1[0]&sour2[0])|(sour1[1]&sour2[1]))
#define NotEmptyField(sour) (sour[0]|sour[1])
#define EmptyField(sour)    ((sour[0]|sour[1])==0)
#define ClearFields(sour)   {sour[0]=sour[1]=0;}
void MakeDWORDField(int i, Word32 *fifi);
void AddDWORDField(int i, Word32 *fifi);

// маски параметров кластера
#define FON_CLU_SOLID    1
#define FON_CLU_ITALIC   2
#define FON_CLU_BOLD     4
#define FON_CLU_SERIF    8
#define FON_CLU_GELV    16
#define FON_CLU_NARROW  32
#define FON_CLU_FIXED   64
#define FON_CLU_TWIN   128

typedef struct tag_welet {
	char raster[WR_MAX_HEIGHT * WR_MAX_WIDTH];
	WORD w;
	WORD h; // actual width and height
	uint32_t summa; // summa of all weighted points
	WORD fill; // number of not-empty points in raster
	SINT num; // linear number of struct in file
	uint32_t fields[NFIELDDWORD]; //  30.11.98 - from which plat. fields
	SINT sr_col;
	SINT sr_row; // source row,col
	SINT nInCTB; // first in CTB
	SINT next; // next with same name
	WORD invalid; // !=0 -> cluster glue
	WORD attr;
	/*            solid      : 1, // confirmed
	 italic     : 1,
	 bold       : 1,
	 serif      : 1,
	 gelv       : 1, // gelvetic
	 update     : 1, // updated
	 fixed      : 1, // don't use auto threshold
	 twin       : 1, // has twin among strong clusters
	 : 8;
	 */
	BYTE let; // ascii code
	BYTE mw;
	BYTE mh; // medium width,height
	BYTE prob; // probability
	BYTE weight; // number of pattern were placed to raster
	char porog; // threshold level for cluster
	BYTE valid; // validity - CTB
	BYTE kegl; // tipical kegl
	uint32_t tablColumn; // from what columns
} welet;

typedef struct tag_raster_header {
	WORD w, h; // actual width and height
	WORD let;  // ascii code
	BYTE code[4]; // code of first intervals
	SINT sr_col, sr_row; // real coord of letter
	WORD num; // number of cluster that was accept raster
	WORD solid :1, // confirmed letter
			:15;
	BYTE reserved[6];
} raster_header;

// results of clusterization
typedef struct tag_clu_info {
	WORD totclu; // total number of clusters
	WORD nsymbols; // Number of symbols were clusterized
	WORD nsolid;
	WORD ninvalid;
	WORD ntwins;
	LONG memused; // memory were used
	SINT rc; // return code
} clu_info;

typedef struct tag_access_tab {
#define NCELL           256
#define ALL_CELLS       (NCELL-1)
#define ENTRIES         24
#define FREECELL        0xffff
#define SIGNATURE       "Fast access table2"
	WORD access_tab[NCELL][ENTRIES];
	WORD all_access[256];
	SINT wlsize; // size of cluster in bytes for external utilities
	char sign[20];
} access_tab;

typedef struct tag_version {
#define MAXVERS 8
	WORD let; // letter code
	SINT prob; // probability
	SINT aux; // working var
	WORD recsource :2; // Who is recognize
#define src_letter      0
#define src_full        1
#define src_3x5         2
#define src_ev          3
	WORD mixp :1; // prob was fixed
	SINT xo :2, yo :2;
} Version;

typedef struct tag_answer {
	Version vers[MAXVERS + 1];
	BYTE nvers;
	SINT ret;
	WORD recletterdone :1, // recognition method
			recfulldone :1, // recognition method
			recdone :1, // recognize was made
			reccopy :1; // answer was filled by copying
} answer;

// clusterization options
#define SIZEBUF   65532l   // get buffers of such sizes
#ifdef WIN32
#define MAXHAU    128      // maximal number of big bitmap buffers
#define MAXSYM    32000    // how many symbols ?
#else
#define MAXHAU    64       // maximal number of big bitmap buffers
#define MAXSYM    4095    // how many symbols ?
#endif

// how many clusters?
#define MAXWEICLUS (4*1024*1024L/sizeof(welet))

typedef struct tag_Nraster_header {
	WORD w;
	WORD h;
	WORD xbyte; // special !
	WORD let;
	BYTE *pHau; // pointer to picture
	BYTE *pHaur; // pointer to thick picture
	SINT sr_col;
	SINT sr_row;
	SINT nInCTB; // number in CTB
	WORD nField; // field in plat
	WORD num; // use to mark - if >0 - invalid, and = ClusterNumber
	BYTE solid :1, fat :1, italic :1, bold :1, serif :1, gelv :1, narrow :1,
			reservBits :1;
	BYTE kegl;
	BYTE prob;
	BYTE valid;
	BYTE tablColumn; // > 0 - from column of tabl
	BYTE reserv[1]; // 32 bytes align
} Nraster_header;

typedef struct tagInfoCluster {
	int32_t mw;
	int32_t mh;
	Word32 fields[NFIELDDWORD];
	Int16 count;
	Int16 first;
	Int16 let;
	BYTE invalid;
	BYTE good;
	BYTE prob;
	BYTE valid;
	BYTE nItalic;
	BYTE nSerif;
	BYTE nGelv;
	BYTE nBold;
	BYTE attr; // attributes - italic,...
	BYTE isSame; // working for test 0O6b
	Word32 tablColumn;
} InfoCluster;

typedef struct tagFontField {
	Word32 field[NFIELDDWORD];
	Word32 testField[NFIELDDWORD];
	Int16 sBig;
	Int16 sLit;
	Word16 inFont[256];
} FONTFIELD;

#define METKA_VALID 0x80  // get as good in font
#define POROG_ANGLES  20           // 18 ?
SINT Razmaz2(BYTE *bSource, BYTE *bDest, SINT xbit, SINT yrow, SINT porogX,
		SINT porogY);
SINT Razmaz2xByte(BYTE *bSource, BYTE *bDest, SINT xbyteAll, SINT xbit,
		SINT yrow, SINT porogX, SINT porogY);
int TestFromGoodRaster(int start, Nraster_header *rh, int NumAll, int NumClus,
		SINT *nClus, InfoCluster *infoC, int porog);
LONG StartHausdorfDLL(int num, void *ExternBuf, uint32_t SizeExternBuf);
void EndHausdorfDLL(void);
int32_t GetNumMemory(void);
void EndNumMemory(void);
int32_t StartAddMemCluster(Word8 *metkaValid, int32_t CurClus, Int16 countFont,
		Word32 *allFields);
int32_t AddClusterMemFont(welet *wel);
SINT Razmaz(BYTE *bSource, BYTE *bDest, SINT xbyte, SINT xbit, SINT yrow,
		BYTE bold);
#endif

