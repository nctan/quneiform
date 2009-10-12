/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 ����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
 ��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

 * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
 ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
 ����� �� ��������.
 * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
 ������ ����������, ������������ ��� ���������������, ������ �����������
 ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
 ����������� ����� �� ��������.
 * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
 ���� ������������ � �������� �������� ��������� �/��� �����������
 ���������, ���������� �� ���� ��, ��� ���������������� �����������
 ����������.

 ��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
 ��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
 ������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
 ������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
 ����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
 ��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
 ������������� ������, ��������� � �������������� ��� ���������� ����������
 ������������� ������������� ��������� (������� ������ ������, ��� ������,
 ������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
 ������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
 �� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
 �������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

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

#ifndef __LINDEFS_H
#define __LINDEFS_H

enum {
	FILE_PCX = 900,
	ONLY_STRETCH = 910,
	ONLY_LINE = 911,
	ONLY_BOX_LINE = 912,
	NO_RASTER = 920,
	YES_RASTER = 921,
	YES_OUT_LIN = 930,
	NO_OUT_LIN = 931,
	YES_OUT_BIN = 940,
	NO_OUT_BIN = 941,
	ID_USER = 999
};

typedef struct lines_par {
	int vmaxwid;
	int vminlen;
	int vlsarea;
	int vlconnill;
	int vlconnils;
	int vlconniss;
	int vstartlenf;
	int vstartlens;
	int gmaxwid;
	int gminlen;
	int glsarea;
	int glconnill;
	int glconnils;
	int glconniss;
	int gstartlenf;
	int gstartlens;
	int lcont;
	int minboxw;
	int bsarf;
	int psarf;
	int psarb;
	int gangl;
	int langl;
	int linnm;
} PAR_LINE;

//#define STRETCH struct h_STRETCH //���� ��१��
//#define LINE struct h_LINE //���ᠭ�� ����� �����
//#define BOX struct h_BOX
//#define KNOTL struct h_KNOTL //����, ��������騥 �� ����祭�� �����
//#define LINES struct h_LINES //��� ������� �����, ���ᮢ � 㧫��.

struct STRETCH {
	CIF::Point16 Beg, End; //��砫� � ����� ��१�� ��᫥ ������
	CIF::Point16 BegOld, EndOld; //��砫� � ����� ��१�� �� ������
	float k; //������ ����� y=kx+b
	float b; //ᬥ饭��
	short thickness; //�।��� ⮫騭� ��१��
};

struct LINE {
	int NumStretch;
	STRETCH *Stretch; //[NumStretch] - �᫮ ��१��� ����� �����
};


struct BOX {
	short Top; //������ ���孥� ����� ����
	short Left; //������ ����� ����� ����
	short Bottom;//������ ������ ����� ����
	short Right; //������ �ࠢ�� ����� ����
	short Diag13, Diag24;
};

struct KNOTL {
	short InVertLine; //����� ���⨪��쭮� �����
	short InVertStretch;//� ����� ��१�� � ���浪� ��室� �� ��१���
	short InHorLine; //����� ��ਧ��⠫쭮� �����
	short InHorStretch; //� ����� ��१�� � ���浪� ��室� �� ��१���
	CIF::Point16 PointIntersection;//�窠 ����祭�� �����
	short TypeIntersection;//⨯ ����祭�� �����
};

struct LINES {
	short NumAll; //��饥 �᫮ �����
	short NumVert; //�᫮ ���⨪����� �����
	short NumHor; //�᫮ ��ਧ��⠫��� �����
	short NumOther;//�᫮ ��⠫��� �����
	LINE *Line; //[NumAll] - ���ᨢ �����
	short NumBox; //�᫮ ��אַ㣮�쭨���
	BOX *Box; //[NumBox] - ���ᨢ ��אַ㣮�쭨���
	short NumKnot;//�᫮ 㧫��
	KNOTL *Knotl; //[NumKnotl] - ���ᨢ 㧫��
};

struct BMPL {
	int widthBit;
	int height;
	int widthByte;
	int NumBuf;
	uchar **bmp; //bmp[NumBuf][64000]
};

#endif
