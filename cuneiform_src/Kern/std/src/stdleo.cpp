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

#include "internal.h"
#pragma hdrstop

#include "std.h"

#include "compat_defs.h"

/* FIXME*/
#define lstrlenW(a) 0

static Int8 leo_case_upper[]={
/* 000 */ ' ',
/* 001 */ ' ',
/* 002 */ ' ',
/* 003 */ ' ',
/* 004 */ ' ',
/* 005 */ ' ',
/* 006 */ ' ',
/* 007 */ ' ',
/* 008 */ ' ',
/* 009 */ ' ',
/* 010 */ ' ',
/* 011 */ ' ',
/* 012 */ ' ',
/* 013 */ ' ',
/* 014 */ ' ',
/* 015 */ ' ',
/* 016 */ ' ',
/* 017 */ ' ',
/* 018 */ ' ',
/* 019 */ ' ',
/* 020 */ ' ',
/* 021 */ ' ',
/* 022 */ ' ',
/* 023 */ ' ',
/* 024 */ ' ',
/* 025 */ ' ',
/* 026 */ ' ',
/* 027 */ ' ',
/* 028 */ ' ',
/* 029 */ ' ',
/* 030 */ ' ',
/* 031 */ ' ',
/*  (032,0x20) */ ' ',
/* !(033,0x21) */ '!',
/* '(034,0x22) */ '\'',
/* #(035,0x23) */ '#',
/* $(036,0x24) */ '$',
/* %(037,0x25) */ '%',
/* &(038,0x26) */ '&',
/* '(039,0x27) */ '\'',
/* ((040,0x28) */ '(',
/* )(041,0x29) */ ')',
/* *(042,0x2a) */ '*',
/* +(043,0x2b) */ '+',
/* ,(044,0x2c) */ ',',
/* -(045,0x2d) */ '-',
/* .(046,0x2e) */ '.',
/* /(047,0x2f) */ '/',
/* 0(048,0x30) */ '0',
/* 1(049,0x31) */ '1',
/* 2(050,0x32) */ '2',
/* 3(051,0x33) */ '3',
/* 4(052,0x34) */ '4',
/* 5(053,0x35) */ '5',
/* 6(054,0x36) */ '6',
/* 7(055,0x37) */ '7',
/* 8(056,0x38) */ '8',
/* 9(057,0x39) */ '9',
/* :(058,0x3a) */ ':',
/* ;(059,0x3b) */ ';',
/* <(060,0x3c) */ '<',
/* =(061,0x3d) */ '=',
/* >(062,0x3e) */ '>',
/* ?(063,0x3f) */ '?',
/* @(064,0x40) */ '@',
/* A(065,0x41) */ 'A',
/* B(066,0x42) */ 'B',
/* C(067,0x43) */ 'C',
/* D(068,0x44) */ 'D',
/* E(069,0x45) */ 'E',
/* F(070,0x46) */ 'F',
/* G(071,0x47) */ 'G',
/* H(072,0x48) */ 'H',
/* I(073,0x49) */ 'I',
/* J(074,0x4a) */ 'J',
/* K(075,0x4b) */ 'K',
/* L(076,0x4c) */ 'L',
/* M(077,0x4d) */ 'M',
/* N(078,0x4e) */ 'N',
/* O(079,0x4f) */ 'O',
/* P(080,0x50) */ 'P',
/* Q(081,0x51) */ 'Q',
/* R(082,0x52) */ 'R',
/* S(083,0x53) */ 'S',
/* T(084,0x54) */ 'T',
/* U(085,0x55) */ 'U',
/* V(086,0x56) */ 'V',
/* W(087,0x57) */ 'W',
/* X(088,0x58) */ 'X',
/* Y(089,0x59) */ 'Y',
/* Z(090,0x5a) */ 'Z',
/* [(091,0x5b) */ '[',
/* \(092,0x5c) */ '\\',
/* ](093,0x5d) */ ']',
/* ^(094,0x5e) */ '^',
/* _(095,0x5f) */ '_',
/* `(096,0x60) */ '`',
/* a(097,0x61) */ 'a',
/* b(098,0x62) */ 'b',
/* c(099,0x63) */ 'C',
/* d(100,0x64) */ 'd',
/* e(101,0x65) */ 'e',
/* f(102,0x66) */ 'f',
/* g(103,0x67) */ 'g',
/* h(104,0x68) */ 'h',
/* i(105,0x69) */ 'i',
/* j(106,0x6a) */ 'j',
/* k(107,0x6b) */ 'k',
/* l(108,0x6c) */ 'l',
/* m(109,0x6d) */ 'm',
/* n(110,0x6e) */ 'n',
/* o(111,0x6f) */ 'O',
/* p(112,0x70) */ 'P',
/* q(113,0x71) */ 'q',
/* r(114,0x72) */ 'r',
/* s(115,0x73) */ 'S',
/* t(116,0x74) */ 't',
/* u(117,0x75) */ 'U',
/* v(118,0x76) */ 'V',
/* w(119,0x77) */ 'W',
/* x(120,0x78) */ 'X',
/* y(121,0x79) */ 'Y',
/* z(122,0x7a) */ 'Z',
/* {(123,0x7b) */ '{',
/* |(124,0x7c) */ '|',
/* }(125,0x7d) */ '}',
/* ~(126,0x7e) */ '~',
/* (127,0x7f) */ '',
/* �(128,0x80) */ '�',
/* �(129,0x81) */ '�',
/* �(130,0x82) */ '�',
/* �(131,0x83) */ '�',
/* �(132,0x84) */ '�',
/* �(133,0x85) */ '�',
/* �(134,0x86) */ '�',
/* �(135,0x87) */ '�',
/* �(136,0x88) */ '�',
/* �(137,0x89) */ '�',
/* �(138,0x8a) */ '�',
/* �(139,0x8b) */ '�',
/* �(140,0x8c) */ '�',
/* �(141,0x8d) */ '�',
/* �(142,0x8e) */ '�',
/* �(143,0x8f) */ '�',
/* �(144,0x90) */ '�',
/* �(145,0x91) */ '�',
/* �(146,0x92) */ '�',
/* �(147,0x93) */ '�',
/* �(148,0x94) */ '�',
/* �(149,0x95) */ '�',
/* �(150,0x96) */ '�',
/* �(151,0x97) */ '�',
/* �(152,0x98) */ '�',
/* �(153,0x99) */ '�',
/* �(154,0x9a) */ '�',
/* �(155,0x9b) */ '�',
/* �(156,0x9c) */ '�',
/* �(157,0x9d) */ '�',
/* �(158,0x9e) */ '�',
/* �(159,0x9f) */ '�',
/* �(160,0xa0) */ '�',
/* �(161,0xa1) */ '�',
/* �(162,0xa2) */ '�',
/* �(163,0xa3) */ '�',
/* �(164,0xa4) */ '�',
/* �(165,0xa5) */ '�',
/* �(166,0xa6) */ '�',
/* �(167,0xa7) */ '�',
/* �(168,0xa8) */ '�',
/* �(169,0xa9) */ '�',
/* �(170,0xaa) */ '�',
/* �(171,0xab) */ '�',
/* �(172,0xac) */ '�',
/* �(173,0xad) */ '�',
/* �(174,0xae) */ '�',
/* �(175,0xaf) */ '�',
/* �(176,0xb0) */ '�',
/* �(177,0xb1) */ '�',
/* �(178,0xb2) */ '�',
/* �(179,0xb3) */ '�',
/* �(180,0xb4) */ '�',
/* �(181,0xb5) */ '�',
/* �(182,0xb6) */ '�',
/* �(183,0xb7) */ '�',
/* �(184,0xb8) */ '�',
/* �(185,0xb9) */ '�',
/* �(186,0xba) */ '�',
/* �(187,0xbb) */ '�',
/* �(188,0xbc) */ '�',
/* �(189,0xbd) */ '�',
/* �(190,0xbe) */ '�',
/* �(191,0xbf) */ '�',
/* �(192,0xc0) */ '�',
/* �(193,0xc1) */ '�',
/* �(194,0xc2) */ '�',
/* �(195,0xc3) */ '�',
/* �(196,0xc4) */ '�',
/* �(197,0xc5) */ '�',
/* �(198,0xc6) */ '�',
/* �(199,0xc7) */ '�',
/* �(200,0xc8) */ '�',
/* �(201,0xc9) */ '�',
/* �(202,0xca) */ '�',
/* �(203,0xcb) */ '�',
/* �(204,0xcc) */ '�',
/* �(205,0xcd) */ '�',
/* �(206,0xce) */ '�',
/* �(207,0xcf) */ '�',
/* �(208,0xd0) */ '�',
/* �(209,0xd1) */ '�',
/* �(210,0xd2) */ '�',
/* �(211,0xd3) */ '�',
/* �(212,0xd4) */ '�',
/* �(213,0xd5) */ '�',
/* �(214,0xd6) */ '�',
/* �(215,0xd7) */ '�',
/* �(216,0xd8) */ '�',
/* �(217,0xd9) */ '�',
/* �(218,0xda) */ '�',
/* �(219,0xdb) */ '�',
/* �(220,0xdc) */ '�',
/* �(221,0xdd) */ '�',
/* �(222,0xde) */ '�',
/* �(223,0xdf) */ '�',
/* �(224,0xe0) */ '�',
/* �(225,0xe1) */ '�',
/* �(226,0xe2) */ '�',
/* �(227,0xe3) */ '�',
/* �(228,0xe4) */ '�',
/* �(229,0xe5) */ '�',
/* �(230,0xe6) */ '�',
/* �(231,0xe7) */ '�',
/* �(232,0xe8) */ '�',
/* �(233,0xe9) */ '�',
/* �(234,0xea) */ '�',
/* �(235,0xeb) */ '�',
/* �(236,0xec) */ '�',
/* �(237,0xed) */ '�',
/* �(238,0xee) */ '�',
/* �(239,0xef) */ '�',
/* �(240,0xf0) */ '�',
/* �(241,0xf1) */ '�',
/* �(242,0xf2) */ '�',
/* �(243,0xf3) */ '�',
/* �(244,0xf4) */ '�',
/* �(245,0xf5) */ '�',
/* �(246,0xf6) */ '�',
/* �(247,0xf7) */ '�',
/* �(248,0xf8) */ '�',
/* �(249,0xf9) */ '�',
/* �(250,0xfa) */ '�',
/* �(251,0xfb) */ '�',
/* �(252,0xfc) */ '�',
/* �(253,0xfd) */ '�',
/* �(254,0xfe) */ '�',
/* �(255,0xff) */ '�'
};
static Int8 leo_case_lower[]={
/* 000 */ ' ',
/* 001 */ ' ',
/* 002 */ ' ',
/* 003 */ ' ',
/* 004 */ ' ',
/* 005 */ ' ',
/* 006 */ ' ',
/* 007 */ ' ',
/* 008 */ ' ',
/* 009 */ ' ',
/* 010 */ ' ',
/* 011 */ ' ',
/* 012 */ ' ',
/* 013 */ ' ',
/* 014 */ ' ',
/* 015 */ ' ',
/* 016 */ ' ',
/* 017 */ ' ',
/* 018 */ ' ',
/* 019 */ ' ',
/* 020 */ ' ',
/* 021 */ ' ',
/* 022 */ ' ',
/* 023 */ ' ',
/* 024 */ ' ',
/* 025 */ ' ',
/* 026 */ ' ',
/* 027 */ ' ',
/* 028 */ ' ',
/* 029 */ ' ',
/* 030 */ ' ',
/* 031 */ ' ',
/*  (032,0x20) */ ' ',
/* !(033,0x21) */ '!',
/* '(034,0x22) */ '\'',
/* #(035,0x23) */ '#',
/* $(036,0x24) */ '$',
/* %(037,0x25) */ '%',
/* &(038,0x26) */ '&',
/* '(039,0x27) */ '\'',
/* ((040,0x28) */ '(',
/* )(041,0x29) */ ')',
/* *(042,0x2a) */ '*',
/* +(043,0x2b) */ '+',
/* ,(044,0x2c) */ ',',
/* -(045,0x2d) */ '-',
/* .(046,0x2e) */ '.',
/* /(047,0x2f) */ '/',
/* 0(048,0x30) */ '0',
/* 1(049,0x31) */ '1',
/* 2(050,0x32) */ '2',
/* 3(051,0x33) */ '3',
/* 4(052,0x34) */ '4',
/* 5(053,0x35) */ '5',
/* 6(054,0x36) */ '6',
/* 7(055,0x37) */ '7',
/* 8(056,0x38) */ '8',
/* 9(057,0x39) */ '9',
/* :(058,0x3a) */ ':',
/* ;(059,0x3b) */ ';',
/* <(060,0x3c) */ '<',
/* =(061,0x3d) */ '=',
/* >(062,0x3e) */ '>',
/* ?(063,0x3f) */ '?',
/* @(064,0x40) */ '@',
/* A(065,0x41) */ 'A',
/* B(066,0x42) */ 'B',
/* C(067,0x43) */ 'c',
/* D(068,0x44) */ 'D',
/* E(069,0x45) */ 'E',
/* F(070,0x46) */ 'F',
/* G(071,0x47) */ 'G',
/* H(072,0x48) */ 'H',
/* I(073,0x49) */ 'I',
/* J(074,0x4a) */ 'J',
/* K(075,0x4b) */ 'K',
/* L(076,0x4c) */ 'L',
/* M(077,0x4d) */ 'M',
/* N(078,0x4e) */ 'N',
/* O(079,0x4f) */ 'o',
/* P(080,0x50) */ 'p',
/* Q(081,0x51) */ 'Q',
/* R(082,0x52) */ 'R',
/* S(083,0x53) */ 's',
/* T(084,0x54) */ 'T',
/* U(085,0x55) */ 'u',
/* V(086,0x56) */ 'v',
/* W(087,0x57) */ 'w',
/* X(088,0x58) */ 'x',
/* Y(089,0x59) */ 'y',
/* Z(090,0x5a) */ 'z',
/* [(091,0x5b) */ '[',
/* \(092,0x5c) */ '\\',
/* ](093,0x5d) */ ']',
/* ^(094,0x5e) */ '^',
/* _(095,0x5f) */ '_',
/* `(096,0x60) */ '`',
/* a(097,0x61) */ 'a',
/* b(098,0x62) */ 'b',
/* c(099,0x63) */ 'C',
/* d(100,0x64) */ 'd',
/* e(101,0x65) */ 'e',
/* f(102,0x66) */ 'f',
/* g(103,0x67) */ 'g',
/* h(104,0x68) */ 'h',
/* i(105,0x69) */ 'i',
/* j(106,0x6a) */ 'j',
/* k(107,0x6b) */ 'k',
/* l(108,0x6c) */ 'l',
/* m(109,0x6d) */ 'm',
/* n(110,0x6e) */ 'n',
/* o(111,0x6f) */ 'o',
/* p(112,0x70) */ 'p',
/* q(113,0x71) */ 'q',
/* r(114,0x72) */ 'r',
/* s(115,0x73) */ 's',
/* t(116,0x74) */ 't',
/* u(117,0x75) */ 'u',
/* v(118,0x76) */ 'v',
/* w(119,0x77) */ 'w',
/* x(120,0x78) */ 'x',
/* y(121,0x79) */ 'y',
/* z(122,0x7a) */ 'z',
/* {(123,0x7b) */ '{',
/* |(124,0x7c) */ '|',
/* }(125,0x7d) */ '}',
/* ~(126,0x7e) */ '~',
/* (127,0x7f) */ '',
/* �(128,0x80) */ '�',
/* �(129,0x81) */ '�',
/* �(130,0x82) */ '�',
/* �(131,0x83) */ '�',
/* �(132,0x84) */ '�',
/* �(133,0x85) */ '�',
/* �(134,0x86) */ '�',
/* �(135,0x87) */ '�',
/* �(136,0x88) */ '�',
/* �(137,0x89) */ '�',
/* �(138,0x8a) */ '�',
/* �(139,0x8b) */ '�',
/* �(140,0x8c) */ '�',
/* �(141,0x8d) */ '�',
/* �(142,0x8e) */ '�',
/* �(143,0x8f) */ '�',
/* �(144,0x90) */ '�',
/* �(145,0x91) */ '�',
/* �(146,0x92) */ '�',
/* �(147,0x93) */ '�',
/* �(148,0x94) */ '�',
/* �(149,0x95) */ '�',
/* �(150,0x96) */ '�',
/* �(151,0x97) */ '�',
/* �(152,0x98) */ '�',
/* �(153,0x99) */ '�',
/* �(154,0x9a) */ '�',
/* �(155,0x9b) */ '�',
/* �(156,0x9c) */ '�',
/* �(157,0x9d) */ '�',
/* �(158,0x9e) */ '�',
/* �(159,0x9f) */ '�',
/* �(160,0xa0) */ '�',
/* �(161,0xa1) */ '�',
/* �(162,0xa2) */ '�',
/* �(163,0xa3) */ '�',
/* �(164,0xa4) */ '�',
/* �(165,0xa5) */ '�',
/* �(166,0xa6) */ '�',
/* �(167,0xa7) */ '�',
/* �(168,0xa8) */ '�',
/* �(169,0xa9) */ '�',
/* �(170,0xaa) */ '�',
/* �(171,0xab) */ '�',
/* �(172,0xac) */ '�',
/* �(173,0xad) */ '�',
/* �(174,0xae) */ '�',
/* �(175,0xaf) */ '�',
/* �(176,0xb0) */ '�',
/* �(177,0xb1) */ '�',
/* �(178,0xb2) */ '�',
/* �(179,0xb3) */ '�',
/* �(180,0xb4) */ '�',
/* �(181,0xb5) */ '�',
/* �(182,0xb6) */ '�',
/* �(183,0xb7) */ '�',
/* �(184,0xb8) */ '�',
/* �(185,0xb9) */ '�',
/* �(186,0xba) */ '�',
/* �(187,0xbb) */ '�',
/* �(188,0xbc) */ '�',
/* �(189,0xbd) */ '�',
/* �(190,0xbe) */ '�',
/* �(191,0xbf) */ '�',
/* �(192,0xc0) */ '�',
/* �(193,0xc1) */ '�',
/* �(194,0xc2) */ '�',
/* �(195,0xc3) */ '�',
/* �(196,0xc4) */ '�',
/* �(197,0xc5) */ '�',
/* �(198,0xc6) */ '�',
/* �(199,0xc7) */ '�',
/* �(200,0xc8) */ '�',
/* �(201,0xc9) */ '�',
/* �(202,0xca) */ '�',
/* �(203,0xcb) */ '�',
/* �(204,0xcc) */ '�',
/* �(205,0xcd) */ '�',
/* �(206,0xce) */ '�',
/* �(207,0xcf) */ '�',
/* �(208,0xd0) */ '�',
/* �(209,0xd1) */ '�',
/* �(210,0xd2) */ '�',
/* �(211,0xd3) */ '�',
/* �(212,0xd4) */ '�',
/* �(213,0xd5) */ '�',
/* �(214,0xd6) */ '�',
/* �(215,0xd7) */ '�',
/* �(216,0xd8) */ '�',
/* �(217,0xd9) */ '�',
/* �(218,0xda) */ '�',
/* �(219,0xdb) */ '�',
/* �(220,0xdc) */ '�',
/* �(221,0xdd) */ '�',
/* �(222,0xde) */ '�',
/* �(223,0xdf) */ '�',
/* �(224,0xe0) */ '�',
/* �(225,0xe1) */ '�',
/* �(226,0xe2) */ '�',
/* �(227,0xe3) */ '�',
/* �(228,0xe4) */ '�',
/* �(229,0xe5) */ '�',
/* �(230,0xe6) */ '�',
/* �(231,0xe7) */ '�',
/* �(232,0xe8) */ '�',
/* �(233,0xe9) */ '�',
/* �(234,0xea) */ '�',
/* �(235,0xeb) */ '�',
/* �(236,0xec) */ '�',
/* �(237,0xed) */ '�',
/* �(238,0xee) */ '�',
/* �(239,0xef) */ '�',
/* �(240,0xf0) */ '�',
/* �(241,0xf1) */ '�',
/* �(242,0xf2) */ '�',
/* �(243,0xf3) */ '�',
/* �(244,0xf4) */ '�',
/* �(245,0xf5) */ '�',
/* �(246,0xf6) */ '�',
/* �(247,0xf7) */ '�',
/* �(248,0xf8) */ '�',
/* �(249,0xf9) */ '�',
/* �(250,0xfa) */ '�',
/* �(251,0xfb) */ '�',
/* �(252,0xfc) */ '�',
/* �(253,0xfd) */ '�',
/* �(254,0xfe) */ '�',
/* �(255,0xff) */ '�'
};

static Int32 leo_case[]={
/* 000 */ 0,
/* 001 */ 0,
/* 002 */ 0,
/* 003 */ 0,
/* 004 */ 0,
/* 005 */ 0,
/* 006 */ 0,
/* 007 */ 0,
/* 008 */ 0,
/* 009 */ 0,
/* 010 */ 0,
/* 011 */ 0,
/* 012 */ 0,
/* 013 */ 0,
/* 014 */ 0,
/* 015 */ 0,
/* 016 */ 0,
/* 017 */ 0,
/* 018 */ 0,
/* 019 */ 0,
/* 020 */ 0,
/* 021 */ 0,
/* 022 */ 0,
/* 023 */ 0,
/* 024 */ 0,
/* 025 */ 0,
/* 026 */ 0,
/* 027 */ 0,
/* 028 */ 0,
/* 029 */ 0,
/* 030 */ 0,
/* 031 */ 0,
/*  (032,0x20) */ 0,
/* !(033,0x21) */ 0,
/* "(034,0x22) */ 0,
/* #(035,0x23) */ STD_LEO_CASE_UPPER,
/* $(036,0x24) */ 0,
/* %(037,0x25) */ 0,
/* &(038,0x26) */ 0,
/* '(039,0x27) */ 0,
/* ((040,0x28) */ 0,
/* )(041,0x29) */ 0,
/* *(042,0x2a) */ 0,
/* +(043,0x2b) */ 0,
/* ,(044,0x2c) */ 0,
/* -(045,0x2d) */ 0,
/* .(046,0x2e) */ 0,
/* /(047,0x2f) */ 0,
/* 0(048,0x30) */ STD_LEO_CASE_UPPER,
/* 1(049,0x31) */ STD_LEO_CASE_UPPER,
/* 2(050,0x32) */ STD_LEO_CASE_UPPER,
/* 3(051,0x33) */ STD_LEO_CASE_UPPER,
/* 4(052,0x34) */ STD_LEO_CASE_UPPER,
/* 5(053,0x35) */ STD_LEO_CASE_UPPER,
/* 6(054,0x36) */ STD_LEO_CASE_UPPER,
/* 7(055,0x37) */ STD_LEO_CASE_UPPER,
/* 8(056,0x38) */ STD_LEO_CASE_UPPER,
/* 9(057,0x39) */ STD_LEO_CASE_UPPER,
/* :(058,0x3a) */ 0,
/* ;(059,0x3b) */ 0,
/* <(060,0x3c) */ 0,
/* =(061,0x3d) */ 0,
/* >(062,0x3e) */ 0,
/* ?(063,0x3f) */ 0,
/* @(064,0x40) */ 0,
/* A(065,0x41) */ STD_LEO_CASE_UPPER,
/* B(066,0x42) */ STD_LEO_CASE_UPPER,
/* C(067,0x43) */ STD_LEO_CASE_UPPER,
/* D(068,0x44) */ STD_LEO_CASE_UPPER,
/* E(069,0x45) */ STD_LEO_CASE_UPPER,
/* F(070,0x46) */ STD_LEO_CASE_UPPER,
/* G(071,0x47) */ STD_LEO_CASE_UPPER,
/* H(072,0x48) */ STD_LEO_CASE_UPPER,
/* I(073,0x49) */ STD_LEO_CASE_UPPER,
/* J(074,0x4a) */ STD_LEO_CASE_UPPER,
/* K(075,0x4b) */ STD_LEO_CASE_UPPER,
/* L(076,0x4c) */ STD_LEO_CASE_UPPER,
/* M(077,0x4d) */ STD_LEO_CASE_UPPER,
/* N(078,0x4e) */ STD_LEO_CASE_UPPER,
/* O(079,0x4f) */ STD_LEO_CASE_UPPER,
/* P(080,0x50) */ STD_LEO_CASE_UPPER,
/* Q(081,0x51) */ STD_LEO_CASE_UPPER,
/* R(082,0x52) */ STD_LEO_CASE_UPPER,
/* S(083,0x53) */ STD_LEO_CASE_UPPER,
/* T(084,0x54) */ STD_LEO_CASE_UPPER,
/* U(085,0x55) */ STD_LEO_CASE_UPPER,
/* V(086,0x56) */ STD_LEO_CASE_UPPER,
/* W(087,0x57) */ STD_LEO_CASE_UPPER,
/* X(088,0x58) */ STD_LEO_CASE_UPPER,
/* Y(089,0x59) */ STD_LEO_CASE_UPPER,
/* Z(090,0x5a) */ STD_LEO_CASE_UPPER,
/* [(091,0x5b) */ 0,
/* \(092,0x5c) */ 0,
/* ](093,0x5d) */ 0,
/* ^(094,0x5e) */ 0,
/* _(095,0x5f) */ 0,
/* `(096,0x60) */ 0,
/* a(097,0x61) */ STD_LEO_CASE_LOWER,
/* b(098,0x62) */ STD_LEO_CASE_LOWER,
/* c(099,0x63) */ STD_LEO_CASE_LOWER,
/* d(100,0x64) */ STD_LEO_CASE_LOWER,
/* e(101,0x65) */ STD_LEO_CASE_LOWER,
/* f(102,0x66) */ STD_LEO_CASE_LOWER,
/* g(103,0x67) */ STD_LEO_CASE_LOWER,
/* h(104,0x68) */ STD_LEO_CASE_LOWER,
/* i(105,0x69) */ STD_LEO_CASE_LOWER,
/* j(106,0x6a) */ STD_LEO_CASE_LOWER,
/* k(107,0x6b) */ STD_LEO_CASE_LOWER,
/* l(108,0x6c) */ STD_LEO_CASE_LOWER,
/* m(109,0x6d) */ STD_LEO_CASE_LOWER,
/* n(110,0x6e) */ STD_LEO_CASE_LOWER,
/* o(111,0x6f) */ STD_LEO_CASE_LOWER,
/* p(112,0x70) */ STD_LEO_CASE_LOWER,
/* q(113,0x71) */ STD_LEO_CASE_LOWER,
/* r(114,0x72) */ STD_LEO_CASE_LOWER,
/* s(115,0x73) */ STD_LEO_CASE_LOWER,
/* t(116,0x74) */ STD_LEO_CASE_LOWER,
/* u(117,0x75) */ STD_LEO_CASE_LOWER,
/* v(118,0x76) */ STD_LEO_CASE_LOWER,
/* w(119,0x77) */ STD_LEO_CASE_LOWER,
/* x(120,0x78) */ STD_LEO_CASE_LOWER,
/* y(121,0x79) */ STD_LEO_CASE_LOWER,
/* z(122,0x7a) */ STD_LEO_CASE_LOWER,
/* {(123,0x7b) */ 0,
/* |(124,0x7c) */ 0,
/* }(125,0x7d) */ 0,
/* ~(126,0x7e) */ 0,
/* (127,0x7f) */ 0,
/* �(128,0x80) */ STD_LEO_CASE_UPPER,
/* �(129,0x81) */ STD_LEO_CASE_UPPER,
/* �(130,0x82) */ STD_LEO_CASE_UPPER,
/* �(131,0x83) */ STD_LEO_CASE_UPPER,
/* �(132,0x84) */ STD_LEO_CASE_UPPER,
/* �(133,0x85) */ STD_LEO_CASE_UPPER,
/* �(134,0x86) */ STD_LEO_CASE_UPPER,
/* �(135,0x87) */ STD_LEO_CASE_UPPER,
/* �(136,0x88) */ STD_LEO_CASE_UPPER,
/* �(137,0x89) */ STD_LEO_CASE_UPPER,
/* �(138,0x8a) */ STD_LEO_CASE_UPPER,
/* �(139,0x8b) */ STD_LEO_CASE_UPPER,
/* �(140,0x8c) */ STD_LEO_CASE_UPPER,
/* �(141,0x8d) */ STD_LEO_CASE_UPPER,
/* �(142,0x8e) */ STD_LEO_CASE_UPPER,
/* �(143,0x8f) */ STD_LEO_CASE_UPPER,
/* �(144,0x90) */ STD_LEO_CASE_UPPER,
/* �(145,0x91) */ STD_LEO_CASE_UPPER,
/* �(146,0x92) */ STD_LEO_CASE_UPPER,
/* �(147,0x93) */ STD_LEO_CASE_UPPER,
/* �(148,0x94) */ STD_LEO_CASE_UPPER,
/* �(149,0x95) */ STD_LEO_CASE_UPPER,
/* �(150,0x96) */ STD_LEO_CASE_UPPER,
/* �(151,0x97) */ STD_LEO_CASE_UPPER,
/* �(152,0x98) */ STD_LEO_CASE_UPPER,
/* �(153,0x99) */ STD_LEO_CASE_UPPER,
/* �(154,0x9a) */ STD_LEO_CASE_UPPER,
/* �(155,0x9b) */ STD_LEO_CASE_UPPER,
/* �(156,0x9c) */ STD_LEO_CASE_UPPER,
/* �(157,0x9d) */ STD_LEO_CASE_UPPER,
/* �(158,0x9e) */ STD_LEO_CASE_UPPER,
/* �(159,0x9f) */ STD_LEO_CASE_UPPER,
/* �(160,0xa0) */ STD_LEO_CASE_LOWER,
/* �(161,0xa1) */ STD_LEO_CASE_LOWER,
/* �(162,0xa2) */ STD_LEO_CASE_LOWER,
/* �(163,0xa3) */ STD_LEO_CASE_LOWER,
/* �(164,0xa4) */ STD_LEO_CASE_LOWER,
/* �(165,0xa5) */ STD_LEO_CASE_LOWER,
/* �(166,0xa6) */ STD_LEO_CASE_LOWER,
/* �(167,0xa7) */ STD_LEO_CASE_LOWER,
/* �(168,0xa8) */ STD_LEO_CASE_LOWER,
/* �(169,0xa9) */ STD_LEO_CASE_LOWER,
/* �(170,0xaa) */ STD_LEO_CASE_LOWER,
/* �(171,0xab) */ STD_LEO_CASE_LOWER,
/* �(172,0xac) */ STD_LEO_CASE_LOWER,
/* �(173,0xad) */ STD_LEO_CASE_LOWER,
/* �(174,0xae) */ STD_LEO_CASE_LOWER,
/* �(175,0xaf) */ STD_LEO_CASE_LOWER,
/* �(176,0xb0) */ 0,
/* �(177,0xb1) */ 0,
/* �(178,0xb2) */ 0,
/* �(179,0xb3) */ 0,
/* �(180,0xb4) */ 0,
/* �(181,0xb5) */ 0,
/* �(182,0xb6) */ 0,
/* �(183,0xb7) */ 0,
/* �(184,0xb8) */ 0,
/* �(185,0xb9) */ 0,
/* �(186,0xba) */ 0,
/* �(187,0xbb) */ 0,
/* �(188,0xbc) */ 0,
/* �(189,0xbd) */ 0,
/* �(190,0xbe) */ 0,
/* �(191,0xbf) */ 0,
/* �(192,0xc0) */ 0,
/* �(193,0xc1) */ 0,
/* �(194,0xc2) */ 0,
/* �(195,0xc3) */ 0,
/* �(196,0xc4) */ 0,
/* �(197,0xc5) */ 0,
/* �(198,0xc6) */ 0,
/* �(199,0xc7) */ 0,
/* �(200,0xc8) */ 0,
/* �(201,0xc9) */ 0,
/* �(202,0xca) */ 0,
/* �(203,0xcb) */ 0,
/* �(204,0xcc) */ 0,
/* �(205,0xcd) */ 0,
/* �(206,0xce) */ 0,
/* �(207,0xcf) */ 0,
/* �(208,0xd0) */ 0,
/* �(209,0xd1) */ 0,
/* �(210,0xd2) */ 0,
/* �(211,0xd3) */ 0,
/* �(212,0xd4) */ 0,
/* �(213,0xd5) */ 0,
/* �(214,0xd6) */ 0,
/* �(215,0xd7) */ 0,
/* �(216,0xd8) */ 0,
/* �(217,0xd9) */ 0,
/* �(218,0xda) */ 0,
/* �(219,0xdb) */ 0,
/* �(220,0xdc) */ 0,
/* �(221,0xdd) */ 0,
/* �(222,0xde) */ 0,
/* �(223,0xdf) */ 0,
/* �(224,0xe0) */ STD_LEO_CASE_LOWER,
/* �(225,0xe1) */ STD_LEO_CASE_LOWER,
/* �(226,0xe2) */ STD_LEO_CASE_LOWER,
/* �(227,0xe3) */ STD_LEO_CASE_LOWER,
/* �(228,0xe4) */ STD_LEO_CASE_LOWER,
/* �(229,0xe5) */ STD_LEO_CASE_LOWER,
/* �(230,0xe6) */ STD_LEO_CASE_LOWER,
/* �(231,0xe7) */ STD_LEO_CASE_LOWER,
/* �(232,0xe8) */ STD_LEO_CASE_LOWER,
/* �(233,0xe9) */ STD_LEO_CASE_LOWER,
/* �(234,0xea) */ STD_LEO_CASE_LOWER,
/* �(235,0xeb) */ STD_LEO_CASE_LOWER,
/* �(236,0xec) */ STD_LEO_CASE_LOWER,
/* �(237,0xed) */ STD_LEO_CASE_LOWER,
/* �(238,0xee) */ STD_LEO_CASE_LOWER,
/* �(239,0xef) */ STD_LEO_CASE_LOWER,
/* �(240,0xf0) */ STD_LEO_CASE_UPPER,
/* �(241,0xf1) */ STD_LEO_CASE_LOWER,
/* �(242,0xf2) */ 0,
/* �(243,0xf3) */ 0,
/* �(244,0xf4) */ 0,
/* �(245,0xf5) */ 0,
/* �(246,0xf6) */ 0,
/* �(247,0xf7) */ 0,
/* �(248,0xf8) */ 0,
/* �(249,0xf9) */ 0,
/* �(250,0xfa) */ 0,
/* �(251,0xfb) */ 0,
/* �(252,0xfc) */ 0,
/* �(253,0xfd) */ 0,
/* �(254,0xfe) */ 0,
/* �(255,0xff) */ 0
};

/* ASCII */
static const char *leo_typeface_class[]={
/* 000 */ "",
/* 001 */ "",
/* 002 */ "",
/* 003 */ "",
/* 004 */ "",
/* 005 */ "",
/* 006 */ "",
/* 007 */ "",
/* 008 */ "",
/* 009 */ "",
/* 010 */ "",
/* 011 */ "",
/* 012 */ "",
/* 013 */ "",
/* 014 */ "",
/* 015 */ "",
/* 016 */ "",
/* 017 */ "",
/* 018 */ "",
/* 019 */ "",
/* 020 */ "",
/* 021 */ "",
/* 022 */ "",
/* 023 */ "",
/* 024 */ "",
/* 025 */ "",
/* 026 */ "",
/* 027 */ "",
/* 028 */ "",
/* 029 */ "",
/* 030 */ "",
/* 031 */ "",
/*  (032,0x20) */ " ",
/* !(033,0x21) */ "!",
/* "(034,0x22) */ "\"",
/* #(035,0x23) */ "#",
/* $(036,0x24) */ "$",
/* %(037,0x25) */ "%",
/* &(038,0x26) */ "&",
/* '(039,0x27) */ "'",
/* ((040,0x28) */ "(",
/* )(041,0x29) */ ")",
/* *(042,0x2a) */ "*",
/* +(043,0x2b) */ "+",
/* ,(044,0x2c) */ ",",
/* -(045,0x2d) */ "-",
/* .(046,0x2e) */ ".",
/* /(047,0x2f) */ "/",
/* 0(048,0x30) */ "0oO��",
/* 1(049,0x31) */ "1lI",
/* 2(050,0x32) */ "2Zz",
/* 3(051,0x33) */ "3��",
/* 4(052,0x34) */ "4��",
/* 5(053,0x35) */ "5Ss",
/* 6(054,0x36) */ "6",
/* 7(055,0x37) */ "7",
/* 8(056,0x38) */ "8",
/* 9(057,0x39) */ "9",
/* :(058,0x3a) */ ":",
/* ;(059,0x3b) */ ";",
/* <(060,0x3c) */ "<",
/* =(061,0x3d) */ "=",
/* >(062,0x3e) */ ">",
/* ?(063,0x3f) */ "?",
/* @(064,0x40) */ "@",
/* A(065,0x41) */ "A�",
/* B(066,0x42) */ "B�",
/* C(067,0x43) */ "C��c",
/* D(068,0x44) */ "D",
/* E(069,0x45) */ "E�",
/* F(070,0x46) */ "F",
/* G(071,0x47) */ "G",
/* H(072,0x48) */ "H��",
/* I(073,0x49) */ "Il1",
/* J(074,0x4a) */ "J",
/* K(075,0x4b) */ "K��",
/* L(076,0x4c) */ "L",
/* M(077,0x4d) */ "M��",
/* N(078,0x4e) */ "N",
/* O(079,0x4f) */ "Oo0��",
/* P(080,0x50) */ "Pp��",
/* Q(081,0x51) */ "Q",
/* R(082,0x52) */ "R",
/* S(083,0x53) */ "Ss5",
/* T(084,0x54) */ "T��",
/* U(085,0x55) */ "Uu",
/* V(086,0x56) */ "Vv",
/* W(087,0x57) */ "Ww",
/* X(088,0x58) */ "Xx��",
/* Y(089,0x59) */ "Yy�",
/* Z(090,0x5a) */ "Zz2",
/* [(091,0x5b) */ "[",
/* \(092,0x5c) */ "\\",
/* ](093,0x5d) */ "]",
/* ^(094,0x5e) */ "^",
/* _(095,0x5f) */ "_",
/* `(096,0x60) */ "`",
/* a(097,0x61) */ "a�",
/* b(098,0x62) */ "b��",
/* c(099,0x63) */ "cC�",
/* d(100,0x64) */ "d",
/* e(101,0x65) */ "e�",
/* f(102,0x66) */ "f",
/* g(103,0x67) */ "g9",
/* h(104,0x68) */ "h",
/* i(105,0x69) */ "i",
/* j(106,0x6a) */ "j",
/* k(107,0x6b) */ "k",
/* l(108,0x6c) */ "Il1",
/* m(109,0x6d) */ "m",
/* n(110,0x6e) */ "n",
/* o(111,0x6f) */ "0oO��",
/* p(112,0x70) */ "pP��",
/* q(113,0x71) */ "q",
/* r(114,0x72) */ "r",
/* s(115,0x73) */ "sS5",
/* t(116,0x74) */ "t",
/* u(117,0x75) */ "uU",
/* v(118,0x76) */ "vV",
/* w(119,0x77) */ "wW",
/* x(120,0x78) */ "Xx��",
/* y(121,0x79) */ "Yy�",
/* z(122,0x7a) */ "zZ2",
/* {(123,0x7b) */ "{",
/* |(124,0x7c) */ "|",
/* }(125,0x7d) */ "}",
/* ~(126,0x7e) */ "~",
/* (127,0x7f) */ "",
/* �(128,0x80) */ "�A",
/* �(129,0x81) */ "�",
/* �(130,0x82) */ "��B",
/* �(131,0x83) */ "��",
/* �(132,0x84) */ "��",
/* �(133,0x85) */ "�E",
/* �(134,0x86) */ "��",
/* �(135,0x87) */ "3��",
/* �(136,0x88) */ "��",
/* �(137,0x89) */ "��",
/* �(138,0x8a) */ "�K�",
/* �(139,0x8b) */ "��",
/* �(140,0x8c) */ "�M�",
/* �(141,0x8d) */ "�H�",
/* �(142,0x8e) */ "0oO��",
/* �(143,0x8f) */ "��",
/* �(144,0x90) */ "pP��",
/* �(145,0x91) */ "cC�",
/* �(146,0x92) */ "�T�",
/* �(147,0x93) */ "Yy�",
/* �(148,0x94) */ "��",
/* �(149,0x95) */ "Xx��",
/* �(150,0x96) */ "��",
/* �(151,0x97) */ "��4",
/* �(152,0x98) */ "��",
/* �(153,0x99) */ "��",
/* �(154,0x9a) */ "��",
/* �(155,0x9b) */ "��",
/* �(156,0x9c) */ "��b",
/* �(157,0x9d) */ "��",
/* �(158,0x9e) */ "��",
/* �(159,0x9f) */ "��",
/* �(160,0xa0) */ "�a",
/* �(161,0xa1) */ "�",
/* �(162,0xa2) */ "��B",
/* �(163,0xa3) */ "��",
/* �(164,0xa4) */ "��",
/* �(165,0xa5) */ "e�",
/* �(166,0xa6) */ "��",
/* �(167,0xa7) */ "��3",
/* �(168,0xa8) */ "��",
/* �(169,0xa9) */ "��",
/* �(170,0xaa) */ "��K",
/* �(171,0xab) */ "��",
/* �(172,0xac) */ "��M",
/* �(173,0xad) */ "��H",
/* �(174,0xae) */ "0oO��",
/* �(175,0xaf) */ "��",
/* �(176,0xb0) */ "�",
/* �(177,0xb1) */ "�",
/* �(178,0xb2) */ "�",
/* �(179,0xb3) */ "�",
/* �(180,0xb4) */ "�",
/* �(181,0xb5) */ "�",
/* �(182,0xb6) */ "�",
/* �(183,0xb7) */ "�",
/* �(184,0xb8) */ "�",
/* �(185,0xb9) */ "�",
/* �(186,0xba) */ "�",
/* �(187,0xbb) */ "�",
/* �(188,0xbc) */ "�",
/* �(189,0xbd) */ "�",
/* �(190,0xbe) */ "�",
/* �(191,0xbf) */ "�",
/* �(192,0xc0) */ "�",
/* �(193,0xc1) */ "�",
/* �(194,0xc2) */ "�",
/* �(195,0xc3) */ "�",
/* �(196,0xc4) */ "�",
/* �(197,0xc5) */ "�",
/* �(198,0xc6) */ "�",
/* �(199,0xc7) */ "�",
/* �(200,0xc8) */ "�",
/* �(201,0xc9) */ "�",
/* �(202,0xca) */ "�",
/* �(203,0xcb) */ "�",
/* �(204,0xcc) */ "�",
/* �(205,0xcd) */ "�",
/* �(206,0xce) */ "�",
/* �(207,0xcf) */ "�",
/* �(208,0xd0) */ "�",
/* �(209,0xd1) */ "�",
/* �(210,0xd2) */ "�",
/* �(211,0xd3) */ "�",
/* �(212,0xd4) */ "�",
/* �(213,0xd5) */ "�",
/* �(214,0xd6) */ "�",
/* �(215,0xd7) */ "�",
/* �(216,0xd8) */ "�",
/* �(217,0xd9) */ "�",
/* �(218,0xda) */ "�",
/* �(219,0xdb) */ "�",
/* �(220,0xdc) */ "�",
/* �(221,0xdd) */ "�",
/* �(222,0xde) */ "�",
/* �(223,0xdf) */ "�",
/* �(224,0xe0) */ "pP��",
/* �(225,0xe1) */ "cC�",
/* �(226,0xe2) */ "�T�",
/* �(227,0xe3) */ "Yy�",
/* �(228,0xe4) */ "�",
/* �(229,0xe5) */ "Xx��",
/* �(230,0xe6) */ "�",
/* �(231,0xe7) */ "��4",
/* �(232,0xe8) */ "�",
/* �(233,0xe9) */ "�",
/* �(234,0xea) */ "�",
/* �(235,0xeb) */ "�",
/* �(236,0xec) */ "��b",
/* �(237,0xed) */ "�",
/* �(238,0xee) */ "�",
/* �(239,0xef) */ "�",
/* �(240,0xf0) */ "�",
/* �(241,0xf1) */ "�",
/* �(242,0xf2) */ "�",
/* �(243,0xf3) */ "�",
/* �(244,0xf4) */ "�",
/* �(245,0xf5) */ "�",
/* �(246,0xf6) */ "�",
/* �(247,0xf7) */ "�",
/* �(248,0xf8) */ "�",
/* �(249,0xf9) */ "�",
/* �(250,0xfa) */ "�",
/* �(251,0xfb) */ "�",
/* �(252,0xfc) */ "�",
/* �(253,0xfd) */ "�",
/* �(254,0xfe) */ "�",
/* �(255,0xff) */ "�"
};

/* ASCII */
static const char *leo_typeface_one[]={
/* 000 */ "",
/* 001 */ "",
/* 002 */ "",
/* 003 */ "",
/* 004 */ "",
/* 005 */ "",
/* 006 */ "",
/* 007 */ "",
/* 008 */ "",
/* 009 */ "",
/* 010 */ "",
/* 011 */ "",
/* 012 */ "",
/* 013 */ "",
/* 014 */ "",
/* 015 */ "",
/* 016 */ "",
/* 017 */ "",
/* 018 */ "",
/* 019 */ "",
/* 020 */ "",
/* 021 */ "",
/* 022 */ "",
/* 023 */ "",
/* 024 */ "",
/* 025 */ "",
/* 026 */ "",
/* 027 */ "",
/* 028 */ "",
/* 029 */ "",
/* 030 */ "",
/* 031 */ "",
/*  (032,0x20) */ " ",
/* !(033,0x21) */ "!",
/* "(034,0x22) */ "\"",
/* #(035,0x23) */ "#",
/* $(036,0x24) */ "$",
/* %(037,0x25) */ "%",
/* &(038,0x26) */ "&",
/* '(039,0x27) */ "'",
/* ((040,0x28) */ "(",
/* )(041,0x29) */ ")",
/* *(042,0x2a) */ "*",
/* +(043,0x2b) */ "+",
/* ,(044,0x2c) */ ",",
/* -(045,0x2d) */ "-",
/* .(046,0x2e) */ ".",
/* /(047,0x2f) */ "/",
/* 0(048,0x30) */ "�",
/* 1(049,0x31) */ "1",
/* 2(050,0x32) */ "2",
/* 3(051,0x33) */ "�",
/* 4(052,0x34) */ "4",
/* 5(053,0x35) */ "5",
/* 6(054,0x36) */ "6",
/* 7(055,0x37) */ "7",
/* 8(056,0x38) */ "8",
/* 9(057,0x39) */ "9",
/* :(058,0x3a) */ ":",
/* ;(059,0x3b) */ ";",
/* <(060,0x3c) */ "<",
/* =(061,0x3d) */ "=",
/* >(062,0x3e) */ ">",
/* ?(063,0x3f) */ "?",
/* @(064,0x40) */ "@",
/* A(065,0x41) */ "�",
/* B(066,0x42) */ "�",
/* C(067,0x43) */ "�",
/* D(068,0x44) */ "D",
/* E(069,0x45) */ "�",
/* F(070,0x46) */ "F",
/* G(071,0x47) */ "G",
/* H(072,0x48) */ "�",
/* I(073,0x49) */ "I",
/* J(074,0x4a) */ "J",
/* K(075,0x4b) */ "�",
/* L(076,0x4c) */ "L",
/* M(077,0x4d) */ "�",
/* N(078,0x4e) */ "N",
/* O(079,0x4f) */ "�",
/* P(080,0x50) */ "�",
/* Q(081,0x51) */ "Q",
/* R(082,0x52) */ "R",
/* S(083,0x53) */ "S",
/* T(084,0x54) */ "�",
/* U(085,0x55) */ "U",
/* V(086,0x56) */ "V",
/* W(087,0x57) */ "W",
/* X(088,0x58) */ "�",
/* Y(089,0x59) */ "�",
/* Z(090,0x5a) */ "Z",
/* [(091,0x5b) */ "[",
/* \(092,0x5c) */ "\\",
/* ](093,0x5d) */ "]",
/* ^(094,0x5e) */ "^",
/* _(095,0x5f) */ "_",
/* `(096,0x60) */ "`",
/* a(097,0x61) */ "�",
/* b(098,0x62) */ "�",
/* c(099,0x63) */ "�",
/* d(100,0x64) */ "d",
/* e(101,0x65) */ "�",
/* f(102,0x66) */ "f",
/* g(103,0x67) */ "g",
/* h(104,0x68) */ "h",
/* i(105,0x69) */ "i",
/* j(106,0x6a) */ "j",
/* k(107,0x6b) */ "k",
/* l(108,0x6c) */ "l",
/* m(109,0x6d) */ "m",
/* n(110,0x6e) */ "n",
/* o(111,0x6f) */ "�",
/* p(112,0x70) */ "�",
/* q(113,0x71) */ "q",
/* r(114,0x72) */ "r",
/* s(115,0x73) */ "S",
/* t(116,0x74) */ "t",
/* u(117,0x75) */ "U",
/* v(118,0x76) */ "V",
/* w(119,0x77) */ "W",
/* x(120,0x78) */ "�",
/* y(121,0x79) */ "�",
/* z(122,0x7a) */ "Z",
/* {(123,0x7b) */ "{",
/* |(124,0x7c) */ "|",
/* }(125,0x7d) */ "}",
/* ~(126,0x7e) */ "~",
/* (127,0x7f) */ "",
/* �(128,0x80) */ "�",
/* �(129,0x81) */ "�",
/* �(130,0x82) */ "�",
/* �(131,0x83) */ "�",
/* �(132,0x84) */ "�",
/* �(133,0x85) */ "�",
/* �(134,0x86) */ "�",
/* �(135,0x87) */ "3",
/* �(136,0x88) */ "�",
/* �(137,0x89) */ "�",
/* �(138,0x8a) */ "�",
/* �(139,0x8b) */ "�",
/* �(140,0x8c) */ "�",
/* �(141,0x8d) */ "�",
/* �(142,0x8e) */ "�",
/* �(143,0x8f) */ "�",
/* �(144,0x90) */ "�",
/* �(145,0x91) */ "�",
/* �(146,0x92) */ "�",
/* �(147,0x93) */ "�",
/* �(148,0x94) */ "�",
/* �(149,0x95) */ "�",
/* �(150,0x96) */ "�",
/* �(151,0x97) */ "�",
/* �(152,0x98) */ "�",
/* �(153,0x99) */ "�",
/* �(154,0x9a) */ "�",
/* �(155,0x9b) */ "�",
/* �(156,0x9c) */ "�",
/* �(157,0x9d) */ "�",
/* �(158,0x9e) */ "�",
/* �(159,0x9f) */ "�",
/* �(160,0xa0) */ "�",
/* �(161,0xa1) */ "�",
/* �(162,0xa2) */ "�",
/* �(163,0xa3) */ "�",
/* �(164,0xa4) */ "�",
/* �(165,0xa5) */ "�",
/* �(166,0xa6) */ "�",
/* �(167,0xa7) */ "3",
/* �(168,0xa8) */ "�",
/* �(169,0xa9) */ "�",
/* �(170,0xaa) */ "�",
/* �(171,0xab) */ "�",
/* �(172,0xac) */ "�",
/* �(173,0xad) */ "�",
/* �(174,0xae) */ "�",
/* �(175,0xaf) */ "�",
/* �(176,0xb0) */ "�",
/* �(177,0xb1) */ "�",
/* �(178,0xb2) */ "�",
/* �(179,0xb3) */ "�",
/* �(180,0xb4) */ "�",
/* �(181,0xb5) */ "�",
/* �(182,0xb6) */ "�",
/* �(183,0xb7) */ "�",
/* �(184,0xb8) */ "�",
/* �(185,0xb9) */ "�",
/* �(186,0xba) */ "�",
/* �(187,0xbb) */ "�",
/* �(188,0xbc) */ "�",
/* �(189,0xbd) */ "�",
/* �(190,0xbe) */ "�",
/* �(191,0xbf) */ "�",
/* �(192,0xc0) */ "�",
/* �(193,0xc1) */ "�",
/* �(194,0xc2) */ "�",
/* �(195,0xc3) */ "�",
/* �(196,0xc4) */ "�",
/* �(197,0xc5) */ "�",
/* �(198,0xc6) */ "�",
/* �(199,0xc7) */ "�",
/* �(200,0xc8) */ "�",
/* �(201,0xc9) */ "�",
/* �(202,0xca) */ "�",
/* �(203,0xcb) */ "�",
/* �(204,0xcc) */ "�",
/* �(205,0xcd) */ "�",
/* �(206,0xce) */ "�",
/* �(207,0xcf) */ "�",
/* �(208,0xd0) */ "�",
/* �(209,0xd1) */ "�",
/* �(210,0xd2) */ "�",
/* �(211,0xd3) */ "�",
/* �(212,0xd4) */ "�",
/* �(213,0xd5) */ "�",
/* �(214,0xd6) */ "�",
/* �(215,0xd7) */ "�",
/* �(216,0xd8) */ "�",
/* �(217,0xd9) */ "�",
/* �(218,0xda) */ "�",
/* �(219,0xdb) */ "�",
/* �(220,0xdc) */ "�",
/* �(221,0xdd) */ "�",
/* �(222,0xde) */ "�",
/* �(223,0xdf) */ "�",
/* �(224,0xe0) */ "�",
/* �(225,0xe1) */ "�",
/* �(226,0xe2) */ "�",
/* �(227,0xe3) */ "�",
/* �(228,0xe4) */ "�",
/* �(229,0xe5) */ "�",
/* �(230,0xe6) */ "�",
/* �(231,0xe7) */ "�",
/* �(232,0xe8) */ "�",
/* �(233,0xe9) */ "�",
/* �(234,0xea) */ "�",
/* �(235,0xeb) */ "�",
/* �(236,0xec) */ "�",
/* �(237,0xed) */ "�",
/* �(238,0xee) */ "�",
/* �(239,0xef) */ "�",
/* �(240,0xf0) */ "�",
/* �(241,0xf1) */ "�",
/* �(242,0xf2) */ "�",
/* �(243,0xf3) */ "�",
/* �(244,0xf4) */ "�",
/* �(245,0xf5) */ "�",
/* �(246,0xf6) */ "�",
/* �(247,0xf7) */ "�",
/* �(248,0xf8) */ "�",
/* �(249,0xf9) */ "�",
/* �(250,0xfa) */ "�",
/* �(251,0xfb) */ "�",
/* �(252,0xfc) */ "�",
/* �(253,0xfd) */ "�",
/* �(254,0xfe) */ "�",
/* �(255,0xff) */ "�"
};

static unsigned const char std_ASCII_to_ANSI[256]=
{
/*       0    1    2    3    4    5    6    7   8    9    a    b    c    d    e    f  */
/* 0*/ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
/* 1*/ 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
/* 2*/ 0x20,0x21,0x22,0xB9,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
/* 3*/ 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
/* 4*/ 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
/* 5*/ 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0xB9,0x5f,
/* 6*/ 0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
/* 7*/ 0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
/* 8*/ 0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
/* 9*/ 0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
/* a*/ 0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
/* b*/ 0xe4,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
/* c*/ 0xB8,0x20,0xbf,0x20,0x20,0xaf,0xb2,0xa5,0xA8,0xba,0x20,0x20,0xaa,0x20,0x20,0x20,
/* d*/ 0x99,0x20,0x20,0x20,0x20,0x22,0xb4,0xab,0xbb,0x20,0xb3,0x20,0x20,0xa9,0xae,0x20,
/* e*/ 0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
/* f*/ 0xe4,0xe4,0x20,0x20,0x20,0xf2,0x20,0xe8,0xe3,0x20,0x20,0x20,0x20,0xe0,0x95,0x20
};

static const char std_ANSI_to_ASCII[257]={
/*0*/    "0000000000000000"     // �� ���� �� ����ࠦ�����
/*1*/    "0000000000000000"     //
/*2*/    " !\"#$%&'()*+,-./"
/*3*/	 "0123456789:;<=>?"
/*4*/	 "@ABCDEFGHIJKLMNO"
/*5*/	 "PQRSTUVWXYZ[\\]^-"
/*6*/	 "`abcdefghijklmno"
/*7*/    "pqrstuvwxyz{|}~\x20"
/*8*/    "\xD3\x20\x20\x20\x20\x20\x20\x20\x20\x20\xCD\x20\xCB\x20\xD2\xCA"
/*9*/    "\xD4\x20\x20\x20\x20\xFE\x20\x20\x20\x54\xC4\x20\xC2\x20\xD1\xC1"
/*A*/    "\x20\x20\x20\x4A\x20\xF2\x20\x20\xF0\x43\xF4\x3C\x20\xAF\x52\xF8"
/*B*/    "\xb0\x20\xF6\xF7\xF3\x20\x20\x20\xF1\x23\xF5\x3E\x6A\x20\x20\xF9"
/*C*/    "����������������"
/*D*/    "����������������"
/*E*/    "����������������"
/*F*/    "����������������"
};
/*
static wchar_t std_ANSI_to_UNICODE[257]=
    L"00000000000000000000000000000000 "
    L"!\"#$%&\'()*+,-./0123456789:;<=>?"
    L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
    L"`abcdefghijklmnopqrstuvwxyz{|}~"
    L"��������������������������������"
    L"��������������������������������"
    L"��������������������������������"
    L"��������������������������������";
*/
static wchar_t std_ANSI_to_UNICODE[257];

STD_FUNC(Word8) stdAsciiToAnsi( Word8 Code)
{
if( !Code )
    return 0;
return    std_ASCII_to_ANSI[Code];
}

STD_FUNC(Word8) stdAnsiToAscii( Word8 Code)
{
if( !Code )
    return 0;
return    std_ANSI_to_ASCII[Code];
}

STD_FUNC(void) stdStrAsciiToAnsi( char* Str)
{
	char* pStr=Str;
	while(*pStr)
	{	*pStr=std_ASCII_to_ANSI[(Word8)*pStr];
		pStr++;
	}
}

STD_FUNC(void) stdStrAnsiToAscii( char* Str)
{
	char* pStr=Str;
	while(*pStr)
	{	*pStr=std_ANSI_to_ASCII[(Word8)*pStr];
		pStr++;
	}
}

STD_FUNC(void) stdStrNAsciiToAnsi( char* Str,Int32 nLen)
{
	if(nLen<=0)
		return;
	for(int nSymb=0;nSymb<nLen;nSymb++)
		Str[nSymb]=std_ASCII_to_ANSI[(Word8)Str[nSymb]];
}

STD_FUNC(void) stdStrNAnsiToAscii( char* Str,Int32 nLen)
{
	if(nLen<=0)
		return;
	for(int nSymb=0;nSymb<nLen;nSymb++)
		Str[nSymb]=std_ANSI_to_ASCII[(Word8)Str[nSymb]];
}

STD_FUNC(Word16) stdAnsiToUnicode( Word8 Code)
{
    return    std_ANSI_to_UNICODE[Code];
}

STD_FUNC(void) stdStrAnsiToUnicode(wchar_t* pUnicode,const char* pAnsi)
{
	const char* pCurAnsi=pAnsi; wchar_t* pCurUnic=pUnicode;
	while(*pCurAnsi)
		*pCurUnic++=std_ANSI_to_UNICODE[(Word8)*pCurAnsi++];
    *pCurUnic=0;
}

STD_FUNC(void) stdStrNAnsiToUnicode(wchar_t* pUnicode,const char* pAnsi,Int32 nLen)
{
	const char* pCurAnsi=pAnsi; wchar_t* pCurUnic=pUnicode;
	while(*pCurAnsi && pCurAnsi-pAnsi<=nLen)
		*pCurUnic++=std_ANSI_to_UNICODE[(Word8)*pCurAnsi++];
    *pCurUnic=0;
}

/*#include "winnls.h"*/

STD_FUNC(void) stdStrUnicodeToAnsi(char* pAnsi,const wchar_t* pUnicode)
{
    WideCharToMultiByte(CP_ACP,NULL,(const char*)pUnicode,lstrlenW(pUnicode),pAnsi,lstrlenW(pUnicode),NULL,NULL);
}

STD_FUNC(Bool32) stdLeoCompareChar(Word8 c1, Word8 c2)
{
const char *p=leo_typeface_class[c1];
Word8   c;
while( *p )
    {
    c=(Word8)*p++;
    if( c==c2 )
        return TRUE;
    }
return FALSE;
}

STD_FUNC(Word8 *) stdLeoGetTypeface(Word8 c)
{
return (Word8 *)leo_typeface_class[c];
}

STD_FUNC(Word8) stdLeoTypefaceChar(Word8 c)
{
return leo_typeface_one[c][0];
}


STD_FUNC( Int32 )   stdLeoIsCase(Word8 Code)
{
return leo_case[ Code ];
}

STD_FUNC( Word8)   stdLeoSetUpperCase(Word8 Code)
{
return (Word8)(leo_case_upper[ Code ]);
}

STD_FUNC( Word8)   stdLeoSetLowerCase(Word8 Code)
{
return (Word8)(leo_case_lower[ Code ]);
}

STD_FUNC(Word8) stdUpperAscii( Word8 c )
{
  if ( c >= (Word8)'a' && c <= (Word8)'z')
    c = c - (Word8)'a' + (Word8)'A';

  if ( c >= (Word8)'�' && c <= (Word8)'�')
    c = c - (Word8)'�' + (Word8)'�';
  if ( c >= (Word8)'�' && c <= (Word8)'�')
    c = c - (Word8)'�' + (Word8)'�';
return c;
}

 /* Function returns LOWER CASE variant of the letter.             */
STD_FUNC(Word8) stdLowerAscii(Word8 c)
{
  if ( c >= (Word8)'A' && c <= (Word8)'Z')
    c = c - (Word8)'A'+ (Word8)'a' ;
  if ( c >= (Word8)'�' && c <= (Word8)'�')
    c = c - (Word8)'�'+ (Word8)'�' ;
  if ( c >= (Word8)'�' && c <= (Word8)'�')
    c = c - (Word8)'�'+ (Word8)'�' ;
return c;
}
