//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�v���p�e�B�X�P�[���r���[�N���X
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
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "EventListFrame.h"
#include "EventListPropertyScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CEventListPropertyScaleView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CEventListPropertyScaleView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CEventListPropertyScaleView::CEventListPropertyScaleView () {
}

// �f�X�g���N�^
CEventListPropertyScaleView::~CEventListPropertyScaleView () {
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���_�̈ړ�
void CEventListPropertyScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	pDC->SetWindowOrg (pEventListFrame->GetColumnScrollPos (), 0);
}

// �`��
void CEventListPropertyScaleView::OnDraw (CDC* pDC) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);
	// �w�i�̓h��Ԃ�
	pDC->FillSolidRect (&rcClient, lColorBtnFace);
	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (pEventListFrame->GetParentFont ());
	CRect theRect (0, 0, 0, 0);
	long lColumnZoom = pEventListFrame->GetColumnZoom ();
	for (long j = 0; j < 8; j++) {
		theRect.top = rcClient.top;
		theRect.bottom = rcClient.bottom;
		theRect.left = theRect.right; 
		theRect.right = theRect.right + pEventListFrame->GetColumnBaseWidth (j) * lColumnZoom;
		pDC->Draw3dRect (&theRect, lColorBtnHighlight, lColorBtnShadow);
		pDC->DrawText (pEventListFrame->GetColumnTitle (j), &theRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
BOOL CEventListPropertyScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// �}�E�X���{�^�������ꂽ��  
void CEventListPropertyScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	point += CSize (pEventListFrame->GetColumnScrollPos (), 0);
	// �J�[�\�������E��ɂ��邩���ׂ�
	long j = 0;
	long lBorderX = 0;
	for (j = 0; j < 8; j++) {
		lBorderX += pEventListFrame->GetColumnBaseWidth (j) * pEventListFrame->GetColumnZoom ();
		if (lBorderX - 2 <= point.x && point.x <= lBorderX + 2) {
			break;
		}
	}
	// �J�[�\�������E��ɂ������ꍇ
	if (0 <= j && j < 8) {
		SetCapture ();
		m_lTempColumnIndex = j;
		m_lTempColumnBaseWidth = pEventListFrame->GetColumnBaseWidth (j);
		m_ptMouseDown = m_ptMouseMoveOld = point;
		::SetCursor (pSekaijuApp->m_hCursorSizeWE);
	}
}

// �}�E�X�E�{�^�������ꂽ��  
void CEventListPropertyScaleView::OnRButtonDown (UINT nFlags, CPoint point) {

}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CEventListPropertyScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (GetCapture () == this) {
		ReleaseCapture ();
		::SetCursor (pSekaijuApp->m_hCursorArrow);
		pEventListFrame->RecalcColumnScrollInfo ();
		pEventListFrame->m_pPropertyScaleView->Invalidate ();
		pEventListFrame->m_pIndexPropertyView->Invalidate ();
	}
}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CEventListPropertyScaleView::OnRButtonUp (UINT nFlags, CPoint point) {

}

// �}�E�X���������ꂽ�Ƃ�
void CEventListPropertyScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	point += CSize (pEventListFrame->GetColumnScrollPos (), 0);
	// �L���v�^�[��
	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		CSize szMouseDelta = point.x - m_ptMouseDown.x;
		long lNewColumnBaseWidth = 
			(m_lTempColumnBaseWidth * pEventListFrame->GetColumnZoom () + szMouseDelta.cx) / 
			pEventListFrame->GetColumnZoom ();
		lNewColumnBaseWidth = CLIP (1, lNewColumnBaseWidth, 1024);
		if (lNewColumnBaseWidth != pEventListFrame->GetColumnBaseWidth (m_lTempColumnIndex)) {
			CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
			pEventListFrame->SetColumnBaseWidth (m_lTempColumnIndex, lNewColumnBaseWidth);
			pEventListFrame->m_pPropertyScaleView->Invalidate ();
			pEventListFrame->m_pIndexPropertyView->Invalidate ();
		}
		m_ptMouseMoveOld = point;
	}
	// ��L���v�^�[��
	else {
		// �J�[�\�������E��ɂ��邩���ׂ�
		long j = 0;
		long lBorderX = 0;
		for (j = 0; j < 8; j++) {
			lBorderX += pEventListFrame->GetColumnBaseWidth (j) * pEventListFrame->GetColumnZoom ();
			if (lBorderX - 2 <= point.x && point.x <= lBorderX + 2) {
				break;
			}
		}
		// �J�[�\�������E��ɂ������ꍇ
		if (0 <= j && j < 8) {
			::SetCursor (pSekaijuApp->m_hCursorSizeWE);
		}
		// �J�[�\�������E��ɂȂ��ꍇ
		else {
			::SetCursor (pSekaijuApp->m_hCursorArrow);
		}
	}

}

// �}�E�X�z�C�[�����񂳂ꂽ��
void CEventListPropertyScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lRowScrollPos = pEventListFrame->GetRowScrollPos ();
		long lRowZoom = pEventListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pEventListFrame->SetRowScrollPos (lRowScrollPos);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
	}
}
