//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃg���b�N�^�C���r���[�N���X
// (C)2002-2011 ���[�Ղ�MIDI�Ղ낶�����Ɓ^����
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
#include "MusicalScoreTrackTimeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CMusicalScoreTrackTimeView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CMusicalScoreTrackTimeView, CSekaijuView)
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

//-----------------------------------------------------------------------------
// �\�z�Ɣj��
//-----------------------------------------------------------------------------

// �R���X�g���N�^
CMusicalScoreTrackTimeView::CMusicalScoreTrackTimeView () {
	m_lCurTime = 0;
	m_lOldTime = 0;
	m_lOldY1 = 0;
	m_lOldY2 = 0;
	m_bOldDraw = TRUE;
	m_pTempEvent = NULL;
	m_lTempMode = 0;
//	m_lMouseDownTime = 0;
//	m_lMouseMoveTime = 0;
//	m_lMouseDownKey = 0;
//	m_lMouseMoveKey = 0;
	m_pLastEvent = NULL;
}

// �f�X�g���N�^
CMusicalScoreTrackTimeView::~CMusicalScoreTrackTimeView () {
}

//-----------------------------------------------------------------------------
// �I�y���[�V����
//-----------------------------------------------------------------------------

// �m�[�g��`���擾����(�߁X�p�~�\��)
CRect CMusicalScoreTrackTimeView::GetNoteRect (MIDIEvent* pNoteOnEvent) {
	ASSERT (pNoteOnEvent);
	ASSERT (FALSE);
	// ���̊֐��͎g��Ȃ��ł��������B
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	if (!MIDIEvent_IsNoteOn (pNoteOnEvent)) {
		return CRect (0,0,0,0);
	}
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTime = MIDIEvent_GetTime (pNoteOnEvent);
	long lKey = MIDIEvent_GetKey (pNoteOnEvent);
	long lDuration = MIDIEvent_GetDuration (pNoteOnEvent);
	long lsf, lmi;
	MIDIData_FindKeySignature (pMIDIData, lTime, &lsf, &lmi);
	long lKeySignature = lsf | (lmi << 8);
	long lTrackIndex = 2; // ��
	long x = pMusicalScoreFrame->TimetoX (lTime);
	long w = pMusicalScoreFrame->TimetoX (lDuration);
	long y = pMusicalScoreFrame->TrackIndexKeytoY (lTrackIndex, lKey, lKeySignature);
	long h = pMusicalScoreFrame->GetTrackZoom ();
	return CRect (x, y - h, x + w, y);
}

// ������`���擾����(�����܂��Ⴍ������)
// �g�p�O�ɕK��MIDIData_CountTrack���Ăяo�����ƁB
CRect CMusicalScoreTrackTimeView::GetNoteRect (MusicalScoreNoteInfo* pNoteInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	long lNoteOnTime = pNoteInfo->m_lNoteOnTime;
	long lNoteOnMeasure = pNoteInfo->m_lNoteOnMeasure;
	MusicalScoreMeasureInfo* pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lNoteOnMeasure);
	long lKeySignature = pMeasureInfo->m_lKeySignature;
	MIDIEvent* pNoteOnEvent = pNoteInfo->m_pNoteOnEvent;
	MIDITrack* pTrack = MIDIEvent_GetParent (pNoteOnEvent);
	long lKey = MIDIEvent_GetKey (pNoteOnEvent);
	long lNoteX = pMusicalScoreFrame->TimetoX (lNoteOnTime);
	long lNoteY = pMusicalScoreFrame->TrackIndexKeytoY (pTrack->m_lTempIndex, lKey, lKeySignature);
	long rx = 4;
	long ry = 4;
	return CRect (lNoteX - rx, lNoteY - ry, lNoteX + rx, lNoteY + ry);	
}

// ���ʒu�̏c������
void CMusicalScoreTrackTimeView::EraseOldLine (CDC* pDC) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	if (rcClient.left <= m_lOldX && m_lOldX <= rcClient.right && m_bOldDraw == TRUE) {
		pDC->SetROP2 (R2_NOT);
		pDC->MoveTo (m_lOldX, m_lOldY1);
		pDC->LineTo (m_lOldX, m_lOldY2);
	}
}

// ���݈ʒu�̏c���`��
void CMusicalScoreTrackTimeView::DrawCurLine (CDC* pDC) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	long x = pMusicalScoreFrame->TimetoX (m_lCurTime);
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
BOOL CMusicalScoreTrackTimeView::ShowPopupMenu (CPoint ptMenu) {

	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (m_lTempMode == 0x1001);

	if (m_pTempEvent) {
		pSekaijuDoc->m_lTempTime = MIDIEvent_GetTime (m_pTempEvent);
		pSekaijuDoc->m_pTempTrack = m_pTempTrack; // 20110103�C��
		pSekaijuDoc->m_pTempEvent = m_pTempEvent;
	}
	else {
		pSekaijuDoc->m_lTempTime = pSekaijuDoc->m_lNewTime;
		pSekaijuDoc->m_pTempTrack = m_pTempTrack; // 20110103�C��
		pSekaijuDoc->m_pTempEvent = NULL;
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

	CMenu theMenu;
	VERIFY (theMenu.LoadMenu (IDR_POPUPMENU31));
	CMenu* pContextMenu = theMenu.GetSubMenu (0);
	pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		ptMenu.x, ptMenu.y, pMusicalScoreFrame);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lTempMode = 0x0000;

	return TRUE;
}

// �����܂��Ⴍ����`�悷��
void CMusicalScoreTrackTimeView::DrawTadpole 
	(CDC* pDC, long x, long y, long rx, long ry, long lFlags) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	if (lFlags & 0x00010000) {
		pt[0].x = ptCenter.x + rx;
		pt[0].y = ptCenter.y;
		pt[1].x = ptCenter.x + rx;
		pt[1].y = ptCenter.y - ry / 2;
		pt[2].x = ptCenter.x + rx / 2;
		pt[2].y = ptCenter.y - ry;
		pt[3].x = ptCenter.x;
		pt[3].y = ptCenter.y - ry;
		pt[4].x = ptCenter.x - rx;
		pt[4].y = ptCenter.y;
		pt[5].x = ptCenter.x - rx;
		pt[5].y = ptCenter.y + ry / 2;
		pt[6].x = ptCenter.x - rx / 2;
		pt[6].y = ptCenter.y + ry;
		pt[7].x = ptCenter.x;
		pt[7].y = ptCenter.y + ry;
		pt[8].x = ptCenter.x + rx;
		pt[8].y = ptCenter.y;
		pDC->Polyline (pt, 9); // ����
		pt[0].x = ptCenter.x;
		pt[0].y = ptCenter.y - ry;
		pt[1].x = ptCenter.x - rx / 2;
		pt[1].y = ptCenter.y - ry;
		pt[2].x = ptCenter.x - rx;
		pt[2].y = ptCenter.y - ry / 2;
		pt[3].x = ptCenter.x - rx;
		pt[3].y = ptCenter.y;
		pDC->Polygon (pt, 4); // ����
		pt[0].x = ptCenter.x + rx;
		pt[0].y = ptCenter.y;
		pt[1].x = ptCenter.x + rx;
		pt[1].y = ptCenter.y + ry / 2;
		pt[2].x = ptCenter.x + rx / 2;
		pt[2].y = ptCenter.y + ry;
		pt[3].x = ptCenter.x;
		pt[3].y = ptCenter.y + ry;
		pDC->Polygon (pt, 4); // ����
	}
	else {
		pt[0].x = ptCenter.x + rx;
		pt[0].y = ptCenter.y;
		pt[1].x = ptCenter.x + rx;
		pt[1].y = ptCenter.y - ry / 2;
		pt[2].x = ptCenter.x + rx / 2;
		pt[2].y = ptCenter.y - ry;
		pt[3].x = ptCenter.x;
		pt[3].y = ptCenter.y - ry;
		pt[4].x = ptCenter.x - rx;
		pt[4].y = ptCenter.y;
		pt[5].x = ptCenter.x - rx;
		pt[5].y = ptCenter.y + ry / 2;
		pt[6].x = ptCenter.x - rx / 2;
		pt[6].y = ptCenter.y + ry;
		pt[7].x = ptCenter.x;
		pt[7].y = ptCenter.y + ry;
		pt[8].x = ptCenter.x + rx;
		pt[8].y = ptCenter.y;
		pDC->Polygon (pt, 8); // ����
	}

}


// �g���L����`�悷��
void CMusicalScoreTrackTimeView::DrawGClef 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[21];
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y + ry;
	pt[1].x = ptCenter.x - rx;
	pt[1].y = ptCenter.y;
	pt[2].x = ptCenter.x - rx;
	pt[2].y = ptCenter.y - ry;
	pt[3].x = ptCenter.x + rx;
	pt[3].y = ptCenter.y - ry;
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y + ry;
	pt[6].x = ptCenter.x + rx;
	pt[6].y = ptCenter.y + ry * 2;
	pt[7].x = ptCenter.x - rx * 1;
	pt[7].y = ptCenter.y + ry * 2;
	pt[8].x = ptCenter.x - rx * 2;
	pt[8].y = ptCenter.y + ry;
	pt[9].x = ptCenter.x - rx * 2;
	pt[9].y = ptCenter.y - ry;
	pt[10].x = ptCenter.x;
	pt[10].y = ptCenter.y - ry * 4;
	pt[11].x = ptCenter.x + rx * 1;
	pt[11].y = ptCenter.y - ry * 5;
	pt[12].x = ptCenter.x + rx * 1;
	pt[12].y = ptCenter.y - ry * 8;
	pt[13].x = ptCenter.x;
	pt[13].y = ptCenter.y - ry * 6;
	pt[14].x = ptCenter.x;
	pt[14].y = ptCenter.y - ry * 4;
	pt[15].x = ptCenter.x + rx;
	pt[15].y = ptCenter.y + ry * 4;
	pt[16].x = ptCenter.x;
	pt[16].y = ptCenter.y + ry * 5;
	pt[17].x = ptCenter.x - rx * 1 / 2;
	pt[17].y = ptCenter.y + ry * 5;
	pDC->Polyline (pt, 18);
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y; 
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - ry; 
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y - ry; 
	pt[3].x = ptCenter.x + rx * 2;
	pt[3].y = ptCenter.y; 
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y - ry; 
	pt[5].x = ptCenter.x + rx;
	pt[5].y = ptCenter.y - ry * 2; 
	pt[6].x = ptCenter.x;
	pt[6].y = ptCenter.y - ry * 2; 
	pt[7].x = ptCenter.x - rx;
	pt[7].y = ptCenter.y - ry;
	pDC->Polygon (pt, 8);
	pt[0].x = ptCenter.x - rx * 2;
	pt[0].y = ptCenter.y - ry * 1;
	pt[1].x = ptCenter.x - rx * 2;
	pt[1].y = ptCenter.y - ry * 2;
	pt[2].x = ptCenter.x + rx * 1;
	pt[2].y = ptCenter.y - ry * 6;
	pt[3].x = ptCenter.x + rx * 1;
	pt[3].y = ptCenter.y - ry * 5;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y - ry * 6; 
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - ry * 7; 
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y - ry * 8;
	pt[3].x = ptCenter.x + rx;
	pt[3].y = ptCenter.y - ry * 7;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x - rx * 1;
	pt[0].y = ptCenter.y + ry * 9 / 2;
	pt[1].x = ptCenter.x - rx * 1 / 2;
	pt[1].y = ptCenter.y + ry * 4;
	pt[2].x = ptCenter.x;
	pt[2].y = ptCenter.y + ry * 9 / 2;
	pt[3].x = ptCenter.x - rx * 1 / 2;
	pt[3].y = ptCenter.y + ry * 5;
	pDC->Polygon (pt, 4);
}

// �w���L����`�悷��
void CMusicalScoreTrackTimeView::DrawFClef 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x - rx * 2;
	pt[1].y = ptCenter.y;
	pt[2].x = ptCenter.x - rx * 2;
	pt[2].y = ptCenter.y - ry;
	pt[3].x = ptCenter.x - rx;
	pt[3].y = ptCenter.y - ry * 2;
	pt[4].x = ptCenter.x + rx;
	pt[4].y = ptCenter.y - ry * 2;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y - ry;
	pt[6].x = ptCenter.x + rx * 2;
	pt[6].y = ptCenter.y + ry;
	pt[7].x = ptCenter.x;
	pt[7].y = ptCenter.y + ry * 4;
	pt[8].x = ptCenter.x - rx * 2;
	pt[8].y = ptCenter.y + ry * 6;
	pDC->Polyline (pt, 9);
	pt[0].x = ptCenter.x - rx * 2;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x - rx * 3 / 2;
	pt[1].y = ptCenter.y - ry * 1 / 2;
	pt[2].x = ptCenter.x - rx;
	pt[2].y = ptCenter.y;
	pt[3].x = ptCenter.x - rx * 3 / 2;
	pt[3].y = ptCenter.y + ry * 1 / 2;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y - ry * 1;
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y + ry * 1;
	pt[3].x = ptCenter.x;
	pt[3].y = ptCenter.y + ry * 4;
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y + ry * 1;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y - ry * 1;
	pt[6].x = ptCenter.x + rx * 1;
	pt[6].y = ptCenter.y - ry * 2;
	pDC->Polygon (pt, 7);
	pt[0].x = ptCenter.x + rx * 3 + rx / 2;
	pt[0].y = ptCenter.y - ry * 1;
	pt[1].x = ptCenter.x + rx * 3;
	pt[1].y = ptCenter.y - ry * 1 - ry / 2;
	pt[2].x = ptCenter.x + rx * 3 - rx / 2;
	pt[2].y = ptCenter.y - ry * 1;
	pt[3].x = ptCenter.x + rx * 3;
	pt[3].y = ptCenter.y - ry * 1 + ry / 2;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x + rx * 3 + rx / 2;
	pt[0].y = ptCenter.y + ry * 1;
	pt[1].x = ptCenter.x + rx * 3;
	pt[1].y = ptCenter.y + ry * 1 - ry / 2;
	pt[2].x = ptCenter.x + rx * 3 - ry / 2;
	pt[2].y = ptCenter.y + ry * 1;
	pt[3].x = ptCenter.x + rx * 3;
	pt[3].y = ptCenter.y + ry * 1 + ry / 2;
	pDC->Polygon (pt, 4);
}

// ��̕`��
void CMusicalScoreTrackTimeView::DrawFlat 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y - ry;
	pt[3].x = ptCenter.x + rx / 2;
	pt[3].y = ptCenter.y;
	pt[4].x = ptCenter.x - rx / 2;
	pt[4].y = ptCenter.y + ry;
	pt[5].x = ptCenter.x - rx / 2;
	pt[5].y = ptCenter.y - ry * 4;
	pDC->Polyline (pt, 6);
}

// �i�`�������̕`��
void CMusicalScoreTrackTimeView::DrawNatural
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x - rx / 2;
	pt[1].y = ptCenter.y + ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y + ry * 3 / 4;
	pDC->Polyline (pt, 3);
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y - ry * 3 / 4;
	pt[1].x = ptCenter.x + rx / 2;
	pt[1].y = ptCenter.y - ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y + ry * 2;
	pDC->Polyline (pt, 3);
};


// #�̕`��
void CMusicalScoreTrackTimeView::DrawSharp
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y + ry + ry / 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y + ry - ry / 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y - ry + ry / 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y - ry - ry / 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x - rx / 2;
	pt[1].y = ptCenter.y + ry * 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x + rx / 2;
	pt[0].y = ptCenter.y - ry * 2;
	pt[1].x = ptCenter.x + rx / 2;
	pt[1].y = ptCenter.y + ry * 2;
	pDC->Polyline (pt, 2);
};

// �����⏕����`�悷��
void CMusicalScoreTrackTimeView::DrawHorzAuxiliaryLine
	(CDC* pDC, long x, long y, long r, long lFlags) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[2];
	pt[0].x = ptCenter.x - r;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x + r;
	pt[1].y = ptCenter.y;
	pDC->Polyline (pt, 2);
}

// �t�_��`�悷��
void CMusicalScoreTrackTimeView::DrawDot
	(CDC* pDC, long x, long y, long rx, long ry, long lFlags) {
	POINT ptCenter;
	ptCenter.x = x + rx * 2;
	ptCenter.y = y - ry / 2;
	POINT pt[4];
	pt[0].x = ptCenter.x - 1;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y - 1;
	pt[2].x = ptCenter.x + 1;
	pt[2].y = ptCenter.y;
	pt[3].x = ptCenter.x;
	pt[3].y = ptCenter.y + 1;
	pDC->Polygon (pt, 4);

}

// ���^�C��`�悷��
void CMusicalScoreTrackTimeView::DrawTieHalf
	(CDC* pDC, long x1, long x2, long y, long rx, long ry, long lFlags) {
	POINT ptCenter;
	ptCenter.x;
	ptCenter.y;
	POINT pt[3];
	pt[0].x = (x2 > x1 ? x1 + rx * 1 : x1 - rx * 1);
	pt[0].y = y + ry * 1;
	pt[1].x = (x2 > x1 ? x1 + rx * 2 : x1 - rx * 2);
	pt[1].y = y + ry * 2;
	pt[2].x = x2;
	pt[2].y = y + ry * 2;
	pDC->Polyline (pt, 3);

}

// �c�_��`�悷��
void CMusicalScoreTrackTimeView::DrawPole (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[2];
	pt[0].x = x1 + rx;
	pt[0].y = y1;
	pt[1].x = x1 + rx;
	pt[1].y = y2;
	pDC->Polyline (pt, 2);
}

// �P�����`�悷��
void CMusicalScoreTrackTimeView::DrawSingleFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[4];
	pt[0].x = x1 + rx;
	pt[0].y = y2;
	pt[1].x = x1 + rx * 3;
	pt[1].y = y2 + ry * 2;
	pt[2].x = x1 + rx * 3;
	pt[2].y = y2 + ry * 3;
	pt[3].x = x1 + rx;
	pt[3].y = y2 + ry;
	pDC->Polygon (pt, 4);
	pt[0].x = x1 + rx * 3;
	pt[0].y = y2 + ry * 3;
	pt[1].x = x1 + rx;
	pt[1].y = y2 + ry * 5;
	pDC->Polyline (pt, 2);
}

// �A������`�悷��
void CMusicalScoreTrackTimeView::DrawChainedFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[4];
	pt[0].x = x1 + rx;
	pt[0].y = y2;
	pt[1].x = x2 + rx;
	pt[1].y = y2;
	pt[2].x = x2 + rx;
	pt[2].y = y2 + ry;
	pt[3].x = x1 + rx;
	pt[3].y = y2 + ry;
	pDC->Polygon (pt, 4);
}

// 3�A������`�悷��(20110905�ǉ�)
void CMusicalScoreTrackTimeView::DrawTripletSign (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[3];
	pt[0].x = x1;
	pt[0].y = y2;
	pt[1].x = x1;
	pt[1].y = y1;
	pt[2].x = (x1 + x2) / 2 - 6;
	pt[2].y = y1;
	pDC->Polyline (pt, 3);
	pt[0].x = (x1 + x2) / 2 + 6;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y1;
	pt[2].x = x2;
	pt[2].y = y2;
	pDC->Polyline (pt, 3);
	CRect rcText ((x1 + x2) / 2 - 6, y1 - 6, (x1 + x2) / 2 + 6, y1 + 6);
	CString strText;
	strText.Format (_T("%d"), (lFlags & 0x000000FF));
	pDC->DrawText (strText, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


// ������`�悷��
void CMusicalScoreTrackTimeView::DrawNote
	(CDC* pDC, long lTrackIndex, MusicalScoreNoteInfo* pNoteInfo, long lFlags) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pNoteEvent = pNoteInfo->m_pNoteOnEvent;
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	ASSERT (0 <= lTrackIndex && lTrackIndex < pMusicalScoreFrame->GetTrackInfoCount ());
	VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex));
	long lTrackFlags = pTrackInfo->m_lFlags; //pMusicalScoreFrame->GetTrackFlags (lTrackIndex);
	long lKey = MIDIEvent_GetKey (pNoteEvent);
	long lDuration = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	long lNoteOnMeasure = pNoteInfo->m_lNoteOnMeasure;
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	ASSERT (0 <= lNoteOnMeasure && lNoteOnMeasure < pMusicalScoreFrame->GetMeasureInfoCount ());
	VERIFY (pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lNoteOnMeasure));
	long lKeySignature = pMeasureInfo->m_lKeySignature; // ���̏��߂̒����L��
	long lTimeSignature = pMeasureInfo->m_lTimeSignature; // ���̏��߂̔��q�L��
	long lLineNo = pMusicalScoreFrame->KeytoLineNo (lKey, lKeySignature);
	long lSF = pMusicalScoreFrame->KeytoSF (lKey, lKeySignature);
	long lnn = lTimeSignature & 0xFF;
	long ldd = (lTimeSignature & 0xFF00) >> 8;

	long x = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOnTime);
	long y = 0;
	long rx = 4;
	long ry = 4;
	long i = 0;

	// �����⏕���̕`��
	switch (lTrackFlags & 0x0000000F) {
	case 1: // �g���L��
		if (lLineNo >= 47) {
			for (i = 47; i <= lLineNo; i += 2) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		else if (lLineNo <= 35) {
			for (i = 35; i >= lLineNo; i -= 2) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		break;
	case 2: // �w���L��
		if (lLineNo >= 35) {
			for (i = 35; i <= lLineNo; i += 2) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		else if (lLineNo <= 23) {
			for (i = 23; i >= lLineNo; i -= 2) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		break;
	case 3: // �啈�\
		if (lLineNo >= 47) {
			for (i = 47; i <= lLineNo; i += 2) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		else if (lLineNo == 35) {
			y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 35);
			pDC->MoveTo (x - rx * 2, y);
			pDC->LineTo (x + rx * 2, y);
		}
		else if (lLineNo <= 23) {
			for (i = 23; i >= lLineNo; i -= 2) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		break;
	}
	// �Վ��L���̕`��
	y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lLineNo);
	switch (lSF) {
	case 0:
		break;
	case 2:
		DrawFlat (pDC, x - 4 * rx, y, rx, ry);
		DrawFlat (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 3:
		DrawFlat (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 4:
		DrawNatural (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 5:
		DrawSharp (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 6:
		DrawSharp (pDC, x - 2 * rx, y, rx, ry);
		DrawSharp (pDC, x - 4 * rx, y, rx, ry);
		break;
	}

	
	// �����܂��Ⴍ���̕`��
	y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lLineNo);
	long lFlags2 = 0;
	if (lDuration >= lTimeResolution * 2) {
		lFlags2 |= 0x00010000; // ����
	}
	if (lDuration < lTimeResolution * lnn * 4 / ldd) {
		lFlags2 |= 0x00100000; // �c������
	}
	DrawTadpole (pDC, x, y, rx, ry, lFlags2);

	// �t�_�̕`��
	if (lDuration == lTimeResolution * 3 ||
		lDuration == lTimeResolution * 3 / 2 ||
		lDuration == lTimeResolution * 3 / 4 ||
		lDuration == lTimeResolution * 3 / 8) {
		DrawDot (pDC, x, y, rx, ry, lFlags2);
	}

	// ���^�C�̕`��
	long x2 = 0;
	long lPreWidth = 0;
	long lPostWidth = 0;
	switch (pNoteInfo->m_lFlags & 0x0000000F) {
	case 1: // ��_
		x2 = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOffTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 2: // _��
		x2 = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOnTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 3: // _��_
		x2 = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOffTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOnTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 4: // ��_|
		x2 = pMusicalScoreFrame->MeasuretoX (pNoteInfo->m_lNoteOffMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 5: // |_��
		x2 = pMusicalScoreFrame->MeasuretoX (pNoteInfo->m_lNoteOnMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 6: // _��_|
		x2 = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOnTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = pMusicalScoreFrame->MeasuretoX (pNoteInfo->m_lNoteOffMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 7: // |_��_
		x2 = pMusicalScoreFrame->MeasuretoX (pNoteInfo->m_lNoteOnMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOffTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 8: // |_�S����_|
		x2 = pMusicalScoreFrame->MeasuretoX (pNoteInfo->m_lNoteOnMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = pMusicalScoreFrame->MeasuretoX (pNoteInfo->m_lNoteOffMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	}

	// �_�̕`��(20110905�C��)
	long lPoleKey = lKey;
	MusicalScoreNoteGroupInfo* pNoteGroupInfo = (MusicalScoreNoteGroupInfo*)(pNoteInfo->m_pNoteGroupInfo);
	if (pNoteGroupInfo) { // �����̏ꍇ
		lPoleKey = pNoteGroupInfo->m_lMaxKey;
	}
	long lLineNo2 = pMusicalScoreFrame->KeytoLineNo (lPoleKey, lKeySignature);
	long y2 = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lLineNo2);
	if (lDuration < lTimeResolution * 4) {
		DrawPole (pDC, x, x2, y, y2 - ry * 7, rx, ry, lFlags2);
	}

	// ���̕`��
	if (lDuration < lTimeResolution && lDuration != lTimeResolution * 2 / 3) {
		// �O���[�v���̉����̎������P��̏ꍇ
		if (pNoteGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime == pNoteGroupInfo->m_pLastNoteInfo->m_lNoteOnTime) {
			// 8�������A�t�_8�������A3�A8�������A16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 2 ||
				lDuration == lTimeResolution * 3 / 4 ||
				lDuration == lTimeResolution / 3 ||
				lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawSingleFlag (pDC, x, x2, y, y2 - ry * 7, rx, ry, lFlags2);
			}
			// 16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawSingleFlag (pDC, x, x2, y, y2 - ry * 5, rx, ry, lFlags2);
			}
			// 32�������A3�A32������
			if (lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawSingleFlag (pDC, x, x2, y, y2 - ry * 3, rx, ry, lFlags2);
			}
		}
		// �O���[�v���̉����������̎����ɂ܂�����ꍇ
		else {
			long x1[3]; // �A�������W[1�i��,2�i��,3�i��]
			long x2[3]; // �A���E���W[1�i��,2�i��,3�i��]
			// �O���[�v���̍ŏ��̎����̉����ł���ꍇ
			if (pNoteGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime == pNoteInfo->m_lNoteOnTime) {
				x1[0] = x1[1] = x1[2] = x;
				x2[0] = x2[1] = x2[2] = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOffTime - lTimeResolution / 8);
			}
			// �O���[�v���̍Ō�̎����̉����ł���ꍇ
			else if (pNoteGroupInfo->m_pLastNoteInfo->m_lNoteOnTime == pNoteInfo->m_lNoteOnTime) {
				x1[0] = x1[1] = x1[2] = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOnTime - lTimeResolution / 8);
				x2[0] = x2[1] = x2[2] = x;
			}
			// �O���[�v���̓r���̎����̉����ł���ꍇ
			else {
				x1[0] = x1[1] = x1[2] = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOnTime - lTimeResolution / 8);
				x2[0] = x2[1] = x2[2] = pMusicalScoreFrame->TimetoX (pNoteInfo->m_lNoteOffTime - lTimeResolution / 8);
				// �O�̎����̉������擾
				MusicalScoreNoteInfo* pPrevNoteInfo = pNoteInfo;
				long lPrevDuration = 0;
				while (pPrevNoteInfo) {
					if (pPrevNoteInfo->m_lNoteOnTime < pNoteInfo->m_lNoteOnTime) {
						break;
					}
					pPrevNoteInfo = pPrevNoteInfo->m_pPrevNoteInfo;
				}
				if (pPrevNoteInfo && pPrevNoteInfo->m_pNoteGroupInfo == pNoteGroupInfo) {
					lPrevDuration = pPrevNoteInfo->m_lNoteOffTime - pPrevNoteInfo->m_lNoteOnTime;
				}
				// ���̎����̉������擾
				MusicalScoreNoteInfo* pNextNoteInfo = pNoteInfo;
				long lNextDuration = 0;
				while (pNextNoteInfo) {
					if (pNextNoteInfo->m_lNoteOnTime > pNoteInfo->m_lNoteOnTime) {
						break;
					}
					pNextNoteInfo = pNextNoteInfo->m_pNextNoteInfo;
				}
				if (pNextNoteInfo && pNextNoteInfo->m_pNoteGroupInfo == pNoteGroupInfo) {
					lNextDuration = pNextNoteInfo->m_lNoteOffTime - pNextNoteInfo->m_lNoteOnTime;
				}
				// 1�i�ڂ̘A��������
				if (lDuration == lTimeResolution / 2 ||
					lDuration == lTimeResolution * 3 / 4 ||
					lDuration == lTimeResolution / 3 ||
					lDuration == lTimeResolution / 4 ||
					lDuration == lTimeResolution * 3 / 8 ||
					lDuration == lTimeResolution / 6 ||
					lDuration == lTimeResolution / 8 ||
					lDuration == lTimeResolution / 12) {
					if (lNextDuration == 0) {
						x2[0] = x;	
					}
					else if (lPrevDuration == 0) {
						x1[0] = x;
					}
				}
				// 2�i�ڂ̘A��������
				if (lDuration == lTimeResolution / 4 ||
					lDuration == lTimeResolution * 3 / 8 ||
					lDuration == lTimeResolution / 6 ||
					lDuration == lTimeResolution / 8 ||
					lDuration == lTimeResolution / 12) {
					if (lNextDuration >= lDuration * 2 || lNextDuration == 0) {
						x2[1] = x;	
					}
					else if (lPrevDuration >= lDuration * 2 || lPrevDuration == 0) {
						x1[1] = x;
					}
				}
				// 3�i�ڂ̘A��������
				if (lDuration == lTimeResolution / 8 ||
					lDuration == lTimeResolution / 12) {
					if (lNextDuration >= lDuration * 2 || lNextDuration == 0) {
						x2[2] = x;
					}
					else if (lPrevDuration >= lDuration * 2 || lPrevDuration == 0) {
						x1[2] = x;
					}
				}
			}
			// �A��1�i��
			// 8�������A�t�_8�������A3�A8�������A16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 2 ||
				lDuration == lTimeResolution * 3 / 4 ||
				lDuration == lTimeResolution / 3 ||
				lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawChainedFlag (pDC, x1[0], x2[0], y, y2 - ry * 7, rx, ry, lFlags2);
			}
			// �A��2�i�ڕ`��
			// 16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawChainedFlag (pDC, x1[1], x2[1], y, y2 - ry * 5, rx, ry, lFlags2);
			}
			// �A��3�i�ڕ`��
			// 32�������A3�A32������
			if (lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawChainedFlag (pDC, x1[2], x2[2], y, y2 - ry * 3, rx, ry, lFlags2);
			}
		}
	}

	// 3�A�����̕`��(20110905�ǉ�)
	if (pNoteInfo->m_pTripletGroupInfo) {
		MusicalScoreTripletGroupInfo* pTripletGroupInfo = 
			(MusicalScoreTripletGroupInfo*)(pNoteInfo->m_pTripletGroupInfo);
		if (pTripletGroupInfo->m_pFirstNoteInfo == pNoteInfo) {
			long x1 = pMusicalScoreFrame->TimetoX (pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime);
			long x2 = pMusicalScoreFrame->TimetoX (pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOnTime);
			long lMinLineNo = pMusicalScoreFrame->KeytoLineNo (pTripletGroupInfo->m_lMinKey, lKeySignature);
			long y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lMinLineNo);
			long n12 = 12 * lTimeResolution / pTripletGroupInfo->m_lMinDur; // 12 * 3 or 12 * 6 or 12 * 12
			long lSpan = pTripletGroupInfo->m_lEndTime - pTripletGroupInfo->m_lBeginTime;
			if (lSpan >= 1) {
				if (12 * lTimeResolution / lSpan >= 1) {
					n12 /= (12 * lTimeResolution / lSpan);
				}
				else {
					n12 /= 12;
				}
			}
			else {
				n12 /= 12;
			}
			DrawTripletSign (pDC, x1, x2, y + 4 * ry, y + 3 * ry, rx, ry, n12);
		}
	}

}

// ���q�L���E�����L���̕`��
void CMusicalScoreTrackTimeView::DrawTimeAndKeySignature 
	(CDC* pDC, long lTrackIndex, long lTime) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (0 <= lTrackIndex && lTrackIndex < pMusicalScoreFrame->GetTrackInfoCount ());
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex));
	long lTrackFlags = pTrackInfo->m_lFlags;//pMusicalScoreFrame->GetTrackFlags (lTrackIndex);
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	long x = pMusicalScoreFrame->MeasuretoX (lMeasure);
	long y;// = pMusicalScoreFrame->TrackIndexKeytoY (lTrackIndex, lKey, lKeySignature);
	long rx = 4; //pMusicalScoreFrame->GetTrackZoom ();
	long ry = 4;
	long lGCrefSharpLineNo[7] = {45, 42, 46, 43, 40, 44, 41};
	long lFCrefSharpLineNo[7] = {31, 28, 32, 29, 33, 30, 34};
	long lGCrefFlatLineNo[7] = {41, 44, 40, 43, 39, 42, 38};
	long lFCrefFlatLineNo[7] = {27, 30, 26, 29, 25, 28, 24};
	long lsf, lmi;
	MIDIData_FindKeySignature (pMIDIData, lTime, &lsf, &lmi);
	long lnn, ldd, lcc, lbb;
	MIDIData_FindTimeSignature (pMIDIData, lTime, &lnn, &ldd, &lcc, &lbb);
	CString strText1;
	CString strText2;
	strText1.Format (_T("%d"), lnn);
	strText2.Format (_T("%d"), (1 << ldd));
	CRect rcText1 (x + rx * 10 + rx * 2 * abs(lsf), 0, x + rx * 10 + rx * 2 * abs(lsf) + 24, 0);
	CRect rcText2 (x + rx * 10 + rx * 2 * abs(lsf), 0, x + rx * 10 + rx * 2 * abs(lsf) + 24, 0);
	long j;
	switch (lTrackFlags & 0x0000000F) {
	case 1: // �g���L��
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 39);
		DrawGClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 41);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	case 2: // �w���L��
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 31);
		DrawFClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 29);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	case 3: // �啈�\
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 39);
		DrawGClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lGCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 41);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 31);
		DrawFClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, lFCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = pMusicalScoreFrame->TrackIndexLineNotoY (lTrackIndex, 29);
		rcText1.top = y - ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y + ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	}

}



//-----------------------------------------------------------------------------
// �I�[�o�[���C�h
//-----------------------------------------------------------------------------

// ���_�̈ړ����I�[�o�[���C�h
void CMusicalScoreTrackTimeView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	pDC->SetWindowOrg (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
}

// �`��
void CMusicalScoreTrackTimeView::OnDraw (CDC* pDC) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penOctave (PS_SOLID, 1, RGB (128, 128, 255));
	CFont* pOldFont = NULL;
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	pDC->SetBkMode (TRANSPARENT);
	long x, y, lTime;
	long lVisibleLeftTime = pMusicalScoreFrame->GetVisibleLeftTime ();
	long lVisibleRightTime = pMusicalScoreFrame->GetVisibleRightTime ();
	long lVisibleTopTrack = pMusicalScoreFrame->GetVisibleTopTrack ();
	long lVisibleBottomTrack = pMusicalScoreFrame->GetVisibleBottomTrack ();
	long lLeftMeasure, lLeftBeat, lLeftTick;
	long lRightMeasure, lRightBeat, lRightTick;
	long lnn, ldd, lcc, lbb;
	long lUnitTick;

	// �ܐ��̕`��
	CPen* pOldPen = pDC->SelectObject (&penKey);
	long lTrackZoom = pMusicalScoreFrame->GetTrackZoom ();
	long rx = 4;
	long ry = 4;
	long i, j;
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) {
				ASSERT (0 <= i && i < pMusicalScoreFrame->GetTrackInfoCount ()); 
				MusicalScoreTrackInfo* pTrackInfo = NULL;
				VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i));
				long lTrackTop = pTrackInfo->m_lTop;
				long lTrackHeight = pTrackInfo->m_lHeight;
				long lTrackFlags = pTrackInfo->m_lFlags;
				long yc = (lTrackTop + lTrackHeight / 2) * lTrackZoom;
				long ii;
				switch (lTrackFlags & 0x0000000F) {
				case 0: // �\�����Ȃ�
					break;
				case 1: // �g���L��
				case 2: // �w���L��
					for (ii = -2; ii <= 2; ii++) {
						y = yc + ii * 2 * ry;
						pDC->MoveTo (rcClient.left, y);
						pDC->LineTo (rcClient.right, y);
					}
					break;
				case 3: // �啈�\
					for (ii = -5; ii <= -1; ii++) {
						y = yc + ii * 2 * ry;
						pDC->MoveTo (rcClient.left, y);
						pDC->LineTo (rcClient.right, y);
					}
					for (ii = 1; ii <= 5; ii++) {
						y = yc + ii * 2 * ry;
						pDC->MoveTo (rcClient.left, y);
						pDC->LineTo (rcClient.right, y);
					}
					break;
				}
			}
		}
		i++;
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
			x = pMusicalScoreFrame->MeasuretoX (i);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, rcClient.top);
			pDC->LineTo (x, rcClient.bottom);
			// �����̕`��
			pDC->SelectObject (&penBeat);
			lUnitTick = lTimeResolution * 4 / (1 << ldd);
			for (long j = 0; j < lnn; j++) {
				x = pMusicalScoreFrame->TimetoX (lTime + j * lUnitTick);
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
			x = pMusicalScoreFrame->TimetoX (lTime);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, rcClient.top);
			pDC->LineTo (x, rcClient.bottom);
		}
		pDC->SelectObject (pOldPen);
	}

	// ���q�L���E�����L���̕`��
	CPen theTrackPen;
	CBrush theTrackBrush;
	pOldFont = pDC->SelectObject (&(pMusicalScoreFrame->m_theTimeMeasureFont));
	pMIDITrack = pMIDIData->m_pFirstTrack;
	if (pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			long lTime = MIDIEvent_GetTime (pMIDIEvent);
			if (MIDIEvent_IsTimeSignature (pMIDIEvent) ||
				MIDIEvent_IsKeySignature (pMIDIEvent)) {
				if (lVisibleLeftTime <= lTime && lTime <= lVisibleRightTime) {
					i = 0;
					MIDITrack* pTempTrack;
					forEachTrack (pMIDIData, pTempTrack) {
						if (pMusicalScoreFrame->IsTrackVisible (i)) {
							long lColor = MIDITrack_GetForeColor (pTempTrack);
							pDC->SetTextColor (lColor);
							theTrackPen.CreatePen (PS_SOLID, 1, lColor);
							theTrackBrush.CreateSolidBrush (lColor);
							CPen* pOldPen = pDC->SelectObject (&theTrackPen);
							CBrush* pOldBrush = pDC->SelectObject (&theTrackBrush);
							DrawTimeAndKeySignature (pDC, i, lTime);
							pDC->SelectObject (pOldPen);
							pDC->SelectObject (pOldBrush);
							theTrackPen.DeleteObject ();
							theTrackBrush.DeleteObject ();
						}
						i++;
					}
				}
			}
		}
	}
	pDC->SelectObject (pOldFont);
	
	// �����̕`��(pNoteEvent�͎������Ă��邱�Ƃ�����̂�pNoteInfo�݂̂��Q�Ƃ��邱��)
	CPen penBar (PS_SOLID, 1, RGB (0, 0, 0));
	pDC->SelectObject (&penBar);
	pOldFont = pDC->SelectObject (&(pMusicalScoreFrame->m_theFont));
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) {
				long lColor = MIDITrack_GetForeColor (pMIDITrack);
				pDC->SetTextColor (lColor);
				CPen theSelectedPen;
				CBrush theSelectedBrush;
				theTrackPen.CreatePen (PS_SOLID, 1, lColor);
				theTrackBrush.CreateSolidBrush (lColor);
				theSelectedPen.CreatePen (PS_SOLID, 1, RGB (0, 0, 0));
				theSelectedBrush.CreateSolidBrush (RGB (0, 0, 0));
				MusicalScoreTrackInfo* pTrackInfo = (MusicalScoreTrackInfo*)(pMusicalScoreFrame->m_theTrackInfoArray.GetAt (i));
				long jMax = pTrackInfo->m_theNoteInfoArray.GetSize ();
				for (j = 0; j < jMax; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
					if (lVisibleLeftTime <= pNoteInfo->m_lNoteOnTime && pNoteInfo->m_lNoteOnTime <= lVisibleRightTime || 
						lVisibleRightTime <= pNoteInfo->m_lNoteOffTime && pNoteInfo->m_lNoteOffTime < lVisibleRightTime ||
						pNoteInfo->m_lNoteOnTime <= lVisibleLeftTime && lVisibleRightTime < pNoteInfo->m_lNoteOffTime) {
						CPen* pOldPen = pDC->SelectObject (pNoteInfo->m_lSelected ? &theSelectedPen : &theTrackPen);
						CBrush* pOldBrush = pDC->SelectObject (pNoteInfo->m_lSelected ? & theSelectedBrush : &theTrackBrush);
						DrawNote (pDC, i, pNoteInfo, 0x00000000);
						pDC->SelectObject (pOldPen);
						pDC->SelectObject (pOldBrush);
					}
				}
				theTrackPen.DeleteObject ();
				theTrackBrush.DeleteObject ();
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);
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
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	Sleep (0);
}

// �r���[�̍X�V
void CMusicalScoreTrackTimeView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// �N���e�B�J���Z�N�V�����̓��b�N����Ă�����̂Ƃ���B
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		PostMessage (WM_TIMER, 0x11, NULL);
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

// �E�B���h�E�N���X�̕ύX
BOOL CMusicalScoreTrackTimeView::PreCreateWindow (CREATESTRUCT& cs) {
	CView::PreCreateWindow (cs);
	cs.lpszClass = AfxRegisterWndClass 
		(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
		NULL, // �f�t�H���g�J�[�\������
		(HBRUSH)::GetStockObject (WHITE_BRUSH), // �f�t�H���g�w�i�u���V
		NULL); // �f�t�H���g�A�C�R������
	return TRUE;
}




//-----------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//-----------------------------------------------------------------------------

// �E�B���h�E������
int CMusicalScoreTrackTimeView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	SetTimer (0x11, 55, NULL);
	return CSekaijuView::OnCreate (lpCreateStruct);
}

// �E�B���h�E�j����
void CMusicalScoreTrackTimeView::OnDestroy () {
	KillTimer (0x11);
	CSekaijuView::OnDestroy ();
}

// �^�C�}�[�Ăяo����
void CMusicalScoreTrackTimeView::OnTimer (UINT nIDEvent) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// ���t�E�L�^���̃J�����g�o�[�ړ�����
	if (nIDEvent == 0x11) {
		if (pSekaijuDoc->m_pMIDIData && pSekaijuDoc->m_pMIDIClock) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CDC* pDC = GetDC ();
			OnPrepareDC (pDC, NULL);
			CRect rcClient;
			GetClientRect (&rcClient);
			rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
			m_lCurTime = MIDIClock_GetTickCount (GetDocument ()->m_pMIDIClock);
			// �O��̎����ƌ��݂̎���������Ă���ꍇ�̂�
			if (m_lOldTime != m_lCurTime) {
				EraseOldLine (pDC);
				DrawCurLine (pDC);
			}
			// �y�[�W�̍X�V����уI�[�g�X�N���[��(�蓮�X�N���[�����͂��Ȃ�)
			if (pMusicalScoreFrame->m_bAutoPageUpdate == TRUE) {
				MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
				long lTimeMode, lTimeResolution;
				MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
				if (lTimeMode == MIDIDATA_TPQNBASE) {
					// ���݈ʒu���E���ւ͂ݏo�����ꍇ
					long lRightMeasure; long lRightBeat; long lRightTick; long lTempRightTime;
					MIDIData_BreakTime (GetDocument()->m_pMIDIData, pMusicalScoreFrame->GetVisibleRightTime (),
						&lRightMeasure, &lRightBeat, &lRightTick);
					MIDIData_MakeTime (GetDocument()->m_pMIDIData, lRightMeasure, 0, 0, &lTempRightTime);
					if (m_lCurTime >= lTempRightTime) {
						long lNewMeasure; long lNewBeat; long lNewTick;
						MIDIData_BreakTime (pMIDIData, m_lCurTime, &lNewMeasure, &lNewBeat, &lNewTick);
						//MIDIData_MakeTime (pMIDIData, lNewMeasure, 0, 0, &lTempRightTime);
						//int x = pMusicalScoreFrame->TimetoXMM (lTempRightTime) + 1;
						int x = pMusicalScoreFrame->MeasuretoX2 (lNewMeasure) + 1;
						pMusicalScoreFrame->SetTimeScrollPos (x);
					}
					// ���݈ʒu�������ւ͂ݏo�����ꍇ
					long lLeftMeasure; long lLeftBeat; long lLeftTick; long lTempLeftTime;
					MIDIData_BreakTime (GetDocument()->m_pMIDIData, pMusicalScoreFrame->GetVisibleLeftTime (),
						&lLeftMeasure, &lLeftBeat, &lLeftTick);
					MIDIData_MakeTime (GetDocument()->m_pMIDIData, lLeftMeasure, 0, 0, &lTempLeftTime);
					if (m_lCurTime < lTempLeftTime) {
						long lNewMeasure; long lNewBeat; long lNewTick;
						MIDIData_BreakTime (GetDocument()->m_pMIDIData, m_lCurTime, &lNewMeasure, &lNewBeat, &lNewTick);
						//MIDIData_MakeTime (GetDocument()->m_pMIDIData, lNewMeasure, 0, 0, &lTempLeftTime);
						//int x = pMusicalScoreFrame->TimetoXMM (lTempLeftTime) + 1;
						int x = pMusicalScoreFrame->MeasuretoX2 (lNewMeasure) + 1;
						pMusicalScoreFrame->SetTimeScrollPos (x);
					}
				}
				else {
					// ���݈ʒu���E���ւ͂ݏo�����ꍇ
					long lRightFrameNumber = pMusicalScoreFrame->GetVisibleRightTime () / lTimeResolution;
					long lTempRightTime = lRightFrameNumber * lTimeResolution;
					if (m_lCurTime >= lTempRightTime) {
						//lTempRightTime = (m_lCurTime / lTimeResolution) * lTimeResolution;
						//long x = pMusicalScoreFrame->TimetoXMM (lTempRightTime) + 1;
						long lNewFrameNumber = m_lCurTime / lTimeResolution;
						int x = pMusicalScoreFrame->MeasuretoX2 (lNewFrameNumber) + 1;
						pMusicalScoreFrame->SetTimeScrollPos (x);
					}
					// ���݈ʒu�������ւ͂ݏo�����ꍇ
					long lLeftFrameNumber = pMusicalScoreFrame->GetVisibleLeftTime () / lTimeResolution;
					long lTempLeftTime = lLeftFrameNumber * lTimeResolution;
					if (m_lCurTime < lTempLeftTime) {
						//lTempLeftTime = (m_lCurTime / lTimeResolution) * lTimeResolution;
						//long x = pMusicalScoreFrame->TimetoXMM (lTempLeftTime) + 1;
						long lNewFrameNumber = m_lCurTime / lTimeResolution;
						int x = pMusicalScoreFrame->MeasuretoX2 (lNewFrameNumber) + 1;
						pMusicalScoreFrame->SetTimeScrollPos (x);
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
			rcClient += CSize (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
			if (!rcClient.PtInRect (m_ptMouseMove)) {
				long lOldTimeScrollPos = pMusicalScoreFrame->GetTimeScrollPos ();
				long lOldTrackScrollPos = pMusicalScoreFrame->GetTrackScrollPos ();
				pSekaijuDoc->m_theCriticalSection.Lock ();
				if (m_ptMouseMove.x < rcClient.left) {
					pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.x > rcClient.right) {
					pMusicalScoreFrame->SendMessage (WM_HSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pMusicalScoreFrame->m_wndTimeScroll.GetSafeHwnd ()));
				}
				if (m_ptMouseMove.y < rcClient.top) {
					pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP,
						(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
				}
				else if (m_ptMouseMove.y > rcClient.bottom) {
					pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN,
						(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
				}
				WORD wX = (WORD)(m_ptMouseMove.x - lOldTimeScrollPos);
				WORD wY = (WORD)(m_ptMouseMove.y - lOldTrackScrollPos);
				PostMessage (WM_MOUSEMOVE, (WPARAM)m_nMouseMoveFlags, (LPARAM)((wY << 16) | wX));
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				::Sleep (0);
			}
		}
	}
}


// �L�[�������ꂽ��
void CMusicalScoreTrackTimeView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	long lOldKey = m_lMouseMoveKey;
	switch (nChar) {
	// ��
	case VK_UP:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEUP, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// ��
	case VK_DOWN:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_LINEDOWN, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageUp
	case VK_PRIOR:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEUP, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
	// PageDown
	case VK_NEXT:
		pMusicalScoreFrame->SendMessage (WM_VSCROLL, (WPARAM)SB_PAGEDOWN, 
			(LPARAM)(pMusicalScoreFrame->m_wndTrackScroll.GetSafeHwnd ()));
		this->SetFocus ();
		break;
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
	// DEL
	case VK_DELETE:
		// �L���v�^�[���Ȃ��
		if (GetCapture () == this) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			switch (m_lTempTool) {
			// �y��
			case ID_MUSICALSCORE_PEN:
				if ((m_lTempMode == 0x0101 || m_lTempMode == 0x0103) && m_pTempEvent != NULL) {
					ASSERT (m_pTempTrack);
					// �u�����Ƃ��Ă���m�[�g���͈ړ����̃m�[�g�폜
					if (m_lTempMode = 0x0101) {
						MIDIEvent_Delete (m_pTempEvent);
						m_pTempEvent = NULL;
					}
					else if (m_lTempMode = 0x0103) {
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
		break;
	// ESC
	case VK_ESCAPE:
		// �L���v�^�[���Ȃ��
		if (GetCapture () == this) {
			pSekaijuDoc->m_theCriticalSection.Lock ();
			CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			switch (m_lTempTool) {
			// �y��
			case ID_MUSICALSCORE_PEN:
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
		// �L���v�^�[���łȂ��ꍇ�̂�
		if (GetCapture () != this) {
			WORD wX = (WORD)(m_ptMouseMove.x - pMusicalScoreFrame->GetTimeScrollPos ());
			WORD wY = (WORD)(m_ptMouseMove.y - pMusicalScoreFrame->GetTrackScrollPos ());
			UINT nFlags = m_nMouseMoveFlags | MK_CONTROL;
			PostMessage (WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)((wY << 16) | wX));
		}
		break;
	}
	return;
}


// �L�[�������ꂽ��
void CMusicalScoreTrackTimeView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// Control
	case VK_CONTROL:
		// �L���v�^�[���łȂ��ꍇ�̂�
		if (GetCapture () != this) {
			WORD wX = (WORD)(m_ptMouseMove.x - pMusicalScoreFrame->GetTimeScrollPos ());
			WORD wY = (WORD)(m_ptMouseMove.y - pMusicalScoreFrame->GetTrackScrollPos ());
			UINT nFlags = m_nMouseMoveFlags & (~MK_CONTROL);
			PostMessage (WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)((wY << 16) | wX));
		}
		break;
	}
	return;
}

// �}�E�X���{�^�������ꂽ��  
void CMusicalScoreTrackTimeView::OnLButtonDown (UINT nFlags, CPoint point) {
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	point += sizWndOrg;
	ASSERT (m_pTempEvent == NULL);
	ASSERT (m_lTempMode == 0);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	CHistoryUnit* pCurHistoryUnit = NULL;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lsf, lmi;
	m_lTempTime = pMusicalScoreFrame->XtoTime (point.x);
	m_lMouseDownTime = m_lTempTime;
	m_lMouseMoveTime = m_lTempTime;
	MIDIData_FindKeySignature (pMIDIData, m_lTempTime, &lsf, &lmi);
	long lKeySignature = lsf | (lmi << 8);
	m_lTempTool = pMusicalScoreFrame->m_lCurTool;
	m_lTempChannel = pMusicalScoreFrame->GetCurChannel ();
	m_lTempChannel = CLIP (0, m_lTempChannel, 15);
	m_lTempVelocity = pMusicalScoreFrame->GetCurVelocity ();
	m_lTempVelocity = CLIP (1, m_lTempVelocity, 127);
	m_lTempDuration = pMusicalScoreFrame->GetCurDuration ();
	m_lTempDuration = CLIP (1, m_lTempDuration, 65535);
	m_lTempSnap = pMusicalScoreFrame->GetCurSnap ();
	m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
	m_pTempEvent = NULL;
	m_pLastEvent = NULL;
	BOOL bPtInNote = FALSE;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIOut* pMIDIOut = NULL;
	MIDIStatus* pMIDIStatus = NULL;
	CString strHistoryName;
	BYTE byMIDIMessage[3];
	long i = 0;
	switch (m_lTempTool) {
	// �y��
	case ID_MUSICALSCORE_PEN:
		// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ
		i = MIDIData_CountTrack (pMIDIData) - 1;
		forEachTrackInverse (pMIDIData, pTempTrack) {
			if (pMusicalScoreFrame->IsTrackVisible (i)) {
				long lPort = MIDITrack_GetOutputPort (pTempTrack);
				if (0 <= lPort && lPort < MAXMIDIOUTDEVICENUM) {
					pMIDIOut = pSekaijuApp->m_pMIDIOut[lPort];
					pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lPort];
				}
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
				long j;
				for (j = 0; j < lNumNoteInfo; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
					CRect rcNote = GetNoteRect (pNoteInfo);
					// �����̉�������N���b�N�����ꍇ
					if (rcNote.top <= point.y && point.y < rcNote.bottom) {
						if (rcNote.left <= point. x && point.x < rcNote.right) {
							// �t�H�[�}�b�g1��MIDI�f�[�^�ōŏ��̃g���b�N�Ƀm�[�g��u���̂��֎~
							if (lFormat == 1 && i == 0) {
								pSekaijuDoc->m_theCriticalSection.Unlock ();
								CString strMsg;
								VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
								AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
								pSekaijuDoc->m_theCriticalSection.Lock ();
								break;
							}
							pTempEvent = pNoteInfo->m_pNoteOnEvent;
							pMusicalScoreFrame->SetCurTrackIndex (i);
							pMusicalScoreFrame->SetCurVelocity (MIDIEvent_GetVelocity (pTempEvent));
							pMusicalScoreFrame->SetCurDuration (MIDIEvent_GetDuration (pTempEvent));
							pMusicalScoreFrame->SetCurChannel (MIDIEvent_GetChannel (pTempEvent));
							m_lTempTrackIndex = pMusicalScoreFrame->GetCurTrackIndex ();
							m_pTempTrack = pTempTrack;//pSekaijuDoc->GetTrack (i);
							m_lTempOutputPort = MIDITrack_GetOutputPort (pTempTrack);
							m_lTempVelocity = pMusicalScoreFrame->GetCurVelocity ();
							m_lTempDuration = pMusicalScoreFrame->GetCurDuration ();
							m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack);
							if (m_lTempChannel < 0 || m_lTempChannel >= 16) {
								m_lTempChannel = pMusicalScoreFrame->GetCurChannel ();
							}
							m_lTempTimeNoteOn = MIDIEvent_GetTime (pTempEvent);
							m_lTempMode = 0x0103;
							m_lTempKey = MIDIEvent_GetKey (pTempEvent);
							m_lMouseDownKey = m_lTempKey;
							m_lMouseMoveKey = m_lTempKey;
							VERIFY (strHistoryName.LoadString (IDS_MODIFY_NOTE));
							VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
							pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
							MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
							if (pLastEvent) {
								if (MIDIEvent_IsEndofTrack (pLastEvent)) {
									pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
									m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent);
								}
							}
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							m_pTempEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent);
							ASSERT (MIDIEvent_GetParent (m_pTempEvent));
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
							pMusicalScoreFrame->UpdateNoteInfoArray (m_lTempTrackIndex, m_pTempTrack);
							pMusicalScoreFrame->UpdateNoteGroupInfoArray (m_lTempTrackIndex, m_pTempTrack);
							Invalidate ();
							::SetCursor (pSekaijuApp->m_hCursorSizeAll);
							pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
							SetTimer (0x21, 55, NULL);
							SetCapture ();
							break;
						}
					}
				}
				if (m_pTempEvent) {
					break;
				}
			}
			i--;
		}
		// �����Ȃ��Ƃ���Ƀ}�E�X���u���ꂽ�ꍇ
		if (m_pTempEvent == NULL) {
			m_lTempTime = (m_lTempTime / m_lTempSnap) * m_lTempSnap;
			m_lTempTimeNoteOn = m_lTempTime;
			m_lTempTrackIndex = pMusicalScoreFrame->YtoTrackIndex (point.y);
			if (m_lTempTrackIndex < 0 || m_lTempTrackIndex >= MIDIData_CountTrack (pMIDIData)) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			m_lTempKey = pMusicalScoreFrame->TrackIndexYtoKey (m_lTempTrackIndex, point.y, lKeySignature);
			m_lMouseDownKey = m_lTempKey;
			m_lMouseMoveKey = m_lTempKey;
			if (lFormat == 1 && m_lTempTrackIndex == 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				break;
			}
			pMusicalScoreFrame->SetCurTrackIndex (m_lTempTrackIndex);
			m_lTempOutputPort = MIDITrack_GetOutputPort (m_pTempTrack);
			pMusicalScoreFrame->SetCurChannel (m_lTempChannel);
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
			pMusicalScoreFrame->UpdateNoteInfoArray (m_lTempTrackIndex, m_pTempTrack);
			pMusicalScoreFrame->UpdateNoteGroupInfoArray (m_lTempTrackIndex, m_pTempTrack);
			Invalidate ();
			pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
		}
		break;
	// ��
	case ID_MUSICALSCORE_LINE:
		break;

	// �����S��
	case ID_MUSICALSCORE_ERASER:
		VERIFY (strHistoryName.LoadString (IDS_DELETE_NOTE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		m_pTempEvent = NULL;
		m_lTempMode = 0x0301;
		// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ
		i = MIDIData_CountTrack (pMIDIData) - 1;
		forEachTrackInverse (pMIDIData, pTempTrack) {
			if (pMusicalScoreFrame->IsTrackVisible (i)) {
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
				long j;
				for (j = 0; j < lNumNoteInfo; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
					CRect rcNote = GetNoteRect (pNoteInfo);
					// �����̉�������N���b�N�����ꍇ
					if (rcNote.top <= point.y && point.y < rcNote.bottom) {
						if (rcNote.left <= point. x && point.x < rcNote.right) {
							// �t�H�[�}�b�g1��MIDI�f�[�^�ōŏ��̃g���b�N�Ƀm�[�g�������̂��֎~
							if (lFormat == 1 && i == 0) {
								pSekaijuDoc->m_theCriticalSection.Unlock ();
								CString strMsg;
								VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DELETE_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
								AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
								pSekaijuDoc->m_theCriticalSection.Lock ();
								break;
							}
							VERIFY (pTempEvent = pNoteInfo->m_pNoteOnEvent);
							pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
							MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
							pMusicalScoreFrame->UpdateNoteInfoArray (i, pTempTrack);
							pMusicalScoreFrame->UpdateNoteGroupInfoArray (i, pTempTrack);
							Invalidate ();
							bPtInNote = TRUE;
							break;
						}
					}
				}
			}
			if (bPtInNote) {
				break;
			}
			i--;
		}
		pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
		SetTimer (0x21, 55, NULL);
		SetCapture ();
		break;

	// �I��
	case ID_MUSICALSCORE_SELECT:
		bPtInNote = FALSE;
		// �����̑I������Ă���m�[�g�̏�Ƀ}�E�X���u���ꂽ�����ׂ�
		i = MIDIData_CountTrack (pMIDIData) - 1;
		forEachTrackInverse (pMIDIData, pTempTrack) {
			if (pMusicalScoreFrame->IsTrackVisible (i)) {
				long lPort = MIDITrack_GetOutputPort (pTempTrack);
				if (0 <= lPort && lPort < MAXMIDIOUTDEVICENUM) {
					pMIDIOut = pSekaijuApp->m_pMIDIOut[lPort];
					pMIDIStatus = pSekaijuApp->m_pMIDIStatus[lPort];
				}
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
				long j;
				for (j = 0; j < lNumNoteInfo; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
					CRect rcNote = GetNoteRect (pNoteInfo);
					// �����̉�������N���b�N�����ꍇ
					if (rcNote.top <= point.y && point.y < rcNote.bottom) {
						if (rcNote.left <= point. x && point.x < rcNote.right) {
							VERIFY (pTempEvent = pNoteInfo->m_pNoteOnEvent);
							if (pSekaijuDoc->IsEventSelected (pTempEvent)) {
								ASSERT (MIDIEvent_IsNoteOn (pTempEvent));
								ASSERT (MIDIEvent_IsNote (pTempEvent));
								if (pTempEvent->m_pNextCombinedEvent) {
									m_pTempEvent = pTempEvent;
									break;
								}
							}
						}
					}
				}
				if (m_pTempEvent) {
					break;
				}
			}
			i--;
		}
		// �����̑I������Ă���m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ(���ʖ��͈ړ����[�h)
		if (m_pTempEvent) {
			pMusicalScoreFrame->SetCurTrackIndex (i);
			pMusicalScoreFrame->SetCurVelocity (MIDIEvent_GetVelocity (m_pTempEvent));
			pMusicalScoreFrame->SetCurDuration (MIDIEvent_GetDuration (m_pTempEvent));
			pMusicalScoreFrame->SetCurChannel (MIDIEvent_GetChannel (m_pTempEvent));
			m_lTempTrackIndex = pMusicalScoreFrame->GetCurTrackIndex ();
			m_pTempTrack = pSekaijuDoc->GetTrack (m_lTempTrackIndex);
			m_lTempVelocity = pMusicalScoreFrame->GetCurVelocity ();
			m_lTempDuration = pMusicalScoreFrame->GetCurDuration ();
			m_lTempChannel = MIDITrack_GetOutputChannel (m_pTempTrack);
			if (m_lTempChannel < 0 || m_lTempChannel >= 16) {
				m_lTempChannel = pMusicalScoreFrame->GetCurChannel ();
			}
			m_lTempTimeNoteOn = MIDIEvent_GetTime (m_pTempEvent);
			m_lTempKey = MIDIEvent_GetKey (m_pTempEvent);
			m_lMouseDownKey = m_lTempKey;
			m_lMouseMoveKey = m_lTempKey;
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
					if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
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
									// �Ή�����m�[�g�I�t�C�x���g��m_lUser1�Ƀ}�E�X�������ꂽ�Ƃ��̃L�[���A
									// �Ή�����m�[�g�I�t�C�x���g��m_lUser2�j�}�E�X�������ꂽ�Ƃ��̃^�C����ێ��B
									ASSERT (MIDIEvent_IsNote (pCloneEvent));
									ASSERT (pCloneEvent->m_pNextCombinedEvent);
									pCloneEvent->m_pNextCombinedEvent->m_lUser1 = MIDIEvent_GetKey (pCloneEvent);
									pCloneEvent->m_pNextCombinedEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
									pSekaijuDoc->SetEventSelected (pCloneEvent, 1);
									m_theTempSelectedEventArray.Add (pCloneEvent);
									if (pTempEvent == m_pTempEvent) {
										m_pTempEvent = pCloneEvent;
									}
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
					if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
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
									// �Ή�����m�[�g�I�t�C�x���g��m_lUser1�Ƀ}�E�X�������ꂽ�Ƃ��̃L�[��ێ��B
									// �Ή�����m�[�g�I�t�C�x���g��m_lUser2�j�}�E�X�������ꂽ�Ƃ��̃^�C����ێ��B
									ASSERT (MIDIEvent_IsNote (pCloneEvent));
									ASSERT (pCloneEvent->m_pNextCombinedEvent);
									pCloneEvent->m_pNextCombinedEvent->m_lUser1 = MIDIEvent_GetKey (pCloneEvent);
									pCloneEvent->m_pNextCombinedEvent->m_lUser2 = MIDIEvent_GetTime (pCloneEvent);
									m_theTempSelectedEventArray.Add (pCloneEvent);
									if (pTempEvent == m_pTempEvent) {
										m_pTempEvent = pCloneEvent;
									}
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
			ASSERT (m_pTempEvent);
			ASSERT (m_pTempTrack);
			ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
			// �I������Ă���C�x���g������g���b�N�����ĕ`��
			i = 0;
			forEachTrack (pMIDIData, pTempTrack) {
				if (pMusicalScoreFrame->IsTrackVisible (i)) {
					pMusicalScoreFrame->UpdateNoteInfoArray (i, pTempTrack);
					pMusicalScoreFrame->UpdateNoteGroupInfoArray (i, pTempTrack);
				}
				i++;
			}
			Invalidate ();
			pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
			
		}
		// �����Ȃ��Ƃ���Ƀ}�E�X���u���ꂽ�ꍇ(�I��͈̓��[�h)
		else {
			// ���I���C�x���g�̑I������(Shift��������Ă��Ȃ��ꍇ����Ctrl��������Ă��Ȃ��ꍇ�̂�)
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
			i = 0;
			forEachTrack (pMIDIData, pTempTrack) {
				if (pMusicalScoreFrame->IsTrackVisible (i)) {
					pMusicalScoreFrame->UpdateNoteInfoArray (i, pTempTrack);
					pMusicalScoreFrame->UpdateNoteGroupInfoArray (i, pTempTrack);
				}
				i++;
			}
			Invalidate ();
			pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
			SetTimer (0x21, 55, NULL);
			SetCapture ();
		}
		break;

	// �X�N���u
	case ID_MUSICALSCORE_SPEAKER:
		if (pSekaijuApp->m_bPlaying) {
			pMusicalScoreFrame->SendMessage (WM_COMMAND, (WPARAM)ID_CONTROL_PLAY, (LPARAM)0);
		}
		pSekaijuApp->SetPlayPosition (pSekaijuDoc, m_lTempTime);
		pSekaijuApp->SendDifferentStatus (SDS_ALL);
		m_lTempMode = 0x0501;
		::SetCursor (pSekaijuApp->m_hCursorSpeaker);
		pMusicalScoreFrame->m_bAutoPageUpdate = FALSE;
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
void CMusicalScoreTrackTimeView::OnRButtonDown (UINT nFlags, CPoint point) {
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

	CPoint ptMenu (point);
	ClientToScreen (&ptMenu);

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	point += sizWndOrg;

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);

	BYTE byMIDIMessage[4];
	memset (byMIDIMessage, 0, sizeof (byMIDIMessage));
	// �}�E�X�L���v�^�[���͗Վ��L��(�����グ����)�����t������B
	if (GetCapture () == this) {
		MIDIOut* pMIDIOut = NULL;
		MIDIStatus* pMIDIStatus = NULL;
		if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
		}
		// �c�[���ʂ̑���
		switch (m_lTempTool) {
		// �y��
		case ID_MUSICALSCORE_PEN:
			// �����ړ���
			if (m_lTempMode == 0x0101 || m_lTempMode == 0x0103) {
				ASSERT (m_pTempEvent != NULL);
				ASSERT (m_pTempTrack != NULL);
				ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
				ASSERT (0 <= m_lTempTrackIndex && m_lTempTrackIndex < MAXMIDITRACKNUM);
				ASSERT (lFormat != 0 && m_lTempTrackIndex != 0);
				long lOldTime = m_lMouseMoveTime;
				long lsf, lmi;
				MIDIData_FindKeySignature (pMIDIData, lOldTime, &lsf, &lmi);
				long lKeySignature = lsf | (lmi << 8);
				long lOldKey = m_lMouseMoveKey;
				long lOldLineNo = pMusicalScoreFrame->KeytoLineNo (lOldKey, lKeySignature);
				// �����������ꍇ
				long lNewKeyUp = CLIP (0, lOldKey + 1, 127);
				long lNewLineNoUp = pMusicalScoreFrame->KeytoLineNo (lNewKeyUp, lKeySignature);
				// �����������ꍇ
				long lNewKeyDown = CLIP (0, lOldKey - 1, 127);
				long lNewLineNoDown = pMusicalScoreFrame->KeytoLineNo (lNewKeyDown, lKeySignature);
				// �ܐ�����̈ʒu���ς��Ȃ��ق����̗p
				long lNewKey = lOldKey;
				if (lNewLineNoUp == lOldLineNo) {
					lNewKey = lNewKeyUp;
				}
				else if (lNewLineNoDown == lOldLineNo) {
					lNewKey = lNewKeyDown;
				}
				// �L�[���ς�����ꍇ�̂�
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
					// �V�����L�[�ݒ�
					MIDIEvent_SetKey (m_pTempEvent, CLIP (0, lNewKey, 127));
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
					m_lMouseMoveKey = lNewKey;
				}
			}
			break;
		// �I��
		case ID_MUSICALSCORE_SELECT:
			// �ړ����͕��ʒ�
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				ASSERT (m_pTempEvent != NULL);
				ASSERT (m_pTempTrack != NULL);
				ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
				ASSERT (0 <= m_lTempTrackIndex && m_lTempTrackIndex < MAXMIDITRACKNUM);
				ASSERT (lFormat != 0 && m_lTempTrackIndex != 0);
				long lOldTime = m_lMouseMoveTime;
				long lsf, lmi;
				MIDIData_FindKeySignature (pMIDIData, lOldTime, &lsf, &lmi);
				long lKeySignature = lsf | (lmi << 8);
				long lDownKey = m_lMouseDownKey;
				long lOldKey = m_lMouseMoveKey;
				long lOldLineNo = pMusicalScoreFrame->KeytoLineNo (lOldKey, lKeySignature);
				// �����������ꍇ
				long lNewKeyUp = CLIP (0, lOldKey + 1, 127);
				long lNewLineNoUp = pMusicalScoreFrame->KeytoLineNo (lNewKeyUp, lKeySignature);
				// �����������ꍇ
				long lNewKeyDown = CLIP (0, lOldKey - 1, 127);
				long lNewLineNoDown = pMusicalScoreFrame->KeytoLineNo (lNewKeyDown, lKeySignature);
				// �ܐ�����̈ʒu���ς��Ȃ��ق����̗p
				long lNewKey = lOldKey;
				if (lNewLineNoUp == lOldLineNo) {
					lNewKey = lNewKeyUp;
				}
				else if (lNewLineNoDown == lOldLineNo) {
					lNewKey = lNewKeyDown;
				}
				// �L�[���ς�����ꍇ�̂�
				if (lOldKey != lNewKey) {
					long lDeltaKey = lNewKey - lDownKey;
					long lTempSelectedEventArrayCount = m_theTempSelectedEventArray.GetSize ();
					long j;
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
					// �V�����L�[�ݒ�
					for (j = 0; j < lTempSelectedEventArrayCount; j++) {
						MIDIEvent* pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (j);
						long lTempKey = pTempEvent->m_pNextCombinedEvent->m_lUser1;
						long lTargetKey = lTempKey + lDeltaKey;
						lTargetKey = CLIP (0, lTargetKey, 127);
						MIDIEvent_SetKey (pTempEvent, lTargetKey);
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
					m_lMouseMoveKey = lNewKey;
				}
			}
			break;
		}
	}

	// �}�E�X�L���v�^�[���łȂ��ꍇ
	else {
	
		ASSERT (m_pTempEvent == NULL);
		ASSERT (m_lTempMode == 0);

		CHistoryUnit* pCurHistoryUnit = NULL;
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		long lFormat = MIDIData_GetFormat (pMIDIData);
		long lsf, lmi;
		m_lTempTime = pMusicalScoreFrame->XtoTime (point.x);
		m_lMouseDownTime = m_lTempTime;
		m_lMouseMoveTime = m_lTempTime;
		MIDIData_FindKeySignature (pMIDIData, m_lTempTime, &lsf, &lmi);
		long lKeySignature = lsf | (lmi << 8);
		m_lTempTool = pMusicalScoreFrame->m_lCurTool;
		m_lTempChannel = pMusicalScoreFrame->GetCurChannel ();
		m_lTempChannel = CLIP (0, m_lTempChannel, 15);
		m_lTempVelocity = pMusicalScoreFrame->GetCurVelocity ();
		m_lTempVelocity = CLIP (1, m_lTempVelocity, 127);
		m_lTempDuration = pMusicalScoreFrame->GetCurDuration ();
		m_lTempDuration = CLIP (1, m_lTempDuration, 65535);
		m_lTempSnap = pMusicalScoreFrame->GetCurSnap ();
		m_lTempSnap = CLIP (1, m_lTempSnap, MIDIData_GetTimeResolution (pMIDIData));
		m_pTempEvent = NULL;
		m_pTempTrack = NULL;
		// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ
		MIDITrack* pTempTrack = NULL;
		long i = 0;
		forEachTrack (pMIDIData, pTempTrack) {
			if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
				long j = 0;
				for (j = 0; j < lNumNoteInfo; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt(j));
					CRect rcNote = GetNoteRect (pNoteInfo);
					// �����̉����̏�Ƀ}�E�X���u���ꂽ�ꍇ
					if (rcNote.left<= point.x && point.x < rcNote.right) {
						if (rcNote.top <= point.y && point.y < rcNote.bottom) {
							m_lTempOutputPort = MIDITrack_GetOutputPort (pTempTrack);
							m_lTempVelocity = pMusicalScoreFrame->GetCurVelocity ();
							m_lTempDuration = pMusicalScoreFrame->GetCurDuration ();
							m_lTempChannel = MIDITrack_GetOutputChannel (pTempTrack);
							if (m_lTempChannel < 0 || m_lTempChannel >= 16) {
								m_lTempChannel = pMusicalScoreFrame->GetCurChannel ();
							}
							m_lTempTimeNoteOn = MIDIEvent_GetTime (pNoteInfo->m_pNoteOnEvent);
							m_pTempEvent = pNoteInfo->m_pNoteOnEvent;
							m_pTempTrack = pTempTrack;
							ASSERT (m_pTempEvent);
							ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
							break;
						}
					}
				}
			}
			i++;
		}
		// �ǂ̉�����ɂ��������Ă��Ȃ��ꍇ(20110207�ǉ�)
		if (m_pTempTrack == NULL && m_pTempEvent == NULL) {
			i = pMusicalScoreFrame->YtoTrackIndex (point.y);
			if (0 <= i && i < pMusicalScoreFrame->GetTrackInfoCount ()) {
				m_pTempTrack = pSekaijuDoc->GetTrack (i);
			}
		}

		m_lTempMode = 0x1001;
		
		ShowPopupMenu (ptMenu);
		
		m_pTempEvent = NULL;
		m_lTempMode = 0x0000;
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
	Invalidate ();


}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CMusicalScoreTrackTimeView::OnLButtonUp (UINT nFlags, CPoint point) {
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

	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	point += sizWndOrg;

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lDownTime = m_lMouseDownTime;
	long lOldTime = m_lMouseMoveTime;
	long lNewTime = lOldTime;
	long lsf, lmi;
	MIDIData_FindKeySignature (pMIDIData, lNewTime, &lsf, &lmi);
	long lKeySignature = lsf | (lmi << 8);
	long lDownKey = m_lMouseDownKey;
	long lOldKey = m_lMouseMoveKey;
	long lNewKey = lOldKey;
	long i = 0;
	// �L���v�^�[���Ȃ��
	if (GetCapture () == this) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
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
		case ID_MUSICALSCORE_PEN:
			ASSERT (m_pTempTrack);
			ASSERT (m_pTempEvent);
			ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
			ASSERT (0 <= m_lTempTrackIndex && m_lTempTrackIndex < MAXMIDITRACKNUM);
			ASSERT (lFormat != 0 && m_lTempTrackIndex != 0);
			ASSERT (m_lTempMode);
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����}�������͉����ړ����̏ꍇ�Ɍ���
			if (m_lTempMode == 0x0101 || m_lTempMode == 0x0103) {
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
		case ID_MUSICALSCORE_LINE:
			break;
		// �����S��
		case ID_MUSICALSCORE_ERASER:
			pSekaijuDoc->SetModifiedFlag (TRUE);
			break;
		// �I��
		case ID_MUSICALSCORE_SELECT:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����ړ������͉������ʒ��̏ꍇ
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				ASSERT (m_pTempTrack);
				ASSERT (m_pTempEvent);
				ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
				ASSERT (0 <= m_lTempTrackIndex && m_lTempTrackIndex < MAXMIDITRACKNUM);
				ASSERT (lFormat != 0 && m_lTempTrackIndex != 0);
				// �e�I���C�x���g�̃}�E�X�_�E�����̃L�[���ƃ^�C�������폜
				long j;
				long lTempSelectedEventCount = m_theTempSelectedEventArray.GetSize ();
				for (j = 0; j < lTempSelectedEventCount; j++) {
					MIDIEvent* pTempEvent = (MIDIEvent*)(m_theTempSelectedEventArray.GetAt (j));
					pTempEvent->m_pNextCombinedEvent->m_lUser1 = 0;
					pTempEvent->m_pNextCombinedEvent->m_lUser2 = 0;
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
					if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
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
				// �V�K�I��
				long lStartTime = __min (lDownTime, lNewTime);
				long lEndTime = __max (lDownTime, lNewTime);
				i = MIDIData_CountTrack (pMIDIData) - 1;
				forEachTrackInverse (pMIDIData, pTempTrack) {
					if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
						MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
						long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
						long lDownKey = pMusicalScoreFrame->TrackIndexYtoKey (i, m_ptMouseDown.y, lKeySignature);
						long lNewKey = pMusicalScoreFrame->TrackIndexYtoKey (i, point.y, lKeySignature);
						long lStartKey = __min (lDownKey, lNewKey);
						long lEndKey = __max (lDownKey, lNewKey);
						long j = 0;
						for (j = 0; j < lNumNoteInfo; j++) {
							MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt(j));
							VERIFY (pTempEvent = pNoteInfo->m_pNoteOnEvent);
							// ���̉����ɑΉ�����m�[�g�I���C�x���g���܂��I������Ă��Ȃ��ꍇ�̂�
							if (MIDIEvent_GetParent (pTempEvent) == pTempTrack) {
								// �������I��͈͓��ɂ���ꍇ
								long lTime = pNoteInfo->m_lNoteOnTime;
								if (lStartTime <= lTime && lTime <= lEndTime) {
									// �L�[���I��͈͓��ɂ���ꍇ
									long lKey = MIDIEvent_GetKey (pTempEvent);
									if (lStartKey <= lKey && lKey <= lEndKey) {
										// �C�x���g�𗚗�u����Ɉڂ��A�V�����C�x���g��I������B
										MIDIEvent* pCloneEvent = pSekaijuDoc->SelectEvent 
											(pTempEvent, 1, pCurHistoryUnit);
									}
								}
							}
						}
					}
					i--;
				}
			}
			break;
		// �X�N���u
		case ID_MUSICALSCORE_SPEAKER:
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
void CMusicalScoreTrackTimeView::OnRButtonUp (UINT nFlags, CPoint point) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

}

// �}�E�X���������ꂽ�Ƃ�
void CMusicalScoreTrackTimeView::OnMouseMove (UINT nFlags, CPoint point) {
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
	
	CClientDC dc (this);
	OnPrepareDC (&dc, NULL);
	CSize sizWndOrg (pMusicalScoreFrame->GetTimeScrollPos (), pMusicalScoreFrame->GetTrackScrollPos ());
	point += sizWndOrg;
	CRect rcClient;
	GetClientRect (&rcClient);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;

	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lDownTime = m_lMouseDownTime;
	long lOldTime = m_lMouseMoveTime;
	long lNewTime = pMusicalScoreFrame->XtoTime (point.x);
	long lsf, lmi;
	MIDIData_FindKeySignature (pMIDIData, lNewTime, &lsf, &lmi);
	long lKeySignature = lsf | (lmi << 8);
	long lDownKey = m_lMouseDownKey;
	long lOldKey = m_lMouseMoveKey;
	long lNewKey = pMusicalScoreFrame->TrackIndexYtoKey (m_lTempTrackIndex, point.y, lKeySignature);
	
	BYTE byMIDIMessage[3];
	// �L���v�^�[���Ȃ��
	if (GetCapture () == this) {
		MIDIOut* pMIDIOut = NULL;
		MIDIStatus* pMIDIStatus = NULL;
		if (0 <= m_lTempOutputPort && m_lTempOutputPort < MAXMIDIOUTDEVICENUM) {
			pMIDIOut = pSekaijuApp->m_pMIDIOut[m_lTempOutputPort];
			pMIDIStatus = pSekaijuApp->m_pMIDIStatus[m_lTempOutputPort];
		}
		long i = 0;
		// �c�[���ʂ̑���
		switch (m_lTempTool) {
		case ID_MUSICALSCORE_PEN:
			ASSERT (m_pTempTrack);
			ASSERT (m_pTempEvent);
			ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
			ASSERT (0 <= m_lTempTrackIndex && m_lTempTrackIndex < MAXMIDITRACKNUM);
			ASSERT (lFormat != 0 && m_lTempTrackIndex != 0);
			ASSERT (m_lTempMode);
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����ړ���
			if (m_lTempMode == 0x0101 || m_lTempMode == 0x0103) {
				// �L�[���ς�����ꍇ���͎������ς�����ꍇ
				if (lOldKey != lNewKey || 
					(lOldTime / m_lTempSnap) * m_lTempSnap != 
					(lNewTime / m_lTempSnap) * m_lTempSnap) {
					// �L�[���ς�����ꍇ�̂�
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
						// �V�����L�[�ݒ�
						MIDIEvent_SetKey (m_pTempEvent, CLIP (0, lNewKey, 127));
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
					// �������ς�����ꍇ�̂�
					if ((lOldTime / m_lTempSnap) * m_lTempSnap != 
						(lNewTime / m_lTempSnap) * m_lTempSnap) {
						// �V���������ݒ�
						long lDeltaTime =
							(lNewTime / m_lTempSnap) * m_lTempSnap - 
							(lOldTime / m_lTempSnap) * m_lTempSnap;
						long lCurTime = MIDIEvent_GetTime (m_pTempEvent);
						long lTempTime = CLIP (0, lCurTime + lDeltaTime, 0x7FFF0000);
						MIDIEvent_SetTime (m_pTempEvent, lTempTime);
					}
					pMusicalScoreFrame->UpdateNoteInfoArray (m_lTempTrackIndex, m_pTempTrack);
					pMusicalScoreFrame->UpdateNoteGroupInfoArray (m_lTempTrackIndex, m_pTempTrack);
					Invalidate ();
				}
			}
			break;
		// ��
		case ID_MUSICALSCORE_LINE:
			break;
		// �����S��
		case ID_MUSICALSCORE_ERASER:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �����̃m�[�g�̏�Ƀ}�E�X���u���ꂽ�ꍇ */
			i = MIDIData_CountTrack (pMIDIData) - 1;
			forEachTrackInverse (pMIDIData, pTempTrack) {
				if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
					MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
					long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
					long j;
					for (j = 0; j < lNumNoteInfo; j++) {
						MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
						CRect rcNote = GetNoteRect (pNoteInfo);
						// �����̉�������N���b�N�����ꍇ
						if (rcNote.top <= point.y && point.y < rcNote.bottom) {
							if (rcNote.left <= point. x && point.x < rcNote.right) {
								VERIFY (pTempEvent = pNoteInfo->m_pNoteOnEvent);
								pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
								MIDITrack_RemoveEvent (pTempTrack, pTempEvent);
								pMusicalScoreFrame->UpdateNoteInfoArray (i, pTempTrack);
								pMusicalScoreFrame->UpdateNoteGroupInfoArray (i, pTempTrack);
								Invalidate ();
								break;
							}
						}
					}
				}
				i--;
			}
			break;
		// �I��
		case ID_MUSICALSCORE_SELECT:
			pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
			// �ړ����͕��ʒ�
			if (m_lTempMode == 0x0402 || m_lTempMode == 0x0403) {
				ASSERT (m_pTempTrack);
				ASSERT (m_pTempEvent);
				ASSERT (MIDIEvent_GetParent (m_pTempEvent) == m_pTempTrack);
				ASSERT (0 <= m_lTempTrackIndex && m_lTempTrackIndex < MAXMIDITRACKNUM);
				ASSERT (lFormat != 0 && m_lTempTrackIndex != 0);
				long lTempSelectedEventArrayCount = m_theTempSelectedEventArray.GetSize ();
				long j;
				// �L�[���ς�����ꍇ���͎������ς�����ꍇ
				if (lOldKey != lNewKey ||
					(lOldTime / m_lTempSnap) * m_lTempSnap != 
					(lNewTime / m_lTempSnap) * m_lTempSnap) {
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
						for (j = 0; j < lTempSelectedEventArrayCount; j++) {
							MIDIEvent* pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (j);
							long lTempKey = pTempEvent->m_pNextCombinedEvent->m_lUser1;
							long lTargetKey = lTempKey + lDeltaKey;
							lTargetKey = CLIP (0, lTargetKey, 127);
							MIDIEvent_SetKey (pTempEvent, lTargetKey);
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
						for (j = 0; j < lTempSelectedEventArrayCount; j++) {
							MIDIEvent* pTempEvent = (MIDIEvent*)m_theTempSelectedEventArray.GetAt (j);
							long lTempTime = pTempEvent->m_pNextCombinedEvent->m_lUser2;
							long lTargetTime = lTempTime + lDeltaTime;
							lTargetTime = CLIP (0, lTargetTime, 0x7FFFFFFF);
							MIDIEvent_SetTime (pTempEvent, lTargetTime);
						}
					}
					i = 0;
					forEachTrack (pMIDIData, pTempTrack) {
						if (pMusicalScoreFrame->IsTrackVisible (i)) {
							pMusicalScoreFrame->UpdateNoteInfoArray (i, pTempTrack);
							pMusicalScoreFrame->UpdateNoteGroupInfoArray (i, pTempTrack);
						}
						i++;
					}
					Invalidate ();
				}
			}
			// �I��͈͈ړ���
			else if (m_lTempMode = 0x0401) {
				// ��B(�I����`�͂�����������͂�����Ȃ�)
				CRect rcRegion;
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
		case ID_MUSICALSCORE_SPEAKER:
			m_lTempTime = pMusicalScoreFrame->XtoTime (point.x);
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
		switch (pMusicalScoreFrame->m_lCurTool) {
		// �`��
		case ID_MUSICALSCORE_PEN:
			i = MIDIData_CountTrack (pMIDIData) - 1;
			forEachTrackInverse (pMIDIData, pMIDITrack) {
				if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
					MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
					long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
					long j;
					for (j = 0; j < lNumNoteInfo; j++) {
						MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
						CRect rcNote = GetNoteRect (pNoteInfo);
						// �����̉������ʉ߂����ꍇ
						if (rcNote.top <= point.y && point.y < rcNote.bottom) {
							if (rcNote.left <= point. x && point.x < rcNote.right) {
								::SetCursor (pSekaijuApp->m_hCursorSizeAll);
								bChanged = TRUE;
								break;
							}
						}
					}
				}
				i--;
			}
			if (bChanged == FALSE) {
				::SetCursor (pSekaijuApp->m_hCursorDraw);
			}
			break;
		// ��
		case ID_MUSICALSCORE_LINE:
			break;
		// �����S��
		case ID_MUSICALSCORE_ERASER:
			::SetCursor (pSekaijuApp->m_hCursorEraser);
			break;
		// �I��
		case ID_MUSICALSCORE_SELECT:
			i = MIDIData_CountTrack (pMIDIData) - 1;
			forEachTrackInverse (pMIDIData, pMIDITrack) {
				if (pMusicalScoreFrame->IsTrackVisible (i) && !(lFormat == 0 && i == 0)) {
					MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
					long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
					long j;
					for (j = 0; j < lNumNoteInfo; j++) {
						MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
						CRect rcNote = GetNoteRect (pNoteInfo);
						// �����̉������ʉ߂����ꍇ
						if (rcNote.top <= point.y && point.y < rcNote.bottom) {
							if (rcNote.left <= point. x && point.x < rcNote.right) {
								VERIFY (pTempEvent = pNoteInfo->m_pNoteOnEvent);
								if (pSekaijuDoc->IsEventSelected (pTempEvent)) {
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
				i--;
			}
			if (!bChanged) {
				::SetCursor (pSekaijuApp->m_hCursorSelect);
			}
			break;
		// �X�s�[�J�[
		case ID_MUSICALSCORE_SPEAKER:
			::SetCursor (pSekaijuApp->m_hCursorSpeaker);
			break;
		// �s��
		default:
			//::SetCursor (pSekaijuApp->m_hCursorArrow);
			break;
		}
	}
	m_lMouseMoveKey = lNewKey;
	m_lMouseMoveTime = lNewTime;
	m_ptMouseMove = point;
	m_nMouseMoveFlags = nFlags;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�z�C�[�����񂳂ꂽ��
void CMusicalScoreTrackTimeView::OnMouseWheel40 (UINT nFlags, CPoint point) {
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


