//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �J���t���ȃR���{�{�b�N�X�{�b�N�X�N���X
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
#include "ColorfulComboBox.h"

// TSIZEOF�}�N�� //20120211�ǉ�
#ifndef TSIZEOF
#define TSIZEOF(STR) (sizeof(STR)/sizeof(TCHAR))
#endif
#ifndef TCSLEN
#ifdef UNICODE
#define TCSLEN(STRING) wcslen(STRING)
#else
#define TCSLEN(STRING) strlen(STRING)
#endif
#endif
#ifndef TCSNCPY
#ifdef UNICODE
#define TCSNCPY(STRING1,STRING2,N) wcsncpy(STRING1,STRING2,N)
#else
#define TCSNCPY(STRING1,STRING2,N) strncpy(STRING1,STRING2,N)
#endif
#endif

// ���b�Z�[�W�}�b�v
IMPLEMENT_DYNCREATE (CColorfulComboBox, CComboBox)

BEGIN_MESSAGE_MAP (CColorfulComboBox, CComboBox)
	//ON_WM_CREATE ()
	ON_WM_DESTROY ()
END_MESSAGE_MAP ()

typedef struct {
	TCHAR m_szText[1024];
	COLORREF m_lForeColor;
	COLORREF m_lBackColor;
} ItemData;

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CColorfulComboBox::CColorfulComboBox () {
	CComboBox::CComboBox ();
}

// �f�X�g���N�^
CColorfulComboBox::~CColorfulComboBox () {
	CComboBox::~CComboBox ();
}


//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �E�B���h�E�X�^�C���ύX
BOOL CColorfulComboBox::PreCreateWindow (CREATESTRUCT& cs) {
	if (!CComboBox::PreCreateWindow (cs)) {
		return FALSE;
	}
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~(CBS_OWNERDRAWVARIABLE | CBS_SORT);
	cs.style |= CBS_OWNERDRAWFIXED;
	return TRUE;
}

// �A�C�e���̍����w��
void CColorfulComboBox::MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
	lpMeasureItemStruct->itemHeight = 14;
}

// �A�C�e���̕`��
void CColorfulComboBox::DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct) {
	int nIndex = lpDrawItemStruct->itemID;
	CRect rcItem = lpDrawItemStruct->rcItem;

	CDC dc;
	dc.Attach (lpDrawItemStruct->hDC);	

	long lBackColor = ::GetSysColor (COLOR_WINDOW);
	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		lBackColor = ::GetSysColor (COLOR_HIGHLIGHT);
	}
	if (nIndex != (UINT)-1) {
		ItemData* pItemData = (ItemData*)GetItemDataPtr (nIndex);
		if (lpDrawItemStruct->itemState & ODS_SELECTED) {
			lBackColor = pItemData->m_lForeColor;
		}
		else {
			lBackColor = pItemData->m_lBackColor;
		}
	}

	CBrush* pBrush = new CBrush;
	pBrush->CreateSolidBrush (lBackColor);
	dc.FillRect (rcItem, pBrush);
	delete pBrush;

	if (lpDrawItemStruct->itemState & ODS_FOCUS) {
		dc.DrawFocusRect (rcItem);
	}

	if (nIndex != (UINT)-1) {
		ItemData* pItemData = (ItemData*)GetItemDataPtr (nIndex);
		CFont* pFont = CFont::FromHandle 
			((HFONT)(::GetStockObject (DEFAULT_GUI_FONT)));
		CFont* pOldFont = dc.SelectObject (pFont);
		long lTextColor = (lpDrawItemStruct->itemState & ODS_SELECTED) ?
			::GetSysColor (COLOR_HIGHLIGHTTEXT) : pItemData->m_lForeColor;
		dc.SetBkMode (TRANSPARENT);
		dc.SetTextColor (lTextColor);
		dc.DrawText (pItemData->m_szText, rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject (pOldFont);
	}
	dc.Detach ();


}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �A�C�e���̒ǉ�
int CColorfulComboBox::AddItem (LPCTSTR lpszText, COLORREF lForeColor, COLORREF lBackColor) {
	int nIndex = AddString (_T(""));
	if (nIndex == CB_ERR || nIndex == CB_ERRSPACE) {
		return -1;
	}
	ItemData* pItemData = (ItemData*)calloc (sizeof (ItemData), 1);
	if (pItemData == NULL) {
		return -1;
	}
	TCSNCPY (pItemData->m_szText, lpszText, 1023);
	pItemData->m_lForeColor = lForeColor;
	pItemData->m_lBackColor = lBackColor;
	int ret = SetItemDataPtr (nIndex, pItemData);
	if (ret < 0) {
		free (pItemData);
		return -1;
	}
	return ret;

}

// �A�C�e���̑S�폜
void CColorfulComboBox::RemoveAllItem () {
	long i;
	long lItemCount = this->GetCount ();
	for (i = 0; i < lItemCount; i++) {
		ItemData* pItemData = (ItemData*)GetItemDataPtr (i);
		free (pItemData);
		pItemData = NULL;
	}
	ResetContent ();
}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E�j��
void CColorfulComboBox::OnDestroy () {
	RemoveAllItem ();
	CComboBox::OnDestroy ();
}


