//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���t�@�C���_�C�A���O�N���X
// (C)2002-2010 ���[�Ղ�MIDI�Ղ낶�����Ɓ^����
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

#ifndef _SEKAIJUFILEDLG_H_
#define _SEKAIJUFILEDLG_H_

class CSekaijuFileDlg : public CFileDialog {

	//DECLARE_DYNAMIC (CSekaijuFileDlg)
	
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
	// �R���X�g���N�^
public:
	CSekaijuFileDlg (BOOL bOpenFileDialog); 
	CSekaijuFileDlg (BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
		LPCTSTR lpszFilter, CWnd* pParentWnd);

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	CString m_strDefExt; // �f�t�H���g�̊g���q(�t�@�C���^�C�v��؂�ւ��邽�тɍX�V)

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
public:
	//virtual int DoModal ();
	virtual void OnTypeChange (); // �t�@�C���^�C�v���ύX���ꂽ
};

#endif
