/*
 * imageinfo.h
 *
 *  Created on: 10.10.2009
 *      Author: uliss
 */

#ifndef IMAGEINFO_H_
#define IMAGEINFO_H_

#include "cttypes.h"

struct CIMAGE_ImageInfo {
	uint16_t wImageHeight;
	uint16_t wImageWidth;
	uint16_t wImageByteWidth;
	uint16_t wImageDisplacement;
	uint16_t wResolutionX;
	uint16_t wResolutionY;
	uchar bFotoMetrics;
	uchar bUnused;
	uint16_t wAddX;
	uint16_t wAddY;
};

typedef Bool16 (*PCIMAGE_Callback_ImageOpen)(CIMAGE_ImageInfo*);
typedef uint16_t (*PCIMAGE_Callback_ImageRead)(pchar, uint16_t);
typedef Bool16 (*PCIMAGE_Callback_ImageClose)(void);

struct CIMAGEIMAGECALLBACK {
	PCIMAGE_Callback_ImageOpen CIMAGE_ImageOpen;
	PCIMAGE_Callback_ImageRead CIMAGE_ImageRead;
	PCIMAGE_Callback_ImageClose CIMAGE_ImageClose;
};

struct CIMAGE_InfoDataInGet {
	uint32_t dwX;
	uint32_t dwY;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint16_t wByteWidth;
	uchar Reserved;
	uchar MaskFlag;
};

struct CIMAGE_InfoDataOutGet {
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint16_t wByteWidth;
	uint16_t byBit;
	uint32_t wBlackBit;
	puchar lpData;
};

struct CIMAGE_InfoDataInReplace {
	uint16_t byBit;
	uint32_t dwX;
	uint32_t dwY;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint16_t wByteWidth;
	puchar lpData;
	uchar Reserved;
	uint16_t wReserved;
	uchar MaskFlag;
};

struct CIMAGE_Rect {
	int32_t dwX;
	int32_t dwY;
	uint32_t dwWidth;
	uint32_t dwHeight;
};

struct CIMAGEBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

struct CIMAGERGBQUAD {
	uchar rgbBlue;
	uchar rgbGreen;
	uchar rgbRed;
	uchar rgbReserved;
};

#endif
