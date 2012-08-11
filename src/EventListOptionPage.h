//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�I�v�V�����y�[�W�N���X
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

#ifndef _EVENTLISTOPTION_H_
#define _EVENTLISTOPTION_H_

class CEventListOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lDefRowZoom;                 // �f�t�H���g�̍s�����g�嗦[�{]
	long m_lDefColumnZoom;              // �f�t�H���g�̗�����g�嗦[�{]
	long m_lDefTrackWidth;              // �f�t�H���g�̃g���b�N��[pixel]
	long m_lDefMillisecWidth;           // �f�t�H���g�̎�:��:�b:�~���b��[pixel]
	long m_lDefTimeWidth;               // �f�t�H���g�̃^�C����[pixel]
	long m_lDefKindWidth;               // �f�t�H���g�̃C�x���g�̎�ޕ�[pixel]
	long m_lDefChWidth;                 // �f�t�H���g�̃`�����l����[pixel]
	long m_lDefVal1Width;               // �f�t�H���g�̒l1��[pixel]
	long m_lDefVal2Width;               // �f�t�H���g�̒l2��[pixel]
	long m_lDefVal3Width;               // �f�t�H���g�̒l3��[pixel]
	BOOL m_bInsertEventAfter;           // �������̃C�x���g�̒���ɑ}������
	BOOL m_bDuplicateEventAfter;        // ���݂̃C�x���g�̒���ɑ}������
	BOOL m_bDeleteEventAfter;           // �폜�����C�x���g�̎��̃C�x���g�Ƀt�H�[�J�X�����킹��

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	// �R���X�g���N�^
	CEventListOptionPage ();
	enum {IDD = IDD_EVENTLISTOPTION};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------

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
	//afx_msg void OnChangeEnableAutoSave ();
	DECLARE_MESSAGE_MAP ()
};

#endif
