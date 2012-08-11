//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// MIDI�o�̓f�o�C�X�y�[�W�N���X
// (C)2002-2012 ���[�Ղ�MIDI�Ղ낶�����Ɓ^����
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


#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"
#include "MIDIDeviceSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE (CMIDIOutDevicePage, CPropertyPage)

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CMIDIOutDevicePage::CMIDIOutDevicePage () : 
CPropertyPage (IDD_MIDIOUTDEVICE) {
}

// �f�X�g���N�^
CMIDIOutDevicePage::~CMIDIOutDevicePage () {
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------
void CMIDIOutDevicePage::DoDataExchange (CDataExchange* pDX) {
	CString strMIDIMapper;
	VERIFY (strMIDIMapper.LoadString (IDS_MIDI_MAPPER));
	long i;
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		// 20100126:�󕶎���Ή�
		// �R���g���[���������o�ϐ�
		if (pDX->m_bSaveAndValidate) {
			if (((CComboBox*)GetDlgItem (IDC_MIDIOUTDEVICE_01 + i))->GetCurSel () == 0) {
				m_strMIDIOutName[i] = _T("");
			}
			else if (((CComboBox*)GetDlgItem (IDC_MIDIOUTDEVICE_01 + i))->GetCurSel () == 1) {
				m_strMIDIOutName[i] = MIDIIO_MIDIMAPPER;
			}
			else {
				DDX_CBString (pDX, IDC_MIDIOUTDEVICE_01 + i, m_strMIDIOutName[i]);
			}
		}
		// �����o�ϐ����R���g���[��
		else {
			if (m_strMIDIOutName[i] == _T("")) {
				((CComboBox*)GetDlgItem (IDC_MIDIOUTDEVICE_01 + i))->SetCurSel (0);
			}
			else if (m_strMIDIOutName[i] == MIDIIO_MIDIMAPPER ||
				m_strMIDIOutName[i] == MIDIIO_MIDIMAPPERJ ||
				m_strMIDIOutName[i] == strMIDIMapper) {
				((CComboBox*)GetDlgItem (IDC_MIDIOUTDEVICE_01 + i))->SetCurSel (1);
			}
			else {
				DDX_CBString (pDX, IDC_MIDIOUTDEVICE_01 + i, m_strMIDIOutName[i]);
			}
		}
	}
}

// �_�C�A���O�̏�����
BOOL CMIDIOutDevicePage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lNum = MIDIOut_GetDeviceNum ();
	/* 20081219 MIDIOut_GetDeviceName����ϒx���̂Ń��[�v�����ύX */
	long lPort, j;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIOUTDEVICE_01 + lPort);
		CString strNone;
		VERIFY (strNone.LoadString (IDS_NONE));
		pComboBox->AddString (strNone);
		//CString strMIDIMapper;
		//VERIFY (strMIDIMapper.LoadString (IDS_MIDI_MAPPER));
		//pComboBox->AddString (strMIDIMapper);
		pComboBox->AddString (_T(MIDIIO_MIDIMAPPER));
	}
	for (j = 0; j < lNum; j++) {
		TCHAR szName[256];
		memset (szName, 0, sizeof (szName));
		MIDIOut_GetDeviceName (j, szName, 255);
		for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
			CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIOUTDEVICE_01 + lPort);
			pComboBox->AddString (szName);
		}
	}
	CDialog::OnInitDialog(); // AddString�͊�{�֐��̌Ăяo�����O(20090625)
	return TRUE;
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMIDIOutDevicePage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIOUTDEVICE_01, IDC_MIDIOUTDEVICE_16, OnSelChange)
END_MESSAGE_MAP ()

void CMIDIOutDevicePage::OnSelChange (UINT nID) {
	SetModified (TRUE);
}
