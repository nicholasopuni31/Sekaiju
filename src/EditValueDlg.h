//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �l�̕ύX�_�C�A���O�N���X
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

#ifndef _EDITVALUEDLG_H_
#define _EDITVALUEDLG_H_

class CEditValueDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	int m_nAmount;                      // �l�̕ύX��
	int m_nUnit;                        // �P��(0=����,1=�p�[�Z���g)
	int m_nTargetKeyAfter;              // �L�[�A�t�^�[�^�b�`��ΏۂƂ���(0/1)
	int m_nTargetControlChange;         // �R���g���[���`�F���W��ΏۂƂ���(0/1)
	int m_nTargetChannelAfter;          // �`�����l���A�t�^�[�^�b�`��ΏۂƂ���(0/1)
	int m_nTargetPitchBend;             // �s�b�`�x���h��ΏۂƂ���(0/1)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEditValueDlg ();                   // �R���X�g���N�^
	enum {IDD = IDD_EDITVALUE};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	BOOL SetAmountRange ();             // �l�̕ύX�ʂ̗L���͈͂�ݒ肷��

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual BOOL OnInitDialog ();       // �_�C�A���O�̏�����

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnChangeUnit (UINT nID);
	afx_msg void OnChangeTarget (UINT nID);
	DECLARE_MESSAGE_MAP ()
};

#endif
