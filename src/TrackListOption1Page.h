//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�I�v�V����1�y�[�W�N���X
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
//

#ifndef _TRACKLISTOPTION1PAGE_H_
#define _TRACKLISTOPTION1PAGE_H_

class CTrackListOption1Page : public CPropertyPage {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lDefRowZoom;                 // �f�t�H���g�̗�����g��{��
	long m_lDefColumnZoom;              // �f�t�H���g�̍s�����g��{��
	long m_lDefTimeZoom;                // �f�t�H���g�̃^�C�������g��{��
	long m_lDefNameWidth;               // �f�t�H���g�̖��O�̕�[pixel]
	long m_lDefColorWidth;              // �f�t�H���g�̐F�̕�[pixel]
	long m_lDefInputOnWidth;            // �f�t�H���g�̓���ON�̕�[pixel]
	long m_lDefInputPortWidth;          // �f�t�H���g�̓��̓|�[�g�̕�[pixel]
	long m_lDefInputChWidth;            // �f�t�H���g�̓���CH�̕�[pixel]
	long m_lDefOutputOnWidth;           // �f�t�H���g�̏o��ON�̕�[pixel]
	long m_lDefOutputPortWidth;         // �f�t�H���g�̏o�̓|�[�g�̕�[pixel]
	long m_lDefOutputChWidth;           // �f�t�H���g�̏o��CH�̕�[pixel]
	long m_lDefViewModeWidth;           // �f�t�H���g�̃r���[���[�h�̕�[pixel]
	// �����ۂ̗�̕�[pixel]�́A�e��̕��ɗ�����g��{�����������l�ƂȂ�B
	
public:
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
	CTrackListOption1Page ();           // �R���X�g���N�^
	enum {IDD = IDD_TRACKLISTOPTION1};

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
