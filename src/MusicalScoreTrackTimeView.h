//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃg���b�N�^�C���r���[�N���X
// (C)2002-2011 ���[�Ղ�MIDI�Ղ낶�����Ɓ^����
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

#ifndef _MUSICALSCORETRACKTIMEVIEW_H_
#define _MUSICALSCORETRACKTIMEVIEW_H_

class CMusicalScoreTrackTimeView : public CSekaijuView {

	DECLARE_DYNCREATE (CMusicalScoreTrackTimeView)

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
protected:
	long m_lCurTime;                    // ���݂̕`��^�C��[tick]����[�T�u�t���[��]
	long m_lOldTime;                    // �O��̕`��^�C��[tick]����[�T�u�t���[��]
	long m_lOldX;                       // �O��̏c��x���W[pixel]
	long m_lOldY1;                      // �O��̏c��y����W[pixel]
	long m_lOldY2;                      // �O��̏c��y�����W[pixel]
	BOOL m_bOldDraw;                    // �O��c����`�悵�����H
	long m_lMouseDownTime;              // �}�E�X�������ꂽ�Ƃ��̎w���^�C��[tick]����[�T�u�t���[��]
	long m_lMouseMoveTime;              // �}�E�X���������ꂽ�Ƃ��̑O��̎w���^�C��[tick]����[�T�u�t���[��]
	long m_lMouseDownKey;               // �}�E�X�������ꂽ�Ƃ��̎w�����K(0�`127)
	long m_lMouseMoveKey;               // �}�E�X���������ꂽ�Ƃ��̑O��̎w�����K(0�`127)
	CPoint m_ptMouseDown;               // �}�E�X�������ꂽ�Ƃ��̍��W
	CPoint m_ptMouseMove;               // �}�E�X���������ꂽ�Ƃ��̑O��̍��W
	UINT m_nMouseDownFlags;             // �}�E�X�������ꂽ�Ƃ��̃t���O
	UINT m_nMouseMoveFlags;             // �}�E�X���������ꂽ�Ƃ��̑O��̃t���O
	long m_lTempTool;                   // �ꎞ�I�ȃc�[��(0�`)
	long m_lTempTrackIndex;             // �ꎞ�I�ȃg���b�N�ԍ�(0�`65535)
	long m_lTempSnap;                   // �ꎞ�I�ȃX�i�b�v�^�C��[tick]
	long m_lTempVelocity;               // �ꎞ�I�ȃx���V�e�B(0�`127)
	long m_lTempDuration;               // �ꎞ�I�ȉ�����(0�`)[tick]
	long m_lTempOutputPort;             // �ꎞ�I�ȏo�̓|�[�g(0�`15)
	long m_lTempChannel;                // �ꎞ�I�ȃ`�����l��(0�`15)
	long m_lTempKey;                    // �ꎞ�I�ȉ��K(0�`127)
	long m_lTempTime;                   // �ꎞ�I�ȃ^�C��(0�`)[tick]
	long m_lTempTimeNoteOn;             // �ꎞ�I�ȃm�[�g�I���^�C��(0�`)[tick]
	MIDITrack* m_pTempTrack;            // �ꎞ�I�ȃg���b�N�ւ̃|�C���^
	MIDIEvent* m_pTempEvent;            // �ꎞ�I�ȃC�x���g�ւ̃|�C���^
	MIDIEvent* m_pLastEvent;            // �ꎞ�I�ȍŌ�̃C�x���g�ւ̃|�C���^
	long m_lTempMode;                   // �ꎞ�I�ȃ��[�h
	CPtrArray m_theTempSelectedEventArray; // �ꎞ�I�ȑI������Ă���C�x���g�̔z��

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMusicalScoreTrackTimeView ();      // �R���X�g���N�^
	virtual ~CMusicalScoreTrackTimeView (); // �f�X�g���N�^
	
	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
protected:
	virtual CRect GetNoteRect (MIDIEvent* pNoteOnEvent);
	virtual CRect GetNoteRect (MusicalScoreNoteInfo* pNoteInfo);
	virtual void EraseOldLine (CDC* pDC);
	virtual void DrawCurLine (CDC* pDC);
	virtual BOOL ShowPopupMenu (CPoint ptMenu);
	virtual void DrawTadpole
		(CDC* pDC, long x, long y, long rx, long ry, long lFlags);
	virtual void DrawHorzAuxiliaryLine
		(CDC* pDC, long x, long y, long r, long lFlags);
	void DrawDot
		(CDC* pDC, long x, long y, long rx, long ry, long lFlags);
	void DrawTieHalf
		(CDC* pDC, long x1, long x2, long y, long rx, long ry, long lFlags);
	void DrawPole 
		(CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawSingleFlag
		(CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawChainedFlag 
		(CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawTripletSign 
		(CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawNote
		(CDC* pDC, long lTrackIndex, MusicalScoreNoteInfo* pNoteInfo, long lFlags);
	void DrawGClef 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawFClef 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawFlat 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawNatural
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawSharp 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawTimeAndKeySignature 
		(CDC* pDC, long lTrackIndex, long lTime);

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
	afx_msg void OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif

