//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �x���V�e�B�̕ύX�_�C�A���O�N���X
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

#ifndef _EDITVELOCITYDLG_H_
#define _EDITVELOCITYDLG_H_

class CEditVelocityDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	int m_nAmount;                      // �x���V�e�B�̕ύX��
	int m_nUnit;                        // �P��(0=����,1=�p�[�Z���g)
	int m_nTargetNoteOn;                // �m�[�g�I����Ώۂɂ���(0/1)
	int m_nTargetNoteOff;               // �m�[�g�I�t��Ώۂɂ���(0/1)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEditVelocityDlg();                 // �R���X�g���N�^
	enum {IDD = IDD_EDITVELOCITY};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	BOOL SetAmountRange ();

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual BOOL OnInitDialog ();       // �_�C�A���O������ 

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnChangeUnit (UINT nID);
	DECLARE_MESSAGE_MAP ()
};

#endif
