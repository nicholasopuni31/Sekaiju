//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ����_�C�A���O�N���X
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

#ifndef _LANGUAGEDLG_H_
#define _LANGUAGEDLG_H_

class CLanguageDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------------
public:
	CString m_strLanguage;              // ���ꖼ

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CLanguageDlg();                    // �R���X�g���N�^
	enum {IDD = IDD_LANGUAGE};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	BOOL FillLanguageCombo ();

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
};

#endif
