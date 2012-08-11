//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃg���b�N�X�P�[���r���[�N���X
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

#ifndef _MUSICALSCORETRACKSCALEVIEW_H_
#define _MUSICALSCORETRACKSCALEVIEW_H_

#include "InplaceEdit.h"

class CMusicalScoreTrackScaleView : public CSekaijuView {
protected:
	DECLARE_DYNCREATE (CMusicalScoreTrackScaleView)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��O��̍��W
	UINT   m_nMouseDownFlags;           // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT   m_nMouseMoveFlags;           // �}�E�X���������ꂽ�Ƃ��O��̃t���O
	long   m_lDownTrack;                // �}�E�X�������ꂽ�Ƃ��̎w���g���b�N(0�`MAXMIDITRACKNUM-1)
	long   m_lCurTrack;                 // �}�E�X���������ꂽ�Ƃ��̌��݂̎w���g���b�N(0�`MAXMIDITRACKNUM-1)
	long   m_lOldTrack;                 // �}�E�X���������ꂽ�Ƃ��̑O��̎w���g���b�N(0�`MAXMIDITRACKNUM-1)
	long   m_lOldTime;                  // �O��̃^�C��[tick]
	BOOL   m_bSettingCellString;         // �Z���̕������ҏW���ł��邩

	// �e�L�X�g�{�b�N�X
	CInplaceEdit  m_theTextBox;

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMusicalScoreTrackScaleView ();           // �R���X�g���N�^
	virtual ~CMusicalScoreTrackScaleView ();  // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
protected:
	void DrawGClef 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawFClef 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawFlat 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawSharp 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawTimeAndKeySignature 
		(CDC* pDC, long lTrackIndex, long lTime);
	
	CString GetCellString (long lTrack);
	BOOL SetCellString (long lTrack, CString strText);
	BOOL IsTextEditing ();
	BOOL BeginTextEditing ();
	BOOL EndTextEditingOK ();
	BOOL EndTextEditingCancel ();
	BOOL MoveTextBox (long lTrackIndex);

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
public:
	virtual void OnDraw (CDC* pDC);
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo = NULL);

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy ();
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()
};

#endif

