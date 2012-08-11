//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �t�@�C���v���p�e�B�_�C�A���O�N���X
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

#ifndef _FILEPROPERTYDLG_H_
#define _FILEPROPERTYDLG_H_

class CFilePropertyDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	CString m_strTitle;                 // �^�C�g��(=�ŏ��̃g���b�N�̍ŏ��̃g���b�N��)
	CString m_strSubTitle;              // �T�u�^�C�g��(=�ŏ��̃g���b�N��2�Ԗڂ̃g���b�N��)
	CString m_strCopyright;             // ���쌠(=�ŏ��̃g���b�N�̍ŏ��̒��쌠)
	CString m_strComment1;              // �R�����g1(=�ŏ��̃g���b�N�̍ŏ��̃e�L�X�g)
	CString m_strComment2;              // �R�����g2(=�ŏ��̃g���b�N��2�Ԗڂ̃e�L�X�g)
	CString m_strComment3;              // �R�����g3(=�ŏ��̃g���b�N��3�Ԗڂ̃e�L�X�g)
	CString m_strNumTrack;              // �g���b�N��
	CString m_strNumEvent;              // �C�x���g��
	CString m_strEndMillisec;           // �I���~���b
	CString m_strEndTime;               // �I���^�C��
	int m_nSMFFormat;                   // SMF�t�H�[�}�b�g0/1/2
	int m_nTimeMode;                    // �^�C�����[�h(0=TPQN�x�[�X,...)
	int m_nResolution;                  // ����\[tick/4������]����[�T�u�t���[��/1�t���[��]

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CFilePropertyDlg ();                // �R���X�g���N�^
	enum {IDD = IDD_FILEPROPERTY};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	void FillResolutionComboAndStatic ();

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
	afx_msg void OnChangeTimeMode (UINT nID);
	DECLARE_MESSAGE_MAP ()
};

#endif
