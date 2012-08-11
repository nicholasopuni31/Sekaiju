//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�t���[���E�B���h�E�N���X
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

#define TRACKLISTFRAME_NUMCOLUMN 21

#define TRACKLISTFRAME_TRACKNAME     0x01
#define TRACKLISTFRAME_VISIBLE       0x30
#define TRACKLISTFRAME_ENABLE        0x31
#define TRACKLISTFRAME_VIEWMODE      0x33
#define TRACKLISTFRAME_FORECOLOR     0x34
#define TRACKLISTFRAME_BACKCOLOR     0x35
#define TRACKLISTFRAME_INPUTON       0x36
#define TRACKLISTFRAME_INPUTPORT     0x37
#define TRACKLISTFRAME_INPUTCHANNEL  0x38
#define TRACKLISTFRAME_OUTPUTON      0x39
#define TRACKLISTFRAME_OUTPUTPORT    0x3A
#define TRACKLISTFRAME_OUTPUTCHANNEL 0x3B
#define TRACKLISTFRAME_TIMEPLUS      0x3C
#define TRACKLISTFRAME_KEYPLUS       0x3D
#define TRACKLISTFRAME_VELOCITYPLUS  0x3E
#define TRACKLISTFRAME_NUMEVENT      0xFF
#define TRACKLISTFRAME_CONTROLCHANGE 0xB0
#define TRACKLISTFRAME_PROGRAMCHANGE 0xC0

#ifndef _TRACKLISTFRAME_H_
#define _TRACKLISTFRAME_H_

class CTrackListFrame : public CChildFrame {
	DECLARE_DYNCREATE (CTrackListFrame)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	long m_lToolBar1Height;             // �c�[���o�[�̍���[pixel]
	long m_lScaleHeight;                // �㕔�ڐ���r���[�̍���[pixel]
	long m_lScaleWidth;                 // �����ڐ���r���[�̕�[pixel]
	long m_lTrackHeight;                // �g���b�N�r���[�̍���[pixel]
	long m_lModeWidth;                  // ���[�h�r���[(���y�C��)�̕�[pixel]
	long m_lTimeWidth;                  // �^�C���r���[(�E�y�C��)�̕�[pixel]
	long m_lHScrollBarHeight;           // �����X�N���[���o�[�̍���[pixel]
	long m_lVScrollBarWidth;            // �����X�N���[���o�[�̕�[pixel]
	long m_lColumnZoom;                 // ������g��{��[�{]
	long m_lTimeZoom;                   // ���ԕ����g��{��[�{]
	long m_lRowZoom;                    // �s�����g��{��[�{]
	long m_lColumnScrollPos;            // ������X�N���[���ʒu[pixel]
	long m_lTimeScrollPos;              // ���ԕ����X�N���[���ʒu[pixel]
	long m_lRowScrollPos;               // �s�����X�N���[���ʒu[pixel]
	long m_lColumnBaseWidth[TRACKLISTFRAME_NUMCOLUMN];  // �e��̕�(�g��{���������Ă��Ȃ�)[pixel]
	CString m_strColumnTitle[TRACKLISTFRAME_NUMCOLUMN]; // �e��̃^�C�g��������
	long m_lColumnContent[TRACKLISTFRAME_NUMCOLUMN];    // �e��̕\�����e���ʃR�[�h
	// �e��̕\�����e���ʃR�[�h�́A
	// TRACKLISTFRAME_TRACKNAME�`TRACKLISTFRAME_PROGRAMCHANGE�̒�����I������B
	// TRACKLISTFRAME_CONTROLCHANGE�ł�9�`16�r�b�g�ڂ�CC#�ԍ����w�肷��B

public:
	CSekaijuToolBar m_wndToolBar1;      // �c�[���o�[
	CView* m_pDummyView;                // �_�~�[�r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pPrintView;                // ����r���[(Visible=FALSE)�ւ̃|�C���^
	CView* m_pScaleView;                // �ڐ���r���[�ւ̃|�C���^
	CView* m_pModeScaleView;            // ���ږڐ���r���[�ւ̃|�C���^
	CView* m_pTimeScaleView;            // �^�C���ڐ���r���[�ւ̃|�C���^
	CView* m_pTrackScaleView;           // �g���b�N�ԍ��ڐ���r���[�ւ̃|�C���^
	CView* m_pTrackModeView;            // �g���b�N�ԍ�-���ڃr���[(���y�C��)�ւ̃|�C���^
	CView* m_pTrackTimeView;            // �g���b�N�ԍ�-�^�C���r���[(�E�y�C��)�ւ̃|�C���^
	CScrollBar m_wndColumnScroll;       // ������X�N���[���o�[
	CScrollBar m_wndTimeScroll;         // ���ԕ����X�N���[���o�[
	CScrollBar m_wndRowScroll;          // �s�����X�N���[���o�[
	CScrollBar m_wndSizeScroll;         // �T�C�Y�X�N���[���o�[
	CBitmapButton m_wndColumnZoomUp;    // ������g��{�^��
	CBitmapButton m_wndColumnZoomDown;  // ������k���{�^��
	CBitmapButton m_wndTimeZoomUp;      // ���ԕ����g��{�^��
	CBitmapButton m_wndTimeZoomDown;    // ���ԕ����k���{�^��
	CBitmapButton m_wndRowZoomUp;       // �s�����g��{�^��
	CBitmapButton m_wndRowZoomDown;     // �s�����k���{�^��

public:
	CFont m_theFont;                    // �g���b�N�r���[�E�B���h�E�Ŏg���t�H���g
	long m_lCurTool;                    // ���ݑI������Ă���c�[���ԍ�(0=�I��,1=����)
	BOOL m_bAutoPageUpdate;             // �����I�Ƀy�[�W���X�V���邩�H

protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ���W(�X�v���b�^�[�𓮂����̂Ɏg��)
	CPoint m_ptMouseMoveOld;            // �}�E�X���������ꂽ�O��̍��W(�X�v���b�^�[�𓮂����̂Ɏg��)


	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CTrackListFrame ();                 // �R���X�g���N�^
	virtual ~CTrackListFrame();         // �f�X�g���N�^


	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	CSekaijuDoc* GetDocument ();
	virtual long GetColumnBaseWidth (long lColumn);
	virtual long SetColumnBaseWidth (long lColumn, long lColumnBaseWidth);
	virtual CString GetColumnTitle (long lColumn);
	virtual long GetColumnContent (long lColumn);
	virtual long GetRowZoom ();
	virtual long GetColumnZoom ();
	virtual long GetTimeZoom ();
	virtual long GetColumnLeft (long lColumn);
	virtual long GetColumnWidth (long lColumn);
	virtual long XtoColumn (long x);
	virtual long ColumntoX (long lMode);
	virtual long XtoTime (long x);
	virtual long TimetoX (long lTime);
	virtual long YtoRow (long y);
	virtual long RowtoY (long lKey);
	virtual long GetVisibleTopRow ();
	virtual long GetVisibleBottomRow ();
	virtual long GetVisibleLeftTime ();
	virtual long GetVisibleRightTime ();
	virtual long GetTimeScrollPos ();
	virtual long GetRowScrollPos ();
	virtual long GetColumnScrollPos ();
	virtual long SetTimeScrollPos (long lTimeScrollPos);
	virtual long SetColumnScrollPos (long lModeScrollPos);
	virtual long SetRowScrollPos (long lTrackScrollPos);
	virtual void DrawSplitterCaptor (CDC* pDC, CPoint pt);
	virtual void RecalcRowScrollInfo ();
	virtual void RecalcColumnScrollInfo ();
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
	afx_msg void OnPaint( );
	afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose ();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnColumnZoomDown ();
	afx_msg void OnColumnZoomUp ();
	afx_msg void OnTimeZoomDown ();
	afx_msg void OnTimeZoomUp ();
	afx_msg void OnRowZoomDown ();
	afx_msg void OnRowZoomUp ();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTrackListInsertTrack ();
	afx_msg void OnUpdateTrackListInsertTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListDuplicateTrack ();
	afx_msg void OnUpdateTrackListDuplicateTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListDeleteTrack ();
	afx_msg void OnUpdateTrackListDeleteTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListMoveUpTrack ();
	afx_msg void OnUpdateTrackListMoveUpTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListMoveDownTrack ();
	afx_msg void OnUpdateTrackListMoveDownTrackUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListSelect ();
	afx_msg void OnUpdateTrackListSelectUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListSpeaker ();
	afx_msg void OnUpdateTrackListSpeakerUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListAutoPageUpdate ();
	afx_msg void OnUpdateTrackListAutoPageUpdateUI (CCmdUI* pCmdUI);
	afx_msg void OnTrackListSaveAs ();
	afx_msg void OnUpdateTrackListSaveAsUI (CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


#endif

