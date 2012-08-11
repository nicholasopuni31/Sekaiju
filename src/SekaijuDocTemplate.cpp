//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���h�L�������g�e���v���[�g�N���X
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
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuDocTemplate.h"
#include "SekaijuToolBar.h"
#include "ChildFrame.h"
#include "TrackListFrame.h"
#include "PianoRollFrame.h"
#include "EventListFrame.h"
#include "MusicalScoreFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CSekaijuDocTemplate::CSekaijuDocTemplate (UINT nIDResource, 
	 CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, 
	 CRuntimeClass* pViewClass)
	: CMultiDocTemplate (nIDResource, pDocClass, pFrameClass, pViewClass) {
	ASSERT (m_docList.IsEmpty());
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// �g���b�N���X�g�t���[���E�B���h�E�̐���
CFrameWnd* CSekaijuDocTemplate::CreateTrackListFrame (CDocument* pDoc) {
	if (pDoc != NULL) {
		ASSERT_VALID(pDoc);
	}
	ASSERT (m_nIDResource != 0);
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	CRuntimeClass* m_pOldFrameClass = m_pFrameClass;
	m_pFrameClass = RUNTIME_CLASS (CTrackListFrame);
	if (m_pFrameClass == NULL)	{
		TRACE0 ("Error: you must override CDocTemplate::CreateTrackListFrame.\n");
		ASSERT (FALSE);
		return NULL;
	}
	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (pFrame == NULL) {
		TRACE1 ("Warning: Dynamic create of frame %hs failed.\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL) {
		TRACE0 ("Warning: creating frame with no default view.\n");
	}
	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			NULL, &context))
	{
		TRACE0 ("Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	m_pFrameClass = m_pOldFrameClass;
	return pFrame;

}


// �s�A�m���[���t���[���E�B���h�E�̐���
CFrameWnd* CSekaijuDocTemplate::CreatePianoRollFrame (CDocument* pDoc) {
	if (pDoc != NULL) {
		ASSERT_VALID(pDoc);
	}
	ASSERT (m_nIDResource != 0);
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	CRuntimeClass* m_pOldFrameClass = m_pFrameClass;
	m_pFrameClass = RUNTIME_CLASS (CPianoRollFrame);
	if (m_pFrameClass == NULL)	{
		TRACE0 ("Error: you must override CDocTemplate::CreatePianoRollFrame.\n");
		ASSERT (FALSE);
		return NULL;
	}
	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (pFrame == NULL) {
		TRACE1 ("Warning: Dynamic create of frame %hs failed.\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL) {
		TRACE0 ("Warning: creating frame with no default view.\n");
	}
	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			NULL, &context))
	{
		TRACE0 ("Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	m_pFrameClass = m_pOldFrameClass;
	return pFrame;

}

// �C�x���g���X�g�t���[���E�B���h�E�̐���
CFrameWnd* CSekaijuDocTemplate::CreateEventListFrame (CDocument* pDoc) {
	if (pDoc != NULL) {
		ASSERT_VALID(pDoc);
	}
	ASSERT (m_nIDResource != 0);
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	CRuntimeClass* m_pOldFrameClass = m_pFrameClass;
	m_pFrameClass = RUNTIME_CLASS (CEventListFrame);
	if (m_pFrameClass == NULL)	{
		TRACE0 ("Error: you must override CDocTemplate::CreateEventListFrame.\n");
		ASSERT(FALSE);
		return NULL;
	}
	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (pFrame == NULL) {
		TRACE1 ("Warning: Dynamic create of frame %hs failed.\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL) {
		TRACE0 ("Warning: creating frame with no default view.\n");
	}
	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			NULL, &context))
	{
		TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	m_pFrameClass = m_pOldFrameClass;
	return pFrame;

}

// ���ʃt���[���E�B���h�E�̐���
CFrameWnd* CSekaijuDocTemplate::CreateMusicalScoreFrame (CDocument* pDoc) {
	if (pDoc != NULL) {
		ASSERT_VALID(pDoc);
	}
	ASSERT (m_nIDResource != 0);
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	CRuntimeClass* m_pOldFrameClass = m_pFrameClass;
	m_pFrameClass = RUNTIME_CLASS (CMusicalScoreFrame);
	if (m_pFrameClass == NULL)	{
		TRACE0 ("Error: you must override CDocTemplate::CreateMusicalScoreFrame.\n");
		ASSERT(FALSE);
		return NULL;
	}
	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (pFrame == NULL) {
		TRACE1 ("Warning: Dynamic create of frame %hs failed.\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL) {
		TRACE0 ("Warning: creating frame with no default view.\n");
	}
	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			NULL, &context))
	{
		TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	m_pFrameClass = m_pOldFrameClass;
	return pFrame;

}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���̃h�L�������g�e���v���[�g�̂��ׂẴh�L�������g�̃r���[���X�V
void CSekaijuDocTemplate::UpdateAllViews (CView* pSender, LPARAM lHint, CObject* pHint) {
	POSITION pos2 = this->GetFirstDocPosition ();
	while (pos2) {
		CDocument* pDocument = this->GetNextDoc (pos2);
		pDocument->UpdateAllViews (pSender, lHint, pHint);
	}	
}

// MIDI�f�[�^�̐V�K�쐬�A����MIDI�f�[�^���J����
CDocument* CSekaijuDocTemplate::OpenDocumentFile (LPCTSTR lpszPathName, BOOL bMakeVisible) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	// ������MIDI�f�[�^���J�����Ƃ������Ă��Ȃ��ꍇ
	if (!pSekaijuApp->m_theGeneralOption.m_bEnableMultiOpen) {
		// �����̊J���Ă���MIDI�f�[�^�����ׂĕ���B
		POSITION pos = GetFirstDocPosition ();
		while (pos != NULL) {
			CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(GetNextDoc (pos));
			if (!pSekaijuDoc->SaveModified ()) {
				return pSekaijuDoc;
			}
			pSekaijuDoc->OnCloseDocument ();
		}
	}
	// �ʏ�̏���
	return CMultiDocTemplate::OpenDocumentFile (lpszPathName, bMakeVisible);
}


IMPLEMENT_DYNAMIC (CSekaijuDocTemplate, CMultiDocTemplate)

