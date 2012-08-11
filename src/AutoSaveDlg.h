//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �����ۑ��_�C�A���O�N���X
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

#ifndef _AUTOSAVEDLG_H_
#define _AUTOSAVEDLG_H_

class CAutoSaveDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	CString m_strExeFilePath;           // ���s�t�@�C���̃p�X��
	int m_nOn;                          // �����ۑ��I��
	int m_lInterval;                    // �����ۑ��Ԋu(1-120)[��]
	int m_nDisableWhilePlaying;         // ���t���͎����ۑ����Ȃ�
	int m_nDisableWhileRecording;       // ���A���^�C�����͒��͎����ۑ����Ȃ�

public:
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
	// �R���X�g���N�^
	CAutoSaveDlg ();
	enum {IDD = IDD_AUTOSAVE};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	BOOL SetIntervalRange ();
	BOOL UpdateInterval ();
	BOOL UpdateDisableWhilePlaying ();
	BOOL UpdateDisableWhileRecording ();

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
	afx_msg void OnChangeOn ();
	afx_msg void OnDeleteAllFileNow ();
};

#endif
