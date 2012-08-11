//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C���v���[�X�G�f�B�b�g�N���X
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
#include <afxext.h>

#include "InplaceEdit.h"


// �A���P�[�V�����̊Ď�
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���b�Z�[�W�}�b�v
IMPLEMENT_DYNCREATE (CInplaceEdit, CEdit)

BEGIN_MESSAGE_MAP (CInplaceEdit, CEdit)
	ON_WM_KILLFOCUS ()
	ON_WM_KEYDOWN ()
	ON_WM_CHAR ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CInplaceEdit::CInplaceEdit () {
}

// �f�X�g���N�^
CInplaceEdit::~CInplaceEdit () {
}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �t�H�[�J�X������ꂽ�Ƃ�
void CInplaceEdit::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CInplaceEditBox::OnKillFocus (pNewWnd=0x%08x)\n", pNewWnd);
	CWnd* pParentWnd = GetParent ();
	if (pParentWnd != pNewWnd) {
		pParentWnd->SendMessage (WM_KILLFOCUS, (WPARAM)pNewWnd, (LPARAM)0L);
	}
	else {
		//pParentWnd->SetFocus ();
	}
	CEdit::OnKillFocus (pNewWnd); //20080809�ǉ�(�J���b�g�̏����h�~)
}

// �L�[����������
void CInplaceEdit::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CWnd* pParentWnd = GetParent ();
	switch (nChar) {
	case VK_RETURN:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		break;
	case VK_ESCAPE:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		break;
	}
	CEdit::OnKeyDown (nChar, nRepCnt, nFlags);
}

// �������͎�
void CInplaceEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) {
	// Enter���������Ƃ��Ƀv�[�v�[������̂�h��
	switch (nChar) {
	case VK_RETURN:
		return;
	}
	CEdit::OnChar (nChar, nRepCnt, nFlags);
}
