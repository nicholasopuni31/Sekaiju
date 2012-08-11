//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���g���m�[���_�C�A���O�N���X
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

#ifndef _METRONOMEDLG_H_
#define _METRONOMEDLG_H_

class CMetronomeDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------------
public:
	int m_nOn;                          // �I��(0=�I�t�A1=ON)
	int m_nOutputPort;                  // �o�̓|�[�g(0�`15)
	int m_nOutputChannel;               // �o�̓`�����l��(0�`15)
	int m_nNoteKey1;                    // �m�[�g�L�[����(0�`127)
	int m_nNoteVel1;                    // �m�[�g�x���V�e�B����(1�`127)
	int m_nNoteKey2;                    // �m�[�g�L�[���(0�`127)
	int m_nNoteVel2;                    // �m�[�ƃx���V�e�B���(0�`127)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMetronomeDlg();                    // �R���X�g���N�^
	enum {IDD = IDD_METRONOME};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	BOOL FillOutputPortCombo ();
	BOOL FillNoteKeyCombo ();

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
	afx_msg void OnChangeOutput ();
	DECLARE_MESSAGE_MAP ()
};

#endif
