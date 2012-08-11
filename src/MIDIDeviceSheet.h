//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// MIDI�f�o�C�X�v���p�e�B�V�[�g�N���X
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

#ifndef _MIDIDEVICESHEET_H_
#define _MIDIDEVICESHEET_H_

#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"

class CMIDIDeviceSheet : public CPropertySheet {
	//--------------------------------------------------------------------------
	// �e�v���p�e�B�y�[�W
	//--------------------------------------------------------------------------
public:
	CMIDIInDevicePage m_theMIDIInDevicePage;        // MIDI���̓f�o�C�X�y�[�W
	CMIDIOutDevicePage m_theMIDIOutDevicePage;      // MIDI�o�̓f�o�C�X�y�[�W
	CMIDIInstDefNormPage m_theMIDIInstDefNormPage;  // MIDI�C���X�g�D�������g(�ʏ�)�y�[�W
	CMIDIInstDefDrumPage m_theMIDIInstDefDrumPage;  // MIDI�C���X�g�D�������g(�h����)�y�[�W
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMIDIDeviceSheet (CWnd* pParentWnd);    // �R���X�g���N�^
	virtual ~CMIDIDeviceSheet ();           // �f�X�g���N�^
	
	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnApplyNow ();
	DECLARE_MESSAGE_MAP ()
};

#endif
