//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃ^�C���X�P�[���r���[�N���X
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

class CMusicalScoreTimeScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CMusicalScoreTimeScaleView)

	//-------------------------------------------------------------------------
	// �A�g���r���[�g
	//-------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	UINT   m_nMouseDownFlags;           // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT   m_nMouseMoveFlags;           // �}�E�X���������ꂽ�Ƃ��̃t���O
	long   m_lDownTime;                 // �}�E�X�������ꂽ�Ƃ��̎w������[tick]����[�T�u�t���[��]
	long   m_lOldTime;                  // �}�E�X���������ꂽ�Ƃ̑O��̎w������[tick]����[�T�u�t���[��]
	long   m_lCurTime;                  // �}�E�X���������ꂽ�Ƃ̌��݂̎w������[tick]����[�T�u�t���[��]
	long   m_lTempSnap;                 // �ꎞ�I�ȃX�i�b�v
	
	//-------------------------------------------------------------------------
	// �\�z�Ɣj��
	//-------------------------------------------------------------------------
public:
	CMusicalScoreTimeScaleView();       // �R���X�g���N�^
	virtual ~CMusicalScoreTimeScaleView(); // �f�X�g���N�^


	//-------------------------------------------------------------------------
	// �I�y���[�V����
	//-------------------------------------------------------------------------
protected:
	virtual void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);

	//-------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//-------------------------------------------------------------------------
public:
	virtual void OnDraw (CDC* pDC);
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);

	//-------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//-------------------------------------------------------------------------
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
	DECLARE_MESSAGE_MAP()
};
