//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���t�@�C���_�C�A���O�N���X
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
#include <afxcmn.h>
#include <afxext.h>
#include <afxdlgs.h>
#include <dlgs.h>       // for standard control IDs for commdlg
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "SekaijuFileDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define new DEBUG_NEW


//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^1
CSekaijuFileDlg::CSekaijuFileDlg (BOOL bOpenFileDialog) : 
	//20091018�C��:MFC8.0�ȑO�ɂ����ẮA���̃f�t�H���g�R���X�g���N�^���g�p����B
	CFileDialog (bOpenFileDialog) {
	//20091018�C��:MFC9.0�ɂ����āACFileDialog��m_bVistaStyle�ɋ����I��FALSE���w�肷��B
	//CFileDialog (bOpenFileDialog, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	NULL, NULL, 0, FALSE) {;
}

// �R���X�g���N�^2
CSekaijuFileDlg::CSekaijuFileDlg (BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
	LPCTSTR lpszFilter, CWnd* pParentWnd) : 
	//20091018�C��:MFC8.0�ȑO�ɂ����ẮA���̃f�t�H���g�R���X�g���N�^���g�p����B
	CFileDialog (bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd) {
	//20091018�C��:MFC9.0�ɂ����āACFileDialog��m_bVistaStyle�ɋ����I��FALSE���w�肷��B
	//CFileDialog (bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd,
	//	0, FALSE) {
}

//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// ���ӎ����FMSDN2002/4��OPENFILENAME�\���̂̐���
// For compatibility reasons, the Places Bar is hidden if Flags is set to 
// OFN_ENABLEHOOK and lStructSize is OPENFILENAME_SIZE_VERSION_400.
	
//int CSekaijuFileDlg::DoModal () {
	//m_ofn.Flags |= OFN_EXPLORER | OFN_ENABLEHOOK;
	//m_ofn.lpfnHook = (LPOFNHOOKPROC) SaveAsHookProc;
	//if (m_bOpenFileDialog) {
	//	return ::GetOpenFileName (&m_ofn);
	//}
	//else {
	//	return ::GetSaveFileName (&m_ofn);
	//}
//}


// �t�@�C���^�C�v�R���{�{�b�N�X���ύX���ꂽ�Ƃ�
void CSekaijuFileDlg::OnTypeChange () {
	// ���ӁFm_ofn.lFlags��OFN_EXPLOLER(0x00080000)��ǉ����Ă������ƁB
	// �ۑ��p�t�@�C���_�C�A���O�̂�
	if (!m_bOpenFileDialog) {
		// �t�@�C�����̊g���q���t�@�C���^�C�v�R���{�{�b�N�X(0x047c)�őI�������g���q�ɏ���������B
		CString strFileTitle = GetFileTitle ();
		if (!strFileTitle.IsEmpty ()) {
			CString strExt;
			if (AfxExtractSubString (strExt, m_ofn.lpstrFilter, 2 * m_ofn.nFilterIndex - 1, (TCHAR)'\0')) {
				LPTSTR pszExtension = _tcschr ((LPTSTR)(LPCTSTR)strExt, _T('.'));//PathFindExtension (strExt);
				if (_tcscmp (pszExtension, _T(".*")) != 0 && TCSLEN (pszExtension) >= 2) {
					// �g���q����͂��Ȃ������ꍇ�Ɏ����I�ɕt�������g���q��ύX
					m_strDefExt = CString (pszExtension + 1);
					m_ofn.lpstrDefExt =  m_strDefExt;
					// �t�@�C�����R���{�{�b�N�X�̊g���q�������ύX
					CString strText = strFileTitle + pszExtension;
					//if (!m_bVistaSytle) {
					SetControlText (0x047c, (LPSTR)(LPCTSTR)strText); 
					// ���ӁF0x047c��Windows2000/XP�X�^�C���̃t�@�C���_�C�A���O�ɂ�����t�@�C����ID
					SetControlText (0x0480, (LPSTR)(LPCTSTR)strText); 
					// ���ӁF0x0480��Windows95/98/ME�X�^�C���̃t�@�C���_�C�A���O�ɂ�����t�@�C����ID
					// TODO�F������WindowsVista�X�^�C���̃t�@�C���_�C�A���O�ɂ�����t�@�C����ID���L�q�B
					// (���O�����ĕۑ��_�C�A���O�̒��g�̃R���g���[��ID��spy++�Œ��ׂ�)
					//}
				}
			}
		}
	}
	CFileDialog::OnTypeChange ();
}

