//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �r���[�N���X
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

// ���̃N���X�̖�����CSekaijuDoc* GetDocument()��񋟂��邱�Ƃ݂̂ł���A
// ���p�I�ȃr���[��CSekaijuView����h�������č�邱�ƁB
// �Ȃ��A���̃N���X��Visible=FALSE�̃_�~�[�r���[�Ƃ��Ă��g����B

#ifndef _SEKAIJUVIEW_H_
#define _SEKAIJUVIEW_H_

class CSekaijuView : public CView {
	DECLARE_DYNCREATE(CSekaijuView)
	
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CSekaijuView ();                    // �R���X�g���N�^
	virtual ~CSekaijuView ();           // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	CSekaijuDoc* GetDocument ();        // �h�L�������g�̎擾
	
	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void OnDraw (CDC* pDC);
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	
	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CSekaijuDoc* CSekaijuView::GetDocument () {
	return (CSekaijuDoc*)m_pDocument;
}
#endif



#endif
