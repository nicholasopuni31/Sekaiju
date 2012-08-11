//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �v���p�e�B�m�[�g�_�C�A���O�N���X
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

#ifndef _PROPERTYNOTEDLG_H_
#define _PROPERTYNOTEDLG_H_

class CPropertyNoteDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	BOOL m_bTrackZeroOrigin;                 // �g���b�N�ԍ���0���琔���邩
	BOOL m_bNoteOnNoteOn0;                   // �m�[�g�I��+�m�[�g�I��(�x���V�e�B0)��
	CStringArray m_theTrackNameArray;        // �g���b�N���z��
	CDWordArray m_theTrackOutputChannelArray;// �o�̓`�����l���z��
	CStringArray m_theKeyNameArray;          // �L�[���z��
	int m_nTrackIndex;                       // �g���b�N
	CString m_strTime;                       // �^�C��
	int m_nChannel;                          // �`�����l��
	int m_nKey;                              // �L�[
	int m_nOnVelocity;                       // �Ō��x���V�e�B
	int m_nOffVelocity;                      // �����x���V�e�B
	int m_nDuration;                         // ������

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CPropertyNoteDlg();                      // �R���X�g���N�^
	enum {IDD = IDD_PROPERTYNOTE};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	BOOL SetChannelRange ();                 // �`�����l���̉ϔ͈͐ݒ�
	BOOL SetOnVelocityRange ();              // �Ō��x���V�e�B�̉ϔ͈͐ݒ�
	BOOL SetOffVelocityRange ();             // �����x���V�e�B�̉ϔ͈͐ݒ�
	BOOL SetDurationRange ();                // �������̉ϔ͈͐ݒ�
	BOOL FillTrackIndexCombo ();             // �g���b�N�C���f�b�N�X�R���{�{�b�N�X�[��
	BOOL FillKeyCombo ();                    // �L�[�C���f�b�N�X�R���{�{�b�N�X�[��

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual BOOL OnInitDialog ();            // �_�C�A���O�̏�����

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg void CPropertyNoteDlg::OnTrackIndexSelEndOK ();
	DECLARE_MESSAGE_MAP ()
};

#endif