//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �l�̕ύX�_�C�A���O�N���X
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
#include "EditValueDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CEditValueDlg::CEditValueDlg () : CDialog (CEditValueDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
	m_nTargetKeyAfter = 0;
	m_nTargetControlChange = 0;
	m_nTargetChannelAfter = 0;
	m_nTargetPitchBend = 0;

}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

BOOL CEditValueDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_RELATIVEUNIT);
	CButton* pTargetPitchBendButton = (CButton*)GetDlgItem (IDC_EDITVALUE_TARGETPITCHBEND);
	CString strValue;
	GetDlgItem (IDC_EDITVALUE_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pAbsoluteUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (-8192, 8191);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (-8192, lValue, 8191));
		}
		else {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (0, 127);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (0, lValue, 127));
		}
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (-16384, 16383);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (-16384, lValue, 16383));
		}
		else {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (-127, 127);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (-127, lValue, 127));
		}
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	return TRUE;
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �f�[�^�G�N�X�`�F���W
void CEditValueDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITVALUE_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITVALUE_ABSOLUTEUNIT, m_nUnit);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETKEYAFTER, m_nTargetKeyAfter);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETCONTROLCHANGE, m_nTargetControlChange);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETCHANNELAFTER, m_nTargetChannelAfter);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETPITCHBEND, m_nTargetPitchBend);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_RELATIVEUNIT);
	CButton* pTargetPitchBendButton = (CButton*)GetDlgItem (IDC_EDITVALUE_TARGETPITCHBEND);
	if (pAbsoluteUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			DDV_MinMaxInt (pDX, m_nAmount, -8192, 8191);
		}
		else {
			DDV_MinMaxInt (pDX, m_nAmount, 0, 127);
		}
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			DDV_MinMaxInt (pDX, m_nAmount, -16384, 16383);
		}
		else {
			DDV_MinMaxInt (pDX, m_nAmount, -127, 127);
		}
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
}

// �_�C�A���O��������
BOOL CEditValueDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditValueDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITVALUE_ABSOLUTEUNIT, IDC_EDITVALUE_PERCENTUNIT, OnChangeUnit)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITVALUE_TARGETKEYAFTER, IDC_EDITVALUE_TARGETPITCHBEND, OnChangeTarget)
END_MESSAGE_MAP ()

// �P�ʂ��ύX���ꂽ
void CEditValueDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}

// �Ώۂ��ύX���ꂽ
void CEditValueDlg::OnChangeTarget (UINT nID) {
	SetAmountRange ();
}
