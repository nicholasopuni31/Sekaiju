//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �s�A�m���[���x���V�e�B�X�P�[���r���[�N���X
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

#ifndef _PIANOROLLVELSCALEVIEW_H_
#define _PIANOROLLVELSCALEVIEW_H_

class CPianoRollVelScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CPianoRollVelScaleView)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	UINT   m_nMouseDownFlags;           // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT   m_nMouseMoveFlags;           // �}�E�X���������ꂽ�Ƃ��̑O��̃t���O
	long   m_lDownTempoBPM;             // �}�E�X�������ꂽ�ʒu�̃e���|[BPM]
	long   m_lCurTempoBPM;              // �}�E�X���������ꂽ���݂̈ʒu�̃e���|[BPM]
	long   m_lOldTempoBPM;              // �}�E�X���������ꂽ�O��̈ʒu�̃e���|[BPM]
	long   m_lDownVel;                  // �}�E�X�������ꂽ�ʒu�̃x���V�e�B���͒l(0�`127)
	long   m_lCurVel;                   // �}�E�X���������ꂽ���݂̈ʒu�̃x���V�e�B���͒l(0�`127)
	long   m_lOldVel;                   // �}�E�X���������ꂽ�O��̈ʒu�̃x���V�e�B���͒l(0�`127)
	long   m_lDownPitchBend;            // �}�E�X�������ꂽ�ʒu�̃s�b�`�x���h
	long   m_lCurPitchBend;             // �}�E�X���������ꂽ���݂̈ʒu�̃s�b�`�x���h
	long   m_lOldPitchBend;             // �}�E�X���������ꂽ�O��̈ʒu�̃s�b�`�x���h

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CPianoRollVelScaleView ();			// �R���X�g���N�^
	virtual ~CPianoRollVelScaleView (); // �f�X�g���N�^

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
