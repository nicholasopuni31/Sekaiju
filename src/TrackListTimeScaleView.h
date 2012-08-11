//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �g���b�N���X�g�^�C���X�P�[���r���[�N���X
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

#ifndef _TRACKLISTTIMESCALEVIEW_H_
#define _TRACKLISTTIMESCALEVIEW_H_

class CTrackListTimeScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListTimeScaleView)
	
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CTrackListTimeScaleView ();             // �R���X�g���N�^
	virtual ~CTrackListTimeScaleView ();    // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;                // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;                // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	UINT   m_nMouseDownFlags;            // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT   m_nMouseMoveFlags;            // �}�E�X���������ꂽ�Ƃ��̑O��̃t���O
	long   m_lDownMeasure;               // �}�E�X�������ꂽ�ʒu�̏���(0�`)
	long   m_lDownBeat;                  // �}�E�X�������ꂽ�ʒu�̔�(0�`)
	long   m_lDownTick;                  // �}�E�X�������ꂽ�ʒu�̃e�B�b�N(0�`)
	long   m_lDownTime;                  // �}�E�X�������ꂽ�ʒu�̃^�C��(0�`)[tick]
	long   m_lOldMeasure;                // �}�E�X���������ꂽ�O��̈ʒu�̏���(0�`)
	long   m_lOldBeat;                   // �}�E�X���������ꂽ�O��̈ʒu�̔�(0�`)
	long   m_lOldTick;                   // �}�E�X���������ꂽ�O��̈ʒu�̃e�B�b�N(0�`)
	long   m_lOldTime;                   // �}�E�X���������ꂽ�O��̈ʒu�̃^�C��(0�`)[tick]
	long   m_lCurMeasure;                // �}�E�X���������ꂽ���݂̈ʒu�̏���(0�`)
	long   m_lCurBeat;                   // �}�E�X���������ꂽ���݂̈ʒu�̔�(0�`)
	long   m_lCurTick;                   // �}�E�X���������ꂽ���݂̈ʒu�̃e�B�b�N(0�`)
	long   m_lCurTime;                   // �}�E�X���������ꂽ���݂̈ʒu�̃^�C��(0�`)[tick]

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	virtual void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy ();
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
