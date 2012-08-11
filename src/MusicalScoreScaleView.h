//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʃX�P�[���E�B���h�E�N���X
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

#ifndef _MUSICALSCORESCALEVIEW_H_
#define _MUSICALSCORESCALEVIEW_H_

class CMusicalScoreScaleView : public CSekaijuView {
	DECLARE_DYNCREATE (CMusicalScoreScaleView)

	//-------------------------------------------------------------------------
	// �\�z�Ɣj��
	//-------------------------------------------------------------------------	
public:
	CMusicalScoreScaleView ();          // �R���X�g���N�^
	virtual ~CMusicalScoreScaleView (); // �f�X�g���N�^
	
	//-------------------------------------------------------------------------	
	// �I�[�o�[���C�h
	//-------------------------------------------------------------------------	
public:
	virtual void OnDraw (CDC* pDC);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);

	//-------------------------------------------------------------------------	
	// ���b�Z�[�W�}�b�v
	//-------------------------------------------------------------------------	
protected:
 	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()
};


#endif
