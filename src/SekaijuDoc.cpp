//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���h�L�������g�N���X
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
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDocTemplate.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "FilePropertyDlg.h"
#include "EditTrackDlg.h"
#include "EditTimeDlg.h"
#include "EditTimeSmpDlg.h"
#include "EditChannelDlg.h"
#include "EditKeyDlg.h"
#include "EditVelocityDlg.h"
#include "EditDurationDlg.h"
#include "EditValueDlg.h"
#include "EditBreakupAndTrillDlg.h"
#include "EditQuantizeDlg.h"
#include "EditBeatScanDlg.h"
#include "EditInsertMeasureDlg.h"
#include "EditRemoveMeasureDlg.h"


#include "HistoryRecord.h"
#include "HistoryUnit.h"

#include "TrackListFrame.h"
#include "PianoRollFrame.h"
#include "EventListFrame.h"
#include "MusicalScoreFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CSekaijuDoc, CDocument)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CSekaijuDoc, CDocument)
	ON_UPDATE_COMMAND_UI (ID_FILE_SAVE, OnUpdateFileSaveUI)
	ON_UPDATE_COMMAND_UI (ID_FILE_SAVE_AS,OnUpdateFileSaveAsUI)
	ON_COMMAND (ID_FILE_PROPERTY, OnFileProperty)
	ON_UPDATE_COMMAND_UI (ID_FILE_PROPERTY,OnUpdateFilePropertyUI)
	ON_COMMAND (ID_EDIT_UNDO, OnEditUndo)

	ON_UPDATE_COMMAND_UI (ID_EDIT_UNDO, OnUpdateEditUndoUI)
	ON_COMMAND (ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI (ID_EDIT_REDO, OnUpdateEditRedoUI)
	ON_COMMAND (ID_EDIT_INITHISTORY, OnEditInitHistory)
	ON_UPDATE_COMMAND_UI (ID_EDIT_INITHISTORY, OnUpdateEditInitHistoryUI)
	ON_COMMAND (ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI (ID_EDIT_CUT, OnUpdateEditCutUI)
	ON_COMMAND (ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI (ID_EDIT_COPY, OnUpdateEditCopyUI)
	ON_COMMAND (ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI (ID_EDIT_PASTE, OnUpdateEditPasteUI)
	ON_COMMAND (ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DELETE, OnUpdateEditDeleteUI)
	ON_COMMAND (ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTALL, OnUpdateEditSelectAllUI)
	ON_COMMAND (ID_EDIT_SELECTNONE,OnEditSelectNone)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTNONE, OnUpdateEditSelectNoneUI)
	ON_COMMAND (ID_EDIT_SELECTBEFORE,OnEditSelectBefore)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTBEFORE, OnUpdateEditSelectBeforeUI)
	ON_COMMAND (ID_EDIT_DESELECTBEFORE,OnEditDeselectBefore)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DESELECTBEFORE, OnUpdateEditDeselectBeforeUI)
	ON_COMMAND (ID_EDIT_SELECTAFTER,OnEditSelectAfter)
	ON_UPDATE_COMMAND_UI (ID_EDIT_SELECTAFTER, OnUpdateEditSelectAfterUI)
	ON_COMMAND (ID_EDIT_DESELECTAFTER,OnEditDeselectAfter)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DESELECTAFTER, OnUpdateEditDeselectAfterUI)

	ON_COMMAND (ID_EDIT_TRACK, OnEditTrack)
	ON_UPDATE_COMMAND_UI (ID_EDIT_TRACK, OnUpdateEditTrackUI)
	ON_COMMAND (ID_EDIT_TIME, OnEditTime)
	ON_UPDATE_COMMAND_UI (ID_EDIT_TIME, OnUpdateEditTimeUI)
	ON_COMMAND (ID_EDIT_CHANNEL, OnEditChannel)
	ON_UPDATE_COMMAND_UI (ID_EDIT_CHANNEL, OnUpdateEditChannelUI)
	ON_COMMAND (ID_EDIT_KEY, OnEditKey)
	ON_UPDATE_COMMAND_UI (ID_EDIT_KEY, OnUpdateEditKeyUI)
	ON_COMMAND (ID_EDIT_VELOCITY, OnEditVelocity)
	ON_UPDATE_COMMAND_UI (ID_EDIT_VELOCITY, OnUpdateEditVelocityUI)
	ON_COMMAND (ID_EDIT_DURATION, OnEditDuration)
	ON_UPDATE_COMMAND_UI (ID_EDIT_DURATION, OnUpdateEditDurationUI)
	ON_COMMAND (ID_EDIT_VALUE, OnEditValue)
	ON_UPDATE_COMMAND_UI (ID_EDIT_VALUE, OnUpdateEditValueUI)
	ON_COMMAND (ID_EDIT_QUANTIZE, OnEditQuantize)
	ON_UPDATE_COMMAND_UI (ID_EDIT_QUANTIZE, OnUpdateEditQuantizeUI)
	ON_COMMAND (ID_EDIT_BREAKUPANDTRILL, OnEditBreakupAndTrill)
	ON_UPDATE_COMMAND_UI (ID_EDIT_BREAKUPANDTRILL, OnUpdateEditBreakupAndTrillUI)
	ON_COMMAND (ID_EDIT_BEATSCAN, OnEditBeatScan)
	ON_UPDATE_COMMAND_UI (ID_EDIT_BEATSCAN, OnUpdateEditBeatScanUI)

	ON_COMMAND (ID_EDIT_INSERTMEASURE, OnEditInsertMeasure) // 20100728�ǉ�
	ON_UPDATE_COMMAND_UI (ID_EDIT_INSERTMEASURE, OnUpdateEditInsertMeasureUI) // 20100728�ǉ�
	ON_COMMAND (ID_EDIT_REMOVEMEASURE, OnEditRemoveMeasure) // 20100728�ǉ�
	ON_UPDATE_COMMAND_UI (ID_EDIT_REMOVEMEASURE, OnUpdateEditRemoveMeasureUI) // 20100728�ǉ�

	ON_COMMAND (ID_VIEW_REDRAW, OnViewRedraw)
	ON_COMMAND (ID_VIEW_TRACKLIST, OnViewTrackList)
	ON_COMMAND (ID_VIEW_PIANOROLL, OnViewPianoRoll)
	ON_COMMAND (ID_VIEW_EVENTLIST, OnViewEventList)
	ON_COMMAND (ID_VIEW_MUSICALSCORE, OnViewMusicalScore) // 20090629�g�p�s��Ԃɐݒ�(��)

	ON_COMMAND (ID_POPUP_SHOWTRACKLIST, OnPopupShowTrackList)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWTRACKLIST, OnUpdatePopupShowTrackListUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_SHOWPIANOROLL, OnPopupShowPianoRoll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWPIANOROLL, OnUpdatePopupShowPianoRollUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_SHOWEVENTLIST, OnPopupShowEventList)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWEVENTLIST, OnUpdatePopupShowEventListUI) // 20100429�ǉ�
	ON_COMMAND (ID_POPUP_SHOWMUSICALSCORE, OnPopupShowMusicalScore) // 20090629�g�p�s��Ԃɐݒ�(��)
	ON_UPDATE_COMMAND_UI (ID_POPUP_SHOWMUSICALSCORE, OnUpdatePopupShowMusicalScoreUI) // 20100429�ǉ� // 20110104�C��
	
	ON_COMMAND (ID_POPUP_TRACKINPUTON, OnPopupTrackInputOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKINPUTON, OnUpdatePopupTrackInputOnUI)
	ON_COMMAND (ID_POPUP_TRACKINPUTOFF, OnPopupTrackInputOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKINPUTOFF, OnUpdatePopupTrackInputOffUI)
	ON_COMMAND (ID_POPUP_TRACKINPUTALL, OnPopupTrackInputAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKINPUTALL, OnUpdatePopupTrackInputAllUI)
	ON_COMMAND (ID_POPUP_TRACKOUTPUTON, OnPopupTrackOutputOn)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKOUTPUTON, OnUpdatePopupTrackOutputOnUI)
	ON_COMMAND (ID_POPUP_TRACKOUTPUTOFF, OnPopupTrackOutputOff)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKOUTPUTOFF, OnUpdatePopupTrackOutputOffUI)
	ON_COMMAND (ID_POPUP_TRACKOUTPUTALL, OnPopupTrackOutputAll)
	ON_UPDATE_COMMAND_UI (ID_POPUP_TRACKOUTPUTALL, OnUpdatePopupTrackOutputAllUI)

	ON_COMMAND (ID_POPUP_CUT, OnPopupCut)
	ON_UPDATE_COMMAND_UI (ID_POPUP_CUT, OnUpdatePopupCutUI)
	ON_COMMAND (ID_POPUP_COPY, OnPopupCopy)
	ON_UPDATE_COMMAND_UI (ID_POPUP_COPY, OnUpdatePopupCopyUI)
	ON_COMMAND (ID_POPUP_PASTE, OnPopupPaste)
	ON_UPDATE_COMMAND_UI (ID_POPUP_PASTE, OnUpdatePopupPasteUI)
	
END_MESSAGE_MAP ()


//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CSekaijuDoc::CSekaijuDoc () {
	m_pMIDIClock = NULL;
	m_pMIDIData = NULL;
	m_lOldTime = 0L;
	m_lNewTime = 0L;
	m_bEditLocked = FALSE;
	m_bSaveLocked = FALSE;
	m_lCurHistoryPosition = 0L;
	m_tmFileOpenTime = 0L;
	m_tmLastAutoSaveTime = 0L;
	m_lTempTime = 0;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
}

// �f�X�g���N�^
CSekaijuDoc::~CSekaijuDoc() {
	// �N���e�B�J���Z�N�V�����̃��b�N
	m_theCriticalSection.Lock ();
	// �����̍폜
	DeleteAllHistoryUnit ();	
	// MIDI�f�[�^�̍폜
	if (m_pMIDIData) {
		MIDIData_Delete (m_pMIDIData);
	}
	m_pMIDIData = NULL;
	// MIDI�N���b�N�̍폜
	if (m_pMIDIClock) {
		MIDIClock_Stop (m_pMIDIClock);
		MIDIClock_Delete (m_pMIDIClock);
	}
	m_pMIDIClock = NULL;
	// �A�v���P�[�V�����ɋ��h�L�������g�����ł������Ƃ�`����B
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->m_pOldDocument = NULL;
	// �N���e�B�J���Z�N�V�����̉���
	m_theCriticalSection.Unlock ();
	// �r���[�̍X�V
	UpdateAllViews (NULL);
}



//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �h�L�������g��V�K�쐬����Ƃ��̃I�[�o�[���C�h
BOOL CSekaijuDoc::OnNewDocument () {
	if (!CDocument::OnNewDocument()) {
		return FALSE;
	}
	// OnNewDocument��FALSE��Ԃ����Ƃ��̔����e�X�g
	// TODO:FALSE��Ԃ����Ƃ��ł�����ɓ��삷��悤�ɂ���B
	// return FALSE;

	BeginWaitCursor ();
	CString strMsg;
	// �N���e�B�J���Z�N�V�����̃��b�N
	m_theCriticalSection.Lock ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// ����MIDI�f�[�^�̍폜
	if (m_pMIDIData != NULL) {
		MIDIData_Delete (m_pMIDIData);
		m_pMIDIData = NULL;
	}
	// ����MIDI�N���b�N�̍폜
	if (m_pMIDIClock != NULL) {
		MIDIClock_Delete (m_pMIDIClock);
		m_pMIDIClock = NULL;
	}
	// �V�KMIDI�f�[�^�̍쐬
	m_pMIDIData = MIDIData_Create (MIDIDATA_FORMAT1, 17, MIDIDATA_TPQNBASE, 120);
	if (m_pMIDIData == NULL) {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		VERIFY (strMsg.LoadString (IDS_MIDIDATA_CREATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}
	// �e�g���b�N�̃f�t�H���g�̑����ݒ�
	long i = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	m_pMIDIData->m_lUser1 = m_lOldTime;
	m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
	m_pMIDIData->m_lUser3 = 0;
	m_pMIDIData->m_lUserFlag = MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		MIDITrack_SetInputOn (pMIDITrack, 1);
		MIDITrack_SetInputPort (pMIDITrack, 0);
		MIDITrack_SetInputChannel (pMIDITrack, i == 0 ? - 1 : ((i + 15) % 16));	
		MIDITrack_SetOutputOn (pMIDITrack, 1);
		MIDITrack_SetOutputPort (pMIDITrack, 0);
		MIDITrack_SetOutputChannel (pMIDITrack, i == 0 ? -1 : ((i + 15) % 16));	
		MIDITrack_SetViewMode (pMIDITrack, (i == 10 ? 1 : 0));
		MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
		MIDITrack_SetBackColor (pMIDITrack, 0x00FFFFFF);
		pMIDITrack->m_lUser1 = 0;
		pMIDITrack->m_lUser2 = 0;
		pMIDITrack->m_lUser3 = 0;
		pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
		this->AddDefaultEventToTrack (pMIDITrack, i == 0 ? 0x000B : 0x0007, NULL);
		i++;
	}

	// �t���O�̏�����
	m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
	m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
	m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
	m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
		}
	}
	
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	// MIDI�N���b�N�̐���
	m_pMIDIClock = MIDIClock_Create (MIDICLOCK_MASTERTPQNBASE, 120, 60000000 / 120);
	if (m_pMIDIClock == NULL) {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		VERIFY (strMsg.LoadString (IDS_MIDICLOCK_CREATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}
	// MIDI�N���b�N�̃^�C�����[�h�Ƒ��x���A�v���P�[�V�����ɍ��킹��(20090625�ǉ�)
	ApplyAppCurSpeedIndex ();

	// ��Ԃ̏�����
	m_lOldTime = 0L;
	m_lNewTime = 0L;
	time (&m_tmFileOpenTime);
	time (&m_tmLastAutoSaveTime);
	m_lCurHistoryPosition = -1;
	// �N���e�B�J���Z�N�V�����̉���
	m_theCriticalSection.Unlock ();
	// �r���[�̍X�V
	UpdateAllViews (NULL, 0xFFFFFFFF);
	EndWaitCursor ();
	::Sleep (0);
	return TRUE;
}

// �h�L�������g���J�����̃I�[�o�[���C�h
BOOL CSekaijuDoc::OnOpenDocument (LPCTSTR lpszPathName) {
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	// �N���e�B�J���Z�N�V�����̃��b�N
	m_theCriticalSection.Lock ();
	// ��MIDIData�͂���͂����Ȃ����A��������΍폜�B
	if (m_pMIDIData) {
		MIDIData_Delete (m_pMIDIData);
		m_pMIDIData = NULL;
	}
	// ��MIDIClock�͂���͂����Ȃ����A��������΍폜�B
	if (m_pMIDIClock) {
		MIDIClock_Delete (m_pMIDIClock);
		m_pMIDIClock = NULL;
	}
	// �A�v���P�[�V�����I�u�W�F�N�g�ւ̃|�C���^�擾
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// MIDI�f�[�^�̓ǂݍ���
	CString strPathName = lpszPathName;
	CString strExt = strPathName.Right (4);
	// ���E���V�[�P���X�t�@�C��(*.skj)�̏ꍇ
	if (strExt.CompareNoCase (_T(".skj")) == 0) {
		m_pMIDIData = MIDIData_LoadFromBinary (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// �t���O�̏�����
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// Cherry�V�[�P���X�t�@�C���̏ꍇ(*.chy)�̏ꍇ
	else if (strExt.CompareNoCase (_T(".chy")) == 0) {
		m_pMIDIData = MIDIData_LoadFromCherry (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// �e�g���b�N�̃f�t�H���g�̑����ݒ�
		long i = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
			pMIDITrack->m_lUser1 = 0;
			pMIDITrack->m_lUser2 = 0;
			pMIDITrack->m_lUser3 = 0;
			pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
			i++;
		}
		// �t���O�̏�����
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// �X�^���_�[�hMIDI�t�@�C��(*.mid)�̏ꍇ
	else if (strExt.CompareNoCase (_T(".mid")) == 0 ||
		strExt.CompareNoCase (_T(".vsq")) == 0) {
		m_pMIDIData = MIDIData_LoadFromSMF (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// �t�H�[�}�b�g0��MIDI�f�[�^�̏ꍇ�A�t�H�[�}�b�g1�ɕϊ�����B
		if (MIDIData_GetFormat (m_pMIDIData) == 0) {
			m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_IS_FORMAT0_N_CONVERT_TO_FORMAT1));
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				VERIFY (MIDIData_SetFormat (m_pMIDIData, 1));
			}
		}
		// �e�g���b�N�̃f�t�H���g�̑����ݒ�
		long i = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
			pMIDITrack->m_lUser1 = 0;
			pMIDITrack->m_lUser2 = 0;
			pMIDITrack->m_lUser3 = 0;
			pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
			i++;
		}
		// �m�[�g�C�x���g�̌���
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					MIDIEvent_Combine (pMIDIEvent);
				}
			}
		}
		// �t���O�̏�����
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// MIDICSV�t�@�C��(*.csv)�̏ꍇ
	else if (strExt.CompareNoCase (_T(".csv")) == 0) {
		m_pMIDIData = MIDIData_LoadFromMIDICSV (lpszPathName);
		if (m_pMIDIData == NULL) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_N_FILE_LOAD_FAILED));
			CString strMsg;
			strMsg.Format (strFormat, lpszPathName);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			return FALSE;
		}
		// �t�H�[�}�b�g0��MIDI�f�[�^�̏ꍇ�A�t�H�[�}�b�g1�ɕϊ�����B
		if (MIDIData_GetFormat (m_pMIDIData) == 0) {
			m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_IS_FORMAT0_N_CONVERT_TO_FORMAT1));
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				VERIFY (MIDIData_SetFormat (m_pMIDIData, 1));
			}
		}
		// �e�g���b�N�̃f�t�H���g�̑����ݒ�
		long i = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
			pMIDITrack->m_lUser1 = 0;
			pMIDITrack->m_lUser2 = 0;
			pMIDITrack->m_lUser3 = 0;
			pMIDITrack->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
			i++;
		}
		// �m�[�g�C�x���g�̌���
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					MIDIEvent_Combine (pMIDIEvent);
				}
			}
		}
		// �t���O�̏�����
		m_pMIDIData->m_lUser1 = m_lOldTime;
		m_pMIDIData->m_lUser2 = MIDIData_GetFormat (m_pMIDIData) == 1 ? 1 : 0;
		m_pMIDIData->m_lUser3 = 0;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_VISIBLE | MIDITRACK_ENABLE;
		m_pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASINSERTED;
		m_pMIDIData->m_lUserFlag &= ~MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASINSERTED;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_RESISTEREDASREMOVED;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASINSERTED;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_RESISTEREDASREMOVED;
			}
		}
	}
	// ����`�̃t�H�[�}�b�g�̏ꍇ
	else {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_S_N_INVALID_EXT_N_FILE_LOAD_FAILED));
		CString strMsg;
		strMsg.Format (strFormat, lpszPathName);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	// MIDI�N���b�N�̐���
	m_pMIDIClock = MIDIClock_Create (lTimeMode, lTimeResolution, 60000000 / 120);
	if (m_pMIDIClock == NULL) {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_MIDICLOCK_CREATE_FAILED));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return FALSE;
	}
	// MIDI�N���b�N�̃^�C�����[�h�Ƒ��x���A�v���P�[�V�����ɍ��킹��(20090625�ǉ�)
	ApplyAppCurSpeedIndex ();

	// ��Ԃ̏�����
	m_lOldTime = 0L;
	m_lNewTime = 0L;
	time (&m_tmFileOpenTime);
	time (&m_tmLastAutoSaveTime);
	m_lCurHistoryPosition = -1;

	// �t�H�[�}�b�g1�̏ꍇ��MIDI�f�[�^�̐������`�F�b�N(1)(20100128�ǉ�)
	if (MIDIData_GetFormat (m_pMIDIData) == 1) {
		long lNumErrorEvent = 0;
		CPtrArray theErrorEventArray;
		// �ŏ��̃g���b�N��MIDI�`�����l���C�x���g���������Ă��Ȃ����`�F�b�N
		pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData);
		if (pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
					lNumErrorEvent++;
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						theErrorEventArray.Add (pMIDIEvent);
					}
				}
			}
		}
		// �ŏ��̃g���b�N��MIDI�`�����l���C�x���g���������Ă����ꍇ
		if (lNumErrorEvent > 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_MIDICHANNELEVENTS_ARE_IN_THE_FIRST_TRACK_ARE_YOU_SURE_TO_REPAIR_THEM));
			CString strMsg;
			strMsg.Format (strFormat, lNumErrorEvent);
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			// �ُ��MIDI�`�����l���C�x���g�𑼂̃g���b�N�Ɉړ�
			if (lRet == IDYES) {
				long i = 0;
				long j = 0;
				MIDITrack* pNewTrack[16] = {NULL};
				for (i = 0; i < 16; i++) {
					VERIFY (pNewTrack[i] = MIDITrack_Create ());
				}
				for (j = 0; j < theErrorEventArray.GetSize (); j++) {
					MIDIEvent* pErrorEvent =  (MIDIEvent*)(theErrorEventArray.GetAt (j));
					VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pErrorEvent));
					long lChannel = MIDIEvent_GetChannel (pErrorEvent);
					if (0 <= lChannel && lChannel < 16) {
						VERIFY (MIDITrack_InsertEvent (pNewTrack[lChannel], pErrorEvent));
					}
				}
				MIDITrack* pPrevTrack = MIDIData_GetFirstTrack (m_pMIDIData);
				for (i = 0; i < 16; i++) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pNewTrack[i]);
					if (pLastEvent) {
						if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
							long lLastTime = MIDIEvent_GetTime (pLastEvent);
							VERIFY (MIDITrack_InsertEndofTrack (pNewTrack[i], lLastTime));
						}
						VERIFY (MIDITrack_SetForeColor (pNewTrack[i], pSekaijuApp->m_lDefTrackColor[(i + 1) % 8]));
						VERIFY (MIDITrack_SetInputPort (pNewTrack[i], 0)); // 20120109�ǉ�
						VERIFY (MIDITrack_SetInputChannel (pNewTrack[i], i)); // 20120109�ǉ�
						VERIFY (MIDITrack_SetOutputPort (pNewTrack[i], 0)); // 20120109�ǉ�
						VERIFY (MIDITrack_SetOutputChannel (pNewTrack[i], i)); // 20120109�ǉ�
						pNewTrack[i]->m_lUser1 = 0;
						pNewTrack[i]->m_lUser2 = 0;
						pNewTrack[i]->m_lUser3 = 0;
						pNewTrack[i]->m_lUserFlag = MIDITRACK_VISIBLE | MIDITRACK_ENABLE;
						VERIFY (MIDIData_InsertTrackAfter (m_pMIDIData, pNewTrack[i], pPrevTrack));
						pPrevTrack = pNewTrack[i];
					}
				}
				VERIFY (MIDITrack_SetInputPort (pMIDITrack, 0)); // 20120109�ǉ�
				VERIFY (MIDITrack_SetInputChannel (pMIDITrack, -1)); // 20120109�ǉ�
				VERIFY (MIDITrack_SetOutputPort (pMIDITrack, 0)); // 20120109�ǉ�
				VERIFY (MIDITrack_SetOutputChannel (pMIDITrack, -1)); // 20120109�ǉ�
			}
			// �ُ�Ȃ܂ܓǂݍ��ޏꍇ�A�ҏW�E�ۑ��Ƃ��s�Ƃ���
			else {
				m_bEditLocked = TRUE;
				m_bSaveLocked = TRUE;
				m_theCriticalSection.Unlock ();
				EndWaitCursor ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_CANT_BE_EDITED_OR_SAVED_BECAUSE_IT_IS_ABNORMAL));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				BeginWaitCursor ();
				m_theCriticalSection.Lock ();
			}
		}
	}

	// �t�H�[�}�b�g1�̏ꍇ��MIDI�f�[�^�̐������`�F�b�N(2)(20100128�ǉ�)
	if (MIDIData_GetFormat (m_pMIDIData) == 1) {
		long i = 0;
		long lNumErrorEvent = 0;
		CPtrArray theErrorEventArray;
		// 2�Ԗڈȍ~�̃g���b�N�Ƀe���|�ESMPTE�I�t�Z�b�g�E���q�L���E�����L���C�x���g���������Ă��Ȃ����`�F�b�N
		forEachTrack (m_pMIDIData, pMIDITrack) {
			if (i >= 1) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsTempo (pMIDIEvent) ||
						MIDIEvent_IsSMPTEOffset (pMIDIEvent) ||
						MIDIEvent_IsTimeSignature (pMIDIEvent) ||
						MIDIEvent_IsKeySignature (pMIDIEvent)) {
						lNumErrorEvent++;
						if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
							theErrorEventArray.Add (pMIDIEvent);
						}
					}
				}
			}
			i++;
		}
		// 2�Ԗڈȍ~�̃g���b�N�Ƀe���|�ESMPTE�I�t�Z�b�g�E���q�L���E�����L���C�x���g���������Ă����ꍇ
		if (lNumErrorEvent > 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_TEMPOEVENTS_ARE_IN_THE_SECOND_TRACK_ARE_YOU_SURE_TO_REPAIR_THEM));
			CString strMsg;
			strMsg.Format (strFormat, lNumErrorEvent);
			long lRet = AfxMessageBox (strMsg, MB_ICONQUESTION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			// �ُ��MIDI�`�����l���C�x���g�𑼂̃g���b�N�Ɉړ�
			if (lRet == IDYES) {
				long j = 0;
				MIDITrack* pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData);
				for (j = 0; j < theErrorEventArray.GetSize (); j++) {
					MIDIEvent* pErrorEvent = (MIDIEvent*)(theErrorEventArray.GetAt (j));
					MIDITrack* pErrorTrack = (MIDITrack*)MIDIEvent_GetParent (pErrorEvent);
					VERIFY (MIDITrack_RemoveEvent (pErrorTrack, pErrorEvent));
					VERIFY (MIDITrack_InsertEvent (pMIDITrack, pErrorEvent));
				}
			}
			// �ُ�Ȃ܂ܓǂݍ��ޏꍇ�A�ҏW�E�ۑ��Ƃ��s�Ƃ���
			else {
				m_bEditLocked = TRUE;
				m_bSaveLocked = TRUE;
				m_theCriticalSection.Unlock ();
				EndWaitCursor ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_THIS_MIDIDATA_CANT_BE_EDITED_OR_SAVED_BECAUSE_IT_IS_ABNORMAL));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				BeginWaitCursor ();
				m_theCriticalSection.Lock ();
			}
		}
	}

	// �r���[�̍X�V
	UpdateAllViews (NULL, 0xFFFFFFFF);

	// �Ō�ɊJ�����t�@�C�����̍X�V
	pSekaijuApp->m_strLastOpenFileName[0] = lpszPathName;

	// �������t�J�n
	if (pSekaijuApp->m_theGeneralOption.m_bOpenPlay == TRUE &&
		pSekaijuApp->m_bPlaying == FALSE) {
		AfxGetMainWnd ()->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
	}
	
	// �N���e�B�J���Z�N�V�����̉���
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
	return TRUE;
}

// �h�L�������g��ۑ�����Ƃ��̃I�[�o�[���C�h
BOOL CSekaijuDoc::OnSaveDocument (LPCTSTR lpszPathName) {
	BeginWaitCursor ();
	CString strPathName (lpszPathName);
	CString strExt = strPathName.Right (4);
	CString strMsg1;
	CString strMsg2;
	long lRet = 0;

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theCriticalSection.Lock ();
	// �^�����̏ꍇ�͘^����~ 
	if (pSekaijuApp->m_bRecording) {
		pSekaijuApp->StopRecording (this);
	}

	if (m_bSaveLocked) {
		return FALSE;
	}


	// ���E���V�[�P���X�t�@�C��(*.skj)�̏ꍇ
	if (strExt.CompareNoCase (_T(".skj")) == 0) {
		lRet = MIDIData_SaveAsBinary (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	// Cherry�V�[�P���X�t�@�C��(*.chy)�̏ꍇ
	else if (strExt.CompareNoCase (_T(".chy")) == 0) {
		lRet = MIDIData_SaveAsCherry (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	// �X�^���_�[�hMIDI�t�@�C��(*.mid)�̏ꍇ
	else if (strExt.CompareNoCase (_T(".mid")) == 0 ||
		strExt.CompareNoCase (_T(".vsq")) == 0) {
		// MIDI�C�x���g�̃`�����l���`�F�b�N
		long lCount = 0;
		MIDITrack* pMIDITrack;
		MIDIEvent* pMIDIEvent;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			// �o�͐�g���b�N�ԍ���1�`16�̂����ꂩ�Ɏw�肳��Ă���Ȃ��
			if (0 <= lOutputChannel && lOutputChannel < 16) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
						long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
						if (lOutputChannel != lChannel) {
							lCount++;
						}
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_CHANNEL_IS_CONFLICTED_N_CONVERT_TO_MIDITRACKS_CHANNEL));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				MIDIEvent* pCloneEvent = NULL;
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDICHANNEL));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
					// �o�͐�g���b�N�ԍ���1�`16�̂����ꂩ�Ɏw�肳��Ă���Ȃ��
					if (0 <= lOutputChannel && lOutputChannel < 16) {
						forEachEvent (pMIDITrack, pMIDIEvent) {
							// MIDI�`�����l���C�x���g�őO�����ɔ񌋍��C�x���g�Ȃ��
							if (MIDIEvent_IsMIDIEvent (pMIDIEvent) && 
								pMIDIEvent->m_pPrevCombinedEvent == NULL) {
								// �g���b�N�̏o�̓`�����l����MIDI�`�����l�����قȂ��Ă���Ȃ��
								if (MIDIEvent_GetChannel (pMIDIEvent) != lOutputChannel) {
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
									pCloneEvent = ReplaceMIDIEvent (pMIDIEvent);
									MIDIEvent_SetChannel (pCloneEvent, lOutputChannel);
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_INSERTEVENT, pCloneEvent));
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
				}
			}
		}
		// �m�[�g�C�x���g�̒���>0�ł��邱�Ƃ̃`�F�b�N
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					if (lDuration <= 0) {
						lCount++;
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_DURATION_IS_LESS_THAN_0_N_DELETE_THESE_MIDIEVENTS));
			strMsg2.Format (strMsg1,lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_NOTEEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					CPtrArray theEventArray;
					theEventArray.RemoveAll ();
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsNoteOn (pMIDIEvent) && 
							MIDIEvent_IsNote (pMIDIEvent)) {
							long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
							if (lDuration <= 0) {
								theEventArray.Add (pMIDIEvent);
							}
						}
					}
					long j;
					for (j = 0; j < theEventArray.GetSize (); j++) {
						pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (j);
						VERIFY (pCurHistoryUnit->AddHistoryRecord
							(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent);
					}
				}
			}
		}
		// �g���b�N�̍Ō�ł͂Ȃ��G���h�I�u�g���b�N�C�x���g�̃`�F�b�N
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
					lCount++;
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_ENDOFTRACKEVENT_IS_NOT_LAST_PLACE_N_DELETE_THESE_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
							MIDIEvent* pTempEvent = MIDIEvent_GetPrevEvent (pMIDIEvent);
							VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
							VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
							pMIDIEvent = pTempEvent;
						}
					}
				}
			}
		}
		// �g���b�N�̍Ō�̃G���h�I�u�g���b�N�C�x���g�̕t���Y��`�F�b�N
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
				lCount++;
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDITRACK_DOESNT_HAVE_ENDOFTRACKEVENT_N_INSERT_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			EndWaitCursor ();
			m_theCriticalSection.Unlock ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			m_theCriticalSection.Lock ();
			BeginWaitCursor ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_ADD_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
						long lTime = (pLastEvent ? MIDIEvent_GetTime (pLastEvent) : 0);
						VERIFY (pMIDIEvent = MIDIEvent_CreateEndofTrack (lTime));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent));
					}
				}
			}
		}

		// �ۑ�
		lRet = MIDIData_SaveAsSMF (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}
	}

	// MIDICSV�t�@�C��(*.csv)�̏ꍇ
	else if (strExt.CompareNoCase (_T(".csv")) == 0) {
		// MIDI�C�x���g�̃`�����l���`�F�b�N
		long lCount = 0;
		MIDITrack* pMIDITrack;
		MIDIEvent* pMIDIEvent;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			// �o�͐�g���b�N�ԍ���1�`16�̂����ꂩ�Ɏw�肳��Ă���Ȃ��
			if (0 <= lOutputChannel && lOutputChannel < 16) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
						long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
						if (lOutputChannel != lChannel) {
							lCount++;
						}
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_CHANNEL_IS_CONFLICTED_N_CONVERT_TO_MIDITRACKS_CHANNEL));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				MIDIEvent* pCloneEvent = NULL;
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDICHANNEL));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
					// �o�͐�g���b�N�ԍ���1�`16�̂����ꂩ�Ɏw�肳��Ă���Ȃ��
					if (0 <= lOutputChannel && lOutputChannel < 16) {
						forEachEvent (pMIDITrack, pMIDIEvent) {
							// MIDI�`�����l���C�x���g�őO�����ɔ񌋍��C�x���g�Ȃ��
							if (MIDIEvent_IsMIDIEvent (pMIDIEvent) && 
								pMIDIEvent->m_pPrevCombinedEvent == NULL) {
								// �g���b�N�̏o�̓`�����l����MIDI�`�����l�����قȂ��Ă���Ȃ��
								if (MIDIEvent_GetChannel (pMIDIEvent) != lOutputChannel) {
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
									pCloneEvent = ReplaceMIDIEvent (pMIDIEvent);
									MIDIEvent_SetChannel (pCloneEvent, lOutputChannel);
									VERIFY (pCurHistoryUnit->AddHistoryRecord 
										(HISTORYRECORD_INSERTEVENT, pCloneEvent));
									pMIDIEvent = pCloneEvent;
								}
							}
						}
					}
				}
			}
		}
		// �m�[�g�C�x���g�̒���>0�ł��邱�Ƃ̃`�F�b�N
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsNote (pMIDIEvent)) {
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					if (lDuration <= 0) {
						lCount++;
					}
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDIEVENTS_DURATION_IS_LESS_THAN_0_N_DELETE_THESE_MIDIEVENTS));
			strMsg2.Format (strMsg1,lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_NOTEEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					CPtrArray theEventArray;
					theEventArray.RemoveAll ();
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsNoteOn (pMIDIEvent) && 
							MIDIEvent_IsNote (pMIDIEvent)) {
							long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
							if (lDuration <= 0) {
								theEventArray.Add (pMIDIEvent);
							}
						}
					}
					long j;
					for (j = 0; j < theEventArray.GetSize (); j++) {
						pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (j);
						VERIFY (pCurHistoryUnit->AddHistoryRecord
							(HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent);
					}
				}
			}
		}
		// �g���b�N�̍Ō�ł͂Ȃ��G���h�I�u�g���b�N�C�x���g�̃`�F�b�N
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
					lCount++;
				}
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_ENDOFTRACKEVENT_IS_NOT_LAST_PLACE_N_DELETE_THESE_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_DELETE_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					forEachEvent (pMIDITrack, pMIDIEvent) {
						if (MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent != pLastEvent) {
							MIDIEvent* pTempEvent = MIDIEvent_GetPrevEvent (pMIDIEvent);
							VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
							VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
							pMIDIEvent = pTempEvent;
						}
					}
				}
			}
		}
		// �g���b�N�̍Ō�̃G���h�I�u�g���b�N�C�x���g�̕t���Y��`�F�b�N
		lCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
				lCount++;
			}
		}
		if (lCount > 0) {
			VERIFY (strMsg1.LoadString (IDS_D_MIDITRACK_DOESNT_HAVE_ENDOFTRACKEVENT_N_INSERT_ENDOFTRACKEVENTS));
			strMsg2.Format (strMsg1, lCount);
			EndWaitCursor ();
			m_theCriticalSection.Unlock ();
			lRet = AfxMessageBox (strMsg2, MB_ICONINFORMATION | MB_YESNO);
			m_theCriticalSection.Lock ();
			BeginWaitCursor ();
			if (lRet == IDYES) {
				CHistoryUnit* pCurHistoryUnit = NULL;
				CString strHistoryName;
				VERIFY (strHistoryName.LoadString (IDS_ADD_ENDOFTRACKEVENT));
				VERIFY (this->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
				forEachTrack (m_pMIDIData, pMIDITrack) {
					MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (!MIDIEvent_IsEndofTrack (pLastEvent)) {
						long lTime = (pLastEvent ? MIDIEvent_GetTime (pLastEvent) : 0);
						VERIFY (pMIDIEvent = MIDIEvent_CreateEndofTrack (lTime));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent));
					}
				}
			}
		}

		// �ۑ�
		lRet = MIDIData_SaveAsMIDICSV (m_pMIDIData, lpszPathName);
		if (lRet == 0) {
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			VERIFY (strMsg1.LoadString (IDS_S_N_MIDIDATA_SAVE_FAILED));
			strMsg2.Format (strMsg1, lpszPathName);
			AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
			return FALSE;
		}

	}
	// ����`�̃t�H�[�}�b�g�̏ꍇ
	else {
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		VERIFY (strMsg1.LoadString (IDS_S_N_INVALID_EXT_N_FILE_SAVE_FAILED));
		strMsg2.Format (strMsg1, lpszPathName);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
		return FALSE;
	}
	SetModifiedFlag (0);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	UpdateAllViews (NULL, 
		SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	EndWaitCursor ();
	return TRUE;
}

// �h�L�������g����鎞�̃I�[�o�[���C�h
void CSekaijuDoc::OnCloseDocument () {
	_RPTF0 (_CRT_WARN, ("CSekaijuDoc::OnCloseDocument ()\n"));
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theCriticalSection.Lock ();
	// �^�����̏ꍇ�͘^����~ 
	// ���ӁF�^�����ɓˑR����ꂽ�ꍇ�A���CSekaijuDoc::OnSaveDocument���Ăяo����邱�Ƃ�����B
	// ����āACSekaijuDoc::OnSaveDocument�̖`����StopRecording���Ăяo���Ă������ƁB
	if (pSekaijuApp->m_bRecording) {
		pSekaijuApp->StopRecording (this);
	}
	m_theCriticalSection.Unlock ();
	CDocument::OnCloseDocument ();
	// CDocument::OnCloseDocument ()����delete this����A���̍ۃf�X�g���N�^���Ăяo�����B
}






//------------------------------------------------------------------------------
// CSekaijuDoc �N���X�̐f�f
//------------------------------------------------------------------------------

#ifdef _DEBUG
void CSekaijuDoc::AssertValid () const {
	CDocument::AssertValid ();
}

void CSekaijuDoc::Dump (CDumpContext& dc) const {
	CDocument::Dump (dc);
}
#endif


//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �A�v���P�[�V�����̃X�s�[�h�C���f�b�N�X��K�p����
BOOL CSekaijuDoc::ApplyAppCurSpeedIndex () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// �������[�h�̎擾
	long lSyncInputPort = 0;
	long lSyncInputMode = 0;
	pSekaijuApp->GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
	// �^�C���x�[�X�̎擾
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	// MIDIClock�����삵�Ă���ꍇ�͋�����~
	MIDIClock_Stop (m_pMIDIClock);
	// MIDI�N���b�N�̃^�C�����[�h�Ƒ��x���A�v���P�[�V�����ɍ��킹��(20090625�ǉ�)
	switch (pSekaijuApp->m_lCurSpeedIndex) {
	case 0: // �X�s�[�h=�Î~
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, 0);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 1: // �X�s�[�h=�ᑬ
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, pSekaijuApp->m_theGeneralOption.m_lSpeedSlow);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 2: // �X�s�[�h=�W��
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, pSekaijuApp->m_theGeneralOption.m_lSpeedNormal);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 3: // �X�s�[�h=����
		MIDIClock_SetTimeBase (m_pMIDIClock, lTimeMode, lTimeResolution);
		MIDIClock_SetSpeed (m_pMIDIClock, pSekaijuApp->m_theGeneralOption.m_lSpeedFast);
		MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_MASTER);
		break;
	case 4: // �X�s�[�h=���@��ɃX���[�u
		if (lSyncInputMode == 1) { // MIDI�^�C�~���O�N���b�N�ɃX���[�u
			MIDIClock_SetTimeBase (m_pMIDIClock, MIDICLOCK_SLAVEMIDITIMINGCLOCK, lTimeResolution);
			MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_SLAVEMIDITIMINGCLOCK);
		}
		else if (lSyncInputMode == 2) { // SMPTE/MTC�ɃX���[�u
			MIDIClock_SetTimeBase (m_pMIDIClock, MIDICLOCK_SLAVESMPTEMTC, lTimeResolution);
			MIDIClock_SetMIDIInSyncMode (m_pMIDIClock, MIDICLOCK_SLAVESMPTEMTC);
		}
		else {
			ASSERT (FALSE);
			// TODO:
		}
		MIDIClock_SetSpeed (m_pMIDIClock, 10000);
		break;
	}
	return TRUE;
}



// �w�莞����MIDI�X�e�[�^�X(���)���v�Z���A�����Ŏw�肳�ꂽpMIDIStatus[]�Ɋi�[����B
void CSekaijuDoc::TimeMIDIStatus (long lTargetTime, MIDIStatus* pMIDIStatus[]) {
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	lTargetTime = CLIP (0, lTargetTime, 0x7FFFFFFF);
	// 20120106 �V�X�e���G�N�X�N���[�V���C�x���g�͂ǂ��̃g���b�N�ɑ}������邩����Ȃ̂ŁA
	// MIDI�`�����l���C�x���g�ɐ旧���Čv�Z����悤�ɏC���B
	// �V�X�e���G�N�X�N���[�V���C�x���g�ɂ���Ԃ̌v�Z
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (MIDITrack_GetOutputOn (pMIDITrack)) {
			long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
			long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (lOutputOn) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					lTime = CLIP (0, lTime + MIDITrack_GetTimePlus (pMIDITrack), 0x7FFFFFFF);
					if (lTime > lTargetTime) { // 20111010����
						break;
					}
					// �V�X�e���G�N�X�N���[�V�u�C�x���g�̏ꍇ
					if (MIDIEvent_IsSysExEvent (pMIDIEvent)) {
						BYTE byMsg[256];
						memset (byMsg, 0, sizeof (byMsg));
						long lLen = MIDIEvent_GetLen (pMIDIEvent);
						MIDIEvent_GetData (pMIDIEvent, byMsg, __min (lLen, sizeof (byMsg)));
						// �o�̓|�[�g�����݂���ꍇ�̂�
						if (pMIDIStatus[lOutputPort] != NULL) {
							MIDIStatus_PutMIDIMessage (pMIDIStatus[lOutputPort], byMsg, lLen);
						}
					}
				}
			}
		}
	}
	// MIDI�`�����l���C�x���g�ɂ���Ԃ̌v�Z
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (MIDITrack_GetOutputOn (pMIDITrack)) {
			long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
			long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			if (lOutputOn) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					lTime = CLIP (0, lTime + MIDITrack_GetTimePlus (pMIDITrack), 0x7FFFFFFF);
					if (lTime > lTargetTime) { // 20111010����
						break;
					}
					// MIDI�`�����l���C�x���g�̏ꍇ
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
						BYTE byMsg[256];
						memset (byMsg, 0, sizeof (byMsg));
						long lLen = MIDIEvent_GetLen (pMIDIEvent);
						MIDIEvent_GetData (pMIDIEvent, byMsg, __min (lLen, sizeof (byMsg)));
						// �g���b�N�Őݒ肳�ꂽ�o�̓`�����l���̔��f
						if (0 <= lOutputChannel && lOutputChannel < 16) {
							byMsg[0] &= 0xF0;
							byMsg[0] |= (BYTE)lOutputChannel;
						}
						// �L�[+�ƃx���V�e�B+�̔��f
						if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
							byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
							byMsg[2] = (BYTE)CLIP (1, byMsg[2] + MIDITrack_GetVelocityPlus (pMIDITrack), 127);
						}
						else if (MIDIEvent_IsNoteOff (pMIDIEvent) || MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
							byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
						}
						// �o�̓|�[�g�����݂���ꍇ�̂�
						if (pMIDIStatus[lOutputPort] != NULL) {
							MIDIStatus_PutMIDIMessage (pMIDIStatus[lOutputPort], byMsg, lLen);
						}
					}
				}
			}
		}
	}
}

// �N���b�v�{�[�h�ɓƎ��e�L�X�g��ݒ�
BOOL CSekaijuDoc::SetClipboardTextPrivate9 (CString& strData) {
	TCHAR* p = NULL;
	HGLOBAL hGlobalMem = NULL;
	if ((hGlobalMem = GlobalAlloc (GHND, strData.GetLength() + 32)) == NULL) {
		//_RPTF0 (_CRT_WARN, "��۰�����؊m�ەs�\�B��߰�͎��s���܂����B\n");
		return FALSE;
	}
	if ((p = (TCHAR*)GlobalLock(hGlobalMem)) == NULL) {
		//_RPTF0 (_CRT_WARN, "��۰�����،Œ�s�\�B��߰�͎��s���܂����B\n");
		GlobalFree(hGlobalMem);
		return FALSE;
	}
	_tcscpy (p, (LPCTSTR)strData);
	if (GlobalUnlock(hGlobalMem) == TRUE) {
		//�܂�ۯ�����Ă��TRUE,������ꂽor�װ��FALSE�B
		//_RPTF0 (_CRT_WARN, "��۰�����،Œ�ُ�B��߰�͎��s���܂����B\n");
		GlobalFree(hGlobalMem);
		return FALSE;
	}
	if(::OpenClipboard (AfxGetMainWnd()->m_hWnd) == FALSE) {
		//_RPTF0 (_CRT_WARN, "�د���ް�޵���ݕs�\�B��߰�͎��s���܂����B\n");
		GlobalFree (hGlobalMem);
		return FALSE;
	}
	if (::EmptyClipboard() == FALSE) {
		::CloseClipboard();//��ɕ��Ȃ����޲�۸ޕ\�����ɂ�������B
		//_RPTF0 (_CRT_WARN, "�د���ް�ޏ������s�\�B��߰�͎��s���܂����B\n");
		GlobalFree (hGlobalMem);
		return FALSE;
	}
	if (::SetClipboardData(CF_PRIVATEFIRST + 9, hGlobalMem) == FALSE) {
		::CloseClipboard();
		//_RPTF0 (_CRT_WARN, "�د���ް�ޓ]���s�\�B��߰�͎��s���܂����B\n");
		GlobalFree (hGlobalMem);
	}
	CloseClipboard();
	// hGlobalMem�͂��͂₱����۸��т̂��̂ł͂Ȃ��B
	// �]���āAGlobalFree(hGlobalMem)�������Ȃ��B
	return TRUE;
}

// �N���b�v�{�[�h�̓Ǝ��e�L�X�g���擾
BOOL CSekaijuDoc::GetClipboardTextPrivate9 (CString& strText) {
	if (IsClipboardFormatAvailable (CF_PRIVATEFIRST + 9) == FALSE) {
		//_RPTF0 (_CRT_WARN, "�د���ް�ނ��L���Ȍ`���ł͂���܂���B�\��t���Ɏ��s���܂����B\n");
		return FALSE;
	}
	if (::OpenClipboard (AfxGetMainWnd()->m_hWnd) == FALSE) {
		//_RPTF0 (_CRT_WARN, "�د���ް�ނ��J���܂���B�\��t���Ɏ��s���܂����B\n");
		return FALSE;
	}
	HGLOBAL hData = NULL;
	if ((hData = GetClipboardData(CF_PRIVATEFIRST + 9)) == NULL) {
		CloseClipboard (); // ��ɕ��Ȃ����޲�۸ޕ\�����ɂ�������B
		//_RPTF0 (_CRT_WARN, "�د���ް�ނ���]���s�\�B�\��t���Ɏ��s���܂����B\n");
		return FALSE;
	}
	TCHAR* p = NULL; // �]�����ޯ̧�B
	if ((p = (TCHAR*)GlobalLock(hData)) == NULL) {
		CloseClipboard ();
		//_RPTF0 (_CRT_WARN, "��۰�����،Œ�s�\�B�\��t���Ɏ��s���܂����B\n");
		return FALSE;
	}
	CString strData = p;
	if ((GlobalUnlock (hData)) == TRUE) {
		// �܂�ۯ�����Ă��TRUE,������ꂽor�װ��FALSE�B
		CloseClipboard();
		//_RPTF0 (_CRT_WARN, "��۰�����،Œ�ُ�(�v���I)�B�\��t���Ɏ��s���܂����B\n");
		return FALSE;
	}
	//�d�v�I�v���O���~���OWindows95(�A�X�L�[) P.929�@�Ɉȉ��̂悤�ɋL�q�B
	//�w�د���ް�ނ�۰�ނ���O���ް���ۯ���ۯ����͂����B�x
	if (::CloseClipboard () == FALSE) {
		//_RPTF0 (_CRT_WARN, "�د���ް�ނ�����܂���B�����������𑱂��܂��B\n");
	}
	strText = strData;
	return TRUE;
}

// �N���b�v�{�[�h���Ǝ��e�L�X�g���ǂ������ׂ�
BOOL CSekaijuDoc::IsClipboardTextPrivate9 () {
	if (IsClipboardFormatAvailable (CF_PRIVATEFIRST + 9) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

// �R�s�[�p��������쐬
long CSekaijuDoc::MakeCopyString (CString& strText, long lTempCopyMode, CHistoryUnit* pHistoryUnit) {
	long i, j, k;
	long lFormat;
	long lTimeMode;
	long lTimeResolution;
	long lBeginTime = 0x7FFFFFFF;
	long lEndTime = 0;
	long lBeginTrack = 0x7FFFFFFF;
	long lEndTrack = 0;
	long lCopiedEventCount = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	//m_theCriticalSection.Lock ();
	lFormat = MIDIData_GetFormat (m_pMIDIData);
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	i = 0;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_SELECTED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				pMIDITrack->m_lUserFlag |= MIDITRACK_SELECTED;
				if (i < lBeginTrack) {
					lBeginTrack = i;
				}
				lEndTrack = i;
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime < lBeginTime) {
					lBeginTime = lTime;
				}
				if (lTime > lEndTime) {
					lEndTime = lTime;
				}
			}
		}
		i++;
	}
	if (lBeginTime == 0x7FFFFFFF || lBeginTrack == 0x7FFFFFFF) {
		//m_theCriticalSection.Unlock ();
		//AfxMessageBox ("�C�x���g�������I������Ă��܂���B", MB_ICONEXCLAMATION);
		//m_theCriticalSection.Lock ();
		ASSERT (FALSE);
		return 0;
	}
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (m_pMIDIData, lBeginTime, &lMeasure, &lBeat, &lTick);
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lBeginTime);

	// MIDI�f�[�^����strData�ɐݒ�
	CString strData;
	CString strTextLine;
	CString strTextTemp;
	
	strTextLine.Format (_T("MDa2 %X %X %X %X %X %X %X %X %X %X\n"),
		(m_pMIDIData->m_lFormat == 0 ? 0 : 2),
		(lTempCopyMode ? lEndTrack - lBeginTrack + 1 : lEndTrack + 1),
		m_pMIDIData->m_lTimeBase,
		m_pMIDIData->m_lReserved1,
		m_pMIDIData->m_lReserved2,
		m_pMIDIData->m_lReserved3,
		m_pMIDIData->m_lReserved4,
		m_pMIDIData->m_lUser1,
		m_pMIDIData->m_lUser2,
		m_pMIDIData->m_lUser3,
		m_pMIDIData->m_lUserFlag);
	strData += strTextLine;

	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (pMIDITrack->m_lUserFlag & MIDITRACK_SELECTED) {
			MIDITrack_CountEvent (pMIDITrack);
			strTextLine.Format (_T("MTr2 %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X\n"),
				pMIDITrack->m_lTempIndex - (lTempCopyMode ? lBeginTrack : 0),
				pMIDITrack->m_lNumEvent,
				pMIDITrack->m_lInputOn,
				pMIDITrack->m_lInputPort,
				pMIDITrack->m_lInputChannel,
				pMIDITrack->m_lOutputOn,
				pMIDITrack->m_lOutputPort,
				pMIDITrack->m_lOutputChannel,
				pMIDITrack->m_lViewMode,
				pMIDITrack->m_lForeColor,
				pMIDITrack->m_lBackColor,
				pMIDITrack->m_lTimePlus,
				pMIDITrack->m_lKeyPlus,
				pMIDITrack->m_lVelocityPlus,
				pMIDITrack->m_lReserved1,
				pMIDITrack->m_lReserved2,
				pMIDITrack->m_lReserved3,
				pMIDITrack->m_lReserved4,
				pMIDITrack->m_lUser1,
				pMIDITrack->m_lUser2,
				pMIDITrack->m_lUser3,
				pMIDITrack->m_lUserFlag);
			j = 0;
			strData += strTextLine;

			forEachEvent (pMIDITrack, pMIDIEvent) {
				strTextLine = _T("");
				if (IsEventSelected (pMIDIEvent)) {
					strTextTemp.Format (_T("MEvt %X %X %X %X"),
						pMIDIEvent->m_lTempIndex,
						pMIDIEvent->m_lTime - lBeginTime,
						pMIDIEvent->m_lKind,
						pMIDIEvent->m_lLen);
					strTextLine += strTextTemp;
					for (k = 0; k < pMIDIEvent->m_lLen; k++) {
						strTextTemp.Format (_T(" %X"), *(pMIDIEvent->m_pData + k));
						strTextLine += strTextTemp;
					}
					strTextTemp.Format (_T(" %X %X %X %X %X %X\n"),
						pMIDIEvent->m_pPrevCombinedEvent ? pMIDIEvent->m_pPrevCombinedEvent->m_lTempIndex : -1,
						pMIDIEvent->m_pNextCombinedEvent ? pMIDIEvent->m_pNextCombinedEvent->m_lTempIndex : -1,
						pMIDIEvent->m_lUser1,
						pMIDIEvent->m_lUser2,
						pMIDIEvent->m_lUser3,
						pMIDIEvent->m_lUserFlag);
					strTextLine += strTextTemp;
					strData += strTextLine;
					lCopiedEventCount++;
				}
				j++;
			}
		}
		i++;
	}

	strText = strData;
	return lCopiedEventCount;
}

// �Ǝ��e�L�X�g��W�J���ē\��t��
long CSekaijuDoc::ParsePasteString
(CString& strData, long lBeginTrackIndex, long lBeginTime, CHistoryUnit* pHistoryUnit) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CString strMsg;
	CString strTextLine;
	long lFormat;
	long lNumTrack;
	long lTimeBase;
	long lTimeMode;
	long lTimeResolution;
	long lNumEvent;
	long lTrackIndex;
	long lEventIndex;
	long lTime;
	long lKind;
	long lLen;
	BYTE byData[1024];
	long lPrevCombinedEvent;
	long lNextCombinedEvent;
	long lInputOn;
	long lInputPort;
	long lInputChannel;
	long lOutputOn;
	long lOutputPort;
	long lOutputChannel;
	long lViewMode;
	long lForeColor;
	long lBackColor;
	long lTimePlus;
	long lKeyPlus;
	long lVelocityPlus;
	long lReserved1;
	long lReserved2;
	long lReserved3;
	long lReserved4;
	long lUser1;
	long lUser2;
	long lUser3;
	long lUserFlag;
	TCHAR szType[256];
	long i = 0;
	long j = 0;
	long k = 0;
	long ret = 0;
	long lErrorNo = 0;
	long lDeletedEventCount = 0;
	long lInsertedEventCount = 0;
	MIDITrack* pClipTrack = NULL;
	MIDIEvent* pClipEvent = NULL;
	MIDIData* pClipData = NULL;

	// �N���b�v�{�[�h�̓��e����V����MIDIData�I�u�W�F�N�g���쐬
	while (1) {
		int n = strData.Find(_T('\n'));
		if (n < 0) {
			break;
		}
		strTextLine = strData.Left (n + 1);
		strData = strData.Mid (n + 1);
		// MDa2�s�̏ꍇ
		if (strTextLine.Left (4) == _T("MDa2")) {
			// 2��ȏ�MDat�����ꂽ�ꍇ
			if (pClipData != NULL) {
				break;
			}
			// MDat���̓ǂݎ��
			_stscanf (strTextLine, _T("%s %x %x %x %x %x %x %x %x\n"),
				szType,
				&lFormat, 
				&lNumTrack, 
				&lTimeBase,
				&lReserved1,
				&lReserved2,
				&lReserved3,
				&lReserved4,
				&lUser1, 
				&lUser2, 
				&lUser3, 
				&lUserFlag);
			if (lFormat < 0 || lFormat > 2) {
				VERIFY (strMsg.LoadString (IDS_ABNORMAL_FORMAT_MIDIDATA_IN_THE_CLIPBOARD));
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
				return -1;
			}
			if (lTimeBase & 0x00008000) {
				lTimeMode = 256 - ((lTimeBase & 0x0000FF00) >> 8);
				lTimeResolution = lTimeBase & 0x000000FF;
			}
			else {
				lTimeMode =  MIDIDATA_TPQNBASE;
				lTimeResolution = lTimeBase & 0x00007FFF;
			}
			if (lTimeMode != MIDIDATA_TPQNBASE &&
				lTimeMode != MIDIDATA_SMPTE24BASE &&
				lTimeMode != MIDIDATA_SMPTE25BASE &&
				lTimeMode != MIDIDATA_SMPTE29BASE &&
				lTimeMode != MIDIDATA_SMPTE30BASE) {
				VERIFY (strMsg.LoadString (IDS_TIMEMODE_OF_CLIPBOARD_IS_ABNORMAL));
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
				return -1;
			}
			// MIDIData�̐���
			pClipData = MIDIData_Create 
				(2, lNumTrack, lTimeMode, lTimeResolution);
			if (pClipData == NULL) {
				VERIFY (strMsg.LoadString (IDS_INSUFFICIENT_MEMORY));
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
				return -1;
			}
		}
		// MTr2�s�̏ꍇ
		else if (strTextLine.Left (4) == _T("MTr2")) {
			_stscanf (strTextLine, _T("%s %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n"),
				szType, 
				&lTrackIndex, 
				&lNumEvent,
				&lInputOn,
				&lInputPort,
				&lInputChannel,
				&lOutputOn,
				&lOutputPort,
				&lOutputChannel,
				&lViewMode,
				&lForeColor,
				&lBackColor,
				&lTimePlus,
				&lKeyPlus,
				&lVelocityPlus,
				&lReserved1,
				&lReserved2,
				&lReserved3,
				&lReserved4,
				&lUser1,
				&lUser2,
				&lUser3,
				&lUserFlag);
			// �}���g���b�N��pClipTrack�Ɍ���
			long i = 0;
			forEachTrack (pClipData, pClipTrack) {
				if (i == lTrackIndex) {
					break;
				}
				i++;
			}
		}
		// MEvt�s�̏ꍇ
		else if (strTextLine.Left (4) == _T("MEvt")) {
			if (pClipTrack == NULL) {
				break;
			}
			TCHAR* p = (TCHAR*)(LPCTSTR)strTextLine;
			_stscanf (p, _T("%s %x %x %x %x"),
				szType, &lEventIndex, &lTime, &lKind, &lLen);
			memset (byData, 0, sizeof (byData));
			for (k = 0; k < 5; k++) {
				p = _tcschr (p, _T(' '));
				if (p == NULL) {
					break;
				}
				while (*p == _T(' ')) {
					p++;
				}
			}
			for (k = 0; k < lLen; k++) {
				_stscanf (p, _T("%x"), &byData[k]);
				p = _tcschr (p, ' ');
				if (p == NULL) {
					break;
				}
				while (*p == ' ') {
					p++;
				}
			}

			_stscanf (p, _T("%x %x %x %x %x %d\n"), 
				&lPrevCombinedEvent, &lNextCombinedEvent,
				&lUser1, &lUser2, &lUser3, &lUserFlag);

			// �V����MIDI�C�x���g�̐���
			pClipEvent = MIDIEvent_Create (lTime, lKind, byData, lLen);
			if (pClipEvent == NULL) {
				continue;
			}
			pClipEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
			pClipEvent->m_lTempIndex = lEventIndex;

			// �g���b�N�ɃC�x���g��}��
			ret = MIDITrack_InsertEvent (pClipTrack, pClipEvent);
			if (ret <= 0) {
				MIDIEvent_Delete (pClipEvent);
				continue;
			}
			
			// �����C�x���g�ł���ꍇ�̌�������(1)
			if (lPrevCombinedEvent != -1) {
				MIDIEvent* pTempEvent;
				forEachEvent (pClipTrack, pTempEvent) {
					if (pTempEvent->m_lTempIndex == lPrevCombinedEvent) {
						pClipEvent->m_pPrevCombinedEvent = pTempEvent;
						pTempEvent->m_pNextCombinedEvent = pClipEvent;
						break;
					}
				}
			}

			// �����C�x���g�ł���ꍇ�̌�������(2)
			if (lNextCombinedEvent != -1) {
				MIDIEvent* pTempEvent;
				forEachEvent (pClipTrack, pTempEvent) {
					if (pTempEvent->m_lTempIndex == lNextCombinedEvent) {
						pClipEvent->m_pNextCombinedEvent = pTempEvent;
						pTempEvent->m_pPrevCombinedEvent = pClipEvent;
						break;
					}
				}
			}
		}
	}

	// ���݂�MIDI�f�[�^�̂��ׂĂ�MIDI�C�x���g�I������
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pNewMIDIEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pNewLastEvent = NULL;
	SelectNoObject (pHistoryUnit);

	// ���݂�MIDI�f�[�^�ɃN���b�v�{�[�hMIDI�f�[�^���}�[�W
	//long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	long lNumClipTrack = MIDIData_CountTrack (pClipData);
	
	// �}�[�W�pMIDI�f�[�^�̃t�H�[�}�b�g������MIDI�f�[�^�̃t�H�[�}�b�g�ɍ��킹��B
	if(m_pMIDIData->m_lFormat== 0) {
		MIDIData_SetFormat (pClipData, 0);
	}

	// �}�[�W�pMIDI�f�[�^�̃^�C���x�[�X������MIDI�f�[�^�̃^�C���x�[�X�ɍ��킹��B
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	if (m_pMIDIData->m_lTimeBase != pClipData->m_lTimeBase) {
		MIDIData_SetTimeBase (pClipData, lTimeMode, lTimeResolution);
	}

	// �K�v�Ȑ������g���b�N��ǉ�����
	if (MIDIData_GetFormat (m_pMIDIData) != 0) {
		long lClipTrackCount = MIDIData_CountTrack (pClipData);
		long lNeedTrackCount = lBeginTrackIndex + lClipTrackCount;
		this->AddTrack (lNeedTrackCount, 0x0002, pHistoryUnit);
	}

	// ���ׂẴg���b�N��EOT�C�x���g�̗����L�^
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent);
				pNewLastEvent = this->ReplaceMIDIEvent (pLastEvent);
			}
		}
	}

	// �}�[�W�pMIDI�f�[�^����C�x���g���������A����MIDI�f�[�^�ɑ}��
	if (MIDIData_GetFormat (m_pMIDIData) == 0) {
		pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData);
	}
	else {
		pMIDITrack = GetTrack (lBeginTrackIndex);
	}
	forEachTrack (pClipData, pClipTrack) {
		ASSERT (pMIDITrack);
		pClipEvent = pClipTrack->m_pFirstEvent;
		while (pClipEvent) {
			ret = MIDITrack_RemoveEvent (pClipTrack, pClipEvent);
			ret = MIDIEvent_SetTime (pClipEvent, pClipEvent->m_lTime + lBeginTime);
			ret = MIDITrack_InsertEvent (pMIDITrack, pClipEvent);
			if (ret > 0) {
				VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pClipEvent));
			}
			lInsertedEventCount += ret;
			pClipEvent = pClipTrack->m_pFirstEvent;
		}
		pMIDITrack = pMIDITrack->m_pNextTrack;
	}

	// ���ׂẴg���b�N��EOT�C�x���g�̗����L�^
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent);
			}
		}
	}

	// �N���b�vMIDI�f�[�^�̍폜
	MIDIData_Delete (pClipData);
	pClipData = NULL;
	return lInsertedEventCount;
}

// �I������Ă���C�x���g�̍폜
BOOL CSekaijuDoc::DeleteSelectedEvent (CHistoryUnit* pHistoryUnit) {
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pPrevEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		// �폜����̂Ō������
		pMIDIEvent = MIDITrack_GetLastEvent (pMIDITrack);
		while (pMIDIEvent) {
			pPrevEvent = MIDIEvent_GetPrevEvent (pMIDIEvent);
			// �I������Ă���Ȃ��
			if (this->IsEventSelected (pMIDIEvent)) {
				// �����C�x���g�̓r���łȂ��Ȃ��
				if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
					// �Ō��EndofTrack�C�x���g�łȂ��Ȃ��
					if (!(MIDIEvent_IsEndofTrack (pMIDIEvent) && 
						MIDIEvent_GetNextEvent (pMIDIEvent) == NULL)) {
						pHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent);
						MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent);
					}
				}
			}
			pMIDIEvent = pPrevEvent;
		}
	}
	return TRUE;
}

// MIDI�C�x���g��I�𖔂͔�I����Ԃɂ���B
long CSekaijuDoc::SetEventSelected (MIDIEvent* pMIDIEvent, BOOL bSelected) {
	ASSERT (pMIDIEvent);
	ASSERT (bSelected == 0 || bSelected == 1);
	long lCount = 0;
	MIDIEvent* pTempEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
	while (pTempEvent) {
		if (bSelected) {
			pTempEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
		}
		else {
			pTempEvent->m_lUserFlag &= ~MIDIEVENT_SELECTED;
		}
		pTempEvent = pTempEvent->m_pNextCombinedEvent;
		lCount++;
	}
	return lCount;
}

// MIDI�C�x���g���I����Ԃ����ׂ�
long CSekaijuDoc::IsEventSelected (MIDIEvent* pMIDIEvent) {
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	return pMIDIEvent->m_lUserFlag & MIDIEVENT_SELECTED ? 1 : 0;
}

// ���ׂĂ�MIDI�C�x���g���I����Ԃɂ��A�����ɋL�^����B
long CSekaijuDoc::SelectNoObject (CHistoryUnit* pHistoryUnit) {
	long lCount = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	ASSERT (pHistoryUnit);	

	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				pCloneEvent = this->SelectEvent (pMIDIEvent, 0, pHistoryUnit);
				if (pCloneEvent) {
					pMIDIEvent = pCloneEvent;
					lCount++;
				}
			}
		}
	}
	_RPTF1 (_CRT_WARN, "SelectNoObject returns %d.\n", lCount);
	return lCount;
}

// ���ׂĂ�MIDI�C�x���g��I����Ԃɂ��A�����ɋL�^����B
long CSekaijuDoc::SelectAllObject (CHistoryUnit* pHistoryUnit) {
	return 1; // TODO;
}

// �w���MIDI�C�x���g��I����Ԗ��͔�I����Ԃɂ��A�����ɋL�^����B
// �I����Ԃ��ω������ꍇ�A�V����MIDI�C�x���g��Ԃ��B�ω����Ȃ������ꍇ�ANULL��Ԃ��B
MIDIEvent* CSekaijuDoc::SelectEvent (MIDIEvent* pMIDIEvent, long bSelected,
									 CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	MIDIEvent* pCloneEvent = NULL;
	MIDIEvent* pFirstCombinedEvent = NULL;
	ASSERT (pHistoryUnit);
	// ����ƈقȂ�I����Ԃ��w��������ꍇ�̂�
	if (this->IsEventSelected (pMIDIEvent) != bSelected) {// &&
		//pMIDIEvent->m_pPrevCombinedEvent == NULL) {
		VERIFY (pFirstCombinedEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent));
		VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pFirstCombinedEvent));
		VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pFirstCombinedEvent));
		// �����C�x���g��擪���珇�ɑI����Ԗ��͔�I����Ԃɂ���B
		MIDIEvent* pTempEvent = pCloneEvent;
		while (pTempEvent) {
			if (bSelected) {
				pTempEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
			}
			else {
				pTempEvent->m_lUserFlag &= ~MIDIEVENT_SELECTED;
			}
			pTempEvent = pTempEvent->m_pNextCombinedEvent;
		}
		VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		return pCloneEvent;
	}
	return NULL;
}

// MIDI�g���b�N��I����Ԗ��͔�I����Ԃɂ��A�����ɋL�^����
MIDITrack* CSekaijuDoc::SelectTrack (MIDITrack* pMIDITrack, long bSelected,
									 CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDITrack);
	ASSERT (pHistoryUnit);
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	// �g���b�N���̂��ׂẴC�x���g��I����Ԗ��͔�I����Ԃɂ���B
	forEachEvent (pMIDITrack, pMIDIEvent) {
		pCloneEvent = this->SelectEvent (pMIDIEvent, bSelected, pHistoryUnit);
		if (pCloneEvent) {
			pMIDIEvent = pCloneEvent;
		}
	}
	return NULL;
}

// MIDI�g���b�N���I����Ԃ����ׂ�
long CSekaijuDoc::IsTrackSelected (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	// �g���b�N���̂��ׂẴC�x���g���I������Ă����1�A�����łȂ����0��Ԃ��B
	MIDIEvent* pMIDIEvent = NULL;
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_SELECTED) == 0) {
			return 0;
		}
	}
	return 1;
}


// �w��g���b�N�̎w�菬�߂�I����Ԗ��͔�I����Ԃɂ��A�����ɋL�^����
long CSekaijuDoc::SelectTrackMeasure (MIDITrack* pMIDITrack, long lMeasure, long bSelected,
									  CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDITrack);
	MIDIEvent* pMIDIEvent;
	MIDIEvent* pCloneEvent;
	// �w�菬�߂̊J�n�^�C���ƏI���^�C�����v�Z
	long lMinTime, lMaxTime;
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lMinTime);
	MIDIData_MakeTime (m_pMIDIData, lMeasure + 1, 0, 0, &lMaxTime);
	ASSERT (pHistoryUnit);
	// �w��g���b�N�̎w�菬�ߓ��̂��ׂĂ�MIDI�C�x���g���I����Ԗ��͔�I����Ԃɂ���
	forEachEvent (pMIDITrack, pMIDIEvent) {
		long lTime = MIDIEvent_GetTime (pMIDIEvent);
		if (lMinTime <= lTime && lTime < lMaxTime) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) { // 20081102:�O���߂���̃m�[�g�I�t�͏��O
				pCloneEvent = this->SelectEvent (pMIDIEvent, bSelected, pHistoryUnit);
				if (pCloneEvent) {
					pMIDIEvent = pCloneEvent;
				}
			}
		}
	}
	return 1;
}

// �w��g���b�N�̎w�菬�߂��I����Ԃ����ׂ�(20100517����d�l�C��)
long CSekaijuDoc::IsTrackMeasureSelected (MIDITrack* pMIDITrack, long lMeasure) {
	ASSERT (pMIDITrack);
	MIDIEvent* pMIDIEvent;
	// �w�菬�߂̊J�n�^�C���ƏI���^�C�����v�Z
	long lMinTime, lMaxTime;
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lMinTime);
	MIDIData_MakeTime (m_pMIDIData, lMeasure + 1, 0, 0, &lMaxTime);
	long lEventNum = 0; // 20100517�ǉ�
	long lSelectedEventNum = 0; // 20100517�ǉ�
	forEachEvent (pMIDITrack, pMIDIEvent) {
		long lTime = MIDIEvent_GetTime (pMIDIEvent);
		if (lMinTime <= lTime && lTime < lMaxTime) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) { // 20081102:�O���߂���̃m�[�g�I�t�͏��O
				lEventNum++;
				if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_SELECTED) != 0) {
					lSelectedEventNum++;
				}
			}
		}
	}
	// �C�x���g�����݂��Ȃ��ꍇ��0��Ԃ��B
	if (lEventNum == 0) {
		return 0;
	}
	// �I������Ă���C�x���g�����݂��Ȃ��ꍇ��1��Ԃ��B
	if (lSelectedEventNum == 0) {
		return 1;
	}
	// �ꕔ�̃C�x���g���I������Ă���ꍇ��2��Ԃ��B
	else if (lEventNum > lSelectedEventNum) {
		return 2;
	}
	// ���ׂẴC�x���g���I������Ă���ꍇ��3��Ԃ��B
	else { // if (lEventNum == lSelectedEventNum) {
		return 3;
	}
}


// �w��C���f�b�N�X�̃g���b�N���擾
MIDITrack* CSekaijuDoc::GetTrack (long lIndex) {
	ASSERT (0 <= lIndex && lIndex < MAXMIDITRACKNUM);
	long i = 0;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (i == lIndex) {
			return pMIDITrack;
		}
		i++;
	}
	return NULL;	
}

// �g���b�N�Ƀf�t�H���g�̃C�x���g��ǉ�����B
// lMode�͈ȉ��̒l��OR
// 0001:�g���b�N�Ƀg���b�N���C�x���g��}��
// 0002:�g���b�N��EOT�C�x���g��}��
// 0004:�g���b�N�ɃR���g���[���`�F���W�E�v���O�����`�F���W��}��
// 0008:�g���b�N�Ƀe���|�E���q�L���E�����L����}��(���ׂă^�C��0)
long CSekaijuDoc::AddDefaultEventToTrack (MIDITrack* pMIDITrack, long lMode, CHistoryUnit* pHistoryUnit) {
	ASSERT (pMIDITrack);
	long lCurTrackCount = MIDIData_CountTrack (m_pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	long lChannel = MIDITrack_GetOutputChannel (pMIDITrack);
	MIDIEvent* pNewEvent = NULL;
	// �g���b�N���C�x���g�̑}��
	if (lMode & 0x0001) {
		VERIFY (pNewEvent = MIDIEvent_CreateTrackName (lTimeResolution * 0 / 120, _T("")));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	// �e���|�E���q�L���E�����L��
	if (lMode & 0x0008) {
		VERIFY (pNewEvent = MIDIEvent_CreateTempo (0, 60000000 / 120));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateTimeSignature (0, 4, 2, 24, 8));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateKeySignature (0, 0, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	// �R���g���[���`�F���W�E�v���O�����`�F���W�̑}��
	if (lMode & 0x0004 && 0 <= lChannel && lChannel <= 15) {
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 120 / 120, lChannel, 0, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 130 / 120, lChannel, 32, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateProgramChange (lTimeResolution * 140 / 120, lChannel, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 150 / 120, lChannel, 1, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 160 / 120, lChannel, 7, 100));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 170 / 120, lChannel, 10, 64));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 180 / 120, lChannel, 11, 127));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 190 / 120, lChannel, 64, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 200 / 120, lChannel, 91, 40));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 210 / 120, lChannel, 93, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
		VERIFY (pNewEvent = MIDIEvent_CreateControlChange (lTimeResolution * 220 / 120, lChannel, 94, 0));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	// �G���h�I�u�g���b�N�C�x���g�̑}��
	if (lMode & 0x0002) {
		VERIFY (pNewEvent = MIDIEvent_CreateEndofTrack (lTimeResolution * 4));
		VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
		if (pHistoryUnit) {
			VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
		}
	}
	return 1;
}

// �K�v�Ȑ������g���b�N��ǉ����A�����ɋL�^����B
// lNeedTrackCount�͍ŏI�I�ȃg���b�N�̑����B�ǉ����ꂽ�g���b�N�̐���Ԃ��B
// lMode�͈ȉ��̒l��OR
// 0001:�ǉ����ꂽ�g���b�N�Ƀg���b�N���C�x���g��}��
// 0002:�ǉ����ꂽ�g���b�N��EOT�C�x���g��}��
// 0004:�ǉ����ꂽ�g���b�N�ɃR���g���[���`�F���W�E�v���O�����`�F���W��}��
// 0008:�ǉ����ꂽ�g���b�N�Ƀe���|�E���q�L���E�����L����}��(�^�C��0)
long CSekaijuDoc::AddTrack (long lNeedTrackCount, long lMode, CHistoryUnit* pHistoryUnit) {
	ASSERT (0 <= lNeedTrackCount && lNeedTrackCount < MAXMIDITRACKNUM);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lFormat = MIDIData_GetFormat (m_pMIDIData);
	// �t�H�[�}�b�g1���̓t�H�[�}�b�g2�̏ꍇ�ȊO�̓g���b�N��ǉ��ł��Ȃ��B
	if (!(lFormat == 1 || lFormat == 2)) {
		return 0;
	}
	long i = 0;
	long lCount = 0;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	long lCurTrackCount = MIDIData_CountTrack (m_pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	long lMode2 = lMode;
	for (i = lCurTrackCount; i < lNeedTrackCount; i++) {
		VERIFY (pTempTrack = MIDITrack_Create ());
		pTempTrack->m_lTempIndex = i;
		MIDITrack_SetViewMode (pTempTrack, 0);
		MIDITrack_SetForeColor (pTempTrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
		MIDITrack_SetBackColor (pTempTrack, 0x00FFFFFF);
		MIDITrack_SetInputOn (pTempTrack, 1);
		MIDITrack_SetInputPort (pTempTrack, 0);
		MIDITrack_SetInputChannel (pTempTrack, (i + 15) % 16);
		MIDITrack_SetOutputOn (pTempTrack, 1);
		MIDITrack_SetOutputPort (pTempTrack, 0);
		MIDITrack_SetOutputChannel (pTempTrack, (i + 15) % 16);
		VERIFY (MIDIData_AddTrack (m_pMIDIData, pTempTrack));
		VERIFY (pHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pTempTrack));
		// �t�H�[�}�b�g1�̏ꍇlMode�𒲐�
		if (lFormat == 1 && i == 0) {
			lMode2 = lMode & ~0x0004; // �ŏ��̃g���b�N�ɃR���g���[���`�F���W�E�v���O�����`�F���W���֎~
		}
		else if (lFormat == 1 && i != 0) {
			lMode2 = lMode & ~0x0008; // 2�Ԗڈȍ~�̃g���b�N�Ƀe���|�E���q�L���E�����L�����֎~
		}
		VERIFY (this->AddDefaultEventToTrack (pTempTrack, lMode2, pHistoryUnit));
		lCount++;
	}
	return lCount;
}


// �g���b�N�̖��O�擾
CString CSekaijuDoc::GetTrackName (MIDITrack* pMIDITrack) {
	TCHAR szBuf[1024];
	memset (szBuf, 0, sizeof (szBuf));
	MIDITrack_GetName (pMIDITrack, szBuf, TSIZEOF (szBuf) - 1);
	return CString (szBuf);
}

// �g���b�N�̖��O�ݒ�
BOOL CSekaijuDoc::SetTrackName (MIDITrack* pMIDITrack, CString strTrackName) {
	CHistoryUnit* pCurHistoryUnit = NULL;
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());
	MIDIEvent* pTempEvent = MIDITrack_GetFirstKindEvent (pMIDITrack, MIDIEVENT_TRACKNAME);
	MIDIEvent* pCloneEvent = NULL;
	// ���Ƀg���b�N���C�x���g������ꍇ
	if (pTempEvent) {
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent);
		VERIFY (pCloneEvent = ReplaceMIDIEvent (pTempEvent));
		VERIFY (MIDIEvent_SetText (pCloneEvent, (LPCTSTR)strTrackName));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
	}
	// ���̃g���b�N�Ƀg���b�N���C�x���g���Ȃ��ꍇ
	else {
		VERIFY (pCloneEvent = MIDIEvent_CreateTrackName (0, strTrackName));
		MIDITrack_InsertEvent (pMIDITrack, pCloneEvent);
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
	}
	return MIDITrack_SetName (pMIDITrack, (LPCTSTR)strTrackName);
}


// �g���b�N�̃C���f�b�N�X���擾
long CSekaijuDoc::GetTrackIndex (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	MIDITrack* pTempTrack;
	long i = 0;
	forEachTrack (m_pMIDIData, pTempTrack) {
		if (pTempTrack == pMIDITrack) {
			return i;
		}
		i++;
	}
	ASSERT (FALSE);
	return -1;
}


// �g���b�N�̉����擾
long CSekaijuDoc::GetTrackVisible (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	return pMIDITrack->m_lUserFlag & MIDITRACK_VISIBLE ? 1 : 0;
}

// �g���b�N�̉����ݒ�
BOOL CSekaijuDoc::SetTrackVisible (MIDITrack* pMIDITrack, long lVisible) {
	ASSERT (pMIDITrack);
	ASSERT (0 <= lVisible && lVisible <= 1);
	if (lVisible) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	else {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_VISIBLE;
	}
	return TRUE;
}

// �g���b�N�̃C�l�[�u��ON/OFF�擾
long CSekaijuDoc::GetTrackEnable (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	return pMIDITrack->m_lUserFlag & MIDITRACK_ENABLE ? 1 : 0;
}

// �g���b�N�̃C�l�[�u��ON/OFF�ݒ�
BOOL CSekaijuDoc::SetTrackEnable (MIDITrack* pMIDITrack, long lEnable) {
	ASSERT (pMIDITrack);
	ASSERT (0 <= lEnable && lEnable <= 1);
	if (lEnable) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_ENABLE;
	}
	else {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_ENABLE;
	}
	return TRUE;
}

// �g���b�N�̑I����Ԃ̎擾
long CSekaijuDoc::GetTrackSelected (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	return pMIDITrack->m_lUserFlag & MIDITRACK_SELECTED ? 1 : 0;
}

// �g���b�N�̑I����Ԃ̐ݒ�
BOOL CSekaijuDoc::SetTrackSelected (MIDITrack* pMIDITrack, long lSelected) {
	ASSERT (pMIDITrack);
	ASSERT (0 <= lSelected && lSelected <= 1);
	if (lSelected) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_SELECTED;
	}
	else {
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_SELECTED;
	}
	return TRUE;
}


// �ŏ��̃R���g���[���`�F���W�C�x���g�̎擾
MIDIEvent* CSekaijuDoc::GetTrackFirstControlChange (MIDITrack* pMIDITrack, long lNumber) {
	_ASSERT (pMIDITrack);
	_ASSERT (0 <= lNumber && lNumber < 128);
	MIDIEvent* pMIDIEvent = NULL;
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsControlChange (pMIDIEvent)) {
			if (MIDIEvent_GetNumber (pMIDIEvent) == lNumber) {
				return pMIDIEvent;
			}
		}
	}
	return NULL;
}

// �ŏ��̃v���O�����`�F���W�C�x���g�̎擾
MIDIEvent* CSekaijuDoc::GetTrackFirstProgramChange (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	MIDIEvent* pMIDIEvent = NULL;
	forEachEvent (pMIDITrack, pMIDIEvent) {
		if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
			return pMIDIEvent;
		}
	}
	return NULL;
}

MIDIEvent* CSekaijuDoc::FindBankMSB (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	if (MIDIEvent_IsControlChange (pMIDIEvent)) {
		long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
		if (lNumber == 32) {
			while (pMIDIEvent) { // 20110508�C��
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == 0) {
						return pMIDIEvent;
					}
				}
				pMIDIEvent = MIDIEvent_GetPrevEvent (pMIDIEvent); // 20110508�ǉ�
			}
		}
	}
	else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
		while (pMIDIEvent) { // 20110508�C��
			if (MIDIEvent_IsControlChange (pMIDIEvent)) {
				if (MIDIEvent_GetNumber (pMIDIEvent) == 0) {
					return pMIDIEvent;
				}
			}
			pMIDIEvent = MIDIEvent_GetPrevEvent (pMIDIEvent); // 20110508�ǉ�
		}
	}
	return NULL;
}

MIDIEvent* CSekaijuDoc::FindBankLSB (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	if (MIDIEvent_IsControlChange (pMIDIEvent)) {
		long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
		if (lNumber == 0) {
			while (pMIDIEvent) { // 20110508�C��
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == 32) {
						return pMIDIEvent;
					}
				}
				pMIDIEvent = MIDIEvent_GetNextEvent (pMIDIEvent); // 20110508�ǉ�
			}
		}
	}
	else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
		while (pMIDIEvent) { // 20110508�C��
			if (MIDIEvent_IsControlChange (pMIDIEvent)) {
				if (MIDIEvent_GetNumber (pMIDIEvent) == 32) {
					return pMIDIEvent;
				}
			}
			pMIDIEvent = MIDIEvent_GetPrevEvent (pMIDIEvent); // 20110508�ǉ�
		}
	}
	return NULL;
}

MIDIEvent* CSekaijuDoc::FindProgramChange (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	if (MIDIEvent_IsControlChange (pMIDIEvent)) {
		long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
		if (lNumber == 0 || lNumber == 32) {
			while (pMIDIEvent) { // 20110508�C��
				if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					return pMIDIEvent;
				}
				pMIDIEvent = MIDIEvent_GetNextEvent (pMIDIEvent); // 20110508�ǉ�
			}
		}
	}
	return NULL;
}

// �w��g���b�N��lTime�ɂ�����lKey�̖���(��F"C#6")���擾 // 20090501�ǉ�
CString CSekaijuDoc::GetKeyName (MIDITrack* pMIDITrack, long lTime, long lKey) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
	// ���̃g���b�N�̕\�����[�h�́u�ʏ�v�̏ꍇ
	if (lTrackViewMode == 0) {
		long lsf, lmi;
		long lOctaveSignature = pSekaijuApp->m_theGeneralOption.m_lOctaveSignature;
		MIDIData_FindKeySignature (m_pMIDIData, lTime, &lsf, &lmi);
		long lIndex = (lsf & 0x0000000F) * 16 + (lKey % 12);
		CString strKeyName;
		strKeyName.Format (_T("%s%d"), 
			pSekaijuApp->m_strNoteKeyName[lIndex], 
			lKey / 12 + lOctaveSignature - 5);
		return strKeyName;
	}
	// ���̃g���b�N�̕\�����[�h�́u�h�����v�̏ꍇ
	else {
		long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
		MIDIInstrumentDefinition* pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
		// ���̃g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`����������
		if (pMIDIInstDef) {
			long lTrackCC0, lTrackCC32, lTrackPC;
			MIDIEvent* pCC0Event = GetTrackFirstControlChange (pMIDITrack, 0);
			if (pCC0Event) {
				lTrackCC0 = MIDIEvent_GetValue (pCC0Event);
			}
			else {
				lTrackCC0 = 0;
			}
			MIDIEvent* pCC32Event = GetTrackFirstControlChange (pMIDITrack, 32);
			if (pCC32Event) {
				lTrackCC32 = MIDIEvent_GetValue (pCC32Event);
			}
			else {
				lTrackCC32 = 0;
			}
			MIDIEvent* pPCEvent = GetTrackFirstProgramChange (pMIDITrack);
			if (pPCEvent) {
				lTrackPC = MIDIEvent_GetNumber (pPCEvent);
			}
			else {
				lTrackPC = 0;
			}
			MIDINoteNameTable* pNoteNameTable = 
				MIDIInstrumentDefinition_GetNoteNameTable 
				(pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort], 
				(lTrackCC0 << 7) | (lTrackCC32), lTrackPC);
			// ���̃C���X�g�D�������g��`�͎w��o���N/�v���O������
			// NoteNameTable�������Ă���
			if (pNoteNameTable) {
				TCHAR szKeyName[256];
				memset (szKeyName, 0, sizeof (szKeyName));
				MIDINoteNameTable_GetName 
					(pNoteNameTable, lKey, szKeyName, TSIZEOF (szKeyName) - 1);
				CString strKeyName (szKeyName);
				return strKeyName;
			}
			// ���̃C���X�g�D�������g��`�͎w��o���N/�v���O������
			// NoteNameTable�������Ă��Ȃ�
			else {
				return _T("");
			}
		}
		// ���̃g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`��������Ȃ�����
		else {
			return _T("");
		}
	}
}

// �^�C������:��:�b:�~���b������ɕϊ� // 20100502�ǉ�
BOOL CSekaijuDoc::LongTimeToStringMillisec (MIDIData* pMIDIData, long lTime, CString* pstrText) {
	long lMilliSec, lHour, lMinute, lSec, lMSec;
	lMilliSec = MIDIData_TimeToMillisec (pMIDIData, lTime);
	lHour = lMilliSec / 3600000;
	lMinute = (lMilliSec % 3600000) / 60000;
	lSec = (lMilliSec % 60000) / 1000;
	lMSec = (lMilliSec % 1000);
	pstrText->Format (_T("%02d:%02d:%02d:%03d"), lHour, lMinute, lSec, lMSec);
	return TRUE;
}

// �^�C��������:��:�e�B�b�N ���� �t���[���ԍ�:�e�B�b�N������ɕϊ� // 20100502�ǉ�
BOOL CSekaijuDoc::LongTimeToStringTime (MIDIData* pMIDIData, long lTime, CString* pstrText) {
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	// TPQN�x�[�X�̏ꍇ(�W��)�clong��"����:��:�e�B�b�N"
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
		pstrText->Format (_T("%05d:%02d:%03d"), lMeasure + 1, lBeat + 1, lTick);
		return TRUE;
	}
	// SMPTE24�x�[�X�ASMPTE25�x�[�X�ASMPTE29.97�x�[�X�ASMPTE30�x�[�X�̏ꍇ�clong��"�t���[��:�T�u�t���[��"
	else if (lTimeMode == MIDIDATA_SMPTE24BASE || lTimeMode == MIDIDATA_SMPTE25BASE ||
		lTimeMode == MIDIDATA_SMPTE29BASE || lTimeMode == MIDIDATA_SMPTE30BASE) {
		long lFrameNumber, lTick;
		lFrameNumber = lTime / lTimeResolution;
		lTick = lTime % lTimeResolution;
		pstrText->Format (_T("%08d:%03d"), lFrameNumber, lTick);
		return TRUE;
	}
	return FALSE;
}

// ����:��:�e�B�b�N ���� �t���[���ԍ�:�e�B�b�N��������^�C���ɕϊ� // 20100502�ǉ�
// �߂�l�F����=0, �ُ�=�G���[���b�Z�[�W�����񃊃\�[�XID(1�`)
long CSekaijuDoc::StringTimeToLongTime (MIDIData* pMIDIData, CString strText, long* pTime) {
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lTemp = 0;
	CString strToken;
	// TPQN�x�[�X�̏ꍇ(�W��)�c"����:��:�e�B�b�N"��long
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lMeasure, lBeat, lTick;
		// ���߂Ɣ��̊Ԃ̃Z�p���[�^�`�F�b�N
		lTemp = strText.Find (_T(":"));
		if (lTemp < 0) {
			return IDS_A_SEPARATOR_BETWEEN_MEASURE_AND_BEAT_IS_NOT_FOUND;
		}
		// ���ߕ����`�F�b�N
		strToken = strText.Left (lTemp);
		if (IsNumeric (strToken) <= 0) {
			return IDS_MEASURE_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// ���ߒl�`�F�b�N
		lMeasure = _ttol (strToken) - 1;
		if (lMeasure < 0 || lMeasure >= 65535) {
			return IDS_MEASURE_VALUE_IS_OUT_OF_RANGE;
		}
		// ���ƃe�B�b�N�̊Ԃ̃Z�p���[�^�`�F�b�N
		strText = strText.Mid (lTemp + 1);
		lTemp = strText.Find (_T(":"));
		if (lTemp < 0) {
			return IDS_A_SEPARATOR_BETWEEN_BEAT_AND_TICK_IS_NOT_FOUND;
		}
		// �������`�F�b�N
		strToken = strText.Left (lTemp);
		if (IsNumeric (strToken) <= 0) {
			return IDS_BEAT_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// ���l�`�F�b�N
		lBeat = _ttol (strToken) - 1;
		if (lBeat < 0 || lBeat >= 100) {
			return IDS_BEAT_VALUE_IS_OUT_OF_RANGE;
		}
		// �e�B�b�N�����`�F�b�N
		strText = strText.Mid (lTemp + 1);
		if (IsNumeric (strToken) <= 0) {
			return IDS_TICK_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// �e�B�b�N�l�`�F�b�N
		lTick = _ttol (strText);
		if (lTick < 0 || lTick > 960) {
			return IDS_TICK_VALUE_IS_OUT_OF_RANGE;
		}
		MIDIData_MakeTime (pMIDIData, lMeasure, lBeat, lTick, pTime);
		*pTime = CLIP (0x00000000, *pTime, 0x7FFFFFFF);
		return 0;
	}
	// SMPTE24�x�[�X�ASMPTE25�x�[�X�ASMPTE29.97�x�[�X�ASMPTE30�x�[�X�̏ꍇ�c"�t���[��:�T�u�t���[��"��long
	else if (lTimeMode == MIDIDATA_SMPTE24BASE || lTimeMode == MIDIDATA_SMPTE25BASE ||
		lTimeMode == MIDIDATA_SMPTE29BASE || lTimeMode == MIDIDATA_SMPTE30BASE) {
		long lFrame, lSubFrame;
		// �t���[���ƃT�u�t���[���̊Ԃ̃Z�p���[�^�`�F�b�N
		lTemp = strText.Find (_T(":"));
		if (lTemp < 0) {
			return IDS_A_SEPARATOR_BETWEEN_FRAME_AND_TICK_IS_NOT_FOUND;
		}
		// �t���[�������`�F�b�N
		strToken = strText.Left (lTemp);
		if (IsNumeric (strToken) <= 0) {
			return IDS_FRAME_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// �t���[���l�`�F�b�N
		lFrame = _ttol (strToken);
		if (lFrame < 0 || lFrame > 99999999) {
			return IDS_FRAME_VALUE_IS_OUT_OF_RANGE;
		}
		// �T�u�t���[�������`�F�b�N
		strText = strText.Mid (lTemp + 1);
		if (IsNumeric (strToken) <= 0) {
			return IDS_SUBFRAME_MUST_BE_HALF_WIDTH_NUMBER;
		}
		// �T�u�t���[���l�`�F�b�N
		lSubFrame = _ttol (strText);
		if (lSubFrame < 0 || lSubFrame > 960) {
			return IDS_SUBFRAME_VALUE_IS_OUT_OF_RANGE;
		}
		*pTime = lFrame * lTimeResolution + lSubFrame;
		*pTime = CLIP (0x00000000, *pTime, 0x7FFFFFFF);
		return 0;
	}
	else {
		return 0;
	}
}

// �V�K�g���b�N���X�g�\��
void CSekaijuDoc::ShowTrackListFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CTrackListFrame* pTrackListFrame = 
		(CTrackListFrame*)(pDocTemplate->CreateTrackListFrame (this));
	pDocTemplate->InitialUpdateFrame (pTrackListFrame, this);
}

// �V�K�s�A�m���[���\��
void CSekaijuDoc::ShowPianoRollFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CPianoRollFrame* pPianoRollFrame = 
		(CPianoRollFrame*)(pDocTemplate->CreatePianoRollFrame (this));
	pDocTemplate->InitialUpdateFrame (pPianoRollFrame, this);

}

// �V�K�C�x���g���X�g�\��
void CSekaijuDoc::ShowEventListFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CEventListFrame* pEventListFrame = 
		(CEventListFrame*)(pDocTemplate->CreateEventListFrame (this));
	pDocTemplate->InitialUpdateFrame (pEventListFrame, this);

}

// �V�K���ʕ\��
void CSekaijuDoc::ShowMusicalScoreFrame () {
	CSekaijuDocTemplate* pDocTemplate = (CSekaijuDocTemplate*)GetDocTemplate ();
	CMusicalScoreFrame* pMusicalScoreFrame = 
		(CMusicalScoreFrame*)(pDocTemplate->CreateMusicalScoreFrame (this));
	pDocTemplate->InitialUpdateFrame (pMusicalScoreFrame, this);

}





// MIDIEvent�𓯂�MIDI�C�x���g�Œu�������A�V��������MIDI�C�x���g��Ԃ��B
// MIDIEvent�̃v���p�e�B��ύX����ۂɂ͕K�����̊֐����Ăяo���A����Ԃ�ۑ�����B
// (1)pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent);
// (2)pCloneEvent = ReplaceMIDIevent (pMIDITrack, pMIDIEvent);
// (3)MIDIEvent_Set* (pCloneEvent, *);
// (4)pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent);
MIDIEvent* CSekaijuDoc::ReplaceMIDIEvent (MIDIEvent* pMIDIEvent) {
	long lCount = 0;
	long lPosition = 0;
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	MIDIEvent* pTargetEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
	MIDIEvent* pCloneEvent = MIDIEvent_CreateClone (pMIDIEvent);
	if (pCloneEvent == NULL) {
		return NULL;
	}
	MIDIEvent* pInsertEvent = MIDIEvent_GetFirstCombinedEvent (pCloneEvent);
	while (pTargetEvent && pInsertEvent) {
		MIDIEvent* pTargetPrevEvent = pTargetEvent->m_pPrevEvent;
		MIDIEvent* pTargetNextEvent = pTargetEvent->m_pNextEvent;
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE; // �t���O��AddHistoryRecord���Ō���
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_DEAD; // �t���O��AddHistoryRecord���Ō���
		VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pTargetEvent));
		VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pInsertEvent, pTargetPrevEvent));
		pTargetEvent = pTargetEvent->m_pNextCombinedEvent;
		pInsertEvent = pInsertEvent->m_pNextCombinedEvent;
		lCount++;
	}
	ASSERT (pTargetEvent == NULL && pInsertEvent == NULL);
	return pCloneEvent;
}

// MIDI�C�x���g�̕������s���B���������C�x���g�͕������C�x���g�̒���ɑ}������Ă���B
// ���ӁFEOT�Ȃǂ𕡐����Ă�NULL���Ԃ�B
MIDIEvent* CSekaijuDoc::DuplicateMIDIEvent (MIDIEvent* pMIDIEvent) {
	ASSERT (pMIDIEvent);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	ASSERT (pMIDITrack);
	long lCount = 0;
	long lRet = 0;
	MIDIEvent* pTargetEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
	MIDIEvent* pCloneEvent = MIDIEvent_CreateClone (pMIDIEvent);
	if (pCloneEvent == NULL) {
		return NULL;
	}
	MIDIEvent* pInsertEvent = pCloneEvent;
	while (pTargetEvent && pInsertEvent) {
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
		pInsertEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
		lRet = MIDITrack_InsertSingleEventAfter (pMIDITrack, pInsertEvent, pTargetEvent);
		if (lRet == 0) {
			MIDIEvent_Delete (pCloneEvent);
			pCloneEvent = NULL;
			return NULL;
		}
		pTargetEvent = pTargetEvent->m_pNextCombinedEvent;
		pInsertEvent = pInsertEvent->m_pNextCombinedEvent;
		lCount++;
	}
	ASSERT (pTargetEvent == NULL && pInsertEvent == NULL);
	return pCloneEvent;
}

// MIDITrack�I�u�W�F�N�g�������ւ��A�V����MIDITrack�I�u�W�F�N�g��Ԃ��B
// ������MIDIEvent�I�u�W�F�N�g�́A�V����MIDITrack�̏��L���ƂȂ�B
MIDITrack* CSekaijuDoc::ReplaceMIDITrack (MIDITrack* pMIDITrack) {
	ASSERT (pMIDITrack);
	MIDIData* pMIDIData = MIDITrack_GetParent (pMIDITrack);
	ASSERT (pMIDIData);
	// �C�x���g��0�̋��MIDITrack���쐬
	MIDITrack* pCloneTrack = NULL;
	pCloneTrack = MIDITrack_Create ();
	if (pCloneTrack == NULL) {
		return NULL;
	}
	pCloneTrack->m_lInputOn = pMIDITrack->m_lInputOn;
	pCloneTrack->m_lInputPort = pMIDITrack->m_lInputPort;
	pCloneTrack->m_lInputChannel = pMIDITrack->m_lInputChannel;
	pCloneTrack->m_lOutputOn = pMIDITrack->m_lOutputOn;
	pCloneTrack->m_lOutputPort = pMIDITrack->m_lOutputPort;
	pCloneTrack->m_lOutputChannel = pMIDITrack->m_lOutputChannel;
	pCloneTrack->m_lTimePlus = pMIDITrack->m_lTimePlus;
	pCloneTrack->m_lKeyPlus = pMIDITrack->m_lKeyPlus;
	pCloneTrack->m_lVelocityPlus = pMIDITrack->m_lVelocityPlus;
	pCloneTrack->m_lViewMode = pMIDITrack->m_lViewMode;
	pCloneTrack->m_lForeColor = pMIDITrack->m_lForeColor;
	pCloneTrack->m_lBackColor = pMIDITrack->m_lBackColor;
	pCloneTrack->m_lUser1 = pMIDITrack->m_lUser1;
	pCloneTrack->m_lUser2 = pMIDITrack->m_lUser2;
	pCloneTrack->m_lUser3 = pMIDITrack->m_lUser3;
	pCloneTrack->m_lUserFlag = pMIDITrack->m_lUserFlag;
	// �O�̃g���b�N�Ǝ��̃g���b�N��ێ����Ă���(�Ȃ����NULL)�B
	MIDITrack* pTargetPrevTrack = pMIDITrack->m_pPrevTrack;
	MIDITrack* pTargetNextTrack = pMIDITrack->m_pNextTrack;
	// �����ւ���MIDI�g���b�N����MIDI�C�x���g�����ׂč����ւ���MIDI�g���b�N�Ɉړ�����B
	MIDITrack_CountEvent (pMIDITrack);
	MIDIEvent* pMIDIEvent = MIDITrack_GetFirstEvent (pMIDITrack);
	while (pMIDIEvent) {
		VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
		VERIFY (MIDITrack_InsertEvent (pCloneTrack, pMIDIEvent));
		pMIDIEvent = MIDITrack_GetFirstEvent (pMIDITrack);
	}
	// MIDI�f�[�^����Â�MIDITrack�������A�V����MIDITrack��ǉ�
	VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
	VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pCloneTrack, pTargetPrevTrack));
	return pCloneTrack;
}

// MIDIData�I�u�W�F�N�g�������ւ��A�V����MIDIData�I�u�W�F�N�g��Ԃ��B
// ������MIDITrack�I�u�W�F�N�g�́A�V����MIDIData�̏��L���ƂȂ�B
MIDIData* CSekaijuDoc::ReplaceMIDIData (MIDIData* pMIDIData) {
	ASSERT (pMIDIData);
	// �g���b�N��0�̋��MIDI�f�[�^���쐬
	MIDIData* pCloneData = NULL;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	pCloneData = MIDIData_Create (lFormat, 0, lTimeMode, lTimeResolution);
	if (pCloneData == NULL) {
		return NULL;
	}
	pCloneData->m_lUser1 = pMIDIData->m_lUser1;
	pCloneData->m_lUser2 = pMIDIData->m_lUser2;
	pCloneData->m_lUser3 = pMIDIData->m_lUser3;
	pCloneData->m_lUserFlag = pMIDIData->m_lUserFlag;
	// �����ւ���MIDI�f�[�^����MIDI�g���b�N�����ׂč����ւ���MIDI�f�[�^�Ɉړ�����B
	MIDIData_CountTrack (pMIDIData);
	MIDITrack* pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	while (pMIDITrack) {
		VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
		VERIFY (MIDIData_AddTrack (pCloneData, pMIDITrack));
		pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	}
	// �Â�MIDIData�������A�V����MIDIData��ݒ�
	this->m_pMIDIData = pCloneData;
	return pCloneData;
}

// �V�K�������j�b�g�̒ǉ�(�������j�b�g��)
BOOL CSekaijuDoc::AddHistoryUnit (CString strName) {
	// ���݂̗������j�b�g�ȍ~�ɂ���s�v�ȗ������j�b�g���폜
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	for (long i = lNumHistoryUnit - 1; i > m_lCurHistoryPosition; i--) {
		CHistoryUnit* pHistoryUnit = (CHistoryUnit*)m_theHistoryUnitArray.GetAt (i);
		m_theHistoryUnitArray.RemoveAt (i);
		delete pHistoryUnit;
	}
	// �V�K�������j�b�g�쐬
	CHistoryUnit* pHistoryUnit = new CHistoryUnit ();
	pHistoryUnit->m_strName = strName;
	pHistoryUnit->m_theTime = CTime::GetCurrentTime ();
	// �������j�b�g�z��ɒǉ�
	m_theHistoryUnitArray.Add (pHistoryUnit);
	// ���̗������j�b�g�̃C���f�b�N�X�����݂̗������j�b�g�Ƃ���B
	m_lCurHistoryPosition = m_theHistoryUnitArray.GetSize () - 1;

	//for Debug
	//FILE* pFile = fopen ("history.txt", "at");
	//fprintf (pFile, "// AddHistoryUnit Index=%d, Name=%s, Time=%d\n", i+1, strName, pHistoryUnit->m_theTime);
	//fclose (pFile);
	
	return TRUE;
}

// ���݂̗������j�b�g���擾����
CHistoryUnit* CSekaijuDoc::GetCurHistoryUnit () {
	// ���݂̗������j�b�g�ԍ���m_lCurHistoryPosition���ێ����Ă���B
	// ���̒l�́A�u���ɖ߂��v�Ńf�N�������g�A�u��蒼���v�ŃC���N�������g����B
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	if (0 <= m_lCurHistoryPosition && m_lCurHistoryPosition < lNumHistoryUnit) {
		return (CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition);
	}
	ASSERT (FALSE);
	return NULL;
}

// �������j�b�g�̊��S����
BOOL CSekaijuDoc::DeleteAllHistoryUnit () {
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	for (long i = lNumHistoryUnit - 1; i >= 0; i--) {
		CHistoryUnit* pHistoryUnit = (CHistoryUnit*)m_theHistoryUnitArray.GetAt (i);
		m_theHistoryUnitArray.RemoveAt (i);
		delete pHistoryUnit;
	}
	m_lCurHistoryPosition = -1;
	return TRUE;
}





//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �w�t�@�C��(&F)�x-�w�㏑���ۑ�(&S)�x
void CSekaijuDoc::OnUpdateFileSaveUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

}

// �w�t�@�C��(&F)�x-�w���O��t���ĕۑ�(&A)�x
void CSekaijuDoc::OnUpdateFileSaveAsUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}


// �w�t�@�C��(&F)�x-�w�v���p�e�B(&R)�x
void CSekaijuDoc::OnFileProperty () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	// 20120107 '\t''\r''\n''\\'�̍����ɑΉ�
	CString strFormat;
	CFilePropertyDlg theDlg;
	TCHAR szBuf1[2048];
	TCHAR szBuf2[2048];
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetTitle (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strTitle = szBuf2;
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetSubTitle (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strSubTitle = szBuf2;
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetCopyright (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strCopyright = szBuf2;
	memset (szBuf1, 0, sizeof (szBuf1));
	memset (szBuf2, 0, sizeof (szBuf2));
	MIDIData_GetComment (m_pMIDIData, szBuf1, TSIZEOF (szBuf1) - 1);
	codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
	theDlg.m_strComment1 = szBuf2;
	long lNumTrack = MIDIData_CountTrack (m_pMIDIData);
	theDlg.m_strNumTrack.Format (_T("%d"), lNumTrack);
	long lNumEvent = 0;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		lNumEvent += MIDITrack_CountEvent (pMIDITrack);
	}
	theDlg.m_strNumEvent.Format (_T("%d"), lNumEvent);
	
	long lSMFFormat = MIDIData_GetFormat (m_pMIDIData);
	theDlg.m_nSMFFormat = (int)lSMFFormat;	
	long lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);
	switch (lTimeMode) {
	case MIDIDATA_TPQNBASE:
		theDlg.m_nTimeMode = 0;
		break;
	case MIDIDATA_SMPTE24BASE:
		theDlg.m_nTimeMode = 1;
		break;
	case MIDIDATA_SMPTE25BASE:
		theDlg.m_nTimeMode = 2;
		break;
	case MIDIDATA_SMPTE29BASE:
		theDlg.m_nTimeMode = 3;
		break;
	case MIDIDATA_SMPTE30BASE:
		theDlg.m_nTimeMode = 4;
		break;
	}
	long lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	theDlg.m_nResolution = (int)lTimeResolution;

	long lEndTime = MIDIData_GetEndTime (m_pMIDIData);
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (m_pMIDIData, lEndTime, &lMeasure, &lBeat, &lTick);
		VERIFY (strFormat.LoadString (IDS_05D_02D_03D_MEASURE_BEAT_TICK));
		theDlg.m_strEndTime.Format (strFormat, lMeasure + 1, lBeat + 1, lTick);
	}
	else if (lTimeMode == MIDIDATA_SMPTE24BASE ||
		lTimeMode == MIDIDATA_SMPTE25BASE ||
		lTimeMode == MIDIDATA_SMPTE29BASE ||
		lTimeMode == MIDIDATA_SMPTE30BASE) {
		long lFrameNumber, lTick;
		lFrameNumber = lEndTime / lTimeResolution;
		lTick = lEndTime % lTimeResolution;
		VERIFY (strFormat.LoadString (IDS_08D_03D_FRAME_TICK));
		theDlg.m_strEndTime.Format (strFormat, lFrameNumber, lTick);
	}
	long lEndMillisec = MIDIData_TimeToMillisec (m_pMIDIData, lEndTime);
	long lHour = (lEndMillisec / 3600000) % 3600000;
	long lMinute = (lEndMillisec / 60000) % 60;
	long lSec = (lEndMillisec / 1000) % 60;
	long lMillisec = lEndMillisec % 1000;
	VERIFY (strFormat.LoadString (IDS_02D_02D_02D_03D_HOUR_MINUTE_SECOND_MILLISEC));
	theDlg.m_strEndMillisec.Format (strFormat, lHour, lMinute, lSec, lMillisec);


	
	if (theDlg.DoModal () == IDOK) {

		if (m_bEditLocked) {
			return;
		}
		
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pTempEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		MIDITrack* pCloneTrack = NULL;
		MIDIData* pCloneData = NULL;

		// �V�����������j�b�g�̗p��
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_FILE_MODIFY_PROPERTY));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		CHistoryUnit* pCurHistoryUnit = NULL;
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		VERIFY (pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData));
		
		// �^�C�g���ݒ�
		// �ŏ��̃g���b�N���C�x���g��T��
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
				break;
			}
		}
		// �ŏ��̃g���b�N���C�x���g��������Ȃ������ꍇ
		if (pTempEvent == NULL) {
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strTitle, 
				theDlg.m_strTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// �ŏ��̃g���b�N�������������ꍇ
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strTitle, 
				theDlg.m_strTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			MIDIEvent_SetText (pCloneEvent, szBuf2);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}

		// �T�u�^�C�g���̐ݒ�(�󕶎��񎞂̓T�u�^�C�g���폜)
		// 2�Ԗڂ̃g���b�N���C�x���g��T��
		long lCount = 0;
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
				if (lCount == 1) {
					break;
				}
				lCount++;
			}
		}
		// �T�u�^�C�g����ݒ肷��ꍇ
		if (theDlg.m_strSubTitle != _T("")) {
			// 2�Ԗڂ̃g���b�N���C�x���g��������Ȃ������ꍇ
			if (pTempEvent == NULL) {
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strSubTitle, 
					theDlg.m_strSubTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
			}
			// 2�Ԗڂ̃g���b�N�������������ꍇ
			else {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strSubTitle, 
					theDlg.m_strSubTitle.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				MIDIEvent_SetText (pCloneEvent, szBuf2);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
		}
		// �T�u�^�C�g�����폜����ꍇ
		else {
			// 2�Ԗڂ̃g���b�N�������������ꍇ
			if (pTempEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pTempEvent));
			}
		}

		// ���쌠�ݒ�(�󕶎����͒��쌠�C�x���g�폜)
		// �ŏ��̒��쌠�C�x���g��T��
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_COPYRIGHTNOTICE) {
				break;
			}
		}
		// ���쌠��ݒ肷��ꍇ
		if (theDlg.m_strCopyright != _T("")) {
			// �ŏ��̒��쌠�C�x���g��������Ȃ������ꍇ
			if (pTempEvent == NULL) {
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strCopyright, 
					theDlg.m_strCopyright.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				VERIFY (pTempEvent = MIDIEvent_CreateCopyrightNotice (0, szBuf2));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
			}
			// �ŏ��̒��쌠�C�x���g�����������ꍇ
			else {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strCopyright, 
					theDlg.m_strCopyright.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				MIDIEvent_SetText (pCloneEvent, szBuf2);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
		}
		// ���쌠���폜����ꍇ
		else {
			// �ŏ��̒��쌠�C�x���g�����������ꍇ
			if (pTempEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pTempEvent));
			}
		}

		// �R�����g1�ݒ�(�󕶎����͍ŏ��̃e�L�X�g�C�x���g�폜)
		// �ŏ��̃e�L�X�g�C�x���g��T��
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TEXTEVENT) {
				break;
			}
		}
		// �R�����g1��ݒ肷��ꍇ
		if (theDlg.m_strComment1 != _T("")) {
			// �ŏ��̃e�L�X�g�C�x���g��������Ȃ������ꍇ
			if (pTempEvent == NULL) {
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strComment1, 
					theDlg.m_strComment1.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				VERIFY (pTempEvent = MIDIEvent_CreateTextEvent (0, szBuf2));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
			}
			// �ŏ��̃e�L�X�g�C�x���g�����������ꍇ
			else {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pTempEvent));
				memset (szBuf2, 0, sizeof (szBuf2));
				str2codestr ((LPTSTR)(LPCTSTR)theDlg.m_strComment1, 
					theDlg.m_strComment1.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
				MIDIEvent_SetText (pCloneEvent, szBuf2);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
		}
		// �R�����g1���폜����ꍇ
		else {
			// �ŏ��̃e�L�X�g�C�x���g�����������ꍇ
			if (pTempEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pTempEvent));
			}
		}

		// �V����SMF�t�H�[�}�b�g�����o
		long lNewSMFFormat = theDlg.m_nSMFFormat;
		// SMF�t�H�[�}�b�g���ύX���ꂽ�ꍇ
		if (lNewSMFFormat != lSMFFormat) {
			// �S�I�u�W�F�N�g�̗����L�^(20090116�����L�^���@�C��)
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord 
					(HISTORYRECORD_REMOVEEVENTALL, MIDITrack_GetFirstEvent (pMIDITrack)));
				}
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEDATA, m_pMIDIData));

			// �S�I�u�W�F�N�g�̌���
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				forEachEventInverse (pMIDITrack, pMIDIEvent) {
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pMIDIEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
				VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
				pMIDITrack = pCloneTrack;
			}
			VERIFY (m_pMIDIData = this->ReplaceMIDIData (m_pMIDIData));

			// �t�H�[�}�b�g0/1/2�ϊ�
			MIDIData_SetFormat (m_pMIDIData, lNewSMFFormat);
			MIDIData_CountTrack (m_pMIDIData);
			long i = 0;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDITrack_SetForeColor (pMIDITrack, pSekaijuApp->m_lDefTrackColor[i % 16]);
				i++;
			}

			// �S�I�u�W�F�N�g�̗����L�^(20090116�����L�^���@�C��)
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTDATA, m_pMIDIData));
			forEachTrack (m_pMIDIData, pMIDITrack) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pMIDITrack));
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENTALL, 
					MIDITrack_GetFirstEvent (pMIDITrack)));
				}
			}
		}
		
		// �V�����^�C�����[�h�ƕ���\�����o
		long lNewTimeMode = 0;
		switch (theDlg.m_nTimeMode) {
		case 0:
			lNewTimeMode = MIDIDATA_TPQNBASE;
			break;
		case 1:
			lNewTimeMode = MIDIDATA_SMPTE24BASE;
			break;
		case 2:
			lNewTimeMode = MIDIDATA_SMPTE25BASE;
			break;
		case 3:
			lNewTimeMode = MIDIDATA_SMPTE29BASE;
			break;
		case 4:
			lNewTimeMode = MIDIDATA_SMPTE30BASE;
			break;
		}
		long lNewTimeResolution = theDlg.m_nResolution;
		// �^�C�����[�h���͕���\���ύX���ꂽ�ꍇ
		if (lNewTimeMode != lTimeMode ||
			lNewTimeResolution != lTimeResolution) {
			CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
			
			// ���t���̏ꍇ�A���t�̈ꎞ��~
			pSekaijuApp->SendAllNoteOff ();
			pSekaijuApp->SendAllHold1Off ();
			pSekaijuApp->SendAllSostenutoOff ();
			pSekaijuApp->SendAllHold2Off ();
			if (pSekaijuApp->m_bPlaying) {
				MIDIClock_Stop (m_pMIDIClock);
			}
			long lTempo = MIDIEVENT_DEFTEMPO;
			VERIFY (MIDIData_FindTempo (m_pMIDIData, 0, &lTempo));

			// �S�I�u�W�F�N�g�̗����L�^(20090116�����L�^���@�C��)
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord 
					(HISTORYRECORD_REMOVEEVENTALL, MIDITrack_GetFirstEvent (pMIDITrack)));
				}
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEDATA, m_pMIDIData));

			// �S�I�u�W�F�N�g�̌���
			forEachTrackInverse (m_pMIDIData, pMIDITrack) {
				forEachEventInverse (pMIDITrack, pMIDIEvent) {
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pMIDIEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
				VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
				pMIDITrack = pCloneTrack;
			}
			VERIFY (m_pMIDIData = this->ReplaceMIDIData (m_pMIDIData));
			
			// �^�C���x�[�X(�^�C�����[�h�y�ѕ���\)�̕ϊ�
			MIDIData_SetTimeBase (m_pMIDIData, lNewTimeMode, lNewTimeResolution);

			// �S�I�u�W�F�N�g�̗����L�^(20090116�����L�^���@�C��)
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTDATA, m_pMIDIData));
			forEachTrack (m_pMIDIData, pMIDITrack) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pMIDITrack));
				if (MIDITrack_GetFirstEvent (pMIDITrack)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord 
					(HISTORYRECORD_INSERTEVENTALL, MIDITrack_GetFirstEvent (pMIDITrack)));
				}
			}
			
			// MIDI�N���b�N�̃^�C�����[�h�Ƒ��x���A�v���P�[�V�����ɍ��킹��(20090625�ǉ�)
			ApplyAppCurSpeedIndex ();

			MIDIClock_Reset (m_pMIDIClock);
			MIDIClock_SetTempo (m_pMIDIClock, lTempo);

			// ���t���������ꍇ�A���t�̑��s
			if (pSekaijuApp->m_bPlaying) {
				MIDIClock_Start (m_pMIDIClock);
			}
		}
		SetModifiedFlag (TRUE);
		UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// �w�t�@�C��(&F)�x-�w�v���p�e�B(&R)�x
void CSekaijuDoc::OnUpdateFilePropertyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}

}


// �w�ҏW(&E)�x-�w���ɖ߂�(&U)�x
void CSekaijuDoc::OnEditUndo () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	CString strMsg;
	this->m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// ���݂̗������j�b�g���Ȃ��ꍇ
	if (m_lCurHistoryPosition <= -1) {
		this->m_theCriticalSection.Unlock ();
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_UNDO_ANYMORE));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	BeginWaitCursor ();
	// ���݂̗������j�b�g���擾
	CHistoryUnit* pHistoryUnit = 
		(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition);
	long lHistoryRecordCount = pHistoryUnit->m_theHistoryRecordArray.GetSize ();
	long i;
	MIDIData* pMIDIData = NULL;
	MIDITrack* pPrevTrack = NULL;
	MIDITrack* pNextTrack = NULL;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDIEvent* pNextEvent = NULL;
	MIDIEvent* pFirstEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pFirstTrack = NULL;
	MIDITrack* pLastTrack = NULL;
	MIDIData* pTempData = NULL;
	
	// �������j�b�g���ɂ��邷�ׂĂ̋L�^�ɂ��āA�������]��
	for (i = lHistoryRecordCount - 1; i >= 0; i--) {
		CHistoryRecord* pHistoryRecord = 
			(CHistoryRecord*)(pHistoryUnit->m_theHistoryRecordArray.GetAt (i));
		switch (pHistoryRecord->m_lType) {
		// MIDIEvent�I�u�W�F�N�g�̑}���L�^��MIDIEvent�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_INSERTEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;
		// MIDIEvent�I�u�W�F�N�g�̏����L�^��MIDIEvent�I�u�W�F�N�g�̕���
		case HISTORYRECORD_REMOVEEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEvent�̑}���ʒu������@�ύX
			if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else {
				long lInsertTime = MIDIEvent_GetTime (pMIDIEvent);
				pTempEvent = pMIDITrack->m_pLastEvent;
				// �g���b�N�̌������}���ʒu��T��
				while (1) {
					// �g���b�N�Ƀf�[�^���Ȃ��A���̓g���b�N�̐擪����Ă悢
					if (pTempEvent == NULL) {
						VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, NULL));
						break;
					}
					// pTempEvent�̒���ɓ���Ă悢
					else {
						// �}��������̂��m�[�g�I�t�C�x���g�̏ꍇ(�x���V�e�B0�̃m�[�g�I�����܂�)
						if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
							// �Ή�����m�[�g�I���C�x���g���O�ɂ͐�΂ɗ���Ȃ� (20090111�ǉ�)
							if (pTempEvent == pMIDIEvent->m_pPrevCombinedEvent) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							// �������̃C�x���g������ꍇ�͓������̑��̃m�[�g�I�t�̒���ɑ}��
							else if (pTempEvent->m_lTime == lInsertTime && MIDIEvent_IsNoteOff (pTempEvent)) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							else if (pTempEvent->m_lTime < lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
						// ���̑��̃C�x���g�̏ꍇ
						else {
							if (pTempEvent->m_lTime <= lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
					}
					pTempEvent = pTempEvent->m_pPrevEvent;
				}
			}
			break;
		// MIDIEvent�I�u�W�F�N�g�̑S�}���L�^���SMIDIEvent�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_INSERTEVENTALL:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;
		// MIDIEvent�I�u�W�F�N�g�̑S�����L�^���SMIDIEvent�I�u�W�F�N�g�̕���
		case HISTORYRECORD_REMOVEEVENTALL:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEvent�̑}���ʒu������@�ύX
			if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDITrack�I�u�W�F�N�g�̑}���L�^��MIDITrack�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_INSERTTRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
			VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
			break;
		// MIDITrack�I�u�W�F�N�g�̏����L�^��MIDITrack�I�u�W�F�N�g�̕���
		case HISTORYRECORD_REMOVETRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			if (pFirstEvent || pLastEvent) {
				VERIFY (pTempEvent = pFirstEvent);
				pTempTrack = MIDIEvent_GetParent (pFirstEvent);
				if (pTempTrack != NULL && pTempTrack != pMIDITrack) {
					while (pTempEvent) {
						VERIFY (MIDITrack_RemoveEvent (pTempTrack, pTempEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
						pTempEvent = MIDITrack_GetFirstEvent (pTempTrack);
					}
				}
			}
			// 20090116 MIDITrack�̑}���ʒu������@�ύX
			if (pNextTrack == NULL) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else if (MIDITrack_GetParent (pNextTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else if (pPrevTrack == NULL) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else if (MIDITrack_GetParent (pPrevTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDIData�I�u�W�F�N�g�̑}���L�^��MIDIData�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_INSERTDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pMIDIData->m_lUserFlag &= ~MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag |= MIDIDATA_DEAD;
			this->m_pMIDIData = NULL;
			break;
		// MIDIData�I�u�W�F�N�g�̏����L�^��MIDIData�I�u�W�F�N�g�̕���
		case HISTORYRECORD_REMOVEDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pFirstTrack = (MIDITrack*)(pHistoryRecord->m_pFirstChild);
			pLastTrack = (MIDITrack*)(pHistoryRecord->m_pLastChild);
			pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
			if (pFirstTrack || pLastTrack) {
				VERIFY (pTempTrack = pFirstTrack);
				pTempData = MIDITrack_GetParent (pFirstTrack);
				if (pTempData != NULL && pTempData != pMIDIData) {
					while (pTempTrack) {
						VERIFY (MIDIData_RemoveTrack (pTempData, pTempTrack));
						VERIFY (MIDIData_AddTrack (pMIDIData, pTempTrack));
						pTempTrack = MIDIData_GetFirstTrack (pTempData);
					}
				}
			}
			this->m_pMIDIData = pMIDIData;
			break;
		}
	}

	// �K�v�ɉ�����MIDIClock�̕␳
	long lMIDIClockTimeMode;
	long lMIDIClockTimeResolution;
	MIDIClock_GetTimeBase (m_pMIDIClock, &lMIDIClockTimeMode, &lMIDIClockTimeResolution);
	long lMIDIDataTimeMode;
	long lMIDIDataTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lMIDIDataTimeMode, &lMIDIDataTimeResolution);
	if (lMIDIClockTimeMode != lMIDIDataTimeMode || 
		lMIDIClockTimeResolution != lMIDIDataTimeResolution) {
		// ���t���̏ꍇ�A���t�̈ꎞ��~
		pSekaijuApp->SendAllNoteOff ();
		pSekaijuApp->SendAllHold1Off ();
		pSekaijuApp->SendAllSostenutoOff ();
		pSekaijuApp->SendAllHold2Off ();
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Stop (m_pMIDIClock);
		}
		long lTempo = MIDIEVENT_DEFTEMPO;
		VERIFY (MIDIData_FindTempo (m_pMIDIData, 0, &lTempo));
		MIDIClock_Reset (m_pMIDIClock);
		MIDIClock_SetTimeBase (m_pMIDIClock, lMIDIDataTimeMode, lMIDIDataTimeResolution);
		MIDIClock_SetTempo (m_pMIDIClock, lTempo);
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Start (m_pMIDIClock);
		}
	}

	// ���݂̗������j�b�g�ԍ����f�N�������g
	this->m_lCurHistoryPosition--;
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	this->SetModifiedFlag (TRUE);
	this->m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	return;
}

// �w�ҏW(&E)�x-�w���ɖ߂�(&U)�x
void CSekaijuDoc::OnUpdateEditUndoUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// ���ɖ߂��Ȃ��ꍇ
	if (m_lCurHistoryPosition <= -1) {
		CString strText;
		VERIFY (strText.LoadString (IDS_UNABLE_TO_UNDO_T_CTRL_Z));
		pCmdUI->SetText (strText);
		pCmdUI->Enable (FALSE);
	}
	// ���ɖ߂���ꍇ
	else {
		CHistoryUnit* pHistoryUnit = 
			(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition);
		CString strFormat;
		CString strText;
		VERIFY (strFormat.LoadString (IDS_UNDO_02D_02D_02D_S_T_CTRL_Z));
		strText.Format (strFormat,
			pHistoryUnit->m_theTime.GetHour (),
			pHistoryUnit->m_theTime.GetMinute (),
			pHistoryUnit->m_theTime.GetSecond (),
			pHistoryUnit->m_strName);
		pCmdUI->SetText (strText);
		pCmdUI->Enable (TRUE);
	}
	m_theCriticalSection.Unlock ();
}


// �w�ҏW(&E)�x-�w��蒼��(&R)�x
void CSekaijuDoc::OnEditRedo () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	this->m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// ���̗������j�b�g���Ȃ��ꍇ
	if (m_lCurHistoryPosition >= lNumHistoryUnit - 1) {
		this->m_theCriticalSection.Unlock ();
		CString strText;
		VERIFY (strText.LoadString (IDS_UNABLE_TO_REDO_ANYMORE));
		AfxMessageBox (strText, MB_ICONEXCLAMATION);
		return;
	}
	
	BeginWaitCursor ();
	// ���̗������j�b�g���擾
	CHistoryUnit* pHistoryUnit = 
		(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition + 1);

	long lHistoryRecordCount = pHistoryUnit->m_theHistoryRecordArray.GetSize ();
	long i;
	MIDIData* pMIDIData = NULL;
	MIDITrack* pPrevTrack = NULL;
	MIDITrack* pNextTrack = NULL;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDIEvent* pNextEvent = NULL;
	MIDIEvent* pFirstEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pFirstTrack = NULL;
	MIDITrack* pLastTrack = NULL;
	MIDIData* pTempData = NULL;

	// �������j�b�g���ɂ��邷�ׂĂ̋L�^�ɂ��āA�O������]��
	for (i = 0; i < lHistoryRecordCount; i++) {
		CHistoryRecord* pHistoryRecord = 
			(CHistoryRecord*)(pHistoryUnit->m_theHistoryRecordArray.GetAt (i));
		switch (pHistoryRecord->m_lType) {
		// MIDIEvent�I�u�W�F�N�g�̑}���L�^��MIDIEvent�I�u�W�F�N�g�̕���
		case HISTORYRECORD_INSERTEVENT:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEvent�̑}���ʒu������@�ύX
			if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else {
				long lInsertTime = MIDIEvent_GetTime (pMIDIEvent);
				pTempEvent = pMIDITrack->m_pLastEvent;
				// �g���b�N�̌������}���ʒu��T��
				while (1) {
					// �g���b�N�Ƀf�[�^���Ȃ��A���̓g���b�N�̐擪����Ă悢
					if (pTempEvent == NULL) {
						VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, NULL));
						break;
					}
					// pTempEvent�̒���ɓ���Ă悢
					else {
						// �}��������̂��m�[�g�I�t�C�x���g�̏ꍇ(�x���V�e�B0�̃m�[�g�I�����܂�)
						if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
							// �Ή�����m�[�g�I���C�x���g���O�ɂ͐�΂ɗ���Ȃ� (20090111�ǉ�)
							if (pTempEvent == pMIDIEvent->m_pPrevCombinedEvent) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							// �������̃C�x���g������ꍇ�͓������̑��̃m�[�g�I�t�̒���ɑ}��
							else if (pTempEvent->m_lTime == lInsertTime && MIDIEvent_IsNoteOff (pTempEvent)) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
							else if (pTempEvent->m_lTime < lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
						// ���̑��̃C�x���g�̏ꍇ
						else {
							if (pTempEvent->m_lTime <= lInsertTime) {
								VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pTempEvent));
								break;
							}
						}
					}
					pTempEvent = pTempEvent->m_pPrevEvent;
				}
			}
			//if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
			//	if (pMIDIEvent->m_pPrevEvent) {
			//		if (pMIDIEvent->m_pPrevEvent->m_lTime == pMIDIEvent->m_lTime) {
			//			ASSERT (MIDIEvent_IsNoteOff (pMIDIEvent->m_pPrevEvent));
			//		}
			//	}
			//}
			break;
		// MIDIEvent�I�u�W�F�N�g�̏����L�^��MIDIEvent�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_REMOVEEVENT:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;

		// MIDIEvent�I�u�W�F�N�g�̑S�}���L�^���SMIDIEvent�I�u�W�F�N�g�̕���
		case HISTORYRECORD_INSERTEVENTALL:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			// 20090116 MIDIEvent�̑}���ʒu������@�ύX
			if (pPrevEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (MIDIEvent_GetParent (pPrevEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventAfter (pMIDITrack, pMIDIEvent, pPrevEvent));
			}
			else if (pNextEvent == NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else if (MIDIEvent_GetParent (pNextEvent) != NULL) {
				VERIFY (MIDITrack_InsertSingleEventBefore (pMIDITrack, pMIDIEvent, pNextEvent));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDIEvent�I�u�W�F�N�g�̑S�����L�^���SMIDIEvent�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_REMOVEEVENTALL:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pParent);
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			pPrevEvent = (MIDIEvent*)(pHistoryRecord->m_pPrevObject);
			pNextEvent = (MIDIEvent*)(pHistoryRecord->m_pNextObject);
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			VERIFY (MIDITrack_RemoveSingleEvent (pMIDITrack, pMIDIEvent));
			break;

		// MIDITrack�I�u�W�F�N�g�̑}���L�^��MIDITrack�I�u�W�F�N�g�̕���
		case HISTORYRECORD_INSERTTRACK:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			if (pFirstEvent || pLastEvent) {
				VERIFY (pTempEvent = pFirstEvent);
				pTempTrack = MIDIEvent_GetParent (pFirstEvent);
				if (pTempTrack != NULL && pTempTrack != pMIDITrack) {
					while (pTempEvent) {
						VERIFY (MIDITrack_RemoveEvent (pTempTrack, pTempEvent));
						VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
						pTempEvent = MIDITrack_GetFirstEvent (pTempTrack);
					}
				}
			}
			// 20090116 MIDITrack�̑}���ʒu������@�ύX
			if (pPrevTrack == NULL) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else if (MIDITrack_GetParent (pPrevTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackAfter (pMIDIData, pMIDITrack, pPrevTrack));
			}
			else if (pNextTrack == NULL) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else if (MIDITrack_GetParent (pNextTrack) == pMIDIData) {
				VERIFY (MIDIData_InsertTrackBefore (pMIDIData, pMIDITrack, pNextTrack));
			}
			else {
				ASSERT (FALSE);
			}
			break;
		// MIDITrack�I�u�W�F�N�g�̏����L�^��MIDITrack�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_REMOVETRACK:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pParent);
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			pPrevTrack = (MIDITrack*)(pHistoryRecord->m_pPrevObject);
			pNextTrack = (MIDITrack*)(pHistoryRecord->m_pNextObject);
			pFirstEvent = (MIDIEvent*)(pHistoryRecord->m_pFirstChild);
			pLastEvent = (MIDIEvent*)(pHistoryRecord->m_pLastChild);
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
			pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
			VERIFY (MIDIData_RemoveTrack (pMIDIData, pMIDITrack));
			break;
		// MIDIData�I�u�W�F�N�g�̑}���L�^��MIDIData�I�u�W�F�N�g�̕���
		case HISTORYRECORD_INSERTDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pFirstTrack = (MIDITrack*)(pHistoryRecord->m_pFirstChild);
			pLastTrack = (MIDITrack*)(pHistoryRecord->m_pLastChild);
			pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
			if (pFirstTrack || pLastTrack) {
				VERIFY (pTempTrack = pFirstTrack);
				pTempData = MIDITrack_GetParent (pFirstTrack);
				if (pTempData != NULL && pTempData != pMIDIData) {
					while (pTempTrack) {
						VERIFY (MIDIData_RemoveTrack (pTempData, pTempTrack));
						VERIFY (MIDIData_AddTrack (pMIDIData, pTempTrack));
						pTempTrack = MIDIData_GetFirstTrack (pTempData);
					}
				}
			}
			this->m_pMIDIData = pMIDIData;
			break;
		// MIDIData�I�u�W�F�N�g�̏����L�^��MIDIData�I�u�W�F�N�g�̔r��
		case HISTORYRECORD_REMOVEDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			pFirstTrack = (MIDITrack*)(pHistoryRecord->m_pFirstChild);
			pLastTrack = (MIDITrack*)(pHistoryRecord->m_pLastChild);
			pMIDIData->m_lUserFlag &= ~MIDIDATA_ALIVE;
			pMIDIData->m_lUserFlag |= MIDIDATA_DEAD;
			this->m_pMIDIData = NULL;
			break;
		}
	}

	// �K�v�ɉ�����MIDIClock�̕␳
	long lMIDIClockTimeMode;
	long lMIDIClockTimeResolution;
	MIDIClock_GetTimeBase (m_pMIDIClock, &lMIDIClockTimeMode, &lMIDIClockTimeResolution);
	long lMIDIDataTimeMode;
	long lMIDIDataTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lMIDIDataTimeMode, &lMIDIDataTimeResolution);
	if (lMIDIClockTimeMode != lMIDIDataTimeMode || 
		lMIDIClockTimeResolution != lMIDIDataTimeResolution) {
		// ���t���̏ꍇ�A���t�̈ꎞ��~
		pSekaijuApp->SendAllNoteOff ();
		pSekaijuApp->SendAllHold1Off ();
		pSekaijuApp->SendAllSostenutoOff ();
		pSekaijuApp->SendAllHold2Off ();
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Stop (m_pMIDIClock);
		}
		long lTempo = MIDIEVENT_DEFTEMPO;
		VERIFY (MIDIData_FindTempo (m_pMIDIData, 0, &lTempo));
		MIDIClock_Reset (m_pMIDIClock);
		MIDIClock_SetTimeBase (m_pMIDIClock, lMIDIDataTimeMode, lMIDIDataTimeResolution);
		MIDIClock_SetTempo (m_pMIDIClock, lTempo);
		if (pSekaijuApp->m_bPlaying) {
			MIDIClock_Start (m_pMIDIClock);
		}
	}
	
	// ���݂̗������j�b�g�ԍ����C���N�������g
	this->m_lCurHistoryPosition++;
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	this->SetModifiedFlag (TRUE);
	this->m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	return;
}

// �w�ҏW(&E)�x-�w��蒼��(&R)�x
void CSekaijuDoc::OnUpdateEditRedoUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	m_theCriticalSection.Lock ();
	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	// ��蒼���ł��Ȃ��ꍇ
	if (m_lCurHistoryPosition >= lNumHistoryUnit - 1) {
		CString strText;
		VERIFY (strText.LoadString (IDS_UNABLE_TO_REDO_T_CTRL_Y));
		pCmdUI->SetText (strText);
		pCmdUI->Enable (FALSE);
	}
	// ��蒼���ł���ꍇ
	else {
		CHistoryUnit* pHistoryUnit = 
			(CHistoryUnit*)m_theHistoryUnitArray.GetAt (m_lCurHistoryPosition + 1);
		CString strFormat;
		CString strText;
		VERIFY (strFormat.LoadString (IDS_REDO_02D_02D_02D_S_T_CTRL_Y));
		strText.Format (strFormat,
			pHistoryUnit->m_theTime.GetHour (),
			pHistoryUnit->m_theTime.GetMinute (),
			pHistoryUnit->m_theTime.GetSecond (),
			pHistoryUnit->m_strName);
		pCmdUI->SetText (strText);
		pCmdUI->Enable (TRUE);
	}
	m_theCriticalSection.Unlock ();
}


// �w�ҏW(&E)�x-�w��Ɨ����̏������x
void CSekaijuDoc::OnEditInitHistory () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	CString strMsg;
	VERIFY (strMsg.LoadString (IDS_INITIALIZE_HISTORY));
	int nRet = AfxMessageBox (strMsg, MB_ICONINFORMATION | MB_YESNO);

	if (nRet == IDYES) {
		BeginWaitCursor ();
		this->m_theCriticalSection.Lock ();
		this->DeleteAllHistoryUnit ();
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
			SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
		this->SetModifiedFlag (TRUE);
		this->m_theCriticalSection.Unlock ();
		EndWaitCursor ();
	}

}



// �w�ҏW(&E)�x-�w��Ɨ����̏������x
void CSekaijuDoc::OnUpdateEditInitHistoryUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	long lNumHistoryUnit = m_theHistoryUnitArray.GetSize ();
	if (lNumHistoryUnit <= 0) {
		pCmdUI->Enable (FALSE);
	}
}




// �w�ҏW(E)�x-�w�؂���(T)�x
void CSekaijuDoc::OnEditCut () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// �N���b�v�{�[�h�ɓ]�����镶������쐬
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0000, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// �N���b�v�{�[�h�ɓƎ��e�L�X�g��]��
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// �I���C�x���g���폜
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_CUT));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());

	this->DeleteSelectedEvent (pCurHistoryUnit);
	
	SetModifiedFlag (TRUE);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);

}

// �w�ҏW(E)�x-�w�؂���(T)�x
void CSekaijuDoc::OnUpdateEditCutUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		if (this->IsTrackSelected (pMIDITrack)) {
			lEnable = 1;
		}
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// �w�ҏW(E)�x-�w�R�s�[(C)�x
void CSekaijuDoc::OnEditCopy () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// �N���b�v�{�[�h�ɓ]�����镶������쐬
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0000, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// �N���b�v�{�[�h�ɓƎ��e�L�X�g��]��
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	EndWaitCursor ();
}

// �w�ҏW(E)�x-�w�R�s�[(C)�x
void CSekaijuDoc::OnUpdateEditCopyUI (CCmdUI* pCmdUI) {
	
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// �ЂƂł�MIDI�C�x���g���I������Ă����Enable
	// �����łȂ����Disable�Ƃ���
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}


// �w�ҏW(E)�x-�w�\��t��(P)�x
void CSekaijuDoc::OnEditPaste () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	BeginWaitCursor ();
	CString strMsg;

	// �N���b�v�{�[�h�̃e�L�X�g���擾
	CString strText;
	long lRet = this->GetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_READ_FROM_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// �e�L�X�g�t�H�[�}�b�g����
	if (strText.Left(5) != _T("MDa2 ")) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_NO_AVAILABLE_MIDIDATA_IN_THE_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// �\��t������
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_PASTE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	m_theCriticalSection.Lock ();
	m_lTempTrackIndex = 0;
	long lMeasure, lBeat, lTick;
	long lTime = m_lNewTime;
	MIDIData_BreakTime (m_pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	MIDIData_MakeTime (m_pMIDIData, lMeasure, 0, 0, &lTime);
	m_lTempTime = lTime;
	long lInsertedEventCount = this->ParsePasteString (strText, m_lTempTrackIndex, m_lTempTime, pCurHistoryUnit);
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);

	// ���ʂ̃��b�Z�[�W�{�b�N�X�\��
	if (lInsertedEventCount == -1) {
		VERIFY (strMsg.LoadString (IDS_PASTE_FAILED));
	}
	else {
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_D_MIDIEVENTS_WERE_INSERTED));
		strMsg.Format (strFormat, lInsertedEventCount);
	}
	AfxMessageBox (strMsg, MB_ICONINFORMATION);
}

// �w�ҏW(E)�x-�w�\��t��(P)�x
void CSekaijuDoc::OnUpdateEditPasteUI (CCmdUI* pCmdUI) {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	
	// �N���b�v�{�[�h�̌`�����Ǝ��e�L�X�g�����ׂ�
	pCmdUI->Enable (this->IsClipboardTextPrivate9 ());
}

// �w�ҏW(E)�x-�w�폜(D)�x
void CSekaijuDoc::OnEditDelete () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	
	BeginWaitCursor ();
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DELETE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());

	this->DeleteSelectedEvent (pCurHistoryUnit);

	SetModifiedFlag (TRUE);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�폜(D)�x
void CSekaijuDoc::OnUpdateEditDeleteUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// MIDI�C�x���g���ЂƂł��I����ԂȂ�Enable
	// �����łȂ����Disable
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// �w�ҏW(E)�x-�w���ׂđI��(A)�x
void CSekaijuDoc::OnEditSelectAll () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	
	// ���ׂĂ�MIDI�C�x���g��I����Ԃɂ���
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_SELECT_ALL));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				pCloneEvent = this->SelectEvent (pMIDIEvent, 1, pCurHistoryUnit);
				if (pCloneEvent) {
					pMIDIEvent = pCloneEvent;
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w���ׂđI��(A)�x
void CSekaijuDoc::OnUpdateEditSelectAllUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// ���ׂẴC�x���g���I������Ă���΂��̍��ڂ��`�F�b�N����
	m_theCriticalSection.Lock ();
	long lCheck = 1;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent) == 0) {
				lCheck = 0;
				break;
			}
		}
		if (lCheck == 0) {
			break;
		}
	}
	pCmdUI->SetCheck (lCheck);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	
}

// �w�ҏW(E)�x-�w���ׂđI������(N)�x
void CSekaijuDoc::OnEditSelectNone () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	
	// ���ׂĂ�MIDI�C�x���g���I����Ԃɂ���
	BeginWaitCursor ();
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DESELECT_ALL));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	this->SelectNoObject (pCurHistoryUnit);
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w���ׂđI������(N)�x
void CSekaijuDoc::OnUpdateEditSelectNoneUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��`�F�b�N����
	m_theCriticalSection.Lock ();
	long lCheck = 1;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent) != 0) {
				lCheck = 0;
				break;
			}
		}
		if (lCheck == 0) {
			break;
		}
	}
	pCmdUI->SetCheck (lCheck);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �w�ҏW(E)�x-�w���݈ʒu���O�̃C�x���g��ǉ��I���x
void CSekaijuDoc::OnEditSelectBefore () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	// ���݈ʒu���O�̃C�x���g��I����Ԃɂ���
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_SELECT_BEFORE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime < m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 1, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w���݈ʒu���O�̃C�x���g��ǉ��I���x
void CSekaijuDoc::OnUpdateEditSelectBeforeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�ҏW(E)�x-�w���݈ʒu���O�̃C�x���g��I�������x
void CSekaijuDoc::OnEditDeselectBefore () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	// ���݈ʒu���O�̃C�x���g��I�������ɂ���
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DESELECT_BEFORE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime < m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 0, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);}

// �w�ҏW(E)�x-�w���݈ʒu���O�̃C�x���g��I�������x
void CSekaijuDoc::OnUpdateEditDeselectBeforeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�ҏW(E)�x-�w���݈ʒu����̃C�x���g��ǉ��I���x
void CSekaijuDoc::OnEditSelectAfter () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	// ���݈ʒu����̃C�x���g��I����Ԃɂ���
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_SELECT_AFTER));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime >= m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 1, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w���݈ʒu����̃C�x���g��ǉ��I���x
void CSekaijuDoc::OnUpdateEditSelectAfterUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�ҏW(E)�x-�w���݈ʒu����̃C�x���g��I�������x
void CSekaijuDoc::OnEditDeselectAfter () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	// ���݈ʒu����̃C�x���g��I�������ɂ���
	BeginWaitCursor ();
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_DESELECT_AFTER));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (pMIDIEvent->m_pPrevCombinedEvent == NULL) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime >= m_lNewTime) {
					pCloneEvent = this->SelectEvent (pMIDIEvent, 0, pCurHistoryUnit);
					if (pCloneEvent) {
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
	}
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w���݈ʒu����̃C�x���g��I�������x
void CSekaijuDoc::OnUpdateEditDeselectAfterUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�ҏW(E)�x-�w�g���b�N�̕ύX...�x
void CSekaijuDoc::OnEditTrack () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);

	CEditTrackDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditTrackDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditTrackDlgStatus.m_nUnit, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		long i, j;
		long lErrorCount = 0;
		long lFormat = MIDIData_GetFormat (m_pMIDIData);
		long lTrackCount = MIDIData_CountTrack (m_pMIDIData);
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_TRACKINDEX));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		// �����C�x���g��2��ړ����Ȃ��悤�ɁA�I������Ă���C�x���g���ꎞ�z��ɓo�^����B
		CPtrArray theTempSelectedEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// �񌋍��C�x���g�̂݁A�����C�x���g�̏ꍇ�͑O�񌋍��̂݁A�Ō��EOT�͏��O
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent) &&
						!(MIDIEvent_IsEndofTrack (pMIDIEvent) &&
						MIDITrack_GetLastEvent (pMIDITrack) == pMIDIEvent)) {
						theTempSelectedEventArray.Add (pMIDIEvent);
					}
				}
			}
		}

		// ���ׂĂ̈ړ��\�ȑI������Ă���C�x���g�ɂ���
		long lTempSelectedEventCount = theTempSelectedEventArray.GetSize ();
		for (j = 0; j < lTempSelectedEventCount; j++) {
			pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
			long lKind = MIDIEvent_GetKind (pMIDIEvent);
			pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
			ASSERT (pMIDITrack);

			// �ړ���g���b�N���v�Z
			long lTargetTrackIndex = 0;
			if (theDlg.m_nUnit == 0) {
				lTargetTrackIndex = (theDlg.m_nAmount - (bTrackZeroOrigin ? 0 : 1));
			}
			else {
				long lTrackIndex = GetTrackIndex (pMIDITrack);
				lTargetTrackIndex = lTrackIndex + theDlg.m_nAmount;
			}
			lTargetTrackIndex = CLIP (0, lTargetTrackIndex, (MAXMIDITRACKNUM - 1));
			MIDITrack* pTargetTrack = this->GetTrack (lTargetTrackIndex);

			// �ړ��\�ȃC�x���g�̏ꍇ�̂�
			if (lFormat == 1 && !MIDIEvent_IsMIDIEvent (pMIDIEvent) && lTargetTrackIndex == 0 ||
				lFormat == 1 && (lKind & 0xF0) != 0x50 && lTargetTrackIndex != 0 ||
				lFormat == 2) {
	
				// �g���b�N���Ȃ��ꍇ�A�g���b�N�̒ǉ��y��EOT�̒ǉ�
				MIDITrack* pNewTrack = NULL;
				MIDIEvent* pNewEvent = NULL;
				long lTrackCount = MIDIData_CountTrack (m_pMIDIData);
				for (i = lTrackCount; i <= lTargetTrackIndex; i++) { 
					VERIFY (pNewTrack = MIDITrack_Create ());
					VERIFY (MIDITrack_SetForeColor (pNewTrack, pSekaijuApp->m_lDefTrackColor[i % 16]));
					VERIFY (MIDITrack_SetInputOn (pNewTrack, TRUE)); 
					VERIFY (MIDITrack_SetInputChannel (pNewTrack, (i + 15) % 16)); 
					VERIFY (MIDITrack_SetOutputOn (pNewTrack, TRUE)); 
					VERIFY (MIDITrack_SetOutputChannel (pNewTrack, (i + 15) % 16)); 
					VERIFY (MIDIData_AddTrack (m_pMIDIData, pNewTrack));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pNewTrack));
					VERIFY (pNewEvent = MIDIEvent_CreateEndofTrack (lTimeResolution * 4));
					VERIFY (MIDITrack_InsertEvent (pNewTrack, pNewEvent));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
				}
				if (pNewEvent) {
					VERIFY (this->SelectEvent (pNewEvent, TRUE, pCurHistoryUnit));
				}

				// ���݃g���b�N���珜�����ړ���g���b�N�ɑ}��
				VERIFY (pTargetTrack = this->GetTrack (lTargetTrackIndex));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = this->ReplaceMIDIEvent (pMIDIEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pCloneEvent));
				VERIFY (MIDITrack_InsertEvent (pTargetTrack, pCloneEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				// ����: EndofTrack�C�x���g��Remove���邱�Ƃ͂ł��邪,��d��Insert���邱�Ƃ͂ł��Ȃ��B
				// TODO: EndofTrack�C�x���g�̐e�ׯ����Ȃ��Ȃ��Ă��܂��o�O���܂�����B
			}
			else {
				lErrorCount++;
			}
		}
		
		theTempSelectedEventArray.RemoveAll ();
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		if (lErrorCount > 0) {
			CString strMsg;
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_MIDIEVENTS_TRACKINDEX_ISNT_CHANGED));
			strMsg.Format (strFormat, lErrorCount);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		}
		pSekaijuApp->m_theEditTrackDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditTrackDlgStatus.m_nUnit = theDlg.m_nUnit;
	}

}

// �w�ҏW(E)�x-�w�g���b�N�̕ύX...�x
void CSekaijuDoc::OnUpdateEditTrackUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	long lFormat = MIDIData_GetFormat (m_pMIDIData);
	if (lFormat == 0) {
		pCmdUI->Enable (FALSE);
	}
	else {
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (this->IsEventSelected (pMIDIEvent)) {
					lEnable = 1;
					break;
				}
			}
			if (lEnable == 1) {
				break;
			}
		}
		pCmdUI->Enable (lEnable);
	}
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}



// �w�ҏW(E)�x-�w�^�C���̕ύX...�x
void CSekaijuDoc::OnEditTime () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);

	// TPQN�x�[�X
	if (lTimeMode == 0) {
		CEditTimeDlg theDlg;
		theDlg.m_nAmount = pSekaijuApp->m_theEditTimeDlgStatus.m_nAmount;
		theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditTimeDlgStatus.m_nUnit, 3);
		if (theDlg.DoModal () == IDOK) {
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			MIDITrack* pMIDITrack = NULL;
			MIDIEvent* pMIDIEvent = NULL;
			MIDIEvent* pCloneEvent = NULL;
			CHistoryUnit* pCurHistoryUnit = NULL;
			CString strHistoryName;
			VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_TIME));
			VERIFY (this->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
			
			// �I������Ă���C�x���g�̂����A�ŏ��̃C�x���g�̎��������߂�
			long lFirstTime = 0x7FFFFFFF;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (this->IsEventSelected (pMIDIEvent)) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lTime < lFirstTime) {
							lFirstTime = lTime;
							break;
						}
					}
				}
			}
			long lnn, ldd, lbb, lcc;
			MIDIData_FindTimeSignature (m_pMIDIData, lFirstTime, &lnn, &ldd, &lbb, &lcc);
			// �����C�x���g��2��ړ����Ȃ��悤�ɁA�I������Ă���C�x���g���ꎞ�z��ɓo�^����B
			CPtrArray theTempSelectedEventArray;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				pMIDITrack->m_lUser1 = 0;
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// �I������Ă���C�x���g�̂�
					if (this->IsEventSelected (pMIDIEvent)) {
						// �񌋍��C�x���g�̂݁A�����C�x���g�̏ꍇ�͑O�񌋍��̂�
						if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
							!MIDIEvent_IsCombined (pMIDIEvent)) {
							theTempSelectedEventArray.Add (pMIDIEvent);
							pMIDITrack->m_lUser1++;
						}
					}
				}
			}
			// EOT�̗���ێ�(EOT���I������Ă��Ȃ��ꍇ�̂�)(20081102�C��)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
					}
				}
			}
			// �����̕ύX
			// �ꎞ�z��̃C�x���g1�ɂ�1��̈ړ���������B
			long lTempSelectedEventArrayCount = theTempSelectedEventArray.GetSize ();
			if (theDlg.m_nAmount < 0) {
				// (EOT���Ō�Ɉړ������邽�߁A������)(20081102�C��)
				for (long j = 0; j < lTempSelectedEventArrayCount; j++) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 / (1 << ldd); // (20081110�v�Z���C��)
					}
					else if (theDlg.m_nUnit == 2) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 * lnn / (1 << ldd);
					}
					else if (theDlg.m_nUnit == 3) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// �m�[�g�I�t�Ɍ��������m�[�g�I���C�x���g�̏ꍇ�́A���������ύX����
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			else {
				// (EOT���ŏ��Ɉړ������邽�߁A�t����)(20081102�C��)
				for (long j = lTempSelectedEventArrayCount - 1; j >= 0; j--) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 / (1 << ldd); // (20081110�v�Z���C��)
					}
					else if (theDlg.m_nUnit == 2) {
						lTime += theDlg.m_nAmount * lTimeResolution * 4 * lnn / (1 << ldd);
					}
					else if (theDlg.m_nUnit == 3) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// �m�[�g�I�t�Ɍ��������m�[�g�I���C�x���g�̏ꍇ�́A���������ύX����
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			theTempSelectedEventArray.RemoveAll ();
			// EOT�̗���ێ�(EOT���I������Ă��Ȃ��ꍇ�̂�)(20081102�C��)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
					}
				}
				pMIDITrack->m_lUser1 = 0;
			}
			this->SetModifiedFlag (TRUE);
			this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			pSekaijuApp->m_theEditTimeDlgStatus.m_nAmount = theDlg.m_nAmount;
			pSekaijuApp->m_theEditTimeDlgStatus.m_nUnit = theDlg.m_nUnit;
		}
	}

	// SMPTE�x�[�X
	else {
		CEditTimeSmpDlg theDlg;
		theDlg.m_nAmount = pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nAmount;
		theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nUnit, 2);
		if (theDlg.DoModal () == IDOK) {
			BeginWaitCursor ();
			m_theCriticalSection.Lock ();
			MIDITrack* pMIDITrack = NULL;
			MIDIEvent* pMIDIEvent = NULL;
			MIDIEvent* pCloneEvent = NULL;
			CHistoryUnit* pCurHistoryUnit = NULL;
			CString strHistoryName;
			VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_TIME));
			VERIFY (this->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
			
			// �I������Ă���C�x���g�̂����A�ŏ��̃C�x���g�̎��������߂�
			long lFirstTime = 0x7FFFFFFF;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (this->IsEventSelected (pMIDIEvent)) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lTime < lFirstTime) {
							lFirstTime = lTime;
							break;
						}
					}
				}
			}
			long lTimeMode, lTimeResolution;
			MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
			long lnn, ldd, lbb, lcc;
			MIDIData_FindTimeSignature (m_pMIDIData, lFirstTime, &lnn, &ldd, &lbb, &lcc);
			// �����C�x���g��2��ړ����Ȃ��悤�ɁA�I������Ă���C�x���g���ꎞ�z��ɓo�^����B
			CPtrArray theTempSelectedEventArray;
			forEachTrack (m_pMIDIData, pMIDITrack) {
				pMIDITrack->m_lUser1 = 0;
				forEachEvent (pMIDITrack, pMIDIEvent) {
					// �I������Ă���C�x���g�̂�
					if (this->IsEventSelected (pMIDIEvent)) {
						// �񌋍��C�x���g�̂݁A�����C�x���g�̏ꍇ�͑O�񌋍��̂�
						if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
							!MIDIEvent_IsCombined (pMIDIEvent)) {
							theTempSelectedEventArray.Add (pMIDIEvent);
							pMIDITrack->m_lUser1++;
						}
					}
				}
			}
			// EOT�̗���ێ�(EOT���I������Ă��Ȃ��ꍇ�̂�)(20081102�C��)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
					}
				}
			}
			// �ꎞ�z��̃C�x���g1�ɂ�1��̈ړ���������B
			long lTempSelectedEventArrayCount = theTempSelectedEventArray.GetSize ();
			if (theDlg.m_nAmount < 0) {
				// (EOT���Ō�Ɉړ������邽�߁A������)(20081102�C��)
				for (long j = 0; j < lTempSelectedEventArrayCount; j++) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution;
					}
					else if (theDlg.m_nUnit == 2) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// �m�[�g�I�t�Ɍ��������m�[�g�I���C�x���g�̏ꍇ�́A���������ύX����
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			else {
				// (EOT���ŏ��Ɉړ������邽�߁A�t����)(20081102�C��)
				for (long j = lTempSelectedEventArrayCount - 1; j >= 0; j--) {
					pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
					long lTime = MIDIEvent_GetTime (pCloneEvent);
					if (theDlg.m_nUnit == 0) {
						lTime += theDlg.m_nAmount;
					}
					else if (theDlg.m_nUnit == 1) {
						lTime += theDlg.m_nAmount * lTimeResolution;
					}
					else if (theDlg.m_nUnit == 2) {
						lTime = lFirstTime + (lTime - lFirstTime) * theDlg.m_nAmount / 100;
						// �m�[�g�I�t�Ɍ��������m�[�g�I���C�x���g�̏ꍇ�́A���������ύX����
						if (MIDIEvent_IsNoteOn (pCloneEvent) && MIDIEvent_IsCombined (pCloneEvent)) {
							long lDuration = MIDIEvent_GetDuration (pCloneEvent);
							lDuration = lDuration * theDlg.m_nAmount / 100;
							lDuration = CLIP (1, lDuration, 65535);
							MIDIEvent_SetDuration (pCloneEvent, lDuration);
						}
					}
					lTime = CLIP (0, lTime, 0x7FFFFFFF);
					VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pMIDIEvent = pCloneEvent;
				}
			}
			theTempSelectedEventArray.RemoveAll ();
			// EOT�̗���ێ�(EOT���I������Ă��Ȃ��ꍇ�̂�)(20081102�C��)
			forEachTrack (m_pMIDIData, pMIDITrack) {
				MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
				if (pLastEvent && pMIDITrack->m_lUser1 > 0) {
					if (MIDIEvent_IsEndofTrack (pLastEvent) && !this->IsEventSelected (pLastEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
					}
				}
				pMIDITrack->m_lUser1 = 0;
			}
			this->SetModifiedFlag (TRUE);
			this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			m_theCriticalSection.Unlock ();
			EndWaitCursor ();
			::Sleep (0);
			pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nAmount = theDlg.m_nAmount;
			pSekaijuApp->m_theEditTimeSmpDlgStatus.m_nUnit = theDlg.m_nUnit;
		}
	}
}

// �w�ҏW(E)�x-�w�^�C���̕ύX...�x
void CSekaijuDoc::OnUpdateEditTimeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�`�����l���̕ύX...�x
void CSekaijuDoc::OnEditChannel () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	CEditChannelDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditChannelDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditChannelDlgStatus.m_nUnit, 2);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		long lTrackOutputChannel = 0;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_CHANNEL));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// �`�����l���̕ύX
		forEachTrack (m_pMIDIData, pMIDITrack) {
			lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// MIDI�`�����l���C�x���g
					// (�m�[�g�I�t�E�m�[�g�I���E�L�[�A�t�^�[�E�R���g���[���`�F���W�E
					// �v���O�����`�F���W�E�`�����l���A�t�^�[�E�s�b�`�x���h)�̂�
					if (MIDIEvent_IsMIDIEvent (pMIDIEvent) &&
						pMIDIEvent->m_pPrevCombinedEvent == NULL) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lChannel = MIDIEvent_GetChannel (pCloneEvent);
						if (theDlg.m_nUnit == 0) {
							if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
								lChannel = lTrackOutputChannel;
							}
						}
						else if (theDlg.m_nUnit == 1) {
							lChannel = theDlg.m_nAmount - 1;
						}
						else if (theDlg.m_nUnit == 2) {
							lChannel += theDlg.m_nAmount;
						}
						lChannel = CLIP (0, lChannel, 15);
						VERIFY (MIDIEvent_SetChannel (pCloneEvent, lChannel));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditChannelDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditChannelDlgStatus.m_nUnit = theDlg.m_nUnit;
	}
}

// �w�ҏW(E)�x-�w�`�����l���̕ύX...�x
void CSekaijuDoc::OnUpdateEditChannelUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�����̕ύX...�x
void CSekaijuDoc::OnEditKey () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	
	CEditKeyDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditKeyDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditKeyDlgStatus.m_nUnit, 1);
	theDlg.m_nTargetNote = CLIP (0, pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetNote, 1);
	theDlg.m_nTargetKeyAfter = CLIP (0, pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetKeyAfter, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_KEY));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// �����̕ύX
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// �m�[�g�I���E�m�[�g�I�t(�񌋍�)�E�L�[�A�t�^�[�^�b�`�̂�
					if ((MIDIEvent_IsNoteOn (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						MIDIEvent_IsNoteOff (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL) &&
						theDlg.m_nTargetNote == 1 ||
						MIDIEvent_IsKeyAftertouch (pMIDIEvent) &&
						theDlg.m_nTargetKeyAfter == 1) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lKey = MIDIEvent_GetKey (pCloneEvent);
						if (theDlg.m_nUnit == 0) {
							lKey += theDlg.m_nAmount;
						}
						else {
							lKey += theDlg.m_nAmount * 12;
						}
						lKey = CLIP (0, lKey, 127);
						VERIFY (MIDIEvent_SetKey (pCloneEvent, lKey));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditKeyDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditKeyDlgStatus.m_nUnit = theDlg.m_nUnit;
		pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetNote = theDlg.m_nTargetNote;
		pSekaijuApp->m_theEditKeyDlgStatus.m_nTargetKeyAfter = theDlg.m_nTargetKeyAfter;
	}
}

// �w�ҏW(E)�x-�w�����̕ύX...�x
void CSekaijuDoc::OnUpdateEditKeyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �w�ҏW(E)�x-�w�x���V�e�B�̕ύX...�x
void CSekaijuDoc::OnEditVelocity () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}
	
	CEditVelocityDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditVelocityDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditVelocityDlgStatus.m_nUnit, 2);
	theDlg.m_nTargetNoteOn = CLIP (0, pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOn, 1);
	theDlg.m_nTargetNoteOff = CLIP (0, pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOff, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_VELOCITY));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// �x���V�e�B�̕ύX
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// �m�[�g�I��(�O�񌋍�)�E�m�[�g�I�t(�O�񌋍�)�̏ꍇ
					if (MIDIEvent_IsNoteOn (pMIDIEvent) && 
						pMIDIEvent->m_pPrevCombinedEvent == NULL && theDlg.m_nTargetNoteOn ||
						MIDIEvent_IsNoteOff (pMIDIEvent) && 
						pMIDIEvent->m_pPrevCombinedEvent == NULL && theDlg.m_nTargetNoteOff) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent* pTempEvent = pCloneEvent;
						while (pTempEvent) {
							if (MIDIEvent_IsNoteOn (pTempEvent) && theDlg.m_nTargetNoteOn ||
								MIDIEvent_IsNoteOff (pTempEvent) && theDlg.m_nTargetNoteOff) {
								long lVelocity = MIDIEvent_GetVelocity (pTempEvent);
								if (theDlg.m_nUnit == 0) {
									lVelocity = theDlg.m_nAmount;
								}
								else if (theDlg.m_nUnit == 1) {
									lVelocity += theDlg.m_nAmount;
								}
								else {
									lVelocity = lVelocity * theDlg.m_nAmount / 100;
								}
								lVelocity = CLIP (0, lVelocity, 127);
								MIDIEvent_SetVelocity (pTempEvent, lVelocity);
							}
							pTempEvent = pTempEvent->m_pNextCombinedEvent;
						}
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nUnit = theDlg.m_nUnit;
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOn = theDlg.m_nTargetNoteOn;
		pSekaijuApp->m_theEditVelocityDlgStatus.m_nTargetNoteOff = theDlg.m_nTargetNoteOff;
	}
}

// �w�ҏW(E)�x-�w�x���V�e�B�̕ύX...�x
void CSekaijuDoc::OnUpdateEditVelocityUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�������̕ύX...�x
void CSekaijuDoc::OnEditDuration () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}

	CEditDurationDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditDurationDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditDurationDlgStatus.m_nUnit, 2);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_DURATION));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		// ���̒����ύX
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// �m�[�g�I�t�Ɍ������ꂽ�m�[�g�I���C�x���g�̂�
					if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsCombined (pMIDIEvent)) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lDuration = MIDIEvent_GetDuration (pCloneEvent);
						if (theDlg.m_nUnit == 0) {
							lDuration = theDlg.m_nAmount;
						}
						else if (theDlg.m_nUnit == 1) {
							lDuration += theDlg.m_nAmount;
						}
						else {
							lDuration = lDuration * theDlg.m_nAmount / 100;
						}
						lDuration = CLIP (1, lDuration, 65535);
						VERIFY (MIDIEvent_SetDuration (pCloneEvent, lDuration));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditDurationDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditDurationDlgStatus.m_nUnit = theDlg.m_nUnit;
	}
}

// �w�ҏW(E)�x-�w�������̕ύX...�x
void CSekaijuDoc::OnUpdateEditDurationUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�l�̕ύX...�x
void CSekaijuDoc::OnEditValue () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}	
	if (m_bEditLocked) {
		return;
	}
	
	CEditValueDlg theDlg;
	theDlg.m_nAmount = pSekaijuApp->m_theEditValueDlgStatus.m_nAmount;
	theDlg.m_nUnit = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nUnit, 2);
	theDlg.m_nTargetKeyAfter = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetKeyAfter, 1);
	theDlg.m_nTargetControlChange = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetControlChange, 1);
	theDlg.m_nTargetChannelAfter = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetChannelAfter, 1);
	theDlg.m_nTargetPitchBend = CLIP (0, pSekaijuApp->m_theEditValueDlgStatus.m_nTargetPitchBend, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_MODIFY_VALUE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		// �l�̕ύX
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// �L�[�A�t�^�[�^�b�`�E�R���g���[���`�F���W�E�`�����l���A�t�^�[�^�b�`
					if (MIDIEvent_IsKeyAftertouch (pMIDIEvent) && theDlg.m_nTargetKeyAfter ||
						MIDIEvent_IsControlChange (pMIDIEvent) && theDlg.m_nTargetControlChange ||
						MIDIEvent_IsChannelAftertouch (pMIDIEvent) && theDlg.m_nTargetChannelAfter) {	
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						if (theDlg.m_nUnit == 0) {
							lValue = theDlg.m_nAmount;
						}
						if (theDlg.m_nUnit == 1) {
							lValue += theDlg.m_nAmount;
						}
						else {
							lValue = lValue * theDlg.m_nAmount / 100;
						}
						lValue = CLIP (0, lValue, 127);
						VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
					// �s�b�`�x���h
					else if (MIDIEvent_IsPitchBend (pMIDIEvent) && theDlg.m_nTargetPitchBend) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						long lValue = MIDIEvent_GetValue (pMIDIEvent);
						if (theDlg.m_nUnit == 0) {
							lValue = theDlg.m_nAmount;
						}
						if (theDlg.m_nUnit == 1) {
							lValue += theDlg.m_nAmount;
						}
						else {
							lValue = 8192 + (lValue - 8192) * theDlg.m_nAmount / 100;
						}
						lValue = CLIP (0, lValue, 16383);
						VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
					}
				}
			}
		}
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditValueDlgStatus.m_nAmount = theDlg.m_nAmount;
		pSekaijuApp->m_theEditValueDlgStatus.m_nUnit = theDlg.m_nUnit;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetKeyAfter = theDlg.m_nTargetKeyAfter;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetControlChange = theDlg.m_nTargetControlChange;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetChannelAfter = theDlg.m_nTargetChannelAfter;
		pSekaijuApp->m_theEditValueDlgStatus.m_nTargetPitchBend = theDlg.m_nTargetPitchBend;
	}
}

// �w�ҏW(E)�x-�w�l�̕ύX...�x
void CSekaijuDoc::OnUpdateEditValueUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�N�H���^�C�Y...�x
void CSekaijuDoc::OnEditQuantize () {

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	

	CEditQuantizeDlg theDlg;
	theDlg.m_lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);
	theDlg.m_lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	theDlg.m_nSnapTimeIndex = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nSnapTimeIndex, 6);
	theDlg.m_nStrength = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nStrength, 100);
	theDlg.m_nTargetNoteOn = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOn, 1);
	theDlg.m_nTargetNoteOff = CLIP (0, pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOff, 1);
	if (theDlg.DoModal () == IDOK) {
		BeginWaitCursor ();
		long lDivision[7] = {1, 2, 3, 4, 6, 8, 12};
		long lSnapTimeIndex = CLIP (0, theDlg.m_nSnapTimeIndex, 6);
		long lSnapTime = __max (1, MIDIData_GetTimeResolution (m_pMIDIData) / lDivision[lSnapTimeIndex]);
		m_theCriticalSection.Lock ();
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_QUANTIZE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}

		// �����C�x���g��2��ړ����Ȃ��悤�ɁA�I������Ă���C�x���g���ꎞ�z��ɓo�^����B
		CPtrArray theTempSelectedEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// �I������Ă���C�x���g�̂�
				if (this->IsEventSelected (pMIDIEvent)) {
					// �m�[�g�I�t�Ɍ������ꂽ�m�[�g�I���C�x���g�̂�
					if (MIDIEvent_IsNoteOn (pMIDIEvent) && MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempSelectedEventArray.Add (pMIDIEvent);
					}
				}
			}
		}

		// �ꎞ�z��̃C�x���g1�ɂ�1��̈ړ���������B
		long lTempSelectedEventArrayCount = theTempSelectedEventArray.GetSize ();
		for (long j = 0; j < lTempSelectedEventArrayCount; j++) {
			pMIDIEvent = (MIDIEvent*)(theTempSelectedEventArray.GetAt (j));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));

			// �����̊J�n�ʒu�ړ�(�������͕ێ������)
			if (theDlg.m_nTargetNoteOn) {
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				long lNewTime = (lTime + lSnapTime / 2) / lSnapTime * lSnapTime;
				long lDeltaTime = lNewTime - lTime;
				long lStDeltaTime = lDeltaTime * theDlg.m_nStrength / 100;
				long lStNewTime = CLIP (0, lTime + lStDeltaTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lStNewTime));
			}

			// �����̏I���ʒu�ړ�(��������ύX���邱�Ƃɂ��)
			if (theDlg.m_nTargetNoteOff) {
				long lBeginTime = MIDIEvent_GetTime (pCloneEvent);
				long lDuration = MIDIEvent_GetDuration (pCloneEvent);
				long lEndTime = lBeginTime + lDuration;
				long lNewEndTime = (lEndTime + lSnapTime / 2) / lSnapTime * lSnapTime;
				long lDeltaTime = lNewEndTime - lEndTime;
				long lStDeltaTime = lDeltaTime * theDlg.m_nStrength / 100;
				long lStNewEndTime = CLIP (0, lEndTime + lStDeltaTime, 0x7FFFFFFF);
				long lStNewDuration = lStNewEndTime - lBeginTime;
				while (lStNewDuration <= 1) {
					lStNewDuration += lSnapTime;
				}
				VERIFY (MIDIEvent_SetDuration (pCloneEvent, lStNewDuration));
			}
	
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pMIDIEvent = pCloneEvent;
		}
		theTempSelectedEventArray.RemoveAll ();

		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}

		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nSnapTimeIndex = theDlg.m_nSnapTimeIndex;
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nStrength = theDlg.m_nStrength;
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOn = theDlg.m_nTargetNoteOn;
		pSekaijuApp->m_theEditQuantizeDlgStatus.m_nTargetNoteOff = theDlg.m_nTargetNoteOff;
	}
}



// �w�ҏW(E)�x-�w�N�H���^�C�Y...�x
void CSekaijuDoc::OnUpdateEditQuantizeUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�ҏW(E)�x-�w�����̍ו����ƃg������...�x
void CSekaijuDoc::OnEditBreakupAndTrill () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	CEditBreakupAndTrillDlg theDlg;
	theDlg.m_lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);
	theDlg.m_lTimeResolution = MIDIData_GetTimeResolution (m_pMIDIData);
	theDlg.m_nDurationIndex = pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex;
	theDlg.m_nEnableTrill = pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill;
	theDlg.m_nKeyShift = pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nKeyShift;

	
	if (theDlg.DoModal () == IDOK) {
		pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex = theDlg.m_nDurationIndex;
		pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill = theDlg.m_nEnableTrill;
		pSekaijuApp->m_theEditBreakupAndTrillDlgStatus.m_nKeyShift = theDlg.m_nKeyShift;

		BeginWaitCursor ();
		m_theCriticalSection.Lock ();

		// �������1���̒���[tick]���v�Z
		long lDurationDiv[7] = {1, 2, 3, 4, 6, 8, 12};
		long lDuration = theDlg.m_lTimeResolution / lDurationDiv[theDlg.m_nDurationIndex];
		lDuration = CLIP (1, lDuration, 65535);
		long lKeyShift = CLIP (-127, theDlg.m_nKeyShift, 127);
		
		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_BREAKUP_AND_TRILL));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		MIDIEvent* pCloneEvent = NULL;
		long i = 0;
		long j = 0;
		long lDeletedNoteEventCount = 0;
		long lInsertedNoteEventCount = 0;
		
		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		
		// �z��ɓo�^���ꂽ�C�x���g���폜
		forEachTrack (m_pMIDIData, pMIDITrack) {
			// �I������Ă���m�[�g�I���C�x���g��z��ɓo�^
			CPtrArray theEventArray;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (this->IsEventSelected (pMIDIEvent)) {
					if (MIDIEvent_IsNote (pMIDIEvent) && MIDIEvent_IsNoteOn (pMIDIEvent)) {
						theEventArray.Add (pMIDIEvent);
					}
				}
			}
			// ���ꂼ��̑I������Ă���m�[�g�C�x���g�ɂ���
			for (i = 0; i < theEventArray.GetSize (); i++) {
				VERIFY (pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (i));
				ASSERT (MIDIEvent_IsNote (pMIDIEvent) && MIDIEvent_IsNoteOn (pMIDIEvent));
				long lOldTime = MIDIEvent_GetTime (pMIDIEvent);
				long lOldChannel = MIDIEvent_GetChannel (pMIDIEvent);
				long lOldDuration = MIDIEvent_GetDuration (pMIDIEvent);
				long lOldKey = MIDIEvent_GetKey (pMIDIEvent);
				long lOldVelocity = MIDIEvent_GetVelocity (pMIDIEvent);
				// �����̃m�[�g�C�x���g���폜
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
				lDeletedNoteEventCount++;
				// �V�����m�[�g�C�x���g��}��
				for (j = 0; j < 65536; j++) {
					long lNewTime = lOldTime + lDuration * j;
					if (lNewTime >= lOldTime + lOldDuration) {
						break;
					}
					long lNewDuration = lDuration;
					if (lNewTime + lNewDuration >= lOldTime + lOldDuration) {
						lNewDuration = lOldTime + lOldDuration - lNewTime;
						ASSERT (lNewDuration > 0);
					}
					long lNewKey = lOldKey;
					if (theDlg.m_nEnableTrill) {
						lNewKey = (j % 2 == 0 ? lOldKey : lOldKey + lKeyShift);
					}
					lNewKey = CLIP (1, lNewKey, 127);
					long lNewVelocity = lOldVelocity;
					long lNewChannel = lOldChannel;
					// �V�����m�[�g�C�x���g���쐬
					MIDIEvent* pNewEvent = MIDIEvent_CreateNote 
						(lNewTime, lNewChannel, lNewKey, lNewVelocity, lNewDuration);
					ASSERT (pNewEvent);
					// �����C�x���g��擪���珇�ɑI����Ԃɂ���B
					MIDIEvent* pTempEvent = pNewEvent;
					while (pTempEvent) {
						pTempEvent->m_lUserFlag |= MIDIEVENT_SELECTED;
						pTempEvent = pTempEvent->m_pNextCombinedEvent;
					}
					// �g���b�N�ɑ}���A�����L�^
					VERIFY (MIDITrack_InsertEvent (pMIDITrack, pNewEvent));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pNewEvent));
					lInsertedNoteEventCount++;
				}
			}
			theEventArray.RemoveAll ();
		}

		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}

		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);

	}
}

// �w�ҏW(E)�x-�w�����̍ו����ƃg������...�x
void CSekaijuDoc::OnUpdateEditBreakupAndTrillUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}



// �w�ҏW(E)�x-�w�r�[�g���o�ƃe���|�����}��...�x
void CSekaijuDoc::OnEditBeatScan () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	CString strMsg;
	
	// �t�H�[�}�b�g�`�F�b�N
	long lFormat = MIDIData_GetFormat (m_pMIDIData);
	if (lFormat == 0) {
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_BEATSCAN_WITH_FORMAT0_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// �^�C���x�[�X�`�F�b�N
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (m_pMIDIData, &lTimeMode, &lTimeResolution);
	long lTrackCount = MIDIData_CountTrack (m_pMIDIData);
	if (lTimeMode != MIDIDATA_TPQNBASE) {
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_BEATSCAN_WITH_SMPTEBASE_MIDIDATA));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// �e�g���b�N�̃g���b�N�����_�C�A���O�N���X�̔z��ɑ��
	long i = 0;
	long j = 0;
	TCHAR szTrackName[256];
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CEditBeatScanDlg theDlg;
	theDlg.m_lTimeMode = lTimeMode;
	theDlg.m_lTimeResolution = lTimeResolution;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		memset (szTrackName, 0, sizeof (szTrackName));
		MIDITrack_GetName (pMIDITrack, szTrackName, TSIZEOF (szTrackName));
		theDlg.m_theTrackNameArray.Add (szTrackName);
		i++;
	}

	theDlg.m_nBeatTrackIndex = pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatTrackIndex;
	theDlg.m_nBeatTrackIndex = CLIP (0, theDlg.m_nBeatTrackIndex, lTrackCount - 1);
	theDlg.m_nBeatIntervalIndex = pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex;
	theDlg.m_nInsertTempo = pSekaijuApp->m_theEditBeatScanDlgStatus.m_nInsertTempo;

	// �_�C�A���O�\��
	if (theDlg.DoModal () == IDOK) {

		pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatTrackIndex = theDlg.m_nBeatTrackIndex;
		pSekaijuApp->m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex = theDlg.m_nBeatIntervalIndex;
		pSekaijuApp->m_theEditBeatScanDlgStatus.m_nInsertTempo = theDlg.m_nInsertTempo;

		m_theCriticalSection.Lock ();
		CDWordArray theBeatTimeArray;
		CDWordArray theBeatMillisecArray;
		CDWordArray theTempoArray;

		// ������̃r�[�g�Ԋu[tick]���v�Z
		long lBeatDiv[7] = {1, 2, 3, 4, 6, 8, 12};
		long lBeatInterval = lTimeResolution / lBeatDiv[theDlg.m_nBeatIntervalIndex];
		
		// �I������Ă���e�r�[�g�̎���[tick]�ƃ~���b[msec]�𒊏o
		MIDITrack* pBeatTrack = this->GetTrack (theDlg.m_nBeatTrackIndex);
		ASSERT (pBeatTrack);
		long lOldTime = 0;
		long lNumBeatCount = 0;
		theBeatTimeArray.Add (0);
		theBeatMillisecArray.Add (0);
		lNumBeatCount++;
		forEachEvent (pBeatTrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					if (lTime > lOldTime) {
						theBeatTimeArray.Add (lTime);
						long lMillisec = MIDIData_TimeToMillisec (m_pMIDIData, lTime);
						theBeatMillisecArray.Add (lMillisec);
						lNumBeatCount++;
						lOldTime = lTime;
					}
				}
			}
		}
		ASSERT (lNumBeatCount == theBeatTimeArray.GetSize ());
		ASSERT (lNumBeatCount == theBeatMillisecArray.GetSize ());
		
		// �w��g���b�N���őI������Ă���r�[�g�̐������Ȃ�����ꍇ�͋p��
		if (lNumBeatCount <= 3) {
			m_theCriticalSection.Unlock ();
			VERIFY (strMsg.LoadString (IDS_SELECTED_NOTEEVENT_IS_TOO_FEW_IN_THE_SPECIFIED_TRACK));
			AfxMessageBox (strMsg);
			return;
		}

		// �r�[�g�X�L�����J�n����[tick]�ƃr�[�g�X�L�����I������[tick]�𒊏o
		long lBeginTime = theBeatTimeArray.GetAt (1);
		long lEndTime = theBeatTimeArray.GetAt (lNumBeatCount - 1);
		long lBeginMeasure, lBeginBeat, lBeginTick;
		MIDIData_BreakTime (m_pMIDIData, lBeginTime, &lBeginMeasure, &lBeginBeat, &lBeginTick);
		long lEndMeasure, lEndBeat, lEndTick;
		MIDIData_BreakTime (m_pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

		// �r�[�g�X�L���������J�n�m�F���b�Z�[�W
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_D_NOTEVENT_IS_DETECTED_IN_THE_SPECIFIED_TRACK_CONTINUE));
		strMsg.Format (strFormat, lNumBeatCount - 1,
			lBeginMeasure + 1, lBeginBeat + 1, lBeginTick,
			lEndMeasure + 1, lEndBeat + 1, lEndTick);
		m_theCriticalSection.Unlock ();
		long lRet = AfxMessageBox (strMsg, MB_ICONINFORMATION | MB_YESNOCANCEL);
		if (lRet == IDNO || lRet == IDCANCEL) {
			return;
		}
		m_theCriticalSection.Lock ();
		
		BeginWaitCursor ();
		
		// �e�r�[�g�ʒu�ɂ�����␳��e���|[�ʕb/4������]���v�Z
		long lTempo = 0;
		MIDIData_FindTempo (m_pMIDIData, 0, &lTempo);
		theTempoArray.Add (lTempo);
		for (j = 1; j < lNumBeatCount - 1; j++) {
			lTempo = 
				(theBeatMillisecArray.GetAt (j + 1) - theBeatMillisecArray.GetAt (j)) *
				1000 * lTimeResolution / lBeatInterval;
			theTempoArray.Add (lTempo);
		}
		MIDIData_FindTempo (m_pMIDIData, lEndTime, &lTempo);
		theTempoArray.Add (lTempo);
		ASSERT (lNumBeatCount == theTempoArray.GetSize ());

		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_BEATSCAN));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (pCloneEvent = ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		
		// �����̃e���|�C�x���g�̍폜
		long lDeletedTempoEventCount = 0;
		if (theDlg.m_nInsertTempo) {
			forEachTrack (m_pMIDIData, pMIDITrack) {
				// lBeginTime�`lEndTime�ɂ���e���|�C�x���g��z��ɓo�^
				CPtrArray theEventArray;
				forEachEvent (pMIDITrack, pMIDIEvent) {
					if (MIDIEvent_IsTempo (pMIDIEvent)) {
						long lTime = MIDIEvent_GetTime (pMIDIEvent);
						if (lBeginTime <= lTime && lTime < lEndTime) {
							theEventArray.Add (pMIDIEvent);
						}
					}
				}
				// �z��ɓo�^���ꂽ�C�x���g���폜
				for (i = 0; i < theEventArray.GetSize (); i++) {
					VERIFY (pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (i));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
					lDeletedTempoEventCount++;
				}
				theEventArray.RemoveAll ();
			}
		}

		// MIDI�C�x���g�̎����ړ�
		long lMovedEventCount = 0;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			j = 0;
			// lBeginTime�ȍ~�ɂ���C�x���g��z��ɓo�^
			// (�����C�x���g�̏ꍇ�͍ŏ��̃C�x���g�̂�)
			// (�G���h�I�u�g���b�N�C�x���g�͎����ړ�����̂ŏ���)
			CPtrArray theEventArray;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime >= lBeginTime) {
					if (pMIDIEvent->m_pPrevCombinedEvent == NULL &&
						!MIDIEvent_IsEndofTrack (pMIDIEvent)) {
						theEventArray.Add (pMIDIEvent);
					}
				}
			}
			// �z��ɓo�^���ꂽ�C�x���g���ʒu�ړ�
			for (i = 0; i < theEventArray.GetSize (); i++) { 
				VERIFY (pMIDIEvent = (MIDIEvent*)theEventArray.GetAt (i));
				ASSERT (pMIDIEvent->m_pPrevCombinedEvent == NULL);
				ASSERT (MIDIEvent_GetParent (pMIDIEvent) == pMIDITrack);
				// �m�[�g�C�x���g(�m�[�g�I��+�m�[�g�I�t)�̏ꍇ
				if (MIDIEvent_IsNote (pMIDIEvent)) {
					long lTime1 = MIDIEvent_GetTime (pMIDIEvent);
					long lDuration = MIDIEvent_GetDuration (pMIDIEvent);
					long lTime2 = lTime1 + lDuration;
					while (j < lNumBeatCount - 1) {
						if ((long)theBeatTimeArray.GetAt (j) <= lTime1 &&
							lTime1 < (long)theBeatTimeArray.GetAt (j + 1)) {
							break;
						}
						j++;
					}
					long k = j;
					while (k < lNumBeatCount - 1) {
						if ((long)theBeatTimeArray.GetAt (k) <= lTime2 &&
							lTime2 < (long)theBeatTimeArray.GetAt (k + 1)) {
							break;
						}
						k++;
					}
					long lNewTime1 = 0;
					long lNewTime2 = 0;
					// �m�[�g�I���ʒu�ړ�
					// �J�n����or�ŏ��̃r�[�g�������O�ɂ���C�x���g
					if (j == 0) {
						ASSERT (0 <= lTime1 && lTime1 < lBeginTime);
						ASSERT (FALSE);
					}
					// �r�[�g����(j)�ƃr�[�g����(j+1)�̊Ԃɂ���C�x���g
					else if (1 <= j && j < lNumBeatCount - 1) {
						ASSERT (lBeginTime <= lTime1 && lTime1 < lEndTime);
						lNewTime1 = 
							lBeginTime +
							lBeatInterval * (j - 1) +
							lBeatInterval *
							(lTime1 - theBeatTimeArray.GetAt (j)) /
							(theBeatTimeArray.GetAt (j + 1) - theBeatTimeArray.GetAt(j));
						lNewTime1 = CLIP (0, lNewTime1, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime1);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// �I������or�Ō�̃r�[�g��������ɂ���C�x���g
					else {
						ASSERT (lTime1 >= lEndTime);
						lNewTime1 =
							lBeginTime +
							lBeatInterval * (lNumBeatCount - 2) +
							lTime1 - theBeatTimeArray.GetAt (lNumBeatCount - 1);
						lNewTime1 = CLIP (0, lNewTime1, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime1);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// �m�[�g�I�t�ʒu�ړ�(�������̕ύX�ɂ��)
					// �J�n����or�ŏ��̃r�[�g�������O�ɂ���C�x���g
					if (k == 0) {
						ASSERT (0 <= lTime2 && lTime2 < lBeginTime);
						ASSERT (FALSE);
					}
					// �r�[�g����(k)�ƃr�[�g����(k+1)�̊Ԃɂ���C�x���g
					else if (1 <= k && k < lNumBeatCount - 1) {
						ASSERT (lBeginTime <= lTime2 && lTime2 < lEndTime);
						long lNewTime2 = 
							lBeginTime +
							lBeatInterval * (k - 1) +
							lBeatInterval *
							(lTime2 - theBeatTimeArray.GetAt (k)) /
							(theBeatTimeArray.GetAt (k + 1) - theBeatTimeArray.GetAt(k));
						lNewTime2 = CLIP (0, lNewTime2, 0x7FFFFFFF);
						long lNewDuration = lNewTime2 - lNewTime1;
						lNewDuration = CLIP (0, lNewDuration, 0xFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetDuration (pCloneEvent, lNewDuration);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// �I������or�Ō�̃r�[�g��������ɂ���C�x���g
					else {
						ASSERT (lTime2 >= lEndTime);
						lNewTime2 =
							lBeginTime +
							lBeatInterval * (lNumBeatCount - 2) +
							lTime2 - theBeatTimeArray.GetAt (lNumBeatCount - 1);
						lNewTime2 = CLIP (0, lNewTime2, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						long lNewDuration = lNewTime2 - lNewTime1;
						lNewDuration = CLIP (0, lNewDuration, 0xFFFF);
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetDuration (pCloneEvent, lNewDuration);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
				}
				// �m�[�g�C�x���g(�m�[�g�I��+�m�[�g�I�t)�ȊO�̏ꍇ
				else {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					while (j < lNumBeatCount - 1) {
						if ((long)theBeatTimeArray.GetAt (j) <= lTime &&
							lTime < (long)theBeatTimeArray.GetAt (j + 1)) {
							break;
						}
						j++;
					}
					// �J�n����or�ŏ��̃r�[�g�������O�ɂ���C�x���g
					if (j == 0) {
						ASSERT (0 <= lTime && lTime < lBeginTime);
						ASSERT (FALSE);
					}
					// �r�[�g����(j)�ƃr�[�g����(j+1)�̊Ԃɂ���C�x���g
					else if (1 <= j && j < lNumBeatCount - 1) {
						ASSERT (lBeginTime <= lTime && lTime < lEndTime);
						long lNewTime = 
							lBeginTime +
							lBeatInterval * (j - 1) +
							lBeatInterval *
							(lTime - theBeatTimeArray.GetAt (j)) /
							(theBeatTimeArray.GetAt (j + 1) - theBeatTimeArray.GetAt(j));
						lNewTime = CLIP (0, lNewTime, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
					// �I������or�Ō�̃r�[�g��������ɂ���C�x���g
					else {
						ASSERT (lTime >= lEndTime);
						long lNewTime =
							lBeginTime +
							lBeatInterval * (lNumBeatCount - 2) +
							lTime - theBeatTimeArray.GetAt (lNumBeatCount - 1);
						lNewTime = CLIP (0, lNewTime, 0x7FFFFFFF);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
						VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
						MIDIEvent_SetTime (pCloneEvent, lNewTime);
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
						pMIDIEvent = pCloneEvent;
						lMovedEventCount++;
					}
				}
			} // for
			theEventArray.RemoveAll ();
		} // forEachTrack

		// �e���|�C�x���g�̑}��
		long lInsertedTempoEventCount = 0;
		if (theDlg.m_nInsertTempo) {
			VERIFY (pMIDITrack = MIDIData_GetFirstTrack (m_pMIDIData));
			for (i = 1; i < theTempoArray.GetSize (); i++) {
				lTempo = theTempoArray.GetAt (i);
				lTempo = CLIP (1, lTempo, 60000000);
				long lTime = lBeginTime + lBeatInterval * (i - 1);
				VERIFY (pMIDIEvent = MIDIEvent_CreateTempo (lTime, lTempo));
				VERIFY (MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
				lInsertedTempoEventCount++;
			}
		}

		// EOT�̗���ێ�
		forEachTrack (m_pMIDIData, pMIDITrack) {
			MIDIEvent* pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent));
				}
			}
		}

		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// �w�ҏW(E)�x-�w�r�[�g���o�ƃe���|�����}��...�x
void CSekaijuDoc::OnUpdateEditBeatScanUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// ���ׂẴC�x���g����I���ł���΂��̍��ڂ��f�B�X�G�[�u������
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable == 1) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}


// �w�ҏW(E)�x-�w���߂�}��...�x(20100728�ǉ�)
void CSekaijuDoc::OnEditInsertMeasure () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);

	CEditInsertMeasureDlg theDlg;
	theDlg.m_nPosition = pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nPosition;
	theDlg.m_nNumMeasure = pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nNumMeasure;
	theDlg.m_bZeroOrigin = (lTimeMode == MIDIDATA_TPQNBASE ? 0 : 1);
	if (theDlg.DoModal () == IDOK) {
		pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nPosition = theDlg.m_nPosition;
		pSekaijuApp->m_theEditInsertMeasureDlgStatus.m_nNumMeasure = theDlg.m_nNumMeasure;

		BeginWaitCursor ();
		m_theCriticalSection.Lock ();

		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_INSERTMEASURE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		long lTime1, lTime2, lMeasureTime, lSlideTime;
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition - 1, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition , 0, 0, &lTime2);
		}
		else {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition + 1, 0, 0, &lTime2);
		}
		lMeasureTime = MAX (0, lTime2 - lTime1);
		lSlideTime = lMeasureTime * theDlg.m_nNumMeasure;
		// �㑱��MIDI�C�x���g�̎����ړ�
		// �����C�x���g��2��ړ����Ȃ��悤�ɁA�㑱�̃C�x���g���ꎞ�z��ɓo�^����B
		CPtrArray theTempFollowingEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUser1 = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// ���ߑ}���ʒu����̃C�x���g�̂�
				if (MIDIEvent_GetTime (pMIDIEvent) >= lTime1) {
					// �񌋍��C�x���g�̂݁A�����C�x���g�̏ꍇ�͑O�񌋍��̂�
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempFollowingEventArray.Add (pMIDIEvent);
						pMIDITrack->m_lUser1++;
					}
				}
			}
		}
		// �ꎞ�z��̃C�x���g1�ɂ�1��̈ړ���������B
		long lTempFollowingEventArrayCount = theTempFollowingEventArray.GetSize ();
		if (lSlideTime < 0) {
			// (EOT���Ō�Ɉړ������邽�߁A������)(20081102�C��)
			for (long j = 0; j < lTempFollowingEventArrayCount; j++) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		else {
			// (EOT���ŏ��Ɉړ������邽�߁A�t����)(20081102�C��)
			for (long j = lTempFollowingEventArrayCount - 1; j >= 0; j--) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		theTempFollowingEventArray.RemoveAll ();
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// �w�ҏW(E)�x-�w���߂�}��...�x(20100728�ǉ�)
void CSekaijuDoc::OnUpdateEditInsertMeasureUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�ҏW(E)�x-�w���߂�����...�x(20100728�ǉ�)
void CSekaijuDoc::OnEditRemoveMeasure () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lTimeMode = MIDIData_GetTimeMode (m_pMIDIData);

	CEditRemoveMeasureDlg theDlg;
	theDlg.m_nPosition = pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nPosition;
	theDlg.m_nNumMeasure = pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nNumMeasure;
	theDlg.m_bZeroOrigin = (lTimeMode == MIDIDATA_TPQNBASE ? 0 : 1);
	if (theDlg.DoModal () == IDOK) {
		pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nPosition = theDlg.m_nPosition;
		pSekaijuApp->m_theEditRemoveMeasureDlgStatus.m_nNumMeasure = theDlg.m_nNumMeasure;

		BeginWaitCursor ();
		m_theCriticalSection.Lock ();

		CHistoryUnit* pCurHistoryUnit = NULL;
		CString strHistoryName;
		VERIFY (strHistoryName.LoadString (IDS_EDIT_INSERTMEASURE));
		VERIFY (this->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
		
		long lTime1, lTime2, lSlideTime;
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition - 1, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition - 1 + theDlg.m_nNumMeasure, 0, 0, &lTime2);
		}
		else {
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition, 0, 0, &lTime1);
			MIDIData_MakeTime (m_pMIDIData, theDlg.m_nPosition + theDlg.m_nNumMeasure, 0, 0, &lTime2);
		}
		lSlideTime = MIN (0, lTime1 - lTime2);
		// lTime1�`lTime2�̃C�x���g�̍폜
		// �����C�x���g��2��ړ����Ȃ��悤�ɁAlTime1�`lTime2�̃C�x���g���ꎞ�z��ɓo�^����B
		CPtrArray theTempDuringEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUser1 = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// lTime1�`lTime2�̃C�x���g�̂�
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				if (lTime1 <= lTime && lTime < lTime2) {
					// �񌋍��C�x���g�̂݁A�����C�x���g�̏ꍇ�͑O�񌋍��̂�
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempDuringEventArray.Add (pMIDIEvent);
						pMIDITrack->m_lUser1++;
					}
				}
			}
		}
		// �ꎞ�z����̃C�x���g���폜(EOT������)
		long lTempDuringEventArrayCount = theTempDuringEventArray.GetSize ();
		for (long j = 0; j < lTempDuringEventArrayCount; j++) {
			pMIDIEvent = (MIDIEvent*)(theTempDuringEventArray.GetAt (j));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = CLIP (0, lTime1, 0x7FFFFFFF);
				MIDIEvent_SetTime (pCloneEvent, lTime);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
			else {
				VERIFY (MIDITrack_RemoveEvent (MIDIEvent_GetParent (pMIDIEvent), pMIDIEvent));
			}
		}
		theTempDuringEventArray.RemoveAll ();
		
		// �㑱��MIDI�C�x���g�̎����ړ�
		// �����C�x���g��2��ړ����Ȃ��悤�ɁA�㑱�̃C�x���g���ꎞ�z��ɓo�^����B
		CPtrArray theTempFollowingEventArray;
		forEachTrack (m_pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUser1 = 0;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				// ���ߑ}���ʒu����̃C�x���g�̂�
				if (MIDIEvent_GetTime (pMIDIEvent) >= lTime1) {
					// �񌋍��C�x���g�̂݁A�����C�x���g�̏ꍇ�͑O�񌋍��̂�
					if (MIDIEvent_IsCombined (pMIDIEvent) && pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						theTempFollowingEventArray.Add (pMIDIEvent);
						pMIDITrack->m_lUser1++;
					}
				}
			}
		}
		// �ꎞ�z��̃C�x���g1�ɂ�1��̈ړ���������B
		long lTempFollowingEventArrayCount = theTempFollowingEventArray.GetSize ();
		if (lSlideTime < 0) {
			// (EOT���Ō�Ɉړ������邽�߁A������)(20081102�C��)
			for (long j = 0; j < lTempFollowingEventArrayCount; j++) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		else {
			// (EOT���ŏ��Ɉړ������邽�߁A�t����)(20081102�C��)
			for (long j = lTempFollowingEventArrayCount - 1; j >= 0; j--) {
				pMIDIEvent = (MIDIEvent*)(theTempFollowingEventArray.GetAt (j));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = ReplaceMIDIEvent (pMIDIEvent));
				long lTime = MIDIEvent_GetTime (pCloneEvent);
				lTime += lSlideTime;
				lTime = CLIP (0, lTime, 0x7FFFFFFF);
				VERIFY (MIDIEvent_SetTime (pCloneEvent, lTime));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pMIDIEvent = pCloneEvent;
			}
		}
		theTempFollowingEventArray.RemoveAll ();
		this->SetModifiedFlag (TRUE);
		this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		m_theCriticalSection.Unlock ();
		EndWaitCursor ();
		::Sleep (0);
	}
}

// �w�ҏW(E)�x-�w���߂�����...�x(20100728�ǉ�)
void CSekaijuDoc::OnUpdateEditRemoveMeasureUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}



// �w�r���[(&V)�x-�w�ĕ\��(&R)�x
void CSekaijuDoc::OnViewRedraw () {
	//this->UpdateAllViews (NULL, 0xFFFFFFFF);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIDATACHANGED | 
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	// TODO:���2�̂ǂ��炪�ǂ����B
}

// �w�r���[(&V)�x-�w�V�K�g���b�N���X�g(&T)�x
void CSekaijuDoc::OnViewTrackList () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	m_theCriticalSection.Unlock ();
	ShowTrackListFrame ();
}

// �w�r���[(&V)�x-�w�V�K�s�A�m���[��(&P)�x
void CSekaijuDoc::OnViewPianoRoll () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	m_theCriticalSection.Unlock ();
	ShowPianoRollFrame ();
}

// �w�r���[(&V)�x-�w�V�K�C�x���g���X�g(&E)�x
void CSekaijuDoc::OnViewEventList () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	m_theCriticalSection.Unlock ();
	ShowEventListFrame ();
}

// �w�r���[(&V)�x-�w�V�K����(&S)�x
void CSekaijuDoc::OnViewMusicalScore () {
	m_theCriticalSection.Lock ();
	m_lTempTime = m_lNewTime;
	m_pTempTrack = NULL;
	m_pTempEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		pMIDITrack->m_lUserFlag |= MIDITRACK_VISIBLE;
	}
	m_theCriticalSection.Unlock ();
	ShowMusicalScoreFrame ();
}


// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂݃g���b�N���X�g�\��(&T)�x
void CSekaijuDoc::OnPopupShowTrackList () {
	if (m_pTempTrack != NULL) { // 20100429�ǉ�
		ShowTrackListFrame ();
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂݃g���b�N���X�g�\��(&T)�x // 20100429�ǉ�
void CSekaijuDoc::OnUpdatePopupShowTrackListUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂݃s�A�m���[���\��(&P)�x
void CSekaijuDoc::OnPopupShowPianoRoll () {
	if (m_pTempTrack != NULL) { // 20100429�ǉ�
		ShowPianoRollFrame ();
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂݃s�A�m���[���\��(&P)�x // 20100429�ǉ�
void CSekaijuDoc::OnUpdatePopupShowPianoRollUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂݃C�x���g���X�g�\��(&E)�x
void CSekaijuDoc::OnPopupShowEventList () {
	if (m_pTempTrack != NULL) { // 20100429�ǉ�
		ShowEventListFrame ();
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂݃C�x���g���X�g�\��(&E)�x // 20100429�ǉ�
void CSekaijuDoc::OnUpdatePopupShowEventListUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ��ʕ\��(&S)�x
void CSekaijuDoc::OnPopupShowMusicalScore () {
	if (m_pTempTrack != NULL) { // 20100429�ǉ�
		ShowMusicalScoreFrame ();
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݕ��ʕ\��(&S)�x // 20100429�ǉ�
void CSekaijuDoc::OnUpdatePopupShowMusicalScoreUI (CCmdUI* pCmdUI) {
	if (m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
	}
}


// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݓ���ON�x
void CSekaijuDoc::OnPopupTrackInputOn () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429�ǉ�
		return;
	}
	
	m_theCriticalSection.Lock ();

	// ����ON/OFF�ύX����(�����L�^)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_INPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetInputOn (pCloneTrack, 1);
		}
		else {
			MIDITrack_SetInputOn (pCloneTrack, 0);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݓ���ON�x
void CSekaijuDoc::OnUpdatePopupTrackInputOnUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݓ���OFF�x
void CSekaijuDoc::OnPopupTrackInputOff () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429�ǉ�
		return;
	}

	m_theCriticalSection.Lock ();

	// ����ON/OFF�ύX����(�����L�^)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_INPUT_OFF));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetInputOn (pCloneTrack, 0);
		}
		else {
			MIDITrack_SetInputOn (pCloneTrack, 1);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݓ���OFF�x
void CSekaijuDoc::OnUpdatePopupTrackInputOffUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N����ON�x
void CSekaijuDoc::OnPopupTrackInputAll () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	m_theCriticalSection.Lock ();
	// ����ON/OFF�ύX����(�����L�^)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ALL_TRACK_INPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		MIDITrack_SetInputOn (pCloneTrack, 1);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N����ON�x
void CSekaijuDoc::OnUpdatePopupTrackInputAllUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݏo��ON�x
void CSekaijuDoc::OnPopupTrackOutputOn () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429�ǉ�
		return;
	}

	m_theCriticalSection.Lock ();
	// �o��ON/OFF�ύX����(�����L�^)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_OUTPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetOutputOn (pCloneTrack, 1);
		}
		else {
			MIDITrack_SetOutputOn (pCloneTrack, 0);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	// ���t��ԕ�������
	long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	pSekaijuApp->m_theCriticalSection.Lock ();
	pSekaijuApp->ResetTempMIDIStatusArray ();
	this->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
	long lFlags;
	// �Đ����Ȃ��(a)���ׂĖ���(b)�m�[�g�݂̂𕜌�����
	if (pSekaijuApp->m_bPlaying) {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		pSekaijuApp->m_bIgnoreNoteEvent = 1;
	}
	// ��~���Ȃ��(a)�m�[�g���������ׂĂ𕜌����邩(b)�����������Ȃ� 
	else {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		pSekaijuApp->m_bIgnoreNoteEvent = 0;
	}
	pSekaijuApp->SendDifferentStatus (lFlags);
	pSekaijuApp->m_theCriticalSection.Unlock ();
	
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݏo��ON�x
void CSekaijuDoc::OnUpdatePopupTrackOutputOnUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݏo��OFF�x
void CSekaijuDoc::OnPopupTrackOutputOff () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}
	if (m_pTempTrack == NULL) {  // 20100429�ǉ�
		return;
	}

	m_theCriticalSection.Lock ();
	// �o��ON/OFF�ύX����(�����L�^)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ONLY_THIS_TRACK_OUTPUT_OFF));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		if (pMIDITrack == m_pTempTrack) {
			MIDITrack_SetOutputOn (pCloneTrack, 0);
		}
		else {
			MIDITrack_SetOutputOn (pCloneTrack, 1);
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	// ���t��ԕ�������
	long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	pSekaijuApp->m_theCriticalSection.Lock ();
	pSekaijuApp->ResetTempMIDIStatusArray ();
	this->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
	long lFlags;
	// �Đ����Ȃ��(a)���ׂĖ���(b)�m�[�g�݂̂𕜌�����
	if (pSekaijuApp->m_bPlaying) {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		pSekaijuApp->m_bIgnoreNoteEvent = 1;
	}
	// ��~���Ȃ��(a)�m�[�g���������ׂĂ𕜌����邩(b)�����������Ȃ� 
	else {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		pSekaijuApp->m_bIgnoreNoteEvent = 0;
	}
	pSekaijuApp->SendDifferentStatus (lFlags);
	pSekaijuApp->m_theCriticalSection.Unlock ();
	
	
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w���̃g���b�N�̂ݏo��OFF�x
void CSekaijuDoc::OnUpdatePopupTrackOutputOffUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_pTempTrack == NULL) { // 20100429�ǉ�
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N�o��ON�x
void CSekaijuDoc::OnPopupTrackOutputAll () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}

	m_theCriticalSection.Lock ();

	// �o��ON/OFF�ύX����(�����L�^)
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_ALL_TRACK_OUTPUT_ON));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	MIDITrack* pMIDITrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack));
		VERIFY (pCloneTrack = this->ReplaceMIDITrack (pMIDITrack));
		MIDITrack_SetOutputOn (pCloneTrack, 1);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		pMIDITrack = pCloneTrack;
	}

	// ���t��ԕ�������
	long lCurTime = MIDIClock_GetTickCount (m_pMIDIClock);
	pSekaijuApp->m_theCriticalSection.Lock ();
	pSekaijuApp->ResetTempMIDIStatusArray ();
	this->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
	long lFlags;
	// �Đ����Ȃ��(a)���ׂĖ���(b)�m�[�g�݂̂𕜌�����
	if (pSekaijuApp->m_bPlaying) {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		pSekaijuApp->m_bIgnoreNoteEvent = 1;
	}
	// ��~���Ȃ��(a)�m�[�g���������ׂĂ𕜌����邩(b)�����������Ȃ� 
	else {
		lFlags = pSekaijuApp->m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		pSekaijuApp->m_bIgnoreNoteEvent = 0;
	}
	pSekaijuApp->SendDifferentStatus (lFlags);
	pSekaijuApp->m_theCriticalSection.Unlock ();

	
	UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED);
	m_theCriticalSection.Unlock ();	
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w�S�g���b�N�o��ON�x
void CSekaijuDoc::OnUpdatePopupTrackOutputAllUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
}

// �w�|�b�v�A�b�v�x-�w�؂���(&T)�x
void CSekaijuDoc::OnPopupCut () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// �N���b�v�{�[�h�ɓ]�����镶������쐬
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0001, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// �N���b�v�{�[�h�ɓƎ��e�L�X�g��]��
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}

	// �I���C�x���g���폜
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_theCriticalSection.Lock ();
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_CUT));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = GetCurHistoryUnit ());

	this->DeleteSelectedEvent (pCurHistoryUnit);
	
	SetModifiedFlag (TRUE);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w�؂���(&T)�x
void CSekaijuDoc::OnUpdatePopupCutUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}

	// �ЂƂł�MIDI�C�x���g���I������Ă����Enable
	// �����łȂ����Disable�Ƃ���
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �w�|�b�v�A�b�v�x-�w�R�s�[(&C)�x
void CSekaijuDoc::OnPopupCopy () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	long lRet = 0;
	long lCopiedEventCount = 0;
	BeginWaitCursor ();

	// �N���b�v�{�[�h�ɓ]�����镶������쐬
	CString strText;
	m_theCriticalSection.Lock ();
	lCopiedEventCount = this->MakeCopyString (strText, 0x0001, NULL);
	UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
	if (lCopiedEventCount == 0) {
		EndWaitCursor ();
		return;
	}

	// �N���b�v�{�[�h�ɓƎ��e�L�X�g��]��
	lRet = this->SetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_WRITE_TO_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	EndWaitCursor ();
}

// �w�|�b�v�A�b�v�x-�w�R�s�[(&C)�x
void CSekaijuDoc::OnUpdatePopupCopyUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}


	// �ЂƂł�MIDI�C�x���g���I������Ă����Enable
	// �����łȂ����Disable�Ƃ���
	m_theCriticalSection.Lock ();
	long lEnable = 0;
	MIDITrack* pMIDITrack;
	MIDIEvent* pMIDIEvent;
	forEachTrack (m_pMIDIData, pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			if (this->IsEventSelected (pMIDIEvent)) {
				lEnable = 1;
				break;
			}
		}
		if (lEnable) {
			break;
		}
	}
	pCmdUI->Enable (lEnable);
	m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�|�b�v�A�b�v�x-�w�\��t��(&T)�x
void CSekaijuDoc::OnPopupPaste () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		return;
	}
	if (m_bEditLocked) {
		return;
	}


	BeginWaitCursor ();
	CString strMsg;

	// �N���b�v�{�[�h�̃e�L�X�g���擾
	CString strText;
	long lRet = this->GetClipboardTextPrivate9 (strText);
	if (lRet == FALSE) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_UNABLE_TO_READ_FROM_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// �e�L�X�g�t�H�[�}�b�g����
	if (strText.Left(5) != _T("MDa2 ")) {
		EndWaitCursor ();
		VERIFY (strMsg.LoadString (IDS_NO_AVAILABLE_MIDIDATA_IN_THE_CLIPBOARD));
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return;
	}
	
	// �\��t������
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_EDIT_PASTE));
	VERIFY (this->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = this->GetCurHistoryUnit ());
	m_theCriticalSection.Lock ();
	long lInsertedEventCount = this->ParsePasteString (strText, m_lTempTrackIndex, m_lTempTime, pCurHistoryUnit);
	this->SetModifiedFlag (TRUE);
	this->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED |
		SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	m_theCriticalSection.Unlock ();
	EndWaitCursor ();
	::Sleep (0);

	// ���ʂ̃��b�Z�[�W�{�b�N�X�\��
	if (lInsertedEventCount == -1) {
		VERIFY (strMsg.LoadString (IDS_PASTE_FAILED));
	}
	else {
		CString strFormat;
		VERIFY (strFormat.LoadString (IDS_D_MIDIEVENTS_WERE_INSERTED));
		strMsg.Format (strFormat, lInsertedEventCount);
	}
	AfxMessageBox (strMsg, MB_ICONINFORMATION);
}

// �w�|�b�v�A�b�v�x-�w�\��t��(&T)�x
void CSekaijuDoc::OnUpdatePopupPasteUI (CCmdUI* pCmdUI) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (pSekaijuApp->m_bRecording ||
		pSekaijuApp->m_bInplaceEditing ||
		pSekaijuApp->m_bInplaceListing ||
		pSekaijuApp->m_bValueUpDowning) {
		pCmdUI->Enable (FALSE);
		return;
	}
	if (m_bEditLocked) {
		pCmdUI->Enable (FALSE);
		return;
	}
	
	// �N���b�v�{�[�h�̌`�����Ǝ��e�L�X�g�����ׂ�
	pCmdUI->Enable (this->IsClipboardTextPrivate9 ());

}




