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

#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// �����Ƃ��ĔF���ł��镶���񂩒��ׂ�(�߂�l:�F���ł��������̕�����)
long IsNumeric (const TCHAR* pszText);

/* 16�i�@�Ŏg�p�\�ȕ����Ȃ�1���A�����łȂ����0��Ԃ� */
long IsHexDigit (TCHAR c);

/* �o�C�i���z���16�i�e�L�X�g������(���p�X�y�[�X��؂�)�ɕϊ����� */
long bin2txt (BYTE* pBin, long lBinLen, TCHAR* pszText, long lTextLen);

/* 16�i�e�L�X�g������(���p�X�y�[�X��؂�)���o�C�i���z��ɕϊ����� */
long txt2bin (TCHAR* pszText, long lTextLen, BYTE* pBin, long lBinLen);

/* '\t''\r''\n''\\'�R�[�h����̕�������R�[�h�����̕�����(C����X�^�C��)�ɕϊ�����@*/
/* �߂�l�F�o���オ����������̃o�C�g�� */
long codestr2str (TCHAR* pszCodeStr, long lCodeStrLen, TCHAR* pszStr, long lStrLen);

/* �R�[�h�����̕�����(C����X�^�C��)��'\t''\r''\n''\\'�R�[�h����̕�����ɕϊ����� */
/* �߂�l�F�o���オ����������̃o�C�g�� */
long str2codestr (TCHAR* pszStr, long lStrLen, TCHAR* pszCodeStr, long lCodeStrLen);


#ifdef __cplusplus
}
#endif

#endif
