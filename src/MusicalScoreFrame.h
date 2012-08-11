//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃt���[���E�B���h�E�N���X
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



// �������(�ЂƂ̂����܂��Ⴍ���ɂ��ЂƂm��)
typedef struct tagMusicalScoreNoteInfo {
	MIDIEvent* m_pNoteOnEvent;          // �m�[�g�I���C�x���g�ւ̃|�C���^
	MIDIEvent* m_pNoteOffEvent;         // �m�[�g�I�t�C�x���g�ւ̃|�C���^
	long m_lNoteOnTime;                 // �m�[�g�I������[Tick]
	long m_lNoteOnMeasure;              // �m�[�g�I������
	long m_lNoteOnBeat;                 // �m�[�g�I����
	long m_lNoteOnTick;                 // �m�[�g�I���e�B�b�N
	long m_lNoteOffTime;                // �m�[�g�I�t����[Tick]
	long m_lNoteOffMeasure;             // �m�[�g�I�t����
	long m_lNoteOffBeat;                // �m�[�g�I�t��
	long m_lNoteOffTick;                // �m�[�g�I�t�e�B�b�N
	long m_lFlags;                      // �t���O
	long m_lSelected;                   // �I�����
	struct tagMusicalScoreNoteInfo* m_pPrevNoteInfo; // ���̉������ւ̃|�C���^(�Ȃ����NULL)
	struct tagMusicalScoreNoteInfo* m_pNextNoteInfo; // �O�̉������ւ̃|�C���^(�Ȃ����NULL)
	void* m_pNoteGroupInfo;             // ���O���[�v���ւ̃|�C���^(�Ȃ����NULL)
	void* m_pTripletGroupInfo;          // 3�A���O���[�v���ւ̃|�C���^(�Ȃ����NULL)
} MusicalScoreNoteInfo;

// ���O���[�v���(�����̉����������Ȃ��đ��˂���)
typedef struct tagMusicalScoreNoteGroupInfo {
	MusicalScoreNoteInfo* m_pFirstNoteInfo; // �ŏ��̉������ւ̃|�C���^
	MusicalScoreNoteInfo* m_pLastNoteInfo;  // �Ō�̉������ւ̃|�C���^
	long m_lBeginTime;                  // �J�n����
	long m_lEndTime;                    // �I������
	long m_lMinKey;                     // �Œቹ�K(0-127)
	long m_lMaxKey;                     // �ō����K(0-127)
	long m_lMinDur;                     // �ŒZ������
	long m_lMaxDur;                     // �Œ�������
	long m_lNumNoteInfo;                // �O���[�v���̉�����
} MusicalScoreNoteGroupInfo;

// 3�A���O���[�v���(�����̉�����-3-�łȂ��đ��˂���)
typedef struct tagMusicalScoreTripletGroupInfo {
	MusicalScoreNoteInfo* m_pFirstNoteInfo; // �ŏ��̉������ւ̃|�C���^
	MusicalScoreNoteInfo* m_pLastNoteInfo;  // �Ō�̉������ւ̃|�C���^
	long m_lBeginTime;                  // �J�n����
	long m_lEndTime;                    // �I������
	long m_lMinKey;                     // �Œቹ�K(0-127)
	long m_lMaxKey;                     // �ō����K(0-127)
	long m_lMinDur;                     // �ŒZ������
	long m_lMaxDur;                     // �Œ�������
	long m_lNumNoteInfo;                // �O���[�v���̉�����
} MusicalScoreTripletGroupInfo;

// �g���b�N���
typedef struct {
	long m_lTop;                        // ��[���W[pixel]
	long m_lHeight;                     // ����[pixel]
	long m_lFlags;                      // �t���O
	long m_lMinKey;                     // �g���b�N���Œቹ�K(0-127)
	long m_lMaxKey;	                    // �g���b�N���ō����K(0-127)
	CPtrArray m_theNoteInfoArray;       // �������̔z��
	CPtrArray m_theNoteGroupInfoArray;  // �����O���[�v���̔z��
	CPtrArray m_theTripletGroupInfoArray; // 3�A�����O���[�v���̔z��
	long m_lTopPrint;                   // ��[���W(����p)[*1/10mm]
	long m_lHeightPrint;                // ����(����p)[*1/10mm]
} MusicalScoreTrackInfo;

// ���ߏ��
typedef struct {
	long m_lTime;                       // ���̏��߂̊J�n�^�C��[Tick][Subframe]
	long m_lDuration;                   // ���̏��߂ɂ̒���[Tick][Subframe]
	long m_lLeft;                       // ���̏��߂̍����W[pixel]
	long m_lSignatureWidth;             // ���̏��߂̔��q�L���E�����L���p��[pixel]
	long m_lPreWidth;                   // ���̏��߂̍��]����[pixel]
	long m_lWidth;                      // ���̏��߂̕�[pixel]
	long m_lPostWidth;                  // ���̏��߂̉E�]����[pixel]
	long m_lFlags;                      // �t���O
	long m_lTimeSignature;              // ���̏��߂̔��q�L��
	long m_lKeySignature;               // ���̏��߂̒����L��
	long m_lLeftPrint;                  // ���̏��߂̍����W(����p)[*1/10mm]
	long m_lSignatureWidthPrint;        // ���̏��߂̔��q�L���E�����L���p��(����p)[*1/10mm]
	long m_lPreWidthPrint;              // ���̏��߂̍��]����(����p)[*1/10mm]
	long m_lWidthPrint;                 // ���̏��߂̕�(����p)[*1/10mm] 
	long m_lPostWidthPrint;             // ���̏��߂̉E�]����[pixel](����p)[*1/10mm]
} MusicalScoreMeasureInfo;




class CMusicalScoreFrame : public CChildFrame {
	DECLARE_DYNCREATE (CMusicalScoreFrame)

	//-------------------------------------------------------------------------
	// �A�g���r���[�g
	//-------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // �c�[���o�[1�̍���[pixel]
	long m_lKeyHeight;                  // �g���b�N�ԍ�-���ԃr���[�̍���[pixel]
	long m_lScaleHeight;                // �㕔�ڐ���r���[�̍���[pixel]
	long m_lScaleWidth;                 // �����ڐ���r���[�̍���[pixel]
	long m_lTimeWidth;                  // �g���b�N�ԍ��[-���ԃr���[�̕�[pixel]
	long m_lHScrollBarHeight;           // �����X�N���[���o�[�̍���[pixel]
	long m_lVScrollBarWidth;            // �����X�N���[���o�[�̍���[pixel]
	long m_lTrackListWidth;             // �g���b�N���X�g�{�b�N�X�̕�[pixel]
	long m_lTrackZoom;                  // �g���b�N�����g��{��[�{]
	long m_lTimeZoom;                   // ���ԕ����g��{��[�{]
	long m_lTrackScrollPos;             // �g���b�N�����X�N���[���ʒu[pixel]
	long m_lTimeScrollPos;              // ���ԕ����X�N���[���ʒu[pixel]
public:
	// �`��⏕���
	CPtrArray   m_theTrackInfoArray;    // �g���b�N���Ƃ̏��
	CPtrArray   m_theMeasureInfoArray;  // ���߂��Ƃ̏��

public:
	CFont m_theFont;                    // ���ʃE�B���h�E�Ŏg���t�H���g
	CFont m_theTimeMeasureFont;         // ���q�L���E�����L���p�̃t�H���g
	CSekaijuToolBar m_wndToolBar1;      // �c�[���o�[1(��)
	CColorfulComboBox m_wndTrackCombo;  // �c�[���o�[��̃g���b�N�R���{�{�b�N�X
	CComboBox m_wndChannelCombo;        // �c�[���o�[��̃`�����l���R���{�{�b�N�X
	CComboBox m_wndSnapCombo;           // �c�[���o�[��̃X�i�b�v�R���{�{�b�N�X
	CComboBox m_wndVelocityCombo;       // �c�[���o�[��̃x���V�e�B�R���{�{�b�N�X
	CComboBox m_wndDurationCombo;       // �c�[���o�[��̉������R���{�{�b�N�X
	CComboBox m_wndResolutionCombo;     // �c�[���o�[��̕\�����x�R���{�{�b�N�X
	CView* m_pDummyView;                // �_�~�[�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pPrintView;                // ����p�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pScaleView;                // �ڐ���r���[�ւ̃|�C���^
	CView* m_pTimeScaleView;            // ���Ԗڐ���r���[�ւ̃|�C���^
	CView* m_pTrackScaleView;           // �g���b�N�ԍ��ڐ���r���[�ւ̃|�C���^
	CView* m_pTrackTimeView;            // �g���b�N�ԍ�-�^�C���r���[�ւ̃|�C���^
	CColorfulCheckListBox m_wndTrackList;  // �g���b�N���X�g�R���{�{�b�N�X
	CScrollBar m_wndTimeScroll;         // ���ԕ����X�N���[���o�[
	CScrollBar m_wndTrackScroll;        // �g���b�N�����X�N���[���o�[
	CScrollBar m_wndSizeScroll;         // �T�C�Y�p�X�N���[���o�[
	CBitmapButton m_wndTimeZoomUp;      // ���ԕ����g��{�^��
	CBitmapButton m_wndTimeZoomDown;    // ���ԕ����k���{�^��
	CBitmapButton m_wndTrackZoomUp;     // �g���b�N�����g��{�^��
	CBitmapButton m_wndTrackZoomDown;   // �g���b�N�����k���{�^��

protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W(�X�v���b�^�[�𓮂����̂Ɏg��)
	CPoint m_ptMouseMoveOld;            // �}�E�X���������ꂽ�Ƃ��̑O��̍��W(�X�v���b�^�[�𓮂����̂Ɏg��)
	BOOL m_bSplitterMovingH;            // ���ݐ����X�v���b�^�[�𓮂����Ă���Ƃ��납�H
	BOOL m_bSplitterMovingV;            // ���ݐ����X�v���b�^�[�𓮂����Ă���Ƃ��납�H

public:
	BOOL m_bAutoPageUpdate;             // �����I�Ƀy�[�W���X�V���邩�H
	long m_lCurTool;                    // ���݂̑I������Ă���c�[��(0=�`��,1=����,2=�����S��,3=�I��,4=����)

protected:
	BOOL m_bOnlyCurTrack;               // ���݂̃g���b�N�݂̂�\�����邩�H
	BOOL m_bShowAllTrack;               // ���ׂẴg���b�N��\�����邩�H
	BOOL m_bTrackVisible[MAXMIDITRACKNUM]; // �g���b�N[0�`65535]�͉����H

	//-------------------------------------------------------------------------
	// �\�z�Ɣj��
	//-------------------------------------------------------------------------
public:
	CMusicalScoreFrame ();              // �R���X�g���N�^
	virtual ~CMusicalScoreFrame ();     // �f�X�g���N�^

	//-------------------------------------------------------------------------
	// �I�y���[�V����
	//-------------------------------------------------------------------------
public:
	virtual CSekaijuDoc* GetDocument ();
	virtual long GetTimeZoom ();
	virtual long GetTrackZoom ();
	virtual long XtoTime (long x);
	virtual long TimetoX (long lTime);
	virtual long MeasuretoX (long lMeasure);
	virtual long MeasuretoX2 (long lMeasure);
	virtual long YtoTrackIndex (long y);
	virtual long TrackIndexYtoKey (long lTrackIndex, long y, long lKeySignature);
	virtual long KeytoLineNo (long lKey, long lKeySignature);
	virtual long KeytoSF (long lKey, long lKeySignature);
	virtual long TrackIndexLineNotoY (long lTrackIndex, long lLineNo);
	virtual long TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature);
	virtual long GetTimeScrollPos ();
	virtual long GetTrackScrollPos ();
	virtual long SetTimeScrollPos (long lTimeScrollPos);
	virtual long SetTrackScrollPos (long lTrackScrollPos);
	virtual long GetVisibleLeftTime ();
	virtual long GetVisibleRightTime ();
	virtual long GetVisibleTopTrack ();
	virtual long GetVisibleBottomTrack ();
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual long GetCurTrackIndex ();
	virtual long GetCurChannel ();
	virtual long GetCurSnap ();
	virtual long GetCurVelocity ();
	virtual long GetCurDuration ();
	virtual long GetCurResolution ();
	virtual BOOL SetCurTrackIndex (long lCurTrackIndex);
	virtual BOOL SetCurChannel (long lCurChannel);
	virtual BOOL SetCurSnap (long lCurSnap);
	virtual BOOL SetCurVelocity (long lCurVelocity);
	virtual BOOL SetCurDuration (long lCurDuration);
	virtual BOOL SetCurResolution (long lCurResolution);
	virtual BOOL IsTrackVisible (long lTrackIndex);
	virtual BOOL SetTrackVisible (long lTrackIndex);
	virtual BOOL UpdateTrackCombo ();
	virtual BOOL UpdateTrackList ();
	virtual BOOL UpdateSnapCombo ();
	virtual BOOL UpdateVelocityCombo ();
	virtual BOOL UpdateDurationCombo ();
	virtual BOOL UpdateResolutionCombo ();
	virtual void RecalcTrackScrollInfo ();
	virtual void RecalcTimeScrollInfo ();


	virtual BOOL DeleteTrackInfoArray ();
	virtual BOOL DeleteMeasureInfoArray ();
	virtual BOOL DeleteNoteInfoArray (long lTrackIndex);
	virtual BOOL DeleteNoteGroupInfoArray (long lTrackIndex);
	virtual BOOL DeleteTripletGroupInfoArray (long lTrackIndex);
	virtual BOOL UpdateMeasureInfoArray ();
	virtual BOOL UpdateTrackInfoArray ();
	virtual BOOL UpdateNoteInfoArray (long lTrackIndex, MIDITrack* pMIDITrack);
	virtual BOOL UpdateNoteGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack);
	virtual BOOL UpdateTripletGroupInfoArray (long lTrackIndex, MIDITrack* pMIDITrack);
	virtual MusicalScoreNoteInfo* CreateNote
		(MIDIEvent* pNoteEvent, long lNoteOnTime, long lNoteOnMeasure, long lNoteOnBeat, long lNoteOnTick, 
		long lNoteOffTime, long lNoteOffMeasure, long lNoteOffBeat, long lNoteOffTick, long lFlags);
	virtual void DeleteNoteInfo (MusicalScoreNoteInfo* pNoteInfo);
	virtual BOOL AddNoteInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteInfo* pNoteInfo);
	MusicalScoreNoteGroupInfo* CreateNoteGroupInfo (MusicalScoreNoteInfo* pNoteInfo);
	void DeleteNoteGroupInfo (MusicalScoreNoteGroupInfo* pNoteGroupInfo);
	BOOL AddNoteGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreNoteGroupInfo* pNoteGroupInfo);
	MusicalScoreTripletGroupInfo* CreateTripletGroupInfo (MusicalScoreNoteInfo* pNoteInfo);
	void DeleteTripletGroupInfo (MusicalScoreTripletGroupInfo* pTripletGroupInfo);
	BOOL AddTripletGroupInfo (MusicalScoreTrackInfo* pTrackInfo, MusicalScoreTripletGroupInfo* pNoteGroupInfo);

	virtual long GetTrackInfoCount ()
		{return m_theTrackInfoArray.GetSize ();} 
	virtual MusicalScoreTrackInfo* GetTrackInfo (long lTrackIndex)
		{return lTrackIndex < m_theTrackInfoArray.GetSize () ? 
		((MusicalScoreTrackInfo*)m_theTrackInfoArray.GetAt (lTrackIndex)) : NULL;}

	virtual long GetMeasureInfoCount ()
		{return m_theMeasureInfoArray.GetSize ();}
	virtual MusicalScoreMeasureInfo* GetMeasureInfo (long lMeasureIndex)
		{return lMeasureIndex < m_theMeasureInfoArray.GetSize () ?
		((MusicalScoreMeasureInfo*)m_theMeasureInfoArray.GetAt (lMeasureIndex)) : NULL;}
	virtual long GetMeasureTime (long lMeasureIndex)
		{return lMeasureIndex < m_theMeasureInfoArray.GetSize () ?
		((MusicalScoreMeasureInfo*)m_theMeasureInfoArray.GetAt (lMeasureIndex))->m_lTime : 0;}
	virtual long GetMeasureLeft (long lMeasureIndex)
		{return lMeasureIndex < m_theMeasureInfoArray.GetSize () ?
		((MusicalScoreMeasureInfo*)m_theMeasureInfoArray.GetAt (lMeasureIndex))->m_lLeft : 0;}


	//-------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//-------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle (BOOL bAddToTitle);
	virtual void RecalcLayout (BOOL bNotify = TRUE);
	virtual BOOL OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	//-------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//-------------------------------------------------------------------------
protected:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy ();
	afx_msg void OnSize (UINT nType, int cx, int cy);
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);

	afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnPaint ();
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnTimeZoomDown ();
	afx_msg void OnTimeZoomUp ();
	afx_msg void OnTrackZoomDown ();
	afx_msg void OnTrackZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMusicalScorePen ();
	afx_msg void OnUpdateMusicalScorePenUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreLine ();
	afx_msg void OnUpdateMusicalScoreLineUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreEraser ();
	afx_msg void OnUpdateMusicalScoreEraserUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreSelect ();
	afx_msg void OnUpdateMusicalScoreSelectUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreSpeaker ();
	afx_msg void OnUpdateMusicalScoreSpeakerUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreWholeNote ();
	afx_msg void OnUpdateMusicalScoreWholeNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreHalfNote ();
	afx_msg void OnUpdateMusicalScoreHalfNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreQuarterNote ();
	afx_msg void OnUpdateMusicalScoreQuarterNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreQuaverNote ();
	afx_msg void OnUpdateMusicalScoreQuaverNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreSemiQuaverNote ();
	afx_msg void OnUpdateMusicalScoreSemiQuaverNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreDemiSemiQuaverNote ();
	afx_msg void OnUpdateMusicalScoreDemiSemiQuaverNoteUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreDotted ();
	afx_msg void OnUpdateMusicalScoreDottedUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreTriplet ();
	afx_msg void OnUpdateMusicalScoreTripletUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreOnlyCurTrack ();
	afx_msg void OnUpdateMusicalScoreOnlyCurTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreShowAllTrack ();
	afx_msg void OnUpdateMusicalScoreShowAllTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnMusicalScoreAutoPageUpdate ();
	afx_msg void OnUpdateMusicalScoreAutoPageUpdateUI (CCmdUI* pCmdUI);

	afx_msg void OnTrackComboSelEndOK ();	
	afx_msg void OnTrackListChkChange ();
	afx_msg void OnTrackListSelChange ();
	afx_msg void OnSnapComboSelEndOK ();
	afx_msg void OnResolutionComboSelEndOK ();

	afx_msg void OnPopupTrackVisibleOn ();
	afx_msg void OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleOff ();
	afx_msg void OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleAll ();
	afx_msg void OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupEventProperty ();
	afx_msg void OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


