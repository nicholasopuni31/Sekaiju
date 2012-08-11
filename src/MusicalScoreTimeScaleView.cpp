//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃ^�C���X�P�[���r���[�N���X
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
#include <afxcmn.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "ColorfulCheckListBox.h"
#include "ColorfulComboBox.h"
#include "MusicalScoreFrame.h"
#include "MusicalScoreTimeScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CMusicalScoreTimeScaleView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CMusicalScoreTimeScaleView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_RBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_RBUTTONUP ()
	ON_WM_MOUSEMOVE ()
	ON_WM_LBUTTONDBLCLK ()
	ON_WM_RBUTTONDBLCLK ()
	ON_WM_TIMER ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//-----------------------------------------------------------------------------
// �\�z�Ɣj��
//-----------------------------------------------------------------------------

// �R���X�g���N�^
CMusicalScoreTimeScaleView::CMusicalScoreTimeScaleView () {
}

// �f�X�g���N�^
CMusicalScoreTimeScaleView::~CMusicalScoreTimeScaleView () {
}

//-----------------------------------------------------------------------------
// �I�y���[�V����
//-----------------------------------------------------------------------------

// �w�莞���Ƀt���O�ƃe�L�X�g��`��
void CMusicalScoreTimeScaleView::DrawFlagAndText 
(CDC* pDC, long lTime, LPCTSTR lpszText, long lColor) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	long x = pMusicalScoreFrame->TimetoX (lTime);
	CRect rcClient;
	GetClientRect (&rcClient);
	CRect rcBack (x, 0, x + 1024, 16);
	CRect rcFlag (x, 3, x + 3, 12);
	CRect rcText (x + 5, 0, x + 1024, 16);
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, lColor);
	CPen* pOldPen = pDC->SelectObject (&pen);
	pDC->FillSolidRect (&rcBack, ::GetSysColor (COLOR_BTNFACE));
	pDC->FillSolidRect (&rcFlag, lColor);
	pDC->MoveTo (x, 3);
	pDC->LineTo (x, 16);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (lColor);
	pDC->DrawText (lpszText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject (pOldPen);
}


// �I�[�o�[���C�h

// ���_�̈ړ����I�[�o�[���C�h
void CMusicalScoreTimeScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	pDC->SetWindowOrg (pMusicalScoreFrame->GetTimeScrollPos (), 0);
}

// �`��
void CMusicalScoreTimeScaleView::OnDraw (CDC* pDC) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);

	pDC->FillSolidRect (&rcClient, lColorBtnFace);
	CFont* pOldFont = pDC->SelectObject (&(pMusicalScoreFrame->m_theFont));
	
	long x, xold;
	long lVisibleLeftTime = pMusicalScoreFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pMusicalScoreFrame->GetVisibleRightTime ();
	TCHAR szBuf[256];
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);

	// ��i�ɔ��q�L���E�����L���E�}�[�J�[�̕`��
	long lTime = 0;
	long lOldTime = 0;
	CString strText1;
	CString strText2;
	pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	if (pMIDITrack) {
		long lColorTrack1 = MIDITrack_GetForeColor (pMIDITrack);
		forEachEvent (pMIDITrack, pMIDIEvent) {
			lTime = MIDIEvent_GetTime (pMIDIEvent);
			if (0 <= lTime && lTime <= lVisibleRightTime) {
				// �e���|
				if (MIDIEvent_IsTempo (pMIDIEvent)) {
					long lTempo;
					lTempo = MIDIEvent_GetTempo (pMIDIEvent);
					strText1.Format (_T("%1.2lf"), (double)60000000 / (double)lTempo);
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += strText1;
					lOldTime = lTime;
				}
				// ���q�L��
				else if (MIDIEvent_IsTimeSignature (pMIDIEvent)) {
					long lnn, ldd, lcc, lbb;
					MIDIEvent_GetTimeSignature (pMIDIEvent, &lnn, &ldd, &lcc, &lbb);
					strText1.Format (_T("%d/%d"), lnn, 1 << ldd);
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += strText1;
					lOldTime = lTime;
				}
				// �����L��
				else if (MIDIEvent_IsKeySignature (pMIDIEvent)) {
					long lsf, lmi;
					MIDIEvent_GetKeySignature (pMIDIEvent, &lsf, &lmi);
					strText1.Format (_T("%d%s"), labs (lsf), lsf >= 0 ? _T("#") : _T("b"));
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += strText1;
					lOldTime = lTime;
				}
				// �}�[�J�[
				else if (MIDIEvent_IsMarker (pMIDIEvent)) {
					memset (szBuf, 0, sizeof (szBuf));
					MIDIEvent_GetText (pMIDIEvent, szBuf, TSIZEOF (szBuf) - 1);
					if (lOldTime != lTime) {
						if (strText2 != _T("")) {
							DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
						}
						strText2 = _T("");
					}
					if (strText2 != _T("")) {
						strText2 += _T(", ");
					}
					strText2 += szBuf;
					lOldTime = lTime;
				}
			}
		}
		if (strText2 != _T("")) {
			DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
		}
	}
	
	// ���i�ɏ��߃{�^���`��
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lLeftMeasure, lLeftBeat, lLeftTick;
		long lRightMeasure, lRightBeat, lRightTick;
		MIDIData_BreakTime (pMIDIData, lVisibleLeftTime, &lLeftMeasure, &lLeftBeat, &lLeftTick);
		MIDIData_BreakTime (pMIDIData, lVisibleRightTime, &lRightMeasure, &lRightBeat, &lRightTick);
		//xold = pMusicalScoreFrame->TimetoXM (lVisibleLeftTime) - 1;
		xold = pMusicalScoreFrame->MeasuretoX (lLeftMeasure) - 1;
		CRect theRect;
		pDC->SetBkMode (TRANSPARENT);
		for (long i = lLeftMeasure; i <= lRightMeasure + 1; i++) {
			//MIDIData_MakeTime (pMIDIData, i, 0, 0, &lTime);
			//x = pMusicalScoreFrame->TimetoXM (lTime);
			x = pMusicalScoreFrame->MeasuretoX (i);
			pDC->MoveTo (x, 16);
			pDC->LineTo (x, 32);
			theRect = CRect (xold, 16, x, 32);
			pDC->Draw3dRect (xold, 16, x - xold, 16, lColorBtnHighlight, lColorBtnShadow);
			memset (szBuf, 0, sizeof (szBuf));
			_sntprintf (szBuf, 255,  _T("%d"), i);
			pDC->SetTextColor (lColorBtnText);
			pDC->DrawText (szBuf, -1, &theRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			xold = x;
		}
	}
	else {
		long lLeftFrameNumber = lVisibleLeftTime / lTimeResolution;
		long lRightFrameNumber = lVisibleRightTime / lTimeResolution;
		xold = pMusicalScoreFrame->TimetoX (lVisibleLeftTime) - 1;
		CRect theRect;
		pDC->SetBkMode (TRANSPARENT);
		for (long i = lLeftFrameNumber; i <= lRightFrameNumber + 1; i++) {
			lTime = i * lTimeResolution;
			x = pMusicalScoreFrame->TimetoX (lTime);
			pDC->MoveTo (x, 16);
			pDC->LineTo (x, 32);
			theRect = CRect (xold, 16, x, 32);
			pDC->Draw3dRect (xold, 16, x - xold, 16, lColorBtnHighlight, lColorBtnShadow);
			memset (szBuf, 0, sizeof (szBuf));
			_sntprintf (szBuf, 255, _T("%d"), i);
			pDC->DrawText (szBuf, -1, &theRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			xold = x;
		}
	}
	pDC->SelectObject (pOldFont);

	if (GetCapture () == this) {
		long lDownX = pMusicalScoreFrame->TimetoX (m_lDownTime);
		long lCurX = pMusicalScoreFrame->TimetoX (m_lCurTime);
		long lTop = rcClient.top;
		long lBottom = rcClient.bottom;
		CRect rect (lDownX, lTop, lCurX, lBottom);
		pDC->SetROP2 (R2_NOT);
		pDC->Rectangle (&rect);
		pDC->SetROP2 (R2_COPYPEN);
	}	

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �r���[�̍X�V
void CMusicalScoreTimeScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// �N���e�B�J���Z�N�V�����̓��b�N����Ă�����̂Ƃ���B
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}





//-----------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//-----------------------------------------------------------------------------

// �E�B���h�E������
BOOL CMusicalScoreTimeScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// �L�[�������ꂽ��
void CMusicalScoreTimeScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	switch (nChar) {
	// ��
	case VK_LEFT:
		pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ��
	case VK_RIGHT:
		pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// Home
	case VK_HOME:
		pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// End
	case VK_END:
		pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	}
	return;
}

// �}�E�X���{�^�������ꂽ�� 
void CMusicalScoreTimeScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �^�����͉������Ȃ�
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ�
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	
	
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);
	point += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);

	// �㔼�����N���b�N�����Ƃ�(���t�ʒu�ړ�)
	if (point.y < rcClient.Height () / 2) { // 20100103�ǉ�
		pSekaijuDoc->m_theCriticalSection.Lock ();
		long lTime = pMusicalScoreFrame->XtoTime (point.x);
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	
	// ������(���ߔԍ���)���N���b�N�����Ƃ�(���ߑI��)
	else {

		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	
		// �����̋L�^
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName; // 20110103�ǉ�
		VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT)); // 20110103�ǉ�
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName)); // 20110103�C��
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

		// ���I���C�x���g�̑I������(Shift��������Ă��Ȃ��ꍇ����Ctrl��������Ă��Ȃ��ꍇ�̂�)
		if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
			pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
		}

		m_lTempSnap = pMusicalScoreFrame->GetCurSnap ();
		m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
		m_lOldTime = pMusicalScoreFrame->XtoTime (m_ptMouseDown.x) / m_lTempSnap * m_lTempSnap;
		m_lDownTime = pMusicalScoreFrame->XtoTime (point.x) / m_lTempSnap * m_lTempSnap;
		m_lCurTime = pMusicalScoreFrame->XtoTime (point.x) / m_lTempSnap * m_lTempSnap;

		SetCapture ();
		SetTimer (0x21, 55, NULL);
		pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
		VERIFY (pMusicalScoreFrame->UpdateTrackInfoArray ());
		Invalidate ();


		m_ptMouseDown = m_ptMouseMove = point;
		m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
}

// �}�E�X�E�{�^�������ꂽ��  
void CMusicalScoreTimeScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CMusicalScoreTimeScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);
	point += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);

	// �^�����͉������Ȃ�
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ�
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}

	// �L���v�^�|���̏ꍇ
	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pTempTrack = NULL;
		MIDIEvent* pTempEvent = NULL;
		long lFormat = MIDIData_GetFormat (pMIDIData);
		
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		long lMinTime = __min (m_lDownTime, m_lCurTime);
		long lMaxTime = __max (m_lDownTime, m_lCurTime);

		// �Y���͈͂ɂ���m�[�g�C�x���g�̑I��
		long i = 0;
		forEachTrack (pMIDIData, pTempTrack) {
			if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
				long j;
				for (j = 0; j < lNumNoteInfo; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
					VERIFY (pTempEvent = pNoteInfo->m_pNoteOnEvent);
					// ���̉����ɑΉ�����m�[�g�I���C�x���g���܂��I������Ă��Ȃ��ꍇ�̂�
					if (MIDIEvent_GetParent (pTempEvent) == pTempTrack) {
						// �������I��͈͓��ɂ���ꍇ
						long lTime = pNoteInfo->m_lNoteOnTime;
						if (lMinTime <= lTime && lTime < lMaxTime) {
							// �C�x���g�𗚗�u����Ɉڂ��A�V�����C�x���g��I������B
							MIDIEvent* pCloneEvent = pSekaijuDoc->SelectEvent 
								(pTempEvent, 1, pCurHistoryUnit);
							
						}
					}
				}
			}
			i++;
		}
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED); 
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CMusicalScoreTimeScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// �}�E�X���������ꂽ�Ƃ�
void CMusicalScoreTimeScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �^�����͉������Ȃ�
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ�
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);
	point += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);

	// �L���v�^�|���̏ꍇ
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();

		m_lCurTime = pMusicalScoreFrame->XtoTime (point.x) / m_lTempSnap * m_lTempSnap;
		m_lDownTime = pMusicalScoreFrame->XtoTime (m_ptMouseDown.x) / m_lTempSnap * m_lTempSnap;
		m_lOldTime = pMusicalScoreFrame->XtoTime (m_ptMouseMove.x) / m_lTempSnap * m_lTempSnap;
		// �O��̃^�C���ƍ���̃^�C�����قȂ�ꍇ�̂�
		if (m_lOldTime != m_lCurTime) {
			// �ڐ���ĕ`��
			Invalidate (TRUE);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;

}

// �}�E�X���{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CMusicalScoreTimeScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// �}�E�X�E�{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CMusicalScoreTimeScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// �^�C�}�[��
void CMusicalScoreTimeScaleView::OnTimer (UINT nIDEvent) {	
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �}�E�X�L���v�^�[���ɃN���C�A���g�̈���͂ݏo�����ꍇ�̎����X�N���[������
	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), 0);
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldTimeScrollPos = pMusicalScoreFrame->GetTimeScrollPos ();
				if (m_ptMouseMove.x < rcClient.left) {
					pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x >= rcClient.right) {
					pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x - lOldTimeScrollPos);
				WORD wY = (WORD)(m_ptMouseMove.y);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}

// �}�E�X�z�C�[�����񂳂ꂽ��
void CMusicalScoreTimeScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pMusicalScoreFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pMusicalScoreFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lTrackScrollPos = pMusicalScoreFrame->GetTrackScrollPos ();
		long ry = 4;
		lTrackScrollPos -= ry * 2 * lDelta / WHEELDELTA;
		pMusicalScoreFrame->SetTrackScrollPos (lTrackScrollPos);
	}
}

