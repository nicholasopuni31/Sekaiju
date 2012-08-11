//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�t���[���E�B���h�E�N���X
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
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "SekaijuFileDlg.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "EventListFrame.h"
#include "EventListPrintView.h"
#include "EventListScaleView.h"
#include "EventListIndexScaleView.h"
#include "EventListPropertyScaleView.h"
#include "EventListIndexPropertyView.h"
#include "common.h"


// �A���P�[�V�����̊Ď�
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �q�E�B���h�E�̃T�C�Y
#define EVENTLISTFRAME_SCALEHEIGHT 16
#define EVENTLISTFRAME_SCALEWIDTH 64
#define EVENTLISTFRAME_PROPERTYWIDTH 600
#define EVENTLISTFRAME_TRACKLISTWIDTH 200
#define EVENTLISTFRAME_INDEXHEIGHT 256
#define EVENTLISTFRAME_SCROLLBARHEIGHT 16
#define EVENTLISTFRAME_SCROLLBARWIDTH 16
#define EVENTLISTFRAME_BORDERWIDTH 2
#define EVENTLISTFRAME_BORDERHEIGHT 2
#define EVENTLISTFRAME_SPLITTERWIDTH 4
#define EVENTLISTFRAME_SPLITTERHEIGHT 4
#define EVENTLISTFRAME_TRACKLISTHEIGHT 128
#define EVENTLISTFRAME_EVENTKINDLISTHEIGHT 128

// �q�E�B���h�EID���`
#define EVENTLISTFRAME_DUMMYVIEW         (AFX_IDW_PANE_FIRST + 0)
#define EVENTLISTFRAME_PRINTVIEW         (AFX_IDW_PANE_FIRST + 31)
#define EVENTLISTFRAME_SCALEVIEW         (AFX_IDW_PANE_FIRST + 32)
#define EVENTLISTFRAME_PROPERTYSCALEVIEW (AFX_IDW_PANE_FIRST + 33)
#define EVENTLISTFRAME_INDEXSCALEVIEW    (AFX_IDW_PANE_FIRST + 34)
#define EVENTLISTFRAME_INDEXPROPERTYVIEW (AFX_IDW_PANE_FIRST + 35)
#define EVENTLISTFRAME_ROWSCROLL         (AFX_IDW_PANE_FIRST + 48)
#define EVENTLISTFRAME_COLUMNSCROLL      (AFX_IDW_PANE_FIRST + 49)
#define EVENTLISTFRAME_SIZEBOX           (AFX_IDW_PANE_FIRST + 51)
#define EVENTLISTFRAME_ROWZOOMDOWN       (AFX_IDW_PANE_FIRST + 52)
#define EVENTLISTFRAME_ROWZOOMUP         (AFX_IDW_PANE_FIRST + 53)
#define EVENTLISTFRAME_COLUMNZOOMDOWN    (AFX_IDW_PANE_FIRST + 54)
#define EVENTLISTFRAME_COLUMNZOOMUP      (AFX_IDW_PANE_FIRST + 55)
#define EVENTLISTFRAME_TRACKLIST         (AFX_IDW_PANE_FIRST + 61)
#define EVENTLISTFRAME_EVENTKINDLIST     (AFX_IDW_PANE_FIRST + 62)

//
// �C�x���g���X�g�t���[���̃N���C�A���g�̈�ɔz�u���ꂽ�I�u�W�F�N�g
//	�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �@
//                                                            m_lVSc  ��
//                  ����m_lScale������ m_lPropertyWidth ������rollBa����
//                      Width                          �@�@�@ rWidth  ���@�@�@  
//		�@��								                   
//        ��         ��������������������������������������������������������������������������
// m_lToolBar1Height ��CSekaijuToolBar                                                       ��
//        ��         ��m_wndToolBar1                                                         ��
//        ��         ��������������������������������������������������������������������������
//        ��         ��CView*      ��CView*                      ������                  ������
//  m_lScaleHeight   ��m_pScaleView��m_pPrepertyScaleView    �@�@������CCheckListBox     ������
//        ��         �����������������������������������������������@��m_wndTrackList    ������
//        ��         ��CView*      ��CView*           �@ �@�@�@�@������(with ScrollBar)  ������
//        ��         ��m_pIndexScal��m_pPrepertyIndexView    �@�@��  ��                  ������
//                   ��eView       ���@�@�@�@�@�@�@�@�@�@�@�@�@�@��  ��������������������������
//                   ���@�@�@      ���@                          ��  ��������        ������  ��
//  m_lIndexHeight   ���@�@�@      ���@�@�@�@�@�@�@�@�@�@�@�@�@�@������������������������������
//                   ���@�@�@      �� �@CScrollBar m_wndRowScroll������                  ������
//                   ���@�@�@      ���@�@�@�@�@�@�@�@�@�@�@�@�@�@������CCheckListBox     ������
//                   ���@�@�@      ��    CButton m_wndRowZoomDwon���|��m_wndEventKindList������
//        ��         ���@�@�@      ���@�@�@�@�@�@�@�@�@�@�@�@�@�@������(with ScrollBar)  ������
//        ��         ���@�@�@      ��  �@  CButton m_wndRowZoomUp���{��                  ������
//        ��         ��������������������������������������������������������������������������     
//m_lHScrollBarHeight�������@�@�@���@�@�@�@�@�@�@�@�@�������|���{������������        ������  ��
//        ��         ��������������������������������������������������������������������������
//        ��           CScrollBar                  CButton CButton
//                     m_wndColumn                 m_wndCo m_wndCo
//                     Scroll                      lumnZoo lumnZoo
//                                                 mDown   mUp
//
// (��)�\�\�\�F�P���ȋ��E(0px)�B
// (��)�������F�������̓I�ȋ��E���BBORDERWIDTH(2px)����BORDERHEIGHT(2px)�Ŏ���������
// (��)�������F�X�v���b�^�[���E���B(��)*2+SPRITTERWIDTH(4px)����SPRITTERHEIGHT(4px)�Ŏ��������́B




// �֐��}�N��
#ifndef CLIP
#define CLIP(A,B,C) ((A)>(B)?(A):((B)>(C)?(C):(B)))
#endif

// ���b�Z�[�W�}�b�v
IMPLEMENT_DYNCREATE(CEventListFrame, CChildFrame)

BEGIN_MESSAGE_MAP(CEventListFrame, CChildFrame)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_SIZE ()
	ON_WM_TIMER ()
	ON_WM_ERASEBKGND ()
	ON_WM_MDIACTIVATE ()
	ON_WM_CLOSE ()
	ON_WM_PAINT ()	
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_HSCROLL ()
	ON_WM_VSCROLL ()
	ON_BN_CLICKED (EVENTLISTFRAME_COLUMNZOOMDOWN, OnColumnZoomDown)
	ON_BN_CLICKED (EVENTLISTFRAME_COLUMNZOOMUP, OnColumnZoomUp)
	ON_BN_CLICKED (EVENTLISTFRAME_ROWZOOMDOWN, OnRowZoomDown)
	ON_BN_CLICKED (EVENTLISTFRAME_ROWZOOMUP, OnRowZoomUp)
	ON_COMMAND (ID_EVENTLIST_INSERTEVENT, OnEventListInsertEvent)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_INSERTEVENT, OnUpdateEventListInsertEventUI)
	ON_COMMAND (ID_EVENTLIST_DUPLICATEEVENT, OnEventListDuplicateEvent)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_DUPLICATEEVENT, OnUpdateEventListDuplicateEventUI)
	ON_COMMAND (ID_EVENTLIST_DELETEEVENT, OnEventListDeleteEvent)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_DELETEEVENT, OnUpdateEventListDeleteEventUI)
	ON_COMMAND (ID_EVENTLIST_ONLYCURTRACK, OnEventListOnlyCurTrack)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_ONLYCURTRACK, OnUpdateEventListOnlyCurTrackUI)
	ON_COMMAND (ID_EVENTLIST_SHOWALLTRACK, OnEventListShowAllTrack)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_SHOWALLTRACK, OnUpdateEventListShowAllTrackUI)
	ON_COMMAND (ID_EVENTLIST_ONLYCURGRAPH, OnEventListOnlyCurEventKind)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_ONLYCURGRAPH, OnUpdateEventListOnlyCurEventKindUI)
	ON_COMMAND (ID_EVENTLIST_SHOWALLGRAPH, OnEventListShowAllEventKind)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_SHOWALLGRAPH, OnUpdateEventListShowAllEventKindUI)
	ON_COMMAND (ID_EVENTLIST_AUTOPAGEUPDATE, OnEventListAutoPageUpdate)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_AUTOPAGEUPDATE, OnUpdateEventListAutoPageUpdateUI)
	ON_COMMAND (ID_EVENTLIST_SAVEAS, OnEventListSaveAs)
	ON_UPDATE_COMMAND_UI (ID_EVENTLIST_SAVEAS, OnUpdateEventListSaveAsUI)

	ON_COMMAND (ID_POPUP_TRACKVISIBLEON, OnPopupTrackVisibleOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEON, OnUpdatePopupTrackVisibleOnUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_TRACKVISIBLEOFF, OnPopupTrackVisibleOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEOFF, OnUpdatePopupTrackVisibleOffUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_TRACKVISIBLEALL, OnPopupTrackVisibleAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEALL, OnUpdatePopupTrackVisibleAllUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_EVENTKINDVISIBLEON, OnPopupEventKindVisibleOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_EVENTKINDVISIBLEON, OnUpdatePopupEventKindVisibleOnUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_EVENTKINDVISIBLEOFF, OnPopupEventKindVisibleOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_EVENTKINDVISIBLEOFF, OnUpdatePopupEventKindVisibleOffUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_EVENTKINDVISIBLEALL, OnPopupEventKindVisibleAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_EVENTKINDVISIBLEALL, OnUpdatePopupEventKindVisibleAllUI) // 20100429�ǉ�

	ON_CBN_SELENDOK (IDC_EVENTTRACKCOMBO, OnTrackComboSelEndOK)	
	ON_CLBN_CHKCHANGE (EVENTLISTFRAME_TRACKLIST, OnTrackListChkChange)
	ON_LBN_SELCHANGE (EVENTLISTFRAME_TRACKLIST, OnTrackListSelChange)
	ON_CBN_SELENDOK (IDC_EVENTKINDCOMBO, OnEventKindComboSelEndOK)	
	ON_CLBN_CHKCHANGE (EVENTLISTFRAME_EVENTKINDLIST, OnEventKindListChkChange)
	ON_LBN_SELCHANGE (EVENTLISTFRAME_EVENTKINDLIST, OnEventKindListSelChange)
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CEventListFrame::CEventListFrame () {
	long i;
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_lIndexHeight = EVENTLISTFRAME_INDEXHEIGHT;
	m_lScaleHeight = EVENTLISTFRAME_SCALEHEIGHT;
	m_lScaleWidth = EVENTLISTFRAME_SCALEWIDTH;
	m_lPropertyWidth = EVENTLISTFRAME_PROPERTYWIDTH;
	m_lTrackListWidth = EVENTLISTFRAME_TRACKLISTWIDTH;
	m_lTrackListHeight = EVENTLISTFRAME_TRACKLISTHEIGHT;
	m_lEventKindListHeight = EVENTLISTFRAME_EVENTKINDLISTHEIGHT;
	m_lHScrollBarHeight = ::GetSystemMetrics (SM_CYHSCROLL);
	m_lVScrollBarWidth = ::GetSystemMetrics (SM_CXVSCROLL);
	m_lRowZoom = pSekaijuApp->m_theEventListOption.m_lDefRowZoom;
	m_lColumnZoom = pSekaijuApp->m_theEventListOption.m_lDefColumnZoom;
	m_lRowScrollPos = 0;
	m_lColumnScrollPos = 0;
	// �񕝂̒�`
	m_lColumnBaseWidth[0] = pSekaijuApp->m_theEventListOption.m_lDefTrackWidth;
	m_lColumnBaseWidth[1] = pSekaijuApp->m_theEventListOption.m_lDefMillisecWidth;
	m_lColumnBaseWidth[2] = pSekaijuApp->m_theEventListOption.m_lDefTimeWidth;
	m_lColumnBaseWidth[3] = pSekaijuApp->m_theEventListOption.m_lDefKindWidth;
	m_lColumnBaseWidth[4] = pSekaijuApp->m_theEventListOption.m_lDefChWidth;
	m_lColumnBaseWidth[5] = pSekaijuApp->m_theEventListOption.m_lDefVal1Width;
	m_lColumnBaseWidth[6] = pSekaijuApp->m_theEventListOption.m_lDefVal2Width;
	m_lColumnBaseWidth[7] = pSekaijuApp->m_theEventListOption.m_lDefVal3Width;
	// ��^�C�g���̒�`
	VERIFY (m_strColumnTitle[0].LoadString (IDS_TRACK));
	VERIFY (m_strColumnTitle[1].LoadString (IDS_HOUR_MINUTE_SECOND_MILLISEC));
	VERIFY (m_strColumnTitle[2].LoadString (IDS_MEASURE_BEAT_TICK));
	VERIFY (m_strColumnTitle[3].LoadString (IDS_EVENT_KIND));
	VERIFY (m_strColumnTitle[4].LoadString (IDS_CHANNEL));
	VERIFY (m_strColumnTitle[5].LoadString (IDS_VALUE1));
	VERIFY (m_strColumnTitle[6].LoadString (IDS_VALUE2));
	VERIFY (m_strColumnTitle[7].LoadString (IDS_VALUE3));
	// �f�t�H���g�t�H���g�̍쐬
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);
	m_bAutoPageUpdate = FALSE;
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	for (i = 0; i < MAXMIDITRACKNUM; i++) {
		m_bTrackVisible[i] = TRUE;
	}
	m_bOnlyCurEventKind = FALSE;
	m_bShowAllEventKind = FALSE;
	for (i = 0; i < 256; i++) {
		m_bEventKindVisible[i] = TRUE;
	}

}

// �f�X�g���N�^
CEventListFrame::~CEventListFrame () {
	m_theFont.DeleteObject ();
}



//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �h�L�������g�̎擾
CSekaijuDoc* CEventListFrame::GetDocument () {
	ASSERT (m_pDummyView);
	return (CSekaijuDoc*)m_pDummyView->GetDocument ();
}

// �E�B���h�E�̃t�H���g���擾
CFont* CEventListFrame::GetParentFont () {
	return &(m_theFont);
}

// �ł���ɕ\������Ă���s���擾
long CEventListFrame::GetVisibleTopRow () {
	return m_lRowScrollPos / m_lRowZoom;
}

// �ł����ɕ\������Ă���s���擾
long CEventListFrame::GetVisibleBottomRow () {
	CRect rcClient;
	m_pIndexScaleView->GetClientRect (&rcClient);
	return (m_lRowScrollPos + rcClient.Height ()) / m_lRowZoom;
}

// ������̃Y�[���{���擾
long CEventListFrame::GetColumnZoom () {
	return m_lColumnZoom;
}

// �s�����̃Y�[���{���擾
long CEventListFrame::GetRowZoom () {
	return m_lRowZoom;
}

// �w�肵����̊�{�����擾
long CEventListFrame::GetColumnBaseWidth (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < 8);
	return m_lColumnBaseWidth[lColumn];
}

// �w�肵����̊�{����ݒ�
long CEventListFrame::SetColumnBaseWidth (long lColumn, long lBaseWidth) {
	ASSERT (0 <= lColumn && lColumn < 8);
	m_lColumnBaseWidth[lColumn] = lBaseWidth;
	return 1;
}

// �w�肵����̃^�C�g�����擾
CString CEventListFrame::GetColumnTitle (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < 8);
	return m_strColumnTitle[lColumn];
}

// �w�肵����̃^�C�g�����擾
long CEventListFrame::SetColumnTitle (long lColumn, const TCHAR* pszColumnTitle) {
	ASSERT (0 <= lColumn && lColumn < 8);
	m_strColumnTitle[lColumn] = pszColumnTitle;
	return 1;
}


// �w��̗�̍����W���擾
long CEventListFrame::GetColumnLeft (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < 8);
	long lSum = 0;
	for (long j = 0; j < lColumn; j++) {
		lSum += m_lColumnBaseWidth[j];
	}
	return lSum * m_lColumnZoom;
}

// �w��̗�̕����擾
long CEventListFrame::GetColumnWidth (long lColumn) {
	ASSERT (0 <= lColumn && lColumn < 8);
	return m_lColumnBaseWidth[lColumn] * m_lColumnZoom;
}

// y���W���C���f�b�N�X�ɕϊ�
long CEventListFrame::YtoRow (long y) {
	return y / m_lRowZoom;
}

// �C���f�b�N�X��y���W�ɕϊ�
long CEventListFrame::RowtoY (long lRow) {
	return m_lRowZoom * lRow;
}

// x���W����v���p�e�B���擾
long CEventListFrame::XtoColumn (long x) {
	long lSum = 0;
	long lNextSum = 0;
	for (long j = 0; j < 8; j++) {		
		lNextSum += m_lColumnBaseWidth[j] * m_lColumnZoom;
		if (lSum <= x && x < lNextSum) {
			return j;
		}
		lSum = lNextSum;
	}
	return -1;
}

// �v���p�e�B��x���W�ɕϊ�
long CEventListFrame::ColumntoX (long lColumn) {
	return GetColumnLeft (lColumn);
}

// ������̃X�N���[���|�W�V�������擾
long CEventListFrame::GetColumnScrollPos () {
	return m_lColumnScrollPos;
}

// �s�����̃X�N���[���|�W�V�������擾
long CEventListFrame::GetRowScrollPos () {
	return m_lRowScrollPos;
}

// ������X�N���[���|�W�V�����̐ݒ�
long CEventListFrame::SetColumnScrollPos (long lColumnScrollPos) {
	long lOldColumnScrollPos = m_lColumnScrollPos;
	m_wndColumnScroll.SetScrollPos (lColumnScrollPos);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
	long lDeltaColumnScrollPos = m_lColumnScrollPos - lOldColumnScrollPos;
	m_pPropertyScaleView->ScrollWindow (-lDeltaColumnScrollPos, 0);
	m_pIndexPropertyView->ScrollWindow (-lDeltaColumnScrollPos, 0);
	m_pPropertyScaleView->UpdateWindow ();
	m_pIndexPropertyView->UpdateWindow ();
	return m_lColumnScrollPos;
}

// �s�����X�N���[���|�W�V�����̐ݒ�
long CEventListFrame::SetRowScrollPos (long lRowScrollPos) {
	long lOldRowScrollPos = m_lRowScrollPos;
	m_wndRowScroll.SetScrollPos (lRowScrollPos);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
	long lDeltaRowScrollPos = m_lRowScrollPos - lOldRowScrollPos;
	m_pIndexScaleView->ScrollWindow (0, -lDeltaRowScrollPos);
	m_pIndexPropertyView->ScrollWindow (0, -lDeltaRowScrollPos);
	m_pIndexScaleView->UpdateWindow ();
	m_pIndexPropertyView->UpdateWindow ();
	return m_lRowScrollPos;
}

// ����MIDI�C�x���g�����擾
long CEventListFrame::GetVisibleEventCount () {
	return m_theVisibleEventArray.GetSize ();
}

// ����MIDI�C�x���g���擾
MIDIEvent* CEventListFrame::GetVisibleEvent (long lIndex) {
	if (0 <= lIndex && lIndex < m_theVisibleEventArray.GetSize ()) {
		return (MIDIEvent*)(m_theVisibleEventArray.GetAt (lIndex));
	}
	return NULL;
}

// �X�v���b�^�[�L���v�^�[�̕`��
void CEventListFrame::DrawSplitterCaptor (CDC* pDC, CPoint pt) {
	CRect rcClient;
	GetClientRect (&rcClient);
	CPen pen;
	CPen* pOldPen;
	pen.CreatePen (PS_SOLID, 4, ::GetSysColor(COLOR_BTNSHADOW));
	pDC->SetROP2 (R2_XORPEN);
	pOldPen = pDC->SelectObject (&pen);
	if (m_bSplitterMovingH) {
		pDC->MoveTo (0, pt.y);
		pDC->LineTo (rcClient.Width (), pt.y);
	}
	if (m_bSplitterMovingV) {
		pDC->MoveTo (pt.x, 0);
		pDC->LineTo (pt.x, rcClient.Height ());
	}
	pDC->SelectObject (pOldPen);
}



/*		"0�V�[�P���X�ԍ�",
		"1�e�L�X�g�C�x���g",
		"2���쌠",
		"3�V�[�P���X���^�g���b�N��",
		"4�C���X�g�D�������g��",
		"5�̎�",
		"6�}�[�J�[",
		"7�L���[�|�C���g",
		"8�v���O������",
		"9�f�o�C�X��",
		"10�`�����l���v���t�B�b�N�X",
		"11�|�[�g�v���t�B�b�N�X",
		"12�G���h�I�u�g���b�N",
		"13�e���|�ݒ�",
		"14SMPTE�I�t�Z�b�g",
		"15���q�L��",
		"16�����L��",
		"17�V�[�P���T�Ǝ��̃C�x���g",
		"18�m�[�g�I�t",
		"19�m�[�g�I��",
		"20�L�[�A�t�^�[�^�b�`",
		"21�R���g���[���[",
		"22�v���O�����`�F���W",
		"23�`�����l���A�t�^�[�^�b�`",
		"24�s�b�`�x���h",
		"25�V�X�e���G�N�X�N���[�V��",
		"26�s���Ȏ�ނ̃C�x���g"};
*/

// ���̃C�x���g�z��̍쐬
BOOL CEventListFrame::MakeVisibleEventArray () {
 	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pMIDIData->m_pFirstTrack;
	MIDIEvent* pMIDIEvent = NULL;

	// ���I���C�x���g��pOldMIDIEvent�ɕێ����Ă���
	long lOldVisibleEventCount = m_theVisibleEventArray.GetSize ();
	long lOldRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	MIDIEvent* pOldMIDIEvent = NULL;
	if (0 <= lOldRow && lOldRow < lOldVisibleEventCount) {
		pOldMIDIEvent = (MIDIEvent*)m_theVisibleEventArray.GetAt (lOldRow);
	}
	
	// ���ݔz��̏�����
	m_theVisibleEventArray.RemoveAll ();
	MIDIEvent* pTempEvent[MAXMIDITRACKNUM];
	long lTime = 0;
	long lMinimumTime = 0;
	long lMinimumTrackIndex = 0;
	long lTrackNum = 0;
	long i = 0;
	// �e�g���b�N�p�̈ꎞ�C�x���g�ێ��X�y�[�X��������
	forEachTrack (pMIDIData, pMIDITrack) {
		pTempEvent[i] = MIDITrack_GetFirstEvent (pMIDITrack);
		i++;
		if (i >= MAXMIDITRACKNUM) break;
	}
	// �e�g���b�N�̃C�x���g���^�C�����ɔz��ɓo�^����
	while (1) {
		lMinimumTime = 0x7FFFFFFF;
		i = 0;
		forEachTrack (pMIDIData, pMIDITrack) {
			if (IsTrackVisible (i)) {
				if (pTempEvent[i] != NULL) {
					lTime = pTempEvent[i]->m_lTime;
					if (lTime < lMinimumTime) {
						lMinimumTime = lTime;
						lMinimumTrackIndex = i;
					}
				}
			}
			i++;
			if (i >= MAXMIDITRACKNUM) break;
		}
		if (lMinimumTrackIndex < MAXMIDITRACKNUM && lMinimumTime != 0x7FFFFFFF) {
			for (pMIDIEvent = pTempEvent[lMinimumTrackIndex];
				pMIDIEvent; pMIDIEvent = pMIDIEvent->m_pNextEvent) {
				if (pMIDIEvent->m_lTime == lMinimumTime) {
					long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
					if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
						lEventKind &= 0x0F;
						lEventKind |= 0x80;
					}
					long lListIndex = EventKindtoListIndex (lEventKind);
					ASSERT (0 <= lListIndex && lListIndex <= 0xFF);
					if (IsEventKindVisible (lListIndex)) {
						m_theVisibleEventArray.Add (pMIDIEvent);
					}
				}
				else {
					break;
				}
			}
			pTempEvent[lMinimumTrackIndex] = pMIDIEvent;
		}
		else {
			break;
		}
	}
	// �s�X�N���[���o�[�̍Ē���
	RecalcRowScrollInfo ();

	// ���C�x���g�Ƀt�H�[�J�X�����킹��
	long lNewCount = m_theVisibleEventArray.GetSize ();
	long lNewRow = CLIP (0, lOldRow, lNewCount - 1);
	if (lNewRow < 0) {
		lNewRow = 0; // lNewCount == 0�̏ꍇ�ɔ�����
	}
	if (pOldMIDIEvent) {
		// ��v����C�x���g�����������ꍇ�A���̃C�x���g�Ƀt�H�[�J�X�����킹��
		for (i = 0; i < lNewCount; i++) {
			if (m_theVisibleEventArray.GetAt (i) == pOldMIDIEvent) {
				break;
			}
		}
		if (i < lNewCount) {
			lNewRow = i;
		}
		// ��v����C�x���g���Ȃ��ꍇ�A���������������̎��̃C�x���g�Ƀt�H�[�J�X�����킹��
		// ��v����C�x���g�������ꍇ�Am_lCurRow�͂��̂܂܂̒l�Ƃ���
		//else {
			//for (i = 0; i < lNewCount; i++) {
			//	MIDIEvent* pTempEvent = (MIDIEvent*)m_theVisibleEventArray.GetAt (i);
			//	if (MIDIEvent_GetTime (pTempEvent) >= MIDIEvent_GetTime (pOldMIDIEvent)) {
			//		break;
			//	}
			//}
			//lNewRow = i;
			//lNewRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
		//}
	}
	if (((CEventListIndexPropertyView*)m_pIndexPropertyView)->IsTextEditing ()) {
		((CEventListIndexPropertyView*)m_pIndexPropertyView)->EndTextEditingCancel ();
	}
	if (((CEventListIndexPropertyView*)m_pIndexPropertyView)->IsListSelecting ()) {
		((CEventListIndexPropertyView*)m_pIndexPropertyView)->EndListSelectingCancel ();
	}
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = lNewRow;
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lNewRow, lCurColumn);
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (lNewRow, lCurColumn);
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (lNewRow, lCurColumn);
	return TRUE;
}

BOOL CEventListFrame::ReplaceVisibleEvent (MIDIEvent* pOldEvent, MIDIEvent* pNewEvent) {
	long lCount = 0;
	long lVisibleEventCount = m_theVisibleEventArray.GetSize ();
	pOldEvent = MIDIEvent_GetFirstCombinedEvent (pOldEvent);
	pNewEvent = MIDIEvent_GetFirstCombinedEvent (pNewEvent);
	while (pOldEvent && pNewEvent) {
		long i = 0;
		for (i = 0; i < lVisibleEventCount; i++) {
			if (m_theVisibleEventArray.GetAt (i) == pOldEvent) {
				m_theVisibleEventArray.SetAt (i, pNewEvent);	
				lCount++;
			}
		}
		pOldEvent = pOldEvent->m_pNextCombinedEvent;
		pNewEvent = pNewEvent->m_pNextCombinedEvent;
	}
	return lCount;
}


// ���݂̃g���b�N�̃C���f�b�N�X���擾
long CEventListFrame::GetCurTrackIndex () {
	return m_wndTrackList.GetCurSel ();
}

// ���݂̃O���t�̎�ނ��擾
long CEventListFrame::GetCurEventKind () {
	return m_wndEventKindList.GetCurSel ();
}

// ���݂̃g���b�N�̃C���f�b�N�X��ݒ�
BOOL CEventListFrame::SetCurTrackIndex (long lCurTrackIndex) {
	ASSERT (0 <= lCurTrackIndex && lCurTrackIndex < MAXMIDITRACKNUM);
	m_wndEventTrackCombo.SetCurSel (lCurTrackIndex);
	m_wndTrackList.SetCurSel (lCurTrackIndex);
	if (m_bOnlyCurTrack) {
		long lCount = m_wndTrackList.GetCount ();
		for (long i = 0; i < lCount; i++) {
			m_wndTrackList.SetCheck (i, (i == lCurTrackIndex ? 1 : 0));
		}
		MakeVisibleEventArray ();
		long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
		long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
		((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
		m_pIndexScaleView->Invalidate ();
		m_pIndexPropertyView->Invalidate ();
	}
	return TRUE;
}

// ���݂̃O���t�̎�ނ�ݒ�
BOOL CEventListFrame::SetCurEventKind (long lCurEventKind) {
	ASSERT (0 <= lCurEventKind && lCurEventKind < 256);
	m_wndEventKindCombo.SetCurSel (lCurEventKind);
	m_wndEventKindList.SetCurSel (lCurEventKind);
	if (m_bOnlyCurEventKind) {
		long lCount = m_wndEventKindList.GetCount ();
		for (long i = 0; i < lCount; i++) {
			m_wndEventKindList.SetCheck (i, (i == lCurEventKind ? 1 : 0));
		}
		MakeVisibleEventArray ();
		long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
		long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
		((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
		m_pIndexScaleView->Invalidate ();
		m_pIndexPropertyView->Invalidate ();
	}
	return TRUE;
}

// ���݂̃`�����l����ݒ�
BOOL CEventListFrame::SetCurChannel (long lCurChannel) {
	ASSERT (-1 <= lCurChannel && lCurChannel < 16);
	m_wndEventChannelCombo.SetCurSel (lCurChannel + 1);
	return TRUE;
}

// �w��C���f�b�N�X�̃g���b�N���\����Ԃ����ׂ�
BOOL CEventListFrame::IsTrackVisible (long lTrackIndex) {
	// (1)���݂̃g���b�N�̂ݕ\����ON�̂Ƃ��́A���݂̃g���b�N�݂̂�Visible�A����UnVisible�B
	// (2)���ׂẴg���b�N��\����ON�̂Ƃ��́A�S�Ẵg���b�N��Visible�B
	// (3)���̑��̏ꍇ(�ʏ펞)�́Am_bTrackVisible[MAXMIDITRACKNUM]�̒l�ɏ]���B
	ASSERT (0 <= lTrackIndex && lTrackIndex < MAXMIDITRACKNUM);
	if (m_bOnlyCurTrack == TRUE &&
		GetCurTrackIndex () == lTrackIndex ||
		m_bShowAllTrack == TRUE ||
		m_bShowAllTrack == FALSE &&
		m_bOnlyCurTrack == FALSE &&
		m_bTrackVisible[lTrackIndex] == TRUE) {
		return TRUE;
	}
	return FALSE;
}

// �w��C���f�b�N�X�̃g���b�N��\����Ԃɂ���
BOOL CEventListFrame::SetTrackVisible (long lTrackIndex) {
	// (1)���݂̃g���b�N�̂ݕ\����ON�̂Ƃ��́A���݂̃g���b�N���w��g���b�N�ɕύX����
	// (2)���ׂẴg���b�N��\����ON�̂Ƃ��́A�������Ȃ��B
	// (3)���̑��̏ꍇ(�ʏ펞)�́Am_bTrackVisible[lTrackIndex]���`�F�b�N�E��������B
	ASSERT (0 <= lTrackIndex && lTrackIndex < MAXMIDITRACKNUM);
	if (m_bOnlyCurTrack == TRUE) {
		m_wndEventTrackCombo.SetCurSel (lTrackIndex);
		m_wndTrackList.SetCurSel (lTrackIndex);
	}
	else if (m_bShowAllTrack == TRUE) {
		;
	}
	else {
		m_wndTrackList.SetCheck (lTrackIndex, TRUE);
		m_bTrackVisible[lTrackIndex] = TRUE;
	}
	return TRUE;
}

// �w���ނ̃C�x���g���\����Ԃ����ׂ�
BOOL CEventListFrame::IsEventKindVisible (long lEventKind) {
	// (1)���݂̃C�x���g��ނ̂ݕ\����ON�̂Ƃ��́A���݂̃O���t�݂̂�Visible�A����UnVisible�B
	// (2)���ׂẴC�x���g��ނ�\����ON�̂Ƃ��́A�S�ẴO���t��Visible�B
	// (3)���̑��̏ꍇ(�ʏ펞)�́Am_bEventKindVisible[MAXMIDITRACKNUM]�̒l�ɏ]���B
	ASSERT (0 <= lEventKind && lEventKind < 128);
	if (m_bOnlyCurEventKind == TRUE &&
		GetCurEventKind () == lEventKind ||
		m_bShowAllEventKind == TRUE ||
		m_bShowAllEventKind == FALSE &&
		m_bOnlyCurEventKind == FALSE &&
		m_bEventKindVisible[lEventKind] == TRUE) {
		return TRUE;
	}
	return FALSE;
}

// �w���ނ̃C�x���g��\����Ԃɂ���
BOOL CEventListFrame::SetEventKindVisible (long lEventKind) {
	// (1)���݂̃C�x���g��ނ̂ݕ\����ON�̂Ƃ��́A���݂̃C�x���g�̎�ނ�ύX����
	// (2)���ׂẴC�x���g��ނ�\����ON�̂Ƃ��́A�������Ȃ��B
	// (3)���̑��̏ꍇ(�ʏ펞)�́Am_bEventKindVisible[lEventKind]���`�F�b�N�E��������B
	ASSERT (0 <= lEventKind && lEventKind < m_wndEventKindList.GetCount ());
	if (m_bOnlyCurEventKind == TRUE) {
		m_wndEventKindCombo.SetCurSel (lEventKind);
		m_wndEventKindList.SetCurSel (lEventKind);
	}
	else if (m_bShowAllEventKind == TRUE) {
		;
	}
	else {
		m_wndEventKindList.SetCheck (lEventKind, TRUE);
		m_bEventKindVisible[lEventKind] = TRUE;
	}
	return TRUE;
}

// �g���b�N�R���{�{�b�N�X�̏[��
BOOL CEventListFrame::UpdateTrackCombo () {
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(m_pDummyView->GetDocument ());
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	//pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;

	// ����Ԃ̕ێ�
	static MIDITrack* pOldMIDITrack[MAXMIDITRACKNUM];
	BOOL bOldTrackVisible[MAXMIDITRACKNUM];
	memcpy (bOldTrackVisible, m_bTrackVisible, sizeof (BOOL) * MAXMIDITRACKNUM);
	long lOldCurSel = m_wndEventTrackCombo.GetCurSel ();
	long lOldCount = m_wndEventTrackCombo.GetCount ();
	MIDITrack* pOldCurTrack = NULL;
	if (0 <= lOldCurSel && lOldCurSel < __min (lOldCount, MAXMIDITRACKNUM)) {
		pOldCurTrack = pOldMIDITrack[lOldCurSel];
	}

	// �R���{�̏�����
	m_wndEventTrackCombo.RemoveAllItem ();

	// �R���{�ɍ��ڒǉ�
	TCHAR szTrackName1[1024];
	TCHAR szTrackName2[1024];
	CString strText;
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		memset (szTrackName1, 0, sizeof (szTrackName1));
		memset (szTrackName2, 0, sizeof (szTrackName2));
		MIDITrack_GetName (pMIDITrack, szTrackName1, TSIZEOF (szTrackName1) - 1);
		codestr2str (szTrackName1, TCSLEN (szTrackName1), szTrackName2, TSIZEOF (szTrackName2) - 1);
		strText.Format (_T("%d-%s"), i + (bTrackZeroOrigin ? 0 : 1), szTrackName2);
		long lForeColor = MIDITrack_GetForeColor (pMIDITrack);
		long lBackColor = ::GetSysColor (COLOR_WINDOW);
		m_wndEventTrackCombo.AddItem (strText, lForeColor, lBackColor);
		
		// ���݂̃g���b�N�ł���ꍇ�I��
		if (pMIDITrack == pOldCurTrack) {
			m_wndEventTrackCombo.SetCurSel (i);
		}
		i++;
	}

	// ���ݑI�����Ă�����̂��Ȃ��ꍇ�͋����I��
	long lNewCount = m_wndEventTrackCombo.GetCount ();
	long lNewCurSel = m_wndEventTrackCombo.GetCurSel ();
	if (m_wndEventTrackCombo.GetCurSel () == CB_ERR) {
		if (0 <= lOldCurSel && lOldCurSel < lNewCount) {
			m_wndEventTrackCombo.SetCurSel (lOldCurSel);
		}
		else if (lNewCount >= 2) {
			m_wndEventTrackCombo.SetCurSel (1);
		}
		else {
			m_wndEventTrackCombo.SetCurSel (0);
		}
		lNewCurSel = m_wndEventTrackCombo.GetCurSel ();
	}

	// ����̃A�b�v�f�[�g�Ăяo���ɔ����Č����ێ�����B
	i = 0;
	memset (pOldMIDITrack, 0, sizeof (MIDITrack*) * MAXMIDITRACKNUM);
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		pOldMIDITrack[i] = pMIDITrack;
		i++;
	}
	
	//pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// �g���b�N���X�g�{�b�N�X�̏[��
BOOL CEventListFrame::UpdateTrackList () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	//pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	// ����Ԃ̕ێ�
	static MIDITrack* pOldMIDITrack[MAXMIDITRACKNUM];
	BOOL bOldTrackVisible[MAXMIDITRACKNUM];
	memcpy (bOldTrackVisible, m_bTrackVisible, sizeof (BOOL) * MAXMIDITRACKNUM);
	long lOldCurSel = m_wndTrackList.GetCurSel ();
	long lOldCount = m_wndTrackList.GetCount ();
	MIDITrack* pOldCurTrack = NULL;
	if (0 <= lOldCurSel && lOldCurSel < __min (lOldCount, MAXMIDITRACKNUM)) {
		pOldCurTrack = pOldMIDITrack[lOldCurSel];
	}

	// ���X�g�̏�����
	m_wndTrackList.RemoveAllItem ();

	// ���X�g�ɍ��ڂ�ǉ�
	long i = 0;
	long j = 0;
	TCHAR szTrackName1[1024];
	TCHAR szTrackName2[1024];
	CString strText;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		memset (szTrackName1, 0, sizeof (szTrackName1));
		memset (szTrackName2, 0, sizeof (szTrackName2));
		MIDITrack_GetName (pMIDITrack, szTrackName1, TSIZEOF (szTrackName1) - 1);
		codestr2str (szTrackName1, TCSLEN (szTrackName1), szTrackName2, TSIZEOF (szTrackName2) - 1);
		strText.Format (_T("%d-%s"), i + (bTrackZeroOrigin ? 0 : 1), szTrackName2);
		long lForeColor = MIDITrack_GetForeColor (pMIDITrack);
		long lBackColor = ::GetSysColor (COLOR_WINDOW);
		m_wndTrackList.AddItem (strText, lForeColor, lBackColor);
		// �`�F�b�N��ԕϐ��̍X�V
		m_bTrackVisible[i] = 1;
		for (j = 0; j < MAXMIDITRACKNUM; j++) {
			if (pOldMIDITrack[j] == NULL) {
				break;
			}
			if (pOldMIDITrack[j] == pMIDITrack) {
				m_bTrackVisible[i] = bOldTrackVisible[j];
				break;
			}
		}
		// ���݂̃g���b�N�ł���ꍇ�I��
		if (pMIDITrack == pOldCurTrack) {
			m_wndTrackList.SetCurSel (i);
		}
		i++;
	}

	// ���ݑI�����Ă�����̂��Ȃ��ꍇ�͋����I��
	long lNewCount = m_wndTrackList.GetCount ();
	long lNewCurSel = m_wndTrackList.GetCurSel ();
	if (m_wndTrackList.GetCurSel () == LB_ERR) {
		if (0 <= lOldCurSel && lOldCurSel < lNewCount) {
			m_wndTrackList.SetCurSel (lOldCurSel);
		}
		else if (lNewCount >= 2) {
			m_wndTrackList.SetCurSel (1);
		}
		else {
			m_wndTrackList.SetCurSel (0);
		}
		lNewCurSel = m_wndTrackList.GetCurSel ();
	}

	// �`�F�b�N�{�b�N�X�̊��S�X�V
	if (m_bShowAllTrack) {
		for (i = 0; i < lNewCount; i++) {
			m_wndTrackList.SetCheck (i, 1);
		}
	}
	else if (m_bOnlyCurTrack) {
		for (i = 0; i < lNewCount; i++) {
			m_wndTrackList.SetCheck (i, 0);
		}
		if (0 <= lNewCurSel && lNewCurSel < lNewCount) {
			m_wndTrackList.SetCheck (lNewCurSel, 1);
		}
	}
	else {
		for (i = 0; i < lNewCount; i++) {
			m_wndTrackList.SetCheck (i, m_bTrackVisible[i]);
		}
	}

	// ����̃A�b�v�f�[�g�Ăяo���ɔ����Č����ێ�����B
	i = 0;
	memset (pOldMIDITrack, 0, sizeof (MIDITrack*) * MAXMIDITRACKNUM);
	forEachTrack (pMIDIData, pMIDITrack) {
		if (i >= MAXMIDITRACKNUM) {
			break;
		}
		pOldMIDITrack[i] = pMIDITrack;
		i++;
	}
	
	//pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// �C�x���g�̎�ނ��烊�X�g�C���f�b�N�X���擾
long CEventListFrame::EventKindtoListIndex (long lEventKind) {
	long lEventKindtoListIndex[256] = {
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 27, 27, 27, 27, 27, 27,
		27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
		10, 11, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 12,
		27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
		27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
		27, 13, 27, 27, 14, 27, 27, 27, 15, 16, 27, 27, 27, 27, 27, 27,
		27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
		27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 17,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
		19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
		22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
		23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
		25, 27, 27, 27, 27, 27, 27, 26, 27, 27, 27, 27, 27, 27, 27, 27
	};
	ASSERT (0 <= lEventKind && lEventKind <= 0xFF);
	if (0 <= lEventKind && lEventKind < 256) {
		return lEventKindtoListIndex[lEventKind];
	}
	return -1;
}

// ���X�g�C���f�b�N�X����C�x���g�̎�ނ��擾
long CEventListFrame::ListIndextoEventKind (long lListIndex) {
	ASSERT (0 <= lListIndex && lListIndex <= 0xFF);
	for (long i = 0; i < 256; i++) {
		if (EventKindtoListIndex (i) == lListIndex) {
			return i;
		}
	}
	return -1;
}


// �s�����X�N���[���o�[(�c)�̃f�U�C���ݒ�
void CEventListFrame::RecalcRowScrollInfo () {
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (m_theVisibleEventArray.GetSize () + 4) * m_lRowZoom;
	si.nPage = m_lIndexHeight;
	m_wndRowScroll.SetScrollInfo (&si, TRUE);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
}

// ������X�N���[���o�[(��)�̃f�U�C���ݒ�
void CEventListFrame::RecalcColumnScrollInfo () {
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	long lSum = 0;
	for (long j = 0; j < 8; j++) {
		lSum += m_lColumnBaseWidth[j];
	}
	si.nMax = lSum * m_lColumnZoom;
	si.nPage = m_lPropertyWidth;
	m_wndColumnScroll.SetScrollInfo (&si, TRUE);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
}







//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �E�B���h�E�������O�̍\���̐ݒ�
BOOL CEventListFrame::PreCreateWindow (CREATESTRUCT& cs) {
	return CWnd::PreCreateWindow (cs);
}

// �E�B���h�E�^�C�g���̎����ݒ�(CMDIChildWnd::OnUpdateFrameTitle�̃I�[�o�[���C�h)
void CEventListFrame::OnUpdateFrameTitle (BOOL bAddToTitle) {
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle (bAddToTitle);
	if ((GetStyle() & FWS_ADDTOTITLE) == 0) {
		return;     // leave child window alone!
	}
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL) {
		CString strEventList;
		strEventList.LoadString (IDS_EVENTLIST);
		CString strTitle;
		if (m_nWindow > 0) {
			strTitle.Format (_T("%s:%d(%s)"), pDocument->GetTitle (), m_nWindow, strEventList);
		}
		else {
			strTitle.Format (_T("%s(%s)"), pDocument->GetTitle (), strEventList);
		}
		this->SetWindowText (strTitle);
	}
}

// �Ĕz�u�p�֐�(CFrameWnd::RecalcLayout�̃I�[�o�[���C�h)
void CEventListFrame::RecalcLayout (BOOL bNotify) {

	CRect rcClient;
	GetClientRect (&rcClient);

	// ��{�N���X�̊֐�������
	CFrameWnd::RecalcLayout (bNotify);

	// �A�C�R�������ɂ͊e�R���g���[���̃T�C�Y���Čv�Z���Ȃ��B
	if (rcClient.Width () == 0 || rcClient.Height () == 0) {
		return;
	}

	// �c�[���o�[1�̍����擾
	CRect rcToolBar1;
	m_wndToolBar1.GetWindowRect (rcToolBar1); 
	m_lToolBar1Height = rcToolBar1.Height ();

	// ���������̈ʒu�v�Z
	if (rcClient.Height () < 
		m_lToolBar1Height + EVENTLISTFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		EVENTLISTFRAME_BORDERHEIGHT * 2) {
		m_lScaleHeight = rcClient.Height () - m_lHScrollBarHeight -
			EVENTLISTFRAME_BORDERHEIGHT * 2 - m_lToolBar1Height;
		m_lIndexHeight = 0;
	}
	else {
		m_lScaleHeight = EVENTLISTFRAME_SCALEHEIGHT;
		m_lIndexHeight = rcClient.Height () - 
			m_lScaleHeight - m_lHScrollBarHeight -
			EVENTLISTFRAME_BORDERHEIGHT * 2 - m_lToolBar1Height;
	}
	if (rcClient.Height () <
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT * 4 +
		EVENTLISTFRAME_SPLITTERHEIGHT + m_lTrackListHeight) {
		m_lTrackListHeight = rcClient.Height () -
			m_lToolBar1Height - EVENTLISTFRAME_BORDERHEIGHT * 4 -
			EVENTLISTFRAME_SPLITTERHEIGHT;
		m_lEventKindListHeight = 0;
	}
	else {
		m_lTrackListHeight = m_lTrackListHeight;
		m_lEventKindListHeight = rcClient.Height () -
			m_lToolBar1Height - EVENTLISTFRAME_BORDERHEIGHT * 4 -
			EVENTLISTFRAME_SPLITTERHEIGHT - m_lTrackListHeight;
	}

	// �������̈ʒu�v�Z
	if (rcClient.Width () <
		EVENTLISTFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		EVENTLISTFRAME_BORDERWIDTH * 4 + EVENTLISTFRAME_SPLITTERWIDTH) {
		m_lScaleWidth = rcClient.Width () - 
			m_lVScrollBarWidth - EVENTLISTFRAME_BORDERWIDTH * 4 - EVENTLISTFRAME_SPLITTERWIDTH;
		m_lTrackListWidth = 0;
		m_lPropertyWidth = 0;
	}
	else if (rcClient.Width () <
		EVENTLISTFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		EVENTLISTFRAME_BORDERWIDTH * 4 + EVENTLISTFRAME_SPLITTERWIDTH + m_lTrackListWidth) {
		m_lScaleWidth = EVENTLISTFRAME_SCALEWIDTH;
		m_lPropertyWidth = 0;
		m_lTrackListWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - EVENTLISTFRAME_BORDERWIDTH * 4 - EVENTLISTFRAME_SPLITTERWIDTH;

	}
	else {
		m_lScaleWidth = EVENTLISTFRAME_SCALEWIDTH;
		m_lTrackListWidth = m_lTrackListWidth; 
		m_lPropertyWidth = rcClient.Width () - m_lScaleWidth - m_lTrackListWidth -
			m_lVScrollBarWidth - EVENTLISTFRAME_BORDERWIDTH * 4 - EVENTLISTFRAME_SPLITTERWIDTH;
	}

	// �r���[�̐���
	if (m_pScaleView) {
		m_pScaleView->MoveWindow (EVENTLISTFRAME_BORDERWIDTH, 
			m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT, 
			m_lScaleWidth, m_lScaleHeight);
	}

	if (m_pPropertyScaleView) {
		m_pPropertyScaleView->MoveWindow (EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth, 
			m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT, 
			m_lPropertyWidth, m_lScaleHeight);
	}

	if (m_pIndexScaleView) {
		m_pIndexScaleView->MoveWindow (EVENTLISTFRAME_BORDERWIDTH, 
			m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lScaleWidth, m_lIndexHeight);
	}

	if (m_pIndexPropertyView) {
		m_pIndexPropertyView->MoveWindow (EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth,
			m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lPropertyWidth, m_lIndexHeight);
	}

	// �X�N���[���o�[�̐���
	m_wndColumnScroll.MoveWindow 
		(EVENTLISTFRAME_BORDERWIDTH,
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lIndexHeight,
		m_lScaleWidth + m_lPropertyWidth - m_lVScrollBarWidth * 2, 
		m_lHScrollBarHeight);

	m_wndRowScroll.MoveWindow 
		(EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth + m_lPropertyWidth, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT,
		m_lVScrollBarWidth, 
		m_lScaleHeight + m_lIndexHeight - m_lVScrollBarWidth * 2);

	m_wndSizeScroll.MoveWindow 
		(EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth + m_lPropertyWidth, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lIndexHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// �Y�[���{�^���̐���
	m_wndColumnZoomDown.MoveWindow 
		(EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lPropertyWidth - m_lVScrollBarWidth * 2, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lIndexHeight,
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndColumnZoomUp.MoveWindow 
		(EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lPropertyWidth - m_lVScrollBarWidth, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lIndexHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndRowZoomDown.MoveWindow
		(EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth + m_lPropertyWidth, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lIndexHeight -
		m_lVScrollBarWidth * 2, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndRowZoomUp.MoveWindow
		(EVENTLISTFRAME_BORDERWIDTH + m_lScaleWidth + m_lPropertyWidth, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT + m_lScaleHeight + m_lIndexHeight -
		m_lVScrollBarWidth, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// �g���b�N���X�g�̐���
	m_wndTrackList.MoveWindow
		(EVENTLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth + 
		m_lPropertyWidth + m_lVScrollBarWidth + EVENTLISTFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT, 
		m_lTrackListWidth, m_lTrackListHeight);

	// �C�x���g�̎�ރ��X�g�̐���
	m_wndEventKindList.MoveWindow
		(EVENTLISTFRAME_BORDERWIDTH * 3 + m_lScaleWidth +
		m_lPropertyWidth + m_lVScrollBarWidth + EVENTLISTFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT * 3 + m_lTrackListHeight +
		EVENTLISTFRAME_SPLITTERHEIGHT, 
		m_lTrackListWidth, m_lEventKindListHeight);

	// �X�N���[���o�[�̃T�C�Y���ω������̂ŁA�o�[�̃f�U�C�����Ē�������B
	RecalcRowScrollInfo ();
	RecalcColumnScrollInfo ();

}



// �N���C�A���g�̈�̐���(CFrameWnd::OnCreateClient�̃I�[�o�[���C�h)
BOOL CEventListFrame::OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext) {

	// �T�C�Y�����p�̃_�~�[�r���[����(Visible = FALSE)
	CWnd* pWnd = NULL;
	m_pDummyView = (CView*)CFrameWnd::CreateView (pContext, EVENTLISTFRAME_DUMMYVIEW);
	if (m_pDummyView == NULL) {
			return FALSE;
	}
	m_pDummyView->ShowWindow (SW_HIDE);
	
	// ����p�̃r���[����(Visible = FALSE)
	pContext->m_pNewViewClass = RUNTIME_CLASS (CEventListPrintView);
	m_pPrintView = (CView*)CFrameWnd::CreateView (pContext, EVENTLISTFRAME_PRINTVIEW);
	if (m_pPrintView == NULL) {
			return FALSE;
	}
	m_pPrintView->ShowWindow (SW_HIDE);

	// ����r���[�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CEventListScaleView);
	m_pScaleView = (CView*)CFrameWnd::CreateView (pContext, EVENTLISTFRAME_SCALEVIEW);
	if (m_pScaleView == NULL) {
			return FALSE;
	}
	m_pScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// �㕔���ږ����r���[�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CEventListPropertyScaleView);
	m_pPropertyScaleView = (CView*)CFrameWnd::CreateView (pContext, EVENTLISTFRAME_PROPERTYSCALEVIEW);
	if (m_pPropertyScaleView == NULL) {
			return FALSE;
	}
	m_pPropertyScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// �����ԍ����r���[�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CEventListIndexScaleView);
	m_pIndexScaleView = (CView*)CFrameWnd::CreateView (pContext, EVENTLISTFRAME_INDEXSCALEVIEW);
	if (m_pIndexScaleView == NULL) {
			return FALSE;
	}
	m_pIndexScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// ���X�g���r���[�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CEventListIndexPropertyView);
	m_pIndexPropertyView = (CView*)CFrameWnd::CreateView (pContext, EVENTLISTFRAME_INDEXPROPERTYVIEW);
	if (m_pIndexPropertyView == NULL) {
			return FALSE;
	}
	m_pIndexPropertyView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// �X�N���[���o�[�̐���
	m_wndColumnScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_HORZ, CRect(0,0,0,0), this, EVENTLISTFRAME_COLUMNSCROLL);
	m_wndRowScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, EVENTLISTFRAME_ROWSCROLL);
	m_wndSizeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_SIZEBOX, CRect(0,0,0,0), this, EVENTLISTFRAME_SIZEBOX);
	
	// �Y�[���{�^���ނ̐���
	m_wndColumnZoomDown.Create 
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, EVENTLISTFRAME_COLUMNZOOMDOWN);
	m_wndColumnZoomUp.Create   
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, EVENTLISTFRAME_COLUMNZOOMUP);
	m_wndRowZoomDown.Create  
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, EVENTLISTFRAME_ROWZOOMDOWN);
	m_wndRowZoomUp.Create    
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, EVENTLISTFRAME_ROWZOOMUP);

	// �g���b�N���X�g�̐���
	m_wndTrackList.Create
		(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
		LBS_NOTIFY|LBS_DISABLENOSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT, 
		CRect(0,0,0,0), this, EVENTLISTFRAME_TRACKLIST);

	// �C�x���g�̎�ނ̐���
	m_wndEventKindList.Create
		(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
		LBS_NOTIFY|LBS_DISABLENOSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT, 
		CRect(0,0,0,0), this, EVENTLISTFRAME_EVENTKINDLIST);

	// �t�H�[�J�X�̐ݒ�
	SetActiveView ((CView*)m_pIndexPropertyView);

	// �R���g���[���̈ʒu���킹��WM_SIZE�Ȃǂɂ��RecalcLaoyout�ɔC����B
	return TRUE;

}

// ����p�̃R�}���h���g���b�v(CFrameWnd::OnCmdMsg�̃I�[�o�[���C�h)
BOOL CEventListFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// ����p�̃R�}���h�̏ꍇ�A�����I��CEventListPrintView�ɓn���B
	if ((nID == ID_FILE_PRINT || nID == ID_FILE_PRINT_DIRECT || nID == ID_FILE_PRINT_PREVIEW) &&
		pSekaijuApp->m_bRecording == FALSE) {
		if (m_pPrintView) {
			return ((CEventListPrintView*)m_pPrintView)->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
		}
		return FALSE;
	}
	// ���̑��̃R�}���h�̓f�t�H���g�̏����Ƃ���B
	return CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}



//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
int CEventListFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	CRect rcTemp;

	// �c�[���o�[1�̍쐬
	if (!m_wndToolBar1.Create (this) ||
		!m_wndToolBar1.LoadToolBar (IDR_EVENTLIST1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.SetBarStyle (m_wndToolBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	//m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	//EnableDocking (CBRS_ALIGN_ANY);
	//DockControlBar (&m_wndToolBar1);

	LoadAccelTable (MAKEINTRESOURCE (IDR_EVENTLIST));

	// �C�x���g�g���b�N�R���{�{�b�N�X�̍쐬
	m_wndToolBar1.SetButtonInfo (4, IDC_EVENTTRACKCOMBO, TBBS_SEPARATOR, 120);
	m_wndToolBar1.GetItemRect (4, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndEventTrackCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_EVENTTRACKCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndEventTrackCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// �C�x���g�^�C���G�f�B�b�g�̍쐬
	m_wndToolBar1.SetButtonInfo (6, IDC_EVENTTIMEEDIT, TBBS_SEPARATOR, 120);
	m_wndToolBar1.GetItemRect (6, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndEventTimeEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_EVENTTIMEEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndEventTimeEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �C�x���g��ރR���{�{�b�N�X�̍쐬
	m_wndToolBar1.SetButtonInfo (8, IDC_EVENTKINDCOMBO, TBBS_SEPARATOR, 150);
	m_wndToolBar1.GetItemRect (8, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndEventKindCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_EVENTKINDCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndEventKindCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �C�x���g�`�����l���R���{�{�b�N�X�̍쐬
	m_wndToolBar1.SetButtonInfo (10, IDC_EVENTCHANNELCOMBO, TBBS_SEPARATOR, 60);
	m_wndToolBar1.GetItemRect (10, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 22;
	if (!m_wndEventChannelCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_EVENTCHANNELCOMBO)) {
		TRACE0 ("Failed to create combo box\n");
		return -1;
	}
	m_wndEventChannelCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �e�N���X�̊֐��Ăяo��
	int nRet = CChildFrame::OnCreate (lpCreateStruct);	
	

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;

	// �g���b�N�R���{�̏[��
	UpdateTrackCombo ();
	m_wndEventTrackCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �C�x���g�̎����G�f�B�b�g�̏[��
	CString strText;
	strText.Format (_T("%05d:%02d:%03d"), 1, 1, 0);
	m_wndEventTimeEdit.SetWindowText (strText);

	// �C�x���g�̎�ރR���{�̏[��

	m_wndEventKindCombo.ResetContent ();
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x00]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x01]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x02]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x03]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x04]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x05]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x06]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x07]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x08]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x09]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x20]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x21]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x2F]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x51]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x54]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x58]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x59]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x7F]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x80]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0x90]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xA0]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xB0]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xC0]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xD0]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xE0]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xF0]);
	m_wndEventKindCombo.AddString (pSekaijuApp->m_strEventKindName[0xF7]);
	m_wndEventKindCombo.SetCurSel (0);
	m_wndEventKindCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �C�x���g�̃`�����l���R���{�̏[��
	strText = _T("n/a");
	m_wndEventChannelCombo.AddString (strText);
	int i = 0;
	for (i = 0; i < 16; i++) {
		strText.Format (_T("%d"), i + 1);
		m_wndEventChannelCombo.AddString (strText);
	}
	m_wndEventChannelCombo.SetCurSel (0);
	m_wndEventChannelCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �g���b�N���X�g�̏[��
	UpdateTrackList ();
	m_wndTrackList.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	long lCount = m_wndTrackList.GetCount ();
	i = 0;
	if (pSekaijuDoc->m_pTempTrack) {
		forEachTrack (pMIDIData, pMIDITrack) {
			if (0 <= i && i < lCount) {
				m_wndTrackList.SetCheck (i, pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
				m_bTrackVisible[i] = (pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
			}
			i++;
		}
	}
	else {
		forEachTrack (pMIDIData, pMIDITrack) {
			if (0 <= i && i < lCount) {
				m_wndTrackList.SetCheck (i, TRUE);
				m_bTrackVisible[i] = (TRUE);
			}
			i++;
		}
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;

	// �J�����g�g���b�N�ƃJ�����g�`�����l�����X�V
	long lTrackIndex = 0;
	if (pSekaijuDoc->m_pTempTrack) {
		lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	}
	else if (MIDIData_GetFormat (pMIDIData) == 1 && lCount >= 2) {
		lTrackIndex = 1;
	}
	ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	if (m_bTrackVisible[lTrackIndex]) {
		SetCurTrackIndex (lTrackIndex);
		pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lOutputChannel && lOutputChannel <= 15) {
				SetCurChannel (lOutputChannel);
			}
		}
	}

	// �O���t�̎�ރ��X�g�̏[��
	CString strIncludingVelocity0NoteOn;
	CString strUnknownEventKind;
	VERIFY (strIncludingVelocity0NoteOn.LoadString (IDS_INCLUDING_VELOCITY0_NOTEON));
	VERIFY (strUnknownEventKind.LoadString (IDS_UNKNOWN_EVENT_KIND));
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x00]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x01]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x02]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x03]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x04]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x05]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x06]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x07]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x08]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x09]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x20]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x21]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x2F]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x51]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x54]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x58]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x59]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x7F]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x80] + strIncludingVelocity0NoteOn);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0x90]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xA0]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xB0]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xC0]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xD0]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xE0]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xF0]);
	m_wndEventKindList.AddString (pSekaijuApp->m_strEventKindName[0xF7]);
	m_wndEventKindList.AddString (strUnknownEventKind);
	m_wndEventKindList.SetCurSel (0);
	m_wndEventKindList.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	lCount = m_wndEventKindList.GetCount ();
	for (i = 0; i < lCount; i++) {
		m_wndEventKindList.SetCheck (i, IsEventKindVisible (i) ? 1 : 0);
	}

	// �J�����g�`�����l���̎����I��
	if (pSekaijuDoc->m_pTempTrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pSekaijuDoc->m_pTempTrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}

	// ���̃C�x���g���X�g�̍쐬	
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);

	// �����y�[�W�X�V�̐ݒ�
	if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) {
		m_bAutoPageUpdate = TRUE;
	}

	SetActiveView (m_pIndexPropertyView, FALSE);
	m_pIndexPropertyView->SetFocus ();

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	
	return nRet;
}

// �E�B���h�E�j����
void CEventListFrame::OnDestroy () {
	CChildFrame::OnDestroy ();
}

// �E�B���h�E�T�C�Y�ύX��
void CEventListFrame::OnSize (UINT nType, int cx, int cy) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// ��{�N���X�̊֐��Ăяo��
	CChildFrame::OnSize (nType, cx, cy);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �^�C�}�[�Ăяo����
void CEventListFrame::OnTimer (UINT nIDEvent) {
	;
}

// �w�i����(CFrameWnd::OnEraseBkgnd�̃I�[�o�[���C�h)
BOOL CEventListFrame::OnEraseBkgnd (CDC* pDC) {
	return 0;
}

// �E�B���h�E���A�N�e�B�u�ɂȂ����Ƃ�
void CEventListFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
	CChildFrame::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);
}

// ����{�^���������ꂽ�Ƃ�
void CEventListFrame::OnClose () {
	_RPTF0 (_CRT_WARN, ("CEventListFrame::OnClose ()\n"));
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// �C���v���[�X�G�f�B�b�g�y�уC���v���[�X���X�g�{�b�N�X�̏I��
	if (m_pIndexPropertyView) {
		CEventListIndexPropertyView* pIndexPropertyView = (CEventListIndexPropertyView*)m_pIndexPropertyView;
		if (pIndexPropertyView->IsTextEditing ()) {
			pIndexPropertyView->EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
		}
		if (pIndexPropertyView->IsListSelecting ()) {
			pIndexPropertyView->EndListSelectingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
		}
	}
	CChildFrame::OnClose ();
}

// �`�悷��Ƃ�
void CEventListFrame::OnPaint () {
	CPaintDC dc (this);
	CRect rcClient;
	GetClientRect (&rcClient);
	// �������̂��ڂݕ`��
	CRect rcClient1 (rcClient);
	rcClient1.top = m_lToolBar1Height;
	rcClient1.right = EVENTLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + 
		m_lPropertyWidth + m_lVScrollBarWidth;
	dc.Draw3dRect (&rcClient1, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient1.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient1, RGB (0, 0, 0), RGB (192, 192, 192));
	// �E�㕔���̂��ڂݕ`��
	CRect rcClient2 (rcClient);
	rcClient2.top = m_lToolBar1Height;
	rcClient2.left = EVENTLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + 
		m_lPropertyWidth + m_lVScrollBarWidth + EVENTLISTFRAME_SPLITTERWIDTH;
	rcClient2.bottom = m_lToolBar1Height + 
		EVENTLISTFRAME_BORDERHEIGHT * 2 + m_lTrackListHeight;
	dc.Draw3dRect (&rcClient2, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient2.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient2, RGB (0, 0, 0), RGB (192, 192, 192));
	// �E�������̂��ڂݕ`��
	CRect rcClient3 (rcClient);
	rcClient3.top = m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT * 2 + 
		EVENTLISTFRAME_SPLITTERHEIGHT + m_lTrackListHeight;
	rcClient3.left = EVENTLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + 
		m_lPropertyWidth + m_lVScrollBarWidth + EVENTLISTFRAME_SPLITTERWIDTH;
	dc.Draw3dRect (&rcClient3, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient3.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient3, RGB (0, 0, 0), RGB (192, 192, 192));
	// �����E�����̕`��
	CBrush brush;
	brush.CreateSolidBrush (::GetSysColor (COLOR_3DFACE));
	CRect rcClient5 (rcClient);
	rcClient5.left = rcClient2.left - 1;
	rcClient5.top = rcClient2.bottom + 1;
	rcClient5.bottom = rcClient3.top - 1;
	dc.FillRect (&rcClient5, &brush);
	// �c���E�����̕`��
	CRect rcClient6 (rcClient);
	rcClient6.left = rcClient1.right + 1;
	rcClient6.right = rcClient2.left - 1;
	dc.FillRect (&rcClient6, &brush);

}

// �}�E�X���{�^�������ꂽ��  
void CEventListFrame::OnLButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// �㔼��(�g���b�N���X�g��)�Ɖ�����(�C�x���g�̎�ޕ�)�̋��E������h���b�O����ƁA���E�����㉺�Ɉړ�
	if (m_bSplitterMovingH || m_bSplitterMovingV) {
		SetCapture ();
		m_ptMouseDown = m_ptMouseMoveOld = point;
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, point);
		ReleaseDC (pDC);
		if (m_bSplitterMovingH && m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeAll);
		}
		else if (m_bSplitterMovingH) {
			::SetCursor (pSekaijuApp->m_hCursorResizeNS);
		}
		else if (m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeWE);
		}
	}

}

// �}�E�X�E�{�^�������ꂽ��  
void CEventListFrame::OnRButtonDown (UINT nFlags, CPoint point) {

}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CEventListFrame::OnLButtonUp (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (GetCapture () == this) {
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, m_ptMouseMoveOld);
		ReleaseDC (pDC);
		ReleaseCapture ();
		::SetCursor (pSekaijuApp->m_hCursorArrow);
		CPoint ptDelta = point - m_ptMouseDown;
		if (m_bSplitterMovingH) {
			m_lTrackListHeight = CLIP (0, (m_lTrackListHeight + ptDelta.y), 1200);
		}
		if (m_bSplitterMovingV) {
			m_lTrackListWidth = CLIP (0, (m_lTrackListWidth - ptDelta.x), 1600);
		}
		RecalcLayout ();
		m_bSplitterMovingH = FALSE;
		m_bSplitterMovingV = FALSE;
		Invalidate ();
	}

}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CEventListFrame::OnRButtonUp (UINT nFlags, CPoint point) {

}

// �}�E�X���������ꂽ�Ƃ�
void CEventListFrame::OnMouseMove (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// �L���v�^�[��
	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, m_ptMouseMoveOld);
		DrawSplitterCaptor (pDC, point);
		ReleaseDC (pDC);
		m_ptMouseMoveOld = point;
	}
	// ��L���v�^�[��
	else {
		// �J�[�\���������X�v���b�^�[��ɂ��邩
		m_bSplitterMovingH =
			m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT * 2 + m_lTrackListHeight <= point.y &&
			point.y < m_lToolBar1Height + EVENTLISTFRAME_BORDERHEIGHT * 2 + m_lTrackListHeight + 
			EVENTLISTFRAME_SPLITTERHEIGHT &&
			point.x >= EVENTLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lPropertyWidth + m_lVScrollBarWidth;
		// �J�[�\���������X�v���b�^�[��ɂ��邩
		m_bSplitterMovingV =
			EVENTLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lPropertyWidth + m_lVScrollBarWidth <= point.x &&
			point.x < EVENTLISTFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lPropertyWidth + m_lVScrollBarWidth +
			EVENTLISTFRAME_SPLITTERWIDTH;
		// �J�[�\���������X�v���b�^�[�Ɛ����X�v���b�^�[�̌������ɂ���ꍇ
		if (m_bSplitterMovingH && m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeAll);
		}
		// �J�[�\���������X�v���b�^�[��ɂ���ꍇ
		else if (m_bSplitterMovingH) {
			::SetCursor (pSekaijuApp->m_hCursorResizeNS);
		}
		// �J�[�\���������X�v���b�^�[��ɂ���ꍇ
		else if (m_bSplitterMovingV) {
			::SetCursor (pSekaijuApp->m_hCursorResizeWE);
		}
		// �J�[�\�����X�v���b�^�[�ɂȂ��ꍇ
		else {
			::SetCursor (pSekaijuApp->m_hCursorArrow);
		}
	}

}

// ������Y�[���_�E��(20091220:���[�ʒu�ێ��@�\�ǉ�)
void CEventListFrame::OnColumnZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldColumnZoom = m_lColumnZoom;
	long lOldColumnPos = m_wndColumnScroll.GetScrollPos ();
	long lNewColumnZoom = CLIP (2, m_lColumnZoom - 1, 16);
	long lNewColumnPos = lOldColumnPos * lNewColumnZoom / lOldColumnZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lColumnZoom = lNewColumnZoom;
	RecalcColumnScrollInfo ();
	m_wndColumnScroll.SetScrollPos (lNewColumnPos);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
	m_pPropertyScaleView->Invalidate ();
	//long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	//long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (lCurRow, lCurColumn);
	m_pIndexPropertyView->Invalidate ();
	m_pIndexPropertyView->SetFocus ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ������Y�[���A�b�v(20091220:���[�ʒu�ێ��@�\�ǉ�)
void CEventListFrame::OnColumnZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldColumnZoom = m_lColumnZoom;
	long lOldColumnPos = m_wndColumnScroll.GetScrollPos ();
	long lNewColumnZoom = CLIP (2, m_lColumnZoom + 1, 16);
	long lNewColumnPos = lOldColumnPos * lNewColumnZoom / lOldColumnZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lColumnZoom = lNewColumnZoom;
	RecalcColumnScrollInfo ();
	m_wndColumnScroll.SetScrollPos (lNewColumnPos);
	m_lColumnScrollPos = m_wndColumnScroll.GetScrollPos ();
	m_pPropertyScaleView->Invalidate ();
	//long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	//long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (lCurRow, lCurColumn);
	m_pIndexPropertyView->Invalidate ();
	m_pIndexPropertyView->SetFocus ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �s�����Y�[���_�E��(20091220:��[�ʒu�ێ��@�\�ǉ��A�����y�[�W�X�V�����I�t�ǉ�)
void CEventListFrame::OnRowZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldRowZoom = m_lRowZoom;
	long lOldRowPos = m_wndRowScroll.GetScrollPos ();
	long lNewRowZoom = CLIP (16, m_lRowZoom - 2, 32);
	long lNewRowPos = lOldRowPos * lNewRowZoom / lOldRowZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lRowZoom = lNewRowZoom;
	RecalcRowScrollInfo ();
	m_wndRowScroll.SetScrollPos (lNewRowPos);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
	m_pIndexScaleView->Invalidate ();
	//long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	//long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (lCurRow, lCurColumn);
	m_pIndexPropertyView->Invalidate ();
	m_pIndexPropertyView->SetFocus ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �s�����Y�[���A�b�v(20091220:��[�ʒu�ێ��@�\�ǉ��A�����y�[�W�X�V�����I�t�ǉ�)
void CEventListFrame::OnRowZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldRowZoom = m_lRowZoom;
	long lOldRowPos = m_wndRowScroll.GetScrollPos ();
	long lNewRowZoom = CLIP (16, m_lRowZoom + 2, 32);
	long lNewRowPos = lOldRowPos * lNewRowZoom / lOldRowZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lRowZoom = lNewRowZoom;
	RecalcRowScrollInfo ();
	m_wndRowScroll.SetScrollPos (lNewRowPos);
	m_lRowScrollPos = m_wndRowScroll.GetScrollPos ();
	m_pIndexScaleView->Invalidate ();
	//long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	//long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (lCurRow, lCurColumn);
	//((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (lCurRow, lCurColumn);
	m_pIndexPropertyView->Invalidate ();
	m_pIndexPropertyView->SetFocus ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �����X�N���[��
void CEventListFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndColumnScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lColumnScrollPos;
		switch (nSBCode) {
		case SB_LINELEFT:
			lNewPos = m_lColumnScrollPos - m_lColumnZoom;
			break;
		case SB_LINERIGHT:
			lNewPos = m_lColumnScrollPos + m_lColumnZoom;
			break;
		case SB_PAGELEFT:
			lNewPos = m_lColumnScrollPos - m_lColumnZoom * 4;
			break;
		case SB_PAGERIGHT:
			lNewPos = m_lColumnScrollPos + m_lColumnZoom * 4;
			break;
		case SB_LEFT: // 20100206�ǉ�
			lNewPos = nMin;
			break;
		case SB_RIGHT: // 20100206�ǉ�
			lNewPos = nMax;
			break;
		case SB_THUMBTRACK:
			lNewPos = nPos;
			break;
		}
		SetColumnScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_pIndexPropertyView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �����X�N���[��
void CEventListFrame::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndRowScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lRowScrollPos;
		switch (nSBCode) {
		case SB_LINEUP:
			lNewPos = m_lRowScrollPos - m_lRowZoom;
			break;
		case SB_LINEDOWN:
			lNewPos = m_lRowScrollPos + m_lRowZoom;
			break;
		case SB_PAGEUP:
			lNewPos = m_lRowScrollPos -
				__max (1, GetVisibleBottomRow () - GetVisibleTopRow ()) * m_lRowZoom;;
			break;
		case SB_PAGEDOWN:
			lNewPos = m_lRowScrollPos + 
				__max (1, GetVisibleBottomRow () - GetVisibleTopRow ()) * m_lRowZoom;
			break;
		case SB_TOP: // 20100206�ǉ�
			lNewPos = nMin;
			break;
		case SB_BOTTOM: // 20100206�ǉ�
			lNewPos = nMax;
			break;
		case SB_THUMBTRACK:
			lNewPos = nPos;
			break;
		}
		SetRowScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_bAutoPageUpdate = FALSE;
		m_pIndexPropertyView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// �w�c�[��(&T)�x-�w�C�x���g�̑}��(&I)�x
void CEventListFrame::OnEventListInsertEvent () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	// ���A���^�C�����͒��͉������Ȃ��B
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ��B
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}

	CString strText;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	// �}���g���b�N
	long lTargetTrackIndex = m_wndEventTrackCombo.GetCurSel ();
	MIDITrack* pTargetTrack = pSekaijuDoc->GetTrack (lTargetTrackIndex);
	if (pTargetTrack == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_THE_EVENT_TO_THIS_TRACK));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// �}������
	long lTargetTime = 0;
	m_wndEventTimeEdit.GetWindowText (strText);
	long lErrorID = pSekaijuDoc->StringTimeToLongTime (pMIDIData, strText, &lTargetTime);
	if (lErrorID > 0) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (lErrorID));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// �}���C�x���g�̎��
	long lTargetKindIndex = m_wndEventKindCombo.GetCurSel ();
	long lTargetKind = ListIndextoEventKind (lTargetKindIndex);
	ASSERT (0x00 <= lTargetKind && lTargetKind <= 0xFF);
	
	// ����EOT������ꍇ�AEOT�̓�d�}���͂ł��Ȃ��B
	if (lTargetKind == MIDIEVENT_ENDOFTRACK) {
		MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pTargetTrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_ENDOFTRACK_TO_THIS_TRACK_ANY_MORE));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				return;
			}
		}
	}

	// MIDI�`�����l���C�x���g�̏ꍇ�̓`�����l����񖳎�
	if (0x80 <= lTargetKind && lTargetKind <= 0xEF) {
		lTargetKind &= 0xF0; 
	}

	// �t�H�[�}�b�g1��MIDI�f�[�^�ɑ΂���C�x���g��ނ̐���������
	long lFormat = MIDIData_GetFormat (pMIDIData);
	if (lFormat == 1) {
		// �ŏ��̃g���b�N�̏ꍇ
		if (pTargetTrack == MIDIData_GetFirstTrack (pMIDIData)) {
			if (0x80 <= lTargetKind && lTargetKind <= 0xEF) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_THIS_EVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				return;
			}
		}
		// 2�Ԗڈȍ~�̃g���b�N�̏ꍇ
		else {
			if (lTargetKind == MIDIEVENT_TEMPO ||
				lTargetKind == MIDIEVENT_SMPTEOFFSET ||
				lTargetKind == MIDIEVENT_TIMESIGNATURE ||
				lTargetKind == MIDIEVENT_KEYSIGNATURE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_THIS_EVENT_TO_THE_SECOND_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				return;
			}
		}
	}
	
	// �m�[�g�I�t�̓m�[�g�I��+�m�[�g�I�t�C�x���g�ɏC��
	if (0x80 <=lTargetKind && lTargetKind <= 0x8F) {
		lTargetKind = MIDIEVENT_NOTEONNOTEOFF;
	}
	// �m�[�g�I���̓m�[�g�I��+�m�[�g�I��0�C�x���g�ɏC��
	else if (0x90 <=lTargetKind && lTargetKind <= 0x9F) {
		lTargetKind = MIDIEVENT_NOTEONNOTEON0;
	}

	// �}���`�����l��
	long lTargetCh = m_wndEventChannelCombo.GetCurSel () - 1;

	long i = 0;
	long lRet = 0;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;

	// �V�����C�x���g�̍쐬
	MIDIEvent* pNewMIDIEvent = MIDIEvent_CreateTextEvent (lTargetTime, _T(""));
	if (pNewMIDIEvent == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_INSUFFICIENT_MEMORY));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	// �C�x���g�̎�ނ�ύX
	MIDIEvent_SetKind (pNewMIDIEvent, lTargetKind);

	// MIDI�C�x���g�̏ꍇ�`�����l����ύX����B
	if (MIDIEvent_IsMIDIEvent (pNewMIDIEvent) && 0 <= lTargetCh && lTargetCh < 16) {
		MIDIEvent_SetChannel (pNewMIDIEvent, lTargetCh);
	}

	// �m�[�g�C�x���g�Œ���0�ȉ��̏ꍇ������4�������Ƃ���B
	if (MIDIEvent_IsNoteOn (pNewMIDIEvent) && MIDIEvent_IsNote (pNewMIDIEvent)) {
		if (MIDIEvent_GetDuration (pNewMIDIEvent) <= 0) {
			long lNewDuration = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
			MIDIEvent_SetDuration (pNewMIDIEvent, CLIP (1, lNewDuration, 65535));
		}
	}

	// �����̕ێ�
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_INSERT_EVENT));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pTargetTrack);
	MIDIEvent* pNewLastEvent = NULL;
	if (pLastEvent) {
		if (MIDIEvent_IsEndofTrack (pLastEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
			VERIFY (pNewLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
			this->ReplaceVisibleEvent (pLastEvent, pNewLastEvent);
		}
	}

	// �^�[�Q�b�g�^�C�����}�����钼�O���͒���̃C�x���g���擾(20100226�C��)
	MIDIEvent* pTargetMIDIEvent = NULL;
	// �������C�x���g�̌���ɑ}������ꍇ(20100226�C��)
	if (pSekaijuApp->m_theEventListOption.m_bInsertEventAfter) {
		MIDIEvent* pEvent = NULL;
		forEachEventInverse (pTargetTrack, pEvent) {
			long lTime = MIDIEvent_GetTime (pEvent);
			if (lTime <= lTargetTime && !MIDIEvent_IsEndofTrack (pEvent)) {
				break;
			}
		}
		pTargetMIDIEvent = pEvent;
	}
	// �������C�x���g�̑O���ɑ}������ꍇ(20100226�C��)
	else {
		MIDIEvent* pEvent = NULL;
		forEachEvent (pTargetTrack, pEvent) {
			long lTime = MIDIEvent_GetTime (pEvent);
			if (lTime == lTargetTime && !MIDIEvent_IsNoteOff (pEvent) ||
				lTime > lTargetTime) {
				break;
			}
		}
		pTargetMIDIEvent = pEvent;
	}

	// �^�[�Q�b�g�g���b�N�֐V�����C�x���g��}������(20100226�C��)
	if (pSekaijuApp->m_theEventListOption.m_bInsertEventAfter) { // 20090226�ǉ�
		lRet = MIDITrack_InsertEventAfter (pTargetTrack, pNewMIDIEvent, pTargetMIDIEvent);
	}
	else { // 20090226�ǉ�
		lRet = MIDITrack_InsertEventBefore (pTargetTrack, pNewMIDIEvent, pTargetMIDIEvent);
	}
	if (lRet == 0) {
		MIDIEvent_Delete (pNewMIDIEvent);
		if (pNewLastEvent) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewLastEvent));
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_INSERT_EVENT_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	// �����̕ێ�
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewMIDIEvent));
	if (pNewLastEvent) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewLastEvent));
	}

	// �w�肵���g���b�N����\���̏ꍇ�A�\����Ԃɂ���B
	if (IsTrackVisible (lTargetTrackIndex) == FALSE) {
		this->SetTrackVisible (lTargetTrackIndex);
	}
	// �w�肵���C�x���g�̎�ނ���\���̏ꍇ�A�\����Ԃɂ���B
	if (IsEventKindVisible (lTargetKindIndex) == FALSE) {
		this->SetEventKindVisible (lTargetKindIndex);
	}
	// �r���[�̍X�V
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �s�����X�N���[�����̍X�V
	this->RecalcRowScrollInfo ();
	// �t�H�[�J�X��V�����C�x���g�ɍ��킹��B
	long lCount = m_theVisibleEventArray.GetSize ();
	for (i = 0; i < lCount; i++) {
		if (m_theVisibleEventArray.GetAt (i) == pNewMIDIEvent) {
			long j = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = i;
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (i, j);
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (i, j);
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (i, j);
			return;
		}
	}
	return;
}

// �w�c�[��(&T)�x-�w�C�x���g�̑}��(&I)�x
void CEventListFrame::OnUpdateEventListInsertEventUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// ���A���^�C�����͒��͉������Ȃ��B
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ��B
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	long lCount = m_theVisibleEventArray.GetSize ();
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	if (0 <= lCurRow && lCurRow < lCount) {
		pCmdUI->Enable (TRUE);
	}
	else {
		pCmdUI->Enable (FALSE);
	}
}

// �w�c�[��(&T)�x-�w�C�x���g�̕���(&D)�x
void CEventListFrame::OnEventListDuplicateEvent () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// ���A���^�C�����͒��͉������Ȃ��B
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ��B
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	
	CString strText;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	long i = 0;
	long lRet = 0;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	MIDIEvent* pSrcMIDIEvent = NULL;
	MIDIEvent* pSrcPrevEvent = NULL;
	MIDIEvent* pSrcNextEvent = NULL;
	MIDITrack* pSrcMIDITrack = NULL;
	MIDIEvent* pNewMIDIEvent = NULL;
	pSrcMIDIEvent = GetVisibleEvent (lCurRow);
	
	// ���ʌ��C�x���g�`�F�b�N
	if (pSrcMIDIEvent == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_CURRENT_MIDIEVENT_IS_EMPTY));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	pSrcMIDIEvent = MIDIEvent_GetFirstCombinedEvent (pSrcMIDIEvent); // 20100226�ǉ�
	pSrcPrevEvent = GetVisibleEvent (lCurRow - 1);

	pSrcNextEvent = GetVisibleEvent (lCurRow + 1);
	pSrcMIDITrack = MIDIEvent_GetParent (pSrcMIDIEvent);
	long lSrcTime = MIDIEvent_GetTime (pSrcMIDIEvent);
	long lSrcTrackIndex = pSekaijuDoc->GetTrackIndex (pSrcMIDITrack);
	long lSrcEventKind = MIDIEvent_GetKind (pSrcMIDIEvent);
	long lSrcKindIndex = EventKindtoListIndex (lSrcEventKind);

	// ����EOT������ꍇ�AEOT�̓�d�}���͂ł��Ȃ��B
	if (lSrcEventKind == MIDIEVENT_ENDOFTRACK) {
		MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pSrcMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_ENDOFTRACK_TO_THIS_TRACK_ANY_MORE));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				return;
			}
		}
	}

	// �V�����C�x���g�̍쐬
	pNewMIDIEvent = MIDIEvent_CreateClone (pSrcMIDIEvent);
	if (pNewMIDIEvent == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_INSUFFICIENT_MEMORY));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// �m�[�g�C�x���g�Œ���0�ȉ��̏ꍇ������4�������Ƃ���B
	if (MIDIEvent_IsNoteOn (pNewMIDIEvent) && MIDIEvent_IsNote (pNewMIDIEvent)) {
		if (MIDIEvent_GetDuration (pNewMIDIEvent) <= 0) {
			long lNewDur = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
			MIDIEvent_SetDuration (pNewMIDIEvent, CLIP (1, lNewDur, 65535));
		}
	}

	// �����̕ێ�
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_INSERT_EVENT));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pSrcMIDITrack);
	MIDIEvent* pNewLastEvent = NULL;
	if (pLastEvent) {
		if (MIDIEvent_IsEndofTrack (pLastEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
			VERIFY (pNewLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
			this->ReplaceVisibleEvent (pLastEvent, pNewLastEvent);
		}
	}

	// ReplaceEvent�ɂ��C�x���g�������N���������ߕ��ʌ��C�x���g�Đݒ�
	pSrcMIDIEvent = GetVisibleEvent (lCurRow);
	pSrcMIDIEvent = MIDIEvent_GetFirstCombinedEvent (pSrcMIDIEvent); // 20100226�ǉ�
	pSrcPrevEvent = GetVisibleEvent (lCurRow - 1);
	pSrcNextEvent = GetVisibleEvent (lCurRow + 1);
	lSrcTime = MIDIEvent_GetTime (pSrcMIDIEvent);
	if (pSrcMIDIEvent) {
		pSrcMIDITrack = MIDIEvent_GetParent (pSrcMIDIEvent);
	}

	// �}����g���b�N�֐V�����C�x���g��}������
	if (pSekaijuApp->m_theEventListOption.m_bDuplicateEventAfter) { // 20100226�ǉ�
		lRet = MIDITrack_InsertEventAfter (pSrcMIDITrack, pNewMIDIEvent, pSrcMIDIEvent);
	}
	else { // 20100226�ǉ�
		lRet = MIDITrack_InsertEventBefore (pSrcMIDITrack, pNewMIDIEvent, pSrcMIDIEvent);
	}
	if (lRet == 0) {
		MIDIEvent_Delete (pNewMIDIEvent);
		if (pNewLastEvent) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewLastEvent));
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_MIDIEVENT_DUPLICATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// �����̕ێ�
	VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewMIDIEvent));
	if (pNewLastEvent) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewLastEvent));
	}

	// �w�肵���g���b�N����\���̏ꍇ�A�\����Ԃɂ���B
	if (IsTrackVisible (lSrcTrackIndex) == FALSE) {
		this->SetTrackVisible (lSrcTrackIndex);
	}
	// �w�肵���C�x���g�̎�ނ���\���̏ꍇ�A�\����Ԃɂ���B
	if (IsEventKindVisible (lSrcKindIndex) == FALSE) {
		this->SetEventKindVisible (lSrcKindIndex);
	}
	// �r���[�̍X�V
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �s�����X�N���[�����̍X�V
	this->RecalcRowScrollInfo ();
	// �t�H�[�J�X��V�����C�x���g�ɍ��킹��B
	long lCount = m_theVisibleEventArray.GetSize ();
	for (i = 0; i < lCount; i++) {
		if (m_theVisibleEventArray.GetAt (i) == pNewMIDIEvent) {
			long j = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = i;
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (i, j);
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (i, j);
			((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (i, j);
			return;
		}
	}
	return;
}


// �w�c�[��(&T)�x-�w�C�x���g�̕���(&D)�x
void CEventListFrame::OnUpdateEventListDuplicateEventUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// ���A���^�C�����͒��͉������Ȃ��B
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ��B
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	long lCount = m_theVisibleEventArray.GetSize ();
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	if (0 <= lCurRow && lCurRow < lCount) {
		pCmdUI->Enable (TRUE);
	}
	else {
		pCmdUI->Enable (FALSE);
	}
}


// �w�c�[��(&T)�x-�w�C�x���g��1�s�폜(&D)�x
void CEventListFrame::OnEventListDeleteEvent () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// ���A���^�C�����͒��͉������Ȃ��B
	if (pSekaijuApp->m_bRecording) {
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ��B
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	MIDIEvent* pSrcMIDIEvent = NULL;
	MIDIEvent* pSrcPrevEvent = NULL;
	MIDIEvent* pSrcPrevPrevEvent = NULL;
	MIDIEvent* pSrcNextEvent = NULL;
	MIDIEvent* pSrcNextNextEvent = NULL;
	long i;
	long lCount = m_theVisibleEventArray.GetSize ();
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	// �폜����C�x���g�̓���
	if (0 <= lCurRow && lCurRow < lCount) {
		CSekaijuDoc* pSekaijuDoc = GetDocument ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		pSrcMIDIEvent = (MIDIEvent*)(m_theVisibleEventArray.GetAt (lCurRow));
		if (lCurRow >= 1) {
			pSrcPrevEvent = (MIDIEvent*)(m_theVisibleEventArray.GetAt (lCurRow - 1));
		}
		if (lCurRow >= 2) {
			pSrcPrevPrevEvent = (MIDIEvent*)(m_theVisibleEventArray.GetAt (lCurRow - 2));
		}
		if (lCurRow < lCount - 1) {
			pSrcNextEvent = (MIDIEvent*)(m_theVisibleEventArray.GetAt (lCurRow + 1));
		}
		if (lCurRow < lCount - 2) {
			pSrcNextNextEvent = (MIDIEvent*)(m_theVisibleEventArray.GetAt (lCurRow + 2));
		}
		// �g���b�N�Ō�̃G���h�I�u�g���b�N�C�x���g�͍폜�ł��Ȃ��B
		if (MIDIEvent_IsEndofTrack (pSrcMIDIEvent) && MIDIEvent_GetNextEvent (pSrcMIDIEvent) == NULL) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_DELETE_THE_LAST_ENDOFTRACK_IN_A_TRACK));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			return;
		}
		// �����L�^
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_DELETE_EVENT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pSrcMIDIEvent));

		// �C�x���g������
		MIDITrack* pSrcMIDITrack = MIDIEvent_GetParent (pSrcMIDIEvent);
		MIDITrack_RemoveEvent (pSrcMIDITrack, pSrcMIDIEvent);
		// �r���[�̍X�V
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		// �s�����X�N���[�����̍X�V
		RecalcRowScrollInfo ();	
		// ���O�̃C�x���g�Ƀt�H�[�J�X�����킹��
		if (!pSekaijuApp->m_theEventListOption.m_bDeleteEventAfter) {
			// �O�̃C�x���g������΂���Ƀt�H�[�J�X�����킹��B
			if (pSrcPrevEvent) {
				lCount = m_theVisibleEventArray.GetSize ();
				for (i = 0; i < lCount; i++) {
					if (m_theVisibleEventArray.GetAt (i) == pSrcPrevEvent) {
						long j = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = i;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (i, j);
						return;
					}
				}
			}
			// �O�̑O�̃C�x���g������΂���Ƀt�H�[�J�X�����킹��B
			if (pSrcPrevPrevEvent) {
				lCount = m_theVisibleEventArray.GetSize ();
				for (i = 0; i < lCount; i++) {
					if (m_theVisibleEventArray.GetAt (i) == pSrcPrevPrevEvent) {
						long j = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = i;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (i, j);
						return;
					}
				}
			}
			// ��L�̂�����ł��Ȃ��ꍇ�A�����C���f�b�N�X�̂��̂Ƀt�H�[�J�X�����킹��B
		}
		// ����̃C�x���g�Ƀt�H�[�J�X�����킹��
		else {
			// ���̃C�x���g������΂���Ƀt�H�[�J�X�����킹��B
			if (pSrcNextEvent) {
				lCount = m_theVisibleEventArray.GetSize ();
				for (i = 0; i < lCount; i++) {
					if (m_theVisibleEventArray.GetAt (i) == pSrcNextEvent) {
						long j = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = i;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (i, j);
						return;
					}
				}
			}
			// ���̎��̃C�x���g������΂���Ƀt�H�[�J�X�����킹��B
			if (pSrcNextNextEvent) {
				lCount = m_theVisibleEventArray.GetSize ();
				for (i = 0; i < lCount; i++) {
					if (m_theVisibleEventArray.GetAt (i) == pSrcNextNextEvent) {
						long j = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow = i;
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveTextBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->MoveListBox (i, j);
						((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (i, j);
						return;
					}
				}
			}
			// ��L�̂�����ł��Ȃ��ꍇ�A�����C���f�b�N�X�̂��̂Ƀt�H�[�J�X�����킹��B
		}
	}
	// �����Ȃ��s�͍폜�ł��Ȃ��B
	else {
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_CURRENT_MIDIEVENT_IS_EMPTY));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
}

// �w�c�[��(&T)�x-�w�C�x���g��1�s�폜(&D)�x
void CEventListFrame::OnUpdateEventListDeleteEventUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// ���A���^�C�����͒��͉������Ȃ��B
	if (pSekaijuApp->m_bRecording) {
		pCmdUI->Enable (FALSE);
		return;
	}
	// MIDI�f�[�^���ҏW���b�N����Ă���ꍇ�͉������Ȃ��B
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	MIDIEvent* pMIDIEvent = NULL;
	long lCount = m_theVisibleEventArray.GetSize ();
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	if (0 <= lCurRow && lCurRow < lCount) {
		pMIDIEvent = (MIDIEvent*)(m_theVisibleEventArray.GetAt (lCurRow));
		if (MIDIEvent_IsEndofTrack (pMIDIEvent) && MIDIEvent_GetNextEvent (pMIDIEvent) == NULL) {
			pCmdUI->Enable (FALSE);
		}
		else {
			pCmdUI->Enable (TRUE);
		}
	}
	else {
		pCmdUI->Enable (FALSE);
	}

}


// �w�c�[��(&T)�x-�w���݂̃g���b�N�̂ݕ\��(&C)�x
void CEventListFrame::OnEventListOnlyCurTrack () {
	int nCount = m_wndTrackList.GetCount ();
	int nCurSel = m_wndTrackList.GetCurSel ();
	if (m_bOnlyCurTrack) {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = FALSE;
	}
	else {
		m_bOnlyCurTrack = TRUE;
		m_bShowAllTrack = FALSE;
	}
	for (int i = 0; i < nCount; i++) {
		m_wndTrackList.SetCheck (i, IsTrackVisible (i) ? 1 : 0);
	}
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�c�[��(&T)�x-�w���݂̃g���b�N�̂ݕ\��(&C)�x
void CEventListFrame::OnUpdateEventListOnlyCurTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bOnlyCurTrack);
}

// �w�c�[��(&T)�x-�w�S�Ẵg���b�N��\��(&A)�x
void CEventListFrame::OnEventListShowAllTrack () {
	int nCount = m_wndTrackList.GetCount ();
	int nCurSel = m_wndTrackList.GetCurSel ();
	if (m_bShowAllTrack) {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = FALSE;
	}
	else {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = TRUE;
	}
	for (int i = 0; i < nCount; i++) {
		m_wndTrackList.SetCheck (i, IsTrackVisible (i) ? 1 : 0);
	}
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();

}	


// �w�c�[��(&T)�x-�w�S�Ẵg���b�N��\��(&A)�x
void CEventListFrame::OnUpdateEventListShowAllTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bShowAllTrack);
}

// �w�c�[��(&T)�x-�w���݂̃O���t�̂ݕ\��(&C)�x
void CEventListFrame::OnEventListOnlyCurEventKind () {
	int nCount = m_wndEventKindList.GetCount ();
	int nCurSel = m_wndEventKindList.GetCurSel ();
	if (m_bOnlyCurEventKind) {
		m_bOnlyCurEventKind = FALSE;
		m_bShowAllEventKind = FALSE;
	}
	else {
		m_bOnlyCurEventKind = TRUE;
		m_bShowAllEventKind = FALSE;
	}
	for (int i = 0; i < nCount; i++) {
		m_wndEventKindList.SetCheck (i, IsEventKindVisible (i) ? 1 : 0);
	}
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�c�[��(&T)�x-�w���݂̃O���t�̂ݕ\��(&C)�x
void CEventListFrame::OnUpdateEventListOnlyCurEventKindUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bOnlyCurEventKind);
}


// �w�c�[��(&T)�x-�w�S�ẴO���t��\��(&A)�x
void CEventListFrame::OnEventListShowAllEventKind () {
	int nCount = m_wndEventKindList.GetCount ();
	int nCurSel = m_wndEventKindList.GetCurSel ();
	if (m_bShowAllEventKind) {
		m_bOnlyCurEventKind = FALSE;
		m_bShowAllEventKind = FALSE;
	}
	else {
		m_bOnlyCurEventKind = FALSE;
		m_bShowAllEventKind = TRUE;
	}
	for (int i = 0; i < nCount; i++) {
		m_wndEventKindList.SetCheck (i, IsEventKindVisible (i) ? 1 : 0);
	}
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();

}

// �w�c�[��(&T)�x-�w�S�ẴO���t��\��(&A)�x
void CEventListFrame::OnUpdateEventListShowAllEventKindUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bShowAllEventKind);
}

// �w�����y�[�W�X�V�x
void CEventListFrame::OnEventListAutoPageUpdate () {
	m_bAutoPageUpdate = !m_bAutoPageUpdate;
}

// �w�����y�[�W�X�V�x
void CEventListFrame::OnUpdateEventListAutoPageUpdateUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoPageUpdate);
}

// �w�C�x���g���X�g��CSV���̓e�L�X�g�ŕۑ��x
void CEventListFrame::OnEventListSaveAs () {
	CString strFileName;
	CSekaijuFileDlg theFileDlg (FALSE);

	CString strTitle;
	VERIFY (strTitle.LoadString (AFX_IDS_APP_TITLE));

	// ���ӎ����FMSDN2002/4��OPENFILENAME�\���̂̐���
	// For compatibility reasons, the Places Bar is hidden if Flags is set to 
	// OFN_ENABLEHOOK and lStructSize is OPENFILENAME_SIZE_VERSION_400.
	// CFileDialog�N���X�ł͋����I��OFN_ENABLEHOOK��AfxCommDlgProc�Ƀt�b�N����B

	// 20081028 : m_ofn�̒l�̐ݒ�(76or88)�͎��ɂ��
	//#ifndef OPENFILENAME_SIZE_VERSION_400
	//    theFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME); //=76(Windows95/98/ME style)
	//#else
	//    theFileDlg.m_ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400; //=76(Windows95/98/ME style)
	//#endif
	
	#if (_WIN32_WINNT >= 0x0500)
	    theFileDlg.m_ofn.lStructSize = 88; //=88(With placebar)
		theFileDlg.m_ofn.pvReserved = NULL;
		theFileDlg.m_ofn.dwReserved = 0;
		theFileDlg.m_ofn.FlagsEx = 0;
	#else
	    theFileDlg.m_ofn.lStructSize = 76; //=76(Without placebar if OFN_ENABLEHOOK used)
	#endif

	theFileDlg.m_ofn.nMaxFileTitle = _MAX_PATH;
	
	//theFileDlg.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	CString strFilterCSV[3];
	CString strFilterTXT[3];
	VERIFY (strFilterCSV[0].LoadString (IDS_COMMA_SEPARATED_TEXT_FILES_AD_CSV));
	VERIFY (strFilterCSV[1].LoadString (IDS_AD_CSV));
	VERIFY (strFilterCSV[2].LoadString (IDS_D_CSV));
	VERIFY (strFilterTXT[0].LoadString (IDS_TAB_SEPARATED_TEXT_FILES_AD_TXT));
	VERIFY (strFilterTXT[1].LoadString (IDS_AD_TXT));
	VERIFY (strFilterTXT[2].LoadString (IDS_D_TXT));

	// �g���q�t�B���^�[�ǉ���������
	strFilter += strFilterCSV[0]; // �J���}��؂�e�L�X�g�t�@�C��(*.csv)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterCSV[1]; // *.csv
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterTXT[0]; // �^�u��؂�e�L�X�g�t�@�C��(*.txt)
	strFilter += (TCHAR)_T('\0');
	strFilter += strFilterTXT[1]; // *.txt
	strFilter += (TCHAR)_T('\0');
	strFilter += (TCHAR)_T('\0');
	theFileDlg.m_ofn.nMaxCustFilter = 1024;

	theFileDlg.m_ofn.lpstrFilter = strFilter;
#ifndef _MAC
	theFileDlg.m_ofn.lpstrTitle = strTitle;
#else
	theFileDlg.m_ofn.lpstrPrompt = strTitle;
#endif

	// �f�t�H���g�̊g���q�ݒ�
	theFileDlg.m_strDefExt = strFilterCSV[2];
	theFileDlg.m_ofn.lpstrDefExt = theFileDlg.m_strDefExt;

	// �t�B���^�[�R���{�{�b�N�X�̃f�t�H���g�I��
	if ((strFileName.Right (4)).CompareNoCase (strFilterCSV[2]) == 0) { // .csv
		theFileDlg.m_ofn.nFilterIndex = 1;
	}
	else if ((strFileName.Right (4)).CompareNoCase (strFilterTXT[2]) == 0) { // .txt
		theFileDlg.m_ofn.nFilterIndex = 2;
	}
	
	theFileDlg.m_ofn.lpstrFile = strFileName.GetBuffer(_MAX_PATH);

	// �t�@�C���_�C�A���O.DoModal
	BOOL bResult = theFileDlg.DoModal() == IDOK ? TRUE : FALSE;

	strFileName.ReleaseBuffer();

	// �g���q���t���Ă��Ȃ��ꍇ�́A�I�������t�@�C���^�C�v�̊g���q�������I�ɕt����
	if (theFileDlg.m_ofn.nFilterIndex == 1 &&
		(strFileName.Right (4)).CompareNoCase (strFilterCSV[2]) != 0) { // .csv
		strFileName += strFilterCSV[2];
	}
	else if (theFileDlg.m_ofn.nFilterIndex == 2 &&
		(strFileName.Right (4)).CompareNoCase (strFilterTXT[2]) != 0) { // .txt
		strFileName += strFilterTXT[2];
	}

	// �L�����Z���������ꂽ�ꍇ�͋p��
	if (bResult == FALSE) {
		return;
	}

	// �r���[���Ȃ��ꍇ�͋p��
	if (m_pIndexPropertyView == NULL) {
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_CURRENT_VIEW_IS_NOT_EXIST));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	CEventListIndexPropertyView* pIndexPropertyView = (CEventListIndexPropertyView*)m_pIndexPropertyView;

	// ������o�b�t�@�m��
	CString strTextLine;
	CString strCell;
	CString strSeparator;
	if (theFileDlg.m_ofn.nFilterIndex == 1) {
		strSeparator = _T(", ");
	}
	else {
		strSeparator = _T("\t");
	}

	// �t�@�C���I�[�v��
	CStdioFile theFile;
	BOOL bRet = theFile.Open (strFileName, CFile::modeCreate | CFile::modeWrite);
	if (bRet == FALSE) {
		CString strMsg;
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_S_N_FILE_OPEN_FAILED));
		strMsg.Format (strFormat, strFileName);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// �擪�s�̕����񏑂�����
	long i = 0;
	long j = 0;
	long jMax = 0;
	strTextLine.Format (_T(""));
	strTextLine += strSeparator;
	for (j = 0; j < 8; j++) {
		strCell = GetColumnTitle (j);
		strTextLine += strCell;
		if (j < 7) {
			strTextLine += strSeparator;
		}
		else {
			strTextLine += _T("\n");
		}
	}
	theFile.WriteString (strTextLine);

	// �e�s�̕����񏑂�����
	long lRowCount = GetVisibleEventCount ();
	for (i = 0; i < lRowCount; i++) {
		strTextLine.Format (_T("%d"), i + 1);
		strTextLine += strSeparator;
		MIDIEvent* pMIDIEvent = GetVisibleEvent (i);
		long lKind = MIDIEvent_GetKind (pMIDIEvent);
		if (!MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			jMax = 6;
		}
		else {
			jMax = 8;
		}
		for (j = 0; j < jMax; j++) {
			strCell = pIndexPropertyView->GetCellString (i, j);
			strTextLine += strCell;
			if (j < jMax - 1) {
				strTextLine += strSeparator;
			}
			else {
				strTextLine += _T("\n");
			}
		}
		theFile.WriteString (strTextLine);
	}

	// �t�@�C���N���[�Y
	theFile.Close ();
}	

// �w�C�x���g���X�g��CSV���̓e�L�X�g�ŕۑ��x
void CEventListFrame::OnUpdateEventListSaveAsUI (CCmdUI* pCmdUI) {
}



// �g���b�N�R���{�{�b�N�X���I������I�������
void CEventListFrame::OnTrackComboSelEndOK () {
	// �J�����g�g���b�N���X�V����
	long lCurTrackIndex = m_wndEventTrackCombo.GetCurSel ();
	SetCurTrackIndex (lCurTrackIndex);
	SetTrackVisible (lCurTrackIndex);
	// �J�����g�`�����l�����X�V����
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (pMIDITrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �r���[�̍�蒼���ƍĕ`��
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();

}

// �g���b�N�C���f�b�N�X���X�g�̃`�F�b�N�{�b�N�X���ω������Ƃ�
void CEventListFrame::OnTrackListChkChange () {
	// �g���b�N�̕\���^��\�����`�F�b�N�{�b�N�X�̏�Ԃɍ��킹��
	long lCount = m_wndTrackList.GetCount ();
	long lCurSel = m_wndTrackList.GetCurSel ();
	if (m_bOnlyCurTrack) {
		m_bOnlyCurTrack = FALSE;
	}
	for (long i = 0; i < lCount; i++) {
		m_bTrackVisible[i] = m_wndTrackList.GetCheck (i);
		if (m_bTrackVisible[i] == FALSE) {
			m_bShowAllTrack = FALSE;
		}
	}
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �g���b�N�C���f�b�N�X�̑I�����ڂ��ω������Ƃ�
void CEventListFrame::OnTrackListSelChange () {
	// �J�����g�g���b�N��ύX����
	long lCurTrackIndex = GetCurTrackIndex ();
	SetCurTrackIndex (lCurTrackIndex);
	// �J�����g�`�����l�����X�V����
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (pMIDITrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �r���[�̍�蒼���ƍĕ`��
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �O���t�̎�ރR���{�{�b�N�X���I������I�������
void CEventListFrame::OnEventKindComboSelEndOK () {
	long lCurEventKind = m_wndEventKindCombo.GetCurSel ();
	SetCurEventKind (lCurEventKind);
	SetEventKindVisible (lCurEventKind);
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �O���t�̎�ރ��X�g�̃`�F�b�N�{�b�N�X���ω������Ƃ�
void CEventListFrame::OnEventKindListChkChange () {
	// �O���t�̕\���^��\�����`�F�b�N�{�b�N�X�̏�Ԃɍ��킹��
	long lCount = m_wndEventKindList.GetCount ();
	long lCurSel = m_wndEventKindList.GetCurSel ();
	if (m_bOnlyCurEventKind) {
		m_bOnlyCurEventKind = FALSE;
	}
	//else { // 20110122�폜
	for (long i = 0; i < lCount; i++) {
		m_bEventKindVisible[i] = m_wndEventKindList.GetCheck (i);
		if (m_bEventKindVisible[i] == FALSE) {
			m_bShowAllEventKind = FALSE;
		}
	}
	//} // 20110122�폜
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �O���t�̎�ރ��X�g�̑I�����ڂ��ω������Ƃ�
void CEventListFrame::OnEventKindListSelChange () {
	// �J�����g�O���t��ύX����B
	long lCurEventKind = GetCurEventKind ();
	SetCurEventKind (lCurEventKind);
}


// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��ON�x
void CEventListFrame::OnPopupTrackVisibleOn () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempEvent == NULL) { // 20100429�ǉ�
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	long lCount = m_wndTrackList.GetCount ();
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (0 <= i && i < lCount) {
			m_wndTrackList.SetCheck (i, pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
			m_bTrackVisible[i] = (pMIDITrack == pSekaijuDoc->m_pTempTrack ? TRUE : FALSE);
		}
		i++;
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	// �J�����g�g���b�N�ƃJ�����g�`�����l�����X�V
	long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	if (m_bTrackVisible[lTrackIndex]) {
		SetCurTrackIndex (lTrackIndex);
		pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lOutputChannel && lOutputChannel <= 15) {
				SetCurChannel (lOutputChannel);
			}
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	MakeVisibleEventArray ();
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��ON�x // 20100429�ǉ�
void CEventListFrame::OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��OFF�x
void CEventListFrame::OnPopupTrackVisibleOff () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempEvent == NULL) { // 20100429�ǉ�
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	long lCount = m_wndTrackList.GetCount ();
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (0 <= i && i < lCount) {
			m_wndTrackList.SetCheck (i, pMIDITrack == pSekaijuDoc->m_pTempTrack ? FALSE : TRUE);
			m_bTrackVisible[i] = (pMIDITrack == pSekaijuDoc->m_pTempTrack ? FALSE : TRUE);
		}
		i++;
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	// �J�����g�g���b�N�ƃJ�����g�`�����l�����X�V
	long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	if (m_bTrackVisible[lTrackIndex]) {
		SetCurTrackIndex (lTrackIndex);
		pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lOutputChannel && lOutputChannel <= 15) {
				SetCurChannel (lOutputChannel);
			}
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	MakeVisibleEventArray ();
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��OFF�x // 20100429�ǉ�
void CEventListFrame::OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N�\��ON�x
void CEventListFrame::OnPopupTrackVisibleAll () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	long lCount = m_wndTrackList.GetCount ();
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (0 <= i && i < lCount) {
			m_wndTrackList.SetCheck (i, 1);
			m_bTrackVisible[i] = TRUE;
		}
		i++;
	}
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	// �J�����g�g���b�N�ƃJ�����g�`�����l�����X�V // 20100429�p�~
	//long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	//ASSERT (0 <= lTrackIndex && lTrackIndex < lCount);
	//if (m_bTrackVisible[lTrackIndex]) {
	//	SetCurTrackIndex (lTrackIndex);
	//	pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
	//	if (pMIDITrack) {
	//		long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
	//		if (0 <= lOutputChannel && lOutputChannel <= 15) {
	//			SetCurChannel (lOutputChannel);
	//		}
	//	}
	//}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	MakeVisibleEventArray ();
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N�\��ON�x // 20100429�ǉ�
void CEventListFrame::OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
}


// �w�|�b�v�A�b�v�x-�w���̃C�x���g�̎�ނ̂ݕ\��ON�x
void CEventListFrame::OnPopupEventKindVisibleOn () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempEvent == NULL) { // 20100429�ǉ�
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pMIDIEvent = GetVisibleEvent (((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow);
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDIEvent);
	long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
	long lListIndex = EventKindtoListIndex (lEventKind);
	long lCount = m_wndEventKindList.GetCount ();
	long i = 0;
	for (i = 0; i < lCount; i++) {
		m_wndEventKindList.SetCheck (i, (i == lListIndex ? TRUE : FALSE));
		m_bEventKindVisible[i] = (i == lListIndex ? TRUE : FALSE);
	}
	if (m_bEventKindVisible[lListIndex]) {
		m_wndEventKindCombo.SetCurSel (lListIndex);
		m_wndEventKindList.SetCurSel (lListIndex);
	}
	m_bOnlyCurEventKind = FALSE;
	m_bShowAllEventKind = FALSE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w���̃C�x���g�̎�ނ̂ݕ\��ON�x // 20100429�ǉ�
void CEventListFrame::OnUpdatePopupEventKindVisibleOnUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempEvent == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w���̃C�x���g�̎�ނ̂ݕ\��OFF�x
void CEventListFrame::OnPopupEventKindVisibleOff () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempEvent == NULL) { // 20100429�ǉ�
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pMIDIEvent = GetVisibleEvent (((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow);
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDIEvent);
	long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
	long lListIndex = EventKindtoListIndex (lEventKind);
	long lCount = m_wndEventKindList.GetCount ();
	long i = 0;
	for (i = 0; i < lCount; i++) {
		m_wndEventKindList.SetCheck (i, (i == lListIndex ? FALSE : TRUE));
		m_bEventKindVisible[i] = (i == lListIndex ? FALSE : TRUE);
	}
	if (m_bEventKindVisible[lListIndex]) {
		m_wndEventKindCombo.SetCurSel (lListIndex);
		m_wndEventKindList.SetCurSel (lListIndex);
	}
	m_bOnlyCurEventKind = FALSE;
	m_bShowAllEventKind = FALSE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w���̃C�x���g�̎�ނ̂ݕ\��OFF�x // 20100429�ǉ�
void CEventListFrame::OnUpdatePopupEventKindVisibleOffUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuDoc->m_pTempEvent == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w�S�C�x���g�̎�ޕ\��ON�x
void CEventListFrame::OnPopupEventKindVisibleAll () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lCount = m_wndEventKindList.GetCount ();
	long i = 0;
	for (i = 0; i < lCount; i++) {
		m_wndEventKindList.SetCheck (i, TRUE);
		m_bEventKindVisible[i] = (TRUE);
	}
	// 20090429�p�~
	//MIDIEvent* pMIDIEvent = pSekaijuDoc->m_pTempEvent; // 20090429�C��
	//MIDITrack* pMIDITrack = pSekaijuDoc->m_pTempTrack; // 20090429�C��
	//if (pMIDIEvent != NULL && pMIDITrack != NULL) { // 20090429�������ǉ�
	//	long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
	//	long lListIndex = EventKindtoListIndex (lEventKind);
	//	if (m_bEventKindVisible[lListIndex]) {
	//		m_wndEventKindCombo.SetCurSel (lListIndex);
	//		m_wndEventKindList.SetCurSel (lListIndex);
	//	}
	//}
	m_bOnlyCurEventKind = FALSE;
	m_bShowAllEventKind = FALSE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	MakeVisibleEventArray ();
	long lCurColumn = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurColumn;
	long lCurRow = ((CEventListIndexPropertyView*)m_pIndexPropertyView)->m_lCurRow;
	((CEventListIndexPropertyView*)m_pIndexPropertyView)->AutoScrolltoShowCell (lCurRow, lCurColumn);
	m_pIndexScaleView->Invalidate ();
	m_pIndexPropertyView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w�S�C�x���g�̎�ޕ\��ON�x // 20100429�ǉ�
void CEventListFrame::OnUpdatePopupEventKindVisibleAllUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
}

