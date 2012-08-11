//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�^�C���X�P�[���r���[�N���X
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
#include "SekaijuToolbar.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "TrackListTimeScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CTrackListTimeScaleView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CTrackListTimeScaleView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
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

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CTrackListTimeScaleView::CTrackListTimeScaleView () {
}

// �f�X�g���N�^
CTrackListTimeScaleView::~CTrackListTimeScaleView () {
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �w�莞���Ƀt���O�ƃe�L�X�g��`��
void CTrackListTimeScaleView::DrawFlagAndText 
(CDC* pDC, long lTime, LPCTSTR lpszText, long lColor) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	long x = pTrackListFrame->TimetoX (lTime);
	CRect rcClient;
	GetClientRect (&rcClient);
	CRect rcBack (x, 0, x + 1024, 16);
	CRect rcFlag (x, 3, x + 4, 12);
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


//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���_�̈ړ����I�[�o�[���C�h
void CTrackListTimeScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	pDC->SetWindowOrg (pTrackListFrame->GetTimeScrollPos (), 0);
}

// �`��
void CTrackListTimeScaleView::OnDraw (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	//long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	//long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	//ong lDeltaMeasure = 1;
	//switch (lTimeMode) {
	//case MIDIDATA_SMPTE24BASE:
	//	lDeltaMeasure = 24;
	//	break;
	//case MIDIDATA_SMPTE25BASE:
	//	lDeltaMeasure = 25;
	//	break;
	//case MIDIDATA_SMPTE29BASE:
	//case MIDIDATA_SMPTE30BASE:
	//	lDeltaMeasure = 30;
	//	break;
	//}
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetTimeScrollPos (), 0);
	pDC->FillSolidRect (&rcClient, ::GetSysColor (COLOR_3DFACE));
	CFont* pOldFont = pDC->SelectObject (&(pTrackListFrame->m_theFont));
	long j;
	long lVisibleLeftTime = pTrackListFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pTrackListFrame->GetVisibleRightTime ();
	long lVisibleTopRow = pTrackListFrame->GetVisibleTopRow ();
	long lVisibleBottomRow = pTrackListFrame->GetVisibleBottomRow ();
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	TCHAR szBuf[256];

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

	// ���ߋ��E���̓t���[�����E��z��ɗ�
	long lLeftMeasure, lLeftBeat, lLeftTick;
	long lRightMeasure, lRightBeat, lRightTick;
	MIDIData_BreakTime (pMIDIData, lVisibleLeftTime, &lLeftMeasure, &lLeftBeat, &lLeftTick);
	MIDIData_BreakTime (pMIDIData, lVisibleRightTime, &lRightMeasure, &lRightBeat, &lRightTick);
	//if (lTimeMode != MIDIDATA_TPQNBASE) {
	//	lLeftMeasure = (lLeftMeasure / lDeltaMeasure) * lDeltaMeasure;
	//	lRightMeasure = (lRightMeasure / lDeltaMeasure) * lDeltaMeasure;
	//}
	long* pBorderTime = (long*)calloc (lRightMeasure - lLeftMeasure + 2, sizeof (long));
	for (j = lLeftMeasure; j <= lRightMeasure + 1; j++) {
		long lTime = 0;
		MIDIData_MakeTime (pMIDIData, j, 0, 0, &lTime);
		*(pBorderTime + j - lLeftMeasure) = lTime;
	}
	
	// �e���߃{�^���̕`��
	pDC->SetBkMode (TRANSPARENT);
	for (j = lLeftMeasure; j < lRightMeasure + 1; j += 1) {
		long lTime0 = *(pBorderTime + j - lLeftMeasure);
		long lTime1 = *(pBorderTime + j - lLeftMeasure + 1);
		long x0 = pTrackListFrame->TimetoX (lTime0);
		long x1 = pTrackListFrame->TimetoX (lTime1);
		CRect theRect (x0, 16, x1, 32);
		long lOffset = (j - lLeftMeasure);
		BOOL bMeasureSelected = FALSE;
		long lMeasureCount = 0;
		long lSelectedMeasureCount = 0;
		long lHalfSelectedMeasureCount = 0;
		long lNothingMeasureCount = 0;
		long lUnSelectedMeasureCount = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			lMeasureCount++;
			long lRet = pSekaijuDoc->IsTrackMeasureSelected (pMIDITrack, j);
			switch (lRet) {
			case 0:
				lNothingMeasureCount++;
				break;
			case 1:
				lUnSelectedMeasureCount++;
				break;
			case 2:
				lHalfSelectedMeasureCount++;
				break;
			case 3:
				lSelectedMeasureCount++;
				break;
			}
		}
		if (lNothingMeasureCount == lMeasureCount) {
			bMeasureSelected = FALSE;
		}
		else if (lSelectedMeasureCount > 0 && lUnSelectedMeasureCount == 0) {
			bMeasureSelected = TRUE;
		}
		else {
			bMeasureSelected = FALSE;
		}
		if (GetCapture () == this) {
			long lMinMeasure = __min (m_lDownMeasure, m_lCurMeasure);
			long lMaxMeasure = __max (m_lDownMeasure, m_lCurMeasure);
			if (lMinMeasure <= j && j <= lMaxMeasure) {
				bMeasureSelected = TRUE;
			}
		}
		memset (szBuf, 0, sizeof (szBuf));
		_sntprintf (szBuf, 255, _T("%d"), j + 1);
		if (bMeasureSelected) {
			pDC->FillSolidRect (&theRect, lColorBlack);
			pDC->Draw3dRect (&theRect, lColorBlack, lColorBtnShadow);
			pDC->SetTextColor (lColorWhite);
			pDC->DrawText (szBuf, -1, &theRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		else {
			pDC->FillSolidRect (&theRect, lColorBtnFace);
			pDC->Draw3dRect (&theRect, lColorBtnHighlight, lColorBtnShadow);
			pDC->SetTextColor (lColorBtnText);
			pDC->DrawText (szBuf, -1, &theRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}

	pDC->SelectObject (pOldFont);

	free (pBorderTime);


	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}





//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
BOOL CTrackListTimeScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// �E�B���h�E�j����
void CTrackListTimeScaleView::OnDestroy () {
	CSekaijuView::OnDestroy ();

}

// �L�[����������
void CTrackListTimeScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	// Delete�L�[
	case VK_DELETE:
		// �w�ҏW(E)�x-�w�폜�x���s (20090823�ǉ�)
		PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		break;
	}
}

// �}�E�X���{�^�������ꂽ�� 
void CTrackListTimeScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �^�����͉������Ȃ�
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetTimeScrollPos (), 0);
	point += CSize (pTrackListFrame->GetTimeScrollPos (), 0);

	// �㔼�����N���b�N�����Ƃ�(���t�ʒu�ړ�)
	if (point.y < rcClient.Height () / 2) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		long lTime = pTrackListFrame->XtoTime (point.x);
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}

	// ������(���ߔԍ���)���N���b�N�����Ƃ�(���ߑI��)
	else {
		pSekaijuDoc->m_theCriticalSection.Lock ();

		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDITrack* pCloneTrack = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		long j;

		// �����̋L�^
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

		// ���I���C�x���g�̑I������(Shift��������Ă��Ȃ��ꍇ����Ctrl��������Ă��Ȃ��ꍇ�̂�)
		if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
			pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
		}

		// �Y�����ߖ��͊Y���t���[�����̑S�C�x���g�̑I��
		//long lOldMeasure, lOldBeat, lOldTick;
		m_lOldTime = pTrackListFrame->XtoTime (m_ptMouseDown.x);
		MIDIData_BreakTime (pMIDIData, m_lOldTime, &m_lOldMeasure, &m_lOldBeat, &m_lOldTick);
		//long lCurMeasure, lCurBeat, lCurTick;
		m_lDownTime = pTrackListFrame->XtoTime (point.x);
		MIDIData_BreakTime (pMIDIData, m_lDownTime, &m_lDownMeasure, &m_lDownBeat, &m_lDownTick);
		//long lCurMeasure, lCurBeat, lCurTick;
		m_lCurTime = pTrackListFrame->XtoTime (point.x);
		MIDIData_BreakTime (pMIDIData, m_lCurTime, &m_lCurMeasure, &m_lCurBeat, &m_lCurTick);
		if (nFlags & MK_SHIFT) {
			long lMinMeasure = __min (m_lOldMeasure, m_lCurMeasure);
			long lMaxMeasure = __max (m_lOldMeasure, m_lCurMeasure);
			forEachTrack (pMIDIData, pMIDITrack) {
				for (j = lMinMeasure; j <= lMaxMeasure; j++) {
					pSekaijuDoc->SelectTrackMeasure (pMIDITrack, j, 1, pCurHistoryUnit);
				}
			}
		}
		else {
			forEachTrack (pMIDIData, pMIDITrack) {
				pSekaijuDoc->SelectTrackMeasure (pMIDITrack, m_lCurMeasure, 1, pCurHistoryUnit);
			}
		}


		SetCapture ();
		SetTimer (0x21, 55, NULL);
		pTrackListFrame->m_bAutoPageUpdate = FALSE;
		Invalidate ();


		m_ptMouseDown = m_ptMouseMove = point;
		m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
}

// �}�E�X�E�{�^�������ꂽ��  
void CTrackListTimeScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CTrackListTimeScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �^�����͉������Ȃ�
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetTimeScrollPos (), 0);
	point += CSize (pTrackListFrame->GetTimeScrollPos (), 0);

	if (GetCapture () == this) {

		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());		
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		ReleaseCapture ();
		KillTimer (0x21);

		if (m_lDownMeasure != m_lCurMeasure) {
			long lMinMeasure = __min (m_lDownMeasure, m_lCurMeasure);
			long lMaxMeasure = __max (m_lDownMeasure, m_lCurMeasure);
			//if (lDownMeasure <= lOldMeasure && lOldMeasure <= lCurMeasure ||
			//	lDownMeasure >= lOldMeasure && lOldMeasure >= lCurMeasure) {
			forEachTrack (pMIDIData, pMIDITrack) {
				long j;
				for (j = lMinMeasure; j <= lMaxMeasure; j++) {
					pSekaijuDoc->SelectTrackMeasure (pMIDITrack, j, 1, pCurHistoryUnit);
				}
			}
		}


		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED); 
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CTrackListTimeScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// �}�E�X���������ꂽ�Ƃ�
void CTrackListTimeScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �^�����͉������Ȃ�
	if (pSekaijuApp->m_bRecording) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
	if (pSekaijuDoc->m_bEditLocked) {
		::SetCursor (pSekaijuApp->m_hCursorNo);
		return;
	}
	
	
	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		rcClient += CSize (pTrackListFrame->GetTimeScrollPos (), 0);
		point += CSize (pTrackListFrame->GetTimeScrollPos (), 0);

		// ���ߖ��̓t���[���̉��I�𖔂͉��I������
		//long lDownMeasure, lDownBeat, lDownTick;
		m_lDownTime = pTrackListFrame->XtoTime (m_ptMouseDown.x);
		MIDIData_BreakTime (pMIDIData, m_lDownTime, &m_lDownMeasure, &m_lDownBeat, &m_lDownTick);
		//long lOldMeasure, lOldBeat, lOldTick;
		m_lOldTime = pTrackListFrame->XtoTime (m_ptMouseMove.x);
		MIDIData_BreakTime (pMIDIData, m_lOldTime, &m_lOldMeasure, &m_lOldBeat, &m_lOldTick);
		//long lCurMeasure, lCurBeat, lCurTick;
		m_lCurTime = pTrackListFrame->XtoTime (point.x);
		MIDIData_BreakTime (pMIDIData, m_lCurTime, &m_lCurMeasure, &m_lCurBeat, &m_lCurTick);
		if (m_lOldMeasure != m_lCurMeasure) {
			Invalidate (FALSE);
		}
		m_ptMouseMove = point;
		m_nMouseMoveFlags = nFlags;
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �}�E�X���{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CTrackListTimeScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// �}�E�X�E�{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CTrackListTimeScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// �^�C�}�[��
void CTrackListTimeScaleView::OnTimer (UINT nIDEvent) {	
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pTrackListFrame->GetTimeScrollPos (), 0);
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldTimeScrollPos = pTrackListFrame->GetTimeScrollPos ();
				if (m_ptMouseMove.x < rcClient.left) {
					pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x >= rcClient.right) {
					pTrackListFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pTrackListFrame->m_wndTimeScroll.GetSafeHwnd ()));
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
void CTrackListTimeScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lRowScrollPos = pTrackListFrame->GetRowScrollPos ();
		long lRowZoom = pTrackListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pTrackListFrame->SetRowScrollPos (lRowScrollPos);
	}
}


