//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�I�v�V����2�y�[�W�N���X
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

#ifndef _TRACKLISTOPTION2PAGE_H_
#define _TRACKLISTOPTION2PAGE_H_

class CTrackListOption2Page : public CPropertyPage {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lDefCC000Width;              // �f�t�H���g��CC#0�̕�[pixel]
	long m_lDefCC032Width;              // �f�t�H���g��CC#32�̕�[pixel]
	long m_lDefPCWidth;                 // �f�t�H���g�̃v���O�����i���o�[�̕�[pixel]
	long m_lDefCC007Width;              // �f�t�H���g�̃{�����[���̕�[pixel]
	long m_lDefCC010Width;              // �f�t�H���g�̃p���̕�[pixel]
	long m_lDefCC091Width;              // �f�t�H���g�̃��o�[�u�̕�[pixel]
	long m_lDefCC093Width;              // �f�t�H���g�̃R�[���X�̕�[pixel]
	long m_lDefCC094Width;              // �f�t�H���g�̃f�B���C�̕�[pixel]
	long m_lDefKeyShiftWidth;           // �f�t�H���g�̃L�[�V�t�g�̕�[pixel]
	long m_lDefVelShiftWidth;           // �f�t�H���g�̃x���V�e�B�V�t�g�̕�[pixel]
	long m_lDefTimeShiftWidth;          // �f�t�H���g�̃^�C���V�t�g�̕�[pixel]
	long m_lDefNumEventWidth;           // �f�t�H���g�̃C�x���g���̕�[pixel]
	// �����ۂ̗�̕�[pixel]�́A�e��̕��ɗ�����g��{�����������l�ƂȂ�B
	
public:
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
	CTrackListOption2Page ();           // �R���X�g���N�^
	enum {IDD = IDD_TRACKLISTOPTION2};

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
