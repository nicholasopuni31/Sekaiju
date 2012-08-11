//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �N�H���^�C�Y�_�C�A���O�N���X
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

#ifndef _EDITQUANTIZEDLG_H_
#define _EDITQUANTIZEDLG_H_

class CEditQuantizeDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	int m_lTimeMode;                    // �^�C�����[�h(0=TPQN�x�[�X, ...)
	int m_lTimeResolution;              // ����\[tick/4������]����[�T�u�t���[��/1�t���[��]
	int m_nSnapTimeIndex;               // �X�i�b�v�^�C���C���f�b�N�X(0=4������,1=8������...)
	int m_nStrength;                    // ���x[%]
	int m_nTargetNoteOn;                // �m�[�g�I����ΏۂƂ���(0/1)
	int m_nTargetNoteOff;               // �m�[�g�I�t��ΏۂƂ���(0/1)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEditQuantizeDlg();                 // �R���X�g���N�^
	enum {IDD = IDD_EDITQUANTIZE};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	BOOL SetAmountRange ();             // ���x�̗L���͈͂�ݒ肷��
	BOOL FillSnapTimeCombo ();          // �X�i�b�v�^�C���R���{�{�b�N�X���[��������


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
	DECLARE_MESSAGE_MAP ()
};

#endif
