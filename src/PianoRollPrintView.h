//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �s�A�m���[������r���[�N���X
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

#ifndef _PIANOROLLPRINTVIEW_H_
#define _PIANOROLLPRINTVIEW_H_

class CPianoRollPrintView : public CSekaijuView {
	DECLARE_DYNCREATE (CPianoRollPrintView)

	// CPianoRollFrame����CPianoRollPrintView::OnCmdMsg�̌Ăяo����������B
	friend class CPianoRollFrame;

	// ����֌W
	CSize m_sizDevPaper;                // �������T�C�Y[�h�b�g]
	CSize m_sizLogPaper;                // �_�����T�C�Y[*1/10mm]
	CSize m_sizLogPrinterDPI;           // �v�����^��DPI
	CFont m_theFont;                    // ����p�t�H���g

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CPianoRollPrintView ();             // �R���X�g���N�^
	virtual ~CPianoRollPrintView ();    // �f�X�g���N�^

	//------------------------------------------------------------------------------
	// �I�y���[�V����
	//------------------------------------------------------------------------------

protected:
	long TimetoX (long lTime);
	long VeltoY (long lVel);
	long PitchBendtoY (long lPitchBend);
	long TempoBPMtoY (long lTempoBPM);
	long KeyToY (long lKey);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);
	void DrawVelScaleView (CDC* pDC);
	void DrawVelTimeView (CDC* pDC);
	void DrawKeyScaleView (CDC* pDC);
	void DrawKeyTimeView (CDC* pDC);
	void DrawScaleView (CDC* pDC);
	void DrawTimeScaleView (CDC* pDC);

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

