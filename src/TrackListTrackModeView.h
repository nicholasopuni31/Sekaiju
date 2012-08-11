//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�g���b�N���[�h�r���[�N���X
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

#ifndef _TRACKLISTTRACKMODEVIEW_H_
#define _TRACKLISTTRACKMODEVIEW_H_

#include "InplaceEdit.h"

class CTrackListTrackModeView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListTrackModeView)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CTrackListTrackModeView();          // �R���X�g���N�^
	virtual ~CTrackListTrackModeView(); // �f�X�g���N�^


	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
	long m_lCurRow;                     // ���݂̍s�ԍ�(0�`)
	long m_lCurColumn;                  // ���݂̗�ԍ�(0�`)
	long m_lCurButtonState;             // ���݂̍s�E��̃{�^�����
	long m_lCurButtonInterval;          // �{�^���̑����Ԋu

protected:
	// �e�L�X�g�{�b�N�X
	CInplaceEdit m_theTextBox;

	// �}�E�X�L���v�^�[������
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ���W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�O��̍��W


	BOOL   m_bSettingCellString;        // �Z���̕������ҏW���ł��邩

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	virtual BOOL GetCellFromPoint (CPoint pt, long* pRow, long* pColumn);
	virtual BOOL AutoScrolltoShowCell (long lRow, long lColumn);
	virtual BOOL IsTextEditing ();
	virtual BOOL BeginTextEditing ();
	virtual BOOL EndTextEditingOK ();
	virtual BOOL EndTextEditingCancel ();
	virtual BOOL BeginValueUpDown ();
	virtual BOOL EndValueUpDown ();
	virtual BOOL AddValueOfCurCell (long lDeltaValue);
	virtual BOOL ToggleValueOfCurCell ();
	virtual BOOL DoColorEditing ();
	virtual CRect GetRectFromCell (long lRow, long lColumn);
	virtual BOOL MoveTextBox (long lRow, long lColumn);
	virtual CString GetCellString (long lRow, long lColumn);
	virtual BOOL SetCellString (long nRow, long nColumn, CString strText);
	virtual BOOL DrawCurFrame (CDC* pDC);

	// �I�[�o�[���C�h
protected:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// ���b�Z�[�W�}�b�v
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnActivate (UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
  	afx_msg void OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP ();
};

#endif

