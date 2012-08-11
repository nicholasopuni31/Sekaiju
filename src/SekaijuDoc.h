//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���h�L�������g�N���X
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

#ifndef _SEKAIJUDOC_H_
#define _SEKAIJUDOC_H_

#define SEKAIJUDOC_ARGB(A,R,G,B) ((A)<<24|(R)<<16|(G)<<8|B)

// UpdateAllViews�ɐݒ肷��lHint�t���O
#define SEKAIJUDOC_PLAYSTARTED         0x00000001
#define SEKAIJUDOC_PLAYSTOPED          0x00000002
#define SEKAIJUDOC_RECORDSTARTED       0x00000004
#define SEKAIJUDOC_RECORDSTOPED        0x00000008
#define SEKAIJUDOC_POSITIONCHANGED     0x00000010

#define SEKAIJUDOC_MIDIDATACHANGED     0x00000100
#define SEKAIJUDOC_MIDITRACKCHANGED    0x00000200
#define SEKAIJUDOC_MIDIEVENTCHANGED    0x00000400

// MIDIData�I�u�W�F�N�g�̃t���O�̈Ӗ�(pMIDITrack->m_lUserFlag);
#define MIDIDATA_VISIBLE              0x00000001
#define MIDIDATA_ENABLE               0x00000002
#define MIDIDATA_SELECTED             0x00000004
#define MIDIDATA_ALIVE                0x00000100
#define MIDIDATA_DEAD                 0x00000200
#define MIDIDATA_RESISTEREDASINSERTED 0x00000400
#define MIDIDATA_RESISTEREDASREMOVED  0x00000800

// MIDITrack�I�u�W�F�N�g�̃t���O�̈Ӗ�(pMIDITrack->m_lUserFlag);
#define MIDITRACK_VISIBLE              0x00000001
#define MIDITRACK_ENABLE               0x00000002
#define MIDITRACK_SELECTED             0x00000004
#define MIDITRACK_ALIVE                0x00000100
#define MIDITRACK_DEAD                 0x00000200
#define MIDITRACK_RESISTEREDASINSERTED 0x00000400
#define MIDITRACK_RESISTEREDASREMOVED  0x00000800

// MIDIEvent�I�u�W�F�N�g�̃t���O�̈Ӗ�(pMIDIEvent->m_lUserFlag);
#define MIDIEVENT_VISIBLE              0x00000001
#define MIDIEVENT_ENABLE               0x00000002
#define MIDIEVENT_SELECTED             0x00000004
#define MIDIEVENT_VIEWMODEDRUM         0x00000008
#define MIDIEVENT_ALIVE                0x00000100
#define MIDIEVENT_DEAD                 0x00000200
#define MIDIEVENT_RESISTEREDASINSERTED 0x00000400
#define MIDIEVENT_RESISTEREDASREMOVED  0x00000800


#define SEKAIJUDOC_MODENATIVE          0x00000000 // Native
#define SEKAIJUDOC_MODEGM1             0x7E000001 // GM1
#define SEKAIJUDOC_MODEGMOFF           0x7E000002 // GMOff(=Native)
#define SEKAIJUDOC_MODEGM2             0x7E000003 // GM2
#define SEKAIJUDOC_MODEGS              0x41000002 // GS
#define SEKAIJUDOC_MODE88              0x41000003 // 88
#define SEKAIJUDOC_MODEXG              0x43000002 // XG

class CSekaijuDoc : public CDocument {

	DECLARE_DYNCREATE (CSekaijuDoc)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	MIDIData* m_pMIDIData;              // �h�L�������g�ƂȂ�MIDI�f�[�^�ւ̃|�C���^
	MIDIClock* m_pMIDIClock;            // MIDI�N���b�N�ւ̃|�C���^
	long m_lOldTime;                    // �O��̉��t�ʒu����(0�`)[tick]
	long m_lNewTime;                    // ���݂̉��t�ʒu����(0�`)[tick]
	CCriticalSection m_theCriticalSection;  // �N���e�B�J���Z�N�V����(��1)
public:
	BOOL m_bEditLocked;                 // ����MIDI�f�[�^�͕ҏW���֎~����Ă��邩�H(20100128�ǉ�)
	BOOL m_bSaveLocked;                 // ����MIDI�f�[�^�͕ۑ����֎~����Ă��邩�H(20100128�ǉ�)
public:
	time_t m_tmFileOpenTime;            // �t�@�C�����Ō�ɊJ��������
	time_t m_tmLastAutoSaveTime;        // �t�@�C�����Ō�ɕۑ���������
public:
	long m_lTempTime;                   // �ꎞ�I�ȃ^�C��(0�`)[tick]
	long m_lTempTrackIndex;             // �ꎞ�I�ȃg���b�N�ԍ�(0�`65535)
	MIDIEvent* m_pTempEvent;            // �ꎞ�I�ȃC�x���g�ւ̃|�C���^
	MIDITrack* m_pTempTrack;            // �ꎞ�I�ȃg���b�N�ւ̃|�C���^
protected:
	long m_lCurHistoryPosition;         // ���݂̗������j�b�g�ԍ�(0�`)
	CPtrArray m_theHistoryUnitArray;    // �������j�b�g�z��
	// ��1�F���C���X���b�h�Ɖ��t�^���p�X���b�h�������ɂ��̃h�L�������g�ɃA�N�Z�X(R/W)����̂�h���B

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CSekaijuDoc();                      // �R���X�g���N�^
	virtual ~CSekaijuDoc();             // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
public:
	virtual BOOL OnNewDocument ();
	virtual BOOL OnOpenDocument (LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument (LPCTSTR lpszPathName);
	virtual void OnCloseDocument ();

	//--------------------------------------------------------------------------
	// �f�f
	//--------------------------------------------------------------------------
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	virtual BOOL    ApplyAppCurSpeedIndex ();
	virtual void    TimeMIDIStatus (long lTargetTime, MIDIStatus* pTempMIDIStatus[]);
	virtual BOOL    SetClipboardTextPrivate9 (CString& strData);
	virtual BOOL    GetClipboardTextPrivate9 (CString& strText);
	virtual BOOL    IsClipboardTextPrivate9 ();
	virtual long    MakeCopyString (CString& strText, long lCopyMode, CHistoryUnit* pHistoryUnit);
	virtual long    ParsePasteString (CString& strData, long lBeginTrackIndex, long lBeginTime, CHistoryUnit* pHistoryUnit);
	virtual BOOL    DeleteSelectedEvent (CHistoryUnit* pHistoryUnit);
	virtual long    SetEventSelected (MIDIEvent* pMIDIEvent, BOOL bSelected);
	virtual long    IsEventSelected (MIDIEvent* pMIDIEvent);
	virtual long    SelectNoObject (CHistoryUnit* pHistoryUnit);
	virtual long    SelectAllObject (CHistoryUnit* pHistoryUnit);
	virtual MIDIEvent* SelectEvent (MIDIEvent* pMIDIEvent, long bSelected, CHistoryUnit* pHistoryUnit);
	virtual MIDITrack* SelectTrack (MIDITrack* pMIDITrack, long bSelected, CHistoryUnit* pHistoryUnit);
	virtual long    IsTrackSelected (MIDITrack* pMIDITrack);
	virtual long    SelectTrackMeasure (MIDITrack* pMIDITrack, long lMeasure, long bSelected, CHistoryUnit* pHistoryUnit);
	virtual long    IsTrackMeasureSelected (MIDITrack* pMIDITrack, long lMeasure);
	virtual long    AddDefaultEventToTrack (MIDITrack* pMIDITrack, long lMode, CHistoryUnit* pHistoryUnit);
	virtual long    AddTrack (long lNeedTrackCount, long lMode, CHistoryUnit* pHistoryUnit);
	virtual MIDITrack* GetTrack (long lIndex);
	virtual long    GetTrackIndex (MIDITrack* pMIDITrack);
	virtual CString GetTrackName (MIDITrack* pMIDITrack);
	virtual BOOL    SetTrackName (MIDITrack* pMIDITrack, CString strTrackName);
	virtual long    GetTrackVisible (MIDITrack* pMIDITrack);
	virtual BOOL    SetTrackVisible (MIDITrack* pMIDITrack, long lVisible);
	virtual long    GetTrackEnable (MIDITrack* pMIDITrack);
	virtual BOOL    SetTrackEnable (MIDITrack* pMIDITrack, long lEnable);
	virtual long    GetTrackSelected (MIDITrack* pMIDITrack);
	virtual BOOL    SetTrackSelected (MIDITrack* pMIDITrack, long lSelected);
	virtual MIDIEvent* GetTrackFirstControlChange (MIDITrack* pMIDITrack, long lNumber);
	virtual MIDIEvent* GetTrackFirstProgramChange (MIDITrack* pMIDITrack);
	virtual MIDIEvent* FindBankMSB (MIDIEvent* pMIDIEvent);
	virtual MIDIEvent* FindBankLSB (MIDIEvent* pMIDIEvent);
	virtual MIDIEvent* FindProgramChange (MIDIEvent* pMIDIEvent);
	virtual CString GetKeyName (MIDITrack* pMIDITrack, long lTime, long lKey); // 20090501�ǉ�
	virtual BOOL    LongTimeToStringMillisec (MIDIData* pMIDIData, long lTime, CString* pstrText); // 20090502�ǉ�
	virtual BOOL    LongTimeToStringTime (MIDIData* pMIDIData, long lTime, CString* pstrText); // 20090502�ǉ�
	virtual long    StringTimeToLongTime (MIDIData* pMIDIData, CString strText, long* pTime); // 20090502�ǉ�

	virtual void    ShowTrackListFrame ();
	virtual void    ShowPianoRollFrame ();
	virtual void    ShowEventListFrame ();
	virtual void    ShowMusicalScoreFrame ();

	virtual MIDIEvent* ReplaceMIDIEvent (MIDIEvent* pMIDIEvent);
	virtual MIDIEvent* DuplicateMIDIEvent (MIDIEvent* pMIDIEvent);
	virtual MIDITrack* ReplaceMIDITrack (MIDITrack* pMIDITrack);
	virtual MIDIData*  ReplaceMIDIData (MIDIData* pMIDIData);

	virtual BOOL AddHistoryUnit (CString m_strName);
	virtual CHistoryUnit* GetCurHistoryUnit ();
	virtual BOOL DeleteAllHistoryUnit ();





	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnUpdateFileSaveUI (CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAsUI (CCmdUI* pCmdUI);
	afx_msg void OnFileProperty ();
	afx_msg void OnUpdateFilePropertyUI (CCmdUI* pCmdUI);
	afx_msg void OnEditUndo ();
	afx_msg void OnUpdateEditUndoUI (CCmdUI* pCmdUI);
	afx_msg void OnEditRedo ();
	afx_msg void OnUpdateEditRedoUI (CCmdUI* pCmdUI);
	afx_msg void OnEditInitHistory ();
	afx_msg void OnUpdateEditInitHistoryUI (CCmdUI* pCmdUI);
	afx_msg void OnEditCut ();
	afx_msg void OnUpdateEditCutUI (CCmdUI* pCmdUI);
	afx_msg void OnEditCopy ();
	afx_msg void OnUpdateEditCopyUI (CCmdUI* pCmdUI);
	afx_msg void OnEditPaste ();
	afx_msg void OnUpdateEditPasteUI (CCmdUI* pCmdUI);
	afx_msg void OnEditDelete ();
	afx_msg void OnUpdateEditDeleteUI (CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll ();
	afx_msg void OnUpdateEditSelectAllUI (CCmdUI* pCmdUI);
	afx_msg void OnEditSelectNone ();
	afx_msg void OnUpdateEditSelectNoneUI (CCmdUI* pCmdUI);
	afx_msg void OnEditSelectBefore ();
	afx_msg void OnUpdateEditSelectBeforeUI (CCmdUI* pCmdUI);
	afx_msg void OnEditDeselectBefore ();
	afx_msg void OnUpdateEditDeselectBeforeUI (CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAfter ();
	afx_msg void OnUpdateEditSelectAfterUI (CCmdUI* pCmdUI);
	afx_msg void OnEditDeselectAfter ();
	afx_msg void OnUpdateEditDeselectAfterUI (CCmdUI* pCmdUI);

	afx_msg void OnEditTrack ();
	afx_msg void OnUpdateEditTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnEditTime ();
	afx_msg void OnUpdateEditTimeUI (CCmdUI* pCmdUI);
	afx_msg void OnEditChannel ();
	afx_msg void OnUpdateEditChannelUI (CCmdUI* pCmdUI);
	afx_msg void OnEditKey ();
	afx_msg void OnUpdateEditKeyUI (CCmdUI* pCmdUI);
	afx_msg void OnEditVelocity ();
	afx_msg void OnUpdateEditVelocityUI (CCmdUI* pCmdUI);
	afx_msg void OnEditDuration ();
	afx_msg void OnUpdateEditDurationUI (CCmdUI* pCmdUI);
	afx_msg void OnEditValue ();
	afx_msg void OnUpdateEditValueUI (CCmdUI* pCmdUI);
	afx_msg void OnEditQuantize ();
	afx_msg void OnUpdateEditQuantizeUI (CCmdUI* pCmdUI);
	afx_msg void OnEditBreakupAndTrill ();
	afx_msg void OnUpdateEditBreakupAndTrillUI (CCmdUI* pCmdUI);
	afx_msg void OnEditBeatScan ();
	afx_msg void OnUpdateEditBeatScanUI (CCmdUI* pCmdUI);
	
	afx_msg void OnEditInsertMeasure (); // 20100728�ǉ�
	afx_msg void OnUpdateEditInsertMeasureUI (CCmdUI* pCmdUI); // 20100728�ǉ�
	afx_msg void OnEditRemoveMeasure (); // 20100728�ǉ�
	afx_msg void OnUpdateEditRemoveMeasureUI (CCmdUI* pCmdUI); // 20100728�ǉ�

	afx_msg void OnViewRedraw ();
	afx_msg void OnViewTrackList ();
	afx_msg void OnViewPianoRoll ();
	afx_msg void OnViewEventList ();
	afx_msg void OnViewMusicalScore ();

	afx_msg void OnPopupShowTrackList ();
	afx_msg void OnUpdatePopupShowTrackListUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupShowPianoRoll ();
	afx_msg void OnUpdatePopupShowPianoRollUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupShowEventList ();
	afx_msg void OnUpdatePopupShowEventListUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupShowMusicalScore ();
	afx_msg void OnUpdatePopupShowMusicalScoreUI (CCmdUI* pCmdUI); // 20100429�ǉ�


	afx_msg void OnPopupTrackInputOn ();
	afx_msg void OnUpdatePopupTrackInputOnUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackInputOff ();
	afx_msg void OnUpdatePopupTrackInputOffUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackInputAll ();
	afx_msg void OnUpdatePopupTrackInputAllUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackOutputOn ();
	afx_msg void OnUpdatePopupTrackOutputOnUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackOutputOff ();
	afx_msg void OnUpdatePopupTrackOutputOffUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackOutputAll ();
	afx_msg void OnUpdatePopupTrackOutputAllUI (CCmdUI* pCmdUI);

	afx_msg void OnPopupCut ();
	afx_msg void OnUpdatePopupCutUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupCopy ();
	afx_msg void OnUpdatePopupCopyUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupPaste ();
	afx_msg void OnUpdatePopupPasteUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP ()

};

#endif
