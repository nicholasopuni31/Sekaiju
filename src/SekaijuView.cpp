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

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CSekaijuView, CView)

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CSekaijuView, CView)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CSekaijuView::CSekaijuView() {
}

// �f�X�g���N�^
CSekaijuView::~CSekaijuView () {
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// �E�B���h�E�X�^�C���̕ύX
BOOL CSekaijuView::PreCreateWindow (CREATESTRUCT& cs) {
	return CView::PreCreateWindow (cs);
}

// �`��(�_�~�[)
void CSekaijuView::OnDraw (CDC* pDC) {
	ASSERT_VALID (GetDocument());
}

// GetDocument(��f�o�b�O �o�[�W�����̓C�����C���ł��B)
#ifdef _DEBUG
CSekaijuDoc* CSekaijuView::GetDocument () {
	ASSERT (m_pDocument->IsKindOf(RUNTIME_CLASS(CSekaijuDoc)));
	return (CSekaijuDoc*)m_pDocument;
}

#endif
