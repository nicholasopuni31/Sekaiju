//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃt���[���E�B���h�E�N���X
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
#include "common.h"
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "ColorfulCheckListBox.h"
#include "ColorfulComboBox.h"
#include "MusicalScoreFrame.h"
#include "MusicalScorePrintView.h"
#include "MusicalScoreScaleView.h"
#include "MusicalScoreTimeScaleView.h"
#include "MusicalScoreTrackScaleView.h"
#include "MusicalScoreTrackTimeView.h"
#include "PropertyNoteDlg.h"

// �A���P�[�V�����̊Ď�
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �q�E�B���h�E�̃T�C�Y
#define MUSICALSCOREFRAME_SCALEHEIGHT 32
#define MUSICALSCOREFRAME_SCALEWIDTH 160
#define MUSICALSCOREFRAME_TIMEWIDTH 600
#define MUSICALSCOREFRAME_TRACKHEIGHT 256


#define MUSICALSCOREFRAME_VELHEIGHT 128
#define MUSICALSCOREFRAME_SCROLLBARHEIGHT 16
#define MUSICALSCOREFRAME_SCROLLBARWIDTH 16
#define MUSICALSCOREFRAME_BORDERWIDTH 2
#define MUSICALSCOREFRAME_BORDERHEIGHT 2
#define MUSICALSCOREFRAME_SPLITTERWIDTH 4
#define MUSICALSCOREFRAME_SPLITTERHEIGHT 4
#define MUSICALSCOREFRAME_TRACKLISTWIDTH 120

// �q�E�B���h�EID���`
#define MUSICALSCOREFRAME_DUMMYVIEW      (AFX_IDW_PANE_FIRST + 0)
#define MUSICALSCOREFRAME_PRINTVIEW      (AFX_IDW_PANE_FIRST + 1)
#define MUSICALSCOREFRAME_SCALEVIEW      (AFX_IDW_PANE_FIRST + 32)
#define MUSICALSCOREFRAME_TIMESCALEVIEW  (AFX_IDW_PANE_FIRST + 33)
#define MUSICALSCOREFRAME_TRACKSCALEVIEW (AFX_IDW_PANE_FIRST + 34)
#define MUSICALSCOREFRAME_TRACKTIMEVIEW  (AFX_IDW_PANE_FIRST + 35)
#define MUSICALSCOREFRAME_VELSCALEVIEW   (AFX_IDW_PANE_FIRST + 36)
#define MUSICALSCOREFRAME_VELTIMEVIEW    (AFX_IDW_PANE_FIRST + 37)
#define MUSICALSCOREFRAME_TIMESCROLL     (AFX_IDW_PANE_FIRST + 48)
#define MUSICALSCOREFRAME_TRACKSCROLL    (AFX_IDW_PANE_FIRST + 49)
#define MUSICALSCOREFRAME_VELSCROLL      (AFX_IDW_PANE_FIRST + 50)
#define MUSICALSCOREFRAME_SIZEBOX        (AFX_IDW_PANE_FIRST + 51)
#define MUSICALSCOREFRAME_TIMEZOOMDOWN   (AFX_IDW_PANE_FIRST + 52)
#define MUSICALSCOREFRAME_TIMEZOOMUP     (AFX_IDW_PANE_FIRST + 53)
#define MUSICALSCOREFRAME_TRACKZOOMDOWN  (AFX_IDW_PANE_FIRST + 54)
#define MUSICALSCOREFRAME_TRACKZOOMUP    (AFX_IDW_PANE_FIRST + 55)
#define MUSICALSCOREFRAME_VELZOOMDOWN    (AFX_IDW_PANE_FIRST + 56)
#define MUSICALSCOREFRAME_VELZOOMUP      (AFX_IDW_PANE_FIRST + 57)
#define MUSICALSCOREFRAME_TRACKLIST      (AFX_IDW_PANE_FIRST + 58)
#define MUSICALSCOREFRAME_GRAPHKINDLIST  (AFX_IDW_PANE_FIRST + 59)

//
// �s�A�m���[���t���[���̃N���C�A���g�̈�ɔz�u���ꂽ�I�u�W�F�N�g
//	�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@   �@�@ m_lScr�@��    m_lTrack
//                  ����m_lScale������     m_lTime  �@������ollBar������  ListWidth   ����
//                      Width                Width     �@�@ Width   ��
//        ��
//        ��        ����������������������������������������������������������������������
// m_lToolBar1Height��CToolBar                                                          ��
//	    �@��        ��m_wndToolBar1                                                     ��
//        ��        ����������������������������������������������������������������������
//        ��        ��CView*      ��CView*  �@�@                ������CChcekListBox ������
//  m_lScaleHeight  ��m_pScaleView��m_pTimeScaleView      �@�@�@������m_wndTrackList������
//        ��        �����������������������������������������������@��(With Scroll  ���@��
//        ��        ��CView*�@    ��CView*    �@�@          �@  ������ Bar)         ������
//        ��        ��m_pKeyScale ��m_pTrackTimeView     �@�@     ���@��              ���@��
//                  ��View        ��  �@�@�@�@�@�@�@�@�@�@�@�@�@������              ���@��
//   m_lKeyHeight   ���@�@�@      ��   CScrollBar m_wndTrackScroll������              ���@��
//                  ���@�@�@      ���@�@�@�@�@�@�@�@�@�@�@�@�@  ������              ������
//                  ���@�@�@      ���@  CButton m_wndTrackZoomDown���|��              ������
//                  ���@�@�@      ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�������@�@�@�@�@�@�@������
//        ��        ���@�@�@      ���@ �@ CButton m_wndTrackZoomUp���{���@�@�@�@�@�@�@���@��
//        ��        ����������������������������������������������������������������������
//m_lScrollBarHeight�������@�@�@���@�@�@�@�@�@�@�@�@�������|���{�������������@�@�������@���@�@
//        ��        ����������������������������������������������������������������������
//        ��                    CScrollBar          CButton   CButton
//                              m_wndTimeScroll     m_wndTime m_wndTime
//                                                  ZoomDown  ZoomDown
//
// (��)�\�\�\�F�P���ȋ��E��(0px)�B
// (��)�������F�������̓I�ȋ��E���BBORDERWIDTH(2px)����BORDERHEIGHT(2px)�Ŏ���������
// (��)�������F�X�v���b�^�[���E���B(��)*2+SPRITTERWIDTH(4px)����SPRITTERHEIGHT(4px)�Ŏ��������́B
//

// �֐��}�N��
#define MUSICALSCOREFRAME_RANGE(A,B,C) ((A)>(B)?(A):((B)>(C)?(C):(B)))

// ���b�Z�[�W�}�b�v
IMPLEMENT_DYNCREATE (CMusicalScoreFrame, CChildFrame)

BEGIN_MESSAGE_MAP (CMusicalScoreFrame, CChildFrame)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_SIZE ()
	ON_WM_TIMER ()
	ON_WM_ERASEBKGND ()
	ON_WM_MDIACTIVATE ()
	ON_WM_PAINT ()	
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_HSCROLL ()	
	ON_WM_VSCROLL ()
	ON_BN_CLICKED (MUSICALSCOREFRAME_TIMEZOOMDOWN, OnTimeZoomDown)
	ON_BN_CLICKED (MUSICALSCOREFRAME_TIMEZOOMUP, OnTimeZoomUp)
	ON_BN_CLICKED (MUSICALSCOREFRAME_TRACKZOOMDOWN, OnTrackZoomDown)
	ON_BN_CLICKED (MUSICALSCOREFRAME_TRACKZOOMUP, OnTrackZoomUp)
//	ON_WM_CHILDACTIVATE ()	
	ON_COMMAND (ID_MUSICALSCORE_PEN, OnMusicalScorePen)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_PEN, OnUpdateMusicalScorePenUI)
	ON_COMMAND (ID_MUSICALSCORE_ERASER, OnMusicalScoreEraser)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_ERASER, OnUpdateMusicalScoreEraserUI)
	ON_COMMAND (ID_MUSICALSCORE_SELECT, OnMusicalScoreSelect)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SELECT, OnUpdateMusicalScoreSelectUI)
	ON_COMMAND (ID_MUSICALSCORE_SPEAKER, OnMusicalScoreSpeaker)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SPEAKER, OnUpdateMusicalScoreSpeakerUI)
	ON_COMMAND (ID_MUSICALSCORE_WHOLENOTE, OnMusicalScoreWholeNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_WHOLENOTE, OnUpdateMusicalScoreWholeNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_HALFNOTE, OnMusicalScoreHalfNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_HALFNOTE, OnUpdateMusicalScoreHalfNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_QUARTERNOTE, OnMusicalScoreQuarterNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_QUARTERNOTE, OnUpdateMusicalScoreQuarterNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_QUAVERNOTE, OnMusicalScoreQuaverNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_QUAVERNOTE, OnUpdateMusicalScoreQuaverNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_SEMIQUAVERNOTE, OnMusicalScoreSemiQuaverNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SEMIQUAVERNOTE, OnUpdateMusicalScoreSemiQuaverNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_DEMISEMIQUAVERNOTE, OnMusicalScoreDemiSemiQuaverNote)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_DEMISEMIQUAVERNOTE, OnUpdateMusicalScoreDemiSemiQuaverNoteUI)
	ON_COMMAND (ID_MUSICALSCORE_DOTTED, OnMusicalScoreDotted)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_DOTTED, OnUpdateMusicalScoreDottedUI)
	ON_COMMAND (ID_MUSICALSCORE_TRIPLET, OnMusicalScoreTriplet)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_TRIPLET, OnUpdateMusicalScoreTripletUI)
	ON_COMMAND (ID_MUSICALSCORE_ONLYCURTRACK, OnMusicalScoreOnlyCurTrack)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_ONLYCURTRACK, OnUpdateMusicalScoreOnlyCurTrackUI)
	ON_COMMAND (ID_MUSICALSCORE_SHOWALLTRACK, OnMusicalScoreShowAllTrack)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_SHOWALLTRACK, OnUpdateMusicalScoreShowAllTrackUI)
	ON_COMMAND (ID_MUSICALSCORE_AUTOPAGEUPDATE, OnMusicalScoreAutoPageUpdate)
	ON_UPDATE_COMMAND_UI (ID_MUSICALSCORE_AUTOPAGEUPDATE, OnUpdateMusicalScoreAutoPageUpdateUI)

	ON_COMMAND (ID_POPUP_TRACKVISIBLEON, OnPopupTrackVisibleOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEON, OnUpdatePopupTrackVisibleOnUI)
	ON_COMMAND (ID_POPUP_TRACKVISIBLEOFF, OnPopupTrackVisibleOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEOFF, OnUpdatePopupTrackVisibleOffUI)
	ON_COMMAND (ID_POPUP_TRACKVISIBLEALL, OnPopupTrackVisibleAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKVISIBLEALL, OnUpdatePopupTrackVisibleAllUI)
	ON_COMMAND (ID_POPUP_EVENTPROPERTY, OnPopupEventProperty)
	ON_UPDATE_COMMAND_UI (ID_POPUP_EVENTPROPERTY, OnUpdatePopupEventPropertyUI)

	ON_CBN_SELENDOK (IDC_SNAPCOMBO, OnSnapComboSelEndOK)	
	ON_CBN_SELENDOK (IDC_TRACKCOMBO, OnTrackComboSelEndOK)	
	ON_CLBN_CHKCHANGE (MUSICALSCOREFRAME_TRACKLIST, OnTrackListChkChange)
	ON_LBN_SELCHANGE (MUSICALSCOREFRAME_TRACKLIST, OnTrackListSelChange)
	ON_CBN_SELENDOK (IDC_RESOLUTIONCOMBO, OnResolutionComboSelEndOK)	


END_MESSAGE_MAP ()

//-----------------------------------------------------------------------------
// �\�z�Ɣj��
//-----------------------------------------------------------------------------

// �R���X�g���N�^
CMusicalScoreFrame::CMusicalScoreFrame () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_lKeyHeight = MUSICALSCOREFRAME_TRACKHEIGHT;
	m_lScaleHeight = MUSICALSCOREFRAME_SCALEHEIGHT;
	m_lScaleWidth = MUSICALSCOREFRAME_SCALEWIDTH;
	m_lTimeWidth = MUSICALSCOREFRAME_TIMEWIDTH;
	//m_lSplitterHeight = MUSICALSCOREFRAME_SPLITTERHEIGHT;
	//m_lSplitterWidth = MUSICALSCOREFRAME_SPLITTERWIDTH;
	m_lHScrollBarHeight = ::GetSystemMetrics (SM_CYHSCROLL);
	m_lVScrollBarWidth = ::GetSystemMetrics (SM_CXVSCROLL);
	m_lTrackListWidth = MUSICALSCOREFRAME_TRACKLISTWIDTH;
	m_lTrackZoom = pSekaijuApp->m_theMusicalScoreOption.m_lDefTrackZoom;
	m_lTimeZoom = pSekaijuApp->m_theMusicalScoreOption.m_lDefTimeZoom;
	m_lTimeScrollPos = 0;
	CString strFontName;
	VERIFY (strFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strFontName);
	CString strTimeMeasureFontName;
	VERIFY (strTimeMeasureFontName.LoadString (IDS_TIMEMEASUREFONTNAME));
	m_theTimeMeasureFont.CreateFont (16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strTimeMeasureFontName);
	m_lCurTool = ID_MUSICALSCORE_PEN;
	m_bAutoPageUpdate = FALSE;
	m_bOnlyCurTrack = FALSE;
	m_bShowAllTrack = FALSE;
	for (long i = 0; i < MAXMIDITRACKNUM; i++) {
		m_bTrackVisible[i] = TRUE;
	}
}

// �f�X�g���N�^
CMusicalScoreFrame::~CMusicalScoreFrame () {
	m_theFont.DeleteObject ();
	m_theTimeMeasureFont.DeleteObject ();
	// �g���b�N���z��폜
	VERIFY (DeleteTrackInfoArray ());
	// ���ߏ��z��폜
	VERIFY (DeleteMeasureInfoArray ());

}

// �h�L�������g�̎擾
CSekaijuDoc* CMusicalScoreFrame::GetDocument () {
	ASSERT (m_pDummyView);
	return (CSekaijuDoc*)m_pDummyView->GetDocument ();
}

//-----------------------------------------------------------------------------
// �I�y���[�V����
//-----------------------------------------------------------------------------

// ���ԕ����̃Y�[���{���擾
long CMusicalScoreFrame::GetTimeZoom () {
	return m_lTimeZoom;
}

// �g���b�N�����̃Y�[���{���擾
long CMusicalScoreFrame::GetTrackZoom () {
	return m_lTrackZoom;
}

// y���W���g���b�N�C���f�b�N�X�ɕϊ�
long CMusicalScoreFrame::YtoTrackIndex (long y) {
	long lNumTrackInfo = GetTrackInfoCount ();
	long i;
	for (i = 0; i < lNumTrackInfo; i++) {
		MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (i);
		if (pTrackInfo->m_lTop * m_lTrackZoom <= y && y < (pTrackInfo->m_lTop + pTrackInfo->m_lHeight) * m_lTrackZoom) {
			return i;
		}
	}
	return MAXMIDITRACKNUM;
}

// y���W���m�[�g�L�[�ɕϊ�
long CMusicalScoreFrame::TrackIndexYtoKey (long lTrackIndex, long y, long lKeySignature) {
	long lNumTrackInfo = GetTrackInfoCount ();
	if (lTrackIndex < 0 || lTrackIndex >= lNumTrackInfo) {
		return -1;
	}
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	long lKey;
	for (lKey = 0; lKey < 128; lKey++) {
		long lTempY = TrackIndexKeytoY (lTrackIndex, lKey, lKeySignature);
		if (lTempY < y) {
			if (KeytoSF (lKey, lKeySignature) == 0) {
				return lKey;
			}
		}
	}
	return 128;
}

// �w��̒����L���ɂ����āA�w��L�[�͕��ʏ�̉��Ԗڂ̐��ɏ�邩��Ԃ��B
// �����L�[�ł������L���ɂ��36�ɂȂ�����37�ɂȂ����肷��B
long CMusicalScoreFrame::KeytoLineNo (long lKey, long lKeySignature) {
	long lsf = lKeySignature & 0x000000FF;
	long lmi = (lKeySignature >> 8) & 0x000000FF;
	lsf = CLIP (-7, (char)lsf, 7);
	long lDeltaTable[15][12] = {
	//   C, -, D, -, E, F, -, G, -, A, -, B
		{0, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7}, // 7b (Cb-major / Ab-minor)
		{0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7}, // 6b (Gb-major / Eb-minor)
		{0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 7}, // 5b (Db-major / Bb-minor)
		{0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 7}, // 4b (Ab-major / F-minor)
		{0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6}, // 3b (Eb-major / C-minor)
		{0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6}, // 2b (Bb-major / G-minor)
		{0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6}, // 1b (F-major / D-minor)
		{0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6}, // 0 (C-major / A-minor)
		{0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6}, // 1# (G-major / E-minor)
		{0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6}, // 2# (D-major / B-minor)
		{0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6}, // 3# (A-major / F#-minor)
		{-1, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6}, // 4# (E-major / C#-minor)
		{-1, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6}, // 5# (B-major / G#-minor)
		{-1, 0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6}, // 6# (F#-major / D#-minor)
		{-1, 0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6}  // 7# (C#-major / A#-minor)
	};
	return ((lKey / 12) * 7 + lDeltaTable[lsf + 7][lKey % 12]);
}

// �w��̒����L���ɂ����āA�w��L�[��#b�̗Վ��L�����t�����ǂ�����Ԃ��B
// �߂�l�F0�Ȃ�,2:bb,3:b,4:�i�`������,5:#,6:##������ׂ��ł���B
long CMusicalScoreFrame::KeytoSF (long lKey, long lKeySignature) {
	long lsf = lKeySignature & 0x000000FF;
	long lmi = (lKeySignature >> 8) & 0x000000FF;
	lsf = CLIP (-7, (char)lsf, 7);
	long lSFTable[15][12] = {
	//   C, -, D, -, E, F, -, G, -, A, -, B
		{4, 0, 2, 0, 0, 4, 0, 4, 0, 2, 0, 0}, // 7b (Cb-major / Ab-minor)
		{4, 0, 4, 0, 3, 0, 0, 4, 0, 2, 0, 0}, // 6b (Gb-major / Eb-minor)
		{0, 0, 4, 0, 3, 0, 0, 4, 0, 4, 0, 3}, // 5b (Db-major / Bb-minor)
		{0, 0, 4, 0, 4, 0, 3, 0, 0, 4, 0, 3}, // 4b (Ab-major / F-minor)
		{0, 3, 0, 0, 4, 0, 3, 0, 0, 4, 0, 4}, // 3b (Eb-major / C-minor)
		{0, 3, 0, 0, 4, 0, 5, 0, 3, 0, 0, 4}, // 2b (Bb-major / G-minor)
		{0, 5, 0, 3, 0, 0, 5, 0, 3, 0, 0, 4}, // 1b (F-major / D-minor)
		{0, 5, 0, 3, 0, 0, 5, 0, 5, 0, 3, 0}, // 0 (C-major / A-minor)
		{0, 5, 0, 5, 0, 4, 0, 0, 5, 0, 3, 0}, // 1# (G-major / E-minor)
		{4, 0, 0, 5, 0, 4, 0, 0, 5, 0, 5, 0}, // 2# (D-major / B-minor) // 20110111 G,G#�C��
		{4, 0, 0, 5, 0, 5, 0, 4, 0, 0, 5, 0}, // 3# (A-major / F#-minor)
		{5, 0, 4, 0, 0, 5, 0, 4, 0, 0, 5, 0}, // 4# (E-major / C#-minor)
		{5, 0, 4, 0, 0, 5, 0, 6, 0, 4, 0, 0}, // 5# (B-major / G#-minor)
		{5, 0, 6, 0, 4, 0, 0, 6, 0, 4, 0, 0}, // 6# (F#-major / D#-minor)
		{0, 0, 6, 0, 4, 0, 0, 6, 0, 6, 0, 4}  // 7# (C#-major / A#-minor)
	};
	return (lSFTable[lsf + 7][lKey % 12]);
}

// �g���b�N�C���f�b�N�X(0�`65535)�E�ܐ��ԍ���Y���W[pixel]�ɕϊ�
long CMusicalScoreFrame::TrackIndexLineNotoY (long lTrackIndex, long lLineNo) {
	ASSERT (0<= lTrackIndex &&lTrackIndex < GetTrackInfoCount ());
	// �f�t�H���g��m_lTrackZoom=4�B
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	VERIFY (pTrackInfo = GetTrackInfo (lTrackIndex));
	long lTrackTop = pTrackInfo->m_lTop;
	long lTrackHeight = pTrackInfo->m_lHeight;
	long lTrackFlags = pTrackInfo->m_lFlags;
	long lKey000Y = 0;
	long ry = 4;
	switch (lTrackFlags & 0x0000000F) {
	case 1: // �g���L��
		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 41 * ry;
		break;
	case 2: // �։��L��
		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 29 * ry;
		break;
	case 3: // �啈�\

		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 35 * ry;
		break;
	default:
		lKey000Y = (lTrackTop + lTrackHeight / 2) * m_lTrackZoom + 35 * ry;
		break;
	}
	return lKey000Y - lLineNo * ry;
}

// �g���b�N�C���f�b�N�X(0�`65535)�E�m�[�g�L�[(0�`127)��y���W[pixel]�ɕϊ�
long CMusicalScoreFrame::TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature) {
	long lLineNo = KeytoLineNo (lKey, lKeySignature);
	return TrackIndexLineNotoY (lTrackIndex, lLineNo);
}



// x���W[pixel]����^�C��[tick]���擾
long CMusicalScoreFrame::XtoTime (long x) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lMeasureCount = m_theMeasureInfoArray.GetSize ();
	long j;
	for (j = 0; j < lMeasureCount - 1; j++) {
		if (GetMeasureLeft (j + 1) * m_lTimeZoom > x) {
			break;
		}
	}
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (j));
	long lDeltaX_TimeZoom = x - 
		(pMeasureInfo->m_lLeft + pMeasureInfo->m_lSignatureWidth + 
		pMeasureInfo->m_lPreWidth) * m_lTimeZoom;
	lDeltaX_TimeZoom = MAX (0, lDeltaX_TimeZoom);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	return pMeasureInfo->m_lTime + lDeltaX_TimeZoom * lTimeResolution / 8 / m_lTimeZoom;
	// ���F�Y�[���{��1�{�̂Ƃ�4�������̒�����8�s�N�Z���ƒ�`���Ă���B

}

// �^�C��[tick]��x���W[pixel]�ɕϊ�
long CMusicalScoreFrame::TimetoX (long lTime) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lMeasureCount = m_theMeasureInfoArray.GetSize ();
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	long j;
	for (j = 0; j < lMeasureCount - 1; j++) {
		if (GetMeasureTime (j + 1) > lTime) {
			break;
		}
	}
	long lMeasureTime = GetMeasureTime (j);
	long lDeltaTime = lTime - lMeasureTime;
	ASSERT (lDeltaTime >= 0);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (j));

	return 
		pMeasureInfo->m_lLeft * m_lTimeZoom + 
		pMeasureInfo->m_lSignatureWidth * m_lTimeZoom +
		pMeasureInfo->m_lPreWidth * m_lTimeZoom +
 		(lDeltaTime * 8 * m_lTimeZoom / lTimeResolution);
	// ���F�Y�[���{��1�{�̂Ƃ�4�������̒�����8�s�N�Z���ƒ�`���Ă���B
}

// ���߂�x���W[pixel]�ɕϊ�(���ߋ��E���ʒu)
long CMusicalScoreFrame::MeasuretoX (long lMeasure) {
	long lMeasureCount = GetMeasureInfoCount ();
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (lMeasure));
	return pMeasureInfo->m_lLeft * m_lTimeZoom;
}

// ���߂�x���W[pixel]�ɕϊ�(���ߋ��E��+���q�����L���ʒu)
long CMusicalScoreFrame::MeasuretoX2 (long lMeasure) {
	long lMeasureCount = GetMeasureInfoCount ();
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = GetMeasureInfo (lMeasure));
	return (pMeasureInfo->m_lLeft + pMeasureInfo->m_lSignatureWidth) * m_lTimeZoom;
}

// ���ԕ����̃X�N���[���|�W�V�����擾
long CMusicalScoreFrame::GetTimeScrollPos () {
	return m_lTimeScrollPos;
}

// �g���b�N�����̃X�N���[���|�W�V�����擾
long CMusicalScoreFrame::GetTrackScrollPos () {
	return m_lTrackScrollPos;
}

// ���ԕ����̃X�N���[���|�W�V�����ݒ�
long CMusicalScoreFrame::SetTimeScrollPos (long lTimeScrollPos) {
	long lOldTimeScrollPos = m_lTimeScrollPos;
	m_wndTimeScroll.SetScrollPos (lTimeScrollPos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	long lDeltaTimeScrollPos = m_lTimeScrollPos - lOldTimeScrollPos;
	m_pTimeScaleView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTrackTimeView->ScrollWindow (-lDeltaTimeScrollPos, 0);
	m_pTimeScaleView->UpdateWindow ();
	m_pTrackTimeView->UpdateWindow ();
	return m_lTimeScrollPos;
}

// �g���b�N���̃X�N���[���|�W�V�����ݒ�
long CMusicalScoreFrame::SetTrackScrollPos (long lTrackScrollPos) {
	long lOldTrackScrollPos = m_lTrackScrollPos;
	m_wndTrackScroll.SetScrollPos (lTrackScrollPos);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
	long lDeltaTrackScrollPos = m_lTrackScrollPos - lOldTrackScrollPos;
	m_pTrackScaleView->ScrollWindow (0, -lDeltaTrackScrollPos);
	m_pTrackTimeView->ScrollWindow (0, -lDeltaTrackScrollPos);
	m_pTrackScaleView->UpdateWindow ();
	m_pTrackTimeView->UpdateWindow ();
	return m_lTrackScrollPos;
}


// �\������Ă���^�C���̍��[�����߂�
long CMusicalScoreFrame::GetVisibleLeftTime () {
	return XtoTime (m_lTimeScrollPos);
}

// �\������Ă���^�C���̉E�[�����߂�
long CMusicalScoreFrame::GetVisibleRightTime () {
	CRect rcClient;
	m_pTimeScaleView->GetClientRect (&rcClient);
	return XtoTime (m_lTimeScrollPos + rcClient.Width ());
}

// �\������Ă���g���b�N�̏�����v�Z
long CMusicalScoreFrame::GetVisibleTopTrack () {
	long lTrackCount = GetTrackInfoCount ();
	long lTrackIndex;
	for (lTrackIndex = 0; lTrackIndex < lTrackCount - 1; lTrackIndex++) {
		if (GetTrackInfo (lTrackIndex + 1)->m_lTop * m_lTrackZoom >= m_lTrackScrollPos) {
			break;
		}
	}
	return lTrackIndex;
}

// �\������Ă���g���b�N�̉������v�Z
long CMusicalScoreFrame::GetVisibleBottomTrack () {
	CRect rcClient;
	m_pTrackScaleView->GetClientRect (&rcClient);
	long lTrackCount = m_theTrackInfoArray.GetSize ();
	long lTrackIndex;
	for (lTrackIndex = 0; lTrackIndex < lTrackCount; lTrackIndex++) {
		if (GetTrackInfo (lTrackIndex)->m_lTop * m_lTrackZoom >= m_lTrackScrollPos + rcClient.Height ()) {
			break;
		}
	}
	return lTrackIndex;
}

// �X�v���b�^�[�L���v�^�[�̕`��
void CMusicalScoreFrame::DrawSplitterCaptor (CDC* pDC, CPoint pt) {
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




// ���݂̃g���b�N�̃C���f�b�N�X���擾
long CMusicalScoreFrame::GetCurTrackIndex () {
	return m_wndTrackList.GetCurSel ();
}

// ���݂̃`�����l�����擾
long CMusicalScoreFrame::GetCurChannel () {
	return m_wndChannelCombo.GetCurSel ();
}

// ���݂̃X�i�b�v[�e�B�b�N]���擾
long CMusicalScoreFrame::GetCurSnap () {
	CString strText;
	m_wndSnapCombo.GetWindowText (strText);
	return _ttol (strText);
}

// ���݂̃x���V�e�B���擾
long CMusicalScoreFrame::GetCurVelocity () {
	CString strText;
	m_wndVelocityCombo.GetWindowText (strText);
	return _ttol (strText);
}

// ���݂�(������)����[�e�B�b�N]���擾
long CMusicalScoreFrame::GetCurDuration () {
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	return _ttol (strText);
}

// ���݂̕\�����x[�e�B�b�N]���擾
long CMusicalScoreFrame::GetCurResolution () {
	CString strText;
	m_wndResolutionCombo.GetWindowText (strText);
	return _ttol (strText);
}

// ���݂̃g���b�N�̃C���f�b�N�X��ݒ�
BOOL CMusicalScoreFrame::SetCurTrackIndex (long lCurTrackIndex) {
	ASSERT (0 <= lCurTrackIndex && lCurTrackIndex < MAXMIDITRACKNUM);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	m_wndTrackCombo.SetCurSel (lCurTrackIndex);
	m_wndTrackList.SetCurSel (lCurTrackIndex);
	//m_pCurTrack = pSekaijuDoc->GetTrack (lCurTrackIndex);
	if (m_bOnlyCurTrack) {
		long lCount = m_wndTrackList.GetCount ();
		for (long i = 0; i < lCount; i++) {
			m_wndTrackList.SetCheck (i, (i == lCurTrackIndex ? 1 : 0));
		}
		m_pTrackTimeView->Invalidate ();
	}
	return TRUE;
}

// ���݂̃`�����l����ݒ�
BOOL CMusicalScoreFrame::SetCurChannel (long lCurChannel) {
	ASSERT (0 <= lCurChannel && lCurChannel < 16);
	m_wndChannelCombo.SetCurSel (lCurChannel);
	return TRUE;
}

// ���݂̃X�i�b�v[�e�B�b�N]��ݒ�
BOOL CMusicalScoreFrame::SetCurSnap (long lCurSnap) {
	ASSERT (1 <= lCurSnap);
	CString strText;
	strText.Format (_T("%d"), lCurSnap);
	m_wndSnapCombo.SetWindowText (strText);
	return TRUE;
}

// ���݂̃x���V�e�B��ݒ�
BOOL CMusicalScoreFrame::SetCurVelocity (long lCurVelocity) {
	ASSERT (0 <= lCurVelocity && lCurVelocity <= 127);
	CString strText;
	strText.Format (_T("%d"), lCurVelocity);
	m_wndVelocityCombo.SetWindowText (strText);
	return TRUE;
}

// ���݂�(������)����[�e�B�b�N]��ݒ�
BOOL CMusicalScoreFrame::SetCurDuration (long lCurDuration) {
	ASSERT (0 <= lCurDuration);
	CString strText;
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
	return TRUE;
}

// ���݂̕\�����x[�e�B�b�N]��ݒ�
BOOL CMusicalScoreFrame::SetCurResolution (long lCurResolution) {
	ASSERT (0 <= lCurResolution);
	CString strText;
	strText.Format (_T("%d"), lCurResolution);
	m_wndResolutionCombo.SetWindowText (strText);
	return TRUE;
}

// �w��C���f�b�N�X�̃g���b�N���\����Ԃ����ׂ�
BOOL CMusicalScoreFrame::IsTrackVisible (long lTrackIndex) {
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
BOOL CMusicalScoreFrame::SetTrackVisible (long lTrackIndex) {
	// (1)���݂̃g���b�N�̂ݕ\����ON�̂Ƃ��́A���݂̃g���b�N���w��g���b�N�ɕύX����
	// (2)���ׂẴg���b�N��\����ON�̂Ƃ��́A�������Ȃ��B
	// (3)���̑��̏ꍇ(�ʏ펞)�́Am_bTrackVisible[lTrackIndex]���`�F�b�N�E��������B
	ASSERT (0 <= lTrackIndex && lTrackIndex < MAXMIDITRACKNUM);
	if (m_bOnlyCurTrack == TRUE) {
		m_wndTrackCombo.SetCurSel (lTrackIndex);
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



// �g���b�N�R���{�̍X�V
BOOL CMusicalScoreFrame::UpdateTrackCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	//pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	// ����Ԃ̕ێ�
	static MIDITrack* pOldMIDITrack[MAXMIDITRACKNUM];
	BOOL bOldTrackVisible[MAXMIDITRACKNUM];
	memcpy (bOldTrackVisible, m_bTrackVisible, sizeof (BOOL) * MAXMIDITRACKNUM);
	long lOldCurSel = m_wndTrackCombo.GetCurSel ();
	long lOldCount = m_wndTrackCombo.GetCount ();
	MIDITrack* pOldCurTrack = NULL;
	if (0 <= lOldCurSel && lOldCurSel < __min (lOldCount, MAXMIDITRACKNUM)) {
		pOldCurTrack = pOldMIDITrack[lOldCurSel];
	}

	// �R���{�̏�����
	m_wndTrackCombo.RemoveAllItem ();

	// �R���{�ɍ��ڂ�ǉ�
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
		m_wndTrackCombo.AddItem (strText, lForeColor, lBackColor);
		
		// ���݂̃g���b�N�ł���ꍇ�I��
		if (pMIDITrack == pOldCurTrack) {
			m_wndTrackCombo.SetCurSel (i);
		}
		i++;
	}

	// ���ݑI�����Ă�����̂��Ȃ��ꍇ�͋����I��
	long lNewCount = m_wndTrackCombo.GetCount ();
	long lNewCurSel = m_wndTrackCombo.GetCurSel ();
	if (m_wndTrackCombo.GetCurSel () == CB_ERR) {
		if (0 <= lOldCurSel && lOldCurSel < lNewCount) {
			m_wndTrackCombo.SetCurSel (lOldCurSel);
		}
		else if (lNewCount >= 2) {
			m_wndTrackCombo.SetCurSel (1);
		}
		else {
			m_wndTrackCombo.SetCurSel (0);
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


// �g���b�N���X�g�{�b�N�X�̍X�V
BOOL CMusicalScoreFrame::UpdateTrackList () {
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
	//m_wndTrackList.ResetContent ();
	m_wndTrackList.RemoveAllItem ();

	// ���X�g�̍X�V
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


// �X�i�b�v�R���{�{�b�N�X�̍X�V
BOOL CMusicalScoreFrame::UpdateSnapCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	long lCurSel = m_wndSnapCombo.GetCurSel ();
	m_wndSnapCombo.ResetContent ();
	// �X�i�b�v�R���{�{�b�N�X�̏[��
	CString strFormat;
	VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE)); // %d-4������
	strText.Format (strFormat, lTimeResolution * 1);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE)); // %d-8������
	strText.Format (strFormat, lTimeResolution / 2);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE)); // %d-3�A8������
	strText.Format (strFormat, lTimeResolution / 3);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE)); // %d-16������
	strText.Format (strFormat, lTimeResolution / 4);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE)); // %d-3�A16������
	strText.Format (strFormat, lTimeResolution / 6);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE)); // %d-32������
	strText.Format (strFormat, lTimeResolution / 8);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE)); // %d-3�A32������
	strText.Format (strFormat, lTimeResolution / 12);
	m_wndSnapCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_FREE)); // %d-���R
	strText.Format (strFormat, 1);
	m_wndSnapCombo.AddString (strText);
	// �J�����g�Z���ݒ�
	if (lCurSel >= 0) {
		m_wndSnapCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}

// �x���V�e�B�R���{�{�b�N�X�̍X�V
BOOL CMusicalScoreFrame::UpdateVelocityCombo () {
	long i;
	CString strText;
	long lCurSel = m_wndVelocityCombo.GetCurSel ();
	m_wndVelocityCombo.ResetContent ();
	// �x���V�e�B�R���{�{�b�N�X�̏[��
	for (i = 127; i >= 1; i--) {
		if (i == 127 || (i % 5) == 0) {
			strText.Format (_T("%d"), i);
			m_wndVelocityCombo.AddString (strText);
		}
	}
	// �J�����g�Z���ݒ�
	if (lCurSel >= 0) {
		m_wndVelocityCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}

// �����R���{�{�b�N�X�̍X�V
BOOL CMusicalScoreFrame::UpdateDurationCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	//long lCurSel = m_wndDurationCombo.GetCurSel (); // 20100328�p�~
	CString strCurText;
	m_wndDurationCombo.GetWindowText (strCurText); // 20100328�ǉ�
	long i = 0;
	long lCurSel = -1;
	for (i = 0; i < m_wndDurationCombo.GetCount (); i++) {
		CString strLBText;
		m_wndDurationCombo.GetLBText (i, strLBText);
		if (strLBText == strCurText) {
			lCurSel = i;
			break;
		}
	} // 20100328�ǉ�
	m_wndDurationCombo.ResetContent ();
	// �����R���{�{�b�N�X�̏[��
	strText.Format (_T("%d"), lTimeResolution * 4);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution * 3);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution * 2);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution * 1);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 2);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 3);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 4);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 6);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 8);
	m_wndDurationCombo.AddString (strText);
	strText.Format (_T("%d"), lTimeResolution / 12);
	m_wndDurationCombo.AddString (strText);
	// �J�����g�Z���̐ݒ�
	if (lCurSel >= 0) {
		m_wndDurationCombo.SetCurSel (lCurSel);
	}
	else { // 20100328�ǉ�
		m_wndDurationCombo.SetWindowText (strCurText); // 20100328�ǉ�
	}
	return TRUE;
}

// �\�����x�R���{�{�b�N�X�̍X�V
BOOL CMusicalScoreFrame::UpdateResolutionCombo () {
	CString strText;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	long lTimeResolution = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
	long lCurSel = m_wndResolutionCombo.GetCurSel ();
	m_wndResolutionCombo.ResetContent ();
	// �\�����x�R���{�{�b�N�X�̏[��
	CString strFormat;
	VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE)); // %d-4������
	strText.Format (strFormat, lTimeResolution * 1);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE)); // %d-8������
	strText.Format (strFormat, lTimeResolution / 2);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE)); // %d-3�A8������
	strText.Format (strFormat, lTimeResolution / 3);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE)); // %d-16������
	strText.Format (strFormat, lTimeResolution / 4);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE)); // %d-3�A16������
	strText.Format (strFormat, lTimeResolution / 6);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE)); // %d-32������
	strText.Format (strFormat, lTimeResolution / 8);
	m_wndResolutionCombo.AddString (strText);
	VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE)); // %d-3�A32������
	strText.Format (strFormat, lTimeResolution / 12);
	m_wndResolutionCombo.AddString (strText);
	// �J�����g�Z���ݒ�
	if (lCurSel >= 0) {
		m_wndResolutionCombo.SetCurSel (lCurSel);
	}
	return TRUE;
}


// �L�[�X�N���[���o�[(�c)�̃f�U�C���ݒ�
void CMusicalScoreFrame::RecalcTrackScrollInfo () {
	long lTrackInfoCount = GetTrackInfoCount ();
	MusicalScoreTrackInfo* pLastTrackInfo = NULL;
	VERIFY (pLastTrackInfo = GetTrackInfo (lTrackInfoCount - 1));
	long lLastTrackTop = pLastTrackInfo->m_lTop;//GetTrackTop (lTrackInfoCount - 1);
	long lLastTrackHeight = pLastTrackInfo->m_lHeight;//GetTrackHeight (lTrackInfoCount - 1);
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (lLastTrackTop + lLastTrackHeight) * m_lTrackZoom;
	si.nPage = m_lKeyHeight;
	m_wndTrackScroll.SetScrollInfo (&si, TRUE);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
}


// �^�C���X�N���[���o�[(��)�̃f�U�C���ݒ�
void CMusicalScoreFrame::RecalcTimeScrollInfo () {
	long lMeasureInfoCount = m_theMeasureInfoArray.GetSize ();
	MusicalScoreMeasureInfo* pFirstMeasureInfo = NULL;
	VERIFY (pFirstMeasureInfo = GetMeasureInfo (0));
	long lStartX = pFirstMeasureInfo->m_lLeft + pFirstMeasureInfo->m_lSignatureWidth;
	MusicalScoreMeasureInfo* pLastMeasureInfo = NULL;
	VERIFY (pLastMeasureInfo = GetMeasureInfo (MAX (0, lMeasureInfoCount - 2)));
	long lEndX = pLastMeasureInfo->m_lLeft;
	SCROLLINFO si;
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = lStartX * m_lTimeZoom;
	si.nMax = lEndX * m_lTimeZoom;
	si.nPage = m_lTimeWidth;
	m_wndTimeScroll.SetScrollInfo (&si, TRUE);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	// ���F�Y�[���{��1�{�̂Ƃ�4�������̒�����8�s�N�Z���ƒ�`���Ă���B
}


// �g���b�N���z��폜(�e�g���b�N���z����̉������z��Ɖ����O���[�v���z��̍폜���܂�)
BOOL CMusicalScoreFrame::DeleteTrackInfoArray () {
	long i;
	long lNumTrackInfo = GetTrackInfoCount ();
	for (i = 0; i < lNumTrackInfo; i++) {
		MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (i);
		// �g���b�N���z��[i]����3�A���O���[�v���z��폜
		VERIFY (DeleteTripletGroupInfoArray (i));
		// �g���b�N���z��[i]���̉����O���[�v���z��폜
		VERIFY (DeleteNoteGroupInfoArray (i));
		// �g���b�N���z��[i]���̉������z��폜
		VERIFY (DeleteNoteInfoArray (i));
		// �g���b�N���z��[i]���폜
		delete (pTrackInfo);
	}
	m_theTrackInfoArray.RemoveAll ();
	return TRUE;
}

// �g���b�N���z��[lTrackIndex]���̉������z��폜
BOOL CMusicalScoreFrame::DeleteNoteInfoArray (long lTrackIndex) {
	long lNumTrackInfo = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfo);
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	ASSERT (pTrackInfo);
	long j;
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	for (j = 0; j < lNumNoteInfo; j++) {
		delete (pTrackInfo->m_theNoteInfoArray.GetAt (j));
	}
	pTrackInfo->m_theNoteInfoArray.RemoveAll ();
	return TRUE;
}

// �g���b�N���z��[lTrackIndex]���̉����O���[�v���z��폜
BOOL CMusicalScoreFrame::DeleteNoteGroupInfoArray (long lTrackIndex) {
	long lNumTrackInfo = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfo);
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	ASSERT (pTrackInfo);
	long j;
	long lNumNoteGroupInfo = pTrackInfo->m_theNoteGroupInfoArray.GetSize ();
	for (j = 0; j < lNumNoteGroupInfo; j++) {
		delete (pTrackInfo->m_theNoteGroupInfoArray.GetAt (j));
	}
	pTrackInfo->m_theNoteGroupInfoArray.RemoveAll ();
	return TRUE;
}

// �g���b�N���z��[lTrackIndex]����3�A�����O���[�v���z��폜
BOOL CMusicalScoreFrame::DeleteTripletGroupInfoArray (long lTrackIndex) {
	long lNumTrackInfo = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfo);
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	ASSERT (pTrackInfo);
	long j;
	long lNumTripletGroupInfo = pTrackInfo->m_theTripletGroupInfoArray.GetSize ();
	for (j = 0; j < lNumTripletGroupInfo; j++) {
		delete (pTrackInfo->m_theTripletGroupInfoArray.GetAt (j));
	}
	pTrackInfo->m_theTripletGroupInfoArray.RemoveAll ();
	return TRUE;
}

// ���ߏ��z��폜
BOOL CMusicalScoreFrame::DeleteMeasureInfoArray () {
	long j;
	long lNumMeasureInfo = GetMeasureInfoCount ();
	for (j = 0; j < lNumMeasureInfo; j++) {
		free (m_theMeasureInfoArray.GetAt (j));
	}
	m_theMeasureInfoArray.RemoveAll ();
	return TRUE;
}

// ���ߏ��z��X�V
BOOL CMusicalScoreFrame::UpdateMeasureInfoArray () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// ���ߏ��z����������񋭐��폜
	VERIFY (DeleteMeasureInfoArray ());

	long lEndMeasure, lEndBeat, lEndTick;
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndTimeM = lEndTime;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);
	if (lEndBeat != 0 || lEndTick != 0) {
		MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lEndTimeM);
	}
	long lFeedTime = lTimeResolution * 4  * 120; // 4/4��120���߂̗]��
	lEndTime = CLIP (0, lEndTimeM + lFeedTime, 0x7FFFFFFF);
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	long lMeasure;
	long lOldTimeSignature = 4 | (2 << 8);
	long lOldKeySignature = 0 | (0 << 8);
	long lNextMeasureLeft = 0;
	long lCurMeasureTime = 0;
	long lNextMeasureTime = 0;
	for (lMeasure = 0; lMeasure < lEndMeasure; lMeasure++) {
		MusicalScoreMeasureInfo* pMeasureInfo = new MusicalScoreMeasureInfo ();
		if (pMeasureInfo == NULL) {
			break;
		}
		// �����W[pixel]
		long lMeasureLeft = lNextMeasureLeft;
		pMeasureInfo->m_lLeft = lNextMeasureLeft;
		// �J�n����[Tick][Subframe]
		pMeasureInfo->m_lTime = lCurMeasureTime;
		// ����[Tick][Subframe]
		MIDIData_MakeTime (pMIDIData, lMeasure + 1, 0, 0, &lNextMeasureTime);
		pMeasureInfo->m_lDuration = lNextMeasureTime - lCurMeasureTime;
		// ���q�L��
		long lnn, ldd, lcc, lbb;
		MIDIData_FindTimeSignature (pMIDIData, lCurMeasureTime, &lnn, &ldd, &lcc, &lbb);
		long lTimeSignature = ((lnn & 0xFF) | ((ldd & 0xFF) << 8));
		pMeasureInfo->m_lTimeSignature = lTimeSignature;
		// �����L��
		long lsf, lmi;
		MIDIData_FindKeySignature (pMIDIData, lCurMeasureTime, &lsf, &lmi);
		long lKeySignature = ((lsf & 0xFF) | ((lmi & 0xFF) << 8));
		pMeasureInfo->m_lKeySignature = lKeySignature;
		// ���q�L��/�����L���p��[pixel]
		if (lKeySignature != lOldKeySignature ||
			lTimeSignature != lOldTimeSignature ||
			lMeasure == 0) { // �����L���������Ă��Ă��`�悷��̂ŗv����
			pMeasureInfo->m_lSignatureWidth = 16;
		}
		else {
			pMeasureInfo->m_lSignatureWidth = 0;			
		}
		// ���]����[pixel]
		pMeasureInfo->m_lPreWidth = 4;
		// ���߉�������[pixel]
		// ���F�Y�[���{��1�{�̂Ƃ�4�������̒�����8�s�N�Z���ƒ�`���Ă���B
		pMeasureInfo->m_lWidth = 8 * 4 * lnn / (1 << ldd);
		// �E�]����[pixel]
		pMeasureInfo->m_lPostWidth = 0;
		// �t���O
		pMeasureInfo->m_lFlags = 0;
		// ����p�ϐ��̉��ݒ�
		pMeasureInfo->m_lLeftPrint = pMeasureInfo->m_lLeft;
		pMeasureInfo->m_lSignatureWidthPrint = pMeasureInfo->m_lSignatureWidth;
		pMeasureInfo->m_lPreWidthPrint = pMeasureInfo->m_lPreWidth;
		pMeasureInfo->m_lWidthPrint = pMeasureInfo->m_lWidth;
		pMeasureInfo->m_lPostWidthPrint = pMeasureInfo->m_lPostWidth;
		// ���ߏ��\���̂�z��ɒǉ�
		m_theMeasureInfoArray.Add (pMeasureInfo);
		// ����̃��[�v�ɔ�����
		lCurMeasureTime = lNextMeasureTime;
		lNextMeasureLeft = 
			lMeasureLeft + 
			pMeasureInfo->m_lSignatureWidth + 
			pMeasureInfo->m_lPreWidth + 
			pMeasureInfo->m_lWidth + 
			pMeasureInfo->m_lPostWidth;
		lOldTimeSignature = lTimeSignature;
		lOldKeySignature = lKeySignature;
	}
	return TRUE;
}

// �g���b�N���z��X�V(�e�g���b�N���z����̉������z��E�����O���[�v���z��̍X�V�܂�)
BOOL CMusicalScoreFrame::UpdateTrackInfoArray () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;

	// �g���b�N���z����������񋭐��폜(�e�g���b�N���z����̉������z��E�����O���[�v���z��̍폜�܂�)
	VERIFY (DeleteTrackInfoArray ());

	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lNextTop = 0;
	// �e�g���b�N�̍ő�L�[�E�ŏ��L�[�擾����уg���L���E�w���L���E�啈�\�̎���
	long i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		MusicalScoreTrackInfo* pTrackInfo = new MusicalScoreTrackInfo ();
		if (pTrackInfo == NULL) {
			break;
		}
		pTrackInfo->m_lTop = lNextTop;
		if (IsTrackVisible (i)) {
			// ���̃g���b�N�̃m�[�g�C�x���g�̍ő�L�[�ƍŏ��L�[�����߂�B
			long lMaxKey = 0;
			long lMinKey = 127;
			long lCount = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) || MIDIEvent_IsNoteOff (pMIDIEvent)) {
					long lKey = MIDIEvent_GetKey (pMIDIEvent);
					if (lKey < lMinKey) {
						lMinKey = lKey;
					}
					if (lKey > lMaxKey) {
						lMaxKey = lKey;
					}
					lCount++;
				}
			}
			// ���������݂��Ȃ�(�g���L���Ƃ���)
			if (lMinKey == 127 && lMaxKey == 0) {
				pTrackInfo->m_lFlags = 1;
				pTrackInfo->m_lHeight = 24;
			}
			// �g���L��
			else if (lMinKey >= 60 && lCount > 0) {
				pTrackInfo->m_lFlags = 1;
				pTrackInfo->m_lHeight = 24;
			}
			// �w���L��
			else if (lMaxKey <= 60 && lCount > 0) {
				pTrackInfo->m_lFlags = 2;
				pTrackInfo->m_lHeight = 24;
			}
			// �啈�\

			else {
				pTrackInfo->m_lFlags = 3;
				pTrackInfo->m_lHeight = 36;
			}
		}
		else {
			// �\�����Ȃ�
			pTrackInfo->m_lFlags = 0;
			pTrackInfo->m_lHeight = 0;
		}
		// ����p�ϐ��̉��ݒ�
		pTrackInfo->m_lTopPrint = pTrackInfo->m_lTop;
		pTrackInfo->m_lHeightPrint = pTrackInfo->m_lHeight;

		m_theTrackInfoArray.Add (pTrackInfo);
		lNextTop = pTrackInfo->m_lTop + pTrackInfo->m_lHeight;
		i++;
	}

	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {  
		if (IsTrackVisible (i)) {
			// �g���b�N���z��[i]���̉������z����X�V
			UpdateNoteInfoArray (i, pMIDITrack);
			// �g���b�N���z��[i]���̉����O���[�v���z����X�V
			UpdateNoteGroupInfoArray (i, pMIDITrack);
			// �g���b�N���z��[i]����3�A���O���[�v���z����X�V
			UpdateTripletGroupInfoArray (i, pMIDITrack);
		}
		i++;
	}

	return TRUE;
}

// �g���b�N���z��[lTrackIndex]���̉������z����X�V
BOOL CMusicalScoreFrame::UpdateNoteInfoArray (long lTrackIndex, MIDITrack* pMIDITrack) {
	long lNumTrackInfoArray = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfoArray);
	ASSERT (pMIDITrack);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pMIDIEvent = NULL;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// �g���b�N���z��[lTrackIndex]���̉������z�����������폜
	VERIFY (DeleteNoteInfoArray (lTrackIndex));

	// SMPTE�x�[�X�ł͑Ή����Ȃ��B
	if (lTimeMode != MIDIDATA_TPQNBASE) {
		return TRUE;
	}

	// ���݂̕\�����x[�e�B�b�N]���擾
	// 0=4������,1=8������,2=3�A8������,3=16������,4=3�A16������,5=32������,6=3�A32������
	long lViewResolutionIndex = m_wndResolutionCombo.GetCurSel ();
	long lViewResolution = lTimeResolution; // ���ʉ����p�\���𑜓x
	long lViewResolution3 = lTimeResolution; // 3�A�����p�\���𑜓x
	switch (lViewResolutionIndex) {
	case 0: // 4������
		lViewResolution = lTimeResolution;
		lViewResolution3 = lTimeResolution;
		break;
	case 1: // 8������
		lViewResolution = lTimeResolution / 2;
		lViewResolution3 = lTimeResolution / 2;
		break;
	case 2: // 3�A8������
		lViewResolution = lTimeResolution / 2;
		lViewResolution3 = lTimeResolution / 3;
		break;
	case 3: // 16������
		lViewResolution = lTimeResolution / 4;
		lViewResolution3 = lTimeResolution / 3;
		break;
	case 4: // 3�A16������
		lViewResolution = lTimeResolution / 4;
		lViewResolution3 = lTimeResolution / 6;
		break;
	case 5: // 32������
		lViewResolution = lTimeResolution / 8;
		lViewResolution3 = lTimeResolution / 6;
		break;
	case 6: // 3�A32������
		lViewResolution = lTimeResolution / 8;
		lViewResolution3 = lTimeResolution / 12;
		break;
	}
	if (lViewResolution <= 0) {
		lViewResolution = 1;
	}
	if (lViewResolution3 <= 0) {
		lViewResolution = 1;
	}

	// �\���\�ȉ����̒���
	long lDur960 = lTimeResolution * 8;
	long lDur840 = lTimeResolution * 7;
	long lDur720 = lTimeResolution * 6;
	long lDur600 = lTimeResolution * 5;
	long lDur480 = lTimeResolution * 4; // �S����
	long lDur360 = lTimeResolution * 3; // �t�_2������
	long lDur240 = lTimeResolution * 2; // 2������
	long lDur180 = lTimeResolution * 3 / 2; // �t�_4������
	long lDur120 = lTimeResolution; // 4������
	long lDur90 = lTimeResolution * 3 / 4; // �t�_8������
	long lDur80 = lTimeResolution * 2 / 3; // 3�A4������
	long lDur60 = lTimeResolution / 2; // 8������
	long lDur45 = lTimeResolution * 3 / 8; // �t�_16������
	long lDur40 = lTimeResolution / 3; // 3�A8������
	long lDur30 = lTimeResolution / 4; // 16������
	long lDur20 = lTimeResolution / 6; // 3�A16������
	long lDur15 = lTimeResolution / 8; // 32������
	long lDur10 = lTimeResolution / 12; // 3�A32������

	// �␳�m�[�g�I���^�C�����v�Z���Am_lUser1�Ɋi�[
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			long lInTime = MIDIEvent_GetTime (pMIDIEvent);
			long lOutTime = lInTime + MIDIEvent_GetDuration (pMIDIEvent);
			// ���]=��:16�������ŃN�H���^�C�Y
			if (lInTime % (lViewResolution3) == 0) { // 3�An������(��:3�A16�������̔{��)	
				pMIDIEvent->m_lUser1 = lInTime;
			}
			else { // �ʏ�n���������͕t�_n������(��:16�������̔{��)
				pMIDIEvent->m_lUser1 = 
					((lInTime + lViewResolution / 2 - 1) / lViewResolution) * (lViewResolution);
			}
		}
	}

	// �␳�m�[�g�I�t�^�C�����v�Z���Am_lUser2�Ɋi�[
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			// ���̃m�[�g�C�x���g�̕␳��J�n�������擾
			long lNoteOnTime, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick;
			lNoteOnTime = pMIDIEvent->m_lUser1;
			MIDIData_BreakTime (pMIDIData, lNoteOnTime, &lNoteOnMeasure, &lNoteOnBeat, &lNoteOnTick);
			// ���̃m�[�g�C�x���g�̐����ȏI���������擾
			long lNoteOffTime, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick;
			lNoteOffTime = MIDIEvent_GetTime (pMIDIEvent->m_pNextCombinedEvent);
			MIDIData_BreakTime (pMIDIData, lNoteOffTime, &lNoteOffMeasure, &lNoteOffBeat, &lNoteOffTick);
			// ���̎����̃m�[�g�C�x���g�̕␳��J�n�������擾
			long lNextOnTime, lNextOnMeasure, lNextOnBeat, lNextOnTick;
			MIDIEvent* pNextEvent = MIDIEvent_GetNextEvent (pMIDIEvent);
			while (pNextEvent) {
				if (MIDIEvent_IsNoteOn (pNextEvent) && MIDIEvent_IsNote (pNextEvent)) {
					if (pNextEvent->m_lUser1 > pMIDIEvent->m_lUser1) {
						break;
					}
				}
				pNextEvent = MIDIEvent_GetNextEvent (pNextEvent);
			}
			if (pNextEvent) {
				lNextOnTime = pNextEvent->m_lUser1;
				MIDIData_BreakTime (pMIDIData, lNextOnTime, &lNextOnMeasure, &lNextOnBeat, &lNextOnTick);
				if (lNextOnMeasure > lNoteOffMeasure) {
					lNextOnTime = GetMeasureTime (lNoteOffMeasure + 1);
				}
			}
			else {
				lNextOnTime = GetMeasureTime (lNoteOnMeasure + 1);
			}
			// 3�An�������̏ꍇ
			if (lNoteOnTime % lViewResolution3 == 0 &&
				(lNextOnTime - lNoteOnTime) % lViewResolution != 0) {
				pMIDIEvent->m_lUser2 = 
					((lNoteOffTime + lViewResolution3 / 2 - 1) / lViewResolution3) * lViewResolution3;
				if (pMIDIEvent->m_lUser2 - pMIDIEvent->m_lUser1 < lViewResolution3) {
					pMIDIEvent->m_lUser2 = pMIDIEvent->m_lUser1 + lViewResolution3; // (��)�t�B��3�A16������
				}
				
			}
			// �ʏ�n���������͕t�_n�������̏ꍇ
			else { 
				pMIDIEvent->m_lUser2 = 
					((lNoteOffTime + lViewResolution / 2 - 1) / lViewResolution) * lViewResolution;
				if (pMIDIEvent->m_lUser2 - pMIDIEvent->m_lUser1 < lViewResolution) {
					pMIDIEvent->m_lUser2 = pMIDIEvent->m_lUser1 + lViewResolution; // (��)�t�B��16������
				}
			}
		}
	}

	// �������̐����Ɣz��ւ̓o�^
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			long lNoteOnMeasure, lNoteOnBeat, lNoteOnTick;
			long lNoteOffMeasure, lNoteOffBeat, lNoteOffTick;

			MIDIData_BreakTime (pMIDIData, pMIDIEvent->m_lUser1, &lNoteOnMeasure, &lNoteOnBeat, &lNoteOnTick);
			MIDIData_BreakTime (pMIDIData, pMIDIEvent->m_lUser2, &lNoteOffMeasure, &lNoteOffBeat, &lNoteOffTick);

			long lMeasure;
			// ���̃m�[�g�C�x���g�̊e�������߂ɂ���
			for (lMeasure = lNoteOnMeasure; lMeasure <= lNoteOffMeasure; lMeasure++) {
				MusicalScoreMeasureInfo* pMeasureInfo = GetMeasureInfo (lMeasure);
				long lMeasureTime =  pMeasureInfo->m_lTime; // ���̏��߂̊J�n�^�C��[�e�B�b�N]
				long lnn = (pMeasureInfo->m_lTimeSignature & 0xFF); // ���q(���̏��߂̔���)[��]
				long ldd = (pMeasureInfo->m_lTimeSignature & 0xFF00) >> 8; // ����
				long lBeatDur = 4 * lTimeResolution / (0x01 << ldd); // 1���̒���[�e�B�b�N]
				long lNextMeasureTime = lMeasureTime + lBeatDur * lnn; // ���̏��߂̊J�n�^�C��[�e�B�b�N]
				if (pMIDIEvent->m_lUser2 - lMeasureTime <= 0) {
					break;
				}
				// �p�^�[��A:���̏��߂Ŏn�܂肱�̏��߂ŏI���
				if (lMeasure == lNoteOnMeasure && 
					(lMeasure == lNoteOffMeasure || (lMeasure == lNoteOffMeasure - 1 && lNoteOffBeat == 0 && lNoteOffTick == 0))) {
					long lDur = pMIDIEvent->m_lUser2 - pMIDIEvent->m_lUser1;
					// �ʏ퉹���ŕ\���ł��Ȃ��ꍇ
					if (lDur != lDur960 &&
						lDur != lDur840 &&
						lDur != lDur720 &&
						lDur != lDur600 &&
						lDur != lDur480 &&
						lDur != lDur360 &&
						lDur != lDur240 && 
						lDur != lDur180 &&
						lDur != lDur120 &&
						lDur != lDur90 &&
						lDur != lDur80 &&
						lDur != lDur60 &&
						lDur != lDur45 &&
						lDur != lDur40 &&
						lDur != lDur30 &&
						lDur != lDur20 &&
						lDur != lDur15 &&
						lDur != lDur10) {
						long lDivideTime1 = 0;
						long lDivideMeasure1 = 0;
						long lDivideBeat1 = 0;
						long lDivideTick1 = 0;
						long lDivideTime2 = 0;
						long lDivideMeasure2 = 0;
						long lDivideBeat2 = 0;
						long lDivideTick2 = 0;
						// a)�ŏ��̔��ɔ��[������A���̔��ɂ܂�����ꍇ
						if (lNoteOnTick > 0 && (lDur > lBeatDur || (lNoteOnBeat != lNoteOffBeat && lNoteOffTick > 0 ))) {
							lDivideTime1 = lMeasureTime + (lNoteOnBeat + 1) * lBeatDur;
							lDivideMeasure1 = lMeasure;
							lDivideBeat1 = lNoteOnBeat + 1;
							lDivideTick1 = 0;
						}
						// b)�Ō�̔��ɔ��[������A�O�̔�����Ȃ���ꍇ
						if (lNoteOnBeat != lNoteOffBeat && lNoteOffTick > 0) {
							lDivideTime2 = lMeasureTime + (lNoteOffBeat) * lBeatDur;
							lDivideMeasure2 = lMeasure;
							lDivideBeat2 = lNoteOffBeat;
							lDivideTick2 = 0;
						}
						// c)�ŏ��̔����͍Ō�̔��ɂȂ���ׂ����[�͂Ȃ��ꍇ
						if (lDivideTime1 == lDivideTime2) {
							// 4���������Z���A���������ł͕\���ł��Ȃ������̏ꍇ�A8������+�c��ŕ\��
							if (lDur < lDur120) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur60;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// 2���������Z���A���������ł͕\���ł��Ȃ������̏ꍇ�A4������+�c��ŕ\��
							else if (lDur < lDur240) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur120;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// �t�_2���������Z���A���������ł͕\���ł��Ȃ������̏ꍇ�A2������+�c��ŕ\��
							else if (lDur < lDur360) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur240;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// 1���������Z���A���������ł͕\���ł��Ȃ������̏ꍇ�A�t�_2������+�c��ŕ\��
							else if (lDur < lDur480) {
								lDivideTime2 = pMIDIEvent->m_lUser1 + lDur360;
								MIDIData_BreakTime (pMIDIData, lDivideTime2, &lDivideMeasure2, &lDivideBeat2, &lDivideTick2);
							}
							// ���̑��̏ꍇ�A������������߁A�ł��߂������ŕ\���B
							else {
								lDivideTime2 = 0;
							}
						}
						// �����E�����Ȃ�(����1��)
						if (lDivideTime1 == 0 && lDivideTime2 == 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000000);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
						}
						// �ŏ��̔����E�ŕ���(����2��)
						else if (lDivideTime1 != 0 && lDivideTime2 == 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								lDivideTime1, lDivideMeasure1, lDivideBeat1, lDivideTick1, 0x00000001);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
							MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
								(pMIDIEvent, lDivideTime1, lDivideMeasure1, lDivideBeat1, lDivideTick1,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
							AddNoteInfo (pTrackInfo, pNoteInfo2);
						}
						// �Ō�̔����E�ŕ���(����2��)
						else if (lDivideTime1 == 0 && lDivideTime2 != 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								lDivideTime2, lDivideMeasure2, lDivideBeat2, lDivideTick2, 0x00000001);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
							MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
								(pMIDIEvent, lDivideTime2, lDivideMeasure2, lDivideBeat2, lDivideTick2,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
							AddNoteInfo (pTrackInfo, pNoteInfo2);
						}
						// �ŏ��̔����E�ƍŌ�̔����E�ŕ���(����3��)
						else if (lDivideTime1 != 0 && lDivideTime2 != 0) {
							MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
								(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
								lDivideTime1,lDivideMeasure1, lDivideBeat1, lDivideTick1, 0x00000001);
							AddNoteInfo (pTrackInfo, pNoteInfo1);
							MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
								(pMIDIEvent, lDivideTime1, lDivideMeasure1, lDivideBeat1, lDivideTick1,
								lDivideTime2,lDivideMeasure2, lDivideBeat2, lDivideTick2, 0x00000003);
							AddNoteInfo (pTrackInfo, pNoteInfo2);
							MusicalScoreNoteInfo* pNoteInfo3 = CreateNote
								(pMIDIEvent, lDivideTime2, lDivideMeasure2, lDivideBeat2, lDivideTick2,
								pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
							AddNoteInfo (pTrackInfo, pNoteInfo3);
						}
					}
					// �ʏ�̉����ŕ\���ł���ꍇ
					else {
						MusicalScoreNoteInfo* pNoteInfo = CreateNote
							(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick,
							pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000000);
						AddNoteInfo (pTrackInfo, pNoteInfo);
					}
				}
				// �p�^�[��B:���̏��߂���n�܂莟�̏��߂փ^�C�ő���
				else if (lMeasure == lNoteOnMeasure && lMeasure < lNoteOffMeasure) {
					long lDur = lNextMeasureTime - pMIDIEvent->m_lUser1;
					// �����E���܂����ł���A�ʏ퉹���ŕ\���ł��Ȃ��ꍇ
					if (lNoteOnBeat < lnn - 1 &&
						lDur != lDur960 &&
						lDur != lDur840 &&
						lDur != lDur720 &&
						lDur != lDur600 &&
						lDur != lDur480 &&
						lDur != lDur360 &&
						lDur != lDur240 && 
						lDur != lDur180 &&
						lDur != lDur120 &&
						lDur != lDur90 &&
						lDur != lDur80 &&
						lDur != lDur60 &&
						lDur != lDur45 &&
						lDur != lDur40 &&
						lDur != lDur30 &&
						lDur != lDur20 &&
						lDur != lDur15 &&
						lDur != lDur10) {
						long lDivideTime = lMeasureTime + (lNoteOnBeat + 1) * lBeatDur;
						long lDivideMeasure = lMeasure;
						long lDivideBeat = lNoteOnBeat + 1;
						long lDivideTick = 0;
						MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
							(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick, 
							lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 0x00000001);
						AddNoteInfo (pTrackInfo, pNoteInfo1);
						MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
							(pMIDIEvent, lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 
							lNextMeasureTime, lMeasure + 1, 0, 0, 0x00000006);
						AddNoteInfo (pTrackInfo, pNoteInfo2);
					}
					// �����ЂƂŕ\���ł���ꍇ
					else {
						MusicalScoreNoteInfo* pNoteInfo = CreateNote
							(pMIDIEvent, pMIDIEvent->m_lUser1, lNoteOnMeasure, lNoteOnBeat, lNoteOnTick, 
							lNextMeasureTime, lNoteOnMeasure + 1, 0, 0, 0x00000004);
						AddNoteInfo (pTrackInfo, pNoteInfo);
					}
				}
				// �p�^�[��C:�O�̏��߂���^�C�ő������̏��߂ŏI������
				else if (lMeasure > lNoteOnMeasure && 
					(lMeasure == lNoteOffMeasure || (lMeasure == lNoteOffMeasure - 1 && lNoteOffBeat == 0 && lNoteOffTick == 0))) {
					long lDur = pMIDIEvent->m_lUser2 - lMeasureTime;
					// �����E���܂����ł���A�ʏ퉹���ŕ\���ł��Ȃ��ꍇ
					if (lNoteOffBeat > 0 &&
						lDur != lDur960 &&
						lDur != lDur840 &&
						lDur != lDur720 &&
						lDur != lDur600 &&
						lDur != lDur480 &&
						lDur != lDur360 &&
						lDur != lDur240 && 
						lDur != lDur180 &&
						lDur != lDur120 &&
						lDur != lDur90 &&
						lDur != lDur80 &&
						lDur != lDur60 &&
						lDur != lDur45 &&
						lDur != lDur40 &&
						lDur != lDur30 &&
						lDur != lDur20 &&
						lDur != lDur15 &&
						lDur != lDur10) {
						long lDivideTime = lMeasureTime + (lNoteOffBeat) * lBeatDur;
						long lDivideMeasure = lMeasure;
						long lDivideBeat = lNoteOffBeat;
						long lDivideTick = 0;
						MusicalScoreNoteInfo* pNoteInfo1 = CreateNote
							(pMIDIEvent, lMeasureTime, lMeasure, 0, 0,
							lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 0x00000007);
						AddNoteInfo (pTrackInfo, pNoteInfo1);
						MusicalScoreNoteInfo* pNoteInfo2 = CreateNote
							(pMIDIEvent, lDivideTime, lDivideMeasure, lDivideBeat, lDivideTick, 
							pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000002);
						AddNoteInfo (pTrackInfo, pNoteInfo2);
					}
					// �����ЂƂŕ\���ł���ꍇ
					else {
						MusicalScoreNoteInfo* pNoteInfo = CreateNote
							(pMIDIEvent, lMeasureTime, lMeasure, 0, 0,
							pMIDIEvent->m_lUser2, lNoteOffMeasure, lNoteOffBeat, lNoteOffTick, 0x00000005);
						AddNoteInfo (pTrackInfo, pNoteInfo);
					}

				}
				// �p�^�[��D:�O�̏��߂���^�C�ő������̏��߂փ^�C�ő���(�S�����̂�)
				else if (lMeasure > lNoteOnMeasure && lMeasure < lNoteOffMeasure) {
					MusicalScoreNoteInfo* pNoteInfo = CreateNote 
						(pMIDIEvent, lMeasureTime, lMeasure, 0, 0,
						lNextMeasureTime, lMeasure + 1, 0, 0, 0x00000008);
					AddNoteInfo (pTrackInfo, pNoteInfo);
				}
			}
		}
	}

	// �������̃|�C���^�A��
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	for (j = 0; j < lNumNoteInfo; j++) {
		MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		if (j > 0) {
			pNoteInfo->m_pPrevNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j - 1));
		}
		else {
			pNoteInfo->m_pPrevNoteInfo = NULL;
		}
		if (j < lNumNoteInfo - 1) {
			pNoteInfo->m_pNextNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j + 1));
		}
		else {
			pNoteInfo->m_pNextNoteInfo = NULL;
		}
	}

	// �␳�m�[�g�I���^�C���ƕ␳�m�[�g�I�t�^�C���̃[�����Z�b�g
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
			pMIDIEvent->m_lUser1 = 0;
			pMIDIEvent->m_lUser2 = 0;
		}
	}
	return TRUE;
}

// �g���b�N���z��[lTrackIndex]���̉����O���[�v���z����X�V
BOOL CMusicalScoreFrame::UpdateNoteGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack) {
	long lNumTrackInfoArray = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfoArray);
	ASSERT (pMIDITrack);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// �\���\�ȉ����̒���
	long lDur960 = lTimeResolution * 8;
	long lDur840 = lTimeResolution * 7;
	long lDur720 = lTimeResolution * 6;
	long lDur600 = lTimeResolution * 5;
	long lDur480 = lTimeResolution * 4; // �S����
	long lDur360 = lTimeResolution * 3; // �t�_2������
	long lDur240 = lTimeResolution * 2; // 2������
	long lDur180 = lTimeResolution * 3 / 2; // �t�_4������
	long lDur120 = lTimeResolution; // 4������
	long lDur90 = lTimeResolution * 3 / 4; // �t�_8������
	long lDur80 = lTimeResolution * 2 / 3; // 3�A4������
	long lDur60 = lTimeResolution / 2; // 8������
	long lDur45 = lTimeResolution * 3 / 8; // �t�_16������
	long lDur40 = lTimeResolution / 3; // 3�A8������
	long lDur30 = lTimeResolution / 4; // 16������
	long lDur20 = lTimeResolution / 6; // 3�A16������
	long lDur15 = lTimeResolution / 8; // 32������
	long lDur10 = lTimeResolution / 12; // 3�A32������

	// �g���b�N���z��[lTrackIndex]���̉����O���[�v���z�����������폜
	VERIFY (DeleteNoteGroupInfoArray (lTrackIndex));

	// �����O���[�v���̐����Ɣz��ւ̓o�^
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	MusicalScoreNoteGroupInfo* pNoteGroupInfo = NULL;
	BOOL bGroupContinueFlag = FALSE;
	for (j = 0; j < lNumNoteInfo; j++) {
		MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
		// ���̉����͊��t�����K�v�ł���ꍇ
		if (lDur < lDur120 && lDur != lDur80) {
			// ���̉������܂��ǂ��̃O���[�v�ɂ��������Ă��Ȃ��ꍇ
			if (pNoteInfo->m_pNoteGroupInfo == NULL) {
				// �V���������O���[�v���𐶐�����K�v������ꍇ
				if (bGroupContinueFlag == FALSE) {
					pNoteGroupInfo = 
						CreateNoteGroupInfo (pNoteInfo);
					AddNoteGroupInfo (pTrackInfo, pNoteGroupInfo);
				}
				pNoteInfo->m_pNoteGroupInfo = pNoteGroupInfo;
				pNoteGroupInfo->m_pLastNoteInfo = pNoteInfo;
				// ���̉���������ꍇ
				if (j + 1 < lNumNoteInfo) {
					MusicalScoreNoteInfo* pNextNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j + 1));
					// ���̉����̃I���^�C�������݂̉����̃I�t�^�C���Ɨ���Ă���ꍇ	
					// ���̉����̃I���^�C�������݂̉����̃I���^�C���Ɠ���łȂ��ꍇ
					if (pNoteInfo->m_lNoteOffTime != pNextNoteInfo->m_lNoteOnTime &&
						pNoteInfo->m_lNoteOnTime != pNextNoteInfo->m_lNoteOnTime) {
						bGroupContinueFlag = FALSE; // ���݂̉����O���[�v�͏I��
					}
					// ���̉����̏����������݂̉����̏������ƈقȂ�ꍇ
					else if (pNoteInfo->m_lNoteOnBeat != pNextNoteInfo->m_lNoteOnBeat ||
						pNoteInfo->m_lNoteOnMeasure != pNextNoteInfo->m_lNoteOnMeasure) {
						bGroupContinueFlag = FALSE; // ���݂̉����O���[�v�͏I��
					}
					// ���̉�����8�������𒴂���̒����̏ꍇ
					else if (pNextNoteInfo->m_lNoteOffTime - pNextNoteInfo->m_lNoteOnTime > lTimeResolution / 2) {
						bGroupContinueFlag = FALSE; // ���݂̉����O���[�v�͏I��
					}
					else {
						bGroupContinueFlag = TRUE; // ���݂̉����O���[�v�𑱂���
					}
				}
				// ���̉������Ȃ��ꍇ
				else {
					bGroupContinueFlag = FALSE;
				}
				// �O���[�v���ő�L�[�E�ŏ��L�[�̍X�V
				long lKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
				if (pNoteGroupInfo->m_lMaxKey < lKey) {
					pNoteGroupInfo->m_lMaxKey = lKey;
				}
				if (pNoteGroupInfo->m_lMinKey > lKey) {
					pNoteGroupInfo->m_lMinKey = lKey;
				}
				// �O���[�v���̍ő咷���E�ŏ������̍X�V
				long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
				if (pNoteGroupInfo->m_lMaxDur < lDur) {
					pNoteGroupInfo->m_lMaxDur = lDur;
				}
				if (pNoteGroupInfo->m_lMinDur > lDur) {
					pNoteGroupInfo->m_lMinDur = lDur;
				}
				// �����O���[�v���̉��������C���N�������g
				pNoteGroupInfo->m_lNumNoteInfo++;
			}
		}
	}

	return TRUE;
}


// �g���b�N���z��[lTrackIndex]����3�A�����O���[�v���z����X�V
BOOL CMusicalScoreFrame::UpdateTripletGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack) {
	long lNumTrackInfoArray = GetTrackInfoCount ();
	ASSERT (0 <= lTrackIndex && lTrackIndex < lNumTrackInfoArray);
	ASSERT (pMIDITrack);
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// �\���\�ȉ����̒���
	long lDur960 = lTimeResolution * 8;
	long lDur840 = lTimeResolution * 7;
	long lDur720 = lTimeResolution * 6;
	long lDur600 = lTimeResolution * 5;
	long lDur480 = lTimeResolution * 4; // �S����
	long lDur360 = lTimeResolution * 3; // �t�_2������
	long lDur240 = lTimeResolution * 2; // 2������
	long lDur180 = lTimeResolution * 3 / 2; // �t�_4������
	long lDur120 = lTimeResolution; // 4������
	long lDur90 = lTimeResolution * 3 / 4; // �t�_8������
	long lDur80 = lTimeResolution * 2 / 3; // 3�A4������
	long lDur60 = lTimeResolution / 2; // 8������
	long lDur45 = lTimeResolution * 3 / 8; // �t�_16������
	long lDur40 = lTimeResolution / 3; // 3�A8������
	long lDur30 = lTimeResolution / 4; // 16������
	long lDur20 = lTimeResolution / 6; // 3�A16������
	long lDur15 = lTimeResolution / 8; // 32������
	long lDur10 = lTimeResolution / 12; // 3�A32������


	// �g���b�N���z��[lTrackIndex]���̉����O���[�v���z�����������폜
	VERIFY (DeleteTripletGroupInfoArray (lTrackIndex));

	// 3�A�����O���[�v���̐����Ɣz��ւ̓o�^
	MusicalScoreTrackInfo* pTrackInfo = GetTrackInfo (lTrackIndex);
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	MusicalScoreTripletGroupInfo* pTripletGroupInfo = NULL;
	BOOL bGroupContinueFlag = FALSE;
	for (j = 0; j < lNumNoteInfo; j++) {
		MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
		// 3�A�����̏ꍇ
		if (lDur == lDur80 ||
			lDur == lDur40 ||
			lDur == lDur20 || 
			lDur == lDur10) {
			// ���̉������܂��ǂ���3�A�O���[�v�ɂ��������Ă��Ȃ��ꍇ
			if (pNoteInfo->m_pTripletGroupInfo == NULL) {
				// �V����3�A�����O���[�v���𐶐�����K�v������ꍇ
				if (bGroupContinueFlag == FALSE) {
					VERIFY (pTripletGroupInfo = CreateTripletGroupInfo (pNoteInfo));
					VERIFY (AddTripletGroupInfo (pTrackInfo, pTripletGroupInfo));
				}
				// �|�C���^�̐ݒ�
				pNoteInfo->m_pTripletGroupInfo = pTripletGroupInfo;
				pTripletGroupInfo->m_pLastNoteInfo = pNoteInfo;
				// ���̉���������ꍇ
				if (j + 1 < lNumNoteInfo) {
					MusicalScoreNoteInfo* pNextNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j + 1));
					// ���̉��������̉�������ł���3�A�����łȂ��ꍇ
					long lNextDur = pNextNoteInfo->m_lNoteOffTime - pNextNoteInfo->m_lNoteOnTime;
					if (pNextNoteInfo->m_lNoteOnTime >= pNoteInfo->m_lNoteOffTime &&
						lNextDur != lDur80 &&
						lNextDur != lDur40 &&
						lNextDur != lDur20 &&
						lNextDur != lDur10) {
						bGroupContinueFlag = FALSE; // ���݂�3�A�����O���[�v�͏I��
					}
					// ���̉����̏������߂����݂̉����̏������߂ƈقȂ�ꍇ
					else if (pNoteInfo->m_lNoteOnMeasure != pNextNoteInfo->m_lNoteOnMeasure) {
						bGroupContinueFlag = FALSE; // ���݂�3�A�����O���[�v�͏I��
					}
					// ���̉����̏����������݂̉����̏������ƈقȂ�ꍇ(�����������E�Ő؂�Ă���ꍇ�̂�)
					else if (pNoteInfo->m_lNoteOnBeat != pNextNoteInfo->m_lNoteOnBeat &&
						pNextNoteInfo->m_lNoteOffTick > 0) {
						bGroupContinueFlag = FALSE; // ���݂�3�A�����O���[�v�͏I��
					}
					else {
						bGroupContinueFlag = TRUE; // ���݂�3�A�����O���[�v�𑱂���
					}
				}
				// ���̉������Ȃ��ꍇ
				else {
					bGroupContinueFlag = FALSE;
				}
				// �O���[�v���ő�L�[�E�ŏ��L�[�̍X�V
				long lKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
				if (pTripletGroupInfo->m_lMaxKey < lKey) {
					pTripletGroupInfo->m_lMaxKey = lKey;
				}
				if (pTripletGroupInfo->m_lMinKey > lKey) {
					pTripletGroupInfo->m_lMinKey = lKey;
				}
				// �O���[�v���̍ő咷���E�ŏ������̍X�V
				long lDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
				if (pTripletGroupInfo->m_lMaxDur < lDur) {
					pTripletGroupInfo->m_lMaxDur = lDur;
				}
				if (pTripletGroupInfo->m_lMinDur > lDur) {
					pTripletGroupInfo->m_lMinDur = lDur;
				}
				// 3�A�����O���[�v���̉��������C���N�������g
				pTripletGroupInfo->m_lNumNoteInfo++;
			}
			// 3�A���J�n����[Tick]�ƏI������[Tick]�̕␳
			if (pTripletGroupInfo->m_lMinDur == lDur80) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur80) * lDur80;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur80 - 1) / lDur80) * lDur80;
			}
			else if (pTripletGroupInfo->m_lMinDur == lDur40) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur40) * lDur40;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur40 - 1) / lDur40) * lDur40;
			}
			else if (pTripletGroupInfo->m_lMinDur == lDur20) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur20) * lDur20;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur20 - 1) / lDur20) * lDur20;
			}
			else if (pTripletGroupInfo->m_lMinDur == lDur10) {
				pTripletGroupInfo->m_lBeginTime = 
					(pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime / lDur10) * lDur10;
				pTripletGroupInfo->m_lEndTime = 
					((pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOffTime + lDur10 - 1) / lDur10) * lDur10;
			}
		}
	}

	return TRUE;
}



MusicalScoreNoteInfo* CMusicalScoreFrame::CreateNote 
	(MIDIEvent* pNoteEvent, long lNoteOnTime, long lNoteOnMeasure, long lNoteOnBeat, long lNoteOnTick, 
	long lNoteOffTime, long lNoteOffMeasure, long lNoteOffBeat, long lNoteOffTick, long lFlags) {
	MusicalScoreNoteInfo* pNoteInfo = new MusicalScoreNoteInfo;
	if (pNoteInfo == NULL) {
		return NULL;
	}
	pNoteInfo->m_pNoteOnEvent = pNoteEvent;
	pNoteInfo->m_pNoteOffEvent = pNoteEvent->m_pNextCombinedEvent;
	pNoteInfo->m_lNoteOnTime = lNoteOnTime;
	pNoteInfo->m_lNoteOnMeasure = lNoteOnMeasure;
	pNoteInfo->m_lNoteOnBeat = lNoteOnBeat;
	pNoteInfo->m_lNoteOnTick = lNoteOnTick;
	pNoteInfo->m_lNoteOffTime = lNoteOffTime;
	pNoteInfo->m_lNoteOffMeasure = lNoteOffMeasure;
	pNoteInfo->m_lNoteOffBeat = lNoteOffBeat;
	pNoteInfo->m_lNoteOffTick = lNoteOffTick;
	pNoteInfo->m_lFlags = lFlags;
	pNoteInfo->m_lSelected = (pNoteEvent->m_lUserFlag & MIDIEVENT_SELECTED) ? 1 : 0;
	pNoteInfo->m_pNoteGroupInfo = NULL;
	pNoteInfo->m_pTripletGroupInfo = NULL;
	pNoteInfo->m_pNextNoteInfo = NULL;
	pNoteInfo->m_pPrevNoteInfo = NULL;
	ASSERT (MIDIEvent_GetParent (pNoteEvent));
	return pNoteInfo;
}

void CMusicalScoreFrame::DeleteNoteInfo (MusicalScoreNoteInfo* pNoteInfo) {
	delete (pNoteInfo);
}

BOOL CMusicalScoreFrame::AddNoteInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteInfo* pNoteInfo) {
	long lNumNoteInfo = pTrackInfo->m_theNoteInfoArray.GetSize ();
	long j;
	// lNoteOnTime���Ɏw��g���b�N�̉������z��ɉ�������o�^����B
	for (j = lNumNoteInfo - 1; j >= 0; j--) {
		MusicalScoreNoteInfo* pNoteInfo2 = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
		if (pNoteInfo2->m_lNoteOnTime <= pNoteInfo->m_lNoteOnTime) {
			break;
		}
	}
	pTrackInfo->m_theNoteInfoArray.InsertAt (j + 1, pNoteInfo);
	return TRUE;
}

MusicalScoreNoteGroupInfo* CMusicalScoreFrame::CreateNoteGroupInfo 
	(MusicalScoreNoteInfo* pNoteInfo) {
	MusicalScoreNoteGroupInfo* pNoteGroupInfo = new MusicalScoreNoteGroupInfo;
	if (pNoteGroupInfo == NULL) {
		return NULL;
	}
	pNoteGroupInfo->m_pFirstNoteInfo = pNoteInfo;
	pNoteGroupInfo->m_pLastNoteInfo = pNoteInfo;
	pNoteGroupInfo->m_lMinKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pNoteGroupInfo->m_lMaxKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pNoteGroupInfo->m_lMinDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pNoteGroupInfo->m_lMaxDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pNoteGroupInfo->m_lNumNoteInfo = 1;
	return pNoteGroupInfo;
}

void CMusicalScoreFrame::DeleteNoteGroupInfo (MusicalScoreNoteGroupInfo* pNoteGroupInfo) {
	delete (pNoteGroupInfo);
}

BOOL CMusicalScoreFrame::AddNoteGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteGroupInfo* pNoteGroupInfo) {
	pTrackInfo->m_theNoteGroupInfoArray.Add (pNoteGroupInfo);
	return TRUE;
}

MusicalScoreTripletGroupInfo* CMusicalScoreFrame::CreateTripletGroupInfo 
	(MusicalScoreNoteInfo* pNoteInfo) {
	MusicalScoreTripletGroupInfo* pTripletGroupInfo = new MusicalScoreTripletGroupInfo;
	if (pTripletGroupInfo == NULL) {
		return NULL;
	}
	pTripletGroupInfo->m_pFirstNoteInfo = pNoteInfo;
	pTripletGroupInfo->m_pLastNoteInfo = pNoteInfo;
	pTripletGroupInfo->m_lMinKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pTripletGroupInfo->m_lMaxKey = MIDIEvent_GetKey (pNoteInfo->m_pNoteOnEvent);
	pTripletGroupInfo->m_lMinDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pTripletGroupInfo->m_lMaxDur = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	pTripletGroupInfo->m_lNumNoteInfo = 1;
	return pTripletGroupInfo;
}

void CMusicalScoreFrame::DeleteTripletGroupInfo (MusicalScoreTripletGroupInfo* pTripletGroupInfo) {
	delete (pTripletGroupInfo);
}

BOOL CMusicalScoreFrame::AddTripletGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreTripletGroupInfo* pTripletGroupInfo) {
	pTrackInfo->m_theNoteGroupInfoArray.Add (pTripletGroupInfo);
	return TRUE;
}



//-----------------------------------------------------------------------------
// �I�[�o�[���C�h
//-----------------------------------------------------------------------------

// �E�B���h�E�������O�̍\���̐ݒ�
BOOL CMusicalScoreFrame::PreCreateWindow (CREATESTRUCT& cs) {
	return  (CWnd::PreCreateWindow(cs));
}

// �E�B���h�E�^�C�g���̎����ݒ�(CMDIChildWnd::OnUpdateFrameTitle�̃I�[�o�[���C�h)
void CMusicalScoreFrame::OnUpdateFrameTitle (BOOL bAddToTitle) {
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle (bAddToTitle);
	if ((GetStyle() & FWS_ADDTOTITLE) == 0) {
		return;     // leave child window alone!
	}
	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL) {
		CString strMusicalScore;
		strMusicalScore.LoadString (IDS_MUSICALSCORE);
		CString strTitle;
		if (m_nWindow > 0) {
			strTitle.Format (_T("%s:%d(%s)"), pDocument->GetTitle (), m_nWindow, strMusicalScore);
		}
		else {
			strTitle.Format (_T("%s(%s)"), pDocument->GetTitle (), strMusicalScore);
		}
		this->SetWindowText (strTitle);
	}
}

// �Ĕz�u�p�֐�(CFrameWnd::RecalcLayout�̃I�[�o�[���C�h)
void CMusicalScoreFrame::RecalcLayout (BOOL bNotify) {

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
	m_wndToolBar1.GetWindowRect (&rcToolBar1); 
	m_lToolBar1Height = rcToolBar1.Height ();

	// ���������̈ʒu�v�Z
	if (rcClient.Height () - m_lToolBar1Height < 
		MUSICALSCOREFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		MUSICALSCOREFRAME_BORDERHEIGHT * 2) {
		m_lScaleHeight = rcClient.Height () - m_lHScrollBarHeight -
			MUSICALSCOREFRAME_BORDERHEIGHT * 2;
		m_lKeyHeight = 0;
	}
	else if (rcClient.Height () - m_lToolBar1Height < 
		MUSICALSCOREFRAME_SCALEHEIGHT + m_lHScrollBarHeight +
		MUSICALSCOREFRAME_BORDERHEIGHT * 2) {
		m_lScaleHeight = MUSICALSCOREFRAME_SCALEHEIGHT;
		m_lKeyHeight = 0;
	}
	else {
		m_lScaleHeight = MUSICALSCOREFRAME_SCALEHEIGHT;
		m_lKeyHeight = rcClient.Height () -  m_lToolBar1Height -
			m_lScaleHeight - m_lHScrollBarHeight -
			MUSICALSCOREFRAME_BORDERHEIGHT * 2;
	}
	//m_lVelHeight = __min (m_lVelHeight, 128);

	// �������̈ʒu�v�Z
	if (rcClient.Width () <
		MUSICALSCOREFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		MUSICALSCOREFRAME_BORDERWIDTH * 4) {
		m_lScaleWidth = rcClient.Width () -
			m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 4;
		m_lTimeWidth = 0;
		m_lTrackListWidth = 0;
			
	}
	
	else if (rcClient.Width () <
		MUSICALSCOREFRAME_SCALEWIDTH + m_lVScrollBarWidth + 
		MUSICALSCOREFRAME_BORDERWIDTH * 4 + MUSICALSCOREFRAME_SPLITTERWIDTH + m_lTrackListWidth) {
		//m_lScaleWidth = rcClient.Width () - 
		//	m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 2;
		m_lScaleWidth = MUSICALSCOREFRAME_SCALEWIDTH;
		m_lTimeWidth = 0;
		m_lTrackListWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 4 -
			MUSICALSCOREFRAME_SPLITTERWIDTH;

	}
	else {
		m_lScaleWidth = MUSICALSCOREFRAME_SCALEWIDTH;
		m_lTimeWidth = rcClient.Width () - m_lScaleWidth -
			m_lVScrollBarWidth - MUSICALSCOREFRAME_BORDERWIDTH * 4 -
			MUSICALSCOREFRAME_SPLITTERWIDTH - m_lTrackListWidth;
		m_lTrackListWidth = m_lTrackListWidth;
	}

	// �r���[�̐���
	if (m_pScaleView) {
		m_pScaleView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH, 
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT, 
			m_lScaleWidth, m_lScaleHeight);
	}

	if (m_pTimeScaleView) {
		m_pTimeScaleView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth, 
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT, 
			m_lTimeWidth, m_lScaleHeight);
	}

	if (m_pTrackScaleView) {
		m_pTrackScaleView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH, 
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lScaleWidth, m_lKeyHeight);
	}

	if (m_pTrackTimeView) {
		m_pTrackTimeView->MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth,
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight, 
			m_lTimeWidth, m_lKeyHeight);
	}

	// �X�N���[���o�[�̐���
	m_wndTimeScroll.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH,
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight,
		m_lScaleWidth + m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lHScrollBarHeight);

	m_wndTrackScroll.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT,
		m_lVScrollBarWidth, 
		m_lScaleHeight + m_lKeyHeight - m_lVScrollBarWidth * 2);

	m_wndSizeScroll.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	// �Y�[���{�^���̐���
	m_wndTimeZoomDown.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lTimeWidth - m_lVScrollBarWidth * 2, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	
	m_wndTimeZoomUp.MoveWindow (MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth +
		m_lTimeWidth - m_lVScrollBarWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 1 + m_lScaleHeight + m_lKeyHeight, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndTrackZoomDown.MoveWindow
		(MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight + m_lKeyHeight -
		m_lVScrollBarWidth * 2, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);

	m_wndTrackZoomUp.MoveWindow
		(MUSICALSCOREFRAME_BORDERWIDTH + m_lScaleWidth + m_lTimeWidth, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT + m_lScaleHeight + m_lKeyHeight -
		m_lVScrollBarWidth, 
		m_lVScrollBarWidth, m_lHScrollBarHeight);
	

	// ���X�g�̐���
	m_wndTrackList.MoveWindow
		(MUSICALSCOREFRAME_BORDERWIDTH * 3 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + MUSICALSCOREFRAME_SPLITTERWIDTH, 
		m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT,
		m_lTrackListWidth,
		m_lScaleHeight + m_lKeyHeight + m_lHScrollBarHeight);


	// �X�N���[���o�[�̃T�C�Y���ω������̂ŁA�o�[�̃f�U�C�����Ē�������B
	RecalcTrackScrollInfo ();
	RecalcTimeScrollInfo ();
}

// �N���C�A���g�̈�̐���(CFrameWnd::OnCreateClient�̃I�[�o�[���C�h)
BOOL CMusicalScoreFrame::OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext) {

	// �T�C�Y�����p�̃_�~�[�r���[����(Visible = FALSE)
	CWnd* pWnd = NULL;
	m_pDummyView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_DUMMYVIEW);
	if (m_pDummyView == NULL) {
			return FALSE;
	}
	m_pDummyView->ShowWindow (SW_HIDE);

	// ����p�r���[�̐���(Visible = FALSE)
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScorePrintView);
	m_pPrintView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_PRINTVIEW);
	if (m_pPrintView == NULL) {
			return FALSE;
	}
	m_pPrintView->ShowWindow (SW_HIDE);
	
	// �r���[1�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreScaleView);
	m_pScaleView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_SCALEVIEW);
	if (m_pScaleView == NULL) {
			return FALSE;
	}
	m_pScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// �r���[2�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreTimeScaleView);
	m_pTimeScaleView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_TIMESCALEVIEW);
	if (m_pTimeScaleView == NULL) {
			return FALSE;
	}
	m_pTimeScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	// �r���[3�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreTrackScaleView);
	m_pTrackScaleView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_TRACKSCALEVIEW);
	if (m_pTrackScaleView == NULL) {
			return FALSE;
	}
	m_pTrackScaleView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
	// �r���[4�̐���
	pContext->m_pNewViewClass = RUNTIME_CLASS (CMusicalScoreTrackTimeView);
	m_pTrackTimeView = (CView*)CFrameWnd::CreateView (pContext, MUSICALSCOREFRAME_TRACKTIMEVIEW);
	if (m_pTrackTimeView == NULL) {
			return FALSE;
	}
	m_pTrackTimeView->ModifyStyleEx (WS_EX_CLIENTEDGE, 0);

	// �X�N���[���o�[�̐���
	m_wndTimeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_HORZ, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TIMESCROLL);
	m_wndTrackScroll.Create    
		(WS_CHILD|WS_VISIBLE|SBS_VERT, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKSCROLL);
	m_wndSizeScroll.Create   
		(WS_CHILD|WS_VISIBLE|SBS_SIZEBOX, CRect(0,0,0,0), this, MUSICALSCOREFRAME_SIZEBOX);
	
	// �Y�[���{�^���ނ̐���
	m_wndTimeZoomDown.Create 
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TIMEZOOMDOWN);
	m_wndTimeZoomUp.Create   
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TIMEZOOMUP);
	m_wndTrackZoomDown.Create  
		(_T("-"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKZOOMDOWN);
	m_wndTrackZoomUp.Create    
		(_T("+"), WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKZOOMUP);

	// �g���b�N���X�g�̍쐬
	m_wndTrackList.Create
		(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
		LBS_NOTIFY|LBS_DISABLENOSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT, 
		CRect(0,0,0,0), this, MUSICALSCOREFRAME_TRACKLIST);
	

	// �R���g���[���̈ʒu���킹��WM_SIZE�Ȃǂɂ��RecalcLaoyout�ɔC����B
	
	
	
	
	// �`����z��̍X�V
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	return TRUE;

}

// ����p�̃R�}���h���g���b�v(CFrameWnd::OnCmdMsg�̃I�[�o�[���C�h)
BOOL CMusicalScoreFrame::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// ����p�̃R�}���h�̏ꍇ�A�����I��CMusicalScorePrintView�ɓn���B
	if ((nID == ID_FILE_PRINT || nID == ID_FILE_PRINT_DIRECT || nID == ID_FILE_PRINT_PREVIEW) &&
		pSekaijuApp->m_bRecording == FALSE) {
		if (m_pPrintView) {
			return ((CMusicalScorePrintView*)m_pPrintView)->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
		}
		return FALSE;
	}
	// ���̑��̃R�}���h�̓f�t�H���g�̏����Ƃ���B
	return CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}

//-----------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//-----------------------------------------------------------------------------

// �E�B���h�E������
int CMusicalScoreFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {

	CRect rcTemp;

	// �c�[���o�[1�̍쐬
	if (!m_wndToolBar1.Create (this) ||
		!m_wndToolBar1.LoadToolBar (IDR_MUSICALSCORE1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.SetBarStyle (m_wndToolBar1.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	//m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	//EnableDocking (CBRS_ALIGN_ANY);
	//DockControlBar (&m_wndToolBar1);

	LoadAccelTable (MAKEINTRESOURCE (IDR_MUSICALSCORE));

	// �g���b�N�R���{�̍쐬
	m_wndToolBar1.SetButtonInfo (5, IDC_TRACKCOMBO, TBBS_SEPARATOR, 80);
	m_wndToolBar1.GetItemRect (5, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndTrackCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_TRACKCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndTrackCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// �`�����l���R���{�̍쐬
	m_wndToolBar1.SetButtonInfo (7, IDC_CHANNELCOMBO, TBBS_SEPARATOR, 40);
	m_wndToolBar1.GetItemRect (7, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndChannelCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_CHANNELCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndChannelCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �X�i�b�v�R���{�̍쐬
	m_wndToolBar1.SetButtonInfo (9, IDC_SNAPCOMBO, TBBS_SEPARATOR, 100);
	m_wndToolBar1.GetItemRect (9, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndSnapCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_SNAPCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndSnapCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �x���V�e�B�R���{�̍쐬
	m_wndToolBar1.SetButtonInfo (11, IDC_VELOCITYCOMBO, TBBS_SEPARATOR, 50);
	m_wndToolBar1.GetItemRect (11, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndVelocityCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWN,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_VELOCITYCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndVelocityCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// �����R���{�̍쐬
	m_wndToolBar1.SetButtonInfo (13, IDC_DURATIONCOMBO, TBBS_SEPARATOR, 50);
	m_wndToolBar1.GetItemRect (13, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndDurationCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWN,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_DURATIONCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndDurationCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	
	// �\�����x�R���{�̍쐬
	m_wndToolBar1.SetButtonInfo (24, IDC_RESOLUTIONCOMBO, TBBS_SEPARATOR, 100);
	m_wndToolBar1.GetItemRect (24, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 120;
	if (!m_wndResolutionCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar1.GetSafeHwnd (), (HMENU)IDC_RESOLUTIONCOMBO)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndResolutionCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));


	// �e�N���X�̊֐��Ăяo��
	int nRet = CChildFrame::OnCreate (lpCreateStruct);

	// �X�N���[���|�W�V�����̈ʒu���킹
	// SetTrackScrollPos (64 * m_lTrackZoom - m_lKeyHeight / 2); 

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;

	// �g���b�N�R���{�{�b�N�X�̏[��
	UpdateTrackCombo ();

	// �`�����l���R���{�̏[��
	int i;
	CString strText;
	for (i = 0; i < 16; i++) {
		strText.Format (_T("%d"), i + 1);
		m_wndChannelCombo.AddString (strText);
	}
	m_wndChannelCombo.SetCurSel (0);

	// �J�����g�`�����l���̎����I��
	if (pSekaijuDoc->m_pTempTrack) {
		long lOutputChannel = MIDITrack_GetOutputChannel (pSekaijuDoc->m_pTempTrack);
		if (0 <= lOutputChannel && lOutputChannel <= 15) {
			SetCurChannel (lOutputChannel);
		}
	}
	
	// �X�i�b�v�R���{�̏[��
	UpdateSnapCombo ();
	m_wndSnapCombo.SetCurSel (3);

	// �x���V�e�B�R���{�̏[��
	UpdateVelocityCombo ();
	m_wndVelocityCombo.SetCurSel (6);

	// �����R���{�̏[��
	UpdateDurationCombo ();
	m_wndDurationCombo.SetCurSel (3);

	// �\�����x�R���{�̏[��
	UpdateResolutionCombo ();
	m_wndResolutionCombo.SetCurSel (4);

	// �g���b�N���X�g�̏[��
	UpdateTrackList ();
	m_wndTrackList.SetCurSel (0);
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

	// �����y�[�W�X�V�̐ݒ�
	if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) {
		m_bAutoPageUpdate = TRUE;
	}

	SetActiveView (m_pTrackTimeView, FALSE);
	m_pTrackTimeView->SetFocus ();

	UpdateMeasureInfoArray ();
	UpdateTrackInfoArray ();

	int x = this->MeasuretoX2 (0) + 1;
	this->SetTimeScrollPos (x);


	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return nRet;
}

// �E�B���h�E�j����
void CMusicalScoreFrame::OnDestroy () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// �g���b�N���z��폜
	VERIFY (DeleteTrackInfoArray ());
	// ���ߏ��z��폜
	VERIFY (DeleteMeasureInfoArray ());

	//KillTimer (0x11);
	CChildFrame::OnDestroy ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �E�B���h�E�T�C�Y�ύX��
void CMusicalScoreFrame::OnSize (UINT nType, int cx, int cy) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// ��{�N���X�̊֐��Ăяo��
	CChildFrame::OnSize (nType, cx, cy);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �^�C�}�[�Ăяo����
void CMusicalScoreFrame::OnTimer (UINT nIDEvent) {
	//if (nIDEvent == 0x11) {
	//	m_pTrackTimeView->SendMessage (WM_TIMER, 11, NULL);
	//	m_pVelTimeView->SendMessage (WM_TIMER, 11, NULL);
	//}
}

// �w�i����(CFrameWnd::OnEraseBkgnd�̃I�[�o�[���C�h)
BOOL CMusicalScoreFrame::OnEraseBkgnd (CDC* pDC) {
	return 0;
}

// �E�B���h�E���A�N�e�B�u�ɂȂ����Ƃ�
void CMusicalScoreFrame::OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
	CChildFrame::OnMDIActivate (bActivate, pActivateWnd, pDeactivateWnd);
}

// �`�悷��Ƃ�
void CMusicalScoreFrame::OnPaint () {
	CPaintDC dc (this);
	CRect rcClient;
	GetClientRect (&rcClient);
	// �����̈�̂��ڂݕ`��
	CRect rcClient1 (rcClient);
	rcClient1.top = m_lToolBar1Height;
	rcClient1.bottom = m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight + m_lHScrollBarHeight;
	rcClient1.right = MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth;
	dc.Draw3dRect (&rcClient1, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient1.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient1, RGB (0, 0, 0), RGB (192, 192, 192));
	// �E�㑤�̈�̂��ڂݕ`��
	CRect rcClient3 (rcClient);
	rcClient3.top = m_lToolBar1Height;
	rcClient3.bottom = m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + 
		m_lScaleHeight + m_lKeyHeight + m_lHScrollBarHeight;
	rcClient3.left = MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + 
		m_lVScrollBarWidth + MUSICALSCOREFRAME_SPLITTERWIDTH;
	dc.Draw3dRect (&rcClient3, RGB (128, 128, 128), RGB (255, 255, 255));
	rcClient3.InflateRect (-1, -1);
	dc.Draw3dRect (&rcClient3, RGB (0, 0, 0), RGB (192, 192, 192));
	// �����E�����̕`��
	CBrush brush;
	brush.CreateSolidBrush (::GetSysColor (COLOR_3DFACE));
	CRect rcClient6 (rcClient);
	rcClient6.left = rcClient1.right + 1;
	rcClient6.right = rcClient3.left - 1;
	dc.FillRect (&rcClient6, &brush);
}

// �L�[�������ꂽ��
void CMusicalScoreFrame::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	switch (nChar) {
	// D(�`��)P(�y��)
	case 'D':
	case 'P':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_PEN, 0);
		}
		break;
	// L(��)
	case 'L':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_LINE, 0);
		}
		break;
	// E(�����S��)
	case 'E':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_ERASER, 0);
		}
		break;
	// S(�I��)
	case 'S':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_SELECT, 0);
		}
		break;
	// B(�X�N���u)
	case 'B':
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_SPEAKER, 0);
		}
		break;
	// 1(�S����)
	case '1' :
	case VK_NUMPAD1:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_WHOLENOTE, 0);
		}
		break;
	// 2(2������)
	case '2' :
	case VK_NUMPAD2:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_HALFNOTE, 0);
		}
		break;
	// 4(4������)
	case '4' :
	case VK_NUMPAD4:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_QUARTERNOTE, 0);
		}
		break;
	// 8(8������)
	case '8' :
	case VK_NUMPAD8:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_QUAVERNOTE, 0);
		}
		break;
	// 16(16������)
	case '6' :
	case VK_NUMPAD6:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_SEMIQUAVERNOTE, 0);
		}
		break;
	// 32(32������)
	case '9' :
	case VK_NUMPAD9:
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_DEMISEMIQUAVERNOTE, 0);
		}
		break;
	// .(�t�_)
	case '.' :
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_DOTTED, 0);
		}
		break;
	// t(3�A)
	case 't' :
		if (GetCapture () == NULL) {
			this->PostMessage (WM_COMMAND, ID_MUSICALSCORE_TRIPLET, 0);
		}
		break;
	}
	return;
}



// �}�E�X���{�^�������ꂽ��  
void CMusicalScoreFrame::OnLButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// �㔼��(�s�A�m���[��)�Ɖ�����(�R���g���[���[)�̋��E������h���b�O����ƁA���E�����㉺�Ɉړ�
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
void CMusicalScoreFrame::OnRButtonDown (UINT nFlags, CPoint point) {
}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CMusicalScoreFrame::OnLButtonUp (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (GetCapture () == this) {
		CSekaijuDoc* pSekaijuDoc = GetDocument ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		CDC* pDC = GetDC ();
		DrawSplitterCaptor (pDC, m_ptMouseMoveOld);
		ReleaseDC (pDC);
		ReleaseCapture ();
		::SetCursor (pSekaijuApp->m_hCursorArrow);
		CPoint ptDelta = point - m_ptMouseDown;
		if (m_bSplitterMovingV) {
			m_lTrackListWidth = MUSICALSCOREFRAME_RANGE (0,
				(m_lTrackListWidth - ptDelta.x), 1600);
		}
		m_bSplitterMovingH = FALSE;
		m_bSplitterMovingV = FALSE;
		RecalcLayout ();
		Invalidate ();
		pSekaijuDoc->m_theCriticalSection.Unlock ();
	}
}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CMusicalScoreFrame::OnRButtonUp (UINT nFlags, CPoint point) {


}

// �}�E�X���������ꂽ�Ƃ�
void CMusicalScoreFrame::OnMouseMove (UINT nFlags, CPoint point) {
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
			m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + m_lScaleHeight + m_lKeyHeight <= point.y &&
			point.y < m_lToolBar1Height + MUSICALSCOREFRAME_BORDERHEIGHT * 2 + m_lScaleHeight + m_lKeyHeight + 
			MUSICALSCOREFRAME_SPLITTERHEIGHT;
		// �J�[�\���������X�v���b�^�[��ɂ��邩
		m_bSplitterMovingV =
			MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + m_lVScrollBarWidth <= point.x &&
			point.x < MUSICALSCOREFRAME_BORDERWIDTH * 2 + m_lScaleWidth + m_lTimeWidth + m_lVScrollBarWidth +
			MUSICALSCOREFRAME_SPLITTERWIDTH;
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

// ���ԕ����Y�[���_�E��(20091220:���[�ʒu�ێ��@�\�ǉ��A�����y�[�W�X�V�����I�t�ǉ�)
void CMusicalScoreFrame::OnTimeZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTimeZoom = m_lTimeZoom;
	long lOldTimePos = m_wndTimeScroll.GetScrollPos ();
	long lNewTimeZoom = CLIP (1, m_lTimeZoom - 1, 16);
	long lNewTimePos = lOldTimePos * lNewTimeZoom / lOldTimeZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lTimeZoom = lNewTimeZoom;
	RecalcTimeScrollInfo ();
	m_wndTimeScroll.SetScrollPos (lNewTimePos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	m_pTimeScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ���ԕ����Y�[���A�b�v(20091220:���[�ʒu�ێ��@�\�ǉ��A�����y�[�W�X�V�����I�t�ǉ�)
void CMusicalScoreFrame::OnTimeZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTimeZoom = m_lTimeZoom;
	long lOldTimePos = m_wndTimeScroll.GetScrollPos ();
	long lNewTimeZoom = CLIP (1, m_lTimeZoom + 1, 16);
	long lNewTimePos = lOldTimePos * lNewTimeZoom / lOldTimeZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_bAutoPageUpdate = FALSE;
	m_lTimeZoom = lNewTimeZoom;
	RecalcTimeScrollInfo ();
	m_wndTimeScroll.SetScrollPos (lNewTimePos);
	m_lTimeScrollPos = m_wndTimeScroll.GetScrollPos ();
	m_pTimeScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	//m_bAutoPageUpdate = TRUE;
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �g���b�N�����Y�[���_�E��(20091220:��[�ʒu�ێ��@�\�ǉ�)
void CMusicalScoreFrame::OnTrackZoomDown () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTrackZoom = m_lTrackZoom;
	long lOldTrackPos = m_wndTrackScroll.GetScrollPos ();
	long lNewTrackZoom = CLIP (1, m_lTrackZoom - 1, 8);
	long lNewTrackPos = lOldTrackPos * lNewTrackZoom / lOldTrackZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lTrackZoom = lNewTrackZoom;
	RecalcTrackScrollInfo ();
	m_wndTrackScroll.SetScrollPos (lNewTrackPos);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �g���b�N�����Y�[���A�b�v(20091220:��[�ʒu�ێ��@�\�ǉ�)
void CMusicalScoreFrame::OnTrackZoomUp () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lOldTrackZoom = m_lTrackZoom;
	long lOldTrackPos = m_wndTrackScroll.GetScrollPos ();
	long lNewTrackZoom = CLIP (1, m_lTrackZoom + 1, 8);
	long lNewTrackPos = lOldTrackPos * lNewTrackZoom / lOldTrackZoom;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lTrackZoom = lNewTrackZoom;
	RecalcTrackScrollInfo ();
	m_wndTrackScroll.SetScrollPos (lNewTrackPos);
	m_lTrackScrollPos = m_wndTrackScroll.GetScrollPos ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// �����X�N���[��
void CMusicalScoreFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndTimeScroll) {
		int nMin = 0;
		int nMax = 0;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lTimeScrollPos;
		switch (nSBCode) {
		case SB_LINELEFT:
			lNewPos = m_lTimeScrollPos - m_lTimeZoom;
			break;
		case SB_LINERIGHT:
			lNewPos = m_lTimeScrollPos + m_lTimeZoom;
			break;
		case SB_PAGELEFT:
			lNewPos = m_lTimeScrollPos - m_lTimeZoom * 4;
			break;
		case SB_PAGERIGHT:
			lNewPos = m_lTimeScrollPos + m_lTimeZoom * 4;
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
		SetTimeScrollPos (MUSICALSCOREFRAME_RANGE (0, lNewPos, 0x7FFFFFFF));
		m_bAutoPageUpdate = FALSE;
		m_pTrackTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �����X�N���[��
void CMusicalScoreFrame::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pScrollBar == &m_wndTrackScroll) {
		int nMin = 0;
		int nMax = 0;
		long ry = 4;
		pScrollBar->GetScrollRange (&nMin, &nMax);
		long lNewPos = m_lTrackScrollPos;
		switch (nSBCode) {
		case SB_LINEUP:
			lNewPos = m_lTrackScrollPos - ry * 2;
			break;
		case SB_LINEDOWN:
			lNewPos = m_lTrackScrollPos + ry * 2;
			break;
		case SB_PAGEUP:
			lNewPos = m_lTrackScrollPos - ry * 10;
			break;
		case SB_PAGEDOWN:
			lNewPos = m_lTrackScrollPos + ry * 10;
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
		SetTrackScrollPos (CLIP (0, lNewPos, 0x7FFFFFFF));
		m_pTrackTimeView->SetFocus ();
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �w�c�[��(&T)�x-�w�y��(&P)�x
void CMusicalScoreFrame::OnMusicalScorePen () {
	m_lCurTool = ID_MUSICALSCORE_PEN;
}

// �w�c�[��(&T)�x-�w�y��(&P)�x
void CMusicalScoreFrame::OnUpdateMusicalScorePenUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_PEN);
}

// �w�c�[��(&T)�x-�w��(&L)�x
void CMusicalScoreFrame::OnMusicalScoreLine () {
	m_lCurTool = ID_MUSICALSCORE_LINE;
}

// �w�c�[��(&T)�x-�w��(&L)�x
void CMusicalScoreFrame::OnUpdateMusicalScoreLineUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_LINE);
}

// �w�c�[��(&T)�x-�w�����S��(&E)�x
void CMusicalScoreFrame::OnMusicalScoreEraser () {
	m_lCurTool = ID_MUSICALSCORE_ERASER;
}

// �w�c�[��(&T)�x-�w�����S��(&E)�x
void CMusicalScoreFrame::OnUpdateMusicalScoreEraserUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_ERASER);
}

// �w�c�[��(&T)�x-�w�I��(&S)�x
void CMusicalScoreFrame::OnMusicalScoreSelect () {
	m_lCurTool = ID_MUSICALSCORE_SELECT;
}

// �w�c�[��(&T)�x-�w�I��(&S)�x
void CMusicalScoreFrame::OnUpdateMusicalScoreSelectUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_SELECT);
}

// �w�c�[��(&T)�x-�w�X�s�[�J(&P)�x
void CMusicalScoreFrame::OnMusicalScoreSpeaker () {
	m_lCurTool = ID_MUSICALSCORE_SPEAKER;
}

// �w�c�[��(&T)�x-�w�X�s�[�J(&P)�x
void CMusicalScoreFrame::OnUpdateMusicalScoreSpeakerUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurTool == ID_MUSICALSCORE_SPEAKER);
}

// �w�c�[��(&T)�x-�w�S�����x
void CMusicalScoreFrame::OnMusicalScoreWholeNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	strText.Format (_T("%d"), lTimeResolution * 4);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w�S�����x
void CMusicalScoreFrame::OnUpdateMusicalScoreWholeNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// �S����
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 4);
}

// �w�c�[��(&T)�x-�w2�������x
void CMusicalScoreFrame::OnMusicalScoreHalfNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 2������
	strText.Format (_T("%d"), lTimeResolution * 2);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w2�������x
void CMusicalScoreFrame::OnUpdateMusicalScoreHalfNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2�������E�t�_2�������E3�A2������
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 2 ||
		lCurDuration == lTimeResolution * 3 ||
		lCurDuration == lTimeResolution * 4 / 3);
}

// �w�c�[��(&T)�x-�w4�������x
void CMusicalScoreFrame::OnMusicalScoreQuarterNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	strText.Format (_T("%d"), lTimeResolution * 1);
	// 4������
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w4�������x
void CMusicalScoreFrame::OnUpdateMusicalScoreQuarterNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 4�������E�t�_4�������E3�A4������
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 1 ||
		lCurDuration == lTimeResolution * 3 / 2 ||
		lCurDuration == lTimeResolution * 2 / 3);
}

// �w�c�[��(&T)�x-�w8�������x
void CMusicalScoreFrame::OnMusicalScoreQuaverNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 8������
	strText.Format (_T("%d"), lTimeResolution / 2);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w8�������x
void CMusicalScoreFrame::OnUpdateMusicalScoreQuaverNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 8�������E�t�_8�������E3�A8������
	pCmdUI->SetCheck (lCurDuration == lTimeResolution / 2 ||
		lCurDuration == lTimeResolution * 3 / 4 ||
		lCurDuration == lTimeResolution * 1 / 3);
}

// �w�c�[��(&T)�x-�w16�������x
void CMusicalScoreFrame::OnMusicalScoreSemiQuaverNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 16������
	strText.Format (_T("%d"), lTimeResolution / 4);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w16�������x
void CMusicalScoreFrame::OnUpdateMusicalScoreSemiQuaverNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration =_ttol (strText);
	// 16�������E�t�_16�������E3�A16������
	pCmdUI->SetCheck (lCurDuration == lTimeResolution / 4 ||
		lCurDuration == lTimeResolution * 3 / 8 ||
		lCurDuration == lTimeResolution * 1 / 6);
}

// �w�c�[��(&T)�x-�w32�������x
void CMusicalScoreFrame::OnMusicalScoreDemiSemiQuaverNote () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	// 32������
	strText.Format (_T("%d"), lTimeResolution / 8);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w32�������x
void CMusicalScoreFrame::OnUpdateMusicalScoreDemiSemiQuaverNoteUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 32�������E�t�_32������
	pCmdUI->SetCheck (lCurDuration == lTimeResolution / 8 ||
		lCurDuration == lTimeResolution * 1 / 12);
}

// �w�c�[��(&T)�x-�w�t�_�x
void CMusicalScoreFrame::OnMusicalScoreDotted () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2���������t�_2������
	if (lCurDuration == lTimeResolution * 2) {
		lCurDuration = lTimeResolution * 3;
	}
	// 4���������t�_4������
	else if (lCurDuration == lTimeResolution * 1) {
		lCurDuration = lTimeResolution * 3 / 2;
	}
	// 8���������t�_8������
	else if (lCurDuration == lTimeResolution / 2) {
		lCurDuration = lTimeResolution * 3 / 4;
	}
	// 16���������t�_16������
	else if (lCurDuration == lTimeResolution / 4) {
		lCurDuration = lTimeResolution * 3 / 8;
	}
	// �t�_2��������2������
	else if (lCurDuration == lTimeResolution * 3) {
		lCurDuration = lTimeResolution * 2;
	}
	// �t�_4��������4������
	else if (lCurDuration == lTimeResolution * 3 / 2) {
		lCurDuration = lTimeResolution * 1;
	}
	// �t�_8��������8������
	else if (lCurDuration == lTimeResolution * 3 / 4) {
		lCurDuration = lTimeResolution / 2;
	}
	// �t�_16��������16������
	else if (lCurDuration == lTimeResolution * 3 / 8) {
		lCurDuration = lTimeResolution / 4;
	}
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w�t�_�x
void CMusicalScoreFrame::OnUpdateMusicalScoreDottedUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2�������E4�������E8�������E16������
	// �t�_2�������E�t�_4�������E�t�_8�������E�t�_16������
	pCmdUI->Enable (lCurDuration == lTimeResolution * 2 ||
		lCurDuration == lTimeResolution * 3 ||
		lCurDuration == lTimeResolution * 1 ||
		lCurDuration == lTimeResolution * 3 / 2 ||
		lCurDuration == lTimeResolution / 2 || 
		lCurDuration == lTimeResolution * 3 / 4 ||
		lCurDuration == lTimeResolution / 4 || 
		lCurDuration == lTimeResolution * 3 / 8);	
	// �t�_2�������E�t�_4�������E�t�_8�������E�t�_16������
	pCmdUI->SetCheck (lCurDuration == lTimeResolution * 3 ||
		lCurDuration == lTimeResolution * 3 / 2 ||
		lCurDuration == lTimeResolution * 3 / 4 ||
		lCurDuration == lTimeResolution * 3 / 8);
}

// �w�c�[��(&T)�x-�w3�A�x
void CMusicalScoreFrame::OnMusicalScoreTriplet () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2��������3�A2������
	if (lCurDuration == lTimeResolution * 2) { 
		lCurDuration = lTimeResolution * 4 / 3;
	}
	// 4��������3�A4������
	else if (lCurDuration == lTimeResolution * 1) {
		lCurDuration = lTimeResolution * 2 / 3;
	}
	// 8��������3�A8������
	else if (lCurDuration == lTimeResolution / 2) {
		lCurDuration = lTimeResolution / 3;
	}
	// 16��������3�A16������
	else if (lCurDuration == lTimeResolution / 4) {
		lCurDuration = lTimeResolution / 6;
	}
	// 32��������3�A32������
	else if (lCurDuration == lTimeResolution / 8) {
		lCurDuration = lTimeResolution / 12;
	}
	// 3�A2��������2������
	else if (lCurDuration == lTimeResolution * 4 / 3) {
		lCurDuration = lTimeResolution * 2;
	}
	// 3�A4��������4������
	else if (lCurDuration == lTimeResolution * 2 / 3) {
		lCurDuration = lTimeResolution * 1;
	}
	// 3�A8��������8������
	else if (lCurDuration == lTimeResolution / 3) {
		lCurDuration = lTimeResolution / 2;
	}
	// 3�A16��������16������
	else if (lCurDuration == lTimeResolution / 6) {
		lCurDuration = lTimeResolution / 4;
	}
	// 3�A32��������32������
	else if (lCurDuration == lTimeResolution / 12) {
		lCurDuration = lTimeResolution / 8;
	}
	strText.Format (_T("%d"), lCurDuration);
	m_wndDurationCombo.SetWindowText (strText);
}

// �w�c�[��(&T)�x-�w3�A�x
void CMusicalScoreFrame::OnUpdateMusicalScoreTripletUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CString strText;
	m_wndDurationCombo.GetWindowText (strText);
	long lCurDuration = _ttol (strText);
	// 2�������E4�������E8�������E16�������E32������
	// 3�A2�������E3�A4�������E3�A8�������E3�A16�������E3�A32������
	pCmdUI->Enable (lCurDuration == lTimeResolution * 2 ||
		lCurDuration == lTimeResolution * 4 / 3 ||
		lCurDuration == lTimeResolution * 1 ||
		lCurDuration == lTimeResolution * 2 / 3 ||
		lCurDuration == lTimeResolution / 2 ||
		lCurDuration == lTimeResolution / 3 || 
		lCurDuration == lTimeResolution / 4 ||
		lCurDuration == lTimeResolution / 6 || 
		lCurDuration == lTimeResolution / 8 ||
		lCurDuration == lTimeResolution / 12);	
	// 3�A2�������E3�A4�������E3�A8�������E3�A16�������E3�A32������
	pCmdUI->SetCheck 
		(lCurDuration == lTimeResolution * 4 / 3 || 
		lCurDuration == lTimeResolution * 2 / 3 ||
		lCurDuration == lTimeResolution / 3 ||
		lCurDuration == lTimeResolution / 6 ||
		lCurDuration == lTimeResolution / 12);
}

// �w�c�[��(&T)�x-�w���݂̃g���b�N�̂ݕ\��(&C)�x
void CMusicalScoreFrame::OnMusicalScoreOnlyCurTrack () {
	long lTrackCount = m_wndTrackList.GetCount ();
	long lTrackCurSel = m_wndTrackList.GetCurSel ();
	if (m_bOnlyCurTrack) {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = FALSE;
	}
	else {
		m_bOnlyCurTrack = TRUE;
		m_bShowAllTrack = FALSE;
	}
	for (long i = 0; i < lTrackCount; i++) {
		m_wndTrackList.SetCheck (i, IsTrackVisible (i) ? 1 : 0);
	}
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// �w�c�[��(&T)�x-�w���݂̃g���b�N�̂ݕ\��(&C)�x
void CMusicalScoreFrame::OnUpdateMusicalScoreOnlyCurTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bOnlyCurTrack);
}

// �w�c�[��(&T)�x-�w�S�Ẵg���b�N��\��(&A)�x
void CMusicalScoreFrame::OnMusicalScoreShowAllTrack () {
	long lTrackCount = m_wndTrackList.GetCount ();
	long lTrackCurSel = m_wndTrackList.GetCurSel ();
	if (m_bShowAllTrack) {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = FALSE;
	}
	else {
		m_bOnlyCurTrack = FALSE;
		m_bShowAllTrack = TRUE;
	}
	for (long i = 0; i < lTrackCount; i++) {
		m_wndTrackList.SetCheck (i, IsTrackVisible (i) ? 1 : 0);
	}
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();

}	

// �w�c�[��(&T)�x-�w�S�Ẵg���b�N��\��(&A)�x
void CMusicalScoreFrame::OnUpdateMusicalScoreShowAllTrackUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bShowAllTrack);
}


// �w�c�[��(&T)�x-�w�����y�[�W�X�V�x
void CMusicalScoreFrame::OnMusicalScoreAutoPageUpdate () {
	m_bAutoPageUpdate = !m_bAutoPageUpdate;
}

// �w�c�[��(&T)�x-�w�����y�[�W�X�V�x
void CMusicalScoreFrame::OnUpdateMusicalScoreAutoPageUpdateUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoPageUpdate);
}


// �g���b�N�R���{���I������I�������
void CMusicalScoreFrame::OnTrackComboSelEndOK () {
	// �J�����g�g���b�N���X�V����
	long lCurTrackIndex = m_wndTrackCombo.GetCurSel ();
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
	// �ĕ`��
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// �g���b�N���X�g�̃`�F�b�N�{�b�N�X���ω������Ƃ�
void CMusicalScoreFrame::OnTrackListChkChange () {
	// �g���b�N�̕\���^��\�����`�F�b�N�{�b�N�X�̏�Ԃɍ��킹��
	long lCount = m_wndTrackList.GetCount ();
	long lCurSel = m_wndTrackList.GetCurSel ();
	if (m_bOnlyCurTrack) {
		m_bOnlyCurTrack = FALSE;
	}
	for (long i = 0; i < lCount; i++) {
		m_bTrackVisible[i] = m_wndTrackList.GetCheck (i);
		if (m_bTrackVisible[i] == FALSE) { // TODO
			m_bShowAllTrack = FALSE;
		}
	}
	VERIFY (UpdateMeasureInfoArray ());
	VERIFY (UpdateTrackInfoArray ());
	RecalcTrackScrollInfo ();
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// �g���b�N���X�g�̑I�����ڂ��ω������Ƃ�
void CMusicalScoreFrame::OnTrackListSelChange () {
	// �J�����g�g���b�N��ύX����
	long lCurTrackIndex = m_wndTrackList.GetCurSel ();
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
	VERIFY (UpdateMeasureInfoArray ()); // 20110104�ǉ�
	VERIFY (UpdateTrackInfoArray ()); // 20110104�ǉ�
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// �X�i�b�v�R���{���I������I�������(20111017�ǉ�)
void CMusicalScoreFrame::OnSnapComboSelEndOK () {
	long lSnapComboIndex = m_wndSnapCombo.GetCurSel ();
	long lResolutionComboIndex = m_wndResolutionCombo.GetCurSel ();
	if (0 <= lSnapComboIndex && lSnapComboIndex <= 6 && lResolutionComboIndex < lSnapComboIndex) {
		m_wndResolutionCombo.SetCurSel (lSnapComboIndex);
		this->PostMessage (WM_COMMAND, (WPARAM)((CBN_SELENDOK << 16) | IDC_RESOLUTIONCOMBO),
			(LPARAM)(m_wndResolutionCombo.GetSafeHwnd ()));
	}
}

// �\�����x�R���{���I������I�������
void CMusicalScoreFrame::OnResolutionComboSelEndOK () {
	// �S�g���b�N�̉������Ɖ����O���[�v�����X�V����
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	UpdateTrackInfoArray ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}


// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��ON�x
void CMusicalScoreFrame::OnPopupTrackVisibleOn () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
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
	if (pSekaijuDoc->m_pTempTrack) { // 20110104�ǉ�
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
	}
	VERIFY (UpdateMeasureInfoArray ()); // 20110104�ǉ�
	VERIFY (UpdateTrackInfoArray ()); // 20110104�ǉ�
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��ON�x
void CMusicalScoreFrame::OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pSekaijuDoc->m_pTempTrack == NULL) {
		pCmdUI->Enable (FALSE);	
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��OFF�x
void CMusicalScoreFrame::OnPopupTrackVisibleOff () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
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
	if (pSekaijuDoc->m_pTempTrack) { // 20110104�ǉ�
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
	}
	VERIFY (UpdateMeasureInfoArray ()); // 20110104�ǉ�
	VERIFY (UpdateTrackInfoArray ()); // 20110104�ǉ�
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ\��OFF�x
void CMusicalScoreFrame::OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	if (pSekaijuDoc->m_pTempTrack == NULL) {
		pCmdUI->Enable (FALSE);	
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N�\��ON�x
void CMusicalScoreFrame::OnPopupTrackVisibleAll () {
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
	// �J�����g�g���b�N�ƃJ�����g�`�����l�����X�V
	if (pSekaijuDoc->m_pTempTrack) { // 20110104�ǉ�
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
	}
	VERIFY (UpdateMeasureInfoArray ()); // 20110104�ǉ�
	VERIFY (UpdateTrackInfoArray ()); // 20110104�ǉ�
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	// �ĕ`��
	m_pTrackScaleView->Invalidate ();
	m_pTrackTimeView->Invalidate ();

}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N�\��ON�x
void CMusicalScoreFrame::OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
}


// �w�|�b�v�A�b�v�x-�w���̃C�x���g�̃v���p�e�B(&R)�x // 20110109�C��
void CMusicalScoreFrame::OnPopupEventProperty () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	if (pSekaijuDoc->m_pTempEvent == NULL || pSekaijuDoc->m_pTempTrack == NULL) {
		return;
	}
	if (!MIDIEvent_IsNote (pSekaijuDoc->m_pTempEvent)) {
		return;
	}
	MIDIEvent* pNoteOffEvent = pSekaijuDoc->m_pTempEvent->m_pNextCombinedEvent;
	if (pNoteOffEvent == NULL) {
		return;
	}
	long lTrackIndex = pSekaijuDoc->GetTrackIndex (pSekaijuDoc->m_pTempTrack);
	long lTime = MIDIEvent_GetTime (pSekaijuDoc->m_pTempEvent);
	CString strTime;
	pSekaijuDoc->LongTimeToStringTime (pSekaijuDoc->m_pMIDIData, lTime, &strTime);
	CPropertyNoteDlg theDlg;
	theDlg.m_bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	theDlg.m_bNoteOnNoteOn0 = (MIDIEvent_GetKind (pNoteOffEvent) & 0xF0) == 0x80 ? FALSE : TRUE;
	MIDITrack* pMIDITrack;
	forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
		TCHAR szTrackName[256];
		memset (szTrackName, 0, sizeof (szTrackName));
		MIDITrack_GetName (pMIDITrack, szTrackName, TSIZEOF (szTrackName));
		theDlg.m_theTrackNameArray.Add (szTrackName);
		long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
		theDlg.m_theTrackOutputChannelArray.Add ((DWORD)lTrackOutputChannel);
	}
	long lKey = 0;
	for (lKey = 0; lKey < 128; lKey++) {
		CString strKeyName;
		strKeyName = pSekaijuDoc->GetKeyName (pSekaijuDoc->m_pTempTrack, lTime, lKey);
		theDlg.m_theKeyNameArray.Add (strKeyName);
	}
	theDlg.m_nTrackIndex = (int)lTrackIndex;
	theDlg.m_strTime = strTime;
	theDlg.m_nChannel = (int)MIDIEvent_GetChannel (pSekaijuDoc->m_pTempEvent) + 1;
	theDlg.m_nKey = (int)MIDIEvent_GetKey (pSekaijuDoc->m_pTempEvent);
	theDlg.m_nOnVelocity = (int)MIDIEvent_GetVelocity (pSekaijuDoc->m_pTempEvent);
	theDlg.m_nOffVelocity = (int)MIDIEvent_GetVelocity (pNoteOffEvent);
	theDlg.m_nDuration = (int)MIDIEvent_GetDuration (pSekaijuDoc->m_pTempEvent);
	if (theDlg.DoModal () == IDOK) {
		// �g���b�N���o
		long lTrackIndex = (long)theDlg.m_nTrackIndex;
		if (MIDIData_GetFormat (pSekaijuDoc->m_pMIDIData) == 1 && lTrackIndex == 0) {
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_NOTEEVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		MIDITrack* pNewTrack = pSekaijuDoc->GetTrack (lTrackIndex);
		if (pNewTrack == NULL) {
			CString strMsg;
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		// �^�C���������long�^�ɕϊ�
		long lTime = MIDIEvent_GetTime (pSekaijuDoc->m_pTempEvent);
		long lErrorID = pSekaijuDoc->StringTimeToLongTime (pSekaijuDoc->m_pMIDIData, theDlg.m_strTime, &lTime);
		if (lErrorID != 0) {
			CString strMsg;
			VERIFY (strMsg.LoadString (lErrorID));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return;
		}
		BeginWaitCursor ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		MIDIEvent* pNewEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		MIDIEvent* pLastEvent = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_NOTE));
		pSekaijuDoc->AddHistoryUnit (strHistoryName);
		CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		// �����g���b�N���ύX����Ȃ��ꍇ
		if (pNewTrack == pSekaijuDoc->m_pTempTrack) {
			// EOT�̗���ێ�(20110109�C��)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
			// �C�x���g�̒u���ƕύX
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pSekaijuDoc->m_pTempEvent);
			VERIFY (pNewEvent = pSekaijuDoc->ReplaceMIDIEvent (pSekaijuDoc->m_pTempEvent));
			MIDIEvent_SetTime (pNewEvent, lTime);
			MIDIEvent_SetChannel (pNewEvent, theDlg.m_nChannel - 1);
			MIDIEvent_SetKey (pNewEvent, theDlg.m_nKey);
			MIDIEvent_SetVelocity (pNewEvent, theDlg.m_nOnVelocity);
			MIDIEvent_SetDuration (pNewEvent, theDlg.m_nDuration);
			if (!theDlg.m_bNoteOnNoteOn0) {
				MIDIEvent* pNewNoteOffEvent = pNewEvent->m_pNextCombinedEvent;
				MIDIEvent_SetVelocity (pNewNoteOffEvent, theDlg.m_nOffVelocity);
			}
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
			// EOT�̗���ێ�(20110109�C��)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}
		// �����g���b�N���ύX�����ꍇ
		else {
			// �C�x���g�̒u���Ə���
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pSekaijuDoc->m_pTempEvent);
			VERIFY (pNewEvent = pSekaijuDoc->ReplaceMIDIEvent (pSekaijuDoc->m_pTempEvent));
			VERIFY (MIDITrack_RemoveEvent (pSekaijuDoc->m_pTempTrack, pNewEvent));
			// EOT�̗���ێ�(20110109�C��)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
			// �C�x���g�̑}���ƕύX
			VERIFY (MIDITrack_InsertEvent (pNewTrack, pNewEvent));
			MIDIEvent_SetTime (pNewEvent, lTime);
			MIDIEvent_SetChannel (pNewEvent, theDlg.m_nChannel - 1);
			MIDIEvent_SetKey (pNewEvent, theDlg.m_nKey);
			MIDIEvent_SetVelocity (pNewEvent, theDlg.m_nOnVelocity);
			MIDIEvent_SetDuration (pNewEvent, theDlg.m_nDuration);
			if (!theDlg.m_bNoteOnNoteOn0) {
				MIDIEvent* pNewNoteOffEvent = pNewEvent->m_pNextCombinedEvent;
				MIDIEvent_SetVelocity (pNewNoteOffEvent, theDlg.m_nOffVelocity);
			}
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent);
			// EOT�̗���ێ�(20110109�C��)
			pLastEvent = MIDITrack_GetLastEvent (pNewTrack);
			if (pLastEvent != NULL) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		SetTrackVisible (theDlg.m_nTrackIndex);
		SetCurTrackIndex (theDlg.m_nTrackIndex);
		SetCurChannel (theDlg.m_nChannel - 1);
		SetCurVelocity (theDlg.m_nOnVelocity);
		SetCurDuration (theDlg.m_nDuration);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		::Sleep (0);
	}
}

// �w�|�b�v�A�b�v�x-�w���̃C�x���g�̃v���p�e�B(&R)�x // 20110104�ǉ�
void CMusicalScoreFrame::OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (pSekaijuDoc->m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (pSekaijuDoc->m_pTempEvent == NULL || pSekaijuDoc->m_pTempTrack == NULL) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (!MIDIEvent_IsNote (pSekaijuDoc->m_pTempEvent)) {
		pCmdUI->Enable (FALSE);
		return;
	}
	MIDIEvent* pNoteOffEvent = pSekaijuDoc->m_pTempEvent->m_pNextCombinedEvent;
	if (pNoteOffEvent == NULL) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

