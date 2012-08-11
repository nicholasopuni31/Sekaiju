//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �s�A�m���[���t���[���E�B���h�E�N���X
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

#ifndef _PIANOROLLFRAME_H_
#define _PIANOROLLFRAME_H_

class CPianoRollFrame : public CChildFrame {
	DECLARE_DYNCREATE (CPianoRollFrame)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // �c�[���o�[1�̍���[pixel]
	long m_lKeyHeight;                  // �L�[�r���[(��y�C��)�̍���[pixel]
	long m_lScaleHeight;                // �㕔�ڐ���r���[�̍���[pixel]
	long m_lVelHeight;                  // �x���V�e�B�r���[(���y�C��)�̍���[pixel]
	long m_lScaleWidth;                 // �����ڐ���r���[�̕�[pixel]
	long m_lTimeWidth;                  // ���ԃr���[(���y�C��)�̕�[pixel]
	long m_lHScrollBarHeight;           // �����X�N���[���o�[�̍���[pixel]
	long m_lVScrollBarWidth;            // �����X�N���[���o�[�̕�[pixel]
	long m_lTrackListWidth;             // �g���b�N���X�g(�E�y�C��)�̕�[pixel]
	long m_lKeyZoom;                    // �L�[�����g��{��[�{]
	long m_lTimeZoom;                   // �^�C�������g��{��[�{]
	long m_lVelZoom;                    // �x���V�e�B�����Y�[���{��[�{]
	long m_lKeyScrollPos;               // �L�[�����X�N���[���ʒu[pixel]
	long m_lVelScrollPos;               // �x���V�e�B�����X�N���[���ʒu[pixel]
	long m_lTimeScrollPos;              // ���ԕ����X�N���[���ʒu[pixel]
public:
	CFont m_theFont;                    // �s�A�m���[���E�B���h�E�Ŏg���t�H���g
public:
	CSekaijuToolBar m_wndToolBar1;      // �c�[���o�[1
	CColorfulComboBox m_wndTrackCombo;  // �g���b�N�R���{�{�b�N�X
	CComboBox m_wndChannelCombo;        // �`�����l���R���{�{�b�N�X
	CComboBox m_wndSnapCombo;           // �X�i�b�v�R���{�{�b�N�X
	CComboBox m_wndVelocityCombo;       // �x���V�e�B�R���{�{�b�N�X
	CComboBox m_wndDurationCombo;       // �������R���{�{�b�N�X
	CComboBox m_wndGraphKindCombo;      // �O���t�̎�ރR���{�{�b�N�X
	CComboBox m_wndGraphSnapCombo;      // �O���t�X�i�b�v�R���{�{�b�N�X
	CView* m_pDummyView;                // �_�~�[�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pPrintView;                // ����p�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pScaleView;                // �ڐ���r���[�ւ̃|�C���^
	CView* m_pTimeScaleView;            // ���Ԗڐ���r���[�ւ̃|�C���^
	CView* m_pKeyScaleView;             // ���K�ڐ���r���[�ւ̃|�C���^
	CView* m_pKeyTimeView;              // ���K-���ԃr���[�ւ̃|�C���^
	CView* m_pVelScaleView;             // �x���V�e�B�ڐ���r���[�ւ̃|�C���^
	CView* m_pVelTimeView;              // �x���V�e�B-���ԃr���[�ւ̃|�C���^
	CColorfulCheckListBox m_wndTrackList; // �g���b�N���X�g�{�b�N�X
	CCheckListBox m_wndGraphKindList;   // �O���t�̎�ރ��X�g�{�b�N�X
	CScrollBar m_wndTimeScroll;         // ���ԕ����X�N���[���o�[
	CScrollBar m_wndKeyScroll;          // ���K�����X�N���[���o�[
	CScrollBar m_wndVelScroll;          // �x���V�e�B�����X�N���[���o�[
	CScrollBar m_wndSizeScroll;         // �T�C�Y�ύX�X�N���[���o�[
	CBitmapButton m_wndTimeZoomUp;      // ���ԕ����g��{�^��
	CBitmapButton m_wndTimeZoomDown;    // ���ԕ����k���{�^��
	CBitmapButton m_wndVelZoomUp;       // �x���V�e�B�����g��{�^��
	CBitmapButton m_wndVelZoomDown;     // �x���V�e�B�����k���{�^��
	CBitmapButton m_wndKeyZoomUp;       // �L�[�����g��{�^��
	CBitmapButton m_wndKeyZoomDown;     // �L�[�����k���{�^��
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
	BOOL m_bOnlyCurGraph;               // ���݂̃O���t�݂̂�\�����邩�H
	BOOL m_bShowAllGraph;               // ���ׂẴO���t��\�����邩�H
	BOOL m_bGraphVisible[256];          // �O���t[0�`255]�͉����H

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CPianoRollFrame ();                 // �R���X�g���N�^
	virtual ~CPianoRollFrame();         // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	virtual CSekaijuDoc* GetDocument ();
	virtual long GetTimeZoom ();
	virtual long GetKeyZoom ();
	virtual long GetVelZoom ();
	virtual long XtoTime (long x);
	virtual long TimetoX (long lTime);
	virtual long YtoKey (long y);
	virtual long KeytoY (long lKey);
	virtual long YtoPitchBend (long y);
	virtual long YtoVel (long y);
	virtual long YtoTempoBPM (long y);
	virtual long VeltoY (long lVel);
	virtual long PitchBendtoY (long lPitchBend);
	virtual long TempoBPMtoY (long lTempo);
	virtual long GetTimeScrollPos ();
	virtual long GetKeyScrollPos ();
	virtual long GetVelScrollPos ();
	virtual long SetTimeScrollPos (long lTimeScrollPos);
	virtual long SetKeyScrollPos (long lKeyScrollPos);
	virtual long SetVelScrollPos (long lVelScrollPos);
	virtual long GetVisibleLeftTime ();
	virtual long GetVisibleRightTime ();
	virtual long GetVisibleTopKey ();
	virtual long GetVisibleBottomKey ();
	virtual long GetVisibleTopVel ();
	virtual long GetVisibleBottomVel ();
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual long GetCurTrackIndex ();
	virtual long GetCurChannel ();
	virtual long GetCurSnap ();
	virtual long GetCurVelocity ();
	virtual long GetCurDuration ();
	virtual long GetCurGraphKind ();
	virtual long GetCurGraphSnap ();
	virtual BOOL SetCurTrackIndex (long lCurTrackIndex);
	virtual BOOL SetCurChannel (long lCurChannel);
	virtual BOOL SetCurSnap (long lCurSnap);
	virtual BOOL SetCurVelocity (long lCurVelocity);
	virtual BOOL SetCurDuration (long lCurDuration);
	virtual BOOL SetCurGraphKind (long lCurGraphKind);
	virtual BOOL SetCurGraphSnap (long lCurGraphKind);
	virtual BOOL IsTrackVisible (long lTrackIndex);
	virtual BOOL SetTrackVisible (long lTrackIndex);
	virtual BOOL IsGraphVisible (long lGraphKind);
	virtual BOOL SetGraphVisible (long lGraphKind);
	virtual BOOL UpdateTrackCombo ();
	virtual BOOL UpdateGraphKindCombo ();
	virtual BOOL UpdateTrackList ();
	virtual BOOL UpdateGraphKindList ();
	virtual BOOL UpdateSnapCombo ();
	virtual BOOL UpdateVelocityCombo ();
	virtual BOOL UpdateDurationCombo ();
	virtual BOOL UpdateGraphSnapCombo ();
	virtual void RecalcKeyScrollInfo ();
	virtual void RecalcVelScrollInfo ();
	virtual void RecalcTimeScrollInfo ();


	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle (BOOL bAddToTitle);
	virtual void RecalcLayout (BOOL bNotify = TRUE);
	virtual BOOL OnCreateClient (LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
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
	afx_msg void OnKeyZoomDown ();
	afx_msg void OnKeyZoomUp ();
	afx_msg void OnVelZoomDown ();
	afx_msg void OnVelZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPianoRollPen ();
	afx_msg void OnUpdatePianoRollPenUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollLine ();
	afx_msg void OnUpdatePianoRollLineUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollEraser ();
	afx_msg void OnUpdatePianoRollEraserUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollSelect ();
	afx_msg void OnUpdatePianoRollSelectUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollSpeaker ();
	afx_msg void OnUpdatePianoRollSpeakerUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollOnlyCurTrack ();
	afx_msg void OnUpdatePianoRollOnlyCurTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollShowAllTrack ();
	afx_msg void OnUpdatePianoRollShowAllTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollOnlyCurGraph ();
	afx_msg void OnUpdatePianoRollOnlyCurGraphUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollShowAllGraph ();
	afx_msg void OnUpdatePianoRollShowAllGraphUI (CCmdUI* pCmdUI);
	afx_msg void OnPianoRollAutoPageUpdate ();
	afx_msg void OnUpdatePianoRollAutoPageUpdateUI (CCmdUI* pCmdUI);

	afx_msg void OnTrackComboSelEndOK ();	
	afx_msg void OnTrackListChkChange ();
	afx_msg void OnTrackListSelChange ();
	afx_msg void OnGraphKindComboSelEndOK ();	
	afx_msg void OnGraphKindListChkChange ();
	afx_msg void OnGraphKindListSelChange ();
	afx_msg void OnPopupEventProperty (); // 20100501�ǉ�
	afx_msg void OnUpdatePopupEventPropertyUI (CCmdUI* pCmdUI); // 20100501�ǉ�

	afx_msg void OnPopupTrackVisibleOn ();
	afx_msg void OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleOff ();
	afx_msg void OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI);
	afx_msg void OnPopupTrackVisibleAll ();
	afx_msg void OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


#endif

