//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// MIDI�o�͓������[�h�y�[�W�N���X
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

#ifndef _MIDIOUTSYNCMODEPAGE_H_
#define _MIDIOUTSYNCMODEPAGE_H_


class CMIDIOutSyncModePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIOutSyncModePage)

public:
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
	CMIDIOutSyncModePage ();            // �R���X�g���N�^
	virtual ~CMIDIOutSyncModePage ();   // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------------
public:
	int m_nMIDIOutSyncMode[MAXMIDIOUTDEVICENUM];

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
	afx_msg void OnSelChange (UINT nID);
	DECLARE_MESSAGE_MAP ()

};

#endif
