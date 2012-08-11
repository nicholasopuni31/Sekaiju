//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���ʈ���r���[�N���X
// (C)2002-2012 ���[�Ղ�MIDI�Ղ낶�����Ɓ^����
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

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
#include <afxpriv.h>
#include "common.h"
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "MusicalScoreFrame.h"
#include "MusicalScorePrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MUSICALSCOREPRINTVIEW_LEFTMARGIN 200
#define MUSICALSCOREPRINTVIEW_RIGHTMARGIN 200
#define MUSICALSCOREPRINTVIEW_TOPMARGIN 200
#define MUSICALSCOREPRINTVIEW_BOTTOMMARGIN 200
#define MUSICALSCOREPRINTVIEW_SCALEHEIGHT 100
#define MUSICALSCOREPRINTVIEW_SCALEWIDTH 120


IMPLEMENT_DYNCREATE (CMusicalScorePrintView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CMusicalScorePrintView, CSekaijuView)
	ON_COMMAND (ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CMusicalScorePrintView::CMusicalScorePrintView () {
	// ����p�����t�H���g�̍쐬(2.5�~��)
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (25, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);

}

// �f�X�g���N�^
CMusicalScorePrintView::~CMusicalScorePrintView () {
	m_theFont.DeleteObject ();
	m_theRowPageInfoArray.RemoveAll ();
	m_theRollPageInfoArray.RemoveAll ();
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// ���ߏ��\���̔z��̈���֌W�������v�Z����B
void CMusicalScorePrintView::CalcMeasureInfoArray () {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	m_theRollPageInfoArray.RemoveAll ();
	long lTimeZoom = 4;
	long lLeftMargin = MUSICALSCOREPRINTVIEW_LEFTMARGIN;
	long lRightMargin = MUSICALSCOREPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = MUSICALSCOREPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	long lMeasureInfoCount = pMusicalScoreFrame->GetMeasureInfoCount ();
	long lMeasure = 0;
	long lMeasure2 = 0;
	long lCurRollPage = 0;
	while (lMeasure < lMeasureInfoCount) {
		long lStartMeasure = lMeasure;
		long lEndMeasure = lMeasure;
		long lCurLeftPrint = 0;
		long lNextLeftPrint = 0;
		long lTempWidth = 0;
		// ���̃y�[�W�ɓ��鏬�ߐ��Ə��ߕ�(��)���v�Z
		while (lMeasure < lMeasureInfoCount) {
			MusicalScoreMeasureInfo* pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lMeasure);
			// ���̏��߂̊J�nx�ʒu[*1/10mm]���v�Z
			// ���̃y�[�W�̍ŏ��̏��߂̏ꍇ�A���q�L���E�����L���K�v
			if (lMeasure - lStartMeasure == 0) {
				lNextLeftPrint =
					lCurLeftPrint +
					(16 + pMeasureInfo->m_lPreWidth + 
					pMeasureInfo->m_lWidth + pMeasureInfo->m_lPostWidth) * 3 * lTimeZoom;
			}
			// �ʏ�̏ꍇ
			else {
				lNextLeftPrint =
					lCurLeftPrint +
					(pMeasureInfo->m_lSignatureWidth + pMeasureInfo->m_lPreWidth + 
					pMeasureInfo->m_lWidth + pMeasureInfo->m_lPostWidth) * 3 * lTimeZoom;
			}
			// ���̏��߂̊J�nx�ʒu[*1/10mm]�����ʕ�����͂ݏo���Ă���ꍇ���[�v�E�o
			if (lNextLeftPrint > lTimeWidth || lMeasure == lMeasureInfoCount - 1) {
				lEndMeasure = lMeasure;
				lTempWidth = lCurLeftPrint;
				// ������1�y�[�W�ɍŒ�1���߂͓����B
				// �ŏI���߂̏ꍇ�̂݁A����ȏ�while���[�v���Ȃ�����
				if (lEndMeasure <= lStartMeasure || lMeasure == lMeasureInfoCount - 1) {
					lEndMeasure = lMeasure + 1;
					lTempWidth = lNextLeftPrint;
					lMeasure++;
				}
				// lMeasure�͍���̃y�[�W�ɂ͊܂߂Ȃ��̂ŁA�C���N�������g�����ɒE�o
				break;
			}
			lCurLeftPrint = lNextLeftPrint;
			lMeasure++;
		}
		// ���̃y�[�W�ɓ��鏬�߂̏��ߏ��\���̂̈���֌W������ݒ�
		lCurLeftPrint = 0;
		lNextLeftPrint = 0;
		for (lMeasure2 = lStartMeasure; lMeasure2 < lEndMeasure; lMeasure2++) {
			MusicalScoreMeasureInfo* pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lMeasure2);
			pMeasureInfo->m_lLeftPrint = 
				lTimeWidth * lCurRollPage + lCurLeftPrint;
			// ���̃y�[�W�̍ŏ��̏��߂̏ꍇ�A���q�L���E�����L���K�v
			if (lMeasure2 - lStartMeasure == 0) {
				pMeasureInfo->m_lSignatureWidthPrint = 
					16 * 3 * lTimeZoom * lTimeWidth / lTempWidth;
			}
			// �ʏ�̏ꍇ
			else {
				pMeasureInfo->m_lSignatureWidthPrint = 
					pMeasureInfo->m_lSignatureWidth * 3 * lTimeZoom * lTimeWidth / lTempWidth;
			}
			pMeasureInfo->m_lPreWidthPrint = 
				pMeasureInfo->m_lPreWidth * 3 * lTimeZoom * lTimeWidth / lTempWidth;
			pMeasureInfo->m_lWidthPrint = 
				pMeasureInfo->m_lWidth * 3 * lTimeZoom * lTimeWidth / lTempWidth;
			pMeasureInfo->m_lPostWidthPrint = 
				pMeasureInfo->m_lPostWidth * 3 * lTimeZoom * lTimeWidth / lTempWidth;
			lNextLeftPrint =
				lCurLeftPrint + pMeasureInfo->m_lSignatureWidthPrint +
				pMeasureInfo->m_lPreWidthPrint + pMeasureInfo->m_lWidthPrint + 
				pMeasureInfo->m_lPostWidthPrint;
			lCurLeftPrint = lNextLeftPrint;
		}
		// ���ʃ��[�������y�[�W���z��ɐ擪���ߔԍ����L�^
		m_theRollPageInfoArray.Add (lStartMeasure);
		// �y�[�W�X�V
		lCurRollPage++;
	}
	m_theRollPageInfoArray.Add (lMeasureInfoCount - 1);
}

// �g���b�N���\���̔z��̈���֌W�������v�Z����B
void CMusicalScorePrintView::CalcTrackInfoArray () {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	m_theRowPageInfoArray.RemoveAll ();
	long lTrackZoom = 4;
	long lTopMargin = MUSICALSCOREPRINTVIEW_TOPMARGIN;
	long lBottomMargin = MUSICALSCOREPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = MUSICALSCOREPRINTVIEW_SCALEHEIGHT;
	long lTrackHeight = m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin;
	long lTrackInfoCount = pMusicalScoreFrame->GetTrackInfoCount ();
	long lTrack = 0;
	long lTrack2 = 0;
	long lCurRowPage = 0;
	while (lTrack < lTrackInfoCount) {
		long lStartTrack = lTrack;
		long lEndTrack = lTrack;
		long lCurTopPrint = 0;
		long lNextTopPrint = 0;
		long lTempHeight = 0;
		// ���̃y�[�W�ɓ���g���b�N���ƃg���b�N����(��)���v�Z
		while (lTrack < lTrackInfoCount) {
			MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrack);
			// ���̃g���b�N�̊J�ny�ʒu[*1/10mm]���v�Z
			lNextTopPrint =
				lCurTopPrint + pTrackInfo->m_lHeight * 2 * lTrackZoom;
			// ���̃g���b�N�̊J�ny�ʒu[*1/10mm]�����ʍ�������͂ݏo���Ă���ꍇ���[�v�E�o
			if (lNextTopPrint > lTrackHeight || lTrack == lTrackInfoCount - 1) {
				lEndTrack = lTrack;
				lTempHeight = lCurTopPrint;
				// ������1�y�[�W�ɍŒ�1�g���b�N�͓����B
				// �ŏI�g���b�N�̏ꍇ�̂݁A����ȏ�while���[�v���Ȃ�����
				if (lEndTrack <= lStartTrack || lTrack == lTrackInfoCount - 1) {
					lEndTrack = lTrack + 1;
					lTempHeight = lNextTopPrint;
					lTrack++;
				}
				// lTrack�͍���̃y�[�W�Ɋ܂߂Ȃ��̂ŃC���N�������g�����ɒE�o
				break;
			}
			lCurTopPrint = lNextTopPrint;
			lTrack++;
		}
		// ���̃y�[�W�ɓ���g���b�N�̃g���b�N���\���̂̈���֌W������ݒ�
		lCurTopPrint = 0;
		lNextTopPrint = 0;
		for (lTrack2 = lStartTrack; lTrack2 < lEndTrack; lTrack2++) {
			MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrack2);
			pTrackInfo->m_lTopPrint = 
				lTrackHeight * lCurRowPage + lCurTopPrint;
			if (lTrackHeight / lTempHeight >= 2) {
				pTrackInfo->m_lHeightPrint = 
					pTrackInfo->m_lHeight * 2 * lTrackZoom * 2;
			}
			else {
				pTrackInfo->m_lHeightPrint = 
					pTrackInfo->m_lHeight * 2 * lTrackZoom * lTrackHeight / lTempHeight;
			}
			lNextTopPrint =
				lCurTopPrint + pTrackInfo->m_lHeightPrint;
			lCurTopPrint = lNextTopPrint;
		}
		// �s�����y�[�W���z��ɐ擪���ߔԍ����L�^
		m_theRowPageInfoArray.Add (lStartTrack);
		// �y�[�W�X�V
		lCurRowPage++;
	}
	m_theRowPageInfoArray.Add (lTrackInfoCount - 1);
}


// �g���b�N�C���f�b�N�X�E�ܐ��ԍ���Y���W[*1/10mm]�ɕϊ�
long CMusicalScorePrintView::TrackIndexLineNotoY (long lTrackIndex, long lLineNo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	ASSERT (0 <= lTrackIndex &&lTrackIndex < pMusicalScoreFrame->GetTrackInfoCount ());
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex));
	long lTrackTopPrint = pTrackInfo->m_lTopPrint;
	long lTrackHeightPrint = pTrackInfo->m_lHeightPrint;
	long lTrackFlags = pTrackInfo->m_lFlags;
	long lKey000Y = 0;
	long ry = 8;
	switch (lTrackFlags & 0x0000000F) {
	case 1: // �g���L��
		lKey000Y = (lTrackTopPrint + lTrackHeightPrint / 2) + 41 * ry;
		break;
	case 2: // �։��L��
		lKey000Y = (lTrackTopPrint + lTrackHeightPrint / 2) + 29 * ry;
		break;
	case 3: // �啈�\
		lKey000Y = (lTrackTopPrint + lTrackHeightPrint / 2) + 35 * ry;
		break;
	default:
		lKey000Y = (lTrackTopPrint + lTrackHeightPrint / 2) + 35 * ry;
		break;
	}

	long lTopMargin = MUSICALSCOREPRINTVIEW_TOPMARGIN;
	long lBottomMargin = MUSICALSCOREPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = MUSICALSCOREPRINTVIEW_SCALEHEIGHT;
	long lTrackHeight = (m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin);
	
	return lTrackHeight * m_lMaxRowPage - (lKey000Y - lLineNo * ry);
}

// �g���b�N�C���f�b�N�X(0�`65535)�E�m�[�g�L�[(0�`127)��y���W[*1/10mm]�ɕϊ�
long CMusicalScorePrintView::TrackIndexKeytoY (long lTrackIndex, long lKey, long lKeySignature) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	long lLineNo = pMusicalScoreFrame->KeytoLineNo (lKey, lKeySignature);
	return TrackIndexLineNotoY (lTrackIndex, lLineNo);
}

// �^�C��[Tick]��x���W[*1/10mm]�ɕϊ�
long CMusicalScorePrintView::TimetoX (long lTime) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lMeasureCount = pMusicalScoreFrame->GetMeasureInfoCount ();
	long lMeasure, lBeat, lTick;
	MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
	ASSERT (0 <= lMeasure && lMeasure < lMeasureCount);
	long j;
	for (j = 0; j < lMeasureCount - 1; j++) {
		if (pMusicalScoreFrame->GetMeasureTime (j + 1) > lTime) {
			break;
		}
	}
	long lMeasureTime = pMusicalScoreFrame->GetMeasureTime (j);
	long lDeltaTime = lTime - lMeasureTime;
	ASSERT (lDeltaTime >= 0);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (j));

	return 
		pMeasureInfo->m_lLeftPrint + 
		pMeasureInfo->m_lSignatureWidthPrint +
		pMeasureInfo->m_lPreWidthPrint +
 		pMeasureInfo->m_lWidthPrint * lDeltaTime / pMeasureInfo->m_lDuration;
}

// ���߂�x���W[pixel]�ɕϊ�(���ߋ��E���ʒu)
long CMusicalScorePrintView::MeasuretoX (long lMeasure) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	long lMeasureInfoCount = pMusicalScoreFrame->GetMeasureInfoCount ();
	ASSERT (0 <= lMeasure && lMeasure < lMeasureInfoCount);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lMeasure));
	return pMeasureInfo->m_lLeftPrint;
}

// ���߂�x���W[pixel]�ɕϊ�(���ߋ��E��+���q�����L���ʒu)
long CMusicalScorePrintView::MeasuretoX2 (long lMeasure) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	long lMeasureInfoCount = pMusicalScoreFrame->GetMeasureInfoCount ();
	ASSERT (0 <= lMeasure && lMeasure < lMeasureInfoCount);
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	VERIFY (pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lMeasure));
	return (pMeasureInfo->m_lLeftPrint + pMeasureInfo->m_lSignatureWidthPrint);
}


// �����܂��Ⴍ����`�悷��
void CMusicalScorePrintView::DrawTadpole 
	(CDC* pDC, long x, long y, long rx, long ry, long lFlags) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	if (lFlags & 0x00010000) {
		pt[0].x = ptCenter.x + rx;
		pt[0].y = ptCenter.y;
		pt[1].x = ptCenter.x + rx;
		pt[1].y = ptCenter.y + ry / 2;
		pt[2].x = ptCenter.x + rx / 2;
		pt[2].y = ptCenter.y + ry;
		pt[3].x = ptCenter.x;
		pt[3].y = ptCenter.y + ry;
		pt[4].x = ptCenter.x - rx;
		pt[4].y = ptCenter.y;
		pt[5].x = ptCenter.x - rx;
		pt[5].y = ptCenter.y - ry / 2;
		pt[6].x = ptCenter.x - rx / 2;
		pt[6].y = ptCenter.y - ry;
		pt[7].x = ptCenter.x;
		pt[7].y = ptCenter.y - ry;
		pt[8].x = ptCenter.x + rx;
		pt[8].y = ptCenter.y;
		pDC->Polyline (pt, 9); // ����
		pt[0].x = ptCenter.x;
		pt[0].y = ptCenter.y + ry;
		pt[1].x = ptCenter.x - rx / 2;
		pt[1].y = ptCenter.y + ry;
		pt[2].x = ptCenter.x - rx;
		pt[2].y = ptCenter.y + ry / 2;
		pt[3].x = ptCenter.x - rx;
		pt[3].y = ptCenter.y;
		pDC->Polygon (pt, 4); // ����
		pt[0].x = ptCenter.x + rx;
		pt[0].y = ptCenter.y;
		pt[1].x = ptCenter.x + rx;
		pt[1].y = ptCenter.y - ry / 2;
		pt[2].x = ptCenter.x + rx / 2;
		pt[2].y = ptCenter.y - ry;
		pt[3].x = ptCenter.x;
		pt[3].y = ptCenter.y - ry;
		pDC->Polygon (pt, 4); // ����
	}
	else {
		pt[0].x = ptCenter.x + rx;
		pt[0].y = ptCenter.y;
		pt[1].x = ptCenter.x + rx;
		pt[1].y = ptCenter.y + ry / 2;
		pt[2].x = ptCenter.x + rx / 2;
		pt[2].y = ptCenter.y + ry;
		pt[3].x = ptCenter.x;
		pt[3].y = ptCenter.y + ry;
		pt[4].x = ptCenter.x - rx;
		pt[4].y = ptCenter.y;
		pt[5].x = ptCenter.x - rx;
		pt[5].y = ptCenter.y - ry / 2;
		pt[6].x = ptCenter.x - rx / 2;
		pt[6].y = ptCenter.y - ry;
		pt[7].x = ptCenter.x;
		pt[7].y = ptCenter.y - ry;
		pt[8].x = ptCenter.x + rx;
		pt[8].y = ptCenter.y;
		pDC->Polygon (pt, 8); // ����
	}

}



// �g���L����`�悷��
void CMusicalScorePrintView::DrawGClef 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[21];
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y - ry;
	pt[1].x = ptCenter.x - rx;
	pt[1].y = ptCenter.y;
	pt[2].x = ptCenter.x - rx;
	pt[2].y = ptCenter.y + ry;
	pt[3].x = ptCenter.x + rx;
	pt[3].y = ptCenter.y + ry;
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y - ry;
	pt[6].x = ptCenter.x + rx;
	pt[6].y = ptCenter.y - ry * 2;
	pt[7].x = ptCenter.x - rx * 1;
	pt[7].y = ptCenter.y - ry * 2;
	pt[8].x = ptCenter.x - rx * 2;
	pt[8].y = ptCenter.y - ry;
	pt[9].x = ptCenter.x - rx * 2;
	pt[9].y = ptCenter.y + ry;
	pt[10].x = ptCenter.x;
	pt[10].y = ptCenter.y + ry * 4;
	pt[11].x = ptCenter.x + rx * 1;
	pt[11].y = ptCenter.y + ry * 5;
	pt[12].x = ptCenter.x + rx * 1;
	pt[12].y = ptCenter.y + ry * 8;
	pt[13].x = ptCenter.x;
	pt[13].y = ptCenter.y + ry * 6;
	pt[14].x = ptCenter.x;
	pt[14].y = ptCenter.y + ry * 4;
	pt[15].x = ptCenter.x + rx;
	pt[15].y = ptCenter.y - ry * 4;
	pt[16].x = ptCenter.x;
	pt[16].y = ptCenter.y - ry * 5;
	pt[17].x = ptCenter.x - rx * 1 / 2;
	pt[17].y = ptCenter.y - ry * 5;
	pDC->Polyline (pt, 18);
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y; 
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y + ry; 
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y + ry; 
	pt[3].x = ptCenter.x + rx * 2;
	pt[3].y = ptCenter.y; 
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y + ry; 
	pt[5].x = ptCenter.x + rx;
	pt[5].y = ptCenter.y + ry * 2; 
	pt[6].x = ptCenter.x;
	pt[6].y = ptCenter.y + ry * 2; 
	pt[7].x = ptCenter.x - rx;
	pt[7].y = ptCenter.y + ry;
	pDC->Polygon (pt, 8);
	pt[0].x = ptCenter.x - rx * 2;
	pt[0].y = ptCenter.y + ry * 1;
	pt[1].x = ptCenter.x - rx * 2;
	pt[1].y = ptCenter.y + ry * 2;
	pt[2].x = ptCenter.x + rx * 1;
	pt[2].y = ptCenter.y + ry * 6;
	pt[3].x = ptCenter.x + rx * 1;
	pt[3].y = ptCenter.y + ry * 5;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y + ry * 6; 
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y + ry * 7; 
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y + ry * 8;
	pt[3].x = ptCenter.x + rx;
	pt[3].y = ptCenter.y + ry * 7;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x - rx * 1;
	pt[0].y = ptCenter.y - ry * 9 / 2;
	pt[1].x = ptCenter.x - rx * 1 / 2;
	pt[1].y = ptCenter.y - ry * 4;
	pt[2].x = ptCenter.x;
	pt[2].y = ptCenter.y - ry * 9 / 2;
	pt[3].x = ptCenter.x - rx * 1 / 2;
	pt[3].y = ptCenter.y - ry * 5;
	pDC->Polygon (pt, 4);
}

// �w���L����`�悷��
void CMusicalScorePrintView::DrawFClef 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x - rx * 2;
	pt[1].y = ptCenter.y;
	pt[2].x = ptCenter.x - rx * 2;
	pt[2].y = ptCenter.y + ry;
	pt[3].x = ptCenter.x - rx;
	pt[3].y = ptCenter.y + ry * 2;
	pt[4].x = ptCenter.x + rx;
	pt[4].y = ptCenter.y + ry * 2;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y + ry;
	pt[6].x = ptCenter.x + rx * 2;
	pt[6].y = ptCenter.y - ry;
	pt[7].x = ptCenter.x;
	pt[7].y = ptCenter.y - ry * 4;
	pt[8].x = ptCenter.x - rx * 2;
	pt[8].y = ptCenter.y - ry * 6;
	pDC->Polyline (pt, 9);
	pt[0].x = ptCenter.x - rx * 2;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x - rx * 3 / 2;
	pt[1].y = ptCenter.y + ry * 1 / 2;
	pt[2].x = ptCenter.x - rx;
	pt[2].y = ptCenter.y;
	pt[3].x = ptCenter.x - rx * 3 / 2;
	pt[3].y = ptCenter.y - ry * 1 / 2;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x;
	pt[0].y = ptCenter.y + ry * 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y + ry * 1;
	pt[2].x = ptCenter.x + rx;
	pt[2].y = ptCenter.y - ry * 1;
	pt[3].x = ptCenter.x;
	pt[3].y = ptCenter.y - ry * 4;
	pt[4].x = ptCenter.x + rx * 2;
	pt[4].y = ptCenter.y - ry * 1;
	pt[5].x = ptCenter.x + rx * 2;
	pt[5].y = ptCenter.y + ry * 1;
	pt[6].x = ptCenter.x + rx * 1;
	pt[6].y = ptCenter.y + ry * 2;
	pDC->Polygon (pt, 7);
	pt[0].x = ptCenter.x + rx * 3 + rx / 2;
	pt[0].y = ptCenter.y + ry * 1;
	pt[1].x = ptCenter.x + rx * 3;
	pt[1].y = ptCenter.y + ry * 1 + ry / 2;
	pt[2].x = ptCenter.x + rx * 3 - rx / 2;
	pt[2].y = ptCenter.y + ry * 1;
	pt[3].x = ptCenter.x + rx * 3;
	pt[3].y = ptCenter.y + ry * 1 - ry / 2;
	pDC->Polygon (pt, 4);
	pt[0].x = ptCenter.x + rx * 3 + rx / 2;
	pt[0].y = ptCenter.y - ry * 1;
	pt[1].x = ptCenter.x + rx * 3;
	pt[1].y = ptCenter.y - ry * 1 + ry / 2;
	pt[2].x = ptCenter.x + rx * 3 - ry / 2;
	pt[2].y = ptCenter.y - ry * 1;
	pt[3].x = ptCenter.x + rx * 3;
	pt[3].y = ptCenter.y - ry * 1 - ry / 2;
	pDC->Polygon (pt, 4);
}

// ��̕`��
void CMusicalScorePrintView::DrawFlat 
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y + ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y + ry;
	pt[3].x = ptCenter.x + rx / 2;
	pt[3].y = ptCenter.y;
	pt[4].x = ptCenter.x - rx / 2;
	pt[4].y = ptCenter.y - ry;
	pt[5].x = ptCenter.x - rx / 2;
	pt[5].y = ptCenter.y + ry * 4;
	pDC->Polyline (pt, 6);
}

// #�̕`��
void CMusicalScorePrintView::DrawSharp
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y - ry - ry / 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y - ry + ry / 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x - rx;
	pt[0].y = ptCenter.y + ry - ry / 2;
	pt[1].x = ptCenter.x + rx;
	pt[1].y = ptCenter.y + ry + ry / 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y + ry * 2;
	pt[1].x = ptCenter.x - rx / 2;
	pt[1].y = ptCenter.y - ry * 2;
	pDC->Polyline (pt, 2);
	pt[0].x = ptCenter.x + rx / 2;
	pt[0].y = ptCenter.y + ry * 2;
	pt[1].x = ptCenter.x + rx / 2;
	pt[1].y = ptCenter.y - ry * 2;
	pDC->Polyline (pt, 2);
};

// �i�`�������̕`��
void CMusicalScorePrintView::DrawNatural
	(CDC* pDC, long x, long y, long rx, long ry) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[9];
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y + ry * 2;
	pt[1].x = ptCenter.x - rx / 2;
	pt[1].y = ptCenter.y - ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y - ry * 3 / 4;
	pDC->Polyline (pt, 3);
	pt[0].x = ptCenter.x - rx / 2;
	pt[0].y = ptCenter.y + ry * 3 / 4;
	pt[1].x = ptCenter.x + rx / 2;
	pt[1].y = ptCenter.y + ry;
	pt[2].x = ptCenter.x + rx / 2;
	pt[2].y = ptCenter.y - ry * 2;
	pDC->Polyline (pt, 3);
};

// �����⏕����`�悷��
void CMusicalScorePrintView::DrawHorzAuxiliaryLine
	(CDC* pDC, long x, long y, long r, long lFlags) {
	POINT ptCenter;
	ptCenter.x = x;
	ptCenter.y = y;
	POINT pt[2];
	pt[0].x = ptCenter.x - r;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x + r;
	pt[1].y = ptCenter.y;
	pDC->Polyline (pt, 2);
}

// �t�_��`�悷��
void CMusicalScorePrintView::DrawDot
	(CDC* pDC, long x, long y, long rx, long ry, long lFlags) {
	POINT ptCenter;
	ptCenter.x = x + rx * 2;
	ptCenter.y = y + ry / 2;
	POINT pt[4];
	pt[0].x = ptCenter.x - 1;
	pt[0].y = ptCenter.y;
	pt[1].x = ptCenter.x;
	pt[1].y = ptCenter.y + 1;
	pt[2].x = ptCenter.x + 1;
	pt[2].y = ptCenter.y;
	pt[3].x = ptCenter.x;
	pt[3].y = ptCenter.y - 1;
	pDC->Polygon (pt, 4);

}

// ���^�C��`�悷��
void CMusicalScorePrintView::DrawTieHalf
	(CDC* pDC, long x1, long x2, long y, long rx, long ry, long lFlags) {
	POINT ptCenter;
	ptCenter.x;
	ptCenter.y;
	POINT pt[3];
	pt[0].x = (x2 > x1 ? x1 + rx * 1 : x1 - rx * 1);
	pt[0].y = y - ry * 1;
	pt[1].x = (x2 > x1 ? x1 + rx * 2 : x1 - rx * 2);
	pt[1].y = y - ry * 2;
	pt[2].x = x2;
	pt[2].y = y - ry * 2;
	pDC->Polyline (pt, 3);

}

// �c�_��`�悷��
void CMusicalScorePrintView::DrawPole (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[2];
	pt[0].x = x1 + rx;
	pt[0].y = y1;
	pt[1].x = x1 + rx;
	pt[1].y = y2;
	pDC->Polyline (pt, 2);
}

// �P�����`�悷��
void CMusicalScorePrintView::DrawSingleFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[4];
	pt[0].x = x1 + rx;
	pt[0].y = y2;
	pt[1].x = x1 + rx * 3;
	pt[1].y = y2 - ry * 2;
	pt[2].x = x1 + rx * 3;
	pt[2].y = y2 - ry * 3;
	pt[3].x = x1 + rx;
	pt[3].y = y2 - ry;
	pDC->Polygon (pt, 4);
	pt[0].x = x1 + rx * 3;
	pt[0].y = y2 - ry * 3;
	pt[1].x = x1 + rx;
	pt[1].y = y2 - ry * 5;
	pDC->Polyline (pt, 2);
}

// �A������`�悷��
void CMusicalScorePrintView::DrawChainedFlag (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[4];
	pt[0].x = x1 + rx;
	pt[0].y = y2;
	pt[1].x = x2 + rx;
	pt[1].y = y2;
	pt[2].x = x2 + rx;
	pt[2].y = y2 - ry;
	pt[3].x = x1 + rx;
	pt[3].y = y2 - ry;
	pDC->Polygon (pt, 4);
}

// 3�A������`�悷��(20110905�ǉ�)
void CMusicalScorePrintView::DrawTripletSign (CDC* pDC, long x1, long x2, long y1, long y2, long rx, long ry, long lFlags) {
	POINT pt[3];
	pt[0].x = x1;
	pt[0].y = y2;
	pt[1].x = x1;
	pt[1].y = y1;
	pt[2].x = (x1 + x2) / 2 - 15;
	pt[2].y = y1;
	pDC->Polyline (pt, 3);
	pt[0].x = (x1 + x2) / 2 + 15;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y1;
	pt[2].x = x2;
	pt[2].y = y2;
	pDC->Polyline (pt, 3);
	CRect rcText ((x1 + x2) / 2 - 15, y1 - 15, (x1 + x2) / 2 + 15, y1 + 15);
	CString strText;
	strText.Format (_T("%d"), (lFlags & 0x000000FF));
	pDC->DrawText (strText, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// ������`�悷��
void CMusicalScorePrintView::DrawNote
	(CDC* pDC, long lTrackIndex, MusicalScoreNoteInfo* pNoteInfo, long lFlags) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIEvent* pNoteEvent = pNoteInfo->m_pNoteOnEvent;
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	ASSERT (0 <= lTrackIndex && lTrackIndex < pMusicalScoreFrame->GetTrackInfoCount ());
	VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex));
	long lTrackFlags = pTrackInfo->m_lFlags;
	long lKey = MIDIEvent_GetKey (pNoteEvent);
	long lDuration = pNoteInfo->m_lNoteOffTime - pNoteInfo->m_lNoteOnTime;
	long lNoteOnMeasure = pNoteInfo->m_lNoteOnMeasure;
	MusicalScoreMeasureInfo* pMeasureInfo = NULL;
	ASSERT (0 <= lNoteOnMeasure && lNoteOnMeasure < pMusicalScoreFrame->GetMeasureInfoCount ());
	VERIFY (pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lNoteOnMeasure));
	long lKeySignature = pMeasureInfo->m_lKeySignature; // ���̏��߂̒����L��
	long lTimeSignature = pMeasureInfo->m_lTimeSignature; // ���̏��߂̔��q�L��
	long lLineNo = pMusicalScoreFrame->KeytoLineNo (lKey, lKeySignature);
	long lSF = pMusicalScoreFrame->KeytoSF (lKey, lKeySignature);
	long lnn = lTimeSignature & 0xFF;
	long ldd = (lTimeSignature & 0xFF00) >> 8;

	long x = this->TimetoX (pNoteInfo->m_lNoteOnTime);
	long y = 0;
	long rx = 8;
	long ry = 8;
	long i = 0;

	// �����⏕���̕`��
	switch (lTrackFlags & 0x0000000F) {
	case 1: // �g���L��
		if (lLineNo >= 47) {
			for (i = 47; i <= lLineNo; i += 2) {
				y = this->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		else if (lLineNo <= 35) {
			for (i = 35; i >= lLineNo; i -= 2) {
				y = this->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		break;
	case 2: // �w���L��
		if (lLineNo >= 35) {
			for (i = 35; i <= lLineNo; i += 2) {
				y = this->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		else if (lLineNo <= 23) {
			for (i = 23; i >= lLineNo; i -= 2) {
				y = this->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		break;
	case 3: // �啈�\
		if (lLineNo >= 47) {
			for (i = 47; i <= lLineNo; i += 2) {
				y = this->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		else if (lLineNo == 35) {
			y = this->TrackIndexLineNotoY (lTrackIndex, 35);
			pDC->MoveTo (x - rx * 2, y);
			pDC->LineTo (x + rx * 2, y);
		}
		else if (lLineNo <= 23) {
			for (i = 23; i >= lLineNo; i -= 2) {
				y = this->TrackIndexLineNotoY (lTrackIndex, i);
				pDC->MoveTo (x - rx * 2, y);
				pDC->LineTo (x + rx * 2, y);
			}
		}
		break;
	}
	// �Վ��L���̕`��
	y = this->TrackIndexLineNotoY (lTrackIndex, lLineNo);
	switch (lSF) {
	case 0:
		break;
	case 2:
		DrawFlat (pDC, x - 4 * rx, y, rx, ry);
		DrawFlat (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 3:
		DrawFlat (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 4:
		DrawNatural (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 5:
		DrawSharp (pDC, x - 2 * rx, y, rx, ry);
		break;
	case 6:
		DrawSharp (pDC, x - 2 * rx, y, rx, ry);
		DrawSharp (pDC, x - 4 * rx, y, rx, ry);
		break;
	}

	
	// �����܂��Ⴍ���̕`��
	y = this->TrackIndexLineNotoY (lTrackIndex, lLineNo);
	long lFlags2 = 0;
	if (lDuration >= lTimeResolution * 2) {
		lFlags2 |= 0x00010000; // ����
	}
	if (lDuration < lTimeResolution * lnn * 4 / ldd) {
		lFlags2 |= 0x00100000; // �c������
	}
	DrawTadpole (pDC, x, y, rx, ry, lFlags2);

	// �t�_�̕`��
	if (lDuration == lTimeResolution * 3 ||
		lDuration == lTimeResolution * 3 / 2 ||
		lDuration == lTimeResolution * 3 / 4 ||
		lDuration == lTimeResolution * 3 / 8) {
		DrawDot (pDC, x, y, rx, ry, lFlags2);
	}

	// ���^�C�̕`��
	long x2 = 0;
	long lPreWidth = 0;
	long lPostWidth = 0;
	switch (pNoteInfo->m_lFlags & 0x0000000F) {
	case 1: // ��_
		x2 = this->TimetoX (pNoteInfo->m_lNoteOffTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 2: // _��
		x2 = this->TimetoX (pNoteInfo->m_lNoteOnTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 3: // _��_
		x2 = this->TimetoX (pNoteInfo->m_lNoteOffTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = this->TimetoX (pNoteInfo->m_lNoteOnTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 4: // ��_|
		x2 = this->MeasuretoX (pNoteInfo->m_lNoteOffMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 5: // |_��
		x2 = this->MeasuretoX (pNoteInfo->m_lNoteOnMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 6: // _��_|
		x2 = this->TimetoX (pNoteInfo->m_lNoteOnTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = this->MeasuretoX (pNoteInfo->m_lNoteOffMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 7: // |_��_
		x2 = this->MeasuretoX (pNoteInfo->m_lNoteOnMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = this->TimetoX (pNoteInfo->m_lNoteOffTime - 15);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	case 8: // |_�S����_|
		x2 = this->MeasuretoX (pNoteInfo->m_lNoteOnMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		x2 = this->MeasuretoX (pNoteInfo->m_lNoteOffMeasure);
		DrawTieHalf (pDC, x, x2, y, rx, ry, lFlags2);
		break;
	}

	// �_�̕`��
	long lPoleKey = lKey;
	MusicalScoreNoteGroupInfo* pNoteGroupInfo = (MusicalScoreNoteGroupInfo*)(pNoteInfo->m_pNoteGroupInfo);
	if (pNoteGroupInfo) { // �����̏ꍇ
		lPoleKey = pNoteGroupInfo->m_lMaxKey;
	}
	long lLineNo2 = pMusicalScoreFrame->KeytoLineNo (lPoleKey, lKeySignature);
	long y2 = this->TrackIndexLineNotoY (lTrackIndex, lLineNo2);
	if (lDuration < lTimeResolution * 4) {
		DrawPole (pDC, x, x2, y, y2 + ry * 7, rx, ry, lFlags2);
	}

	// ���̕`��
	if (lDuration < lTimeResolution && lDuration != lTimeResolution * 2 / 3) {
		// �O���[�v���̉����̎������P��̏ꍇ
		if (pNoteGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime == pNoteGroupInfo->m_pLastNoteInfo->m_lNoteOnTime) {
			// 8�������A�t�_8�������A3�A8�������A16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 2 ||
				lDuration == lTimeResolution * 3 / 4 ||
				lDuration == lTimeResolution / 3 ||
				lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawSingleFlag (pDC, x, x2, y, y2 + ry * 7, rx, ry, lFlags2);
			}
			// 16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawSingleFlag (pDC, x, x2, y, y2 + ry * 5, rx, ry, lFlags2);
			}
			// 32�������A3�A32������
			if (lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawSingleFlag (pDC, x, x2, y, y2 + ry * 3, rx, ry, lFlags2);
			}
		}
		// �O���[�v���̉����������̎����ɂ܂�����ꍇ
		else {
			long x1[3]; // �A�������W[1�i��,2�i��,3�i��]
			long x2[3]; // �A���E���W[1�i��,2�i��,3�i��]
			// �O���[�v���̍ŏ��̎����̉����ł���ꍇ
			if (pNoteGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime == pNoteInfo->m_lNoteOnTime) {
				x1[0] = x1[1] = x1[2] = x;
				x2[0] = x2[1] = x2[2] = this->TimetoX (pNoteInfo->m_lNoteOffTime - lTimeResolution / 8);
			}
			// �O���[�v���̍Ō�̎����̉����ł���ꍇ
			else if (pNoteGroupInfo->m_pLastNoteInfo->m_lNoteOnTime == pNoteInfo->m_lNoteOnTime) {
				x1[0] = x1[1] = x1[2] = this->TimetoX (pNoteInfo->m_lNoteOnTime - lTimeResolution / 8);
				x2[0] = x2[1] = x2[2] = x;
			}
			// �O���[�v���̓r���̎����̉����ł���ꍇ
			else {
				x1[0] = x1[1] = x1[2] = this->TimetoX (pNoteInfo->m_lNoteOnTime - lTimeResolution / 8);
				x2[0] = x2[1] = x2[2] = this->TimetoX (pNoteInfo->m_lNoteOffTime - lTimeResolution / 8);
				// �O�̎����̉������擾
				MusicalScoreNoteInfo* pPrevNoteInfo = pNoteInfo;
				long lPrevDuration = 0;
				while (pPrevNoteInfo) {
					if (pPrevNoteInfo->m_lNoteOnTime < pNoteInfo->m_lNoteOnTime) {
						break;
					}
					pPrevNoteInfo = pPrevNoteInfo->m_pPrevNoteInfo;
				}
				if (pPrevNoteInfo && pPrevNoteInfo->m_pNoteGroupInfo == pNoteGroupInfo) {
					lPrevDuration = pPrevNoteInfo->m_lNoteOffTime - pPrevNoteInfo->m_lNoteOnTime;
				}
				// ���̎����̉������擾
				MusicalScoreNoteInfo* pNextNoteInfo = pNoteInfo;
				long lNextDuration = 0;
				while (pNextNoteInfo) {
					if (pNextNoteInfo->m_lNoteOnTime > pNoteInfo->m_lNoteOnTime) {
						break;
					}
					pNextNoteInfo = pNextNoteInfo->m_pNextNoteInfo;
				}
				if (pNextNoteInfo && pNextNoteInfo->m_pNoteGroupInfo == pNoteGroupInfo) {
					lNextDuration = pNextNoteInfo->m_lNoteOffTime - pNextNoteInfo->m_lNoteOnTime;
				}
				// 1�i�ڂ̘A��������
				if (lDuration == lTimeResolution / 2 ||
					lDuration == lTimeResolution * 3 / 4 ||
					lDuration == lTimeResolution / 3 ||
					lDuration == lTimeResolution / 4 ||
					lDuration == lTimeResolution * 3 / 8 ||
					lDuration == lTimeResolution / 6 ||
					lDuration == lTimeResolution / 8 ||
					lDuration == lTimeResolution / 12) {
					if (lNextDuration == 0) {
						x2[0] = x;	
					}
					else if (lPrevDuration == 0) {
						x1[0] = x;
					}
				}
				// 2�i�ڂ̘A��������
				if (lDuration == lTimeResolution / 4 ||
					lDuration == lTimeResolution * 3 / 8 ||
					lDuration == lTimeResolution / 6 ||
					lDuration == lTimeResolution / 8 ||
					lDuration == lTimeResolution / 12) {
					if (lNextDuration >= lDuration * 2) {
						x2[1] = x;	
					}
					else if (lPrevDuration >= lDuration * 2) {
						x1[1] = x;
					}
				}
				// 3�i�ڂ̘A��������
				if (lDuration == lTimeResolution / 8 ||
					lDuration == lTimeResolution / 12) {
					if (lNextDuration >= lDuration * 2) {
						x2[2] = x;
					}
					else if (lPrevDuration >= lDuration * 2) {
						x1[2] = x;
					}
				}
			}
			// �A��1�i��
			// 8�������A�t�_8�������A3�A8�������A16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 2 ||
				lDuration == lTimeResolution * 3 / 4 ||
				lDuration == lTimeResolution / 3 ||
				lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawChainedFlag (pDC, x1[0], x2[0], y, y2 + ry * 7, rx, ry, lFlags2);
			}
			// �A��2�i�ڕ`��
			// 16�������A�t�_16�������A3�A16�������A32�������A3�A32������
			if (lDuration == lTimeResolution / 4 ||
				lDuration == lTimeResolution * 3 / 8 ||
				lDuration == lTimeResolution / 6 ||
				lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawChainedFlag (pDC, x1[1], x2[1], y, y2 + ry * 5, rx, ry, lFlags2);
			}
			// �A��3�i�ڕ`��
			// 32�������A3�A32������
			if (lDuration == lTimeResolution / 8 ||
				lDuration == lTimeResolution / 12) {
				DrawChainedFlag (pDC, x1[2], x2[2], y, y2 + ry * 3, rx, ry, lFlags2);
			}
		}
	}

	// 3�A�����̕`��(20110905�ǉ�)
	if (pNoteInfo->m_pTripletGroupInfo) {
		MusicalScoreTripletGroupInfo* pTripletGroupInfo = 
			(MusicalScoreTripletGroupInfo*)(pNoteInfo->m_pTripletGroupInfo);
		if (pTripletGroupInfo->m_pFirstNoteInfo == pNoteInfo) {
			long x1 = this->TimetoX (pTripletGroupInfo->m_pFirstNoteInfo->m_lNoteOnTime);
			long x2 = this->TimetoX (pTripletGroupInfo->m_pLastNoteInfo->m_lNoteOnTime);
			long lMinLineNo = pMusicalScoreFrame->KeytoLineNo (pTripletGroupInfo->m_lMinKey, lKeySignature);
			long y = this->TrackIndexLineNotoY (lTrackIndex, lMinLineNo);
			long n12 = 12 * lTimeResolution / pTripletGroupInfo->m_lMinDur; // 12 * 3 or 12 * 6 or 12 * 12
			long lSpan = pTripletGroupInfo->m_lEndTime - pTripletGroupInfo->m_lBeginTime;
			if (lSpan >= 1) {
				if (12 * lTimeResolution / lSpan >= 1) {
					n12 /= (12 * lTimeResolution / lSpan);
				}
				else {
					n12 /= 12;
				}
			}
			else {
				n12 /= 12;
			}
			DrawTripletSign (pDC, x1, x2, y - 4 * ry, y - 3 * ry, rx, ry, n12);
		}
	}

}



// ���q�L���E�����L���̕`��
void CMusicalScorePrintView::DrawTimeAndKeySignature 
	(CDC* pDC, long lTrackIndex, long lTime) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	ASSERT (0 <= lTrackIndex && lTrackIndex < pMusicalScoreFrame->GetTrackInfoCount ());
	MusicalScoreTrackInfo* pTrackInfo = NULL;
	VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackIndex));
	long lTrackFlags = pTrackInfo->m_lFlags;
	long lMeasure, lBeat, lTick;
	long x;
	if (lTime >= 0) {
		MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
		x =  this->MeasuretoX (lMeasure);
	}
	else {
		x = 120;
	}
	long y;
	long rx = 8;
	long ry = 8;
	long lGCrefSharpLineNo[7] = {45, 42, 46, 43, 40, 44, 41};
	long lFCrefSharpLineNo[7] = {31, 28, 32, 29, 33, 30, 34};
	long lGCrefFlatLineNo[7] = {41, 44, 40, 43, 39, 42, 38};
	long lFCrefFlatLineNo[7] = {27, 30, 26, 29, 25, 28, 24};
	long lsf, lmi;
	MIDIData_FindKeySignature (pMIDIData, lTime, &lsf, &lmi);
	long lnn, ldd, lcc, lbb;
	MIDIData_FindTimeSignature (pMIDIData, lTime, &lnn, &ldd, &lcc, &lbb);
	CString strText1;
	CString strText2;
	strText1.Format (_T("%d"), lnn);
	strText2.Format (_T("%d"), (1 << ldd));
	CRect rcText1 (x + rx * 10 + rx * 2 * abs(lsf), 0, x + rx * 10 + rx * 2 * abs(lsf) + 24, 0);
	CRect rcText2 (x + rx * 10 + rx * 2 * abs(lsf), 0, x + rx * 10 + rx * 2 * abs(lsf) + 24, 0);
	long j;
	switch (lTrackFlags & 0x0000000F) {
	case 1: // �g���L��
		y = this->TrackIndexLineNotoY (lTrackIndex, 39);
		DrawGClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lGCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lGCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = this->TrackIndexLineNotoY (lTrackIndex, 41);
		rcText1.top = y + ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y - ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	case 2: // �w���L��
		y = this->TrackIndexLineNotoY (lTrackIndex, 31);
		DrawFClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lFCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lFCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = this->TrackIndexLineNotoY (lTrackIndex, 29);
		rcText1.top = y + ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y - ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	case 3: // �啈�\
		y = this->TrackIndexLineNotoY (lTrackIndex, 39);
		DrawGClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lGCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lGCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = this->TrackIndexLineNotoY (lTrackIndex, 41);
		rcText1.top = y + ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y - ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		y = this->TrackIndexLineNotoY (lTrackIndex, 31);
		DrawFClef (pDC, x + rx * 4, y, rx, ry);
		if (lsf > 0) {
			for (j = 1; j <= lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lFCrefSharpLineNo[j - 1]);
				DrawSharp (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		else if (lsf < 0) {
			for (j = 1; j <= -lsf; j++) {
				y = this->TrackIndexLineNotoY (lTrackIndex, lFCrefFlatLineNo[j - 1]);
				DrawFlat (pDC, x + rx * 8 + rx * 2 * j, y, rx, ry);
			}
		}
		y = this->TrackIndexLineNotoY (lTrackIndex, 29);
		rcText1.top = y + ry * 4;
		rcText1.bottom = y;
		pDC->DrawText (strText1, &rcText1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rcText2.top = y;
		rcText2.bottom = y - ry * 4;
		pDC->DrawText (strText2, &rcText2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	}
}




// �w�莞���Ƀt���O�ƃe�L�X�g��`��
void CMusicalScorePrintView::DrawFlagAndText 
(CDC* pDC, long lTime, LPCTSTR lpszText, long lColor) {
	long x = TimetoX (lTime);
	CRect rcBack (x, MUSICALSCOREPRINTVIEW_SCALEHEIGHT / 2, x + 1024, MUSICALSCOREPRINTVIEW_SCALEHEIGHT);
	CRect rcFlag (x, MUSICALSCOREPRINTVIEW_SCALEHEIGHT * 6 / 10, x + 20, MUSICALSCOREPRINTVIEW_SCALEHEIGHT * 9 / 10);
	CRect rcText (x + 30, MUSICALSCOREPRINTVIEW_SCALEHEIGHT / 2, x + 1024, MUSICALSCOREPRINTVIEW_SCALEHEIGHT);
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, lColor);
	CPen* pOldPen = pDC->SelectObject (&pen);
	pDC->FillSolidRect (&rcBack, ::GetSysColor (COLOR_BTNFACE));
	pDC->FillSolidRect (&rcFlag, lColor);
	pDC->MoveTo (x, MUSICALSCOREPRINTVIEW_SCALEHEIGHT * 5 / 10);
	pDC->LineTo (x, MUSICALSCOREPRINTVIEW_SCALEHEIGHT * 9 / 10);
	pDC->SetBkMode (TRANSPARENT);
	long lOldColor = pDC->GetTextColor ();
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	pDC->SetTextColor (lColor);
	pDC->DrawText (lpszText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->SetTextColor (lOldColor);
	pDC->SelectObject (pOldFont);
	pDC->SelectObject (pOldPen);
}


// �g���b�N�ԍ����`��
void CMusicalScorePrintView::DrawTrackScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pTempTrack = NULL;
	pSekaijuDoc->m_theCriticalSection.Lock ();

	long lTopMargin = MUSICALSCOREPRINTVIEW_TOPMARGIN;
	long lBottomMargin = MUSICALSCOREPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = MUSICALSCOREPRINTVIEW_SCALEHEIGHT;
	long lTrackHeight = (m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin);

	CFont* pOldFont = NULL;
	pDC->SetBkMode (TRANSPARENT);
	long lVisibleTopTrack = 0;
	long lVisibleBottomTrack = MIDIData_CountTrack (pMIDIData);
	long rx = 8;
	long ry = 8;
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	long i = 0;

	// �{�^�����`��
	i = 0;
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	pOldFont = pDC->SelectObject (&(this->m_theFont));
	forEachTrack (pMIDIData, pTempTrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long x1 = 0;
				long x2 = 100;
				long y1 = lTrackHeight * m_lMaxRowPage - pTrackInfo->m_lTopPrint;
				long y2 = lTrackHeight * m_lMaxRowPage - (pTrackInfo->m_lTopPrint + pTrackInfo->m_lHeightPrint);
				pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
				pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
				pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
				pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
				pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
				CRect rcText (x1, y1, x2, y2);
				TCHAR szBuf[256];
				memset (szBuf, 0, sizeof (szBuf));
				_sntprintf (szBuf, 255, _T("%d"), i + (bTrackZeroOrigin ? 0 : 1));
				pDC->DrawText (szBuf, -1, &rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);
	
	
	// �ܐ��`��	
	i = 0;
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen* pOldPen = pDC->SelectObject (&penKey);
	forEachTrack (pMIDIData, pTempTrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lTrackTopPrint = pTrackInfo->m_lTopPrint;
				long lTrackHeightPrint = pTrackInfo->m_lHeightPrint;
				long lTrackFlags  = pTrackInfo->m_lFlags;
				long yc = (lTrackTopPrint + lTrackHeightPrint / 2);
				long ii;
				long y;
				switch (lTrackFlags & 0x0000000F) {
				case 0:
					break;
				case 1: // �g���L��
				case 2: // �w���L��
					for (ii = -2; ii <= 2; ii++) {
						y = lTrackHeight * m_lMaxRowPage - (yc + ii * 2 * ry);
						pDC->MoveTo (120, y);
						pDC->LineTo (MUSICALSCOREPRINTVIEW_SCALEWIDTH, y);
					}
					break;
				case 3: // �啈�\
					for (ii = -5; ii <= -1; ii++) {
						y = lTrackHeight * m_lMaxRowPage - (yc + ii * 2 * ry);
						pDC->MoveTo (120, y);
						pDC->LineTo (MUSICALSCOREPRINTVIEW_SCALEWIDTH, y);
					}
					for (ii = 1; ii <= 5; ii++) {
						y = lTrackHeight * m_lMaxRowPage - (yc + ii * 2 * ry);
						pDC->MoveTo (120, y);
						pDC->LineTo (MUSICALSCOREPRINTVIEW_SCALEWIDTH, y);
					}
					break;
				}
				// �啈�\�c���`��
				long y1, y2;
				switch (lTrackFlags & 0x0000000F) {
				case 3:
					y1 = lTrackHeight * m_lMaxRowPage - (yc - 10 * ry);
					y2 = lTrackHeight * m_lMaxRowPage - (yc + 10 * ry);
					pDC->FillSolidRect
						(120 - 2, y1, 2, y2 - y1 + 1, RGB (192, 192, 192));
					pDC->FillSolidRect
						(120 - 10, y1, 5, y2 - y1 + 1, RGB (192, 192, 192));
				}
			}
		}
		i++;
	}
	pDC->SelectObject (pOldPen);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ���ʃ��[�����`��
void CMusicalScorePrintView::DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = lCurPage % m_lMaxRowPage;
	long lCurRollPage = lCurPage / m_lMaxRowPage;

	CPen penMeasure (PS_SOLID, 1, RGB (128, 128, 255));
	CPen penBeat (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penKey (PS_SOLID, 1, RGB (192, 192, 192));
	CPen penOctave (PS_SOLID, 1, RGB (128, 128, 255));

	CFont* pOldFont = NULL;
	pDC->SetBkMode (TRANSPARENT);
	long lTopMargin = MUSICALSCOREPRINTVIEW_TOPMARGIN;
	long lBottomMargin = MUSICALSCOREPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = MUSICALSCOREPRINTVIEW_SCALEHEIGHT;
	long lTrackHeight = m_sizLogPaper.cy - lTopMargin - lBottomMargin - lScaleHeight;
	long x, y, lTime;
	long lVisibleLeftMeasure = m_theRollPageInfoArray.GetAt (lCurRollPage);
	long lVisibleRightMeasure = m_theRollPageInfoArray.GetAt (lCurRollPage + 1);
	long lVisibleLeftTime = 0;
	long lVisibleRightTime = 0;
	MIDIData_MakeTime (pMIDIData, lVisibleLeftMeasure, 0, 0, &lVisibleLeftTime);
	MIDIData_MakeTime (pMIDIData, lVisibleRightMeasure, 0, 0, &lVisibleRightTime);
	long lVisibleTopTrack = m_theRowPageInfoArray.GetAt (lCurRowPage);
	long lVisibleBottomTrack = m_theRowPageInfoArray.GetAt (lCurRowPage + 1);
	long lnn, ldd, lcc, lbb;
	long lUnitTick;

	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	long i, j;

	//�g���b�N���`��
	i = 0;
	pOldFont = pDC->SelectObject (&this->m_theFont);
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) { 
				MusicalScoreMeasureInfo* pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lVisibleLeftMeasure);
				MusicalScoreTrackInfo* pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i);
				long lColor = MIDITrack_GetForeColor (pMIDITrack);
				pDC->SetTextColor (lColor);
				CString strTrackName;
				strTrackName = GetCellString (i);
				CRect rcTrackName 
					(pMeasureInfo->m_lLeftPrint, 
					lTrackHeight * m_lMaxRowPage - pTrackInfo->m_lTopPrint, 
					pMeasureInfo->m_lLeftPrint + 1000,
					lTrackHeight * m_lMaxRowPage - (pTrackInfo->m_lTopPrint + pTrackInfo->m_lHeightPrint));
				pDC->DrawText (strTrackName, &rcTrackName, DT_TOP | DT_LEFT);
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);

	// �ܐ��̕`��
	CPen* pOldPen = pDC->SelectObject (&penKey);
	long lMeasureInfoCount = pMusicalScoreFrame->GetMeasureInfoCount ();
	x = this->MeasuretoX (lMeasureInfoCount - 1);
	long rx = 8;
	long ry = 8;
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) {
				ASSERT (0 <= i && i < pMusicalScoreFrame->GetTrackInfoCount ()); 
				MusicalScoreTrackInfo* pTrackInfo = NULL;
				VERIFY (pTrackInfo = pMusicalScoreFrame->GetTrackInfo (i));
				long lTrackTopPrint = pTrackInfo->m_lTopPrint;
				long lTrackHeightPrint = pTrackInfo->m_lHeightPrint;
				long lTrackFlags = pTrackInfo->m_lFlags;
				long yc = (lTrackTopPrint + lTrackHeightPrint / 2);
				long ii;
				switch (lTrackFlags & 0x0000000F) {
				case 0: // �\�����Ȃ�
					break;
				case 1: // �g���L��
				case 2: // �w���L��
					for (ii = -2; ii <= 2; ii++) {
						y = lTrackHeight * m_lMaxRowPage - (yc + ii * 2 * ry);
						pDC->MoveTo (0, y);
						pDC->LineTo (x, y);
					}
					break;
				case 3: // �啈�\
					for (ii = -5; ii <= -1; ii++) {
						y = lTrackHeight * m_lMaxRowPage - (yc + ii * 2 * ry);
						pDC->MoveTo (0, y);
						pDC->LineTo (x, y);
					}
					for (ii = 1; ii <= 5; ii++) {
						y = lTrackHeight * m_lMaxRowPage - (yc + ii * 2 * ry);
						pDC->MoveTo (0, y);
						pDC->LineTo (x, y);
					}
					break;
				}
			}
		}
		i++;
	}

	// �c���̕`��
	long lTrackInfoCount = pMusicalScoreFrame->GetTrackInfoCount ();
	ASSERT (1 <= lTrackInfoCount && lTrackInfoCount < MAXMIDITRACKNUM);
	MusicalScoreTrackInfo* pLastTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackInfoCount - 1);
	long lTrackBottom = (pLastTrackInfo->m_lTopPrint + pLastTrackInfo->m_lHeightPrint);
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		for (i = lVisibleLeftMeasure; i <= lVisibleRightMeasure; i++) {
			// ���ߐ��̕`��
			MIDIData_MakeTimeEx (pMIDIData, i, 0, 0, &lTime, &lnn, &ldd, &lcc, &lbb);
			x = this->MeasuretoX (i);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, lTrackHeight * m_lMaxRowPage - 0);
			pDC->LineTo (x, lTrackHeight * m_lMaxRowPage - lTrackBottom);
			// �����̕`��
			pDC->SelectObject (&penBeat);
			lUnitTick = lTimeResolution * 4 / (1 << ldd);
			for (long j = 0; j < lnn; j++) {
				x = this->TimetoX (lTime + j * lUnitTick);
				pDC->MoveTo (x, lTrackHeight * m_lMaxRowPage - 0);
				pDC->LineTo (x, lTrackHeight * m_lMaxRowPage - lTrackBottom);
			}
		}
	}
	else {
		for (i = lVisibleLeftMeasure; i <= lVisibleRightMeasure; i++) {
			// �t���[�����E���̕`��
			lTime = i * lTimeResolution;
			x = this->TimetoX (lTime);
			pDC->SelectObject (&penMeasure);
			pDC->MoveTo (x, 0);
			pDC->LineTo (x, lTrackHeight);
		}
		pDC->SelectObject (pOldPen);
	}

	// ���q�L���E�����L���̕`��(�y�[�W�擪���ߗp)
	CPen theTrackPen;
	CBrush theTrackBrush;
//	pOldFont = pDC->SelectObject (&(pMusicalScoreFrame->m_theTimeMeasureFont));
	long lMeasure = 0;
	for (lMeasure = lVisibleLeftMeasure; lMeasure < lVisibleRightMeasure; lMeasure++) {
		MusicalScoreMeasureInfo* pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lMeasure);
		// �Y�����߂̔��q�����L���X�y�[�X������ꍇ
		if (pMeasureInfo->m_lSignatureWidthPrint > 0) {
			i = 0;
			MIDITrack* pTempTrack;
			MIDIData_MakeTime (pMIDIData, lMeasure, 0, 0, &lTime);
			forEachTrack (pMIDIData, pTempTrack) {
				if (pMusicalScoreFrame->IsTrackVisible (i)) {
					long lColor = MIDITrack_GetForeColor (pTempTrack);
					pDC->SetTextColor (lColor);
					theTrackPen.CreatePen (PS_SOLID, 1, lColor);
					theTrackBrush.CreateSolidBrush (lColor);
					CPen* pOldPen = pDC->SelectObject (&theTrackPen);
					CBrush* pOldBrush = pDC->SelectObject (&theTrackBrush);
					DrawTimeAndKeySignature (pDC, i, lTime);
					pDC->SelectObject (pOldPen);
					pDC->SelectObject (pOldBrush);
					theTrackPen.DeleteObject ();
					theTrackBrush.DeleteObject ();
				}
				i++;
			}
		}
	}

	// ���q�L���E�����L���̕`��
	pMIDITrack = pMIDIData->m_pFirstTrack;
	if (pMIDITrack) {
		forEachEvent (pMIDITrack, pMIDIEvent) {
			long lTime = MIDIEvent_GetTime (pMIDIEvent);
			if (MIDIEvent_IsTimeSignature (pMIDIEvent) ||
				MIDIEvent_IsKeySignature (pMIDIEvent)) {
				if (lVisibleLeftTime <= lTime && lTime <= lVisibleRightTime) {
					long lMeasure, lBeat, lTick;
					MIDIData_BreakTime (pMIDIData, lTime, &lMeasure, &lBeat, &lTick);
					long lMeasureInfoCount = pMusicalScoreFrame->GetMeasureInfoCount ();
					ASSERT (0 <= lMeasure && lMeasure < lMeasureInfoCount);
					MusicalScoreMeasureInfo* pMeasureInfo = pMusicalScoreFrame->GetMeasureInfo (lMeasure);
					i = 0;
					MIDITrack* pTempTrack;
					forEachTrack (pMIDIData, pTempTrack) {
						if (pMusicalScoreFrame->IsTrackVisible (i)) {
							long lColor = MIDITrack_GetForeColor (pTempTrack);
							pDC->SetTextColor (lColor);
							theTrackPen.CreatePen (PS_SOLID, 1, lColor);
							theTrackBrush.CreateSolidBrush (lColor);
							CPen* pOldPen = pDC->SelectObject (&theTrackPen);
							CBrush* pOldBrush = pDC->SelectObject (&theTrackBrush);
							DrawTimeAndKeySignature (pDC, i, lTime);
							pDC->SelectObject (pOldPen);
							pDC->SelectObject (pOldBrush);
							theTrackPen.DeleteObject ();
							theTrackBrush.DeleteObject ();
						}
						i++;
					}
				}
			}
		}
	}
//	pDC->SelectObject (pOldFont);
	
	// �����̕`��
	CPen penBar (PS_SOLID, 1, RGB (0, 0, 0));
	pDC->SelectObject (&penBar);
	pOldFont = pDC->SelectObject (&(this->m_theFont));
	i = 0;
	forEachTrack (pMIDIData, pMIDITrack) {
		if (pMusicalScoreFrame->IsTrackVisible (i)) {
			if (lVisibleTopTrack <= i && i <= lVisibleBottomTrack) {
				long lColor = MIDITrack_GetForeColor (pMIDITrack);
				pDC->SetTextColor (lColor);
				CPen theSelectedPen;
				CBrush theSelectedBrush;
				theTrackPen.CreatePen (PS_SOLID, 1, lColor);
				theTrackBrush.CreateSolidBrush (lColor);
				theSelectedPen.CreatePen (PS_SOLID, 1, RGB (0, 0, 0));
				theSelectedBrush.CreateSolidBrush (RGB (0, 0, 0));
				MusicalScoreTrackInfo* pTrackInfo = (MusicalScoreTrackInfo*)(pMusicalScoreFrame->m_theTrackInfoArray.GetAt (i));
				long jMax = pTrackInfo->m_theNoteInfoArray.GetSize ();
				for (j = 0; j < jMax; j++) {
					MusicalScoreNoteInfo* pNoteInfo = (MusicalScoreNoteInfo*)(pTrackInfo->m_theNoteInfoArray.GetAt (j));
					MIDIEvent* pNoteEvent = pNoteInfo->m_pNoteOnEvent;
					if (lVisibleLeftTime <= pNoteInfo->m_lNoteOnTime && pNoteInfo->m_lNoteOnTime <= lVisibleRightTime || 
						lVisibleRightTime <= pNoteInfo->m_lNoteOffTime && pNoteInfo->m_lNoteOffTime < lVisibleRightTime ||
						pNoteInfo->m_lNoteOnTime <= lVisibleLeftTime && lVisibleRightTime < pNoteInfo->m_lNoteOffTime) {
						CPen* pOldPen = pDC->SelectObject (pSekaijuDoc->IsEventSelected (pNoteEvent) ? &theSelectedPen : &theTrackPen);
						CBrush* pOldBrush = pDC->SelectObject (pSekaijuDoc->IsEventSelected (pNoteEvent) ? & theSelectedBrush : &theTrackBrush);
						DrawNote (pDC, i, pNoteInfo, 0x00000000);
						pDC->SelectObject (pOldPen);
						pDC->SelectObject (pOldBrush);
					}
				}
				theTrackPen.DeleteObject ();
				theTrackBrush.DeleteObject ();
			}
		}
		i++;
	}
	pDC->SelectObject (pOldFont);
	pDC->SelectObject (pOldPen);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
	Sleep (0);
}


// ����]�����`��
void CMusicalScorePrintView::DrawScaleView (CDC* pDC, CPrintInfo* pInfo) {
	// ����]�����h��Ԃ�
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (0, 0, MUSICALSCOREPRINTVIEW_SCALEWIDTH, MUSICALSCOREPRINTVIEW_SCALEHEIGHT, lColorBtnFace);
}

// �����ڐ���`��
void CMusicalScorePrintView::DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pMIDIEvent = NULL;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick);

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = lCurPage % m_lMaxRowPage;
	long lCurRollPage = lCurPage / m_lMaxRowPage;	

	long lVisibleLeftMeasure = m_theRollPageInfoArray.GetAt (lCurRollPage);
	long lVisibleRightMeasure = m_theRollPageInfoArray.GetAt (lCurRollPage + 1);

	// �w�i�h��Ԃ�
	long lLeftMargin = MUSICALSCOREPRINTVIEW_LEFTMARGIN;
	long lRightMargin = MUSICALSCOREPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = MUSICALSCOREPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	pDC->FillSolidRect (0, 0, lTimeWidth * m_lMaxRollPage, MUSICALSCOREPRINTVIEW_SCALEHEIGHT, lColorBtnFace);

	// �������[��������
	pDC->SetBkMode (TRANSPARENT);
	long lOldTime = 0;
	CString strText1;
	CString strText2;
	pMIDITrack = MIDIData_GetFirstTrack (pMIDIData);
	if (pMIDITrack) {
		long lColorTrack1 = MIDITrack_GetForeColor (pMIDITrack);
		forEachEvent (pMIDITrack, pMIDIEvent) {
			long lTime = MIDIEvent_GetTime (pMIDIEvent);
			// �e���|
			if (MIDIEvent_IsTempo (pMIDIEvent)) {
				long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
				strText1.Format (_T("%1.2lf"), (double)60000000 / (double)lTempo);
				if (lOldTime != lTime) {
					if (strText2 != _T("")) {
						DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
					}
					strText2 = _T("");
				}
				if (strText2 != _T("")) {
					strText2 += _T(", ");
				}
				strText2 += strText1;
				lOldTime = lTime;
			}
			// ���q�L��
			else if (MIDIEvent_IsTimeSignature (pMIDIEvent)) {
				long lnn, ldd, lcc, lbb;
				MIDIEvent_GetTimeSignature (pMIDIEvent, &lnn, &ldd, &lcc, &lbb);
				strText1.Format (_T("%d/%d"), lnn, 1 << ldd);
				if (lOldTime != lTime) {
					if (strText2 != _T("")) {
						DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
					}
					strText2 = _T("");
				}
				if (strText2 != _T("")) {
					strText2 += _T(", ");
				}
				strText2 += strText1;
				lOldTime = lTime;
			}
			// �����L��
			else if (MIDIEvent_IsKeySignature (pMIDIEvent)) {
				long lsf, lmi;
				MIDIEvent_GetKeySignature (pMIDIEvent, &lsf, &lmi);
				strText1.Format (_T("%d%s"), labs (lsf), lsf >= 0 ? _T("#") : _T("b"));
				if (lOldTime != lTime) {
					if (strText2 != _T("")) {
						DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
					}
					strText2 = _T("");
				}
				if (strText2 != _T("")) {
					strText2 += _T(", ");
				}
				strText2 += strText1;
				lOldTime = lTime;
			}
			// �}�[�J�[
			else if (MIDIEvent_IsMarker (pMIDIEvent)) {
				TCHAR szBuf[256];
				memset (szBuf, 0, sizeof (szBuf));
				MIDIEvent_GetText (pMIDIEvent, szBuf, TSIZEOF (szBuf) - 1);
				if (lOldTime != lTime) {
					if (strText2 != _T("")) {
						DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
					}
					strText2 = _T("");
				}
				if (strText2 != _T("")) {
					strText2 += _T(", ");
				}
				strText2 += szBuf;
				lOldTime = lTime;
			}
		}
		if (strText2 != _T("")) {
			DrawFlagAndText (pDC, lOldTime, strText2, lColorTrack1);
		}
	}

	// �����ڐ���
	pDC->SetTextColor (lColorBtnText);
	if (lTimeMode == MIDIDATA_TPQNBASE) {
		long j;
		CFont* pOldFont = pDC->SelectObject (&m_theFont);
		for (j = lVisibleLeftMeasure; j < lVisibleRightMeasure; j++) {
			long x1 = MeasuretoX (j);
			long x2 = MeasuretoX (j + 1);
			long y1 = 0;
			long y2 = MUSICALSCOREPRINTVIEW_SCALEHEIGHT / 2;
			pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
			pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
			pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
			pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
			CRect rcText (x1, y1, x2, y2);
			CString strText;
			strText.Format (_T("%d"), j + 1);
			pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		pDC->SelectObject (pOldFont);
	}
	else {
		long lEndFrameNumber = lEndTime / lTimeResolution;
	}

	pSekaijuDoc->m_theCriticalSection.Unlock ();

}

// �Z���̕�������擾(�g���b�N������)
CString CMusicalScorePrintView::GetCellString (long lTrack) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	ASSERT (0 <= lTrack && lTrack < MAXMIDITRACKNUM);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDITrack* pMIDITrack = pSekaijuDoc->GetTrack (lTrack);
	MIDIEvent* pMIDIEvent = NULL;
	CString strText;
	strText = pSekaijuDoc->GetTrackName (pMIDITrack);
	TCHAR szBuf2[2048];
	memset (szBuf2, 0, sizeof (szBuf2));
	codestr2str ((LPTSTR)(LPCTSTR)strText, strText.GetLength (), szBuf2, TSIZEOF (szBuf2) - 1);
	strText = szBuf2;
	return strText;
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �����
void CMusicalScorePrintView::OnPrint (CDC* pDC, CPrintInfo* pInfo) {

	pDC->SetMapMode (MM_LOMETRIC);
	CPoint ptWindowOrg (0, 0);

	long lLeftMargin = MUSICALSCOREPRINTVIEW_LEFTMARGIN;
	long lRightMargin = MUSICALSCOREPRINTVIEW_RIGHTMARGIN;
	long lTopMargin = MUSICALSCOREPRINTVIEW_TOPMARGIN;
	long lBottomMargin = MUSICALSCOREPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = MUSICALSCOREPRINTVIEW_SCALEHEIGHT;
	long lTrackHeight = (m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin);
	long lScaleWidth = MUSICALSCOREPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	
	long lCurPage = pInfo->m_nCurPage - 1;
	long lCurRowPage = lCurPage % m_lMaxRowPage;
	long lCurRollPage = lCurPage / m_lMaxRowPage;



	CRgn theRgn;
	CRect rcClip;

	
	// �g���b�N�ڐ��蕔
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lTrackHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin + lTrackHeight * (m_lMaxRowPage - lCurRowPage - 1);
	pDC->SetWindowOrg (ptWindowOrg);
	DrawTrackScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// ����]����
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = -lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin + lTrackHeight;
	rcClip.bottom = lBottomMargin + lTrackHeight + lScaleHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lTrackHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// ���ʃ��[����
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lTimeWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lTrackHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin - lScaleWidth + lTimeWidth * lCurRollPage;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin + lTrackHeight * (m_lMaxRowPage - lCurRowPage - 1);
	pDC->SetWindowOrg (ptWindowOrg);
	DrawTrackTimeView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// �����ڐ��蕔
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lTimeWidth;
	rcClip.top = lBottomMargin + lTrackHeight;
	rcClip.bottom = lBottomMargin + lTrackHeight + lScaleHeight;
	if (pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC))) {
		CPreviewDC* pPreviewDC = (CPreviewDC*)pDC;
		pPreviewDC->LPtoDP (&rcClip.TopLeft ());
		pPreviewDC->LPtoDP (&rcClip.BottomRight ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.TopLeft ());
		pPreviewDC->PrinterDPtoScreenDP (&rcClip.BottomRight ());
		CPoint ptOrg;
		::GetViewportOrgEx (pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}
	else {
		pDC->LPtoDP (&rcClip.TopLeft ());
		pDC->LPtoDP (&rcClip.BottomRight ());
	}
	theRgn.CreateRectRgnIndirect (rcClip);
	pDC->SelectClipRgn (&theRgn);
	ptWindowOrg.x = -lLeftMargin - lScaleWidth + lTimeWidth * lCurRollPage;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lTrackHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawTimeScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// �w�b�_�[(�^�C�g��)
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	pDC->SetTextColor (RGB (0, 0 ,0));
	CRect rcText;
	rcText.left = lLeftMargin;
	rcText.right = m_sizLogPaper.cx - lRightMargin;
	rcText.top = m_sizLogPaper.cy - lTopMargin;
	rcText.bottom = m_sizLogPaper.cy - lTopMargin / 2;
	TCHAR szText[256];
	memset (szText, 0, sizeof (szText));
	MIDIData_GetTitle (GetDocument()->m_pMIDIData, szText, TSIZEOF (szText));
	CString strText;
	if (TCSLEN (szText) == 0) {
		strText = GetDocument()->GetTitle ();
	}
	else {
		strText.Format (_T("%s"), szText);
	}
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject (pOldFont);

	// �t�b�^�[(�y�[�W��/�S�y�[�W��)
	rcText.left = lLeftMargin;
	rcText.right = m_sizLogPaper.cx - lRightMargin;
	rcText.top = lTopMargin / 2;
	rcText.bottom = lTopMargin;
	strText.Format (_T("%d/%d"), pInfo->m_nCurPage, pInfo->GetMaxPage ());
	pOldFont = pDC->SelectObject (&m_theFont);
	pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject (pOldFont);

}

// ���������
BOOL CMusicalScorePrintView::OnPreparePrinting (CPrintInfo* pInfo) {
	// �f�t�H���g�̈������
	return DoPreparePrinting (pInfo);
}

// ����J�n��
void CMusicalScorePrintView::OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo) {
	CMusicalScoreFrame* pMusicalScoreFrame = (CMusicalScoreFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	pSekaijuDoc->m_theCriticalSection.Lock ();
	
	// ���T�C�Y�̎擾
	m_sizDevPaper.cx = pDC->GetDeviceCaps (HORZRES); // [pixel]
	m_sizDevPaper.cy = pDC->GetDeviceCaps (VERTRES); // [pixel]
	m_sizLogPaper.cx = pDC->GetDeviceCaps (HORZSIZE) * 10; // [*0.1mm]
	m_sizLogPaper.cy = pDC->GetDeviceCaps (VERTSIZE) * 10; // [*0.1mm]
	m_sizLogPrinterDPI.cx = pDC->GetDeviceCaps (LOGPIXELSX); // [dpi]
	m_sizLogPrinterDPI.cy = pDC->GetDeviceCaps (LOGPIXELSY); // [dpi]
	
	// MIDI�f�[�^�̐������擾
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// �g���b�N���z��Ə��ߏ��z��̈���p�����v�Z
	this->CalcTrackInfoArray ();
	this->CalcMeasureInfoArray ();

	// ���[�����̕��Ɖ������y�[�W�����v�Z
	long lEndTime = MIDIData_GetEndTime (pMIDIData);
	long lEndMeasure, lEndBeat, lEndTick;
	MIDIData_BreakTime (pMIDIData, lEndTime, &lEndMeasure, &lEndBeat, &lEndTick); 
	long lLastTime = 0;
	MIDIData_MakeTime (pMIDIData, lEndMeasure + 1, 0, 0, &lLastTime);
	long lRollWidth = this->TimetoX (lLastTime);
	long lLeftMargin = MUSICALSCOREPRINTVIEW_LEFTMARGIN;
	long lRightMargin = MUSICALSCOREPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = MUSICALSCOREPRINTVIEW_SCALEWIDTH;
	long lTimeWidth = m_sizLogPaper.cx - lScaleWidth - lLeftMargin - lRightMargin;
	if (lRollWidth % MAX (lTimeWidth, 1) == 0) {
		m_lMaxRollPage = lRollWidth / MAX (lTimeWidth, 1);
	}
	else {
		m_lMaxRollPage = lRollWidth / MAX (lTimeWidth, 1) + 1;
	}

	// �c�����y�[�W�����v�Z
	long lTopMargin = MUSICALSCOREPRINTVIEW_TOPMARGIN;
	long lBottomMargin = MUSICALSCOREPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = MUSICALSCOREPRINTVIEW_SCALEHEIGHT;
	long lTrackHeight = m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin;
	long lTrackInfoCount = pMusicalScoreFrame->GetTrackInfoCount ();
	ASSERT (1 <= lTrackInfoCount && lTrackInfoCount <= MAXMIDITRACKNUM);
	MusicalScoreTrackInfo* pLastTrackInfo = pMusicalScoreFrame->GetTrackInfo (lTrackInfoCount - 1);
	long lTrackBottom = (pLastTrackInfo->m_lTopPrint + pLastTrackInfo->m_lHeightPrint);
	if (lTrackBottom % MAX (lTrackHeight, 1) == 0) {
		m_lMaxRowPage = lTrackBottom / MAX (lTrackHeight, 1);
	}
	else {
		m_lMaxRowPage = lTrackBottom / MAX (lTrackHeight, 1) + 1;
	}

	// ����y�[�W���̐ݒ�
	m_lMaxPage = m_lMaxRowPage * m_lMaxRollPage;
	pInfo->SetMaxPage (m_lMaxPage);

	pSekaijuDoc->m_theCriticalSection.Unlock ();
}

// ����I����
void CMusicalScorePrintView::OnEndPrinting (CDC* pDC, CPrintInfo* pInfo) {
}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

