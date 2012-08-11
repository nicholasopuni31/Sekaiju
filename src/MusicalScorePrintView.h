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

#ifndef _MUSICALSCOREPRINTVIEW_H_
#define _MUSICALSCOREPRINTVIEW_H_

class CMusicalScorePrintView : public CSekaijuView {
	DECLARE_DYNCREATE (CMusicalScorePrintView)

	// CMusicalScoreFrame����CMusicalScorePrintView::OnCmdMsg�̌Ăяo����������B
	friend class CMusicalScoreFrame;

	// ����֌W
	CSize m_sizDevPaper;                // �������T�C�Y[�h�b�g]
	CSize m_sizLogPaper;                // �_�����T�C�Y[*1/10mm]
	CSize m_sizLogPrinterDPI;           // �v�����^��DPI
	CFont m_theFont;                    // ����p�t�H���g
	long m_lMaxRowPage;                 // �ő�y�[�W��(�c����)
	long m_lMaxRollPage;                // �ő�y�[�W��(���ʃ��[������)
	long m_lMaxPage;                    // �ő�y�[�W��(�c����*���ʃ��[������)
	CUIntArray m_theRowPageInfoArray;    // �c�����̃y�[�W���z��(�擪���ߔԍ�)
	CUIntArray m_theRollPageInfoArray;   // ���ʃ��[�������̃y�[�W���z��(�擪�g���b�N�ԍ�)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMusicalScorePrintView ();             // �R���X�g���N�^
	virtual ~CMusicalScorePrintView ();    // �f�X�g���N�^

	//------------------------------------------------------------------------------
	// �I�y���[�V����
	//------------------------------------------------------------------------------

protected:
	
	long GetRowPageInfoCount () {return m_theRowPageInfoArray.GetSize ();};
	long GetRollPageInfoCount () {return m_theRollPageInfoArray.GetSize ();};

	void CalcMeasureInfoArray ();
	void CalcTrackInfoArray ();

	long TrackIndexLineNotoY (long lTrackIndex, long lLineNo);
	long TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature);
	long TimetoX (long lTime);
	long MeasuretoX (long lMeasure);
	long MeasuretoX2 (long lMeasure);
	
	void DrawTadpole (CDC* pDC, long x, long y, long rx, long ry, long lFlags);
	void DrawGClef (CDC* pDC, long x, long y, long rx, long ry);
	void DrawFClef (CDC* pDC, long x, long y, long rx, long ry);
	void DrawFlat (CDC* pDC, long x, long y, long rx, long ry);
	void DrawSharp (CDC* pDC, long x, long y, long rx, long ry);
	void DrawNatural (CDC* pDC, long x, long y, long rx, long ry);
	void DrawTimeAndKeySignature (CDC* pDC, long lTrackIndex, long lTime);
	void DrawHorzAuxiliaryLine (CDC* pDC, long x, long y, long r, long lFlags);
	void DrawDot (CDC* pDC, long x, long y, long rx, long ry, long lFlags);
	void DrawTieHalf (CDC* pDC, long x1, long x2, long y, long rx, long ry, long lFlags);
	void DrawPole (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawSingleFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawChainedFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawTripletSign (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags);
	void DrawNote (CDC* pDC, long lTrackIndex, MusicalScoreNoteInfo* pNoteInfo, long lFlags);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);

	void DrawTrackScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo);

	CString GetCellString (long lTrack);

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

