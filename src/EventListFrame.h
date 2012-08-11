//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�t���[���E�B���h�E�N���X
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

#ifndef _EVENTLISTFRAME_H_
#define _EVENTLISTFRAME_H_

class CEventListFrame : public CChildFrame {
	DECLARE_DYNCREATE (CEventListFrame)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // �c�[���o�[�̍���[pixel]
	long m_lScaleHeight;                // ���ږڐ���r���[�̍���[pixel]
	long m_lIndexHeight;                // �C�x���g�ԍ�-���ڃr���[�̍���[pixel]
	long m_lScaleWidth;                 // �C�x���g�ԍ��ڐ���r���[�̕�[pixel]
	long m_lPropertyWidth;              // �C�x���g�ԍ�-���ڃr���[�̕�[pixel]
	long m_lTrackListWidth;             // �g���b�N���X�g�{�b�N�X�̕�[pixel]
	long m_lTrackListHeight;            // �g���b�N���X�g�{�b�N�X�̍���[pixel]
	long m_lEventKindListHeight;        // �C�x���g�̎�ރ��X�g�{�b�N�X�̍���[pixel]
	long m_lHScrollBarHeight;           // �����X�N���[���o�[�̍���[pixel]
	long m_lVScrollBarWidth;            // �����X�N���[���o�[�̕�[pixel]
	long m_lRowZoom;                    // �s�����Y�[���{��[�{]
	long m_lColumnZoom;                 // ������Y�[���{��[�{]
	long m_lRowScrollPos;               // �s�����X�N���[���ʒu[pixel]
	long m_lColumnScrollPos;            // ������X�N���[���ʒu[pixel]

public:
	CSekaijuToolBar m_wndToolBar1;      // �c�[���o�[
	CColorfulComboBox m_wndEventTrackCombo; // �c�[���o�[��̃C�x���g�̃g���b�N�ԍ��R���{�{�b�N�X
	CEdit m_wndEventTimeEdit;           // �c�[���o�[��̃C�x���g�̃^�C���G�f�B�b�g�{�b�N�X
	CComboBox m_wndEventKindCombo;      // �c�[���o�[��̃C�x���g�̎�ރR���{�{�b�N�X
	CComboBox m_wndEventChannelCombo;   // �c�[���o�[��̃C�x���g�̃`�����l���R���{�{�b�N�X
	CView* m_pDummyView;                // �_�~�[�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pPrintView;                // ����p�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pScaleView;                // �ڐ���r���[�ւ̃|�C���^
	CView* m_pPropertyScaleView;        // ���ږڐ���r���[�ւ̃|�C���^
	CView* m_pIndexScaleView;           // �C�x���g�ԍ��ڐ���r���[�ւ̃|�C���^
	CView* m_pIndexPropertyView;        // �C�x���g�ԍ�-���ڃr���[�ւ̃|�C���^
	CColorfulCheckListBox m_wndTrackList; // �g���b�N���X�g�{�b�N�X
	CCheckListBox m_wndEventKindList;   // �C�x���g�̎�ރ��X�g�{�b�N�X
	CScrollBar m_wndColumnScroll;       // ������X�N���[���o�[
	CScrollBar m_wndRowScroll;          // �s�����X�N���[���o�[
	CScrollBar m_wndSizeScroll;         // �T�C�Y�X�N���[���o�[
	CBitmapButton m_wndColumnZoomUp;    // ������g��{�^��
	CBitmapButton m_wndColumnZoomDown;  // ������k���{�^��
	CBitmapButton m_wndRowZoomUp;       // �s�����g��{�^��
	CBitmapButton m_wndRowZoomDown;     // �s�����k���{�^��

public:
	BOOL m_bAutoPageUpdate;             // �����I�Ƀy�[�W���X�V���邩�H

protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W(�X�v���b�^�[�𓮂����̂Ɏg��)
	CPoint m_ptMouseMoveOld;            // �}�E�X���������ꂽ�Ƃ��̑O��̍��W(�X�v���b�^�[�𓮂����̂Ɏg��)
	BOOL m_bSplitterMovingH;            // ���ݐ����X�v���b�^�[�𓮂����Ă���Ƃ��납�H
	BOOL m_bSplitterMovingV;            // ���ݐ����X�v���b�^�[�𓮂����Ă���Ƃ��납�H
	CPtrArray m_theVisibleEventArray;   // �\��������C�x���g�ւ̃|�C���^�̔z��
	CFont m_theFont;                    // �C�x���g���X�g�E�B���h�E�Ŏg���t�H���g
	long m_lColumnBaseWidth[8];         // �e��̕�[pixel]
	CString m_strColumnTitle[8];        // �e��̃^�C�g��������(��F"�g���b�N", "��:��:�b:�~���b"...)

protected:
	BOOL m_bOnlyCurTrack;               // ���݂̃g���b�N�݂̂�\�����邩�H
	BOOL m_bShowAllTrack;               // ���ׂẴg���b�N��\�����邩�H
	BOOL m_bTrackVisible[MAXMIDITRACKNUM]; // �e�X�̃g���b�N[0�`65525]�͉����H
	BOOL m_bOnlyCurEventKind;           // ���݂̃C�x���g�̎�ނ݂̂�\�����邩�H
	BOOL m_bShowAllEventKind;           // ���ׂẴC�x���g�̎�ނ�\�����邩�H
	BOOL m_bEventKindVisible[256];      // �e�X�̃C�x���g�̎��[0�`255]�͉����H

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEventListFrame ();                 // �R���X�g���N�^
	virtual ~CEventListFrame ();        // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	virtual CSekaijuDoc* GetDocument ();
	virtual CFont* GetParentFont ();
	virtual long GetVisibleTopRow ();
	virtual long GetVisibleBottomRow ();
	virtual long GetColumnBaseWidth (long lColumn);
	virtual long SetColumnBaseWidth (long lColumn, long lBaseWidth);
	virtual CString GetColumnTitle (long lColumn);
	virtual long SetColumnTitle (long lColumn, const TCHAR* pszColumnTitle);
	virtual long GetColumnLeft (long lColumn);
	virtual long GetColumnWidth (long lColumn);
	virtual long XtoColumn (long x);
	virtual long ColumntoX (long lColumn);
	virtual long YtoRow (long y);
	virtual long RowtoY (long lRow);
	virtual long GetColumnZoom ();
	virtual long GetRowZoom ();
	virtual long GetColumnScrollPos ();
	virtual long GetRowScrollPos ();
	virtual long SetColumnScrollPos (long lColumnScrollPos);
	virtual long SetRowScrollPos (long lRowScrollPos);
	virtual long GetVisibleEventCount ();
	virtual MIDIEvent* GetVisibleEvent (long lIndex);
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual BOOL MakeVisibleEventArray ();
	virtual BOOL ReplaceVisibleEvent (MIDIEvent* pOldEvent, MIDIEvent* pNewEvent);
	virtual long GetCurTrackIndex ();
	virtual long GetCurEventKind ();
	virtual BOOL SetCurTrackIndex (long lCurTrackIndex);
	virtual BOOL SetCurEventKind (long lCurEventKind);
	virtual BOOL SetCurChannel (long lCurChannel);
	virtual BOOL IsTrackVisible (long lTrackIndex);
	virtual BOOL SetTrackVisible (long lTrackIndex);
	virtual BOOL IsEventKindVisible (long lEventKind);
	virtual BOOL SetEventKindVisible (long lEventKind);
	virtual BOOL UpdateTrackCombo ();
	virtual BOOL UpdateTrackList ();
	virtual long EventKindtoListIndex (long lEventKind);
	virtual long ListIndextoEventKind (long lListIndex);
	virtual void RecalcRowScrollInfo ();
	virtual void RecalcColumnScrollInfo ();

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
	afx_msg void OnClose ();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnColumnZoomDown ();
	afx_msg void OnColumnZoomUp ();
	afx_msg void OnRowZoomDown ();
	afx_msg void OnRowZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	afx_msg void OnEventListInsertEvent ();
	afx_msg void OnUpdateEventListInsertEventUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListDuplicateEvent ();
	afx_msg void OnUpdateEventListDuplicateEventUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListDeleteEvent ();
	afx_msg void OnUpdateEventListDeleteEventUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListOnlyCurTrack ();
	afx_msg void OnUpdateEventListOnlyCurTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListShowAllTrack ();
	afx_msg void OnUpdateEventListShowAllTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListOnlyCurEventKind ();
	afx_msg void OnUpdateEventListOnlyCurEventKindUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListShowAllEventKind ();
	afx_msg void OnUpdateEventListShowAllEventKindUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListAutoPageUpdate ();
	afx_msg void OnUpdateEventListAutoPageUpdateUI (CCmdUI* pCmdUI);
	afx_msg void OnEventListSaveAs ();
	afx_msg void OnUpdateEventListSaveAsUI (CCmdUI* pCmdUI);

	afx_msg void OnTrackComboSelEndOK ();
	afx_msg void OnTrackListChkChange ();
	afx_msg void OnTrackListSelChange ();

	afx_msg void OnEventKindComboSelEndOK ();	
	afx_msg void OnEventKindListChkChange ();
	afx_msg void OnEventKindListSelChange ();

	afx_msg void OnPopupTrackVisibleOn ();
	afx_msg void OnUpdatePopupTrackVisibleOnUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupTrackVisibleOff ();
	afx_msg void OnUpdatePopupTrackVisibleOffUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupTrackVisibleAll ();
	afx_msg void OnUpdatePopupTrackVisibleAllUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupEventKindVisibleOn ();
	afx_msg void OnUpdatePopupEventKindVisibleOnUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupEventKindVisibleOff ();
	afx_msg void OnUpdatePopupEventKindVisibleOffUI (CCmdUI* pCmdUI); // 20100429�ǉ�
	afx_msg void OnPopupEventKindVisibleAll ();
	afx_msg void OnUpdatePopupEventKindVisibleAllUI (CCmdUI* pCmdUI); // 20100429�ǉ�

	DECLARE_MESSAGE_MAP()
};

#endif

