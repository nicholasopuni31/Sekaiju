//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �����̕ύX�_�C�A���O�N���X
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

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditKeyDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// �R���X�g���N�^
CEditKeyDlg::CEditKeyDlg () : CDialog (CEditKeyDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
	m_nTargetNote = 0;
	m_nTargetKeyAfter = 0;
}

// �I�y���[�V����
BOOL CEditKeyDlg::SetAmountRange () {
	CButton* pHalfUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_HALFUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITKEY_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pHalfUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetRange (-127, 127);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetPos (CLIP (-127, lValue, 127));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetRange (-10, 10);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetPos (CLIP (-10, lValue, 10));
	}
	return TRUE;
}


// �I�[�o�[���C�h

// �f�[�^�G�N�X�`�F���W
void CEditKeyDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITKEY_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITKEY_HALFUNIT, m_nUnit);
	DDX_Check (pDX, IDC_EDITKEY_TARGETNOTE, m_nTargetNote);
	DDX_Check (pDX, IDC_EDITKEY_TARGETKEYAFTER, m_nTargetKeyAfter);
	CButton* pHalfUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_HALFUNIT);
	if (pHalfUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -127, 127);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, -10, 10);
	}
}


BOOL CEditKeyDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CEditKeyDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITKEY_HALFUNIT, IDC_EDITKEY_OCTAVEUNIT, OnChangeUnit)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditKeyDlg ���b�Z�[�W �n���h��

// �P�ʂ��ύX���ꂽ
void CEditKeyDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


