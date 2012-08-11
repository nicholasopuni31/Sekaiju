//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g����r���[�N���X
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

#ifndef _EVENTLISTPRINTVIEW_H_
#define _EVENTLISTPRINTVIEW_H_

class CEventListPrintView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CEventListPrintView)

	// CEventListFrame����CEventListPrintView::OnCmdMsg�̌Ăяo����������B
	friend class CEventListFrame;

	// ����֌W
	CSize m_sizDevPaper;                // �������T�C�Y[�h�b�g]
	CSize m_sizLogPaper;                // �_�����T�C�Y[*1/10mm]
	CSize m_sizLogPrinterDPI;           // �v�����^��DPI
	CFont m_theFont;                    // ����p�t�H���g
	long m_lNumEventPerPage;            // 1�y�[�W������̃C�x���g��
	long m_lMaxPage;                    // �ő�y�[�W��

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEventListPrintView();             // �R���X�g���N�^
	virtual ~CEventListPrintView ();   // �f�X�g���N�^

	//------------------------------------------------------------------------------
	// �I�y���[�V����
	//------------------------------------------------------------------------------

protected:
	CRect GetRectFromCell (long lRow, long lColumn);
	CString GetCellString (long lRow, long lColumn);
	void DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo);
	void DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo);

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrint (CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting (CPrintInfo* pInfo);
	virtual void OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting (CDC* pDC, CPrintInfo* pInfo);

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
};

#endif
