//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���߂̑}���_�C�A���O�N���X
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
#include "EditInsertMeasureDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CEditInsertMeasureDlg::CEditInsertMeasureDlg () : CDialog (CEditInsertMeasureDlg::IDD) {
	m_nPosition = 0;
	m_nNumMeasure = 0;
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

BOOL CEditInsertMeasureDlg::SetAmountRange () {
	CString strValue;
	GetDlgItem (IDC_EDITINSERTMEASURE_POSITION)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (m_bZeroOrigin) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetRange (0, 32767);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetPos (CLIP (0, lValue, 32767));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetRange (1, 32767);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_POSITIONSP))->SetPos (CLIP (1, lValue, 32767));
	}
	GetDlgItem (IDC_EDITINSERTMEASURE_NUMMEASURE)->GetWindowText (strValue);
	lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_NUMMEASURESP))->SetRange (1, 256);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITINSERTMEASURE_NUMMEASURESP))->SetPos (CLIP (1, lValue, 256));
	return TRUE;
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �f�[�^�G�N�X�`�F���W
void CEditInsertMeasureDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITINSERTMEASURE_POSITION, m_nPosition);
	if (m_bZeroOrigin) {
		DDV_MinMaxInt (pDX, m_nPosition, 0, 32767);
	}
	else {
		DDV_MinMaxInt (pDX, m_nPosition, 1, 32767);
	}
	DDX_Text (pDX, IDC_EDITINSERTMEASURE_NUMMEASURE, m_nNumMeasure);
	DDV_MinMaxInt (pDX, m_nNumMeasure, 1, 256);
}

// �_�C�A���O��������
BOOL CEditInsertMeasureDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditInsertMeasureDlg, CDialog)
END_MESSAGE_MAP ()



