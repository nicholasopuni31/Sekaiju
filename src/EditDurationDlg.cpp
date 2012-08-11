//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �������̕ύX�_�C�A���O�N���X
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
#include "EditDurationDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CEditDurationDlg::CEditDurationDlg () : CDialog (CEditDurationDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

BOOL CEditDurationDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITDURATION_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITDURATION_RELATIVEUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITDURATION_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pAbsoluteUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITDURATION_AMOUNTSP))->SetRange (1, 16383);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITDURATION_AMOUNTSP))->SetPos (CLIP (1, lValue, 16383));
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITDURATION_AMOUNTSP))->SetRange (-16383, 16383);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITDURATION_AMOUNTSP))->SetPos (CLIP (-16383, lValue, 16383));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITDURATION_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITDURATION_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	return TRUE;
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �f�[�^�G�N�X�`�F���W
void CEditDurationDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITDURATION_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITDURATION_ABSOLUTEUNIT, m_nUnit);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITDURATION_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITDURATION_RELATIVEUNIT);
	if (pAbsoluteUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 16383);
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -16383, 16383);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
}

// �_�C�A���O��������
BOOL CEditDurationDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditDurationDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITDURATION_ABSOLUTEUNIT, IDC_EDITDURATION_PERCENTUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// �P�ʂ��ύX���ꂽ
void CEditDurationDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


