//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// MIDI�������[�h�v���p�e�B�V�[�g�N���X
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

#ifndef _MIDISYNCMODESHEET_H_
#define _MIDISYNCMODESHEET_H_

#include "MIDIInSyncModePage.h"
#include "MIDIOutSyncModePage.h"

class CMIDISyncModeSheet : public CPropertySheet {

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMIDISyncModeSheet (CWnd* pParentWnd);  // �R���X�g���N�^
	virtual ~CMIDISyncModeSheet ();         // �f�X�g���N�^
	
	//--------------------------------------------------------------------------
	// �e�y�[�W
	//--------------------------------------------------------------------------
public:
	CMIDIInSyncModePage m_theMIDIInSyncModePage;    // MIDI���͓������[�h�y�[�W
	CMIDIOutSyncModePage m_theMIDIOutSyncModePage;  // MIDI�o�͓������[�h�y�[�W
	
	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnApplyNow ();
	DECLARE_MESSAGE_MAP ()
};

#endif
