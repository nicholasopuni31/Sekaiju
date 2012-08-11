//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�g���b�N���[�h�r���[�N���X
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
#include "common.h"
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
#include "TrackListTrackModeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDC_TEXTBOX 3939

IMPLEMENT_DYNCREATE (CTrackListTrackModeView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CTrackListTrackModeView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_KILLFOCUS ()
	ON_WM_KEYDOWN ()
	ON_WM_KEYUP ()
	ON_WM_CHAR ()
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_LBUTTONDBLCLK ()
	ON_WM_RBUTTONDBLCLK ()
	ON_WM_TIMER ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CTrackListTrackModeView::CTrackListTrackModeView () {
	m_lCurRow = 0;
	m_lCurColumn = 0;
	m_lCurButtonState = 0x00;
	m_lCurButtonInterval = 200;
	m_bSettingCellString = 0;
}

// �f�X�g���N�^
CTrackListTrackModeView::~CTrackListTrackModeView () {
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// (X,Y)���W����Z���ԍ����擾
BOOL CTrackListTrackModeView::GetCellFromPoint 
(CPoint pt, long* pRow, long* pColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	_ASSERT (pRow);
	_ASSERT (pColumn);
	*pRow = pTrackListFrame->YtoRow (pt.y);
	*pColumn = pTrackListFrame->XtoColumn (pt.x);
	return TRUE;
}

// �w��Z�����r���[����͂ݏo�����ꍇ�̃I�[�g�X�N���[������
BOOL CTrackListTrackModeView::AutoScrolltoShowCell (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	long lNewPos = 0;
	long lRowZoom = pTrackListFrame->GetRowZoom ();
	// �w��Z�����r���[�̍��ɂ͂ݏo�����ꍇ�̏���
	if (pTrackListFrame->GetColumnLeft (lColumn) < rcClient.left) {
		lNewPos = pTrackListFrame->GetColumnLeft (lColumn);
		pTrackListFrame->SetColumnScrollPos (lNewPos);
	}
	// �w��Z�����r���[�̉E�ɂ͂ݏo�����ꍇ�̏���
	else if (pTrackListFrame->GetColumnLeft (lColumn) + 
		pTrackListFrame->GetColumnWidth (lColumn) > rcClient.right) {
		lNewPos = pTrackListFrame->GetColumnLeft (lColumn) + 
			pTrackListFrame->GetColumnWidth (lColumn) - rcClient.Width ();
		pTrackListFrame->SetColumnScrollPos (lNewPos);
	}
	// �w��Z�����r���[�̏�ɂ͂ݏo�����ꍇ�̏���
	if (lRow * lRowZoom < rcClient.top) {
		lNewPos = lRow * lRowZoom;
		pTrackListFrame->SetRowScrollPos (lNewPos);		
	}
	// �w��Z�����r���[�̉��ɂ͂ݏo�����ꍇ�̏���
	else if ((lRow + 1) * lRowZoom > rcClient.bottom) {
		lNewPos = (lRow + 1) * lRowZoom - rcClient.Height ();
		pTrackListFrame->SetRowScrollPos (lNewPos);
	}
	return TRUE;
}

// ���݃e�L�X�g�{�b�N�X�ŕҏW�����ǂ����Ԃ��B
BOOL CTrackListTrackModeView::IsTextEditing () {
	return (m_theTextBox.GetStyle () & WS_VISIBLE) ? TRUE : FALSE;
}

// �e�L�X�g�{�b�N�X�ł̕ҏW���J�n����B
BOOL CTrackListTrackModeView::BeginTextEditing () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	//ASSERT (lFormat == 0 && m_lCurRow == 0 || lFormat == 1 && m_lCurRow != 0);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->m_bInplaceEditing = 1;
	CString strCellString = GetCellString (m_lCurRow, m_lCurColumn);
	m_theTextBox.SetWindowText (strCellString);
	m_theTextBox.SetSel (0, -1, TRUE);
	m_theTextBox.EmptyUndoBuffer ();
	m_theTextBox.ShowWindow (SW_SHOW);
	m_theTextBox.SetFocus ();
	m_theTextBox.UpdateWindow ();
	return TRUE;
}

// �e�L�X�g�{�b�N�X�ł̕ҏW���I�����A�V�����l���i�[����B
BOOL CTrackListTrackModeView::EndTextEditingOK () {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// �V���������̍쐬�E�s���g���b�N�̒ǉ��E�l�̐ݒ��SetCellString�����B

	// ���݂̃Z���̕ҏW�e�L�X�g�𔽉f
	CString strText;
	m_theTextBox.GetWindowText (strText);
	if (SetCellString (m_lCurRow, m_lCurColumn, strText)) {
		pSekaijuDoc->SetModifiedFlag (TRUE);
		pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
	}

	// �ҏW�I��
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// �e�L�X�g�{�b�N�X�ł̕ҏW���I�����A�V�����l���i�[���Ȃ��B
BOOL CTrackListTrackModeView::EndTextEditingCancel () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// �l�̑������J�n����
BOOL CTrackListTrackModeView::BeginValueUpDown () {
	_RPTF0 (_CRT_WARN, "BeginValueUpDown\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (m_lCurButtonState == 0x0000);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	long lNumber = lColumnContent >> 16;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	//ASSERT (lFormat == 0 && m_lCurRow == 0 || lFormat == 1 && m_lCurRow != 0);
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	long lChannel = 0;


	// ���ڕʂ�
	switch (lColumnContent & 0xFFFF) {
	// ���̓|�[�g�ԍ��A���̓`�����l���A�o�̓|�[�g�ԍ��A�o�̓`�����l���̏ꍇ
	case TRACKLISTFRAME_INPUTPORT:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_INPUTCHANNEL:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_OUTPUTPORT:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	// �R���g���[���`�F���W�̏ꍇ
	case TRACKLISTFRAME_CONTROLCHANGE:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_CONTROLCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ���̃g���b�N�ōŏ���CC#lNumber�C�x���g��T���B
		lChannel = MIDITrack_GetOutputChannel (pTempTrack);
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_CONTROLCHANGE | lChannel);
		while (pTempEvent) {
			if (MIDIEvent_GetNumber (pTempEvent) == lNumber) {
				break;
			}
			pTempEvent = pTempEvent->m_pNextSameKindEvent;
		}
		// �Ȃ������ꍇ�A�R���g���[���`�F���W�C�x���g��V�K�쐬���ăg���b�N�ɒǉ��B
		if (pTempEvent == NULL) {
			lChannel = CLIP (0, lChannel, 15);
			VERIFY (pTempEvent = MIDIEvent_CreateControlChange (0, lChannel, lNumber, 0));
			VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
		}
		// �������ꍇ�A�����̂��̂�ێ����A�V�����R���g���[���`�F���W�C�x���g�ɒu��������B
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		}
		break;
	// �v���O�����`�F���W�̏ꍇ
	case TRACKLISTFRAME_PROGRAMCHANGE:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_PROGRAMCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ���̃g���b�N�ōŏ��̃v���O�����`�F���W�C�x���g��T���B
		lChannel = MIDITrack_GetOutputChannel (pTempTrack);
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_PROGRAMCHANGE | lChannel);
		// �Ȃ������ꍇ�A�v���O�����`�F���W�C�x���g��V�K�쐬���ăg���b�N�ɒǉ��B
		if (pTempEvent == NULL) {
			lChannel = CLIP (0, lChannel, 15);
			VERIFY (pTempEvent = MIDIEvent_CreateProgramChange (0, lChannel, 0));
			VERIFY (MIDITrack_InsertEvent (pTempTrack, pTempEvent));
		}
		// �������ꍇ�A�����̂��̂�ێ����A�V�����v���O�����`�F���W�C�x���g�ɒu��������B
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		}
		break;
	// �^�C��+�A�L�[+�A�x���V�e�B+�̏ꍇ(20091126�ǉ�)
	case TRACKLISTFRAME_TIMEPLUS:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_TIMEPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_KEYPLUS:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_KEYPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	case TRACKLISTFRAME_VELOCITYPLUS:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VELPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// �����̃g���b�N��ێ����邽�߂ɒu��������B
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack)); 
		break;
	}
	return TRUE;
}

// �l�̑������I������
BOOL CTrackListTrackModeView::EndValueUpDown () {
	_RPTF0 (_CRT_WARN, "EndValueUpDown\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	long lNumber = lColumnContent >> 16;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	// �V���������̗p��
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	VERIFY (pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow));
	long lChannel = MIDITrack_GetOutputChannel (pTempTrack);
	lChannel = CLIP (0, lChannel, 15);
	// ���ڕʂ�
	switch (lColumnContent & 0xFFFF) {
	case TRACKLISTFRAME_INPUTPORT:
	case TRACKLISTFRAME_INPUTCHANNEL:
	case TRACKLISTFRAME_OUTPUTPORT:
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pTempTrack));
		break;
	// �R���g���[���`�F���W
	case TRACKLISTFRAME_CONTROLCHANGE:
		// ���̃g���b�N�ōŏ���CC#lNumber�C�x���g��T���B
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_CONTROLCHANGE | lChannel);
		while (pTempEvent) {
			if (MIDIEvent_GetNumber (pTempEvent) == lNumber) {
				break;
			}
			pTempEvent = pTempEvent->m_pNextSameKindEvent;
		}
		// ���ӁF�t�H�[�}�b�g0�̍ŏ��ȊO�̃g���b�N���̓t�H�[�}�b�g1�̍ŏ��̃g���b�N�Őݒ肵�Ă͂Ȃ�Ȃ��B
		ASSERT (pTempEvent);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		break;
	// �v���O�����`�F���W
	case MIDIEVENT_PROGRAMCHANGE:
		// ���̃g���b�N�ōŏ��̃v���O�����`�F���W�C�x���g��T���B
		pTempEvent = MIDITrack_GetFirstKindEvent (pTempTrack, MIDIEVENT_PROGRAMCHANGE | lChannel);
		// ���ӁF�t�H�[�}�b�g0�̍ŏ��ȊO�̃g���b�N���̓t�H�[�}�b�g1�̍ŏ��̃g���b�N�Őݒ肵�Ă͂Ȃ�Ȃ��B
		ASSERT (pTempEvent);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		break;
	// �^�C��+�A�L�[+�A�x���V�e�B+�̏ꍇ(20091126�ǉ�)
	case TRACKLISTFRAME_TIMEPLUS:
	case TRACKLISTFRAME_KEYPLUS:
	case TRACKLISTFRAME_VELOCITYPLUS:
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pTempTrack));
	}
	return TRUE;
}

// ���݂̃Z���̒l�𑝌�������
BOOL CTrackListTrackModeView::AddValueOfCurCell (long lDeltaValue) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	// ���݂̃g���b�N�ւ̃|�C���^���擾
	// (�Y������g���b�N�� BeginValueUpDown�Œǉ�����Ă���͂��ł���)
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	VERIFY (pMIDITrack = pSekaijuDoc->GetTrack (m_lCurRow));
	// �l�̔��f
	long lValue = 0;
	long lNumber = 0;
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	switch (lColumnContent & 0xFFFF) {
	// �R���g���[���`�F���W(0�`127)�̏ꍇ
	case TRACKLISTFRAME_CONTROLCHANGE:
		// ���ӁF�t�H�[�}�b�g0�̍ŏ��ȊO�̃g���b�N���̓t�H�[�}�b�g1�̍ŏ��̃g���b�N�ł͐ݒ肵�Ă͂Ȃ�Ȃ��B
		lNumber = pTrackListFrame->GetColumnContent (m_lCurColumn) >> 16;
		VERIFY (pMIDIEvent = pSekaijuDoc->GetTrackFirstControlChange (pMIDITrack, lNumber));
		lValue = MIDIEvent_GetValue (pMIDIEvent);
		// (Bank Select MSB (CC#0) ���� Bank Select LSB (CC#32)) �̏ꍇ���p�b�`�T�[�`ON�̂Ƃ�
		if ((lNumber == 0 || lNumber == 32) && pSekaijuApp->m_theGeneralOption.m_bPatchSearch) {
			long lRet;
			MIDIEvent* pCC0Event = pSekaijuDoc->FindBankMSB (pMIDIEvent);
			long lBankMSB = 0;
			if (pCC0Event) {
				lBankMSB = MIDIEvent_GetValue (pCC0Event);
			}
			MIDIEvent* pCC32Event = pSekaijuDoc->FindBankLSB (pMIDIEvent);
			long lBankLSB = 0;
			if (pCC32Event) {
				lBankLSB = MIDIEvent_GetValue (pCC32Event);
			}
			MIDIEvent* pPCEvent = pSekaijuDoc->FindProgramChange (pMIDIEvent);
			long lProgramChange = 0;
			if (pPCEvent) {
				lProgramChange = MIDIEvent_GetNumber (pPCEvent);
			}
			long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
			MIDIInstrumentDefinition* pMIDIInstDef = lTrackViewMode ?
				pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort] :
				pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			// �C���X�g�D�������g��`���ݒ肳��Ă���ꍇ
			if (pMIDIInstDef) {
				MIDIPatchNameTable* pMIDIPatchNameTable = NULL;
				long lBank = 0;
				TCHAR szBuf[256];
				long lNewValue = lValue + (lDeltaValue > 0 ? 1 : -1);
				long lTempNewValue = lValue; // �Ō�ɉ��F�����������V�����l
				while (0 <= lNewValue && lNewValue <= 127) {
					if (lNumber == 0) {
						lBank = (lNewValue << 7) | lBankLSB;
					}
					else if (lNumber == 32) {
						lBank = (lBankMSB << 7) | lNewValue;
					}
					// ���F�����邩���ׂ�
					pMIDIPatchNameTable = 
					MIDIInstrumentDefinition_GetPatchNameTable (pMIDIInstDef, lBank);				
					lRet = 0;
					if (pMIDIPatchNameTable) {
						lRet = MIDIPatchNameTable_GetName 
							(pMIDIPatchNameTable, lProgramChange, szBuf, TSIZEOF (szBuf));
					}
					// ���F���������ꍇ
					if (lRet > 0) {
						lTempNewValue = lNewValue;
						if (lNewValue - lValue >= lDeltaValue && lDeltaValue > 0 ||
							lNewValue - lValue <= lDeltaValue && lDeltaValue < 0) {
							break;
						}
					}
					lNewValue += (lDeltaValue > 0 ? 1 : -1);
				}
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lTempNewValue, 127)));
			}
			// �C���X�g�D�������g��`���ݒ肳��Ă��Ȃ��ꍇ(�ʏ�̏ꍇ�Ɠ�������)
			else {
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
			}
		}
		// �ʏ�̏ꍇ
		else {
			VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
		}
		break;
	// �v���O�����`�F���W(0�`127)�̏ꍇ
	case TRACKLISTFRAME_PROGRAMCHANGE:
		// ���ӁF�t�H�[�}�b�g0�̍ŏ��ȊO�̃g���b�N���̓t�H�[�}�b�g1�̍ŏ��̃g���b�N�ł͐ݒ肵�Ă͂Ȃ�Ȃ��B
		VERIFY (pMIDIEvent = pSekaijuDoc->GetTrackFirstProgramChange (pMIDITrack));
		lValue = MIDIEvent_GetValue (pMIDIEvent);
		// �p�b�`�T�[�`ON�̂Ƃ�
		if (pSekaijuApp->m_theGeneralOption.m_bPatchSearch) {
			long lRet;
			MIDIEvent* pCC0Event = pSekaijuDoc->FindBankMSB (pMIDIEvent);
			long lBankMSB = 0;
			if (pCC0Event) {
				lBankMSB = MIDIEvent_GetValue (pCC0Event);
			}
			MIDIEvent* pCC32Event = pSekaijuDoc->FindBankLSB (pMIDIEvent);
			long lBankLSB = 0;
			if (pCC32Event) {
				lBankLSB = MIDIEvent_GetValue (pCC32Event);
			}
			MIDIEvent* pPCEvent = pMIDIEvent;
			long lProgramChange = 0;
			if (pPCEvent) {
				lProgramChange = MIDIEvent_GetNumber (pPCEvent);
			}
			long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
			long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
			MIDIInstrumentDefinition* pMIDIInstDef = lTrackViewMode ?
				pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort] :
				pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			// �C���X�g�D�������g��`���ݒ肳��Ă���ꍇ
			if (pMIDIInstDef) {
				MIDIPatchNameTable* pMIDIPatchNameTable = NULL;
				long lBank = 0;
				TCHAR szBuf[256];
				long lNewValue = lValue + (lDeltaValue > 0 ? 1 : -1);
				long lTempNewValue = lValue; // �Ō�ɉ��F�����������V�����l
				while (0 <= lNewValue && lNewValue <= 127) {
					lBank = (lBankMSB << 7) | lBankLSB;
					// ���F�����邩���ׂ�
					pMIDIPatchNameTable = 
					MIDIInstrumentDefinition_GetPatchNameTable (pMIDIInstDef, lBank);				
					lRet = 0;
					if (pMIDIPatchNameTable) {
						lRet = MIDIPatchNameTable_GetName 
							(pMIDIPatchNameTable, lNewValue, szBuf, TSIZEOF (szBuf));
					}
					// ���F���������ꍇ
					if (lRet > 0) {
						lTempNewValue = lNewValue;
						if (lNewValue - lValue >= lDeltaValue && lDeltaValue > 0 ||
							lNewValue - lValue <= lDeltaValue && lDeltaValue < 0) {
							break;
						}
					}
					lNewValue += (lDeltaValue > 0 ? 1 : -1);
				}
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lTempNewValue, 127)));
			}
			// �C���X�g�D�������g��`���ݒ肳��Ă��Ȃ��ꍇ(�ʏ�̏ꍇ�Ɠ�������)
			else {
				VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
			}
		}
		// �ʏ�̏ꍇ
		else {
			VERIFY (MIDIEvent_SetValue (pMIDIEvent, CLIP (0, lValue + lDeltaValue, 127)));
		}
		break;
	// ���̓|�[�g(0�`15)�̏ꍇ
	case TRACKLISTFRAME_INPUTPORT:
		lValue = MIDITrack_GetInputPort (pMIDITrack);
		VERIFY (MIDITrack_SetInputPort (pMIDITrack, CLIP (0, lValue + lDeltaValue, 15)));
		break;
	// ���̓`�����l��(0�`15)�̏ꍇ
	case TRACKLISTFRAME_INPUTCHANNEL:
		lValue = MIDITrack_GetInputChannel (pMIDITrack);
		VERIFY (MIDITrack_SetInputChannel (pMIDITrack, CLIP (-1, lValue + lDeltaValue, 15)));
		break;
	// �o�̓|�[�g(0�`15)�̏ꍇ
	case TRACKLISTFRAME_OUTPUTPORT:
		lValue = MIDITrack_GetOutputPort (pMIDITrack);
		VERIFY (MIDITrack_SetOutputPort (pMIDITrack, CLIP (0, lValue + lDeltaValue, 15)));
		break;
	// �o�̓`�����l��(0�`15)�̏ꍇ
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		lValue = MIDITrack_GetOutputChannel (pMIDITrack);
		VERIFY (MIDITrack_SetOutputChannel (pMIDITrack, CLIP (-1, lValue + lDeltaValue, 15)));
		break;
	// �^�C��+(-127�`127)�̏ꍇ
	case TRACKLISTFRAME_TIMEPLUS:
		lValue = MIDITrack_GetTimePlus (pMIDITrack);
		VERIFY (MIDITrack_SetTimePlus (pMIDITrack, CLIP (-127, lValue + lDeltaValue, 127)));
		break;
	// �L�[+(-127�`127)�̏ꍇ
	case TRACKLISTFRAME_KEYPLUS:
		lValue = MIDITrack_GetKeyPlus (pMIDITrack);
		VERIFY (MIDITrack_SetKeyPlus (pMIDITrack, CLIP (-127, lValue + lDeltaValue, 127)));
		break;
	// �x���V�e�B+(-127�`127)�̏ꍇ
	case TRACKLISTFRAME_VELOCITYPLUS:
		lValue = MIDITrack_GetVelocityPlus (pMIDITrack);
		VERIFY (MIDITrack_SetVelocityPlus (pMIDITrack, CLIP (-127, lValue + lDeltaValue, 127)));
		break;
	}

	return TRUE;
}

// �Z���̒l��On/Off��؂�ւ���B
BOOL CTrackListTrackModeView::ToggleValueOfCurCell () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	CHistoryUnit* pCurHistoryUnit = NULL;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lValue = 0;
	MIDITrack* pTempTrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CString strHistoryName;
	// �ݒ肷�鍀�ڂ̒l�̔��f
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	switch (lColumnContent & 0xFFFF) {
	// (���g�p����)
	case TRACKLISTFRAME_VISIBLE:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VISIBLE_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ����ێ��̂��ߌ���g���b�N�������ւ�
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// Visible ON/OFF�؂�ւ�
		lValue = pSekaijuDoc->GetTrackVisible (pCloneTrack);
		pSekaijuDoc->SetTrackVisible (pCloneTrack, !lValue);
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;
	// (���g�p����)
	case TRACKLISTFRAME_ENABLE:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_ENABLE_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ����ێ��̂��ߌ���g���b�N�������ւ�
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// Enable ON/OFF�؂�ւ�
		lValue = pSekaijuDoc->GetTrackEnable (pCloneTrack);
		pSekaijuDoc->SetTrackEnable (pCloneTrack, !lValue);
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;
	// MIDI����ON/OFF
	case TRACKLISTFRAME_INPUTON:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUT_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ����ێ��̂��ߌ���g���b�N�������ւ�
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// ����ON/OFF�؂�ւ�
		lValue = MIDITrack_GetInputOn (pCloneTrack);
		MIDITrack_SetInputOn (pCloneTrack, !lValue);
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;
	// MIDI�o��ON/OFF
	case TRACKLISTFRAME_OUTPUTON:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUT_ON_OFF));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ����ێ��̂��ߌ���g���b�N�������ւ�
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// �o��ON/OFF�؂�ւ�
		lValue = MIDITrack_GetOutputOn (pCloneTrack);
		MIDITrack_SetOutputOn (pCloneTrack, !lValue);
		lValue = MIDITrack_GetOutputOn (pCloneTrack);
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		// �o��ON��OFF�ɂȂ����ꍇ
		if (lValue == 0) {
			long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pSekaijuApp->ResetTempMIDIStatusArray ();
			pSekaijuDoc->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
			long lFlags;
			// �Đ����Ȃ�΃m�[�g�̂ݕ���
			if (pSekaijuApp->m_bPlaying) {
				lFlags = SDS_NOTE;
				pSekaijuApp->m_bIgnoreNoteEvent = 1;
			}
			// ��~���Ȃ�Ή����������Ȃ� 
			else {
				lFlags = 0;
				pSekaijuApp->m_bIgnoreNoteEvent = 0;
			}
			pSekaijuApp->SendDifferentStatus (lFlags);
			pSekaijuApp->m_theCriticalSection.Unlock ();
		}
		// �o��OFF��ON�ɂȂ����ꍇ
		else {
			long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pSekaijuApp->ResetTempMIDIStatusArray ();
			pSekaijuDoc->TimeMIDIStatus (lCurTime, pSekaijuApp->m_pTempMIDIStatus);
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
		}
		break;
	// �\�����[�h(�ʏ�/�h����)
	case TRACKLISTFRAME_VIEWMODE:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VIEWMODE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
		pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
		if (pTempTrack == NULL) {
			return FALSE;
		}
		// ����ێ��̂��ߌ���g���b�N�������ւ�
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
		VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
		// �ʏ�/�h�����؂�ւ�
		lValue = MIDITrack_GetViewMode (pCloneTrack);
		MIDITrack_SetViewMode (pCloneTrack, !lValue);
		// �����L�^
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
		break;	
	}
	return TRUE;
}

// �F�̑I��
BOOL CTrackListTrackModeView::DoColorEditing () {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lFormat = MIDIData_GetFormat (pMIDIData);
	MIDITrack* pTempTrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	// �����̗p��
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	VERIFY (strHistoryName.LoadString (IDS_MODIFY_COLOR));
	VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// ���݂̃g���b�N�ւ̃|�C���^���擾
	pSekaijuDoc->AddTrack (m_lCurRow + 1, 0x000F, pCurHistoryUnit);
	pTempTrack = pSekaijuDoc->GetTrack (m_lCurRow);
	if (pTempTrack == NULL) {
		return FALSE;
	}
	// �F�ݒ�_�C�A���O�̕\��
	long lColor = 0;
	long lRet = 0;
	CColorDialog theDlg;
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	switch (lColumnContent & 0xFFFF) {
	// �O�i�F
	case TRACKLISTFRAME_FORECOLOR:
		lColor = MIDITrack_GetForeColor (pTempTrack);
		theDlg.m_cc.rgbResult = lColor;
		theDlg.m_cc.Flags |= (CC_FULLOPEN | CC_RGBINIT);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		lRet = theDlg.DoModal ();
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRet == IDOK) {
			lColor = (theDlg.GetColor () & 0x00FFFFFF);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pTempTrack));
			VERIFY (pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pTempTrack));
			MIDITrack_SetForeColor (pCloneTrack, lColor);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack));
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIEVENTCHANGED);
			return TRUE;
		}
		break;
	// �w�i�F(���g�p)
	case TRACKLISTFRAME_BACKCOLOR:
		break;
	}
	return FALSE;
}

// �Z���̕�������擾
CString CTrackListTrackModeView::GetCellString (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lRow);
	MIDIEvent* pMIDIEvent = NULL;
	CString strText;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	long lValue = 0;
	long lNumber = 0;
	MIDIIn* pMIDIIn = NULL;
	MIDIOut* pMIDIOut = NULL;
	CString strNone;
	VERIFY (strNone.LoadString (IDS_NONE));
	if (pMIDITrack) {
		long lColumnContent = pTrackListFrame->GetColumnContent (lColumn);
		TCHAR szBuf2[2048];
		memset (szBuf2, 0, sizeof (szBuf2));
		switch (lColumnContent & 0xFFFF) {
		// �g���b�N��
		case TRACKLISTFRAME_TRACKNAME:
			strText = pSekaijuDoc->GetTrackName (pMIDITrack);
			codestr2str ((LPTSTR)(LPCTSTR)strText, strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			strText = szBuf2;
			break;
		// ��(���g�p)
		case TRACKLISTFRAME_VISIBLE:
			if (pSekaijuDoc->GetTrackVisible (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_SHOW));
			}
			else {
				VERIFY (strText.LoadString (IDS_HIDE));	
			}
			break;
		// �C�l�[�u��(���g�p)
		case TRACKLISTFRAME_ENABLE:
			if (pSekaijuDoc->GetTrackEnable (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_ENABLE));
			}
			else {
				VERIFY (strText.LoadString (IDS_LOCK));	
			}
			break;
		// ����ON
		case TRACKLISTFRAME_INPUTON:
			if (MIDITrack_GetInputOn (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_ON));
			}
			else {
				VERIFY (strText.LoadString (IDS_OFF));	
			}
			break;
		// �o��ON
		case TRACKLISTFRAME_OUTPUTON:
			if (MIDITrack_GetOutputOn (pMIDITrack)) {			
				VERIFY (strText.LoadString (IDS_ON));
			}
			else {
				VERIFY (strText.LoadString (IDS_OFF));	
			}
			break;
		// ���̓|�[�g�ԍ�
		case TRACKLISTFRAME_INPUTPORT:
			lValue = MIDITrack_GetInputPort (pMIDITrack);
			ASSERT (0 <= lValue && lValue < MAXMIDIINDEVICENUM);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pMIDIIn = pSekaijuApp->m_pMIDIIn[lValue];
			if (pMIDIIn) {
				strText.Format (_T("%3d-%s"), lValue + 1, pMIDIIn->m_pDeviceName);
			}
			else {
				strText.Format (_T("%3d-%s"), lValue + 1, strNone);
			}
			pSekaijuApp->m_theCriticalSection.Unlock ();
			break;
		// ���̓`�����l��
		case TRACKLISTFRAME_INPUTCHANNEL:
			lValue = MIDITrack_GetInputChannel (pMIDITrack);
			if (0 <= lValue && lValue <= 15) {
				strText.Format (_T("%3d"), lValue + 1);
			}
			else {
				VERIFY (strText.LoadString (IDS_N_A));
			}
			break;
		// �o�̓|�[�g�ԍ�
		case TRACKLISTFRAME_OUTPUTPORT:
			lValue = MIDITrack_GetOutputPort (pMIDITrack);
			ASSERT (0 <= lValue && lValue < MAXMIDIOUTDEVICENUM);
			pSekaijuApp->m_theCriticalSection.Lock ();
			pMIDIOut = pSekaijuApp->m_pMIDIOut[lValue];
			if (pMIDIOut) {
				strText.Format (_T("%3d-%s"), lValue + 1, pMIDIOut->m_pDeviceName);
			}
			else {
				strText.Format (_T("%3d-%s"), lValue + 1, strNone);
			}
			pSekaijuApp->m_theCriticalSection.Unlock ();
			break;
		// �o�̓`�����l��
		case TRACKLISTFRAME_OUTPUTCHANNEL:
			lValue = MIDITrack_GetOutputChannel (pMIDITrack);
			if (0 <= lValue && lValue <= 15) {
				strText.Format (_T("%3d"), lValue + 1);
			}
			else {
				VERIFY (strText.LoadString (IDS_N_A));
			}
			break;
		// �\�����[�h(�ʏ�^�h����)
		case TRACKLISTFRAME_VIEWMODE:
			if (MIDITrack_GetViewMode (pMIDITrack)) {
				VERIFY (strText.LoadString (IDS_DRUM));
			}
			else {
				VERIFY (strText.LoadString (IDS_NORM));
			}
			break;
		// �R���g���[���`�F���W
		case TRACKLISTFRAME_CONTROLCHANGE:
			lValue = -1;
			lNumber = pTrackListFrame->GetColumnContent (lColumn) >> 16;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsControlChange (pMIDIEvent)) {
					if (MIDIEvent_GetNumber (pMIDIEvent) == lNumber) {
						lValue = MIDIEvent_GetValue (pMIDIEvent);
						break;
					}
				}
			}
			if (0 <= lValue && lValue <= 127) {
				strText.Format (_T("%3d"), lValue);
			}
			else {
				strText = _T("---");
			}
			break;
		// �v���O�����`�F���W
		case TRACKLISTFRAME_PROGRAMCHANGE:
			lValue = -1;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
					lValue = MIDIEvent_GetNumber (pMIDIEvent);
					break;
				}
			}
			if (0 <= lValue && lValue <= 127 && pMIDIEvent) {
				long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
				ASSERT (0 <= lTrackOutputPort && lTrackOutputPort < 16);
				long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
				ASSERT (-1 <= lTrackOutputChannel && lTrackOutputChannel < 16);
				if (lTrackOutputChannel == -1) {
					lTrackOutputChannel = MIDIEvent_GetChannel (pMIDIEvent);
				}
				ASSERT (0 <= lTrackOutputChannel && lTrackOutputChannel < 16);
				long lTrackViewMode  = MIDITrack_GetViewMode (pMIDITrack);
				TCHAR szBuf[256];
				memset (szBuf, 0, sizeof (szBuf));
				long lBankMSB = MIDIEvent_GetBankMSB (pMIDIEvent);
				long lBankLSB = MIDIEvent_GetBankLSB (pMIDIEvent);
				long lBank = (lBankMSB << 7) | lBankLSB; //MIDIEvent_GetBank (pMIDIEvent);
				MIDIInstrumentDefinition* pInstDef = NULL;
				// ���̃g���b�N�̕\�����[�h���u�ʏ�v�̏ꍇ
				if (lTrackViewMode == 0) {
					pInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
				}
				// ���̃g���b�N�̕\�����[�h���u�h�����v�̏ꍇ
				else {
					pInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
				}
				// ���̃g���b�N�̃C���X�g�D�������g��`����������
				if (pInstDef) {
					MIDIPatchNameTable* pPatchNameTable =
						MIDIInstrumentDefinition_GetPatchNameTable (pInstDef, lBank);
					// ���̃C���X�g�D�������g��`�̎w��o���N��PatchNameTable����������
					if (pPatchNameTable) {
						TCHAR szBuf[256];
						memset (szBuf, 0, 256);
						MIDIPatchNameTable_GetName 
							(pPatchNameTable, lValue, szBuf, 255);
						strText.Format (_T("%d-%s"), lValue, szBuf);
					}
					else {
						strText.Format (_T("%d"), lValue);
					}
				}
				else {
					strText.Format (_T("%d"), lValue);
				}
			}
			else {
				strText = _T("---");
			}
			break;
		// �^�C��+
		case TRACKLISTFRAME_TIMEPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetTimePlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// �L�[+
		case TRACKLISTFRAME_KEYPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetKeyPlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// �x���V�e�B+
		case TRACKLISTFRAME_VELOCITYPLUS:
			if (lFormat == 1 && lRow == 0) {
				strText.Format (_T("---"));
			}
			else {
				lValue = MIDITrack_GetVelocityPlus (pMIDITrack);
				strText.Format (_T("%3d"), lValue);
			}
			break;
		// �C�x���g��
		case TRACKLISTFRAME_NUMEVENT:
			lValue = MIDITrack_CountEvent (pMIDITrack);
			strText.Format (_T("%8d"), lValue);
			break;
		}
	}
	return strText;
}

// �Z���̕������ݒ�
BOOL CTrackListTrackModeView::SetCellString (long lRow, long lColumn, CString strText) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	m_bSettingCellString = 1;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	long lFormat = MIDIData_GetFormat (pMIDIData);
	CHistoryUnit* pCurHistoryUnit = NULL;
	CString strHistoryName;
	long lValue = 0;
	long lNumber = 0;
	MIDIIn* pMIDIIn = NULL;
	MIDIOut* pMIDIOut = NULL;
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	long lColumnContent = pTrackListFrame->GetColumnContent (lColumn);
	switch (lColumnContent & 0xFFFF) {
	// �g���b�N��
	case TRACKLISTFRAME_TRACKNAME:
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_TRACKNAME));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �ŏ��̃g���b�N���C�x���g��T��
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_GetKind (pTempEvent) == MIDIEVENT_TRACKNAME) {
				break;
			}
		}
		// �ŏ��̃g���b�N���C�x���g��������Ȃ������ꍇ
		if (pTempEvent == NULL) {
			TCHAR szBuf2[2048];
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)(strText), strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			VERIFY (pTempEvent = MIDIEvent_CreateTrackName (0, szBuf2));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// �ŏ��̃g���b�N�������������ꍇ
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
			TCHAR szBuf2[2048];
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)(strText), strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			MIDIEvent_SetText (pCloneEvent, szBuf2);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}
		break;
	// ���̓|�[�g�ԍ�
	case TRACKLISTFRAME_INPUTPORT:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= 0 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// ���̓|�[�g�ԍ��̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetInputPort (pCloneTrack, CLIP (0, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// ���̓`�����l��
	case TRACKLISTFRAME_INPUTCHANNEL:
		strText.TrimLeft ();
		if (strText == _T("n/a")) {
			strText = _T("0");
		}
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= -1 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_INPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// ���̓`�����l���̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetInputChannel (pCloneTrack, CLIP (-1, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// �o�̓|�[�g�ԍ�
	case TRACKLISTFRAME_OUTPUTPORT:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= 0 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTPORT));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �o�̓|�[�g�ԍ��̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetOutputPort (pCloneTrack, CLIP (0, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// �o�̓`�����l��
	case TRACKLISTFRAME_OUTPUTCHANNEL:
		strText.TrimLeft ();
		if (strText == _T("n/a")) {
			strText = _T("0");
		}
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue <= -1 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pSekaijuApp->m_theCriticalSection.Lock ();
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_OUTPUTCHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �o�̓`�����l���̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetOutputChannel (pCloneTrack, CLIP (-1, lValue - 1, 15));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		pSekaijuApp->m_theCriticalSection.Unlock ();
		m_bSettingCellString = 0;
		return TRUE;
	// �R���g���[���`�F���W(CC#0, CC#32, �{�����[��, �p��, ���o�[�u, �R�[���X, �f�B���C�Ȃ�)
	case TRACKLISTFRAME_CONTROLCHANGE:
		if (lFormat == 1 && lRow == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lNumber = pTrackListFrame->GetColumnContent (lColumn) >> 16;
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < 0 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_CONTROLCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �ŏ���CC#lNumber�C�x���g��T��
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_IsControlChange (pTempEvent)) {
				if (MIDIEvent_GetNumber (pTempEvent) == lNumber) {
					break;
				}
			}
		}
		// �ŏ���CC#lNumber�C�x���g��������Ȃ������ꍇ
		if (pTempEvent == NULL) {
			long lChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			VERIFY (pTempEvent = MIDIEvent_CreateControlChange (0, lChannel, lNumber, lValue));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// �ŏ���CC#lNumber�����������ꍇ
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}
		m_bSettingCellString = 0;
		return TRUE;
	// �v���O�����`�F���W
	case TRACKLISTFRAME_PROGRAMCHANGE:
		if (lFormat == 1 && lRow == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < 0 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_0_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_PROGRAMCHANGE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		// ����Ώۂ̃g���b�N���Ȃ��ꍇ�́A�K�v�Ȑ������g���b�N��ǉ�����B
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �ŏ��̃v���O�����`�F���W�C�x���g��T��
		forEachEvent (pMIDITrack, pTempEvent) {
			if (MIDIEvent_IsProgramChange (pTempEvent)) {
				break;
			}
		}
		// �ŏ��̃v���O�����`�F���W�C�x���g��������Ȃ������ꍇ
		if (pTempEvent == NULL) {
			long lChannel = MIDITrack_GetOutputChannel (pMIDITrack);
			VERIFY (pTempEvent = MIDIEvent_CreateProgramChange (0, lChannel, lValue));
			VERIFY (MIDITrack_InsertEvent (pMIDITrack, pTempEvent));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		}
		// �ŏ��̃v���O�����`�F���W�C�x���g�����������ꍇ
		else {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		}
		m_bSettingCellString = 0;
		return TRUE;

	// �^�C��+
	case TRACKLISTFRAME_TIMEPLUS:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < -127 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_TIMEPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �^�C��+�̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetTimePlus (pCloneTrack, CLIP (-127, lValue, 127));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		m_bSettingCellString = 0;
		return TRUE;
	// �L�[+
	case TRACKLISTFRAME_KEYPLUS:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < -127 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_KEYPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �L�[+�̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetKeyPlus (pCloneTrack, CLIP (-127, lValue, 127));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		m_bSettingCellString = 0;
		return TRUE;
	// �x���V�e�B+
	case TRACKLISTFRAME_VELOCITYPLUS:
		strText.TrimLeft ();
		if (IsNumeric (strText) == 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		lValue = _ttol (strText);
		if (lValue < -127 || lValue > 127) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_INPUT_HALF_WIDTH_NUMBER_FROM_M127_TO_127));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �V���������̗p��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_VELPLUS));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		// ���݂̃g���b�N�ւ̃|�C���^���擾
		pSekaijuDoc->AddTrack (lRow + 1, 0x000F, pCurHistoryUnit);
		pMIDITrack = pSekaijuDoc->GetTrack (lRow);
		if (pMIDITrack == NULL) {
			return FALSE;
		}
		// �x���V�e�B+�̐ݒ�
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVETRACK, pMIDITrack);
		pCloneTrack = pSekaijuDoc->ReplaceMIDITrack (pMIDITrack);
		MIDITrack_SetVelocityPlus (pCloneTrack, CLIP (-127, lValue, 127));
		pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTTRACK, pCloneTrack);
		m_bSettingCellString = 0;
		return TRUE;
	// �C�x���g��
	case TRACKLISTFRAME_NUMEVENT:
		return FALSE;
	}
	m_bSettingCellString = 0;
	return TRUE;
}

// ���݂̃Z���̒����`��`��
BOOL CTrackListTrackModeView::DrawCurFrame (CDC* pDC) {
	CRect rcCurFrame = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcCurFrame.left -= 1;
	rcCurFrame.right -= 1;
	rcCurFrame.top -= 1;
	rcCurFrame.bottom -= 1;
	CPen thePen (PS_SOLID, 3, RGB (0, 0, 0));
	CPen* pOldPen = pDC->SelectObject (&thePen);
	int nOldDrawMode = pDC->SetROP2 (R2_NOT);
	pDC->MoveTo (rcCurFrame.left, rcCurFrame.top);
	pDC->LineTo (rcCurFrame.left, rcCurFrame.bottom);
	pDC->LineTo (rcCurFrame.right, rcCurFrame.bottom);
	pDC->LineTo (rcCurFrame.right, rcCurFrame.top);
	pDC->LineTo (rcCurFrame.left, rcCurFrame.top);
	pDC->SetROP2 (nOldDrawMode);
	pDC->SelectObject (pOldPen);
	return TRUE;

}

// �Z���̒����`���擾
CRect CTrackListTrackModeView::GetRectFromCell (long lRow, long lColumn) {
	long j;
	CRect rcCell (0, 0, 0 ,0);
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParentFrame ();
	for (j = 0; j < lColumn; j++) {
		rcCell.left += pTrackListFrame->GetColumnWidth (j);
	}
	rcCell.right = rcCell.left + pTrackListFrame->GetColumnWidth (j);
	rcCell.top = lRow * pTrackListFrame->GetRowZoom ();
	rcCell.bottom = (lRow + 1) * pTrackListFrame->GetRowZoom ();
	return rcCell;
}

// �e�L�X�g�{�b�N�X�̈ړ�(VISIBLE==FALSE���܂�)
BOOL CTrackListTrackModeView::MoveTextBox (long lRow, long lColumn) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	ASSERT (0 <= lRow && lRow < MAXMIDITRACKNUM);
	ASSERT (0 <= lColumn && lColumn < TRACKLISTFRAME_NUMCOLUMN);
	CRect rcNewCell = GetRectFromCell (lRow, lColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos(), pTrackListFrame->GetRowScrollPos ());
	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
	}
	else {
		lButtonWidth = 1;
	}
	m_theTextBox.MoveWindow 
		(rcNewCell.left, rcNewCell.top + pTrackListFrame->GetRowZoom () / 2 - 6, 
		rcNewCell.Width () - lButtonWidth, 13);
	return TRUE;
}




//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���_�̈ړ����I�[�o�[���C�h
void CTrackListTrackModeView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	pDC->SetWindowOrg (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
}

// �`��
void CTrackListTrackModeView::OnDraw (CDC* pDC) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos());
	CFont* pOldFont = pDC->SelectObject (&(pTrackListFrame->m_theFont));

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorGrayText = ::GetSysColor (COLOR_GRAYTEXT);

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;

	long i, j;
	long lRowZoom = pTrackListFrame->GetRowZoom ();
	long lColumnZoom = pTrackListFrame->GetColumnZoom ();
	// �s�̔w�i�F�`��
	long lTrackCount = MIDIData_CountTrack (pMIDIData);
	long lVisibleTopRow = pTrackListFrame->GetVisibleTopRow ();
	long lVisibleBottomRow = pTrackListFrame->GetVisibleBottomRow ();
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		long lBackColor = (i % 2 ? RGB(255,255,192) : RGB (255,255,255));
		if (0 <= i && i < lTrackCount) {
			pMIDITrack = pSekaijuDoc->GetTrack (i);
		}
		else {
			pMIDITrack = NULL;
		}
		if (pMIDITrack) {
			if (pSekaijuDoc->IsTrackSelected (pMIDITrack)) {
				lBackColor = (i % 2 ? RGB(224,224,192) : RGB (224,224,255));
			}
		}
		pDC->FillSolidRect 
			(rcClient.left, i * lRowZoom, 
			rcClient.Width (), (i + 1) * lRowZoom, 
			lBackColor);
	}
	// �����̕`��
	CPen penTrack (PS_SOLID, 1, RGB (128, 128, 128));
	CPen* pOldPen = pDC->SelectObject (&penTrack);
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		long y = i * lRowZoom;
		pDC->MoveTo (rcClient.left, y - 1);
		pDC->LineTo (rcClient.right, y - 1);
	}
	pDC->SelectObject (pOldPen);
	// �c���̕`��
	CPen penColumn (PS_SOLID, 1, RGB (128, 128, 128));
	pOldPen = pDC->SelectObject (&penColumn);
	long x = 0;
	for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
		pDC->MoveTo (x - 1, rcClient.top);
		pDC->LineTo (x - 1, rcClient.bottom);
		x += pTrackListFrame->GetColumnBaseWidth (j) * lColumnZoom;
	}
	pDC->SelectObject (pOldPen);


	// �����̕`��
	CRect rcText (0, 0, 0, 0);
	pDC->SetBkMode (TRANSPARENT);
	i = 0;
	j = 0;
	// �`�悷�ׂ��J�n�s�ƏI���s�̌v�Z
	long lStartRow = __max (lVisibleTopRow, 0);
	long lEndRow = __min (MAXMIDITRACKNUM, lVisibleBottomRow + 1);
	// �e�Z���̕����Ȃǂ̕`��
	for (i = lStartRow; i < lEndRow; i++) {
		if (i < lTrackCount) {
			pMIDITrack = pSekaijuDoc->GetTrack (i);
		}
		else {
			pMIDITrack = NULL;
		}
		if (pMIDITrack) {
			long lTrackForeColor = MIDITrack_GetForeColor (pMIDITrack);
			pDC->SetTextColor (lTrackForeColor);
			// �e��ɂ���
			for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
				CRect rcCell = GetRectFromCell (i, j);
				CRect rcText = rcCell;
				long lColumnContent = pTrackListFrame->GetColumnContent (j);
				if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
					rcText.right -= 10;
				}
				// �e�Z���̕����`��
				CString strCellString;
				CRect rcColor;
				switch (lColumnContent & 0xFFFF) {
				case TRACKLISTFRAME_FORECOLOR:
					rcColor.left = rcCell.left + 2;
					rcColor.right = rcCell.right - 3;
					rcColor.top = rcCell.top + lRowZoom / 2 - 6;
					rcColor.bottom = rcCell.top + lRowZoom / 2 + 6;
					pDC->FillSolidRect (&rcColor, lTrackForeColor); 
					break;
				case TRACKLISTFRAME_TRACKNAME:
				case TRACKLISTFRAME_VISIBLE:
				case TRACKLISTFRAME_ENABLE:
				case TRACKLISTFRAME_INPUTON:
				case TRACKLISTFRAME_INPUTPORT:
				case TRACKLISTFRAME_INPUTCHANNEL:
				case TRACKLISTFRAME_OUTPUTON:
				case TRACKLISTFRAME_OUTPUTPORT:
				case TRACKLISTFRAME_OUTPUTCHANNEL:
				case TRACKLISTFRAME_VIEWMODE:
				case TRACKLISTFRAME_CONTROLCHANGE:
				case TRACKLISTFRAME_PROGRAMCHANGE:
				case TRACKLISTFRAME_TIMEPLUS:
				case TRACKLISTFRAME_KEYPLUS:
				case TRACKLISTFRAME_VELOCITYPLUS:
				case TRACKLISTFRAME_NUMEVENT:
					strCellString = GetCellString (i, j);
					pDC->DrawText (strCellString, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
					break;
				}
			}
		}
	}	

	// �e�Z���̃{�^���ނ̕`��
	CPen penColorBtnText (PS_SOLID, 1, lColorBtnText);
	CPen penColorGrayText (PS_SOLID, 1, lColorGrayText);
	CBrush brsColorBtnText;
	CBrush brsColorGrayText;
	brsColorBtnText.CreateSolidBrush (lColorBtnText);
	brsColorGrayText.CreateSolidBrush (lColorGrayText);
	CBrush* pOldBrush = NULL;

	CRect rcButton;
	CRect rcCell;
	POINT pt[3];
	pOldPen = pDC->SelectObject (&penColorBtnText);
	pOldBrush = pDC->SelectObject (&brsColorBtnText);
	
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		for (j = 0; j < TRACKLISTFRAME_NUMCOLUMN; j++) {
			long lColumnContent = pTrackListFrame->GetColumnContent (j);
			if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				rcCell = GetRectFromCell (i, j);
				rcButton = rcCell;
				// ��{�^��
				rcButton.left = rcCell.right - lButtonWidth;
				rcButton.bottom = rcCell.top + lRowZoom / 2;
				pDC->FillSolidRect (&rcButton, ::GetSysColor (COLOR_3DFACE));
				if (i == m_lCurRow && j == m_lCurColumn && (m_lCurButtonState & 0x01) == 0x01) {
					pDC->Draw3dRect (&rcButton, lColorBtnShadow, lColorBtnHighlight);
				}
				else {
					pDC->Draw3dRect (&rcButton, lColorBtnHighlight, lColorBtnShadow);
				}
				pt[0].x = rcCell.right - lButtonWidth / 2;
				pt[0].y = rcCell.top + lRowZoom * 1 / 4 - 1;
				pt[1].x = pt[0].x - 2;
				pt[1].y = pt[0].y + 2;
				pt[2].x = pt[0].x + 2;
				pt[2].y = pt[0].y + 2;
				pDC->Polygon (pt, 3);
				// ���{�^��
				rcButton.top = rcCell.top + lRowZoom / 2;
				rcButton.bottom = rcCell.bottom;
				pDC->FillSolidRect (&rcButton, ::GetSysColor (COLOR_3DFACE));
				if (i == m_lCurRow && j == m_lCurColumn && (m_lCurButtonState & 0x02) == 0x02) {
					pDC->Draw3dRect (&rcButton, lColorBtnShadow, lColorBtnHighlight);
				}
				else {
					pDC->Draw3dRect (&rcButton, lColorBtnHighlight, lColorBtnShadow);
				}
				pt[0].x = rcCell.right - lButtonWidth / 2;
				pt[0].y = rcCell.top + lRowZoom * 3 / 4 + 1;
				pt[1].x = pt[0].x - 2;
				pt[1].y = pt[0].y - 2;
				pt[2].x = pt[0].x + 2;
				pt[2].y = pt[0].y - 2;
				pDC->Polygon (pt, 3);
			}
		}
	}
	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldBrush);

	// �J�����g�Z���̘g�`��
	DrawCurFrame (pDC);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �R�}���h���[�e�B���O�̃I�[�o�[���C�h
BOOL CTrackListTrackModeView::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	// ����:pHandlerInfo==NULL�̎��̓R�}���h�����s���ANULL�łȂ��Ƃ���
	// �R�}���h�͎��s����pHandlerInfo�̒��g��ݒ肷�邱�Ƃ��Ӗ�����B
	if (nCode == CN_COMMAND && pHandlerInfo == NULL) {
	}
	return CView::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
BOOL CTrackListTrackModeView::OnCreate (LPCREATESTRUCT lpcs) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParentFrame ();
	ASSERT (pTrackListFrame);
	m_theTextBox.Create (WS_CHILD /*| WS_VISIBLE | WS_BORDER*/ | ES_AUTOHSCROLL, 
		CRect (0, pTrackListFrame->GetRowZoom () / 2 - 6, 
		pTrackListFrame->GetColumnWidth (0), 13), this, IDC_TEXTBOX);
	m_theTextBox.SetFont (&(pTrackListFrame->m_theFont));
	return CSekaijuView::OnCreate (lpcs);
	
}

// �t�H�[�J�X���������Ƃ�
void CTrackListTrackModeView::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CTrackListTrackModeView::OnKillFocus (pNewWnd=0x%08x)\n", (long)pNewWnd);
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	// �C���v���[�X�e�L�X�g�{�b�N�X�Ƀt�H�[�J�X���ڂ����ꍇ������
	if (pNewWnd != &m_theTextBox && !m_bSettingCellString) {
		// �e�L�X�g�ҏW���̏ꍇ�͂��̓��e���m��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
		// �L�[�{�[�h�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	CView::OnKillFocus (pNewWnd);
}

// �L�[����������
void CTrackListTrackModeView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {

	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos());
	
	// �Â��Z���̍ĕ`��(20100128:������h�~�̂��ߏ������ǉ�)
	if (IsTextEditing () == FALSE && nChar != VK_CONTROL && nChar != VK_SHIFT && nChar != VK_MENU) {
		CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos());
		rcOldCell.InflateRect (2, 2);
		InvalidateRect (&rcOldCell);
	}

	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	long lFormat = MIDIData_GetFormat (pMIDIData);

	switch (nChar) {
	// ��L�[
	case VK_UP:
		// �e�L�X�g�ҏW���̏ꍇ�͂��̓��e���m��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// �L�[�{�[�h�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// �J�����g�Z�����ЂƂ�ֈړ�
		m_lCurRow = CLIP (0, m_lCurRow - 1, MAXMIDITRACKNUM - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// ���L�[
	case VK_LEFT:
		// �e�L�X�g�ҏW���̏ꍇ�͂��̓��e���m��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// �L�[�{�[�h�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// �J�����g�Z�����ЂƂ��ֈړ�
		m_lCurColumn = CLIP (0, m_lCurColumn - 1, TRACKLISTFRAME_NUMCOLUMN - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// �E�L�[
	case VK_RIGHT:
		// �e�L�X�g�ҏW���̏ꍇ�͂��̓��e���m��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// �L�[�{�[�h�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// �J�����g�Z�����ЂƂE�ֈړ�
		m_lCurColumn = CLIP (0, m_lCurColumn + 1, TRACKLISTFRAME_NUMCOLUMN - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// ���L�[
	case VK_DOWN:
		// �e�L�X�g�ҏW���̏ꍇ�͂��̓��e���m��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// �L�[�{�[�h�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// �J�����g�Z�����ЂƂ��ֈړ�
		m_lCurRow = CLIP (0, m_lCurRow + 1, MAXMIDITRACKNUM - 1);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		break;
	// ���s(Enter)�L�[
	case VK_RETURN:
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// �L�[�{�[�h�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x0F00) {
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xF0F0;
		}
		// �e�L�X�g�ҏW���łȂ��ꍇ
		if (IsTextEditing () == FALSE) {
			// ���A���^�C�����͒��͕ҏW�֎~
			if (pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// ���b�N����Ă���ꍇ�͕ҏW�֎~
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else {
				// �����񖔂͐��l�̗�
				if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_TRACKNAME ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
					BeginTextEditing ();
				}
				// �F
				else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_FORECOLOR ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_BACKCOLOR) {
					if (DoColorEditing () == TRUE) {
						pSekaijuDoc->SetModifiedFlag (TRUE);
						pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					}
				}
				// BOOL�^
				else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_VISIBLE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_ENABLE ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTON ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTON ||
					(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VIEWMODE) {
					ToggleValueOfCurCell ();
					pSekaijuDoc->SetModifiedFlag (TRUE);
					pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
				}
			}
		}
		// �e�L�X�g�ҏW���̏ꍇ(CInplaceEdit�N���X����PostMessage�����B)
		else {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
		break;
	// �G�X�P�[�v(Esc)�L�[(CInplaceEdit�N���X����PostMessage�����B)
	case VK_ESCAPE:
		EndTextEditingCancel ();
		break;
	// +�L�[
	case VK_ADD:
	case 187:
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			KillTimer (1);
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// ���ڂ����l�̏ꍇ�̂�
		if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
			// ���A���^�C�����͒��͕ҏW�֎~
			if (pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// ���b�N����Ă���ꍇ�͕ҏW�֎~
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else { // 20091126else��ǉ�
				// ���񉟂����̂݃L�[�{�[�h����J�n
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					SetCapture ();
					m_lCurButtonState |= 0x0101;
				}
				// �l�̑���
				if (::GetKeyState (VK_SHIFT) < 0) {
					AddValueOfCurCell (10);
				}
				else {
					AddValueOfCurCell (1);
				}
			}
		}
		break;
	// -�L�[
	case VK_SUBTRACT:
	case 189:
		// �}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			KillTimer (1);
			ReleaseCapture ();
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		// ���ڂ����l�̏ꍇ�̂�
		if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
			// ���A���^�C�����͒��͕ҏW�֎~
			if (pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// ���b�N����Ă���ꍇ�͕ҏW�֎~
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else { // 20091126else��ǉ�
				// ���񉟂����̂݃L�[�{�[�h����J�n
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					SetCapture ();
					m_lCurButtonState |= 0x0202;
				}
				// �l�̑���
				if (::GetKeyState (VK_SHIFT) < 0) {
					AddValueOfCurCell (-10);
				}
				else {
					AddValueOfCurCell (-1);
				}
			}
		}
		break;
	// Delete�L�[ (20090823�ǉ�)
	case VK_DELETE:
		pTrackListFrame->PostMessage (WM_COMMAND, ID_TRACKLIST_DELETETRACK, NULL);
		break;
	// Insert�L�[ (20090823�ǉ�)
	case VK_INSERT:
		// Ctrl��������Ă���
		if (GetKeyState (VK_CONTROL) < 0) { 
			pTrackListFrame->PostMessage (WM_COMMAND, ID_TRACKLIST_DUPLICATETRACK, NULL);
		}
		// Ctrl��������Ă��Ȃ�
		else {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_TRACKLIST_INSERTTRACK, NULL);
		}
		break;
	}
	
	// �V�����Z���̕`��(20100128:������h�~�̂��ߏ������ǉ�)
	if (IsTextEditing () == FALSE && nChar != VK_CONTROL && nChar != VK_SHIFT && nChar != VK_MENU) {
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell);
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �L�[�����グ��
void CTrackListTrackModeView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	switch (nChar) {
	// +�L�[
	case VK_ADD:
	case 187:
		// �L�[�{�[�h�ő��쒆�̏ꍇ
		if (m_lCurButtonState & 0x0F00) {
			// +�L�[����̂ݒ�~
			m_lCurButtonState &= ~0x0101;
			// ���͂≽�̑�����s���Ă��Ȃ��Ȃ��
			if (m_lCurButtonState == 0x0000) {
				ReleaseCapture ();
				EndValueUpDown ();
				pSekaijuDoc->SetModifiedFlag (TRUE);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			}
		}
		break;
	// -�L�[
	case VK_SUBTRACT:
	case 189:
		// �L�[�{�[�h�ő��쒆�̏ꍇ
		if (m_lCurButtonState & 0x0F00) {
			// -�L�[����̂ݒ�~
			m_lCurButtonState &= ~0x0202;
			// ���͂≽�̑�����s���Ă��Ȃ��Ȃ��
			if (m_lCurButtonState == 0x0000) {
				ReleaseCapture ();
				EndValueUpDown ();
				pSekaijuDoc->SetModifiedFlag (TRUE);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
			}
		}
		break;
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �������͎�
void CTrackListTrackModeView::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
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

	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_TRACKNAME && 32 <= nChar && nChar <= 126 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE && 48 <= nChar && nChar <= 57 ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS && (48 <= nChar && nChar <= 57 || nChar == '-') ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS && (48 <= nChar && nChar <= 57 || nChar == '-') ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS && (48 <= nChar && nChar <= 57 || nChar == '-')) {
		BeginTextEditing ();
		//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
		m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
		// ��20080722�F���̍s��Windows2000�Ή��B
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();

	::Sleep (0);
}

// �}�E�X���{�^�������ꂽ��  
void CTrackListTrackModeView::OnLButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	BOOL bOldTextEditing = FALSE;
	// �e�L�X�g�ҏW�̏I��
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
		bOldTextEditing = TRUE;
	}
	
	// �Â��Z���̕`��
	CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcOldCell.InflateRect (2, 2);
	InvalidateRect (&rcOldCell);

	// �V�����Z���̐ݒ�
	if (m_lCurButtonState == 0x0000) {
		long i, j;
		GetCellFromPoint (point, &i, &j);
		if (0 <= i && i < MAXMIDITRACKNUM && 0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
			m_lCurRow = i;
			m_lCurColumn = j;
		}
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
	}
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	
	// �{�^���̉������𔻕�
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
		// �㖔�͉��̃{�^���������ꂽ�ꍇ
		if (point.x > rcNewCell.right - lButtonWidth) {
			long lFormat = MIDIData_GetFormat (pMIDIData);
			// �^�����͉������Ȃ�
			if (pSekaijuApp->m_bRecording) {
				;
			}
			// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
			else if (pSekaijuDoc->m_bEditLocked) {
				;
			}
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			else if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else {
				// �}�E�X�ɂ�鑀����J�n�ł���ꍇ
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					// ��̃{�^���������ꂽ�ꍇ
					if (point.y < rcNewCell.top + pTrackListFrame->GetRowZoom () / 2) {
						m_lCurButtonState |= 0x0011;
						AddValueOfCurCell (1);
					}
					// ���̃{�^���������ꂽ�ꍇ
					else {
						m_lCurButtonState |= 0x0012;
						AddValueOfCurCell (-1);
					}
					m_lCurButtonInterval = 200;
					SetTimer (1, m_lCurButtonInterval, NULL);
					SetCapture ();
				}
				// ���Ƀ}�E�X���삪�Ȃ���Ă���ꍇ
				else if (m_lCurButtonState & 0x00F0) {
					m_lCurButtonState |= 0x0010;
				}
			}
		}
	}

	// �V�����Z���̕`��
	rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �}�E�X�E�{�^�������ꂽ��  
void CTrackListTrackModeView::OnRButtonDown (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();

	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;

	CPoint ptMenu (point);
	ClientToScreen (&ptMenu);
	
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	BOOL bOldTextEditing = FALSE;
	// �e�L�X�g�ҏW�̏I��
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
		bOldTextEditing = TRUE;
	}
	
	// �Â��Z���̕`��
	CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcOldCell.InflateRect (2, 2);
	InvalidateRect (&rcOldCell);
	
	// �V�����Z���̐ݒ�
	if (m_lCurButtonState == 0x0000) {
		long i, j;
		GetCellFromPoint (point, &i, &j);
		if (0 <= i && i < MAXMIDITRACKNUM && 0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
			m_lCurRow = i;
			m_lCurColumn = j;
		}
		MoveTextBox (m_lCurRow, m_lCurColumn);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
	}
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);

	// �{�^���̉������𔻕�
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
		// �㖔�͉��̃{�^���������ꂽ�ꍇ
		if (point.x > rcNewCell.right - lButtonWidth) {
			long lFormat = MIDIData_GetFormat (pMIDIData);
			// �^�����͉������Ȃ�
			if (pSekaijuApp->m_bRecording) {
				;
			}
			// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
			else if (pSekaijuDoc->m_bEditLocked) {
				;
			}
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			else if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else {
				// �}�E�X�ɂ�鑀����J�n�ł���ꍇ
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					// ��̃{�^���������ꂽ�ꍇ
					if (point.y < rcNewCell.top + pTrackListFrame->GetRowZoom () / 2) {
						m_lCurButtonState |= 0x0021;
						AddValueOfCurCell (10);
					}
					// ���̃{�^���������ꂽ�ꍇ
					else {
						m_lCurButtonState |= 0x0022;
						AddValueOfCurCell (-10);
					}
					m_lCurButtonInterval = 200;
					SetTimer (1, m_lCurButtonInterval, NULL);
					SetCapture ();
				}
				// ���Ƀ}�E�X���삪�Ȃ���Ă���ꍇ
				else if (m_lCurButtonState & 0x00F0) {
					m_lCurButtonState |= 0x0020;
				}
			}
		}
		// �{�^���͉�����Ă��Ȃ��ꍇ
		else {
			long lTrackIndex = pTrackListFrame->YtoRow (point.y);
			pSekaijuDoc->m_lTempTime = pTrackListFrame->XtoTime (point.x);
			pSekaijuDoc->m_pTempTrack = pSekaijuDoc->GetTrack (lTrackIndex);
			pSekaijuDoc->m_pTempEvent = NULL;
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);

			CMenu theMenu;
			VERIFY (theMenu.LoadMenu (IDR_POPUPMENU01));
			CMenu* pContextMenu = theMenu.GetSubMenu (0);
			pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				ptMenu.x, ptMenu.y, pTrackListFrame);
			
			pSekaijuDoc->m_theCriticalSection.Lock ();
		}
	}
	// �{�^���̂Ȃ��Z���̏ꍇ
	else {
		long lTrackIndex = pTrackListFrame->YtoRow (point.y);
		pSekaijuDoc->m_lTempTime = pTrackListFrame->XtoTime (point.x);
		pSekaijuDoc->m_pTempTrack = pSekaijuDoc->GetTrack (lTrackIndex);
		pSekaijuDoc->m_pTempEvent = NULL;
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
		
		CMenu theMenu;
		VERIFY (theMenu.LoadMenu (IDR_POPUPMENU01));
		CMenu* pContextMenu = theMenu.GetSubMenu (0);
		pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			ptMenu.x, ptMenu.y, pTrackListFrame);
		
		pSekaijuDoc->m_theCriticalSection.Lock ();
	}

	// �V�����Z���̕`��
	rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CTrackListTrackModeView::OnLButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
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
	
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	// ���}�E�X�ő��쒆�̏ꍇ�̂�
	if (m_lCurButtonState & 0x0010) {
		// ���}�E�X����̏I��
		m_lCurButtonState &= ~0x0010;
		// ���͂�}�E�X�ő��삳��Ă��Ȃ��ꍇ
		if ((m_lCurButtonState & 0x00F0) == 0x0000) {
			EndValueUpDown ();
			m_lCurButtonState = 0x0000;
			KillTimer (1);
			ReleaseCapture ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
	}

	// �V�����Z���̕`��
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CTrackListTrackModeView::OnRButtonUp (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
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

	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	
	pSekaijuDoc->m_theCriticalSection.Lock ();

	// �E�}�E�X�ő��쒆�̏ꍇ�̂�
	if (m_lCurButtonState & 0x0020) {
		// �E�}�E�X����̏I��
		m_lCurButtonState &= ~0x0020;
		// ���͂�}�E�X�ő��삳��Ă��Ȃ��ꍇ
		if ((m_lCurButtonState & 0x00F0) == 0x0000) {
			EndValueUpDown ();
			m_lCurButtonState = 0x0000;
			KillTimer (1);
			ReleaseCapture ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
		}
	}

	// �V�����Z���̕`��
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X���������ꂽ�Ƃ�
void CTrackListTrackModeView::OnMouseMove (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

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

}

// �}�E�X���{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CTrackListTrackModeView::OnLButtonDblClk (UINT nFlags, CPoint point) {

	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

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
	
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lFormat = MIDIData_GetFormat (pMIDIData);

	long lButtonWidth = 12;//::GetSystemMetrics (SM_CXVSCROLL);
	// OnLButtonDown�����s���A���ݕҏW���̃Z��������߁A�V�����t�H�[�J�X�ɃZ�����ړ�
	SendMessage (WM_LBUTTONDOWN, nFlags, ((point.y  & 0x0000FFFF) << 16) | (point.x & 0x0000FFFF));
	if (IsTextEditing () == TRUE) {
		return;
	}
	if (m_lCurButtonState != 0x0000) {
		return;
	}

	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

	// �V�����Z���̎擾
	long i, j;
	GetCellFromPoint (point, &i, &j);
	if (0 <= i && i < MAXMIDITRACKNUM && 0 <= j && j < TRACKLISTFRAME_NUMCOLUMN) {
		m_lCurRow = i;
		m_lCurColumn = j;
	}
	CRect rcCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	
	ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
	long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
	if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_TRACKNAME ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
		// �Z�����̃{�^���ȊO�̕���
		if (point.x < rcCell.right - lButtonWidth) {
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else {
				// �e�L�X�g�ҏW���[�h�ɓ˓�
				BeginTextEditing ();
			}
		}
	}
	else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_FORECOLOR ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_BACKCOLOR) {
		// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
		if (lFormat == 0 && m_lCurRow >= 1) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
		}
		// ���݂̃Z���𑀍�\�ȏꍇ
		else {
			// �J���[�_�C�A���O�\��
			if (DoColorEditing () == TRUE) {
				pSekaijuDoc->SetModifiedFlag (TRUE);
				pSekaijuDoc->UpdateAllViews (NULL); // TODO
			}
		}
	}
	else if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_VISIBLE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_ENABLE ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTON ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTON ||
		(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VIEWMODE) {
		// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
		if (lFormat == 0 && m_lCurRow >= 1) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
			pSekaijuDoc->m_theCriticalSection.Lock ();
		}
		// ���݂̃Z���𑀍�\�ȏꍇ
		else {
			// �l�̔��]
			ToggleValueOfCurCell ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews (NULL); // TODO
		}
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CTrackListTrackModeView::OnRButtonDblClk (UINT nFlags, CPoint point) {

}

// �^�C�}�[��
void CTrackListTrackModeView::OnTimer (UINT nIDEvent) {	
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*) GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	if (nIDEvent == 1) {
		// �^�C�}�[�̉���
		if (m_lCurButtonInterval > 50) {
			KillTimer (1);
			m_lCurButtonInterval = 50;
			SetTimer (1, m_lCurButtonInterval, NULL);
		}
		// ��{�^�������N���b�N�ŉ�����Ă���ꍇ
		if ((m_lCurButtonState & 0x00FF) == 0x0011) {
			AddValueOfCurCell (1);
		}
		// ���{�^�������N���b�N�ŉ�����Ă���ꍇ
		else if ((m_lCurButtonState & 0x00FF) == 0x0012) {
			AddValueOfCurCell (-1);
		}
		// ��{�^�����E�N���b�N�ŉ�����Ă���ꍇ
		if ((m_lCurButtonState & 0x00FF) == 0x0021) {
			AddValueOfCurCell (10);
		}
		// ���{�^�����E�N���b�N�ŉ�����Ă���ꍇ
		else if ((m_lCurButtonState & 0x00FF) == 0x0022) {
			AddValueOfCurCell (-10);
		}
		// ��{�^�������N���b�N�ŉ�����Ă���ꍇ
		if ((m_lCurButtonState & 0x00FF) == 0x0031) {
			AddValueOfCurCell (10);
		}
		// ���{�^�������N���b�N�ŉ�����Ă���ꍇ
		else if ((m_lCurButtonState & 0x00FF) == 0x0032) {
			AddValueOfCurCell (-10);
		}
	}
	// �Z���̍ĕ`��
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell, FALSE);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �}�E�X�z�C�[�����񂳂ꂽ��
void CTrackListTrackModeView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CTrackListFrame* pTrackListFrame = (CTrackListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	// ���t�ʒu�̈ړ�
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pTrackListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	// �J�����g�Z���̒l�̑���
	else if (GetKeyState (VK_MENU) < 0) {
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

		CRect rcClient;
		GetClientRect (&rcClient);
		rcClient += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		point += CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());

		// �e�L�X�g�ҏW�̏I��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
		}
		
		// �Â��Z���̕`��
		CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcOldCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		rcOldCell.InflateRect (2, 2);
		InvalidateRect (&rcOldCell);

		// �V�����Z���̐ݒ�
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		
		// �{�^���̉������𔻕�
		ASSERT (0 <= m_lCurColumn && m_lCurColumn < TRACKLISTFRAME_NUMCOLUMN);
		long lColumnContent = pTrackListFrame->GetColumnContent (m_lCurColumn);
		if ((lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_INPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTPORT ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_OUTPUTCHANNEL ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_CONTROLCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_PROGRAMCHANGE ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS ||
			(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
			long lFormat = MIDIData_GetFormat (pMIDIData);
			// �t�H�[�}�b�g0�̏ꍇ�͍ŏ��̃g���b�N�ȊO����s�\
			if (lFormat == 0 && m_lCurRow >= 1) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_EDIT_THIS_TRACK_IN_FORMAT0_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃR���g���[���`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_CONTROLCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_CONTROLCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńv���O�����`�F���W�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == MIDIEVENT_PROGRAMCHANGE) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_PROGRAMCHANGE_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ń^�C��+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_TIMEPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_TIMEPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�ŃL�[+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_KEYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_KEYPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// �t�H�[�}�b�g1�̏ꍇ�͍ŏ��̃g���b�N�Ńx���V�e�B+�̎g�p�s��
			else if (lFormat == 1 && m_lCurRow == 0 &&
				(lColumnContent & 0xFFFF) == TRACKLISTFRAME_VELOCITYPLUS) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SET_VELPLUS_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
				pSekaijuDoc->m_theCriticalSection.Lock ();
			}
			// ���݂̃Z���𑀍�\�ȏꍇ
			else {
				// �}�E�X�ɂ�鑀����J�n�ł���ꍇ
				if (m_lCurButtonState == 0x0000) {
					long lDeltaValue = GetKeyState (VK_SHIFT) < 0 ? 10 : 1;
					BeginValueUpDown ();
					// ��񂵂̏ꍇ
					if (lDelta > 0) {
						AddValueOfCurCell (lDeltaValue);
					}
					// ���񂵂̏ꍇ
					else {
						AddValueOfCurCell (-lDeltaValue);
					}
					EndValueUpDown ();
					pSekaijuDoc->SetModifiedFlag (TRUE);
					pSekaijuDoc->UpdateAllViews 
						(NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
				}
			}
		}

		// �V�����Z���̕`��
		rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pTrackListFrame->GetColumnScrollPos (), pTrackListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell);

		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}
	// ��ʃX�N���[��
	else {
		long lRowScrollPos = pTrackListFrame->GetRowScrollPos ();
		long lRowZoom = pTrackListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pTrackListFrame->SetRowScrollPos (lRowScrollPos);
	}
}
