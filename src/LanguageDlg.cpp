//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ����_�C�A���O�N���X
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
#include <afxcmn.h>
#include <afxext.h>

#include "resource.h"
#include "LanguageDlg.h"

#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CLanguageDlg::CLanguageDlg () : CDialog (CLanguageDlg::IDD) {
	m_strLanguage = _T("");
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// ����R���{�{�b�N�X�̏[��
BOOL CLanguageDlg::FillLanguageCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_LANGUAGE_COMBO);
	if (pComboBox == NULL) {
		return FALSE;
	}
	pComboBox->ResetContent ();
	TCHAR* pszLanguage[] = {_T("Japanese"), _T("English")};
	long i = 0;
	for (i = 0; i < sizeof (pszLanguage) / sizeof (pszLanguage[0]); i++) {
		pComboBox->AddString (pszLanguage[i]);
		if (pSekaijuApp->m_strLanguage.Compare (pszLanguage[i]) == 0) {
			pComboBox->SetCurSel (i);
		}
	}
	return TRUE;
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �f�[�^�G�N�X�`�F���W
void CLanguageDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);

	DDX_CBString (pDX, IDC_LANGUAGE_COMBO, m_strLanguage);
}

// �_�C�A���O�̏�����
BOOL CLanguageDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	FillLanguageCombo ();
	return bRet;
}



//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CLanguageDlg, CDialog)
END_MESSAGE_MAP ()




