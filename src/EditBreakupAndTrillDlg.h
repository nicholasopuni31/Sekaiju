//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �ו����ƃg�����_�C�A���O�N���X
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

#ifndef _EDITBREAKUPANDTRILLDLG_H_
#define _EDITBREAKUPANDTRILLDLG_H_

class CEditBreakupAndTrillDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lTimeMode;                   // �^�C�����[�h
	long m_lTimeResolution;             // ����\[tick/4������]����[�T�u�t���[��/1�t���[��]
	int m_nDurationIndex;               // �ו������1���̒����C���f�b�N�X(0=4�������`)
	int m_nEnableTrill;                 // �g������L���ɂ��邩�H
	int m_nKeyShift;                    // �g�������̃L�[�V�t�g

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEditBreakupAndTrillDlg();          // �R���X�g���N�^
	enum {IDD = IDD_EDITBREAKUPANDTRILL};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	BOOL FillDurationCombo ();          // �������R���{�{�b�N�X�̏[��
	BOOL SetKeyShiftRange ();           // �g�������̃L�[�V�t�g�̗L���͈͐ݒ�
	BOOL UpdateKeyShift ();             // �g�������̃L�[�V�t�g�G�f�B�b�g�{�b�N�X�X�V

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();       // �_�C�A���O�̏�����

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
	afx_msg void OnChangeEnableTrill ();
};

#endif
