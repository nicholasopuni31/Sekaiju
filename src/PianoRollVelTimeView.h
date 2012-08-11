//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �s�A�m���[���x���V�e�B�^�C���r���[�N���X
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

#ifndef _PIANOROLLVELTIMEVIEW_H_
#define _PIANOROLLVELTIMEVIEW_H_

class CPianoRollVelTimeView : public CSekaijuView {
	DECLARE_DYNCREATE(CPianoRollVelTimeView)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CPianoRollVelTimeView();            // �R���X�g���N�^
	virtual ~CPianoRollVelTimeView();   // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
	
	// �J�����g�|�W�V�����c���`��p
protected:
	long m_lCurTime;                    // ���݂̕`��^�C��[tick]
	long m_lOldTime;                    // �O��̕`��^�C��[tick]
	long m_lOldX;                       // �O��̏c��x���W[pixel]
	long m_lOldY1;                      // �O��̏c��y����W[pixel]
	long m_lOldY2;                      // �O��̏c��y�����W[pixel]
	BOOL m_bOldDraw;                    // �O��c����`�悵�����H
protected:
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	UINT m_nMouseDownFlags;             // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT m_nMouseMoveFlags;             // �}�E�X���������ꂽ�Ƃ��̑O��̃t���O
	long m_lTempTool;                   // �ꎞ�I�ȃc�[��
	long m_lTempTrackIndex;             // �ꎞ�I�ȃg���b�N�ԍ�(0�`65535)
	long m_lTempSnap;                   // �ꎞ�I�ȃX�i�b�v(1�`)[tick]
	long m_lTempVelocity;               // �ꎞ�I�ȃx���V�e�B(1�`127)
	long m_lTempPortIndex;              // �ꎞ�I�ȃ|�[�g�ԍ�(0�`15)
	long m_lTempChannel;                // �ꎞ�I�ȃ`�����l��(0�`15)
	long m_lTempGraphKind;              // �ꎞ�I�ȃO���t�̎��
	long m_lTempGraphSnap;              // �ꎞ�I�ȃO���t�̃X�i�b�v(1�`)[tick]
	long m_lTempTime;                   // �ꎞ�I�ȃ^�C��(0�`)[tick]
	long m_lTempTimeNoteOn;             // �ꎞ�I�ȃm�[�g�I���^�C��(0�`)[tick]
	MIDIEvent* m_pTempEvent;            // �ꎞ�I�ȃC�x���g�ւ̃|�C���^
	MIDIEvent* m_pLastEvent;            // �ꎞ�I�ȍŌ�̃C�x���g�ւ̃|�C���^
	long m_lTempMode;                   // �ꎞ�I�ȃ��[�h
	long m_lLastInsertedValue;
	long m_lLastInsertedTempoBPM;
	long m_lLastInsertedPitchBend;

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
protected:
	virtual void EraseOldLine (CDC* pDC);
	virtual void DrawCurLine (CDC* pDC);

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy ();
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif


