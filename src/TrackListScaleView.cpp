//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�X�P�[���r���[�N���X
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
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolbar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "TrackListScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CTrackListScaleView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CTrackListScaleView, CSekaijuView)
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CTrackListScaleView::CTrackListScaleView () {
}

// �f�X�g���N�^
CTrackListScaleView::~CTrackListScaleView () {
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �`��
void CTrackListScaleView::OnDraw (CDC* pDC) {
	CSekaijuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rcClient;
	GetClientRect (&rcClient);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (&rcClient, lColorBtnFace);

}

// �r���[�̍X�V
void CTrackListScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// �N���e�B�J���Z�N�V�����̓��b�N����Ă�����̂Ƃ���B
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// ���t�J�n�����ꍇ�A���A���^�C�����͊J�n�����ꍇ�A�ʒu�ړ������ꍇ
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) { // 20091224�ǉ�
			pTrackListFrame->m_bAutoPageUpdate = TRUE;
		}
	}
	// MIDI�f�[�^����MIDI�g���b�N����MIDI�C�x���g���ύX���ꂽ�ꍇ
	if ((lHint & SEKAIJUDOC_MIDIDATACHANGED) ||
		(lHint & SEKAIJUDOC_MIDITRACKCHANGED) ||
		(lHint & SEKAIJUDOC_MIDIEVENTCHANGED)) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		long lEndTime = MIDIData_GetEndTime (pMIDIData);
		pMainFrame->SetPositionScrollRange (0, lEndTime, TRUE);
		pTrackListFrame->RecalcRowScrollInfo ();
		pTrackListFrame->RecalcColumnScrollInfo ();
		pTrackListFrame->RecalcTimeScrollInfo ();
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �}�E�X�z�C�[�����񂳂ꂽ��
void CTrackListScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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
