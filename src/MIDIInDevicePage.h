//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// MIDI���̓f�o�C�X�y�[�W�N���X
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

#ifndef _MIDIINDEVICEPAGE_H_
#define _MIDIINDEVICEPAGE_H_

class CMIDIInDevicePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIInDevicePage)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
	CString m_strMIDIInName[MAXMIDIINDEVICENUM];    // MIDIIn�f�o�C�X��[0�`15]

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMIDIInDevicePage ();               // �R���X�g���N�^
	virtual ~CMIDIInDevicePage ();      // �f�X�g���N�^

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
