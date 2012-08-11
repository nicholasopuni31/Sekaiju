//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�g���b�N�X�P�[���r���[�N���X
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

#ifndef _TRACKLISTTRACKSCALEVIEW_H_
#define _TRACKLISTTRACKSCALEVIEW_H_

class CTrackListTrackScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListTrackScaleView)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	UINT   m_nMouseDownFlags;           // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT   m_nMouseMoveFlags;           // �}�E�X���������ꂽ�Ƃ��̑O��̃t���O
	long   m_lDownRow;                  // �}�E�X�������ꂽ�Ƃ��̍s�ԍ�
	long   m_lCurRow;                   // �}�E�X���w�����݂̍s�ԍ�
	long   m_lOldRow;                   // �}�E�X���w���O��̍s�ԍ�

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CTrackListTrackScaleView();             // �R���X�g���N�^
	virtual ~CTrackListTrackScaleView ();   // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void OnDraw (CDC* pDC);
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
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
