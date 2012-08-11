//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�C���f�b�N�X�v���p�e�B�r���[�N���X
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

#ifndef _EVENTLISTINDEXPROPERTYVIEW_H_
#define _EVENTLISTINDEXPROPERTYVIEW_H_

#include "InplaceEdit.h"
#include "InplaceListBox.h"

class CEventListIndexPropertyView : public CSekaijuView {

	DECLARE_DYNCREATE (CEventListIndexPropertyView)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEventListIndexPropertyView();          // �R���X�g���N�^
	virtual ~CEventListIndexPropertyView(); // �f�X�g���N�^


	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lCurRow;                     // ���݂̍s�ԍ�(0�`)
	long m_lCurColumn;                  // ���݂̗�ԍ�(0�`)
	long m_lCurButtonState;             // ���݂̃{�^�����
	long m_lCurButtonInterval;          // ���݂̃{�^���̑����Ԋu
	CInplaceEdit m_theTextBox;          // �C���v���[�X�e�L�X�g�{�b�N�X
	CInplaceListBox m_theListBox;       // �C���v���[�X���X�g�{�b�N�X

protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	MIDITrack* m_pCloneTrack;           // �ꎞ�I�ȕ������ꂽ�g���b�N�ւ̃|�C���^
	MIDIEvent* m_pCloneEvent;           // �ꎞ�I�ȕ������ꂽ�C�x���g�ւ̃|�C���^
	MIDIEvent* m_pLastEvent;            // �ꎞ�I�ȍŌ�̃C�x���g�ւ̃|�C���^
	BOOL   m_bSettingCellString;        // �Z���̕������ݒ蒆��

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
protected:
	virtual BOOL GetCellFromPoint (CPoint pt, long* pRow, long* pColumn);
	virtual CRect GetRectFromCell (long lRow, long lColumn);

public:
	virtual BOOL AutoScrolltoShowCell (long lRow, long lColumn);
	virtual BOOL IsTextEditing ();
	virtual BOOL IsListSelecting ();
	virtual BOOL BeginTextEditing ();
	virtual BOOL BeginListSelecting ();
	virtual BOOL EndTextEditingOK ();
	virtual BOOL EndListSelectingOK ();
	virtual BOOL EndTextEditingCancel ();
	virtual BOOL EndListSelectingCancel ();
	virtual BOOL BeginValueUpDown ();
	virtual BOOL EndValueUpDown ();
	virtual BOOL AddValueOfCurCell (long lDeltaValue);
	virtual BOOL MoveTextBox (long lRow, long lColumn);
	virtual BOOL MoveListBox (long lRow, long lColumn);
	virtual BOOL UpdateParentToolbarControl ();
	virtual CString GetCellString (long lRow, long lColumn);
	virtual BOOL SetCellString (long lRow, long lColumn, CString strText);
	virtual BOOL ShowPopupMenu (CPoint point);
	virtual int DrawCurFrame (CDC* pDC);

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
public:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw (CDC* pDC);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy ();
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

