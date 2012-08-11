//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���h�L�������g�e���v���[�g�N���X
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

#ifndef _SEKAIJUDOCTEMPLETE_H_
#define _SEKAIJUDOCTEMPLETE_H_

class CSekaijuDocTemplate : public CMultiDocTemplate {
	DECLARE_DYNAMIC (CSekaijuDocTemplate)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
	// �R���X�g���N�^
public:
	CSekaijuDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
	CFrameWnd* CreateTrackListFrame (CDocument* pDoc);
	CFrameWnd* CreatePianoRollFrame (CDocument* pDoc);
	CFrameWnd* CreateEventListFrame (CDocument* pDoc);
	CFrameWnd* CreateMusicalScoreFrame (CDocument* pDoc);	
	
	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
	virtual void UpdateAllViews (CView* pSender,  LPARAM lHint = 0L, CObject* pHint = NULL);
	virtual CDocument* OpenDocumentFile (LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);

};

#endif
