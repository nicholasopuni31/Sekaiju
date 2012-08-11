//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�C���f�b�N�X�v���p�e�B�r���[�N���X
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
#include "common.h"
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
#include "EventListFrame.h"
#include "EventListIndexPropertyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDC_TEXTBOX 3939

IMPLEMENT_DYNCREATE (CEventListIndexPropertyView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CEventListIndexPropertyView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
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
CEventListIndexPropertyView::CEventListIndexPropertyView () {
	m_lCurRow = 0;
	m_lCurColumn = 0;
	m_lCurButtonState = 0x00;
	m_lCurButtonInterval = 200;
	m_bSettingCellString = 0;
}

// �f�X�g���N�^
CEventListIndexPropertyView::~CEventListIndexPropertyView () {
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// (X,Y)���W����Z���ԍ����擾
BOOL CEventListIndexPropertyView::GetCellFromPoint 
(CPoint pt, long* pRow, long* pColumn) {
	_ASSERT (pRow);
	_ASSERT (pColumn);
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	*pRow = pEventListFrame->YtoRow (pt.y);
	*pColumn = pEventListFrame->XtoColumn (pt.x);
	return TRUE;
}

// �w��Z�����r���[����͂ݏo�����ꍇ�̃I�[�g�X�N���[������
BOOL CEventListIndexPropertyView::AutoScrolltoShowCell (long lRow, long lColumn) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	long lNewPos = 0;
	// �w��Z�����r���[�̍��ɂ͂ݏo�����ꍇ�̏���
	if (pEventListFrame->GetColumnLeft (lColumn) < rcClient.left) {
		lNewPos = pEventListFrame->GetColumnLeft (lColumn);
		pEventListFrame->SetColumnScrollPos (lNewPos);
	}
	// �w��Z�����r���[�̉E�ɂ͂ݏo�����ꍇ�̏���
	else if (pEventListFrame->GetColumnLeft (lColumn) + 
		pEventListFrame->GetColumnWidth (lColumn) > rcClient.right) {
		lNewPos = pEventListFrame->GetColumnLeft (lColumn) +
			pEventListFrame->GetColumnWidth (lColumn) - rcClient.Width ();
		pEventListFrame->SetColumnScrollPos (lNewPos);
	}
	// �w��Z�����r���[�̏�ɂ͂ݏo�����ꍇ�̏���
	if (lRow * pEventListFrame->GetRowZoom () < rcClient.top) {
		lNewPos = lRow * pEventListFrame->GetRowZoom ();
		pEventListFrame->SetRowScrollPos (lNewPos);		
	}
	// �w��Z�����r���[�̉��ɂ͂ݏo�����ꍇ�̏���
	else if ((lRow + 1) * pEventListFrame->GetRowZoom ()  > rcClient.bottom) {
		lNewPos = (lRow + 1) * pEventListFrame->GetRowZoom () - rcClient.Height ();
		pEventListFrame->SetRowScrollPos (lNewPos);
	}
	return TRUE;
}

// ���݃e�L�X�g�{�b�N�X�ŕҏW�����ǂ����Ԃ��B
BOOL CEventListIndexPropertyView::IsTextEditing () {
	return (m_theTextBox.GetStyle () & WS_VISIBLE) ? TRUE : FALSE;
}

// ���݃��X�g�{�b�N�X�őI�𒆂��ǂ����Ԃ��B
BOOL CEventListIndexPropertyView::IsListSelecting () {
	return (m_theListBox.GetStyle () & WS_VISIBLE) ? TRUE : FALSE;
}

// �e�L�X�g�{�b�N�X�ł̕ҏW���J�n����B
BOOL CEventListIndexPropertyView::BeginTextEditing () {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	MIDIEvent* pMIDIEvent = NULL;
	ASSERT (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount);
	//if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
	VERIFY (pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow));
	//}
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	pSekaijuApp->m_bInplaceEditing = 1;
	m_theTextBox.SetWindowText (GetCellString (m_lCurRow, m_lCurColumn));
	m_theTextBox.SetSel (0, -1, TRUE);
	m_theTextBox.EmptyUndoBuffer ();
	m_theTextBox.ShowWindow (SW_SHOW);
	m_theTextBox.SetFocus ();
	m_theTextBox.UpdateWindow ();
	return TRUE;
}

// ���X�g�{�b�N�X�ł̕ҏW���J�n����B
BOOL CEventListIndexPropertyView::BeginListSelecting () {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = pEventListFrame->GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pMIDIData->m_pFirstTrack;
	MIDIEvent* pMIDIEvent = NULL;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	pSekaijuApp->m_bInplaceEditing = 1;
	// �g���b�N�̏ꍇ
	if (m_lCurColumn == 0) {
		long i = 0;
		TCHAR szText[1024];
		CString strText;
		m_theListBox.ResetContent ();
		forEachTrack (pMIDIData, pMIDITrack) {
			memset (szText, 0, sizeof (szText));
			MIDITrack_GetName (pMIDITrack, szText, TSIZEOF (szText) - 1);
			strText.Format (_T("%d-%s"), i + (bTrackZeroOrigin ? 0 : 1), szText);
			m_theListBox.AddString (strText);
			i++;
		}
	}
	// �C�x���g�̎�ނ̏ꍇ
	else if (m_lCurColumn == 3) {
		m_theListBox.ResetContent ();
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x00]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x01]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x02]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x03]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x04]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x05]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x06]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x07]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x08]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x09]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x20]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x21]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x2F]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x51]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x54]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x58]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x59]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x7F]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x80]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0x90]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xA0]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xB0]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xC0]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xD0]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xE0]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xF0]);
		m_theListBox.AddString (pSekaijuApp->m_strEventKindName[0xF7]);
	}
	long i = 0;
	long lCount = m_theListBox.GetCount ();
	CString strCurCellString = GetCellString (m_lCurRow, m_lCurColumn);
	CString strListBoxString;
	for (i = 0; i < lCount; i++) {
		m_theListBox.GetText (i, strListBoxString);
		if (strListBoxString == strCurCellString) {
			m_theListBox.SetCurSel (i);
		}
	}
	m_theListBox.ShowWindow (SW_SHOW);
	m_theListBox.SetFocus ();
	m_theListBox.UpdateWindow ();
	return TRUE;
}




// �e�L�X�g�{�b�N�X�ł̕ҏW���I�����A�V�����l���i�[����B
BOOL CEventListIndexPropertyView::EndTextEditingOK () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// ���݂̃Z���̕ҏW�e�L�X�g���擾
	CString strBuf;
	m_theTextBox.GetWindowText (strBuf);
	// �l�̔��f
	if (SetCellString (m_lCurRow, m_lCurColumn, strBuf)) {
		CSekaijuDoc* pSekaijuDoc = GetDocument ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
		pSekaijuDoc->UpdateAllViews 
			(NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
	}
	// �ҏW�I��	
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// ���X�g�{�b�N�X�ł̑I�����I�����A�V�����l���i�[����B
BOOL CEventListIndexPropertyView::EndListSelectingOK () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// ���݂̃Z���̑I���e�L�X�g���擾
	int nCurSel = m_theListBox.GetCurSel ();
	if (nCurSel == LB_ERR) {
		return FALSE;
	}
	CString strText;
	m_theListBox.GetText (nCurSel, strText);
	// �l�̔��f
	if (SetCellString (m_lCurRow, m_lCurColumn, strText)) {
		CSekaijuDoc* pSekaijuDoc = GetDocument ();
		pSekaijuDoc->SetModifiedFlag (TRUE);
		pSekaijuDoc->UpdateAllViews 
			(NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
	}
	// �I���I��	
	m_theListBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}


// �e�L�X�g�{�b�N�X�ł̕ҏW���I�����A�V�����l���i�[���Ȃ��B
BOOL CEventListIndexPropertyView::EndTextEditingCancel () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theTextBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// ���X�g�{�b�N�X�ł̑I�����I�����A�V�����l���i�[���Ȃ��B
BOOL CEventListIndexPropertyView::EndListSelectingCancel () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	m_theListBox.ShowWindow (SW_HIDE);
	this->SetFocus ();
	pSekaijuApp->m_bInplaceEditing = 0;
	return TRUE;
}

// �l�̑������J�n����
BOOL CEventListIndexPropertyView::BeginValueUpDown () {
	_RPTF0 (_CRT_WARN, "BeginValueUpDown\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (m_lCurButtonState == 0x0000);
	MIDIEvent* pTempEvent = NULL;
	MIDITrack* pTempTrack = NULL;
	MIDITrack* pCloneTrack = NULL;
	MIDIEvent* pCloneEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	m_pLastEvent = NULL;
	m_pCloneEvent = NULL;
	m_pCloneTrack = NULL;
	CString strHistoryName;
	// ���݂̃C�x���g�ƃg���b�N���擾
	VERIFY (pTempEvent = pEventListFrame->GetVisibleEvent (m_lCurRow));
	VERIFY (pTempTrack = MIDIEvent_GetParent (pTempEvent));
	// ���ڕʂ�
	switch (m_lCurColumn) {
	case 2: // ���߁F���F�e�B�b�N
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_TIME));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
		if (pLastEvent) {
			if (pLastEvent != pTempEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
					VERIFY (pEventListFrame->ReplaceVisibleEvent (pLastEvent, m_pLastEvent));
				}
			}
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		VERIFY (pEventListFrame->ReplaceVisibleEvent (pTempEvent, pCloneEvent));
		break;
	case 4: // �C�x���g�̃`�����l��
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_CHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		VERIFY (pEventListFrame->ReplaceVisibleEvent (pTempEvent, pCloneEvent));
		break;
	case 5: // �l1
	case 6: // �l2
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_VALUE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent));
		VERIFY (pEventListFrame->ReplaceVisibleEvent (pTempEvent, pCloneEvent));
		break;
	case 7: // �l3
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_VALUE));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		pLastEvent = MIDITrack_GetLastEvent (pTempTrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
				VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				VERIFY (pEventListFrame->ReplaceVisibleEvent (pLastEvent, m_pLastEvent));
			}
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pTempEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pTempEvent)); 
		VERIFY (pEventListFrame->ReplaceVisibleEvent (pTempEvent, pCloneEvent));
		break;
	}
	return TRUE;
}


// �l�̑������I������
BOOL CEventListIndexPropertyView::EndValueUpDown () {
	_RPTF0 (_CRT_WARN, "EndValueUpDown\n");
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pTempTrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// ���݂̃C�x���g�ƃg���b�N���擾
	VERIFY (pTempEvent = pEventListFrame->GetVisibleEvent (m_lCurRow));
	VERIFY (pTempTrack = MIDIEvent_GetParent (pTempEvent));
	// �����L�^
	// ���ڕʂ�
	switch (m_lCurColumn) {
	case 2: // ���߁F���F�e�B�b�N
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		if (m_pLastEvent) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
		}
		break;
	case 4: // �C�x���g�̃`�����l��
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		break;
	case 5: // �l1
	case 6: // �l2
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		break;
	case 7: // �l3
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent));
		if (m_pLastEvent) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
		}
		break;
	}
	return TRUE;
}


// �Z���̒l�𑝌�������
BOOL CEventListIndexPropertyView::AddValueOfCurCell (long lDeltaValue) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	// ���݂̃g���b�N�ւ̃|�C���^���擾
	MIDIEvent* pMIDIEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
		pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	}
	// �l�̔��f
	long lValue = 0;
	long lNumber = 0;
	switch (m_lCurColumn) {
	case 0:
		return FALSE;
	case 1:
		return FALSE;
	// ����:��:�e�B�b�N
	case 2:
		lValue = MIDIEvent_GetTime (pMIDIEvent) + lDeltaValue;
		lValue = CLIP (0, lValue, 0x7FFFFFFF);
		MIDIEvent_SetTime (pMIDIEvent, lValue);
		return TRUE;
	// ���
	case 3:
		return FALSE;
	// �`�����l��
	case 4:
		if (!MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			return FALSE;
		}
		lValue = MIDIEvent_GetChannel (pMIDIEvent) + lDeltaValue;
		lValue = CLIP (0, lValue, 15);
		MIDIEvent_SetChannel (pMIDIEvent, lValue);
		return TRUE;
	// �l1
	case 5:
		if (MIDIEvent_IsNoteOn (pMIDIEvent) ||
			MIDIEvent_IsNoteOff (pMIDIEvent) ||
			MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
			lValue = MIDIEvent_GetKey (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (0, lValue, 127);
			MIDIEvent_SetKey (pMIDIEvent, lValue);
			return TRUE;
		}
		else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
			lValue = MIDIEvent_GetNumber (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (0, lValue, 127);
			MIDIEvent_SetNumber (pMIDIEvent, lValue);
			return TRUE;
		}
		else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
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
			return TRUE;
		}
		else if (MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
			lValue = MIDIEvent_GetValue (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (0, lValue, 127);
			MIDIEvent_SetValue (pMIDIEvent, lValue);
			return TRUE;
		}
		else if (MIDIEvent_IsPitchBend (pMIDIEvent)) {
			lValue = MIDIEvent_GetValue (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (0, lValue, 16384);
			MIDIEvent_SetValue (pMIDIEvent, lValue);
			return TRUE;
		}
		return FALSE;
	// �l2
	case 6:
		if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
			lValue = MIDIEvent_GetVelocity (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (1, lValue, 127);
			MIDIEvent_SetVelocity (pMIDIEvent, lValue);
			return TRUE;
		}
		else if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
			long lKind = MIDIEvent_GetKind (pMIDIEvent);
			if ((lKind & 0xF0) == 0x80) {
				lValue = MIDIEvent_GetVelocity (pMIDIEvent) + lDeltaValue;
				lValue = CLIP (0, lValue, 127);
				MIDIEvent_SetVelocity (pMIDIEvent, lValue);
				return TRUE;
			}
			return FALSE;
		}
		else if (MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
			lValue = MIDIEvent_GetValue (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (0, lValue, 127);
			MIDIEvent_SetValue (pMIDIEvent, lValue);
			return TRUE;
		}
		else if (MIDIEvent_IsControlChange (pMIDIEvent)) {
			lNumber = MIDIEvent_GetNumber (pMIDIEvent);
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
			return TRUE;
		}
		return FALSE;
	// �l3
	case 7:
		if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
			lValue = MIDIEvent_GetDuration (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (1, lValue, 65535);
			MIDIEvent_SetDuration (pMIDIEvent, lValue);
			return TRUE;
		}
		else if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
			lValue = MIDIEvent_GetDuration (pMIDIEvent) + lDeltaValue;
			lValue = CLIP (-65535, lValue, -1);
			MIDIEvent_SetDuration (pMIDIEvent, lValue);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}



// �Z���̕�������擾����
CString CEventListIndexPropertyView::GetCellString (long lRow, long lColumn) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (lRow < 0 || lRow >= pEventListFrame->GetVisibleEventCount ()) {
		return _T("");
	}
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (lRow);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	MIDIClock* pMIDIClock = pSekaijuDoc->m_pMIDIClock;
	long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
	long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
	long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
	long lTime = MIDIEvent_GetTime (pMIDIEvent);
	long lKind = MIDIEvent_GetKind (pMIDIEvent);
	if (!MIDIEvent_IsMIDIEvent (pMIDIEvent) && lColumn > 5) {
		lColumn = 5;
	}
	CString strText;
	TCHAR szBuf1[2048];
	TCHAR szBuf2[2048];
	switch (lColumn) {
	// �g���b�N
	case 0:
		memset (szBuf1, 0, sizeof (szBuf1));
		memset (szBuf2, 0, sizeof (szBuf2));
		MIDITrack_GetName (pMIDITrack, szBuf1, TSIZEOF (szBuf1) - 1);
		codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
		strText.Format (_T("%d-%s"), 
			pSekaijuDoc->GetTrackIndex (pMIDITrack) + (bTrackZeroOrigin ? 0 : 1), szBuf2);
		break;
	// ��:��:�b:�~���b
	case 1:
		VERIFY (pSekaijuDoc->LongTimeToStringMillisec (pSekaijuDoc->m_pMIDIData, lTime, &strText));
		break;
	// ����:��:�e�B�b�N ���� �t���[���ԍ�:�e�B�b�N
	case 2:
		VERIFY (pSekaijuDoc->LongTimeToStringTime (pSekaijuDoc->m_pMIDIData, lTime, &strText));
		break;
	// �C�x���g�̎��
	case 3:
		strText = pSekaijuApp->m_strEventKindName[lKind];
		break;
	// �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂݁B���̑���n/a)�B
	case 4:
		if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			strText.Format (_T("%3d"), MIDIEvent_GetChannel (pMIDIEvent) + 1);
		}
		else {
			strText = _T("n/a");
		}
		break;
	// �l1
	case 5:
		// �V�[�P���X�ԍ�
		if (MIDIEvent_IsSequenceNumber (pMIDIEvent)) {
			strText.Format (_T("%d"), MIDIEvent_GetNumber (pMIDIEvent));
		}
		// �e�L�X�g�n�C�x���g
		else if (0x01 <= lKind && lKind <= 0x1F) {
			TCHAR szBuf1[2048];
			TCHAR szBuf2[2048];
			memset (szBuf1, 0, sizeof (szBuf1));
			memset (szBuf2, 0, sizeof (szBuf2));
			MIDIEvent_GetText (pMIDIEvent, szBuf1, TSIZEOF (szBuf1) - 1);
			codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, TSIZEOF (szBuf2) - 1);
			strText = szBuf2;
		}
		// �`�����l���v���t�B�b�N�X,�|�[�g�v���t�B�b�N�X
		else if (MIDIEvent_IsChannelPrefix (pMIDIEvent) || 
			MIDIEvent_IsPortPrefix (pMIDIEvent)) {
			strText.Format (_T("%d"), MIDIEvent_GetNumber (pMIDIEvent) + 1);
		}
		// �G���h�I�u�g���b�N
		else if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
			strText = _T("");
		}
		// �e���|�C�x���g
		else if (MIDIEvent_IsTempo (pMIDIEvent)) {
			long lTempo;
			double dBPM;
			lTempo = MIDIEvent_GetTempo (pMIDIEvent);
			dBPM = (double)60000000 / (double)lTempo;
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_1P2LF_BPM_EQ_D_MICROSEC_PER_QUARTER_NOTE));
			strText.Format (strFormat, dBPM, lTempo);
		}
		// SMPTE�I�t�Z�b�g
		else if (MIDIEvent_IsSMPTEOffset (pMIDIEvent)) {
			CString strMode[5];
			VERIFY (strMode[0].LoadString (IDS_24));
			VERIFY (strMode[1].LoadString (IDS_25));
			VERIFY (strMode[2].LoadString (IDS_29P97));
			VERIFY (strMode[3].LoadString (IDS_30));
			VERIFY (strMode[4].LoadString (IDS_ERROR));
			long lMode, lHour, lMinute, lSec, lFrame, lSubFrame;
			MIDIEvent_GetSMPTEOffset (pMIDIEvent, &lMode, &lHour, &lMinute, &lSec, &lFrame, &lSubFrame);
			if (lMode < 0 || lMode > 4) {
				lMode = 4;
			}
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_FPS_D_HOUR_D_MINUTE_D_SEC_D_FRAME_D_SUBFRAME));
			strText.Format (strFormat, 
				strMode[lMode], lHour, lMinute, lSec, lFrame, lSubFrame);
		}
		// ���q�L��
		else if (lKind == MIDIEVENT_TIMESIGNATURE) {
			long lnn, ldd, lcc, lbb;
			MIDIEvent_GetTimeSignature (pMIDIEvent, &lnn, &ldd, &lcc, &lbb);
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_PER_D_D_CLOCK_PER_BEAT_D_32DIVNOTE_PER_BEAT));
			strText.Format (strFormat, lnn, 1 << ldd, lcc, lbb);
		}
		// �����L��
		else if (lKind == MIDIEVENT_KEYSIGNATURE) {
			long lsf, lmi;
			MIDIEvent_GetKeySignature (pMIDIEvent, &lsf, &lmi);
			CString strSF; // "#"����"b"
			CString strMi; // "major"����"minor"
			CString strKeySignatureName;
			// ����
			if (lmi == 0) {
				if (0 <= lsf && lsf < 8) {
					VERIFY (strSF.LoadString (IDS_SHARP));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MA + lsf ));
				}
				else if (-8 <= lsf && lsf < 0) {
					VERIFY (strSF.LoadString (IDS_FLAT));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MA + 16 + lsf));
				}
				VERIFY (strMi.LoadString (IDS_MAJOR));
			}
			// �Z��
			else {
				if (0 <= lsf && lsf < 8) {
					VERIFY (strSF.LoadString (IDS_SHARP));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MI + lsf ));
				}
				else if (-8 <= lsf && lsf < 0) {
					VERIFY (strSF.LoadString (IDS_FLAT));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MI + 16 + lsf));
				}
				VERIFY (strMi.LoadString (IDS_MINOR));
			}
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_S_S_EQ_S));			
			strText.Format (strFormat, abs (lsf), strSF, strMi, strKeySignatureName);
		}
		// �m�[�g�I�t�E�m�[�g�I���E�L�[�A�t�^�[�^�b�`
		else if (0x80 <= lKind && lKind <= 0xAF) {
			long lKey = MIDIEvent_GetKey (pMIDIEvent);
			CString strKeyName;
			strKeyName = pSekaijuDoc->GetKeyName (pMIDITrack, lTime, lKey);
			strText.Format (_T("%d-%s"), lKey, strKeyName);
		}
		// �R���g���[���`�F���W
		else if (0xB0 <= lKind && lKind <= 0xBF) {
			long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
			long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
			if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
				lChannel = lTrackOutputChannel;	
			}
			MIDIInstrumentDefinition* pMIDIInstDef = NULL;
			// ���̃g���b�N�̕\�����[�h�́u�ʏ�v�̏ꍇ
			if (lTrackViewMode == 0) {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			}
			// ���̃g���b�N�̕\�����[�h�́u�h�����v�̏ꍇ
			else {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
			}
			// ���̃g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`����������
			if (pMIDIInstDef) {
				MIDIControllerNameTable* pControllerNameTable = 
				MIDIInstrumentDefinition_GetControllerNameTable (pMIDIInstDef);
				// ���̃C���X�g�D�������g��`��ControllerNameTable�������Ă���
				if (pControllerNameTable) {
					TCHAR szBuf[256];
					memset (szBuf, 0, sizeof (szBuf));
					MIDIControllerNameTable_GetName (pControllerNameTable, lNumber, szBuf, 255);
					strText.Format (_T("%d-%s"), lNumber, szBuf);
				}
				// ���̃C���X�g�D�������g��`��ControllerNameTable�������Ă��Ȃ�
				else {
					strText.Format (_T("%d"), lNumber);
				}
			}
			// ���g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`���Ȃ�����
			else {
				strText.Format (_T("%d"), lNumber);
			}
		}
		// �v���O�����`�F���W
		else if (0xC0 <= lKind && lKind <= 0xCF) {
			long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
			long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
			if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
				lChannel = lTrackOutputChannel;	
			}
			MIDIInstrumentDefinition* pMIDIInstDef = NULL;
			// ���̃g���b�N�̕\�����[�h�́u�ʏ�v�̏ꍇ
			if (lTrackViewMode == 0) {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			}
			// ���̃g���b�N�̕\�����[�h�́u�h�����v�̏ꍇ
			else {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
			}
			// ���̃g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`����������
			if (pMIDIInstDef) {
				long lBankMSB = MIDIEvent_GetBankMSB (pMIDIEvent);
				long lBankLSB = MIDIEvent_GetBankLSB (pMIDIEvent);
				long lBank = (lBankMSB << 7) | lBankLSB;
				MIDIPatchNameTable* pPatchNameTable = 
				MIDIInstrumentDefinition_GetPatchNameTable (pMIDIInstDef, lBank);
				// ���̃C���X�g�D�������g��`�͎w��o���N��PatchNameTable�������Ă���
				if (pPatchNameTable) {
					TCHAR szBuf[256];
					memset (szBuf, 0, sizeof (szBuf));
					MIDIPatchNameTable_GetName (pPatchNameTable, lNumber, szBuf, 255);
					strText.Format (_T("%d-%s"), lNumber, szBuf);
				}
				// ���̃C���X�g�D�������g��`�͎w��o���N��PatchNameTable�������Ă��Ȃ�
				else {
					strText.Format (_T("%d"), lNumber);
				}
			}
			// ���g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`���Ȃ�����
			else {
				strText.Format (_T("%d"), lNumber);
			}
		}
		// �`�����l���A�t�^�[�^�b�`
		else if (0xD0 <= lKind && lKind <= 0xDF) {
			long lValue = MIDIEvent_GetValue (pMIDIEvent);
			strText.Format (_T("%d"), lValue);
		}
		// �s�b�`�x���h
		else if (0xE0 <= lKind && lKind <= 0xEF) {
			long lValue = MIDIEvent_GetValue (pMIDIEvent);
			strText.Format (_T("%d"), lValue - 8192);
		}
		// ���̑��̃C�x���g(16�i�_���v)
		else {
			BYTE byBin[1024];
			TCHAR szText[1024];
			memset (byBin, 0, sizeof (byBin));
			memset (szText, 0, sizeof (szText));
			MIDIEvent_GetData (pMIDIEvent, byBin, 1024);
			bin2txt (byBin, MIDIEvent_GetLen (pMIDIEvent), szText, 1023);
			strText = szText;
		}
		break;
	// �l2
	case 6:
		// �m�[�g
		if (0x80 <= lKind && lKind <= 0x9F) {
			strText.Format (_T("%d"), MIDIEvent_GetVelocity (pMIDIEvent));
		}
		// �L�[�A�t�^�[�^�b�`�E�R���g���[���`�F���W
		else if (0xA0 <= lKind && lKind <= 0xBF) {
			strText.Format (_T("%d"), MIDIEvent_GetValue (pMIDIEvent));
		}
		// ���̑�
		else {
			strText.Format (_T("---"));
		}
		break;
	// �l3
	case 7:
		// �m�[�g
		if (0x80 <= lKind && lKind <= 0x9F) {
			strText.Format (_T("%d"), MIDIEvent_GetDuration (pMIDIEvent));
		}
		// ���̑�
		else {
			strText.Format (_T("---"));
		}
		break;
	}
	return strText;
}

// �Z���ɕ������ݒ�(�s���ȕ�����̏ꍇ��FALSE��Ԃ��ݒ肵�Ȃ�)
BOOL CEventListIndexPropertyView::SetCellString (long lRow, long lColumn, CString strText) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (lRow < 0 || lRow >= pEventListFrame->GetVisibleEventCount ()) {
		return FALSE;
	}
	m_bSettingCellString = 1;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (lRow);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	MIDITrack* pNewMIDITrack = NULL;
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	MIDIClock* pMIDIClock = pSekaijuDoc->m_pMIDIClock;
	long lTime = MIDIEvent_GetTime (pMIDIEvent);
	long lKind = MIDIEvent_GetKind (pMIDIEvent);
	if (!MIDIEvent_IsMIDIEvent (pMIDIEvent) && lColumn > 5) {
		lColumn = 5;
	}
	//CString strMsg;
	CString strToken;
	CString strHistoryName;
	long lNumTrack = MIDIData_CountTrack (pMIDIData);
	long lValue = 0;
	long lTemp = 0;
	long i = 0;
	MIDIEvent* pCloneEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	m_pLastEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	switch (lColumn) {
	// �g���b�N
	case 0:
		// �g���b�N�����`�F�b�N
		if (IsNumeric (strText) <= 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_TRACK_MUST_BE_HALF_WIDTH_NUMBER));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �g���b�N�͈̓`�F�b�N
		lValue = _ttol (strText) - (bTrackZeroOrigin ? 0 : 1);
		if (lValue < 0 || lValue >= lNumTrack) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_TRACK_VALUE_IS_OUT_OF_RANGE));
			//strMsg.Format ("�g���b�N�ԍ���%d�ȏ�%d�ȉ��̔��p��������͂��Ă��������B", 
			//	(bTrackZeroOrigin ? 0 : 1), lNumTrack + (bTrackZeroOrigin ? 0 : 1) - 1);
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �t�H�[�}�b�g1�̏ꍇ�A
		// �e���|�ESMPTE�I�t�Z�b�g�E���q�L���E�����L���̓g���b�N��2�Ԗڈȍ~�̃g���b�N�ɒu���Ȃ��B
		if (MIDIData_GetFormat (pMIDIData) == 1 && lValue != 0 &&
			(MIDIEvent_IsTempo (pMIDIEvent) || MIDIEvent_IsSMPTEOffset (pMIDIEvent) ||
			MIDIEvent_IsTimeSignature (pMIDIEvent) || MIDIEvent_IsKeySignature (pMIDIEvent))) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_THIS_EVENT_TO_THE_SECOND_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �t�H�[�}�b�g1�̏ꍇ�A
		// MIDI�`�����l���C�x���g�̓g���b�N�͍ŏ��̃g���b�N�ɒu���Ȃ��B
		if (MIDIData_GetFormat (pMIDIData) == 1 && lValue == 0 &&
			MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_THIS_EVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		if (MIDIEvent_IsEndofTrack (pMIDIEvent) && MIDIEvent_GetNextEvent (pMIDIEvent) == NULL) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MOVE_THE_LAST_ENDOFTRACK_TO_THE_OTHER_TRACK));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		pNewMIDITrack = pSekaijuDoc->GetTrack (lValue);
		if (pNewMIDITrack == NULL) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_MODIFY_EVENT_TRACK_FAILED));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			m_bSettingCellString = 0;
			return FALSE;
		}
		if (pEventListFrame->IsTrackVisible (lValue) == FALSE) {
			pEventListFrame->SetTrackVisible (lValue);
		}
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_TRACK));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
		VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));

		pLastEvent = MIDITrack_GetLastEvent (pNewMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
				VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
			}
		}
		VERIFY (MIDITrack_InsertEvent (pNewMIDITrack, pMIDIEvent));
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
		if (m_pLastEvent) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
		}
		return TRUE;
	// ��:��:�b:�~���b
	case 1:
		m_bSettingCellString = 0;
		return FALSE;
	// ����:��:�e�B�b�N
	case 2:
		lValue = 0;
		lTemp = pSekaijuDoc->StringTimeToLongTime (pSekaijuDoc->m_pMIDIData, strText, &lValue);
		if (lTemp > 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (lTemp));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_TIME));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (pLastEvent != pMIDIEvent) { // 20090712�ǉ�
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
		}
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
		VERIFY (MIDIEvent_SetTime (pCloneEvent, lValue));
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		if (m_pLastEvent) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
		}
		return TRUE;
	// �C�x���g�̎��
	case 3:
		// �g���b�N�Ō�̃G���h�I�u�g���b�N�C�x���g���ύX�����̂�j�~
		if (MIDIEvent_IsEndofTrack (pMIDIEvent) && 
			pMIDITrack->m_pLastEvent == pMIDIEvent) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MODIFY_THE_LAST_ENDOFTRADK));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// ��v���閼�O�̃C�x���g�̎�ނ�T��
		for (i = 0; i < 255; i++) {
			if (strText == pSekaijuApp->m_strEventKindName[i]) {
				break;
			}
		}
		// �C�x���g�̎��i�Ɉ�v����
		if (0 <= i && i < 255) {
			MIDITrack* pFirstTrack = MIDIData_GetFirstTrack (pMIDIData);
			// MIDI�`�����l���C�x���g�̏ꍇ�̓`�����l����񖳎�
			if (0x80 <= i && i <= 0xEF) {
				i &= 0xF0;
			}
			// �m�[�g�I�t�̏ꍇ�͋����I�Ƀm�[�g�I��+�m�[�g�I�t�Ƃ���B
			if (0x80 <= i && i <= 0x8F) {
				i = MIDIEVENT_NOTEONNOTEOFF;
			}
			// �m�[�g�I���̏ꍇ�͋����I�Ƀm�[�g�I��+�m�[�g�I��0�Ƃ���B
			else if (0x90 <= i && i <= 0x9F) {
				i = MIDIEVENT_NOTEONNOTEON0;
			}
			// SMF�t�H�[�}�b�g1�Ńg���b�N0�ȊO��
			// �e���|�ASMPTE�I�t�Z�b�g�A���q�A����������̂�h�~
			if (MIDIData_GetFormat (pMIDIData) == 1 && (pMIDITrack != pFirstTrack) &&
				(i == 0x51 || i == 0x54 || i == 0x58 || i == 0x59)) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_ARE_YOU_SURE_TO_MOVE_THIS_EVENT_TO_THE_FIRST_TRACK));
				int nRet = AfxMessageBox (strMsg, MB_ICONINFORMATION | MB_YESNOCANCEL);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				// �����I�ɍŏ��̃g���b�N�Ɉړ����Ȃ�
				if (nRet != IDYES) {
					pSekaijuDoc->m_theCriticalSection.Unlock ();
					CString strMsg;
					VERIFY (strMsg.LoadString (IDS_FAILED_TO_MODIFY_EVENT_KIND));
					AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
					pSekaijuDoc->m_theCriticalSection.Lock ();
					m_bSettingCellString = 0;
					return FALSE;
				}
				// �����I�ɍŏ��̃g���b�N�Ɉړ�����
				else {
					long lListIndex = pEventListFrame->EventKindtoListIndex (i);
					if (pEventListFrame->IsEventKindVisible (lListIndex) == FALSE) {
						pEventListFrame->SetEventKindVisible (lListIndex);
					}
					VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_KIND));
					VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
					VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
					pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
					if (pLastEvent) {
						if (MIDIEvent_IsEndofTrack (pLastEvent)) {
							VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
							VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
							VERIFY (pEventListFrame->ReplaceVisibleEvent (pLastEvent, m_pLastEvent));
						}
					}
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
					VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
					VERIFY (MIDITrack_RemoveEvent (pMIDITrack, pCloneEvent));
					VERIFY (MIDIEvent_SetKind (pCloneEvent, i));
					VERIFY (MIDITrack_InsertEvent (pFirstTrack, pCloneEvent));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					if (m_pLastEvent) {
						VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
					}
				}
			}
			// SMF�t�H�[�}�b�g1�Ńg���b�N0��MIDI�`�����l���C�x���g������̂�h�~
			else if (MIDIData_GetFormat (pMIDIData) == 1 && (pMIDITrack == pFirstTrack) && 
				(0x80 <= i && i <= 0xEF || i == MIDIEVENT_NOTEONNOTEOFF || 
				i == MIDIEVENT_NOTEONNOTEON0)) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_UNABLE_TO_INSERT_THIS_EVENT_TO_THE_FIRST_TRACK_IN_FORMAT1_MIDIDATA));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// �ٕ��C�x���g�łȂ��ꍇ���B
			else {
				VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_KIND));
				VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
				VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
				MIDIEvent_SetKind (pCloneEvent, i);
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			}
			m_bSettingCellString = 0;
			return TRUE;
		}
		// ��v����C�x���g�̎�ނ͂Ȃ�����
		else {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNEXPECTED_EVENT_KIND));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		break;
	// �`�����l��
	case 4:
		// MIDI�`�����l���C�x���g�ȊO�̏ꍇ�̓`�����l����ݒ�ł��Ȃ�
		if (!MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_SPECIFIY_EVENT_CHANNEL_OF_THIS_EVENT));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �`�����l�������`�F�b�N
		strText.TrimLeft ();
		if (IsNumeric (strText) <= 0) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_CHANNEL_MUST_BE_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �`�����l���l�`�F�b�N
		lValue = _ttol (strText);
		if (lValue <= 0 || lValue > 16) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_CHANNEL_MUST_BE_HALF_WIDTH_NUMBER_FROM_1_TO_16));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �`�����l���ύX
		VERIFY (strHistoryName.LoadString (IDS_MODIFY_EVENT_CHANNEL));
		VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
		VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
		VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
		VERIFY (MIDIEvent_SetChannel (pCloneEvent, lValue - 1));
		VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
		m_bSettingCellString = 0;
		return TRUE;
	// �l1
	case 5:
		// �V�[�P���X�ԍ��C�x���g
		if (MIDIEvent_IsSequenceNumber (pMIDIEvent)) {
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_SEQUENCENUMBER_MUST_BE_HALF_WIDTH_NUMBER_FROM_0_TO_65535));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 65535);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_SEQUENCENUMBER_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetNumber (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �e�L�X�g�n�C�x���g
		else if (0x01 <= lKind && lKind <= 0x1F) {
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_TEXTEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			TCHAR szBuf2[2048];
			memset (szBuf2, 0, sizeof (szBuf2));
			str2codestr ((LPTSTR)(LPCTSTR)strText, strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
			VERIFY (MIDIEvent_SetText (pCloneEvent, szBuf2) >= 0);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �`�����l���v���t�B�b�N�X
		else if (MIDIEvent_IsChannelPrefix (pMIDIEvent)) {
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_CHANNELPREFIX_MUST_BE_HALF_WIDTH_NUMBER_FROM_1_TO_16));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (1, lValue, 16);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_CHANNELPREFIX_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetNumber (pCloneEvent, lValue - 1));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �|�[�g�v���t�B�b�N�X
		else if (MIDIEvent_IsPortPrefix (pMIDIEvent)) {
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_PORTPREFIX_MUST_BE_HALF_WIDTH_NUMBER_FROM_0_TO_255));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 255);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_PORTPREFIX_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetNumber (pCloneEvent, lValue - 1));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �G���h�I�u�g���b�N
		else if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_ENDOFTRACK_HAS_NO_VALUE));
			AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		// �e���|�C�x���g
		else if (MIDIEvent_IsTempo (pMIDIEvent)) {
			long lCurTime, lMillisec;
			double dTemp;
#ifdef UNICODE
			dTemp = _wtol (strText); // TODO:VC++4.0��_ttof���g���Ȃ��̂ŉ�
#else
			dTemp = atof (strText);
#endif
			dTemp = CLIP (1, dTemp, 60000);
			lValue = (long)((60000000 / dTemp));
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_TEMPO_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetTempo (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			// TODO:MIDIClock�������j���O���͈ꎞ��~
			if (pSekaijuApp->m_bPlaying) {
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
			}
			lCurTime = MIDIClock_GetTickCount (pMIDIClock);
			lMillisec = MIDIData_TimeToMillisec (pMIDIData, lCurTime);
			MIDIClock_SetMillisec (pMIDIClock, lMillisec);
			MIDIData_FindTempo (pMIDIData, lTime, &lValue);
			MIDIClock_SetTempo (pMIDIClock, lValue);
			// TODO:MIDIClock�̃����j���O��Ԃ𕜋A
			if (pSekaijuApp->m_bPlaying) {
				MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
			}
			m_bSettingCellString = 0;
			return TRUE;
		}
		// SMPTE�I�t�Z�b�g
		else if (MIDIEvent_IsSMPTEOffset (pMIDIEvent)) {
			long lMaxFrame[4] = {23, 24, 29, 29};
			long lMode, lHour, lMinute, lSec, lFrame, lSubFrame;
			// FPS�ƃt���[���̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// FPS
			strToken = strText.Left (lTemp);
			if (strToken.Left (2) == _T("24")) {
				lMode = 0;
			}
			else if (strToken.Left (2) == _T("25")) {
				lMode = 1;
			}
			else if (strToken.Left (5) == _T("29.97")) {
				lMode = 2;
			}
			else if (strToken.Left (2) == _T("30")) {
				lMode = 3;
			}
			else {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_FPS_MUST_BE_24_OR_25_OR_29P97_OR_30));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// �t���[���Ǝ��̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// ��
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_HOUR_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lHour = _ttol (strText);
			lHour = CLIP (0, lHour, 23);
			// ���ƕ��̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// ��
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_MINUTE_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lMinute = _ttol (strText);
			lMinute = CLIP (0, lMinute, 59);
			// ���ƕb�̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// �b
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_SECOND_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lSec = _ttol (strText);
			lSec = CLIP (0, lSec, 59);
			// �b�ƃt���[���̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// �t���[��
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_FRAME_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lFrame = _ttol (strText);
			lFrame = CLIP (0, lFrame, lMaxFrame[lMode]);
			// �t���[���ƃT�u�t���[���̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// �T�u�t���[��
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			lSubFrame = _ttol (strText);
			lSubFrame = CLIP (0, lSubFrame, 99);
			// SMPTE�I�t�Z�b�g�̐ݒ�
			CString strHistoryName;
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_SMPTEOFFSET_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetSMPTEOffset 
				(pCloneEvent, lMode, lHour, lMinute, lSec, lFrame, lSubFrame));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// ���q�L��
		else if (lKind == MIDIEVENT_TIMESIGNATURE) {
			long lnn, ldd, lcc, lbb;
			// ���q�ƕ���̊Ԃ̃Z�p���[�^
			lTemp = strText.Find (_T("/"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_BETWEEN_NUMERATOR_AND_DENOMINATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// ���q
			strToken = strText.Left (lTemp);
			if (IsNumeric (strToken) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_NUMERATOR_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lnn = _ttol (strToken);
			lnn = CLIP (1, lnn, 127);
			// ����Ƃ̊Ԃ̃Z�p���[�^
			strText = strText.Mid (lTemp + 1);
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// ����
			strToken = strText.Left (lTemp);
			if (IsNumeric (strToken) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_DENOMINATOR_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			ldd = _ttol (strToken);
			//if (ldd != 1 && ldd != 2 && ldd != 4 && ldd != 8 && ldd != 16 && ldd != 32) {
			//	return FALSE;
			//}
			switch (ldd) {
			case 1:
				ldd = 0;
				break;
			case 2:
				ldd = 1;
				break;
			case 4:
				ldd = 2;
				break;
			case 8:
				ldd = 3;
				break;
			case 16:
				ldd = 4;
				break;
			case 32:
				ldd = 5;
				break;
			default:
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_DENOMINATOR_MUST_BE_1_OR_2_OR_4_OR_8_OR_16_OR_32));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// [�N���b�N/��]��[32������/��]�̊Ԃ̃Z�p���[�^
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_A_SEPARATOR_IS_NOT_FOUND));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			// 1��������̃N���b�N��[�N���b�N/��]
			strToken = strText.Left (lTemp);
			if (IsNumeric (strToken) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_CLOCKSPERBEAT_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lcc = _ttol (strToken);
			lcc = CLIP (1, lcc, 127);
			// 1���������32�������̐�[32������/��]
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			if (IsNumeric (strText) <= 0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_32DIVNOTEPERBEAT_MUST_BE_HALF_WIDTH_NUMBER));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lbb = _ttol (strText);
			lbb = CLIP (1, lbb, 127);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_TIMESIGNATURE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetTimeSignature (pCloneEvent, lnn, ldd, lcc, lbb));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �����L��
		else if (lKind == MIDIEVENT_KEYSIGNATURE) {
			long lsf, lmi;
			lTemp = strText.Find (_T(":"));
			if (lTemp < 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			// #����b�̐�
			strToken = strText.Left (lTemp);
			strToken.TrimLeft ();
			strToken.TrimRight ();
			TCHAR* szTemp[15] = {_T("7b"), _T("6b"), _T("5b"), _T("4b"), _T("3b"), _T("2b"), _T("1b"), 
				_T("0"), _T("1#"), _T("2#"), _T("3#"), _T("4#"), _T("5#"), _T("6#"), _T("7#")};
			for (i = 0; i < 15; i++) {
				if (_tcscmp ((LPCTSTR)strToken, szTemp[i]) == 0) {
					break;
				}
			}
			if (_tcscmp ((LPCTSTR)strToken, _T("0b")) == 0 || 
				_tcscmp ((LPCTSTR)strToken, _T("0#")) == 0) {
				i = 7;
			}
			if (i >= 15) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_KEYSIGNATURE_MUST_BE_7B_6B_5B_4B_3B_2B_1B_0_1S_2S_3S_4S_5S_6S_7S));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			lsf = (long)(char)(i - 7);
			// "major"����"minor"
			strText = strText.Mid (lTemp + 1);
			strText.TrimLeft ();
			CString strMajor;
			CString strMinor;
			VERIFY (strMajor.LoadString (IDS_MAJOR));
			VERIFY (strMinor.LoadString (IDS_MINOR));
			if (_tcsnccmp ((LPCTSTR)strText, (LPCTSTR)strMajor, strMajor.GetLength ()) == 0) {
				lmi = 0;
			}
			else if (_tcsnccmp ((LPCTSTR)strText, (LPCTSTR)strMinor, strMinor.GetLength ()) == 0) {
				lmi = 1;
			}
			else {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_KEYSCALE_MUST_BE_MAJOR_OR_MINOR));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_KEYSIGNATURE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetKeySignature (pCloneEvent, lsf, lmi));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			return TRUE;
		}
		// �V�[�P���T�ŗL�̃C�x���g
		else if (MIDIEvent_IsSequencerSpecific (pMIDIEvent)) {
			BYTE byData[1024];
			memset (byData, 0, sizeof (byData));
			long lRet = txt2bin ((TCHAR*)(LPCTSTR)strText, 1023, byData, 1024);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_SEQUENCERSPECIFIC));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetData (pCloneEvent, byData, lRet) >= 0);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �m�[�g�E�L�[�A�t�^�[�^�b�`
		else if (0x80 <= lKind && lKind <= 0xAF) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 127);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetKey (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �R���g���[���`�F���W�E�v���O�����`�F���W
		else if (0xB0 <= lKind && lKind <= 0xCF) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 127);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			(MIDIEvent_SetNumber (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �`�����l���A�t�^�[�^�b�`
		else if (0xD0 <= lKind && lKind <= 0xDF) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 127);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �s�b�`�x���h
		else if (0xE0 <= lKind && lKind <= 0xEF) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText) + 8192;
			lValue = CLIP (0, lValue, 16383);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �V�X�e���G�N�X�N���[�V�u(�ʏ�)
		else if (lKind == 0xF0) {
			BYTE byData[1024];
			memset (byData, 0, sizeof (byData));
			long lRet = txt2bin ((TCHAR*)(LPCTSTR)strText, 1023, byData, 1024);
			if (byData[0] != 0xF0) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_SYSEX_DATA_MUST_BEGIN_WITH_F0));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			for (long j = 1; j < lRet - 1; j++) { // 20080809����
				if (byData[j] >= 0x80) {
					pSekaijuDoc->m_theCriticalSection.Unlock ();
					CString strMsg;
					VERIFY (strMsg.LoadString (IDS_SYSEX_DATA_BETWEE_F0_AND_F7_MUST_BE_FROM_00_TO_7F));
					AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
					pSekaijuDoc->m_theCriticalSection.Lock ();
					m_bSettingCellString = 0;
					return FALSE;
				}
			}
			if (byData[lRet - 1] != 0xF7) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				CString strMsg;
				VERIFY (strMsg.LoadString (IDS_SYSEX_DATA_MUST_END_WITH_F7));
				AfxMessageBox (strMsg, MB_ICONEXCLAMATION | MB_OK);
				pSekaijuDoc->m_theCriticalSection.Lock ();
				m_bSettingCellString = 0;
				return FALSE;
			}
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_SYSEX_NORMAL));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetData (pCloneEvent, byData, lRet) >= 0);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �V�X�e���G�N�X�N���[�V��(�C��)
		else if (lKind == 0xF7) {
			BYTE byData[1024];
			memset (byData, 0, sizeof (byData));
			long lRet = txt2bin ((TCHAR*)(LPCTSTR)strText, 1023, byData, 1024);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_SYSEX_ARBITRARY));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetData (pCloneEvent, byData, lRet) >= 0);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// ���̑��̃C�x���g(16�i�_���v)
		else {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			CString strMsg;
			VERIFY (strMsg.LoadString (IDS_UNABLE_TO_MODIFY_UNDEFINED_EVENT));
			AfxMessageBox (strMsg);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			m_bSettingCellString = 0;
			return FALSE;
		}
		break;
	// �l2
	case 6:
		// �m�[�g�I�t�E�m�[�g�I��
		if (0x80 <= lKind && lKind <= 0x9F) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 127);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetVelocity (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �L�[�A�t�^�[�^�b�`�E�R���g���[���`�F���W
		else if (0xA0 <= lKind && lKind <= 0xBF) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (0, lValue, 127);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			m_bSettingCellString = 0;
			return TRUE;
		}
		// ���̑�
		else {
			m_bSettingCellString = 0;
			return FALSE;
		}
		break;
	// �l3
	case 7:
		// �m�[�g�I�t
		if (0x80 <= lKind && lKind <= 0x8F) {
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			lValue = CLIP (-65535, lValue, 0);
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetDuration (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			if (m_pLastEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
			}
			m_bSettingCellString = 0;
			return TRUE;
		}
		// �m�[�g�I��
		else if (0x90 <= lKind && lKind <= 0x9F) {
			long lVelocity = MIDIEvent_GetVelocity (pMIDIEvent);
			if (IsNumeric (strText) <= 0) {
				m_bSettingCellString = 0;
				return FALSE;
			}
			lValue = _ttol (strText);
			// �m�[�g�I�t
			if (lVelocity == 0) {
				lValue = CLIP (-65535, lValue, 0);
			}
			// �m�[�g�I��
			else {
				lValue = CLIP (0, lValue, 65535);
			}
			VERIFY (strHistoryName.LoadString (IDS_MODIFY_MIDIEVENT_VALUE));
			VERIFY (pSekaijuDoc->AddHistoryUnit (strHistoryName));
			VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
			pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
			if (pLastEvent) {
				if (MIDIEvent_IsEndofTrack (pLastEvent)) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
					VERIFY (m_pLastEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
				}
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pMIDIEvent));
			VERIFY (MIDIEvent_SetDuration (pCloneEvent, lValue));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			if (m_pLastEvent) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, m_pLastEvent));
			}
			m_bSettingCellString = 0;
			return TRUE;
		}
		// ���̑�
		else {
			m_bSettingCellString = 0;
			return FALSE;
		}
		break;
	}
	m_bSettingCellString = 0;
	return FALSE;
}

int CEventListIndexPropertyView::DrawCurFrame (CDC* pDC) {
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
	return 1;
}






// �Z���̒����`���擾
CRect CEventListIndexPropertyView::GetRectFromCell (long lRow, long lColumn) {
	ASSERT (0 <= lColumn && lColumn < 8);
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	BOOL bCombined = FALSE; // ��5�`7���������Ă���ꍇTRUE;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	// ��5�`7�̃Z���̌�������
	if (0 <= lRow && lRow < lVisibleEventCount) {
		if (lColumn >= 5) {
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (lRow);
			if (!MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
				bCombined = TRUE;
			}
		}
	}
	else {
		bCombined = TRUE;
	}
	// �Z����`�̌v�Z
	CRect rcCell (0, 0, 0 ,0);
	if (bCombined && lColumn >= 5) {
		rcCell.left = pEventListFrame->GetColumnLeft (5);
		rcCell.right = rcCell.left + pEventListFrame->GetColumnWidth (5) + 
			pEventListFrame->GetColumnWidth (6) + pEventListFrame->GetColumnWidth (7);
	}
	else {
		rcCell.left = pEventListFrame->GetColumnLeft (lColumn);
		rcCell.right = rcCell.left + pEventListFrame->GetColumnWidth (lColumn);
	}
	long lRowZoom = pEventListFrame->GetRowZoom ();
	rcCell.top = lRow * lRowZoom;
	rcCell.bottom = (lRow + 1) * lRowZoom;
	ASSERT (rcCell.left < rcCell.right);
	ASSERT (rcCell.top < rcCell.bottom);
	return rcCell;
}

// �e�L�X�g�{�b�N�X�̈ړ�(VISIBLE==FALSE���܂�)
BOOL CEventListIndexPropertyView::MoveTextBox (long lRow, long lColumn) {
	ASSERT (0 <= lRow && lRow <= 0x7FFFFFFF);
	ASSERT (0 <= lColumn && lColumn < 8);
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	long lButtonWidth = 12; //::GetSystemMetrics (SM_CXVSCROLL);
	CRect rcNewCell = GetRectFromCell (lRow, lColumn);
	rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	m_theTextBox.MoveWindow 
		(rcNewCell.left, rcNewCell.top + pEventListFrame->GetRowZoom () / 2 - 6, 
		rcNewCell.Width () - (lColumn >= 0 ? lButtonWidth : 1), 13);
	return TRUE;
}

// ���X�g�{�b�N�X�̈ړ�(VISIBLE==FALSE���܂�)
BOOL CEventListIndexPropertyView::MoveListBox (long lRow, long lColumn) {
	ASSERT (0 <= lRow && lRow <= 0x7FFFFFFF);
	ASSERT (0 <= lColumn && lColumn < 8);
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	m_theListBox.MoveWindow 
		(rcNewCell.left, rcNewCell.bottom, 
		rcNewCell.Width (), rcNewCell.Height () + 64);
	return TRUE;
}

// �c�[���o�[�̃R���g���[���X�V
BOOL CEventListIndexPropertyView::UpdateParentToolbarControl () {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)GetDocument ();
	CString strText;
	long lIndex;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	if (m_lCurRow < 0 || m_lCurRow >= lVisibleEventCount) {
		return FALSE;
	}
	MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	if (pMIDIEvent == NULL) {
		return FALSE;
	}
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
	//switch (m_lCurColumn) {
	// �g���b�N�R���{�X�V
	//case 0:
		lIndex = pSekaijuDoc->GetTrackIndex (pMIDITrack);
		pEventListFrame->m_wndEventTrackCombo.SetCurSel (lIndex);
		pEventListFrame->m_wndTrackList.SetCurSel (lIndex);
	//	break;
	// �C�x���g�̎����G�f�B�b�g�X�V
	//case 1:
	//case 2:
		strText = GetCellString (m_lCurRow, 2);
		pEventListFrame->m_wndEventTimeEdit.SetWindowText (strText);
	//	break;
	// �C�x���g�̎�ރR���{�X�V
	//case 3:
		if (0 <= lEventKind && lEventKind < 256) {
			lIndex = pEventListFrame->EventKindtoListIndex (lEventKind);
			pEventListFrame->m_wndEventKindCombo.SetCurSel (lIndex);
			pEventListFrame->m_wndEventKindList.SetCurSel (lIndex);
		}
	//	break;
	// �C�x���g�̃`�����l���R���{�X�V
	//case 4:
		strText = GetCellString (m_lCurRow, 4);
		if (strText == _T("n/a")) {
			pEventListFrame->m_wndEventChannelCombo.SetCurSel (0);
		}
		else {
			lIndex = _ttol (strText);
			if (1 <= lIndex && lIndex <= 16) {
				pEventListFrame->m_wndEventChannelCombo.SetCurSel (lIndex);
			}
		}
	//	break;
	//}
	return TRUE;
}

// �|�b�v�A�b�v���j���[�̕\��
BOOL CEventListIndexPropertyView::ShowPopupMenu (CPoint ptMenu) {

	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (m_lCurButtonState == 0x0000);
	
	MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	if (pMIDIEvent) {
		pSekaijuDoc->m_lTempTime = MIDIEvent_GetTime (pMIDIEvent);
		pSekaijuDoc->m_pTempTrack = MIDIEvent_GetParent (pMIDIEvent);
		pSekaijuDoc->m_pTempEvent = pMIDIEvent;
	}
	else {
		pSekaijuDoc->m_lTempTime = pSekaijuDoc->m_lNewTime;
		//long lCurTrackIndex = pEventListFrame->GetCurTrackIndex ();
		pSekaijuDoc->m_pTempTrack = NULL; // 20100429�C��
		pSekaijuDoc->m_pTempEvent = NULL;
	}
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

	CMenu theMenu;
	VERIFY (theMenu.LoadMenu (IDR_POPUPMENU21));
	CMenu* pContextMenu = theMenu.GetSubMenu (0);
	pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		ptMenu.x, ptMenu.y, pEventListFrame);

	pSekaijuDoc->m_theCriticalSection.Lock ();
	m_lCurButtonState = 0x0000;

	return TRUE;
}








//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���_�̈ړ����I�[�o�[���C�h
void CEventListIndexPropertyView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	pDC->SetWindowOrg (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
}

// �`��
void CEventListIndexPropertyView::OnDraw (CDC* pDC) {

	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// �N���e�B�J���Z�N�V�������b�N
	pSekaijuDoc->m_theCriticalSection.Lock ();

	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pMIDIEvent = NULL;

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (),pEventListFrame->GetRowScrollPos ());

	int i, j;
	long lButtonWidth = 12; //::GetSystemMetrics (SM_CXVSCROLL);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorGrayText = ::GetSysColor (COLOR_GRAYTEXT);

	// �w�i�̓h��Ԃ�
	long lVisibleTopRow = pEventListFrame->GetVisibleTopRow ();
	long lVisibleBottomRow = pEventListFrame->GetVisibleBottomRow ();
	long lRowZoom = pEventListFrame->GetRowZoom ();
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		long lBackColor = i % 2 ? RGB (255, 255, 192) : RGB (255, 255, 255);
		MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (i);
		if (pMIDIEvent) {
			if (!MIDIEvent_IsFloating (pMIDIEvent)) {
				// �I������Ă���C�x���g�̏ꍇ
				if (pSekaijuDoc->IsEventSelected (pMIDIEvent)) {
					lBackColor = i % 2 ? RGB (224, 224, 192) : RGB (224, 224, 255); 
				}
				// �I������Ă��Ȃ��C�x���g�̏ꍇ
				else {
					lBackColor = i % 2 ? RGB (255, 255, 192) : RGB (255, 255, 255);
				}
			}
		}
		long y = i * lRowZoom;
		pDC->FillSolidRect (rcClient.left, y, rcClient.right, y + lRowZoom, lBackColor);
	}	

	// �����̕`��(�e�s�̉��ɐ�)
	CPen penIndex (PS_SOLID, 1, RGB (128, 128, 128));
	CPen* pOldPen = pDC->SelectObject (&penIndex);
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		long y = (i + 1) * lRowZoom - 1;
		pDC->MoveTo (rcClient.left, y);
		pDC->LineTo (rcClient.right, y);
	}
	pDC->SelectObject (pOldPen);

	// �c���̕`��(�e��̉E�ɐ�)
	CPen penColumn (PS_SOLID, 1, RGB (128, 128, 128));
	pOldPen = pDC->SelectObject (&penColumn);
	long x = 0;
	long lColumnZoom = pEventListFrame->GetColumnZoom ();
	for (j = 0; j < 8; j++) {
		x += pEventListFrame->GetColumnBaseWidth (j) * lColumnZoom;
		if (j == 5 || j == 6) {
			continue;
		}
		pDC->MoveTo (x - 1, rcClient.top);
		pDC->LineTo (x - 1, rcClient.bottom);
	}
	pDC->SelectObject (pOldPen);

	// �����̕`��
	CRect rcCell;
	CRect rcText;
	CString strText;
	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (pEventListFrame->GetParentFont ());
	long lVisibleEventCount =pEventListFrame->GetVisibleEventCount ();
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		// �e�L�X�g�J���[�̐ݒ�
		long lForeColor = RGB (255, 255, 255);
		if (0 <= i && i < lVisibleEventCount) {
			VERIFY (pMIDIEvent = pEventListFrame->GetVisibleEvent (i));
		}
		if (pMIDIEvent) {
			// TODO:MIDI�C�x���g�������ւ����Ă��邪�܂�UpdateAllViews����ĂȂ��ꍇ�̉��[�u
			if (MIDIEvent_IsFloating (pMIDIEvent)) {
				continue;
			}
			MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
			ASSERT (pMIDITrack);
			lForeColor = MIDITrack_GetForeColor (pMIDITrack);
			pDC->SetTextColor (lForeColor);
		}
		// �ׯ���
		rcCell = GetRectFromCell (i, 0);
		rcText = rcCell;
		rcText.right -= lButtonWidth;
		_ASSERT (15 <= rcText.Height () && rcText.Height () <= 32);
		if (pMIDIEvent) {
			strText = GetCellString (i, 0);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// ��:��:�b:�ؕb
		rcCell = GetRectFromCell (i, 1);
		rcText = rcCell;
		rcText.right -= lButtonWidth;
		if (pMIDIEvent) {
			strText = GetCellString (i, 1);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// ����:��:è��
		rcCell = GetRectFromCell (i, 2);
		rcText = rcCell;
		rcText.right -= lButtonWidth;
		if (pMIDIEvent) {
			strText = GetCellString (i, 2);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// �C�x���g�̎��
		rcCell = GetRectFromCell (i, 3);
		rcText = rcCell;
		rcText.right -= lButtonWidth;
		if (pMIDIEvent) {
			strText = GetCellString (i, 3);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// �`�����l��
		rcCell = GetRectFromCell (i, 4);
		rcText = rcCell;
		rcText.right -= lButtonWidth;
		if (pMIDIEvent) {
			strText = GetCellString (i, 4);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// �l1
		rcCell = GetRectFromCell (i, 5);
		rcText = rcCell;
		rcText.right -= lButtonWidth;
		if (pMIDIEvent) {
			strText = GetCellString (i, 5);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// �l2, �l3
		if (pMIDIEvent) {
			long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
			if (0x80 <= lEventKind && lEventKind <= 0xEF) {
				rcCell = GetRectFromCell (i, 6);
				rcText = rcCell;
				rcText.right -= lButtonWidth;
				strText = GetCellString (i, 6);
				pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			
				rcCell = GetRectFromCell (i, 7);
				rcText = rcCell;
				rcText.right -= lButtonWidth;
				strText = GetCellString (i, 7);
				pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			
			}
		}
	}	
	pDC->SelectObject (pOldFont);

	
	// �{�^���ނ̕`��
	CPen penColorBtnText (PS_SOLID, 1, lColorBtnText);
	CPen penColorGrayText (PS_SOLID, 1, lColorGrayText);
	CBrush brsColorBtnText;
	CBrush brsColorGrayText;
	brsColorBtnText.CreateSolidBrush (lColorBtnText);
	brsColorGrayText.CreateSolidBrush (lColorGrayText);
	CBrush* pOldBrush = NULL;
	CRect rcButton;
	for (i = lVisibleTopRow; i <= lVisibleBottomRow; i++) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (i);
		long jMax;
		if (pMIDIEvent) {
			jMax = MIDIEvent_IsMIDIEvent (pMIDIEvent) ? 8 : 6;
		}
		else {
			jMax = 6;
		}
		for (j = 0; j < jMax; j++) {
			// �㉺�{�^�����h���b�v�_�E�����̔���
			BOOL bDropDown = FALSE;
			if (j == 0 || j == 3) {
				bDropDown = TRUE;
			}
			// �����̐F�����߂�y���̑I��
			if (pMIDIEvent) {
				if (j == 1 ||
					j >= 4 && !MIDIEvent_IsMIDIEvent (pMIDIEvent) ||
					j == 7 && MIDIEvent_IsKeyAftertouch (pMIDIEvent) ||
					j == 7 && MIDIEvent_IsControlChange (pMIDIEvent) ||
					j >= 6 && MIDIEvent_IsProgramChange (pMIDIEvent) ||
					j >= 6 && MIDIEvent_IsChannelAftertouch (pMIDIEvent) ||
					j >= 6 && MIDIEvent_IsPitchBend (pMIDIEvent)) {
					//continue;
					pOldPen = pDC->SelectObject (&penColorGrayText);
					pOldBrush = pDC->SelectObject (&brsColorGrayText);
				 
				}
				else {
					pOldPen = pDC->SelectObject (&penColorBtnText);
					pOldBrush = pDC->SelectObject (&brsColorBtnText);
				}
			}
			else {
				//continue;
				pOldPen = pDC->SelectObject (&penColorGrayText);
				pOldBrush = pDC->SelectObject (&brsColorGrayText);
			}
			rcCell = GetRectFromCell (i, j);
			rcButton  = rcCell;
			POINT pt[3];
			// �h���b�v�_�E���{�^���̏ꍇ
			if (bDropDown) {
				rcButton.left = rcCell.right - lButtonWidth;
				pDC->FillSolidRect (&rcButton, ::GetSysColor (COLOR_3DFACE));
				if (i == m_lCurRow && j == m_lCurColumn && (m_lCurButtonState & 0x0F)) {
					pDC->Draw3dRect (&rcButton, lColorBtnShadow, lColorBtnHighlight);
				}
				else {
					pDC->Draw3dRect (&rcButton, lColorBtnHighlight, lColorBtnShadow);
				}
				pt[0].x = rcCell.right - lButtonWidth / 2;
				pt[0].y = rcCell.top + lRowZoom * 1 / 2 + 1;
				pt[1].x = pt[0].x - 2;
				pt[1].y = pt[0].y - 2;
				pt[2].x = pt[0].x + 2;
				pt[2].y = pt[0].y - 2;
				pDC->Polygon (pt, 3);
			}
			// �㉺�{�^���̏ꍇ
			else {
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
			pDC->SelectObject (pOldPen);
			pDC->SelectObject (pOldBrush);
		}
	}

	// �J�����g�Z���̘g�`��
	DrawCurFrame (pDC);

	// �N���e�B�J���Z�N�V�������b�N����
	pSekaijuDoc->m_theCriticalSection.Unlock ();

}

// �r���[�̍X�V
void CEventListIndexPropertyView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// �N���e�B�J���Z�N�V�����̓��b�N����Ă�����̂Ƃ���B
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		PostMessage (WM_TIMER, 0x11, NULL);
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}

//#define CN_COMMAND              0               // void ()
//#define CN_UPDATE_COMMAND_UI    ((UINT)(-1))    // void (CCmdUI*)
//#define CN_EVENT                ((UINT)(-2))    // OLE event

// �R�}���h���[�e�B���O�̃I�[�o�[���C�h
BOOL CEventListIndexPropertyView::OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	// ����:pHandlerInfo==NULL�̎��̓R�}���h�����s���ANULL�łȂ��Ƃ���
	// �R�}���h�͎��s����pHandlerInfo�̒��g��ݒ肷�邱�Ƃ��Ӗ�����B
	if (nCode == CN_COMMAND && pHandlerInfo == NULL) {
		// �L�[�{�[�h�����쒆���̓}�E�X�����쒆�̏ꍇ
		if (m_lCurButtonState & 0x0FF0) {
		}
	}
	return CView::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �E�B���h�E������
BOOL CEventListIndexPropertyView::OnCreate (LPCREATESTRUCT lpcs) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	m_theTextBox.Create (WS_CHILD | ES_AUTOHSCROLL, 
		CRect (0, pEventListFrame->GetRowZoom () / 2 - 6, pEventListFrame->GetColumnWidth (0), 13), 
		this, IDC_TEXTBOX);
	m_theTextBox.SetFont (pEventListFrame->GetParentFont ());
	m_theListBox.Create (WS_CHILD | WS_VSCROLL | LBS_NOTIFY, 
		CRect (0, 0, 0, 0), this, IDC_TEXTBOX);
	m_theListBox.SetFont (pEventListFrame->GetParentFont ());

	SetTimer (0x11, 55, NULL);
	return CSekaijuView::OnCreate (lpcs);
}

// �E�B���h�E�j��
void CEventListIndexPropertyView::OnDestroy () {
	KillTimer (0x11);
}

// �t�H�[�J�X���������Ƃ�
void CEventListIndexPropertyView::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CEventListIndexPropertyView::OnKillFocus (pNewWnd=0x%08x)\n", (long)pNewWnd);
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	// �C���v���[�X�e�L�X�g�{�b�N�X���̓C���v���[�X���X�g�{�b�N�X�Ƀt�H�[�J�X���ڂ����ꍇ������
	if (pNewWnd != &m_theTextBox && pNewWnd != &m_theListBox && !m_bSettingCellString) {
		// �e�L�X�g�ҏW���̏ꍇ�͂��̓��e���m��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews 
				(NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
		}
		// ���X�g�I�𒆂̏ꍇ�͂��̓��e���m��
		if (IsListSelecting ()) {
			EndListSelectingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
			pSekaijuDoc->UpdateAllViews 
				(NULL, SEKAIJUDOC_MIDIEVENTCHANGED | SEKAIJUDOC_MIDITRACKCHANGED);
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
void CEventListIndexPropertyView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

	CString strText;
	// �Â��Z���̕`��(��̓I�ɂ̓Z���g�̍폜)(20100128:������h�~�̂��ߏ������ǉ�)
	if (IsTextEditing () == FALSE && IsListSelecting () == FALSE
		 && nChar != VK_CONTROL && nChar != VK_SHIFT && nChar != VK_MENU) {
		CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcOldCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
		rcOldCell.InflateRect (2, 2);
		InvalidateRect (&rcOldCell);
	}

	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	long lVisibleRows = 0;

	// �Y���s��MIDI�C�x���g���擾
	MIDIEvent* pMIDIEvent = NULL;
	if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	}
	long lKind = 0;
	if (pMIDIEvent) {
		lKind = MIDIEvent_GetKind (pMIDIEvent);
	}

	switch (nChar) {
	// ��L�[
	case VK_UP:
		// �e�L�X�g�{�b�N�X�ҏW���̏ꍇ�͕ҏW���e���m�肵�A���̃r���[�Ƀt�H�[�J�X���ڂ��B
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			m_lCurButtonState = 0x0000;
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
		m_lCurButtonState = 0x0000;
		// �J�����g�Z�����ЂƂ�ֈړ�
		m_lCurRow = CLIP (0, m_lCurRow - 1, 0x7FFFFFFF);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
		if (pSekaijuApp->m_bPlaying == FALSE) {
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			if (pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
			}
		}
		break;
	// ���L�[
	case VK_DOWN:
		// �e�L�X�g�{�b�N�X�ҏW���̏ꍇ�͕ҏW���e���m�肵�A���̃r���[�Ƀt�H�[�J�X���ڂ��B
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			m_lCurButtonState = 0x0000;
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
		m_lCurButtonState = 0x0000;
		// �J�����g�Z�����ЂƂ��ֈړ�
		m_lCurRow = CLIP (0, m_lCurRow + 1, lVisibleEventCount);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
		if (pSekaijuApp->m_bPlaying == FALSE) {
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			if (pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
			}
		}
		break;
	// PageUp
	case VK_PRIOR:
		// �e�L�X�g�{�b�N�X�ҏW���̏ꍇ�͕ҏW���e���m�肵�A���̃r���[�Ƀt�H�[�J�X���ڂ��B
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			m_lCurButtonState = 0x0000;
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
		m_lCurButtonState = 0x0000;
		// �J�����g�Z����1�y�[�W��ֈړ�
		lVisibleRows = pEventListFrame->GetVisibleBottomRow () - pEventListFrame->GetVisibleTopRow ();
		m_lCurRow = CLIP (0, m_lCurRow - __max (1, lVisibleRows), 0x7FFFFFFF);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
		if (pSekaijuApp->m_bPlaying == FALSE) {
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			if (pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
			}
		}
		break;
	// PageDown
	case VK_NEXT:
		// �e�L�X�g�{�b�N�X�ҏW���̏ꍇ�͕ҏW���e���m�肵�A���̃r���[�Ƀt�H�[�J�X���ڂ��B
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			m_lCurButtonState = 0x0000;
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
		m_lCurButtonState = 0x0000;
		// �J�����g�Z����1�y�[�W���ֈړ�
		lVisibleRows = pEventListFrame->GetVisibleBottomRow () - pEventListFrame->GetVisibleTopRow ();
		m_lCurRow = CLIP (0, m_lCurRow + __max (1, lVisibleRows), lVisibleEventCount);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
		if (pSekaijuApp->m_bPlaying == FALSE) {
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			if (pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
			}
		}
		break;
	// ���L�[
	case VK_LEFT:
		// �e�L�X�g�{�b�N�X�ҏW���̏ꍇ�͕ҏW���e���m�肵�A���̃r���[�Ƀt�H�[�J�X���ڂ��B
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			m_lCurButtonState = 0x0000;
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
		m_lCurButtonState = 0x0000;
		// �J�����g�Z�������ֈړ�
		m_lCurColumn = CLIP (0, m_lCurColumn - 1, 7);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		UpdateParentToolbarControl ();
		break;
	// �E�L�[
	case VK_RIGHT:
		// �e�L�X�g�{�b�N�X�ҏW���̏ꍇ�͕ҏW���e���m�肵�A���̃r���[�Ƀt�H�[�J�X���ڂ��B
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			m_lCurButtonState = 0x0000;
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
		m_lCurButtonState = 0x0000;
		// �J�����g�Z�����E�ֈړ�
		m_lCurColumn = CLIP (0, m_lCurColumn + 1, 7);
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		UpdateParentToolbarControl ();
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
		m_lCurButtonState = 0x0000;
		// �e�L�X�g�ҏW���łȂ��A�����X�g�I�𒆂łȂ��ꍇ�A
		// ���̃Z���̕ҏW���̓��X�g����̑I�����J�n����B
		if (IsTextEditing () == FALSE && IsListSelecting () == FALSE) {
			AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
			MoveTextBox (m_lCurRow, m_lCurColumn);
			MoveListBox (m_lCurRow, m_lCurColumn);
			pEventListFrame->m_bAutoPageUpdate = FALSE;
			UpdateParentToolbarControl ();

			// ���A���^�C�����͒��͉������Ȃ�
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}			
			// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
			if (pSekaijuDoc->m_bEditLocked) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return;
			}			

			MIDIEvent* pMIDIEvent = NULL;
			lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
			if (m_lCurRow < lVisibleEventCount) {
				pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			}
			else {
				break;
			}
			long lKind = MIDIEvent_GetKind (pMIDIEvent);
			switch (m_lCurColumn) {
			case 0: // �g���b�N
				BeginListSelecting ();
				break;
			case 2: // �^�C��
				BeginTextEditing ();
				break;
			case 3: // �C�x���g�̎��
				BeginListSelecting ();
				break;
			case 4: // �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂�)
				if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
					BeginTextEditing ();
				}
				break;
			case 5: // �l1
				if (!MIDIEvent_IsEndofTrack (pMIDIEvent)) {
					BeginTextEditing ();
				}
				break;
			case 6: // �l2
				if (!MIDIEvent_IsEndofTrack (pMIDIEvent) && !(0xC0 <= lKind && lKind <= 0xEF)) {
					BeginTextEditing ();
				}
				break;
			case 7: // �l3
				if (!MIDIEvent_IsEndofTrack (pMIDIEvent) && 0x80 <= lKind && lKind <= 0x9F) {
					BeginTextEditing ();
				}
				break;
			}
		}
		// �e�L�X�g�ҏW���̏ꍇ�A�e�L�X�g�ҏW���m��I������(CInplaceEdit�N���X����PostMessage�����B)
		else if (IsTextEditing ()) {
			EndTextEditingOK ();
		}
		// ���X�g�I�𒆂̏ꍇ�A���X�g�I�����m��I������(CInplaceList�N���X����PostMessage�����B)
		else if (IsListSelecting ()) {
			EndListSelectingOK ();
		}
		break;
	// �G�X�P�[�v(Esc)�L�[(CInplaceEdit�N���X����PostMessage�����B)
	case VK_ESCAPE:
		// �e�L�X�g�ҏW���̏ꍇ�A�e�L�X�g�ҏW���L�����Z���I������(CInplaceEdit�N���X����PostMessage�����B)
		if (IsTextEditing ()) {
			EndTextEditingCancel ();
		}
		// ���X�g�I�𒆂̏ꍇ�A���X�g�I�����L�����Z���I������(CInplaceList�N���X����PostMessage�����B)
		else if (IsListSelecting ()) {
			EndListSelectingCancel ();
		}
		break;
	// +�L�[
	case VK_ADD:
	case 187:
		// ���Ƀ}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		// ���A���^�C�����͒��͉������Ȃ�
		if (pSekaijuDoc->m_bEditLocked) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			return;
		}			
		// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
		if (pSekaijuDoc->m_bEditLocked) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			return;
		}			
		switch (m_lCurColumn) {
		case 2:
		case 4:
		case 5:
		case 6:
		case 7:
			// ���񉟂����̂݃L�[�{�[�h����J�n
			if (m_lCurButtonState == 0x0000) {
				BeginValueUpDown ();
				SetCapture ();
				m_lCurButtonState |= 0x0101;
			}
			// �l�̑���
			if (::GetKeyState (VK_SHIFT) < 0) {
				if (m_lCurColumn == 5 && 0x80 <= lKind && lKind <= 0xAF) {
					AddValueOfCurCell (12);
				}
				else {
					AddValueOfCurCell (10);
				}
			}
			else {
				AddValueOfCurCell (1);
			}
			break;
		}
		break;
	// -�L�[
	case VK_SUBTRACT:
	case 189:
		// ���Ƀ}�E�X�ő��쒆�̏ꍇ�͂��̑�����~
		if (m_lCurButtonState & 0x00F0) {
			ReleaseCapture ();
			KillTimer (1);
			EndValueUpDown ();
			m_lCurButtonState &= 0xFF00;
		}
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		// ���A���^�C�����͒��͉������Ȃ�
		if (pSekaijuDoc->m_bEditLocked) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			return;
		}			
		// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
		if (pSekaijuDoc->m_bEditLocked) {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			return;
		}			
		switch (m_lCurColumn) {
		case 2:
		case 4:
		case 5:
		case 6:
		case 7:
			// ���񉟂����̂݃L�[�{�[�h����J�n
			if (m_lCurButtonState == 0x0000) {
				BeginValueUpDown ();
				SetCapture ();
				m_lCurButtonState |= 0x0202;
			}
			// �l�̑���
			if (::GetKeyState (VK_SHIFT) < 0) {
				if (m_lCurColumn == 5 && 0x80 <= lKind && lKind <= 0xAF) {
					AddValueOfCurCell (-12);
				}
				else {
					AddValueOfCurCell (-10);
				}
			}
			else {
				AddValueOfCurCell (-1);
			}
			break;
		}
		break;
	}
	
	// �V�����Z���̕`��(20100128:������h�~�̂��ߏ������ǉ�)
	if (IsTextEditing () == FALSE && IsListSelecting () == FALSE
		 && nChar != VK_CONTROL && nChar != VK_SHIFT && nChar != VK_MENU) {
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell);
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �L�[�����グ��
void CEventListIndexPropertyView::OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
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



// �������ړ��͎�
void CEventListIndexPropertyView::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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

	MIDIEvent* pMIDIEvent = NULL;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	if (m_lCurRow < lVisibleEventCount) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	}
	else {
		return;
	}
	long lKind = MIDIEvent_GetKind (pMIDIEvent);

	switch (m_lCurColumn) {
	case 0: // �g���b�N
		return;
	case 2: // ����:��:�e�B�b�N
		if (48 <= nChar && nChar <= 57) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		return;
	case 3: // �C�x���g�̎��
		return;
	case 4: // �`�����l��(MIDI�`�����l���C�x���g�̂Ƃ��̂�)
		if (0x80 <= lKind && lKind <= 0xEF && 48 <= nChar && nChar <= 57) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		return;
	case 5: // �l1
		if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
		}
		else if (!MIDIEvent_IsMIDIEvent (pMIDIEvent) && 33 <= nChar && nChar <= 126) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		else if (48 <= nChar && nChar <=57) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		return;
	case 6: // �l2
		if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
		}
		else if (!MIDIEvent_IsMIDIEvent (pMIDIEvent) && 33 <= nChar && nChar <= 126) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		else if (0x80 <= lKind && lKind <= 0xBF && 48 <= nChar && nChar <= 57) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		return;
	case 7: // �l3
		if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
		}
		else if (!MIDIEvent_IsMIDIEvent (pMIDIEvent) && 33 <= nChar && nChar <= 126) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		else if (0x80 <= lKind && lKind <= 0x9F && 48 <= nChar && nChar <= 57) {
			BeginTextEditing ();
			//m_theTextBox.PostMessage (WM_CHAR, nChar, (nRepCnt << 16) | nFlags);
			m_theTextBox.PostMessage (WM_CHAR, nChar, (nFlags & 0xFFFF0000) | (nRepCnt & 0x0000FFFF));
			// ��20080722�F���̍s��Windows2000�Ή��B
		}
		return;
	}
}


// �}�E�X���{�^�������ꂽ�� 
void CEventListIndexPropertyView::OnLButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	point += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

	BOOL bOldTextEditing = FALSE;
	BOOL bOldListSelecting = FALSE;
	long lButtonWidth = 12; //::GetSystemMetrics (SM_CXVSCROLL);
	// �e�L�X�g�ҏW�̏I��
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		m_lCurButtonState = 0x0000;
		bOldTextEditing = TRUE;
	}

	// ���X�g�I���I��
	if (IsListSelecting ()) {
		EndListSelectingCancel ();
		m_lCurButtonState = 0x0000;
		bOldListSelecting = TRUE;
	}
	
	// �Â��Z���̕`��
	CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcOldCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	rcOldCell.InflateRect (2, 2);
	InvalidateRect (&rcOldCell);
	
	// �V�����Z���̐ݒ�(���O���N���b�N�����ꍇ�͉����N����Ȃ��ŏI��)
	if (m_lCurButtonState == 0x0000) {
		long i = 0;
		long j = 0;
		GetCellFromPoint (point, &i, &j);
		if (0 <= i && i <= 0x7FFFFFFF && 0 <= j && j < 8) {
			m_lCurRow = i;
			m_lCurColumn = j;
		}
		else {
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);
			return;
		}
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
	}
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);

	// �Y���s��MIDI�C�x���g���擾
	MIDIEvent* pMIDIEvent = NULL;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	}

	// MIDI�C�x���g�̂���s�̏ꍇ�̂�
	long lRowZoom = pEventListFrame->GetRowZoom ();
	if (pMIDIEvent) {
		long lKind = MIDIEvent_GetKind (pMIDIEvent);
		// ��ƃC�x���g�ɂ�胊�X�g�I���J�n���͒l�̑����J�n
		switch (m_lCurColumn) {
		case 0: // �g���b�N
		case 3: // �C�x���g�̎��
			// �^�����͉������Ȃ�
			if (pSekaijuApp->m_bRecording) {
				break;
			}
			// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
			if (pSekaijuDoc->m_bEditLocked) {
				break;
			}
			if (point.x >  rcNewCell.right - lButtonWidth && bOldListSelecting == FALSE) {
				m_lCurButtonState = 0x0003;
				BeginListSelecting ();
			}
			break;
		case 2: // ����:��:�e�B�b�N
			if (point.x > rcNewCell.right - lButtonWidth) {
				// �^�����͉������Ȃ�
				if (pSekaijuApp->m_bRecording) {
					break;
				}
				// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
				if (pSekaijuDoc->m_bEditLocked) {
					break;
				}
				// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					// ��̃{�^���������ꂽ�ꍇ
					if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
			break;
		case 4: // �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂�)
			if (0x80 <= lKind && lKind <= 0xEF) {
				if (point.x > rcNewCell.right - 10) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
			break;
		case 5: // �l1
			if (0x80 <= lKind && lKind <= 0xEF) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
			break;
		case 6: // �l2
			if (0x80 <= lKind && lKind <= 0xBF) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
			break;
		case 7: // �l3
			if (0x80 <= lKind && lKind <= 0x9F) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
			break;
		}
	}

	// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
	if (pSekaijuApp->m_bPlaying == FALSE) {
		MIDIEvent* pMIDIEvent = NULL;
		lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
		if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
			pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
		}
		if (pMIDIEvent) {
			long lTime = MIDIEvent_GetTime (pMIDIEvent);
			pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
		}
	}

	// �V�����Z���̕`��
	rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell, FALSE);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�������ꂽ��  
void CEventListIndexPropertyView::OnRButtonDown (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

	pSekaijuDoc->m_theCriticalSection.Lock ();

	CPoint ptMenu = point;
	ClientToScreen (&ptMenu);

	CRect rcClient;
	GetClientRect (&rcClient);
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	point += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

	BOOL bOldTextEditing = FALSE;
	BOOL bOldListSelecting = FALSE;
	long lButtonWidth = 12; //::GetSystemMetrics (SM_CXVSCROLL);
	// �e�L�X�g�ҏW�̏I��
	if (IsTextEditing ()) {
		EndTextEditingOK ();
		m_lCurButtonState = 0x0000;
		bOldTextEditing = TRUE;
	}

	// ���X�g�I���I��
	if (IsListSelecting ()) {
		EndListSelectingCancel ();
		m_lCurButtonState = 0x0000;
		bOldListSelecting = TRUE;
	}
	
	// �Â��Z���̕`��
	CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	rcOldCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	rcOldCell.InflateRect (2, 2);
	InvalidateRect (&rcOldCell);
	
	// �V�����Z���̐ݒ�
	if (m_lCurButtonState == 0x0000) {
		long i = 0;
		long j = 0;
		GetCellFromPoint (point, &i, &j);
		if (0 <= i && i <= 0x7FFFFFFF && 0 <= j && j < 8) {
			m_lCurRow = i;
			m_lCurColumn = j;
		}
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
	}
	CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);

	// �Y���s��MIDI�C�x���g���擾
	MIDIEvent* pMIDIEvent = NULL;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	}

	// MIDI�C�x���g�̂���s�̏ꍇ�̂�
	long lRowZoom = pEventListFrame->GetRowZoom ();
	if (pMIDIEvent) {
		long lKind = MIDIEvent_GetKind (pMIDIEvent);
		// ��ƃC�x���g�ɂ�胊�X�g�I���J�n���͒l�̑����J�n
		switch (m_lCurColumn) {
		case 0: // �g���b�N
		case 1: // ��:��:�b:�~���b
			ShowPopupMenu (ptMenu);
			break;
		case 3: // �C�x���g�̎��
			if (point.x > rcNewCell.right - lButtonWidth) {
				// �^�����͉������Ȃ�
				if (pSekaijuApp->m_bRecording) {
					break;
				}
				// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
				if (pSekaijuDoc->m_bEditLocked) {
					break;
				}
				m_lCurButtonState = 0x03;
				BeginListSelecting ();
			}
			else {
				ShowPopupMenu (ptMenu);
			}
			break;
		case 2: // ����:��:�e�B�b�N
			if (point.x > rcNewCell.right - lButtonWidth) {
				// �^�����͉������Ȃ�
				if (pSekaijuApp->m_bRecording) {
					break;
				}
				// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
				if (pSekaijuDoc->m_bEditLocked) {
					break;
				}
				// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
				if (m_lCurButtonState == 0x0000) {
					BeginValueUpDown ();
					// ��̃{�^���������ꂽ�ꍇ
					if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
			else {
				ShowPopupMenu (ptMenu);
			}
			break;
		case 4: // �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂�)
			if (0x80 <= lKind && lKind <= 0xEF) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
				else {
					ShowPopupMenu (ptMenu);
				}
			}
			else {
				ShowPopupMenu (ptMenu);
			}
			break;
		case 5: // �l1
			if (0x80 <= lKind && lKind <= 0xEF) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
							m_lCurButtonState |= 0x0021;
							if (0x80 <= lKind && lKind <= 0xA0) {
								AddValueOfCurCell (12);
							}
							else {
								AddValueOfCurCell (10);
							}
						}
						// ���̃{�^���������ꂽ�ꍇ
						else {
							m_lCurButtonState |= 0x0022;
							if (0x80 <= lKind && lKind <= 0xA0) {
								AddValueOfCurCell (-12);
							}
							else {
								AddValueOfCurCell (-10);
							}
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
				else {
					ShowPopupMenu (ptMenu);
				}
			}
			else {
				ShowPopupMenu (ptMenu);
			}
			break;
		case 6: // �l2
			if (0x80 <= lKind && lKind <= 0xBF) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
				else {
					ShowPopupMenu (ptMenu);
				}
			}
			else {
				ShowPopupMenu (ptMenu);
			}
			break;
		case 7: // �l3
			if (0x80 <= lKind && lKind <= 0x9F) {
				if (point.x > rcNewCell.right - lButtonWidth) {
					// �^�����͉������Ȃ�
					if (pSekaijuApp->m_bRecording) {
						break;
					}
					// MIDI�f�[�^�����b�N����Ă���ꍇ�͉������Ȃ�
					if (pSekaijuDoc->m_bEditLocked) {
						break;
					}
					// �L�[�{�[�h���̓}�E�X�ɂ�鑀����J�n����ꍇ
					if (m_lCurButtonState == 0x0000) {
						BeginValueUpDown ();
						// ��̃{�^���������ꂽ�ꍇ
						if (point.y < rcNewCell.top + pEventListFrame->GetRowZoom () / 2) {
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
				else {
					ShowPopupMenu (ptMenu);
				}
			}
			else {
				ShowPopupMenu (ptMenu);
			}
			break;
		}
	}

	// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
	if (pSekaijuApp->m_bPlaying == FALSE) {
		MIDIEvent* pMIDIEvent = NULL;
		lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
		if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
			pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
		}
		if (pMIDIEvent) {
			long lTime = MIDIEvent_GetTime (pMIDIEvent);
			pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
		}
	}

	// �V�����Z���̕`��
	rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	rcNewCell.InflateRect (2, 2);
	InvalidateRect (&rcNewCell, FALSE);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);

}

// �}�E�X���{�^�������ꂽ�Ƃ�
void CEventListIndexPropertyView::OnLButtonUp (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�������ꂽ�Ƃ�
void CEventListIndexPropertyView::OnRButtonUp (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);


}

// �}�E�X���������ꂽ�Ƃ�
void CEventListIndexPropertyView::OnMouseMove (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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
void CEventListIndexPropertyView::OnLButtonDblClk (UINT nFlags, CPoint point) {

	long lButtonWidth = 12; //::GetSystemMetrics (SM_CXVSCROLL);

	// ���OnLButtonDown�����s�����A�e�L�X�g�ҏW�I���E���X�g�I���I���E�J�����g�Z���̈ړ����ς܂��Ă����B
	SendMessage (WM_LBUTTONDOWN, nFlags, ((point.y  & 0x0000FFFF) << 16) | (point.x & 0x0000FFFF));
	if (IsTextEditing () || IsListSelecting ()) {
		return;
	}
	if (m_lCurButtonState != 0x0000) {
		return;
	}

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();

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
	rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
	point += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

	// �V�����Z���̎擾
	long i = 0;
	long j = 0;
	GetCellFromPoint (point, &i, &j);
	if (0 <= i && i <= 0x7FFFFFFF && 0 <= j && j < 8) {
		m_lCurRow = i;
		m_lCurColumn = j;
	}
	CRect rcCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
	
	// �e�L�X�g�ҏW���[�h���͐�p�_�C�A���O���[�h�ɓ˓��B
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	if (point.x < rcCell.right - lButtonWidth) {
		MIDIEvent* pMIDIEvent = NULL;
		if (m_lCurRow < lVisibleEventCount) {
			VERIFY (pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow));
		}
		else {
			return;
		}
		long lKind = MIDIEvent_GetKind (pMIDIEvent);
		switch (m_lCurColumn) {
		case 0: // �g���b�N
			BeginListSelecting ();
			break;
		case 2: // ����:��:�e�B�b�N
			BeginTextEditing ();
			break;
		case 3: // �C�x���g�̎��
			BeginListSelecting ();
			break;
		case 4: // �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂�)
			if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
				BeginTextEditing ();
			}
			break;
		case 5: // �l1
			if (!MIDIEvent_IsEndofTrack (pMIDIEvent)) {
				BeginTextEditing ();
			}
			break;
		case 6: // �l2
			if (!MIDIEvent_IsEndofTrack (pMIDIEvent) && !(0xC0 <= lKind && lKind <= 0xEF)) {
				BeginTextEditing ();
			}
			break;
		case 7: // �l3
			if (!MIDIEvent_IsEndofTrack (pMIDIEvent) && !(0xA0 <= lKind && lKind <= 0xEF)) {
				BeginTextEditing ();
			}
			break;
		}
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�E�{�^�����_�u���N���b�N���ꂽ�Ƃ�
void CEventListIndexPropertyView::OnRButtonDblClk (UINT nFlags, CPoint point) {
}

// �^�C�}�[��
void CEventListIndexPropertyView::OnTimer (UINT nIDEvent) {	

	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	// �Y���s��MIDI�C�x���g���擾
	MIDIEvent* pMIDIEvent = NULL;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
		pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
	}
	long lKind = 0;
	if (pMIDIEvent) {
		lKind = MIDIEvent_GetKind (pMIDIEvent);
	}

	// �㉺�{�^������+-�L�[�ɂ��l�̑���
	if (nIDEvent == 1) {
		// ��������
		if (m_lCurButtonInterval > 50) {
			KillTimer (1);
			m_lCurButtonInterval = 50;
			SetTimer (1, m_lCurButtonInterval, NULL);
		}
		// ��{�^�������N���b�N�ŉ�����Ă���
		if ((m_lCurButtonState & 0x00FF) == 0x0011) {
			AddValueOfCurCell (1);
		}
		// ���{�^�������N���b�N�ŉ�����Ă���
		else if ((m_lCurButtonState & 0x00FF) == 0x0012) {
			AddValueOfCurCell (-1);
		}
		// ��{�^�����E�N���b�N�ŉ�����Ă���
		if ((m_lCurButtonState & 0x00FF) == 0x0021) {
			if (m_lCurColumn == 5 && 0x80 <= lKind && lKind <= 0xAF) {
				AddValueOfCurCell (12);
			}
			else {
				AddValueOfCurCell (10);
			}
		}
		// ���{�^�����E�N���b�N�ŉ�����Ă���
		else if ((m_lCurButtonState & 0x00FF) == 0x0022) {
			if (m_lCurColumn == 5 && 0x80 <= lKind && lKind <= 0xAF) {
				AddValueOfCurCell (-12);
			}
			else {
				AddValueOfCurCell (-10);
			}
		}
		// ��{�^�������N���b�N�ŉ�����Ă���
		if ((m_lCurButtonState & 0x00FF) == 0x0031) {
			AddValueOfCurCell (10);
		}
		// ���{�^�������N���b�N�ŉ�����Ă���
		else if ((m_lCurButtonState & 0x00FF) == 0x0032) {
			AddValueOfCurCell (-10);
		}
		
		// �Z���̍ĕ`��
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell, FALSE);
	}

	// �Đ����Ȃǂ̃J�����g�Z�������ړ��p(�y�[�W�X�V�@�\�t��)
	else if (nIDEvent == 0x11) {
		if (pEventListFrame->m_bAutoPageUpdate == TRUE
			&& IsTextEditing () == FALSE && IsListSelecting () == FALSE) {
			long lCurTime = pSekaijuDoc->m_lNewTime;
			long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			long lEventTime = 0;
			if (pMIDIEvent) {
				lEventTime = MIDIEvent_GetTime (pMIDIEvent);
			}
			// �J�����g�Z�����X�V����K�v������ꍇ�̂�
			if (lCurTime != lEventTime) { 
				// �Z�������ݎ����̃C�x���g�ɍ��킹��B			
				long i;
				for (i = 0; i < lVisibleEventCount; i++) {
					VERIFY (pMIDIEvent = pEventListFrame->GetVisibleEvent (i));
					if (MIDIEvent_GetTime (pMIDIEvent) >= lCurTime) {
						break;
					}
				}
				CClientDC dc (this);
				OnPrepareDC (&dc, NULL);

				CRect rcClient;
				GetClientRect (&rcClient);
				rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

				// ���t���[���̏���
				DrawCurFrame (&dc);
				// �J�����g�Z���̍X�V								
				m_lCurRow = i;
				MoveTextBox (m_lCurRow, m_lCurColumn);
				MoveListBox (m_lCurRow, m_lCurColumn);
				// �V�t���[���̕`��
				DrawCurFrame (&dc);
												
				int n = 0;
				// �J�����g�Z�����r���[�̏�ɂ͂ݏo�����ꍇ�̏���
				long lRowZoom = pEventListFrame->GetRowZoom ();
				if (m_lCurRow * lRowZoom < rcClient.top) {
					n = m_lCurRow * lRowZoom;
					pEventListFrame->SetRowScrollPos (n);		
					MoveTextBox (m_lCurRow, m_lCurColumn);
					MoveListBox (m_lCurRow, m_lCurColumn);
				}
				// �J�����g�Z�����r���[�̉��ɂ͂ݏo�����ꍇ�̏���
				else if ((m_lCurRow + 1) * lRowZoom > rcClient.bottom) {
					n = m_lCurRow * lRowZoom;
					pEventListFrame->SetRowScrollPos (n);
					MoveTextBox (m_lCurRow, m_lCurColumn);
					MoveListBox (m_lCurRow, m_lCurColumn);
				}
			}
		}
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �}�E�X�z�C�[�����񂳂ꂽ��
void CEventListIndexPropertyView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	
	// ���t�ʒu�̈ړ�
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
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
		rcClient += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
		point += CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());

		// �e�L�X�g�ҏW�̏I��
		if (IsTextEditing ()) {
			EndTextEditingOK ();
			pSekaijuDoc->SetModifiedFlag (TRUE);
		}
		// ���X�g�I���I��
		if (IsListSelecting ()) {
			EndListSelectingCancel ();
			m_lCurButtonState = 0x0000;
		}
		
		// �Â��Z���̕`��
		CRect rcOldCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcOldCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
		rcOldCell.InflateRect (2, 2);
		InvalidateRect (&rcOldCell);

		// �V�����Z���̐ݒ�
		AutoScrolltoShowCell (m_lCurRow, m_lCurColumn);
		MoveTextBox (m_lCurRow, m_lCurColumn);
		MoveListBox (m_lCurRow, m_lCurColumn);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
		UpdateParentToolbarControl ();
		CRect rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);

		// �Y���s��MIDI�C�x���g���擾
		MIDIEvent* pMIDIEvent = NULL;
		long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
		if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
			pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
		}		

		// MIDI�C�x���g�̂���s�̏ꍇ�̂�
		long lRowZoom = pEventListFrame->GetRowZoom ();
		if (pMIDIEvent) {
			long lKind = MIDIEvent_GetKind (pMIDIEvent);
			long lDeltaValue = GetKeyState (VK_SHIFT) < 0 ? 10 : 1;
			// ��ƃC�x���g�ɂ�胊�X�g�I���J�n���͒l�̑����J�n
			switch (m_lCurColumn) {
			case 2: // ����:��:�e�B�b�N
				if (m_lCurButtonState == 0x0000) {
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
					pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
				}
				break;
			case 4: // �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂�)
				if (0x80 <= lKind && lKind <= 0xEF) {
					if (m_lCurButtonState == 0x0000) {
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
						pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					}
				}
				break;
			case 5: // �l1
				if (0x80 <= lKind && lKind <= 0xEF) {
					if (m_lCurButtonState == 0x0000) {
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
						pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					}
				}
				break;
			case 6: // �l2
				if (0x80 <= lKind && lKind <= 0xBF) {
					if (m_lCurButtonState == 0x0000) {
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
						pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					}
				}
				break;
			case 7: // �l3
				if (0x80 <= lKind && lKind <= 0x9F) {
					if (m_lCurButtonState == 0x0000) {
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
						pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_MIDIEVENTCHANGED);
					}
				}
				break;
			}
		}

		// ��~���̏ꍇ�A���݂̃Z���̃C�x���g�������Đ������Ƃ���
		if (pSekaijuApp->m_bPlaying == FALSE) {
			MIDIEvent* pMIDIEvent = NULL;
			lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
			if (0 <= m_lCurRow && m_lCurRow < lVisibleEventCount) {
				pMIDIEvent = pEventListFrame->GetVisibleEvent (m_lCurRow);
			}
			if (pMIDIEvent) {
				long lTime = MIDIEvent_GetTime (pMIDIEvent);
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, lTime);
			}
		}

		// �V�����Z���̕`��
		rcNewCell = GetRectFromCell (m_lCurRow, m_lCurColumn);
		rcNewCell -= CSize (pEventListFrame->GetColumnScrollPos (), pEventListFrame->GetRowScrollPos ());
		rcNewCell.InflateRect (2, 2);
		InvalidateRect (&rcNewCell);

		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);
	}

	// ��ʃX�N���[��
	else {
		long lRowScrollPos = pEventListFrame->GetRowScrollPos ();
		long lRowZoom = pEventListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pEventListFrame->SetRowScrollPos (lRowScrollPos);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
	}
}