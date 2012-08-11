//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �s�A�m���[���x���V�e�B�X�P�[���r���[�N���X
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
#include "PianoRollVelScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPianoRollVelScaleView, CView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CPianoRollVelScaleView, CView)
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

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CPianoRollVelScaleView::CPianoRollVelScaleView () {
}

// �f�X�g���N�^
CPianoRollVelScaleView::~CPianoRollVelScaleView () {
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���_�̈ړ�
void CPianoRollVelScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pDC->SetWindowOrg (0, pPianoRollFrame->GetVelScrollPos ());
}

// �`��
void CPianoRollVelScaleView::OnDraw (CDC* pDC) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);


	// �w�i�̓h��Ԃ�
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (&rcClient, lColorBtnFace);

	// �ڐ��艡���̕`��
	CPen penKey (PS_SOLID, 1, RGB (0, 0, 0));
	CPen penOctave (PS_SOLID, 1, RGB (0, 0, 0));
	CPen* pOldPen = pDC->SelectObject (&penKey);
	long i;
	for (i = 0; i <= 8; i++) {
		if (i != 4) {
			pDC->SelectObject (&penKey);
		}
		else {
			pDC->SelectObject (&penOctave);
		}
		long y = pPianoRollFrame->VeltoY (i * 16);
		pDC->MoveTo (rcClient.left, y);
		pDC->LineTo (rcClient.right, y);
	}

	// �x���V�e�B�E�`�����l���A�t�^�[�E�R���g���[���`�F���W�l�̕`��
	BOOL bControlChangeVisible = FALSE;
	CString strText;
	CRect rcText (rcClient);
	pDC->SelectObject (&(pPianoRollFrame->m_theFont));
	for (i = 4; i < 4 + 127; i++) {
		if (pPianoRollFrame->IsGraphVisible (i)) {
			bControlChangeVisible = TRUE;
			break;
		}
	}
	// �x���V�e�B�l���̓`�����l���A�t�^�[�^�b�`�l���̓R���g���[���`�F���W�l�̕`��(0�`64�`127)
	if (pPianoRollFrame->GetCurGraphKind () == 1 ||
		pPianoRollFrame->GetCurGraphKind () == 2 ||
		pPianoRollFrame->GetCurGraphKind () >= 4) {
		for (i = 0; i <= 8; i++) {
			strText.Format (_T("%d"), CLIP (0, (i * 16), 127));
			long y = pPianoRollFrame->VeltoY (i * 16);
			rcText.top = y - 8;
			rcText.bottom = y + 8;
			pDC->DrawText (strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	// �s�b�`�x���h�l�̕`��(-8192�`0�`8191)
	else if (pPianoRollFrame->GetCurGraphKind () == 3) {
		for (i = 0; i <= 8; i++) {
			strText.Format (_T("%d"), CLIP (-8192, (i - 4) *  2048, 8191));
			long y = pPianoRollFrame->VeltoY (i * 16);
			rcText.top = y - 8;
			rcText.bottom = y + 8;
			pDC->DrawText (strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	// �e���|�̕`��(0�`256)
	else if (pPianoRollFrame->GetCurGraphKind () == 0) {
		for (i = 0; i <= 8; i++) {
			strText.Format (_T("%d"), CLIP (0, (i * 32), 256));
			long y = pPianoRollFrame->VeltoY (i * 16);
			rcText.top = y - 8;
			rcText.bottom = y + 8;
			pDC->DrawText (strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	// �I��͈͂̔��](�L���v�^�|���̂�)
	if (GetCapture () == this) {
		long lDownY = m_ptMouseDown.y;//pPianoRollFrame->KeytoY (m_lDownKey);
		long lCurY = m_ptMouseMove.y;//pPianoRollFrame->KeytoY (m_lCurKey);
		long lTop = __min (lDownY, lCurY) - 1;
		long lBottom = __max (lDownY, lCurY);
		long w = rcClient.right;
		CRect rect (0, lTop, w, lBottom);
		pDC->SetROP2 (R2_NOT);
		pDC->Rectangle (&rect);
		pDC->SetROP2 (R2_COPYPEN);
	}
	pDC->SelectObject (pOldPen);

}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
BOOL CPianoRollVelScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// �L�[�������ꂽ��
void CPianoRollVelScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	switch (nChar) {
	// ��
	case VK_UP:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ��
	case VK_DOWN:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// Delete�L�[
	case VK_DELETE:
		// �w�ҏW(E)�x-�w�폜�x���s (20090823�ǉ�)
		PostMessage (WM_COMMAND, ID_EDIT_DELETE, NULL);
		break;
	}
	return;
}

// �}�E�X���{�^�������ꂽ�� 
void CPianoRollVelScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
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
	

	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
	point += CSize (0, pPianoRollFrame->GetVelScrollPos ());

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_SELECT_DESELECT));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());

	// ���I���C�x���g�̑I������(Shift��������Ă��Ȃ��ꍇ����Ctrl��������Ă��Ȃ��ꍇ�̂�)
	if ((nFlags & MK_SHIFT) == 0 && (nFlags & MK_CONTROL) == 0) {
		pSekaijuDoc->SelectNoObject (pCurHistoryUnit);
	}

	m_ptMouseDown = m_ptMouseMove = point;
	m_nMouseDownFlags = m_nMouseMoveFlags = nFlags;

	m_lOldTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseDown.y);
	m_lDownTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
	m_lCurTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
	m_lOldVel = pPianoRollFrame->YtoVel (m_ptMouseDown.y);
	m_lDownVel = pPianoRollFrame->YtoVel (point.y);
	m_lCurVel = pPianoRollFrame->YtoVel (point.y);
	m_lOldPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseDown.y);
	m_lDownPitchBend = pPianoRollFrame->YtoPitchBend (point.y);
	m_lCurPitchBend = pPianoRollFrame->YtoPitchBend (point.y);


	SetTimer (0x21, 55, NULL);
	SetCapture ();
	Invalidate ();

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�������ꂽ��  
void CPianoRollVelScaleView::OnRButtonDown (UINT nFlags, CPoint point) {
}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CPianoRollVelScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
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

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
	point += CSize (0, pPianoRollFrame->GetVelScrollPos ());

	if (GetCapture () == this) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
		KillTimer (0x21);
		ReleaseCapture ();

		long lMinTempoBPM = __min (m_lDownTempoBPM, m_lCurTempoBPM);
		long lMaxTempoBPM = __max (m_lDownTempoBPM, m_lCurTempoBPM);
		long lMinVel = __min (m_lDownVel, m_lCurVel);
		long lMaxVel = __max (m_lDownVel, m_lCurVel);
		long lMinPitchBend = __min (m_lDownPitchBend, m_lCurPitchBend);
		long lMaxPitchBend = __max (m_lDownPitchBend, m_lCurPitchBend);
		long i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			// ���̃g���b�N�����̏ꍇ
			if (pPianoRollFrame->IsTrackVisible (i)) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// �e���|�C�x���g�̏ꍇ
					if (MIDIEvent_IsTempo (pMIDIEvent)) {
						// �e���|�����̏ꍇ
						if (pPianoRollFrame->IsGraphVisible (0)) {
							long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
							long lTempoBPM = 60000000 / lTempo;
							if (lMinTempoBPM <= lTempoBPM && lTempoBPM <= lMaxTempoBPM) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// �m�[�g�C�x���g�̏ꍇ
					else if ((MIDIEvent_IsNoteOn (pMIDIEvent) ||
						MIDIEvent_IsNoteOff (pMIDIEvent)) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						// �x���V�e�B�����̏ꍇ
						if (pPianoRollFrame->IsGraphVisible (1)) {
							long lVel = MIDIEvent_GetVelocity (pMIDIEvent);
							if (lMinVel <= lVel && lVel <= lMaxVel) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// �`�����l���A�t�^�[�^�b�`�C�x���g�̏ꍇ
					else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
						// �`�����l���A�t�^�[�^�b�`�����̏ꍇ
						if (pPianoRollFrame->IsGraphVisible (2)) {
							long lVel = MIDIEvent_GetValue (pMIDIEvent);
							if (lMinVel <= lVel && lVel <= lMaxVel) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// �s�b�`�x���h�C�x���g�̏ꍇ
					else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
						// �s�b�`�x���h�����̏ꍇ
						if (pPianoRollFrame->IsGraphVisible (3)) {
							long lPitchBend = MIDIEvent_GetValue (pMIDIEvent);
							if (lMinPitchBend <= lPitchBend && lPitchBend <= lMaxPitchBend) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
					// �R���g���[���`�F���W�̏ꍇ
					else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
						long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
						// �w��i���o�[(CC#)�̃R���g���[���`�F���W�����̏ꍇ
						if (pPianoRollFrame->IsGraphVisible (4 + lNumber)) {
							long lVel = MIDIEvent_GetValue (pMIDIEvent);
							if (lMinVel <= lVel && lVel <= lMaxVel) {
								if (pSekaijuDoc->IsEventSelected (pMIDIEvent) == 0) {
									MIDIEvent* pCloneEvent = 
										pSekaijuDoc->SelectEvent 
										(pMIDIEvent, 1, pCurHistoryUnit);
									ASSERT (pCloneEvent);
									pMIDIEvent = pCloneEvent;
								}
							}
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
void CPianoRollVelScaleView::OnRButtonUp (UINT nFlags, CPoint point) {
}

// �}�E�X���������ꂽ�Ƃ�
void CPianoRollVelScaleView::OnMouseMove (UINT nFlags, CPoint point) {
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

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
	point += CSize (0, pPianoRollFrame->GetVelScrollPos ());

	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		m_lCurTempoBPM = pPianoRollFrame->YtoTempoBPM (point.y);
		m_lDownTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseDown.y);
		m_lOldTempoBPM = pPianoRollFrame->YtoTempoBPM (m_ptMouseMove.y);
		m_lCurVel = pPianoRollFrame->YtoVel (point.y);
		m_lDownVel = pPianoRollFrame->YtoVel (m_ptMouseDown.y);
		m_lOldVel = pPianoRollFrame->YtoVel (m_ptMouseMove.y);
		m_lCurPitchBend = pPianoRollFrame->YtoPitchBend (point.y);
		m_lDownPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseDown.y);
		m_lOldPitchBend = pPianoRollFrame->YtoPitchBend (m_ptMouseMove.y);
		if (m_lOldTempoBPM != m_lCurTempoBPM ||
			m_lOldVel != m_lCurVel ||
			m_lOldPitchBend != m_lCurPitchBend) {
			Invalidate (TRUE);
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;


}

// �}�E�X���{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CPianoRollVelScaleView::OnLButtonDblClk (UINT nFlags, CPoint point) {
}

// �}�E�X�E�{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CPianoRollVelScaleView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}


// �^�C�}�[��
void CPianoRollVelScaleView::OnTimer (UINT nIDEvent) {	

	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	if (nIDEvent == 0x21) {
		if (GetCapture () == this) {
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (0, pPianoRollFrame->GetVelScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				long lOldVelScrollPos = pPianoRollFrame->GetVelScrollPos ();
				if (m_ptMouseMove.y < rcClient.top) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y >= rcClient.bottom) {
					pPianoRollFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pPianoRollFrame->m_wndVelScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldVelScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}


// �}�E�X�z�C�[�����񂳂ꂽ��
void CPianoRollVelScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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
		long lVelScrollPos = pPianoRollFrame->GetVelScrollPos ();
		long lVelZoom = pPianoRollFrame->GetKeyZoom ();
		lVelScrollPos -= lVelZoom * lDelta / WHEELDELTA;
		pPianoRollFrame->SetVelScrollPos (lVelScrollPos);
	}
}

