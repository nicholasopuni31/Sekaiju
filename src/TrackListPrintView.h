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

#ifndef _TRACKLISTPRINTVIEW_H_
#define _TRACKLISTPRINTVIEW_H_

class CTrackListPrintView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CTrackListPrintView)

	// CTrackListFrame����CTrackListPrintView::OnCmdMsg�̌Ăяo����������B
	friend class CTrackListFrame;

	// ����֌W
	CSize m_sizDevPaper;                // �������T�C�Y[�h�b�g]
	CSize m_sizLogPaper;                // �_�����T�C�Y[*1/10mm]
	CSize m_sizLogPrinterDPI;           // �v�����^��DPI
	CFont m_theFont;                    // ����p�t�H���g
	long m_lNumTrackPerPage;            // 1�y�[�W������̃g���b�N��
	long m_lMaxRowPage;                 // �ő�y�[�W��(�c����)
	long m_lMaxColPage;                 // �ő�y�[�W��(������)
	long m_lMaxRollPage;                // �ő�y�[�W��(�s�A�m���[������)
	long m_lMaxPage;                    // �ő�y�[�W��(�c����*(������+�s�A�m���[������))

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CTrackListPrintView();              // �R���X�g���N�^
	virtual ~CTrackListPrintView ();    // �f�X�g���N�^

	//------------------------------------------------------------------------------
	// �I�y���[�V����
	//------------------------------------------------------------------------------
protected:
	CString GetCellString (long lRow, long lColumn);
	long TimetoX (long lTime);
	void DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo);
	void DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);
	void DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo);

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
