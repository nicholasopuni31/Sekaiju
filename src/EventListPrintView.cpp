//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �C�x���g���X�g����r���[�N���X
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
#include <afxmt.h>
#include <afxpriv.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "common.h"
#include "mousewheel.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "EventListFrame.h"
#include "EventListPrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVENTLISTPRINTVIEW_SCALEHEIGHT 40
#define EVENTLISTPRINTVIEW_SCALEWIDTH 100
#define EVENTLISTPRINTVIEW_LEFTMARGIN 200
#define EVENTLISTPRINTVIEW_RIGHTMARGIN 200
#define EVENTLISTPRINTVIEW_TOPMARGIN 200
#define EVENTLISTPRINTVIEW_BOTTOMMARGIN 200

IMPLEMENT_DYNCREATE (CEventListPrintView, CSekaijuView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CEventListPrintView, CSekaijuView)
	ON_COMMAND (ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND (ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CEventListPrintView::CEventListPrintView () {
	// ����p�����t�H���g�̍쐬(2.5�~��)
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theFont.CreateFont (25, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);

}

// �f�X�g���N�^
CEventListPrintView::~CEventListPrintView () {
	m_theFont.DeleteObject ();
}


//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �Z���̒����`���擾
CRect CEventListPrintView::GetRectFromCell (long lRow, long lColumn) {
	ASSERT (0 <= lColumn && lColumn < 8);
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	BOOL bCombined = FALSE; // ��5�`7���������Ă���ꍇTRUE;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	// ��5�`7�̃Z���̌�������
	if (0 <= lRow && lRow < lVisibleEventCount) {
		if (lColumn >= 5) {
			MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (lRow);
			if (!MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
				bCombined = TRUE;
			}
		}
	}
	else {
		bCombined = TRUE;
	}
	// �c���E�̒�`
	long lLeftMargin = EVENTLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = EVENTLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = EVENTLISTPRINTVIEW_SCALEWIDTH;
	long lWidthDiv[] = {0, 2, 4, 6, 8, 9, 11, 13, 15};
	// �Z����`�̌v�Z
	CRect rcCell (0, 0, 0 ,0);
	if (bCombined && lColumn >= 5) {
		rcCell.left = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[5] / 15;
		rcCell.right = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[8] / 15;
	}
	else {
		rcCell.left = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[lColumn] / 15;
		rcCell.right = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[lColumn + 1] / 15;
	}
	long lRowZoom = 40;
	rcCell.top = lRowZoom * m_lNumEventPerPage * m_lMaxPage - lRowZoom * (lRow + 1);
	rcCell.bottom = lRowZoom * m_lNumEventPerPage * m_lMaxPage - lRowZoom * (lRow + 0);
	ASSERT (rcCell.left < rcCell.right);
	ASSERT (rcCell.top < rcCell.bottom);
	return rcCell;
}

// �Z���̕�������擾����
CString CEventListPrintView::GetCellString (long lRow, long lColumn) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	if (lRow < 0 || lRow >= pEventListFrame->GetVisibleEventCount ()) {
		return _T("");
	}
	BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
	MIDIEvent* pMIDIEvent = pEventListFrame->GetVisibleEvent (lRow);
	MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	MIDIClock* pMIDIClock = pSekaijuDoc->m_pMIDIClock;
	long lTrackOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
	long lTrackOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
	long lTrackViewMode = MIDITrack_GetViewMode (pMIDITrack);
	long lTime = MIDIEvent_GetTime (pMIDIEvent);
	long lKind = MIDIEvent_GetKind (pMIDIEvent);
	if (!MIDIEvent_IsMIDIEvent (pMIDIEvent) && lColumn > 5) {
		lColumn = 5;
	}
	CString strText;
	TCHAR szBuf1[2048];
	TCHAR szBuf2[2048];
	switch (lColumn) {
	// �g���b�N(20120107�C��)
	case 0:
		memset (szBuf1, 0, sizeof (szBuf1));
		memset (szBuf2, 0, sizeof (szBuf2));
		MIDITrack_GetName (pMIDITrack, szBuf1, TSIZEOF (szBuf1) - 1);
		codestr2str (szBuf1, TCSLEN (szBuf1), szBuf2, sizeof (szBuf2));
		strText.Format (_T("%d-%s"), 
			pSekaijuDoc->GetTrackIndex (pMIDITrack) + (bTrackZeroOrigin ? 0 : 1), szBuf2);
		break;
	// ��:��:�b:�~���b
	case 1:
		VERIFY (pSekaijuDoc->LongTimeToStringMillisec (pSekaijuDoc->m_pMIDIData, lTime, &strText));
		break;
	// ����:��:�e�B�b�N ���� �t���[���ԍ�:�e�B�b�N
	case 2:
		VERIFY (pSekaijuDoc->LongTimeToStringTime (pSekaijuDoc->m_pMIDIData, lTime, &strText));
		break;
	// �C�x���g�̎��
	case 3:
		strText = pSekaijuApp->m_strEventKindName[lKind];
		break;
	// �`�����l��(MIDI�`�����l���C�x���g�̏ꍇ�̂݁B���̑���n/a)�B
	case 4:
		if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			strText.Format (_T("%3d"), MIDIEvent_GetChannel (pMIDIEvent) + 1);
		}
		else {
			strText = _T("n/a");
		}
		break;
	// �l1
	case 5:
		// �V�[�P���X�ԍ�
		if (MIDIEvent_IsSequenceNumber (pMIDIEvent)) {
			strText.Format (_T("%d"), MIDIEvent_GetNumber (pMIDIEvent));
		}
		// �e�L�X�g�n�C�x���g
		else if (0x01 <= lKind && lKind <= 0x1F) {
			TCHAR szBuf[1024];
			memset (szBuf, 0, 1024);
			MIDIEvent_GetText (pMIDIEvent, szBuf, 1024);
			strText = szBuf;
		}
		// �`�����l���v���t�B�b�N�X,�|�[�g�v���t�B�b�N�X
		else if (MIDIEvent_IsChannelPrefix (pMIDIEvent) || 
			MIDIEvent_IsPortPrefix (pMIDIEvent)) {
			strText.Format (_T("%d"), MIDIEvent_GetNumber (pMIDIEvent) + 1);
		}
		// �G���h�I�u�g���b�N
		else if (MIDIEvent_IsEndofTrack (pMIDIEvent)) {
			strText = _T("");
		}
		// �e���|�C�x���g
		else if (MIDIEvent_IsTempo (pMIDIEvent)) {
			long lTempo;
			double dBPM;
			lTempo = MIDIEvent_GetTempo (pMIDIEvent);
			dBPM = (double)60000000 / (double)lTempo;
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_1P2LF_BPM_EQ_D_MICROSEC_PER_QUARTER_NOTE));
			strText.Format (strFormat, dBPM, lTempo);
		}
		// SMPTE�I�t�Z�b�g
		else if (MIDIEvent_IsSMPTEOffset (pMIDIEvent)) {
			CString strMode[5];
			VERIFY (strMode[0].LoadString (IDS_24));
			VERIFY (strMode[1].LoadString (IDS_25));
			VERIFY (strMode[2].LoadString (IDS_29P97));
			VERIFY (strMode[3].LoadString (IDS_30));
			VERIFY (strMode[4].LoadString (IDS_ERROR));
			long lMode, lHour, lMinute, lSec, lFrame, lSubFrame;
			MIDIEvent_GetSMPTEOffset (pMIDIEvent, &lMode, &lHour, &lMinute, &lSec, &lFrame, &lSubFrame);
			if (lMode < 0 || lMode > 4) {
				lMode = 4;
			}
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_S_FPS_D_HOUR_D_MINUTE_D_SEC_D_FRAME_D_SUBFRAME));
			strText.Format (strFormat, 
				strMode[lMode], lHour, lMinute, lSec, lFrame, lSubFrame);
		}
		// ���q�L��
		else if (lKind == MIDIEVENT_TIMESIGNATURE) {
			long lnn, ldd, lcc, lbb;
			MIDIEvent_GetTimeSignature (pMIDIEvent, &lnn, &ldd, &lcc, &lbb);
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_PER_D_D_CLOCK_PER_BEAT_D_32DIVNOTE_PER_BEAT));
			strText.Format (strFormat, lnn, 1 << ldd, lcc, lbb);
		}
		// �����L��
		else if (lKind == MIDIEVENT_KEYSIGNATURE) {
			long lsf, lmi;
			MIDIEvent_GetKeySignature (pMIDIEvent, &lsf, &lmi);
			CString strSF; // "#"����"b"
			CString strMi; // "major"����"minor"
			CString strKeySignatureName;
			// ����
			if (lmi == 0) {
				if (0 <= lsf && lsf < 8) {
					VERIFY (strSF.LoadString (IDS_SHARP));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MA + lsf ));
				}
				else if (-8 <= lsf && lsf < 0) {
					VERIFY (strSF.LoadString (IDS_FLAT));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MA + 16 + lsf));
				}
				VERIFY (strMi.LoadString (IDS_MAJOR));
			}
			// �Z��
			else {
				if (0 <= lsf && lsf < 8) {
					VERIFY (strSF.LoadString (IDS_SHARP));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MI + lsf ));
				}
				else if (-8 <= lsf && lsf < 0) {
					VERIFY (strSF.LoadString (IDS_FLAT));
					VERIFY (strKeySignatureName.LoadString (IDS_KEYSIGNATURE_0MI + 16 + lsf));
				}
				VERIFY (strMi.LoadString (IDS_MINOR));
			}
			CString strFormat;
			VERIFY (strFormat.LoadString (IDS_D_S_S_EQ_S));			
			strText.Format (strFormat, abs (lsf), strSF, strMi, strKeySignatureName);
		}
		// �m�[�g�I�t�E�m�[�g�I���E�L�[�A�t�^�[�^�b�`
		else if (0x80 <= lKind && lKind <= 0xAF) {
			long lKey = MIDIEvent_GetKey (pMIDIEvent);
			CString strKeyName;
			strKeyName = pSekaijuDoc->GetKeyName (pMIDITrack, lTime, lKey);
			strText.Format (_T("%d-%s"), lKey, strKeyName);
		}
		// �R���g���[���`�F���W
		else if (0xB0 <= lKind && lKind <= 0xBF) {
			long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
			long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
			if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
				lChannel = lTrackOutputChannel;	
			}
			MIDIInstrumentDefinition* pMIDIInstDef = NULL;
			// ���̃g���b�N�̕\�����[�h�́u�ʏ�v�̏ꍇ
			if (lTrackViewMode == 0) {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			}
			// ���̃g���b�N�̕\�����[�h�́u�h�����v�̏ꍇ
			else {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
			}
			// ���̃g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`����������
			if (pMIDIInstDef) {
				MIDIControllerNameTable* pControllerNameTable = 
				MIDIInstrumentDefinition_GetControllerNameTable (pMIDIInstDef);
				// ���̃C���X�g�D�������g��`��ControllerNameTable�������Ă���
				if (pControllerNameTable) {
					TCHAR szBuf[256];
					memset (szBuf, 0, sizeof (szBuf));
					MIDIControllerNameTable_GetName (pControllerNameTable, lNumber, szBuf, 255);
					strText.Format (_T("%d-%s"), lNumber, szBuf);
				}
				// ���̃C���X�g�D�������g��`��ControllerNameTable�������Ă��Ȃ�
				else {
					strText.Format (_T("%d"), lNumber);
				}
			}
			// ���g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`���Ȃ�����
			else {
				strText.Format (_T("%d"), lNumber);
			}
		}
		// �v���O�����`�F���W
		else if (0xC0 <= lKind && lKind <= 0xCF) {
			long lNumber = MIDIEvent_GetNumber (pMIDIEvent);
			long lChannel = MIDIEvent_GetChannel (pMIDIEvent);
			if (0 <= lTrackOutputChannel && lTrackOutputChannel < 16) {
				lChannel = lTrackOutputChannel;	
			}
			MIDIInstrumentDefinition* pMIDIInstDef = NULL;
			// ���̃g���b�N�̕\�����[�h�́u�ʏ�v�̏ꍇ
			if (lTrackViewMode == 0) {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefNorm[lTrackOutputPort];
			}
			// ���̃g���b�N�̕\�����[�h�́u�h�����v�̏ꍇ
			else {
				pMIDIInstDef = pSekaijuApp->m_pMIDIInstDefDrum[lTrackOutputPort];
			}
			// ���̃g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`����������
			if (pMIDIInstDef) {
				long lBankMSB = MIDIEvent_GetBankMSB (pMIDIEvent);
				long lBankLSB = MIDIEvent_GetBankLSB (pMIDIEvent);
				long lBank = (lBankMSB << 7) | lBankLSB;
				MIDIPatchNameTable* pPatchNameTable = 
				MIDIInstrumentDefinition_GetPatchNameTable (pMIDIInstDef, lBank);
				// ���̃C���X�g�D�������g��`�͎w��o���N��PatchNameTable�������Ă���
				if (pPatchNameTable) {
					TCHAR szBuf[256];
					memset (szBuf, 0, sizeof (szBuf));
					MIDIPatchNameTable_GetName (pPatchNameTable, lNumber, szBuf, 255);
					strText.Format (_T("%d-%s"), lNumber, szBuf);
				}
				// ���̃C���X�g�D�������g��`�͎w��o���N��PatchNameTable�������Ă��Ȃ�
				else {
					strText.Format (_T("%d"), lNumber);
				}
			}
			// ���g���b�N�̎w�肷��|�[�g�̃C���X�g�D�������g��`���Ȃ�����
			else {
				strText.Format (_T("%d"), lNumber);
			}
		}
		// �`�����l���A�t�^�[�^�b�`
		else if (0xD0 <= lKind && lKind <= 0xDF) {
			long lValue = MIDIEvent_GetValue (pMIDIEvent);
			strText.Format (_T("%d"), lValue);
		}
		// �s�b�`�x���h
		else if (0xE0 <= lKind && lKind <= 0xEF) {
			long lValue = MIDIEvent_GetValue (pMIDIEvent);
			strText.Format (_T("%d"), lValue - 8192);
		}
		// ���̑��̃C�x���g(16�i�_���v)
		else {
			BYTE byBin[1024];
			TCHAR szText[1024];
			memset (byBin, 0, sizeof (byBin));
			memset (szText, 0, sizeof (szText));
			MIDIEvent_GetData (pMIDIEvent, byBin, 1024);
			bin2txt (byBin, MIDIEvent_GetLen (pMIDIEvent), szText, 1023);
			strText = szText;
		}
		break;
	// �l2
	case 6:
		// �m�[�g
		if (0x80 <= lKind && lKind <= 0x9F) {
			strText.Format (_T("%d"), MIDIEvent_GetVelocity (pMIDIEvent));
		}
		// �L�[�A�t�^�[�^�b�`�E�R���g���[���`�F���W
		else if (0xA0 <= lKind && lKind <= 0xBF) {
			strText.Format (_T("%d"), MIDIEvent_GetValue (pMIDIEvent));
		}
		// ���̑�
		else {
			strText.Format (_T("---"));
		}
		break;
	// �l3
	case 7:
		// �m�[�g
		if (0x80 <= lKind && lKind <= 0x9F) {
			strText.Format (_T("%d"), MIDIEvent_GetDuration (pMIDIEvent));
		}
		// ���̑�
		else {
			strText.Format (_T("---"));
		}
		break;
	}
	return strText;
}


// �����x�����`��
void CEventListPrintView::DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_theFont);

	long lRowZoom = 40;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	long lRowCount = m_lNumEventPerPage * pInfo->GetMaxPage ();
	long lCurPage = pInfo->m_nCurPage - 1;
	long i;
	long iMin = m_lNumEventPerPage * lCurPage;
	long iMax = m_lNumEventPerPage * (lCurPage + 1);
	for (i = iMin; i < iMax; i++) {
		long x1 = 0;
		long x2 = EVENTLISTPRINTVIEW_SCALEWIDTH;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
		pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
		pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
		CRect rcText (x1, y1, x2, y2);
		CString strText;
		strText.Format (_T("%d"), pSekaijuApp->m_theGeneralOption.m_bEventZeroOrigin ? i : i + 1);
		pDC->DrawText (strText, rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// ����`��
void CEventListPrintView::DrawScaleView (CDC* pDC, CPrintInfo* pInfo) {
	// ����]�����`��
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (0, 0, EVENTLISTPRINTVIEW_SCALEWIDTH, EVENTLISTPRINTVIEW_SCALEHEIGHT, lColorBtnFace);
}

// ���X�g���`��
void CEventListPrintView::DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();

	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);

	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&m_theFont);

	CPen thePen (PS_SOLID, 1, RGB (128, 128, 128));
	CPen* pOldPen = pDC->SelectObject (&thePen);

	long lRowZoom = 40;
	long lLeftMargin = EVENTLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = EVENTLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = EVENTLISTPRINTVIEW_SCALEWIDTH;
	long lVisibleEventCount = pEventListFrame->GetVisibleEventCount ();
	long lRowCount = m_lNumEventPerPage * pInfo->GetMaxPage ();
	long lCurPage = pInfo->m_nCurPage - 1;
	long i;
	long iMin = m_lNumEventPerPage * lCurPage;
	long iMax = m_lNumEventPerPage * (lCurPage + 1);
	for (i = iMin; i < iMax; i++) {
		long x1 = 0;
		long x2 = m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth;
		long y1 = lRowZoom * (lRowCount - i);
		long y2 = y1 - lRowZoom;
		long lBackColor = (i % 2 ? RGB (255, 255, 200) : RGB (255, 255, 255));
		// �w�i�̓h��Ԃ�
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lBackColor);
		// �����̕`��
		pDC->MoveTo (x1, y1 + 1);
		pDC->LineTo (x2, y1 + 1);

		// �e�L�X�g�J���[�̐ݒ�
		long lForeColor = RGB (0, 0, 0);
		pDC->SetBkMode (TRANSPARENT);
		CRect rcText;
		CString strText;
		MIDIEvent* pMIDIEvent = NULL;
		if (0 <= i && i < lVisibleEventCount) {
			VERIFY (pMIDIEvent = pEventListFrame->GetVisibleEvent (i));
		}
		if (pMIDIEvent) {
			// TODO:MIDI�C�x���g�������ւ����Ă��邪�܂�UpdateAllViews����ĂȂ��ꍇ�̉��[�u
			if (MIDIEvent_IsFloating (pMIDIEvent)) {
				continue;
			}
			MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
			ASSERT (pMIDITrack);
			lForeColor = MIDITrack_GetForeColor (pMIDITrack);
			pDC->SetTextColor (lForeColor);
		}
		// �ׯ���
		rcText = GetRectFromCell (i, 0);
		if (pMIDIEvent) {
			strText = GetCellString (i, 0);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		pDC->MoveTo (rcText.right - 1, rcText.top);
		pDC->LineTo (rcText.right - 1, rcText.bottom);
		// ��:��:�b:�ؕb
		rcText = GetRectFromCell (i, 1);
		if (pMIDIEvent) {
			strText = GetCellString (i, 1);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		pDC->MoveTo (rcText.right - 1, rcText.top);
		pDC->LineTo (rcText.right - 1, rcText.bottom);
		// ����:��:è��
		rcText = GetRectFromCell (i, 2);
		if (pMIDIEvent) {
			strText = GetCellString (i, 2);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		pDC->MoveTo (rcText.right - 1, rcText.top);
		pDC->LineTo (rcText.right - 1, rcText.bottom);
		// �C�x���g�̎��
		rcText = GetRectFromCell (i, 3);
		if (pMIDIEvent) {
			strText = GetCellString (i, 3);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		pDC->MoveTo (rcText.right - 1, rcText.top);
		pDC->LineTo (rcText.right - 1, rcText.bottom);
		// �`�����l��
		rcText = GetRectFromCell (i, 4);
		if (pMIDIEvent) {
			strText = GetCellString (i, 4);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		pDC->MoveTo (rcText.right - 1, rcText.top);
		pDC->LineTo (rcText.right - 1, rcText.bottom);
		// �l1
		rcText = GetRectFromCell (i, 5);
		if (pMIDIEvent) {
			strText = GetCellString (i, 5);
			pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		// �l2, �l3
		if (pMIDIEvent) {
			long lEventKind = MIDIEvent_GetKind (pMIDIEvent);
			if (0x80 <= lEventKind && lEventKind <= 0xEF) {
				pDC->MoveTo (rcText.right - 1, rcText.top);
				pDC->LineTo (rcText.right - 1, rcText.bottom);
				rcText = GetRectFromCell (i, 6);
				strText = GetCellString (i, 6);
				pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			
				pDC->MoveTo (rcText.right - 1, rcText.top);
				pDC->LineTo (rcText.right - 1, rcText.bottom);
				rcText = GetRectFromCell (i, 7);
				strText = GetCellString (i, 7);
				pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			
			}
		}
	}
	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �㕔���x�����`��
void CEventListPrintView::DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo) {
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	long lColorBlack = RGB (0, 0, 0);
	long lColorWhite = RGB (255, 255, 255);
	long lLeftMargin = EVENTLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = EVENTLISTPRINTVIEW_RIGHTMARGIN;
	long lScaleWidth = EVENTLISTPRINTVIEW_SCALEWIDTH;
	long lWidthDiv[] = {0, 2, 4, 6, 8, 9, 11, 13, 15};
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (RGB (0, 0, 0));
	CFont* pOldFont = pDC->SelectObject (&m_theFont);
	for (long j = 0; j < 8; j++) {
		long x1 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j] / 15;
		long x2 = (m_sizLogPaper.cx - lLeftMargin - lRightMargin - lScaleWidth) * lWidthDiv[j + 1] / 15;
		long y1 = 0;
		long y2 = EVENTLISTPRINTVIEW_SCALEHEIGHT;
		pDC->FillSolidRect (CRect (x1, y1, x2, y2), lColorBtnFace);
		pDC->FillSolidRect (CRect (x1, y2 - 1, x2, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x1 + 1, y2), lColorBtnHighlight);
		pDC->FillSolidRect (CRect (x1, y1, x2, y1 + 1), lColorBtnShadow);
		pDC->FillSolidRect (CRect (x2 - 1, y1, x2, y2), lColorBtnShadow);
		CRect rcText (x1, y1, x2, y2);
		CString strText (pEventListFrame->GetColumnTitle (j));
		pDC->DrawText (strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}


//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �����
void CEventListPrintView::OnPrint (CDC* pDC, CPrintInfo* pInfo) {

	pDC->SetMapMode (MM_LOMETRIC);
	CPoint ptWindowOrg (0, 0);

	long lLeftMargin = EVENTLISTPRINTVIEW_LEFTMARGIN;
	long lRightMargin = EVENTLISTPRINTVIEW_RIGHTMARGIN;
	long lTopMargin = EVENTLISTPRINTVIEW_TOPMARGIN;
	long lBottomMargin = EVENTLISTPRINTVIEW_BOTTOMMARGIN;
	long lScaleHeight = EVENTLISTPRINTVIEW_SCALEHEIGHT;
	long lScaleWidth = EVENTLISTPRINTVIEW_SCALEWIDTH;
	long lListHeight = (m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin) / 40 * 40;
	long lListWidth = (m_sizLogPaper.cx - lScaleWidth- lLeftMargin - lRightMargin);
	
	CRgn theRgn;
	CRect rcClip;

	// �C�x���g�ԍ���
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lListHeight;
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
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin  + lListHeight * (pInfo->GetMaxPage () - pInfo->m_nCurPage);
	pDC->SetWindowOrg (ptWindowOrg);
	DrawIndexScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();
	

	// ����]����
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin;
	rcClip.right = lLeftMargin + lScaleWidth;
	rcClip.top = lBottomMargin + lListHeight;
	rcClip.bottom = lBottomMargin + lListHeight + lScaleHeight;
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
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lListHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawScaleView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();

	// �C�x���g���X�g��
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lListWidth;
	rcClip.top = lBottomMargin;
	rcClip.bottom = lBottomMargin + lListHeight;
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
	ptWindowOrg.x = -lLeftMargin - lScaleWidth;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin + lListHeight * (pInfo->GetMaxPage () - pInfo->m_nCurPage);
	pDC->SetWindowOrg (ptWindowOrg);
	DrawIndexPropertyView (pDC, pInfo);
	pDC->SelectClipRgn (NULL);
	theRgn.DeleteObject ();
	

	// �㕔���ږ���
	ptWindowOrg.x = 0;
	ptWindowOrg.y = m_sizLogPaper.cy;
	pDC->SetWindowOrg (ptWindowOrg);
	rcClip.left = lLeftMargin + lScaleWidth;
	rcClip.right = lLeftMargin + lScaleWidth + lListWidth;
	rcClip.top = lBottomMargin + lListHeight;
	rcClip.bottom = lBottomMargin + lListHeight + lScaleHeight;
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
	ptWindowOrg.x = -lLeftMargin - lScaleWidth;
	ptWindowOrg.y = m_sizLogPaper.cy - lBottomMargin - lListHeight;
	pDC->SetWindowOrg (ptWindowOrg);
	DrawPropertyScaleView (pDC, pInfo);
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
BOOL CEventListPrintView::OnPreparePrinting (CPrintInfo* pInfo) {
	// �f�t�H���g�̈������
	return DoPreparePrinting (pInfo);
}

// ����J�n��
void CEventListPrintView::OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	
	// ���T�C�Y�̎擾
	m_sizDevPaper.cx = pDC->GetDeviceCaps (HORZRES); // [pixel]
	m_sizDevPaper.cy = pDC->GetDeviceCaps (VERTRES); // [pixel]
	m_sizLogPaper.cx = pDC->GetDeviceCaps (HORZSIZE) * 10; // [*0.1mm]
	m_sizLogPaper.cy = pDC->GetDeviceCaps (VERTSIZE) * 10; // [*0.1mm]
	m_sizLogPrinterDPI.cx = pDC->GetDeviceCaps (LOGPIXELSX); // [dpi]
	m_sizLogPrinterDPI.cy = pDC->GetDeviceCaps (LOGPIXELSY); // [dpi]

	// ���X�g�S�̂̍������v�Z
	long lRowZoom = 40;
	long lScaleHeight = EVENTLISTPRINTVIEW_SCALEHEIGHT;
	long lTopMargin = EVENTLISTPRINTVIEW_TOPMARGIN;
	long lBottomMargin = EVENTLISTPRINTVIEW_BOTTOMMARGIN;
	long lNumVisibleEvent = pEventListFrame->GetVisibleEventCount ();
	m_lNumEventPerPage = MAX ((m_sizLogPaper.cy - lScaleHeight - lTopMargin - lBottomMargin) / lRowZoom, 1);
	m_lMaxPage = lNumVisibleEvent / m_lNumEventPerPage + 1;

	// ����y�[�W���̐ݒ�
	pInfo->SetMaxPage (m_lMaxPage);
}

// ����I����
void CEventListPrintView::OnEndPrinting (CDC* pDC, CPrintInfo* pInfo) {
}

//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

