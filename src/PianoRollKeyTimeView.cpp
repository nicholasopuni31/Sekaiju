//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �s�A�m���[���L�[�^�C���r���[�N���X
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
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "PianoRollFrame.h"
#include "PianoRollKeyTimeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CPianoRollKeyTimeView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CPianoRollKeyTimeView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_TIMER ()
	ON_WM_KEYDOWN ()
	ON_WM_KEYUP ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_RBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_RBUTTONUP ()
	ON_WM_MOUSEMOVE ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CPianoRollKeyTimeView::CPianoRollKeyTimeView () {
	m_lCurTime = m_lOldTime = 0;
	m_lOldY1 = m_lOldY2 = 0;
	m_bOldDraw = TRUE;
	m_pTempEvent = NULL;
	m_lTempMode = 0;
	m_pLastEvent = NULL;
}

// �f�X�g���N�^
CPianoRollKeyTimeView::~CPianoRollKeyTimeView () {
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �m�[�g��`���擾����
CRect CPianoRollKeyTimeView::GetNoteRect (MIDIEvent* pNoteOnEvent) {
	ASSERT (pNoteOnEvent);
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	if (!MIDIEvent_IsNoteOn (pNoteOnEvent)) {
		return CRect (0,0,0,0);
	}
	long lTime = MIDIEvent_GetTime (pNoteOnEvent);
	long lKey = MIDIEvent_GetKey (pNoteOnEvent);
	long lDuration = MIDIEvent_GetDuration (pNoteOnEvent);
	long x = pPianoRollFrame->TimetoX (lTime);
	long w = pPianoRollFrame->TimetoX (lDuration);
	long y = pPianoRollFrame->KeytoY (lKey);
	long h = pPianoRollFrame->GetKeyZoom ();
	return CRect (x, y - h, x + w, y);
}




// ���ʒu�̏c������
void CPianoRollKeyTimeView::EraseOldLine (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	if (rcClient.left <= m_lOldX && m_lOldX <= rcClient.right && m_bOldDraw == TRUE) {
		pDC->SetROP2 (R2_NOT);
		pDC->MoveTo (m_lOldX, m_lOldY1);
		pDC->LineTo (m_lOldX, m_lOldY2);
	}
}

// ���݈ʒu�̏c���`��
void CPianoRollKeyTimeView::DrawCurLine (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	long x = pPianoRollFrame->TimetoX (m_lCurTime);
	if (rcClient.left <= x && x <= rcClient.right) {
		pDC->SetROP2 (R2_NOT);
		pDC->MoveTo (x, rcClient.top);
		pDC->LineTo (x, rcClient.bottom);
		m_bOldDraw = TRUE;
		m_lOldTime = m_lCurTime;
		m_lOldX = x;
		m_lOldY1 = rcClient.top;
		m_lOldY2 = rcClient.bottom;
	}
	else {
		m_bOldDraw = FALSE;
	}
}

// �|�b�v�A�b�v���j���[�̕\��
BOOL CPianoRollKeyTimeView::ShowPopupMenu (CPoint ptMenu) {

	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (m_lTempMode == 0x1001);
	// �m�[�g�C�x���g��Ń|�b�v�A�b�v���j���[��\�������ꍇ
	if (m_pTempEvent) {
		pSekaijuDoc->m_lTempTime = MIDIEvent_GetTime (m_pTempEvent);
		pSekaijuDoc->m_pTempTrack = MIDIEvent_GetParent (m_pTempEvent);
		pSekaijuDoc->m_pTempEvent = m_pTempEvent;
	}
	// �����Ȃ��Ƃ���Ń|�b�v�A�b�v���j���[��\�������ꍇ
	else {
		pSekaijuDoc->m_lTempTime = pSekaijuDoc->m_lNewTime;
		pSekaijuDoc->m_pTempTrack = NULL; //pSekaijuDoc->m_pTempTrack; // 20100429�C��
		pSekaijuDoc->m_pTempEvent = NULL;
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

	CMenu theMenu;
	VERIFY (theMenu.LoadMenu (IDR_POPUPMENU11));
	CMenu* pContextMenu = theMenu.GetSubMenu (0);
	pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		ptMenu.x, ptMenu.y, pPianoRollFrame);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lTempMode = 0x0000;

	return TRUE;
}



//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���_�̈ړ����I�[�o�[���C�h
void CPianoRollKeyTimeView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pDC->SetWindowOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
}

// �`��
void CPianoRollKeyTimeView::OnDraw (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penOctave (PS_SOLID, 1, RGB (128, 128, 255));
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long x, y, lTime;
	long lVisibleLeftTime = pPianoRollFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pPianoRollFrame->GetVisibleRightTime ();
	long lVisibleTopKey = pPianoRollFrame->GetVisibleTopKey ();
	long lVisibleBottomKey = pPianoRollFrame->GetVisibleBottomKey ();
	long lLeftMeasure, lLeftBeat, lLeftTick;
	long lRightMeasure, lRightBeat, lRightTick;
	long lnn, ldd, lcc, lbb;
	long lUnitTick;
	long temp;

	// �����̕`��
	CPen* pOldPen = pDC->SelectObject (&penKey);
	long lKeyZoom = pPianoRollFrame->GetKeyZoom ();
	long i, j;
	for (j = lVisibleBottomKey; j <= lVisibleTopKey + 1; j++) {
		y = pPianoRollFrame->KeytoY (j);
		// �������̓h��ׂ�
		temp = j % 12;
		if (temp == 1 || temp == 3 || temp == 6 || temp == 8 || temp == 10) {
			pDC->FillSolidRect (rcClient.left, y, rcClient.Width (), -lKeyZoom, RGB (255, 255, 192));
		}
		// �����`��
		pDC->MoveTo (rcClient.left, y);
		pDC->LineTo (rcClient.right, y);
	}

	// �c���̕`��
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		MIDIData_BreakTime (pMIDIData, lVisibleLeftTime, &lLeftMeasure, &lLeftBeat, &lLeftTick);
		MIDIData_BreakTime (pMIDIData, lVisibleRightTime, &lRightMeasure, &lRightBeat, &lRightTick);
		for (i = lLeftMeasure; i <= lRightMeasure; i++) {
			// ���ߐ��̕`��
			MIDIData_MakeTimeEx (pMIDIData, i, 0, 0, &lTime, &lnn, &ldd, &lcc, &lbb);
			x = pPianoRollFrame->TimetoX (lTime);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, rcClient.top);
			pDC->LineTo (x, rcClient.bottom);
			// �����̕`��
			pDC->SelectObject (&penBeat);
			lUnitTick = lTimeResolution * 4 / (1 << ldd);
			for (long j = 1; j < lnn; j++) {
				x = pPianoRollFrame->TimetoX (lTime + j * lUnitTick);
				pDC->MoveTo (x, rcClient.top);
				pDC->LineTo (x, rcClient.bottom);
			}
		}
	}
	else {
		long lLeftFrameNumber = lVisibleLeftTime / lTimeResolution;
		long lRightFrameNumber = lVisibleRightTime / lTimeResolution;
		for (i = lLeftFrameNumber; i <= lRightFrameNumber; i++) {
			// �t���[�����E���̕`��
			lTime = i * lTimeResolution;
			x = pPianoRollFrame->TimetoX (lTime);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, rcClient.top);
			pDC->LineTo (x, rcClient.bottom);
		}
		pDC->SelectObject (pOldPen);
	}

	// �I�N�^�[�u���̕`��
	pDC->SelectObject (&penOctave);
	for (j = lVisibleBottomKey; j <= lVisibleTopKey; j++) {
		if (j % 12 == 0) {
			y = pPianoRollFrame->KeytoY (j);
			pDC->MoveTo (rcClient.left, y);
			pDC->LineTo (rcClient.right, y);
		}
	}
	
	// �s�A�m���[���o�[�̕`��
	long w;
	CPen penBar (PS_SOLID, 1, RGB (0, 0, 0));
	pDC->SelectObject (&penBar);
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pPianoRollFrame->IsTrackVisible (i)) {
			long lColor = MIDITrack_GetForeColor (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					if (pMIDIEvent->m_lTime < lVisibleRightTime) {
						x = pPianoRollFrame->TimetoX (MIDIEvent_GetTime (pMIDIEvent));
						w = pPianoRollFrame->TimetoX (MIDIEvent_GetDuration (pMIDIEvent));
						y = pPianoRollFrame->KeytoY (MIDIEvent_GetKey (pMIDIEvent));
						if (pSekaijuDoc->IsEventSelected (pMIDIEvent)) {
							pDC->FillSolidRect (CRect (x, y, x + w, y - lKeyZoom), RGB (0, 0, 0));
						}
						else {
							pDC->FillSolidRect (CRect (x, y, x + w, y - lKeyZoom), lColor);
						}
						pDC->Draw3dRect (x, y - lKeyZoom, w, lKeyZoom ,RGB (255,255,255), RGB (0, 0, 0));
					}
				}
			}
		}
		i++;
	}

	pDC->SelectObject (pOldPen);

	// ���݈ʒu�̕`��
	DrawCurLine (pDC);

	// �}�E�X�g���b�J�[���K�v�ȏꍇ�A�}�E�X�g���b�J�[�̕`��
	if (GetCapture () == this) {
		if (m_lTempMode == 0x0401) {
			pDC->SetROP2 (R2_NOT);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseDown.y);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseMove.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseMove.y);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseDown.x, m_ptMouseMove.y);
			pDC->MoveTo (m_ptMouseMove.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseMove.y);
			pDC->SetROP2 (R2_COPYPEN);
		}
		// ���C���g���b�J�[���K�v�ȏꍇ�A���C���g���b�J�[�̕`��
		else if (m_lTempMode == 0x0201) {
			pDC->SetROP2 (R2_NOT);
			pDC->MoveTo (m_ptMouseDown.x, m_ptMouseDown.y);
			pDC->LineTo (m_ptMouseMove.x, m_ptMouseMove.y);
			pDC->SetROP2 (R2_COPYPEN);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	Sleep (0);
}

// �r���[�̍X�V
void CPianoRollKeyTimeView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// �N���e�B�J���Z�N�V�����̓��b�N����Ă�����̂Ƃ���B
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		PostMessage (WM_TIMER, 0x11, NULL);
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

// �E�B���h�E�N���X�̕ύX
BOOL CPianoRollKeyTimeView::PreCreateWindow (CREATESTRUCT& cs) {
	CView::PreCreateWindow (cs);
	cs.lpszClass = AfxRegisterWndClass 
		(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
		NULL, // �f�t�H���g�J�[�\������
		(HBRUSH)::GetStockObject (WHITE_BRUSH), // �f�t�H���g�w�i�u���V
		NULL); // �f�t�H���g�A�C�R������
	return TRUE;
}




//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
int CPianoRollKeyTimeView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	SetTimer (0x11, 55, NULL);
	return CSekaijuView::OnCreate (lpCreateStruct);
}

// �E�B���h�E�j����
void CPianoRollKeyTimeView::OnDestroy () {
	KillTimer (0x11);
	CSekaijuView::OnDestroy ();
}

// �^�C�}�[�Ăяo����
void CPianoRollKeyTimeView::OnTimer (UINT nIDEvent) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	

	// ���t�E�L�^���̃J�����g�o�[�ړ�����
	if (nIDEvent == 0x11) {
		if (pSekaijuDoc->m_pMIDIData && pSekaijuDoc->m_pMIDIClock) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
			
			CDC* pDC = GetDC ();
			OnPrepareDC (pDC, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
			m_lCurTime = MIDIClock_GetTickCount (GetDocument ()->m_pMIDIClock);
			// �O��̎����ƌ��݂̎���������Ă���ꍇ�̂�
			if (m_lOldTime != m_lCurTime) {
				EraseOldLine (pDC);
				DrawCurLine (pDC);
			}
			// �y�[�W�̍X�V����уI�[�g�X�N���[��(�蓮�X�N���[�����͂��Ȃ�)
			if (pPianoRollFrame->m_bAutoPageUpdate == TRUE) {
				long lTimeMode, lTimeResolution;
				MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
				if (lTimeMode == MIDIDATA_TPQNBASE) {
					// ���݈ʒu���E���ւ͂ݏo�����ꍇ
					long lRightMeasure; long lRightBeat; long lRightTick; long lTempRightTime;
					MIDIData_BreakTime (pMIDIData, pPianoRollFrame->GetVisibleRightTime (),
						&lRightMeasure, &lRightBeat, &lRightTick);
					MIDIData_MakeTime (pMIDIData, lRightMeasure, 0, 0, &lTempRightTime);
					if (m_lCurTime >= lTempRightTime) {
						long lNewMeasure; long lNewBeat; long lNewTick;
						MIDIData_BreakTime (pMIDIData, m_lCurTime, &lNewMeasure, &lNewBeat, &lNewTick);
						MIDIData_MakeTime (pMIDIData, lNewMeasure, 0, 0, &lTempRightTime);
						int x = pPianoRollFrame->TimetoX (lTempRightTime);
						pPianoRollFrame->SetTimeScrollPos (x);
					}
					// ���݈ʒu�������ւ͂ݏo�����ꍇ
					long lLeftMeasure; long lLeftBeat; long lLeftTick; long lTempLeftTime;
					MIDIData_BreakTime (pMIDIData, pPianoRollFrame->GetVisibleLeftTime (),
						&lLeftMeasure, &lLeftBeat, &lLeftTick);
					MIDIData_MakeTime (pMIDIData, lLeftMeasure, 0, 0, &lTempLeftTime);
					if (m_lCurTime < lTempLeftTime) {
						long lNewMeasure; long lNewBeat; long lNewTick;
						MIDIData_BreakTime (GetDocument()->m_pMIDIData, m_lCurTime, &lNewMeasure, &lNewBeat, &lNewTick);
						MIDIData_MakeTime (GetDocument()->m_pMIDIData, lNewMeasure, 0, 0, &lTempLeftTime);
						int x = pPianoRollFrame->TimetoX (lTempLeftTime);
						pPianoRollFrame->SetTimeScrollPos (x);
					}
				}
				else {
					// ���݈ʒu���E���ւ͂ݏo�����ꍇ
					long lRightFrameNumber = pPianoRollFrame->GetVisibleRightTime () / lTimeResolution;
					long lTempRightTime = lRightFrameNumber * lTimeResolution;
					if (m_lCurTime >= lTempRightTime) {
						lTempRightTime = (m_lCurTime / lTimeResolution) * lTimeResolution;
						long x = pPianoRollFrame->TimetoX (lTempRightTime);
						pPianoRollFrame->SetTimeScrollPos (x);
					}
					// ���݈ʒu�������ւ͂ݏo�����ꍇ
					long lLeftFrameNumber = pPianoRollFrame->GetVisibleLeftTime () / lTimeResolution;
					long lTempLeftTime = lLeftFrameNumber * lTimeResolution;
					if (m_lCurTime < lTempLeftTime) {
						lTempLeftTime = (m_lCurTime / lTimeResolution) * lTimeResolution;
						long x = pPianoRollFrame->TimetoX (lTempLeftTime);
						pPianoRollFrame->SetTimeScrollPos (x);
					}
				}
			}
			ReleaseDC (pDC);
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);
		}
	}
	// �}�E�X�L���v�^�[���ɃN���C�A���g�̈���͂ݏo�����ꍇ�̎����X�N���[������
	else if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				long lOldTimeScrollPos = pPianoRollFrame->GetTimeScrollPos ();
				long lOldKeyScrollPos = pPianoRollFrame->GetKeyScrollPos ();
				pSekaijuDoc->m_theCriticalSection.Lock ();
				if (m_ptMouseMove.x < rcClient.left) {
					pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x > rcClient.right) {
					pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				if (m_ptMouseMove.y < rcClient.top) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y > rcClient.bottom) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x - lOldTimeScrollPos);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldKeyScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}


// �L�[�������ꂽ��
void CPianoRollKeyTimeView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	switch (nChar) {
	// ��
	case VK_UP:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ��
	case VK_DOWN:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndKeyScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ��
	case VK_LEFT:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ��
	case VK_RIGHT:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// Home
	case VK_HOME:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// End
	case VK_END:
		pPianoRollFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndTimeScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// DEL
	case VK_DELETE:
		// �L���v�^�[���Ȃ��
		if (GetCapture () == this) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			long lOldKey = pPianoRollFrame->YtoKey (m_ptMouseMove.y);
			switch (m_lTempTool) {
			// �y��
			case ID_PIANOROLL_PEN:
				if ((m_lTempMode == 0x0101 || m_lTempMode == 0x0102) && m_pTempEvent != NULL) {
					ASSERT (m_pTempTrack);
					// �u�����Ƃ��Ă���m�[�g���͈ړ����̃m�[�g�폜
					if (m_lTempMode = 0x0101) {
						MIDIEvent_Delete (m_pTempEvent);
						m_pTempEvent = NULL;
					}
					else if (m_lTempMode = 0x0102) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, m_pTempEvent);
						MIDITrack_RemoveEvent (m_pTempTrack, m_pTempEvent);
					}
					// �����L�^
					if (m_pLastEvent) {
						if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
						}
					}
					// �Â��L�[����
					MIDIOut* pMIDIOut = NULL;
					MIDIStatus* pMIDIStatus = NULL;
					BYTE byMIDIMessage[3];
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)0;
					if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
						pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
						pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
					}
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
					ReleaseCapture ();
					KillTimer (0x21);
					m_lTempMode = 0;
					pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
				}
				break;
			}
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);
		}
		// �L���v�^�[���łȂ��Ȃ��
		else {
			// �w�ҏW(E)�x-�w�폜�x���s (20090823�ǉ�)
			PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		}
		break;
	// ESC
	case VK_ESCAPE:
		// �L���v�^�[���Ȃ��
		if (GetCapture () == this) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			long lOldKey = pPianoRollFrame->YtoKey (m_ptMouseMove.y);
			switch (m_lTempTool) {
			// �y��
			case ID_PIANOROLL_PEN:
				if (m_lTempMode == 0x0101 && m_pTempEvent != NULL) {
					ASSERT (m_pTempTrack);
					// �u�����Ƃ��Ă���m�[�g�p�~
					MIDIEvent_Delete (m_pTempEvent);
					m_pTempEvent = NULL;
					// �����L�^
					if (m_pLastEvent) {
						if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
						}
					}
					// �Â��L�[����
					MIDIOut* pMIDIOut = NULL;
					MIDIStatus* pMIDIStatus = NULL;
					if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
						pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
						pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
					}
					BYTE byMIDIMessage[3];
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)0;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
					ReleaseCapture ();
					KillTimer (0x21);
					m_lTempMode = 0;
					pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					break;
				}
			}
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);
		}
		break;
	// Control
	case VK_CONTROL:
		if (1) {
			WORD wX = (WORD)(m_ptMouseMove.x - pPianoRollFrame->GetTimeScrollPos ());
			WORD wY = (WORD)(m_ptMouseMove.y - pPianoRollFrame->GetKeyScrollPos ());
			UINT nFlags = m_nMouseMoveFlags | MK_CONTROL;
			PostMessage (WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)((wY << 16) | wX));
		}
		break;
	}
	return;
}


// �L�[�������ꂽ��
void CPianoRollKeyTimeView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// Control
	case VK_CONTROL:
		if (1) {
			WORD wX = (WORD)(m_ptMouseMove.x - pPianoRollFrame->GetTimeScrollPos ());
			WORD wY = (WORD)(m_ptMouseMove.y - pPianoRollFrame->GetKeyScrollPos ());
			UINT nFlags = m_nMouseMoveFlags & (~MK_CONTROL);
			PostMessage (WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)((wY << 16) | wX));
		}
		break;
	}
	return;
}

// �}�E�X���{�^�������ꂽ��  
void CPianoRollKeyTimeView::OnLButtonDown (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	point += sizWndOrg;
	ASSERT (m_pTempEvent == NULL);
	ASSERT (m_lTempMode == 0);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	CString strHistoryName;
	CHistoryUnit* pCurHistoryUnit = NULL;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
	m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
	m_lTempKey = pPianoRollFrame->YtoKey (point.y);
	m_lTempTime = pPianoRollFrame->XtoTime (point.x);
	m_lTempTool = pPianoRollFrame->m_lCurTool;
	m_lTempChannel = pPianoRollFrame->GetCurChannel ();
	m_lTempChannel = CLIP (0, m_lTempChannel, 15);
	m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
	m_lTempVelocity = CLIP (1, m_lTempVelocity, 127);
	m_lTempDuration = pPianoRollFrame->GetCurDuration ();
	m_lTempDuration = CLIP (1, m_lTempDuration, 65535);
	m_lTempSnap = pPianoRollFrame->GetCurSnap ();
	m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
	m_pTempEvent = NULL;
	m_pLastEvent = NULL;
	CRect rcRegion;
	BOOL bPtInNote = FALSE;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIOut* pMIDIOut = NULL;
	MIDIStatus* pMIDIStatus = NULL;
	BYTE byMIDIMessage[3];
	long i = 0;
	switch (m_lTempTool) {
	// �y��
	case ID_PIANOROLL_PEN:
		// �t�H�[�}�b�g1��MIDI�f�[�^�ōŏ��̃g���b�N�Ƀm�[�g��u���̂��֎~
		if (lFormat == 1 && m_lTempTrackIndex == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			break;
		}
		// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ
		i = MIDIData_CountTrack (pMIDIData) - 1; // 20090629�ǉ�
		forEachTrackInverse (pMIDIData, pTempTrack) { // 20090629�C��
			long lPort = MIDITrack_GetOutputPort (pTempTrack);
			if (0 <= lPort && lPort < MAXMIDIOUTDEVICENUM) {
				pMIDIOut = pSekaijuApp->m_pMIDIOut[lPort];
				pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lPort];
			}
			if (pPianoRollFrame->IsTrackVisible (i)) { // 20090629�C��
				forEachEventInverse (pTempTrack, pTempEvent) {
					if (MIDIEvent_IsNoteOn (pTempEvent) && MIDIEvent_IsNote (pTempEvent)) {
						CRect rcNote = GetNoteRect (pTempEvent);
						if (rcNote.top <= point.y && point.y <= rcNote.bottom) {
							// �����`�̍��ӏ���N���b�N
							if (rcNote.left <= point. x && point.x < rcNote.left + rcNote.Width () / 4) {
								pPianoRollFrame->SetCurTrackIndex (i);
								pPianoRollFrame->SetCurVelocity (MIDIEvent_GetVelocity (pTempEvent));
								pPianoRollFrame->SetCurDuration (MIDIEvent_GetDuration (pTempEvent));
								pPianoRollFrame->SetCurChannel (MIDIEvent_GetChannel (pTempEvent));
								m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
								m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
								m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
								m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
								m_lTempDuration = pPianoRollFrame->GetCurDuration ();
								m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack); // 20100226�C��
								if (m_lTempChannel < 0 || m_lTempChannel >= 16) { // 20100226�ǉ�
									m_lTempChannel = pPianoRollFrame->GetCurChannel (); // 20100226�ǉ�
								} // 20100226�ǉ�
								m_lTempTimeNoteOn = MIDIEvent_GetTime (pTempEvent);
								m_lTempMode = 0x0103;
								VERIFY (strHistoryName.LoadString (IDS_MODIFY_NOTE));
								VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
								pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
								pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
								if (pLastEvent) {
									if (MIDIEvent_IsEndofTrack (pLastEvent)) {
										pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
										m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
									}
								}
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
								m_pTempEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
								rcRegion = GetNoteRect (m_pTempEvent);
								InvalidateRect (rcRegion - sizWndOrg);
								::SetCursor (pSekaijuApp->m_hCursorSizeWE);
								pPianoRollFrame->m_bAutoPageUpdate = FALSE;
								SetTimer (0x21, 55, NULL);
								SetCapture ();
								break;
							}
							// �����`�̉E�ӏ���N���b�N
							else if (rcNote.right - rcNote.Width () / 4 < point. x && point.x <= rcNote.right) {
								pPianoRollFrame->SetCurTrackIndex (i);
								pPianoRollFrame->SetCurVelocity (MIDIEvent_GetVelocity (pTempEvent));
								pPianoRollFrame->SetCurDuration (MIDIEvent_GetDuration (pTempEvent));
								pPianoRollFrame->SetCurChannel (MIDIEvent_GetChannel (pTempEvent));
								m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
								m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
								m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
								m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
								m_lTempDuration = pPianoRollFrame->GetCurDuration ();
								m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack); // 20100226�C��
								if (m_lTempChannel < 0 || m_lTempChannel >= 16) { // 20100226�ǉ�
									m_lTempChannel = pPianoRollFrame->GetCurChannel (); // 20100226�ǉ�
								} // 20100226�ǉ�
								m_lTempTimeNoteOn = MIDIEvent_GetTime (pTempEvent);
								m_lTempMode = 0x0104;
								VERIFY (strHistoryName.LoadString (IDS_MODIFY_NOTE));
								VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
								pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
								pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
								if (pLastEvent) {
									if (MIDIEvent_IsEndofTrack (pLastEvent)) {
										pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
										m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
									}
								}
								pCurHistoryUnit->AddHistoryRecord 
									(HISTORYRECORD_REMOVEEVENT, pTempEvent);
								m_pTempEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
								rcRegion = GetNoteRect (m_pTempEvent);
								InvalidateRect (rcRegion - sizWndOrg);
								::SetCursor (pSekaijuApp->m_hCursorSizeWE);
								pPianoRollFrame->m_bAutoPageUpdate = FALSE;
								SetTimer (0x21, 55, NULL);
								SetCapture ();
								break;
							}
							// �����`�̓������N���b�N
							else if (rcNote.left <= point. x && point.x <= rcNote.right) {
								pPianoRollFrame->SetCurTrackIndex (i);
								pPianoRollFrame->SetCurVelocity (MIDIEvent_GetVelocity (pTempEvent));
								pPianoRollFrame->SetCurDuration (MIDIEvent_GetDuration (pTempEvent));
								pPianoRollFrame->SetCurChannel (MIDIEvent_GetChannel (pTempEvent));
								m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
								m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
								m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
								m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
								m_lTempDuration = pPianoRollFrame->GetCurDuration ();
								m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack); // 20100226�C��
								if (m_lTempChannel < 0 || m_lTempChannel >= 16) { // 20100226�ǉ�
									m_lTempChannel = pPianoRollFrame->GetCurChannel (); // 20100226�ǉ�
								} // 20100226�ǉ�
								m_lTempTimeNoteOn = MIDIEvent_GetTime (pTempEvent);
								byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
								byMIDIMessage[1] = (BYTE)CLIP (0, m_lTempKey + MIDITrack_GetKeyPlus (pTempTrack), 127);
								byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (pTempTrack), 127);								
								if (pMIDIOut) {
									MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
								}
								if (pMIDIStatus) {
									MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
								}
								m_lTempMode = 0x0102;
								VERIFY (strHistoryName.LoadString (IDS_MODIFY_NOTE));
								VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
								pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
								pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
								if (pLastEvent) {
									if (MIDIEvent_IsEndofTrack (pLastEvent)) {
										pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
										m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
									}
								}
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
								m_pTempEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
								rcRegion = GetNoteRect (m_pTempEvent);
								if (nFlags & MK_CONTROL) {
									::SetCursor (pSekaijuApp->m_hCursorSizeAll);
								}
								else {
									::SetCursor (pSekaijuApp->m_hCursorSizeAll);
								}
								InvalidateRect (rcRegion - sizWndOrg);
								pPianoRollFrame->m_bAutoPageUpdate = FALSE;
								SetTimer (0x21, 55, NULL);
								SetCapture ();
								break;
							}
						}
					}
				}
				if (m_lTempMode != 0) {
					break;
				}
			}
			i--;
		}
		// �����Ȃ��Ƃ���Ƀ}�E�X���u���ꂽ�ꍇ
		if (m_pTempEvent == NULL) {
			if (MIDIData_GetFormat (pMIDIData) == 1 &&
				m_pTempTrack == MIDIData_GetFirstTrack (pMIDIData)) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				return;
			}
			if (pPianoRollFrame->IsTrackVisible (m_lTempTrackIndex) == FALSE) {
				pPianoRollFrame->SetTrackVisible (m_lTempTrackIndex);
				pPianoRollFrame->m_pKeyScaleView->Invalidate ();
				pPianoRollFrame->m_pKeyTimeView->Invalidate ();
				pPianoRollFrame->m_pVelScaleView->Invalidate ();
				pPianoRollFrame->m_pVelTimeView->Invalidate ();
			}
			m_lTempTime = (m_lTempTime / m_lTempSnap) * m_lTempSnap;
			m_lTempTimeNoteOn = m_lTempTime;
			m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
			m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
			m_pTempEvent = MIDIEvent_CreateNote 
				(m_lTempTime, m_lTempChannel, m_lTempKey, m_lTempVelocity, m_lTempDuration);
			if (m_pTempEvent == NULL) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_ANY_MORE_BEACUSE_OF_INSUFFICIENT_MEMORY));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				return;
			}
			VERIFY (strHistoryName.LoadString (IDS_INSERT_NOTE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			pLastEvent = MIDITrack_GetLastEvent (m_pTempTrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
					m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
				}
			}
			MIDITrack_InsertEvent (m_pTempTrack, m_pTempEvent);
			m_lTempMode = 0x0101;
			if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
				pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
				pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
			}
			byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
			byMIDIMessage[1] = (BYTE)CLIP (0, m_lTempKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
			byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (m_pTempTrack), 127);
			if (pMIDIOut) {
				MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
			}
			if (pMIDIStatus) {
				MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
			}
			rcRegion = GetNoteRect (m_pTempEvent);
			InvalidateRect (rcRegion  - sizWndOrg);
			pPianoRollFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
		}
		break;
	// ��
	case ID_PIANOROLL_LINE:
		if (MIDIData_GetFormat (pMIDIData) == 1 &&
			m_pTempTrack == MIDIData_GetFirstTrack (pMIDIData)) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		if (pPianoRollFrame->IsTrackVisible (m_lTempTrackIndex) == FALSE) {
			pPianoRollFrame->SetTrackVisible (m_lTempTrackIndex);
			pPianoRollFrame->m_pKeyScaleView->Invalidate ();
			pPianoRollFrame->m_pKeyTimeView->Invalidate ();
			pPianoRollFrame->m_pVelScaleView->Invalidate ();
			pPianoRollFrame->m_pVelTimeView->Invalidate ();
		}
		m_lTempTime = (m_lTempTime / m_lTempSnap) * m_lTempSnap;
		m_lTempTimeNoteOn = m_lTempTime;
		m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
		m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
		if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
		}
		byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
		byMIDIMessage[1] = (BYTE)CLIP (0, m_lTempKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
		byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (m_pTempTrack), 127);
		if (pMIDIOut) {
			MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
		}
		if (pMIDIStatus) {
			MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
		}
		::SetCursor (pSekaijuApp->m_hCursorLine);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		m_lTempMode = 0x0201;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;

	// �����S��
	case ID_PIANOROLL_ERASER:
		// �t�H�[�}�b�g1��MIDI�f�[�^�ōŏ��̃g���b�N�Ƀm�[�g�������̂��֎~
		if (lFormat == 1 && m_lTempTrackIndex == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DELETE_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			break;
		}
		VERIFY (strHistoryName.LoadString (IDS_DELETE_NOTE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		m_pTempEvent = NULL;
		m_lTempMode = 0x0301;
		// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�����ׂ�
		bPtInNote = FALSE;
		i = MIDIData_CountTrack (pMIDIData) - 1;
		forEachTrackInverse (pMIDIData, pTempTrack) {
			if (pPianoRollFrame->IsTrackVisible (i)) {
				forEachEventInverse (pTempTrack, pTempEvent) {
					if (MIDIEvent_IsNoteOn (pTempEvent) && MIDIEvent_IsNote (pTempEvent)) {
						CRect rcNote = GetNoteRect (pTempEvent);
						// �����̃m�[�g�̏���N���b�N�����ꍇ
						if (rcNote.top <= point.y && point.y <= rcNote.bottom &&
							rcNote.left <= point.x && point. x<= rcNote.right) {
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
							rcRegion = rcNote - sizWndOrg;
							InvalidateRect (rcRegion);
							bPtInNote = TRUE;
							break;
						}
					}
				}
			}
			if (bPtInNote) {
				break;
			}
			i++;
		}
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;

	// �I��
	case ID_PIANOROLL_SELECT:
		bPtInNote = FALSE;
		i = 0;
		// �����̑I������Ă���m�[�g�̏�Ƀ}�E�X���u���ꂽ�����ׂ�
		forEachTrack (pMIDIData, pTempTrack) {
			long lPort = MIDITrack_GetOutputPort (pTempTrack);
			if (0 <= lPort && lPort < MAXMIDIOUTDEVICENUM) {
				pMIDIOut = pSekaijuApp->m_pMIDIOut[lPort];
				pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lPort];
			}
			if (pPianoRollFrame->IsTrackVisible (i)) {
				forEachEvent (pTempTrack, pTempEvent) {
					if (MIDIEvent_IsNoteOn (pTempEvent) && MIDIEvent_IsNote (pTempEvent) &&
						pSekaijuDoc->IsEventSelected (pTempEvent)) {
						CRect rcNote = GetNoteRect (pTempEvent);
						if (rcNote.top <= point.y && point.y <= rcNote.bottom &&
							rcNote.left <= point.x && point. x<= rcNote.right) {
							bPtInNote = TRUE;
							break;
						}
					}
				}
				if (bPtInNote) {
					break;
				}
			}
			i++;
		}
		// �����̑I������Ă���m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ(���ʖ��͈ړ����[�h)
		if (bPtInNote) {
			pPianoRollFrame->SetCurTrackIndex (i);
			pPianoRollFrame->SetCurVelocity (MIDIEvent_GetVelocity (pTempEvent));
			pPianoRollFrame->SetCurDuration (MIDIEvent_GetDuration (pTempEvent));
			pPianoRollFrame->SetCurChannel (MIDIEvent_GetChannel (pTempEvent));
			m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
			m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
			m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
			m_lTempDuration = pPianoRollFrame->GetCurDuration ();
			m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack); // 20100226�C��
			if (m_lTempChannel < 0 || m_lTempChannel >= 16) { // 20100226�ǉ�
				m_lTempChannel = pPianoRollFrame->GetCurChannel (); // 20100226�ǉ�
			} // 20100226�ǉ�
			m_lTempTimeNoteOn = MIDIEvent_GetTime (pTempEvent);
			m_pTempEvent = pTempEvent;
			byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
			byMIDIMessage[1] = (BYTE)CLIP (0, m_lTempKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
			byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (m_pTempTrack), 127);
			if (pMIDIOut) {
				MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
			}
			if (pMIDIStatus) {
				MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
			}
			m_theTempSelectedEventArray.RemoveAll ();
			// ���ʂ̏ꍇ(CONTROL��������Ă���)
			if (nFlags & MK_CONTROL) {
				VERIFY (strHistoryName.LoadString (IDS_DUPLICATE_NOTE));
				VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
				pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
				i = 0; //20080902�ǉ�
				forEachTrack (pMIDIData, pTempTrack) { //20080902�C��
					if (pPianoRollFrame->IsTrackVisible (i)) {
						// ���̃g���b�N��EndofTrack�C�x���g�𗚗��L�^
						pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
						if (pLastEvent) {
							if (MIDIEvent_IsEndofTrack (pLastEvent)) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
								m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
							}
						}
						// �I���m�[�g�C�x���g�̕���
						forEachEvent (pTempTrack, pTempEvent) {
							if (MIDIEvent_IsNoteOn (pTempEvent) &&
								pSekaijuDoc->IsEventSelected (pTempEvent) &&
								pTempEvent->m_pPrevCombinedEvent == NULL) {
								// �I�����ꂽ�m�[�g�I�����I����Ԃɂ���B
								MIDIEvent* pCloneEvent = NULL; 
								pCloneEvent = pSekaijuDoc->SelectEvent (pTempEvent, 0, pCurHistoryUnit);
								if (pCloneEvent == NULL) {
									continue;
								}
								// �m�[�g�I���𕡎ʂ��A���ʂ�������I����Ԃɂ���B
								pCloneEvent = pSekaijuDoc->DuplicateMIDIEvent (pCloneEvent);
								if (pCloneEvent) {
									pCloneEvent->m_lUser1 = MIDIEvent_GetKey (pCloneEvent);
									pCloneEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
									pSekaijuDoc->SetEventSelected (pCloneEvent, 1);
									m_theTempSelectedEventArray.Add (pCloneEvent);
									pTempEvent = pCloneEvent;
								}	
							}
						}
					}
					i++;
				}
				::SetCursor (pSekaijuApp->m_hCursorSizeAllCopy);
				m_lTempMode = 0x0403;
			}
			// �ړ��̏ꍇ(CONTROL��������Ă��Ȃ�)
			else {
				VERIFY (strHistoryName.LoadString (IDS_MOVE_NOTE));
				VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
				pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
				i = 0; //20080902�ǉ�
				forEachTrack (pMIDIData, pTempTrack) {
					if (pPianoRollFrame->IsTrackVisible (i)) {
						pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
						if (pLastEvent) {
							if (MIDIEvent_IsEndofTrack (pLastEvent)) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
								m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
							}
						}
						forEachEvent (pTempTrack, pTempEvent) {
							if (MIDIEvent_IsNoteOn (pTempEvent) &&
								pSekaijuDoc->IsEventSelected (pTempEvent) &&
								pTempEvent->m_pPrevCombinedEvent == NULL) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
								MIDIEvent* pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
								if (pCloneEvent) {
									pCloneEvent->m_lUser1 = MIDIEvent_GetKey (pCloneEvent);
									pCloneEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
									m_theTempSelectedEventArray.Add (pCloneEvent);
									pTempEvent = pCloneEvent;
								}
							}
						}
					}
					i++;
				}
				::SetCursor (pSekaijuApp->m_hCursorSizeAll);
				m_lTempMode = 0x0402;
			}
			rcRegion = GetNoteRect (m_pTempEvent);
			InvalidateRect (rcRegion - sizWndOrg);
			pPianoRollFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
			
		}
		// �����Ȃ��Ƃ���Ƀ}�E�X���u���ꂽ�ꍇ(�I��͈̓��[�h)
		else {
			// ���I���C�x���g�̑I������(Shift��������Ă��Ȃ��ꍇ����Ctrl��������Ă��Ȃ��ꍇ�̂�)
			CString strHistoryName;
			VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT_NOTE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
				pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
			}
			m_lTempMode = 0x0401;
			dc.SetROP2 (R2_NOT);
			dc.SetPixel (point, RGB (0, 0, 0));
			dc.SetROP2 (R2_COPYPEN);
			::SetCursor (pSekaijuApp->m_hCursorSelect);
			Invalidate ();
			pPianoRollFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
		}
		break;

	// �X�N���u
	case ID_PIANOROLL_SPEAKER:
		if (pSekaijuApp->m_bPlaying) {
			pPianoRollFrame->SendMessage (WM_COMMAND, (WPARAM)ID_CONTROL_PLAY, (LPARAM)0);
		}
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
		pSekaijuApp->SendDifferentStatus (SDS_ALL);
		m_lTempMode = 0x0501;
		::SetCursor (pSekaijuApp->m_hCursorSpeaker);
		pPianoRollFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;
	}
	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�������ꂽ��  
void CPianoRollKeyTimeView::OnRButtonDown (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// �}�E�X�L���v�^�[���͉������Ȃ�
	if (GetCapture () == this) {
		return;
	}
	
	// ���̑��쒆�͉������Ȃ�
	if (m_lTempMode != 0x0000) {
		return;
	}

	CPoint ptMenu (point);
	ClientToScreen (&ptMenu);

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	point += sizWndOrg;
	ASSERT (m_pTempEvent == NULL);
	ASSERT (m_lTempMode == 0);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	CHistoryUnit* pCurHistoryUnit = NULL;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	m_lTempTrackIndex = pPianoRollFrame->GetCurTrackIndex ();
	m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
	m_lTempKey = pPianoRollFrame->YtoKey (point.y);
	m_lTempTime = pPianoRollFrame->XtoTime (point.x);
	m_lTempTool = pPianoRollFrame->m_lCurTool;
	m_lTempChannel = pPianoRollFrame->GetCurChannel ();
	m_lTempChannel = CLIP (0, m_lTempChannel, 15);
	m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
	m_lTempVelocity = CLIP (1, m_lTempVelocity, 127);
	m_lTempDuration = pPianoRollFrame->GetCurDuration ();
	m_lTempDuration = CLIP (1, m_lTempDuration, 65535);
	m_lTempSnap = pPianoRollFrame->GetCurSnap ();
	m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
	m_pTempEvent = NULL;
	m_pLastEvent = NULL;
	CRect rcRegion;
	BOOL bPtInNote = FALSE;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIOut* pMIDIOut = NULL;
	MIDIStatus* pMIDIStatus = NULL;
	// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ
	long i = MIDIData_CountTrack (pMIDIData) - 1; // 20090629�C��
	forEachTrackInverse (pMIDIData, pTempTrack) { // 20090629�C��
		long lPort = MIDITrack_GetOutputPort (pTempTrack);
		if (0 <= lPort && lPort < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[lPort];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lPort];
		}
		if (pPianoRollFrame->IsTrackVisible (i)) {
			forEachEventInverse (pTempTrack, pTempEvent) { // 20090629�C��
				if (MIDIEvent_IsNoteOn (pTempEvent) && MIDIEvent_IsNote (pTempEvent)) {
					CRect rcNote = GetNoteRect (pTempEvent);
					// �����`�̓������N���b�N
					if (rcNote.top <= point.y && point.y <= rcNote.bottom) {
						if (rcNote.left <= point. x && point.x <= rcNote.right) {
 							//pPianoRollFrame->SetCurTrackIndex (i);
							//pPianoRollFrame->SetCurVelocity (MIDIEvent_GetVelocity (pTempEvent));
							//pPianoRollFrame->SetCurDuration (MIDIEvent_GetDuration (pTempEvent));
							//pPianoRollFrame->SetCurChannel (MIDIEvent_GetChannel (pTempEvent));
							m_lTempTrackIndex = i;
							m_pTempTrack = pSekaijuDoc->GetTrack (i);
							m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
							m_lTempVelocity = pPianoRollFrame->GetCurVelocity ();
							m_lTempDuration = pPianoRollFrame->GetCurDuration ();
							m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack); // 20110115�C��
							if (m_lTempChannel < 0 || m_lTempChannel >= 16) { // 20110115�ǉ�
								m_lTempChannel = pPianoRollFrame->GetCurChannel (); // 20110115�ǉ�
							} // 20110115�ǉ�
							m_lTempTimeNoteOn = MIDIEvent_GetTime (pTempEvent);
							m_pTempEvent = pTempEvent;
							break;
						}
					}
				}
			}
			if (m_pTempEvent) {
				break;
			}
		}
		i--; // 20090629�C��
	}
	m_lTempMode = 0x1001;
	
	ShowPopupMenu (ptMenu);
	
	m_pTempEvent = NULL;
	m_lTempMode = 0x0000;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CPianoRollKeyTimeView::OnLButtonUp (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	point += sizWndOrg;

	long lDownKey = pPianoRollFrame->YtoKey (m_ptMouseDown.y);
	long lOldKey = pPianoRollFrame->YtoKey (m_ptMouseMove.y);
	long lNewKey = pPianoRollFrame->YtoKey (point.y);
	long lDownTime = pPianoRollFrame->XtoTime (m_ptMouseDown.x);
	long lOldTime = pPianoRollFrame->XtoTime (m_ptMouseMove.x);
	long lNewTime = pPianoRollFrame->XtoTime (point.x);
	long lStartKey = MIN (lDownKey, lNewKey);
	long lEndKey = MAX (lDownKey, lNewKey);
	long lStartTime = MIN (lDownTime, lNewTime);
	long lEndTime = MAX (lDownTime, lNewTime);
	long i = 0;
	CRect rcRegion;
	// �L���v�^�[���Ȃ��
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
		MIDIOut* pMIDIOut = NULL;
		MIDIStatus* pMIDIStatus = NULL;
		if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
		}
		CHistoryUnit* pCurHistoryUnit = NULL;
		MIDITrack* pTempTrack = NULL;
		MIDIEvent* pTempEvent = NULL;
		MIDIEvent* pLastEvent = NULL;
		BYTE byMIDIMessage[3];
		switch (m_lTempTool) {
		// �y��
		case ID_PIANOROLL_PEN:
			ASSERT (m_pTempTrack);
			ASSERT (m_pTempEvent);
			ASSERT (m_lTempMode);
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����}�������͉����ړ����̏ꍇ�Ɍ���
			if (m_lTempMode == 0x0101 || m_lTempMode == 0x0102) {
				// �Â��L�[����
				byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
				byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
				byMIDIMessage[2] = (BYTE)0;
				if (pMIDIOut) {
					MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
				}
				if (pMIDIStatus) {
					MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
				}
			}
			pSekaijuDoc->SetModifiedFlag (TRUE);
			// �����L�^
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pTempEvent);
			if (m_pLastEvent) {
				if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
					pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
				}
			}
			break;
		// ��
		case ID_PIANOROLL_LINE:
			ASSERT (m_pTempTrack);
			ASSERT (m_lTempMode);
			if (m_lTempMode == 0x0201) {
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_INSERT_NOTE));
				VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
				// �Â��L�[����
				byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
				byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
				byMIDIMessage[2] = (BYTE)0;
				if (pMIDIOut) {
					MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
				}
				if (pMIDIStatus) {
					MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
				}
				// EOT����
				pLastEvent = MIDITrack_GetLastEvent (m_pTempTrack);
				if (pLastEvent) {
					if (MIDIEvent_IsEndofTrack (pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
						m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
					}
				}
				// �m�[�g�C�x���g�̑}��
				long lKey = 0;
				long lStartTime2 = 0;
				long lEndTime2 = 0;
				long lDur = 0;
				long lCh = m_lTempChannel & 0x0F;
				long lVel = CLIP (1, m_lTempVelocity, 127); 
				// �������̏ꍇ
				if (lEndTime == lStartTime) {
					;
				}
				// �������̏ꍇ
				else if (lEndKey == lStartKey) {
					lDur = lEndTime - lStartTime;
					MIDIEvent* pNewEvent = MIDIEvent_CreateNote 
						(lStartTime, lCh, lKey, lVel, lDur);
					if (pNewEvent) {
						MIDITrack_InsertEvent (m_pTempTrack, pNewEvent);
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
					}
				}
				// �������E�㖔�͉E�と�����̏ꍇ
				else if (lDownKey < lOldKey && lDownTime < lOldTime ||
					lDownKey > lOldKey && lDownTime > lOldTime) {
					for (lKey = lStartKey; lKey <= lEndKey; lKey++) {
						lStartTime2 = lStartTime + 
							(lEndTime - lStartTime) * (lKey - lStartKey) / (lEndKey - lStartKey);
						lEndTime2 = lStartTime +
							(lEndTime - lStartTime) * (lKey + 1 - lStartKey) / (lEndKey - lStartKey);
						lDur = lEndTime2 - lStartTime2;
						MIDIEvent* pNewEvent = MIDIEvent_CreateNote 
							(lStartTime2, lCh, lKey, lVel, lDur);
						if (pNewEvent) {
							MIDITrack_InsertEvent (m_pTempTrack, pNewEvent);
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
						}
					}
				}
				// �E�������㖔�͍��と�E���̏ꍇ
				else if (lDownKey < lOldKey && lDownTime > lOldTime ||
					lDownKey > lOldKey && lDownTime < lOldTime) {
					for (lKey = lEndKey; lKey >= lStartKey; lKey--) {
						lStartTime2 = lStartTime + 
							(lEndTime - lStartTime) * (lEndKey - lKey) / (lEndKey - lStartKey);
						lEndTime2 = lStartTime +
							(lEndTime - lStartTime) * (lEndKey - lKey + 1) / (lEndKey - lStartKey);
						lDur = lEndTime2 - lStartTime2;
						MIDIEvent* pNewEvent = MIDIEvent_CreateNote 
							(lStartTime2, lCh, lKey, lVel, lDur);
						if (pNewEvent) {
							MIDITrack_InsertEvent (m_pTempTrack, pNewEvent);
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
						}
					}
				}
				// EOT����
				if (m_pLastEvent) {
					if (MIDIEvent_IsEndofTrack (m_pLastEvent)) {
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent);
					}
				}
				pSekaijuDoc->SetModifiedFlag (TRUE);
			}
			break;
		// �����S��
		case ID_PIANOROLL_ERASER:
			pSekaijuDoc->SetModifiedFlag (TRUE);
			break;
		// �I��
		case ID_PIANOROLL_SELECT:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����ړ������͉������ʒ��̏ꍇ
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				// �e�I���C�x���g�̃}�E�X�_�E�����̃L�[���ƃ^�C�������폜
				long j;
				long lTempSelectedEventCount = m_theTempSelectedEventArray.GetSize ();
				for (j = 0; j < lTempSelectedEventCount; j++) {
					MIDIEvent* pTempEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
					pTempEvent->m_lUser1 = 0;
					pTempEvent->m_lUser2 = 0;
				}
				// ���ʂ̏ꍇ
				if (m_lTempMode == 0x0403) {
					// ���̈ʒu�Ɠ����ꍇ�͕��ʂ��Ȃ��B
					if ((lDownKey == lNewKey) &&
						(lDownTime / m_lTempSnap) * m_lTempSnap == 
						(lNewTime / m_lTempSnap) * m_lTempSnap) {
						for (j = 0; j < lTempSelectedEventCount; j++) {
							MIDIEvent* pCloneEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
							MIDIEvent_Delete (pCloneEvent);
						}
					}
					// ���̈ʒu�ƈقȂ�ꏊ�ɕ��ʂ��ꂽ�ꍇ�A���ʊm��A�����L�^�B
					else {
						for (j = 0; j < lTempSelectedEventCount; j++) {
							MIDIEvent* pCloneEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
						}
					}
				}
				// �ړ��̏ꍇ
				else if (m_lTempMode == 0x0402) {
					for (j = 0; j < lTempSelectedEventCount; j++) {
						MIDIEvent* pCloneEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
						pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
					}
				}
				// EOT�̗����L�^
				i = 0;
				forEachTrack (pMIDIData, pTempTrack) {
					if (pPianoRollFrame->IsTrackVisible (i)) {
						pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
						if (pLastEvent) {
							if (MIDIEvent_IsEndofTrack (pLastEvent)) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent);
							}
						}
					}
					i++;
				}
				// �Â��L�[����
				byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
				byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
				byMIDIMessage[2] = (BYTE)0;
				if (pMIDIOut) {
					MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
				}
				if (pMIDIStatus) {
					MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
				}
				pSekaijuDoc->SetModifiedFlag (TRUE);
			}
			// �I��͈͕ύX���̏ꍇ
			else if (m_lTempMode == 0x0401) {
				// �V�K��`�����I��(�����E�փh���b�O)
				if (m_ptMouseMove.x >= m_ptMouseDown.x) {
					forEachTrack (pMIDIData, pTempTrack) {
						if (pPianoRollFrame->IsTrackVisible (i)) {
							forEachEvent (pTempTrack, pTempEvent) {
								if ((MIDIEvent_IsNoteOn (pTempEvent) || MIDIEvent_IsNoteOff (pTempEvent)) &&
									pTempEvent->m_pPrevCombinedEvent == NULL) {
									long lTime = MIDIEvent_GetTime (pTempEvent);
									long lDur = MIDIEvent_GetDuration (pTempEvent);
									if (lStartTime <= lTime && lTime <= lEndTime &&
										lStartTime <= lTime + lDur && lTime + lDur <= lEndTime) {
										long lKey = MIDIEvent_GetKey (pTempEvent);
										if (lStartKey + 1 <= lKey && lKey <= lEndKey - 1) { // 2010042
											MIDIEvent* pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
							}
						}
						i++;
					}
				}
				// �V�K��`�ڐG�I��(�E�����փh���b�O) // 20100503�ǉ�
				else {
					forEachTrack (pMIDIData, pTempTrack) {
						if (pPianoRollFrame->IsTrackVisible (i)) {
							forEachEvent (pTempTrack, pTempEvent) {
								if ((MIDIEvent_IsNoteOn (pTempEvent) || MIDIEvent_IsNoteOff (pTempEvent)) &&
									pTempEvent->m_pPrevCombinedEvent == NULL) {
									long lTime = MIDIEvent_GetTime (pTempEvent);
									long lDur = MIDIEvent_GetDuration (pTempEvent);
									if (lStartTime <= lTime && lTime <= lEndTime ||
										lStartTime <= lTime + lDur && lTime + lDur <= lEndTime ||
										lTime <= lStartTime && lEndTime <= lTime + lDur) {
										long lKey = MIDIEvent_GetKey (pTempEvent);
										if (lStartKey <= lKey && lKey <= lEndKey) {
											MIDIEvent* pCloneEvent = pSekaijuDoc->SelectEvent 
												(pTempEvent, 1, pCurHistoryUnit);
											if (pCloneEvent) {
												pTempEvent = pCloneEvent;
											}
										}
									}
								}
							}
						}
						i++;
					}
				}
			}
			break;

		// �X�N���u
		case ID_PIANOROLL_SPEAKER:
			pSekaijuApp->SendAllNoteOff ();
			pSekaijuApp->SendAllSoundOff ();
			break;
		}
		m_theTempSelectedEventArray.RemoveAll ();
		m_pTempEvent = NULL;
		m_lTempMode = 0;
		KillTimer (0x21);
		ReleaseCapture ();
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CPianoRollKeyTimeView::OnRButtonUp (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

}

// �}�E�X���������ꂽ�Ƃ�
void CPianoRollKeyTimeView::OnMouseMove (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
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
	
	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pPianoRollFrame->GetTimeScrollPos (), pPianoRollFrame->GetKeyScrollPos ());
	point += sizWndOrg;
	CRect rcClient;
	GetClientRect (&rcClient);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	long lDownKey = pPianoRollFrame->YtoKey (m_ptMouseDown.y);
	long lOldKey = pPianoRollFrame->YtoKey (m_ptMouseMove.y);
	long lNewKey = pPianoRollFrame->YtoKey (point.y);
	long lDownTime = pPianoRollFrame->XtoTime (m_ptMouseDown.x);
	long lOldTime = pPianoRollFrame->XtoTime (m_ptMouseMove.x);
	long lNewTime = pPianoRollFrame->XtoTime (point.x);
	CRect rcRegion;
	
	BYTE byMIDIMessage[3];
	// �L���v�^�[���Ȃ��
	if (GetCapture () == this) {
		// �}�E�X����������Ă��Ȃ��̂ɂ��̊֐����Ăяo���ꂽ�ꍇ�͉������Ȃ�(20090629�ǉ�)
		if (point == m_ptMouseMove) {
			pSekaijuDoc->m_theCriticalSection.Unlock (); // 20090704�C��
			return;
		}
		MIDIOut* pMIDIOut = NULL;
		MIDIStatus* pMIDIStatus = NULL;
		pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
		if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
		}
		long i = 0;
		BOOL bErased = FALSE;
		// �c�[���ʂ̑���
		switch (m_lTempTool) {
		case ID_PIANOROLL_PEN:
			ASSERT (m_pTempTrack);
			ASSERT (m_pTempEvent);
			ASSERT (m_lTempMode);
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����ړ���
			if (m_lTempMode == 0x0101 || m_lTempMode == 0x0102) {
				// �L�[���ς�����ꍇ
				if (lOldKey != lNewKey) {
					rcRegion = GetNoteRect (m_pTempEvent);
					rcRegion.InflateRect (1, 1);
					InvalidateRect (rcRegion - sizWndOrg);
					// �Â��L�[����
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)0;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
					// �V�����L�[��ݒ�
					MIDIEvent_SetKey (m_pTempEvent, CLIP (0, lNewKey, 127));
					rcRegion = GetNoteRect (m_pTempEvent);
					rcRegion.InflateRect (1, 1);
					InvalidateRect (rcRegion - sizWndOrg);
					// �V�����L�[����
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lNewKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (m_pTempTrack), 127);;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
				}
				// �������ς�����ꍇ
				if ((lOldTime / m_lTempSnap) * m_lTempSnap != 
					(lNewTime / m_lTempSnap) * m_lTempSnap) {
					long lDeltaTime =
						(lNewTime / m_lTempSnap) * m_lTempSnap - 
						(lOldTime / m_lTempSnap) * m_lTempSnap;
					long lCurTime = MIDIEvent_GetTime (m_pTempEvent);
					rcRegion = GetNoteRect (m_pTempEvent);
					rcRegion.InflateRect (1, 1);
					InvalidateRect (rcRegion - sizWndOrg);
					long lTempTime = CLIP (0, lCurTime + lDeltaTime, 0x7FFF0000);
					MIDIEvent_SetTime (m_pTempEvent, lTempTime);
					rcRegion = GetNoteRect (m_pTempEvent);
					rcRegion.InflateRect (1, 1);
					InvalidateRect (rcRegion - sizWndOrg);
				}
			}
			// �����̊J�n�����ړ�
			else if (m_lTempMode == 0x0103) {
				// �������ς�����ꍇ(20081227�C��)
				if (lOldTime != lNewTime) {
					long lOldEventTime = MIDIEvent_GetTime (m_pTempEvent);
					long lNewEventTime = CLIP (0, lNewTime, 0x7FFF0000);
					lNewEventTime = ((lNewEventTime + m_lTempSnap/ 2) / m_lTempSnap) * m_lTempSnap; 
					long lOldEventDur = MIDIEvent_GetDuration (m_pTempEvent);
					long lNewEventDur = lOldEventDur - (lNewEventTime - lOldEventTime);
					lNewEventDur = CLIP (MIN (m_lTempSnap, lOldEventDur), lNewEventDur, 65565);
					if (lOldEventTime != lNewEventTime || lOldEventDur != lNewEventDur) {
						rcRegion = GetNoteRect (m_pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
						MIDIEvent_SetTime (m_pTempEvent, lNewEventTime);
						MIDIEvent_SetDuration (m_pTempEvent, lNewEventDur);
						pPianoRollFrame->SetCurDuration (lNewEventDur);
						rcRegion = GetNoteRect (m_pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
					}
				}
			}
			// �����̏I�������ړ�
			else if (m_lTempMode == 0x0104) {
				// �������ς�����ꍇ(20081227�C��)
				if (lOldTime != lNewTime) {
					long lOldEventTime = MIDIEvent_GetTime (m_pTempEvent);
					long lOldEventDur = MIDIEvent_GetDuration (m_pTempEvent);
					long lOldEventEndTime = lOldEventTime + lOldEventDur;
					long lNewEventEndTime = CLIP (0, lNewTime, 0x7FFFFFFF);
					lNewEventEndTime = ((lNewEventEndTime + m_lTempSnap/ 2) / m_lTempSnap) * m_lTempSnap;
					long lNewEventDur = lNewEventEndTime - lOldEventTime;
					lNewEventDur = CLIP (MIN (m_lTempSnap, lOldEventDur), lNewEventDur, 65565);
					if (lOldEventEndTime != lNewEventEndTime || lOldEventDur != lNewEventDur) {
						rcRegion = GetNoteRect (m_pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
						MIDIEvent_SetDuration (m_pTempEvent, lNewEventDur);
						pPianoRollFrame->SetCurDuration (lNewEventDur);
						rcRegion = GetNoteRect (m_pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
					}
				}
			}
			break;

		// ��
		case ID_PIANOROLL_LINE:
			ASSERT (m_pTempTrack);
			ASSERT (m_lTempMode);
			// �����ړ���
			if (m_lTempMode == 0x0201) {
				rcRegion = CRect (m_ptMouseDown, m_ptMouseMove);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_ptMouseDown, point);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				// �L�[���ς�����ꍇ
				if (lOldKey != lNewKey) {
					// �Â��L�[����
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)0;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
					// �V�����L�[����
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lNewKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (m_pTempTrack), 127);;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
				}
			}
			break;
		// �����S��
		case ID_PIANOROLL_ERASER:
			bErased = FALSE; // 20090629�ǉ�
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ */
			i = MIDIData_CountTrack (pMIDIData) - 1;
			forEachTrackInverse (pMIDIData, pTempTrack) {
				if (pPianoRollFrame->IsTrackVisible (i)) {
					forEachEventInverse (pTempTrack, pTempEvent) { // 20090629�ǉ�
						if (MIDIEvent_IsNoteOn (pTempEvent) && MIDIEvent_IsNote (pTempEvent)) {
							CRect rcNote = GetNoteRect (pTempEvent);
							// �����`�̓�����ʉ�
							if (rcNote.top <= point.y && point.y <= rcNote.bottom &&
								rcNote.left <= point.x && point.x <= rcNote.right) {
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
								MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
								rcRegion = rcNote;
								rcRegion.InflateRect (1, 1);
								bErased = TRUE;  // 20090629�ǉ�
								InvalidateRect (rcRegion - sizWndOrg);
								break;
							}
						}
					}
					if (bErased) { // 20090629�ǉ�
						break;
					}
				}
				i--;
			}
			break;
		// �I��
		case ID_PIANOROLL_SELECT:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �ړ����͕��ʒ�
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				long lTempSelectedEventArrayCount = m_theTempSelectedEventArray.GetSize ();
				long i;
				// �L�[���ς�����ꍇ
				if (lOldKey != lNewKey) {
					long lDeltaKey = lNewKey - lDownKey;
					// �Â��L�[����
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lOldKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)0;
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
					// �V�����L�[��ݒ�
					for (i = 0; i < lTempSelectedEventArrayCount; i++) {
						MIDIEvent* pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (i);
						rcRegion = GetNoteRect (pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
						long lTempKey = pTempEvent->m_lUser1;
						long lTargetKey = lTempKey + lDeltaKey;
						lTargetKey = CLIP (0, lTargetKey, 127);
						MIDIEvent_SetKey (pTempEvent, lTargetKey);
						rcRegion = GetNoteRect (pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
					}
					// �V�����L�[����
					byMIDIMessage[0] = 0x90 | (m_lTempChannel & 0x0F);
					byMIDIMessage[1] = (BYTE)CLIP (0, lNewKey + MIDITrack_GetKeyPlus (m_pTempTrack), 127);
					byMIDIMessage[2] = (BYTE)CLIP (1, m_lTempVelocity + MIDITrack_GetVelocityPlus (m_pTempTrack), 127);
					if (pMIDIOut) {
						MIDIOut_PutMIDIMessage (pMIDIOut, byMIDIMessage, 3);
					}
					if (pMIDIStatus) {
						MIDIStatus_PutMIDIMessage (pMIDIStatus, byMIDIMessage, 3);
					}
				}
				// �������ς�����ꍇ
				if ((lOldTime / m_lTempSnap) * m_lTempSnap != 
					(lNewTime / m_lTempSnap) * m_lTempSnap) {
					long lDeltaTime =
						(lNewTime / m_lTempSnap) * m_lTempSnap - 
						(lDownTime / m_lTempSnap) * m_lTempSnap;
					for (i = 0; i < lTempSelectedEventArrayCount; i++) {
						MIDIEvent* pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (i);
						rcRegion = GetNoteRect (pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
						long lTempTime = pTempEvent->m_lUser2;
						long lTargetTime = lTempTime + lDeltaTime;
						lTargetTime = CLIP (0, lTargetTime, 0x7FFFFFFF);
						MIDIEvent_SetTime (pTempEvent, lTargetTime);
						rcRegion = GetNoteRect (pTempEvent);
						rcRegion.InflateRect (1, 1);
						InvalidateRect (rcRegion - sizWndOrg);
					}
				}
			}
			// �I��͈͈ړ���
			else if (m_lTempMode = 0x0401) {
				// ��B(�I����`�͂�����������͂�����Ȃ�)
				rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, m_ptMouseMove.x, m_ptMouseDown.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_ptMouseDown.x, m_ptMouseMove.y, m_ptMouseMove.x, m_ptMouseMove.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, m_ptMouseDown.x, m_ptMouseMove.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_ptMouseMove.x, m_ptMouseDown.y, m_ptMouseMove.x, m_ptMouseMove.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);

				rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, point.x, m_ptMouseDown.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_ptMouseDown.x, point.y, point.x, point.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (m_ptMouseDown.x, m_ptMouseDown.y, m_ptMouseDown.x, point.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
				rcRegion = CRect (point.x, m_ptMouseDown.y, point.x, point.y);
				rcRegion.NormalizeRect ();
				rcRegion.InflateRect (1, 1);
				InvalidateRect (rcRegion - sizWndOrg);
			}
			break;

		// �X�N���u
		case ID_PIANOROLL_SPEAKER:
			m_lTempTime = pPianoRollFrame->XtoTime (point.x);
			pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
			pSekaijuApp->SendDifferentStatus (SDS_ALL);
			break;
		}
	}
	// ��L���v�^�[��(�J�[�\���̎�ނ����ύX)
	else {
		long i = 0;
		BOOL bChanged = FALSE;
		MIDITrack* pMIDITrack;
		MIDIEvent* pMIDIEvent;
		switch (pPianoRollFrame->m_lCurTool) {
		// �`��
		case ID_PIANOROLL_PEN:
			i = MIDIData_CountTrack (pMIDIData) - 1; // 20090629�ǉ�
			forEachTrackInverse (pMIDIData, pMIDITrack) { // 20090629�C��
				if (pPianoRollFrame->IsTrackVisible (i)) {
					forEachEventInverse (pMIDITrack, pMIDIEvent) { // 20090629�C��
						if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
							if (MIDIEvent_GetKey (pMIDIEvent) == lNewKey) {
								long lNoteOnTime = MIDIEvent_GetTime (pMIDIEvent);
								long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
								long lNoteOffTime = lNoteOnTime + lDuration;
								if (lNoteOnTime <= lNewTime && lNewTime < lNoteOnTime + lDuration / 4) {
									::SetCursor (pSekaijuApp->m_hCursorSizeWE);
									bChanged = TRUE;
									break;
								}
								else if (lNoteOnTime + lDuration / 4 <= lNewTime && 
									lNewTime <= lNoteOffTime - lDuration / 4) {
									if (nFlags & MK_CONTROL) {
										::SetCursor (pSekaijuApp->m_hCursorSizeAll);
									}
									else {
										::SetCursor (pSekaijuApp->m_hCursorSizeAll);
									}
									bChanged = TRUE;
									break;
								}
								else if (lNoteOffTime - lDuration / 4 < lNewTime && lNewTime <= lNoteOffTime) {
									::SetCursor (pSekaijuApp->m_hCursorSizeWE);
									bChanged = TRUE;
									break;
								}
							}
						}
					}
					if (bChanged) { // 20090629�ǉ�
						break;
					}
				}
				i--;
			}
			if (bChanged == FALSE) {
				::SetCursor (pSekaijuApp->m_hCursorDraw);
			}
			break;
		// ��
		case ID_PIANOROLL_LINE:
			::SetCursor (pSekaijuApp->m_hCursorLine);
			break;
		// �����S��
		case ID_PIANOROLL_ERASER:
			::SetCursor (pSekaijuApp->m_hCursorEraser);
			break;
		// �I��
		case ID_PIANOROLL_SELECT:
			i = 0;
			forEachTrack (pMIDIData, pMIDITrack) {
				if (pPianoRollFrame->IsTrackVisible (i)) {
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (pSekaijuDoc->IsEventSelected (pMIDIEvent)) {
							if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
								if (MIDIEvent_GetKey (pMIDIEvent) == lNewKey) {
									long lNoteOnTime = MIDIEvent_GetTime (pMIDIEvent);									long lNoteOffTime = lNoteOnTime + MIDIEvent_GetDuration (pMIDIEvent);
									if (lNoteOnTime <= lNewTime && lNewTime <= lNoteOffTime) {
										if (nFlags & MK_CONTROL) {
											::SetCursor (pSekaijuApp->m_hCursorSizeAllCopy);
										}
										else {
											::SetCursor (pSekaijuApp->m_hCursorSizeAll);
										}
										bChanged = TRUE;
										break;
									}
								}
							}
						}
					}
				}
				i++;
			}
			if (!bChanged) {
				::SetCursor (pSekaijuApp->m_hCursorSelect);
			}
			break;
		// �X�s�[�J�[
		case ID_PIANOROLL_SPEAKER:
			::SetCursor (pSekaijuApp->m_hCursorSpeaker);
			break;
		// �s��
		default:
			//::SetCursor (pSekaijuApp->m_hCursorArrow);
			break;
		}
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�z�C�[�����񂳂ꂽ��
void CPianoRollKeyTimeView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pPianoRollFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pPianoRollFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lKeyScrollPos = pPianoRollFrame->GetKeyScrollPos ();
		long lKeyZoom = pPianoRollFrame->GetKeyZoom ();
		lKeyScrollPos -= lKeyZoom * lDelta / WHEELDELTA;
		pPianoRollFrame->SetKeyScrollPos (lKeyScrollPos);
	}
}