//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���p���[�`��
// (C)2002-2012 ���[�Ղ�MIDI�Ղ낶�����Ɓ^����
//******************************************************************************

/* This library is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU Lesser General Public */
/* License as published by the Free Software Foundation; either */
/* version 2.1 of the License, or (at your option) any later version. */

/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/* Lesser General Public License for more details. */

/* You should have received a copy of the GNU Lesser General Public */
/* License along with this library; if not, write to the Free Software */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include "winver.h"
#include "common.h"

// TSIZEOF�}�N�� //20120211�ǉ�
#ifndef TSIZEOF
#define TSIZEOF(STR) (sizeof(STR)/sizeof(TCHAR))
#endif
#ifndef TCSLEN
#ifdef UNICODE
#define TCSLEN(STRING) wcslen(STRING)
#else
#define TCSLEN(STRING) strlen(STRING)
#endif
#endif
#ifndef TCSNCPY
#ifdef UNICODE
#define TCSNCPY(STRING1,STRING2,N) wcsncpy(STRING1,STRING2,N)
#else
#define TCSNCPY(STRING1,STRING2,N) strncpy(STRING1,STRING2,N)
#endif
#endif

// �����Ƃ��ĔF���ł��镶���񂩒��ׂ�(�߂�l:�F���ł��������̕�����)
long IsNumeric (const TCHAR* pszText) {
	long i;
	TCHAR c = 0;
	long iStart = 0;
	long lLen = TCSLEN (pszText);
	if (lLen == 0) {
		return 0;
	}
	c = *(pszText + 0);
	if (c == _T('-')) {
		iStart = 1;
	}
	for (i = iStart; i < lLen; i++) {
		c = *(pszText + i);
		if (!_istdigit (c)) {
			return i - iStart;
		}
	}
	return i - iStart;
}


/* 16�i�@�Ŏg�p�\�ȕ����Ȃ�0�ȊO���A�����łȂ����0��Ԃ� */
long IsHexDigit (TCHAR c) {
	return _istxdigit ((TCHAR)c);
}

/* �o�C�i���z���16�i�e�L�X�g������(���p�X�y�[�X��؂�)�ɕϊ����� */
long bin2txt (BYTE* pBin, long lBinLen, TCHAR* pszText, long lTextLen) {
	long i, iMax;
	TCHAR szText[16];
	memset (pszText, 0, lTextLen * sizeof (TCHAR));
	iMax = __min ((lTextLen - 1) / 3, lBinLen);
	for (i = 0; i < iMax; i++) {
		_sntprintf (szText, TSIZEOF (szText) - 1, _T("%02X "), *(pBin + i));
		_tcscpy (pszText + 3 * i, szText);
	}
	if (i > 0) {
		*(pszText + 3 * i - 1) = _T('\0');
	}
	return i;
}

/* 16�i�e�L�X�g������(���p�X�y�[�X��؂�)���o�C�i���z��ɕϊ����� */
long txt2bin (TCHAR* pszText, long lTextLen, BYTE* pBin, long lBinLen) {
	long i, iMax;
	TCHAR* p = pszText;
	iMax = lBinLen;
	memset (pBin, 0, lBinLen);
	for (i = 0; i < iMax; i++) {
		/* �擪�X�y�[�X��΂� */
		while (1) {
			if (p - pszText >= lTextLen) {
				return i;
			}
			else if (_tcsncmp (p, _T(" "), 1) == 0 || _tcsncmp (p, _T("\t"), 1) == 0) {
				p++;
			}
			else if (*p == _T('\0')) {
				return i;
			}
			else {
				break;
			}
		}
		/* �Y��2�����ǂݍ��� */
		if (!IsHexDigit (*p) || !IsHexDigit (*(p+1)) ||	IsHexDigit (*(p+2))) {
			memset (pBin, 0, lBinLen);
			return 0;
		}
		_stscanf (p, _T("%x"), pBin + i);
		p += 2;
	}
	return i;
}

/* '\t''\r''\n''\\'�R�[�h����̕�������R�[�h�����̕�����(C����X�^�C��)�ɕϊ�����@*/
/* �߂�l�F�o���オ����������̃o�C�g�� */
long codestr2str (TCHAR* pszCodeStr, long lCodeStrLen, TCHAR* pszStr, long lStrLen) {
	TCHAR* p1 = pszCodeStr;
	TCHAR* p2 = pszStr;
	while (p1 - pszCodeStr < lCodeStrLen && p2 - pszStr < lStrLen) {
#ifndef UNICODE
		if (p1 - pszCodeStr < lCodeStrLen - 1 && p2 - pszStr < lStrLen - 1 &&
			(0x80 <= (unsigned char)*p1 && (unsigned char)*p1 <= 0x9F || 
			0xE0 <= (unsigned char)*p1 && (unsigned char)*p1 <= 0xFC)) {
			*p2++ = *p1++;
			*p2++ = *p1++;
		}
		else 
#endif		
		if (p2 - pszStr < lStrLen - 1 && *p1 == _T('\t')) {
			*p2++ = _T('\\');
			*p2++ = _T('t');
			p1++;
		}
		else if (p2 - pszStr < lStrLen - 1 && *p1 == _T('\r')) {
			*p2++ = _T('\\');
			*p2++ = _T('r');
			p1++;
		}
		else if (p2 - pszStr < lStrLen - 1 && *p1 == _T('\n')) {
			*p2++ = _T('\\');
			*p2++ = _T('n');
			p1++;
		}
		else if (*p1 == _T('\\')) {
			*p2++ = _T('\\');
			*p2++ = _T('\\');
			p1++;
		}
		else {
			*p2++ = *p1++;
		}
	}
	return (p2 - pszStr);
}

/* �R�[�h�����̕�����(C����X�^�C��)��'\t''\r''\n''\\'�R�[�h����̕�����ɕϊ����� */
/* �߂�l�F�o���オ����������̃o�C�g�� */
long str2codestr (TCHAR* pszStr, long lStrLen, TCHAR* pszCodeStr, long lCodeStrLen) {
	TCHAR* p1 = pszStr;
	TCHAR* p2 = pszCodeStr;
	while (p1 - pszStr < lStrLen && p2 - pszCodeStr < lCodeStrLen) {
#ifndef UNICODE
		if (p1 - pszStr < lStrLen - 1 && p2 - pszCodeStr < lCodeStrLen - 1 &&
			(0x80 <= (unsigned char)*p1 && (unsigned char)*p1 <= 0x9F || 
			0xE0 <= (unsigned char)*p1 && (unsigned char)*p1 <= 0xFC)) {
			*p2++ = *p1++;
			*p2++ = *p1++;
		}
		else 
#endif			
		if (p1 - pszStr < lStrLen - 1 && *p1 == _T('\\') && *(p1 + 1) == _T('t')) {
			*p2++ = _T('\t');
			p1++;
			p1++;
		}
		else if (p1 - pszStr < lStrLen - 1 && *p1 == _T('\\') && *(p1 + 1) == _T('r')) {
			*p2++ = _T('\r');
			p1++;
			p1++;
		}
		else if (p1 - pszStr < lStrLen - 1 && *p1 == _T('\\') && *(p1 + 1) == _T('n')) {
			*p2++ = _T('\n');
			p1++;
			p1++;
		}
		else if (p1 - pszStr < lStrLen + 1 && *p1 == _T('\\') && *(p1 + 1) == _T('\\')) {
			*p2++ = _T('\\');
			p1++;
			p1++;
		}
		else {
			*p2++ = *p1++;
		}
	}
	return (pszCodeStr - p2);
}
