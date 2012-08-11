//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g�v���p�e�B�X�P�[���r���[�N���X
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

#ifndef _EVENTLISTPROPERTYSCALEVIEW_H_
#define _EVENTLISTPROPERTYSCALEVIEW_H_

class CEventListPropertyScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CEventListPropertyScaleView)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMoveOld;            // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	long   m_lTempColumnIndex;          // �}�E�X�������ꂽ�Ƃ��̃t���O
	long   m_lTempColumnBaseWidth;      // �}�E�X���������ꂽ�Ƃ��̑O��̃t���O

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEventListPropertyScaleView ();             // �R���X�g���N�^
	virtual ~CEventListPropertyScaleView();     // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:


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
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()

};

#endif
