//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���A�v���P�[�V�����N���X
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
#include "resource.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDocManager.h"
#include "SekaijuDocTemplate.h"
#include "SekaijuToolBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "PianoRollFrame.h"
#include "TrackListFrame.h"

#include "MIDIDeviceSheet.h"
#include "MIDISyncModeSheet.h"
#include "AutoSaveDlg.h"
#include "LanguageDlg.h"
#include "MetronomeDlg.h"
#include "OptionSheet.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP (CSekaijuApp, CWinApp)
	ON_COMMAND (ID_FILE_NEW, CWinApp::OnFileNew)
	ON_UPDATE_COMMAND_UI (ID_FILE_NEW, CSekaijuApp::OnUpdateFileNewUI)
	ON_COMMAND (ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_UPDATE_COMMAND_UI (ID_FILE_OPEN, CSekaijuApp::OnUpdateFileOpenUI)
	ON_COMMAND (ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND (ID_CONTROL_TOBEGIN, OnControlToBegin)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_TOBEGIN, OnUpdateControlToBeginUI)
	ON_COMMAND (ID_CONTROL_TOEND, OnControlToEnd)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_TOEND, OnUpdateControlToEndUI)
	ON_COMMAND (ID_CONTROL_PLAY, OnControlPlay)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_PLAY, OnUpdateControlPlayUI)
	ON_COMMAND (ID_CONTROL_RECORD, OnControlRecord)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_RECORD, OnUpdateControlRecordUI)
	ON_COMMAND (ID_CONTROL_PREVMEASURE, OnControlPrevMeasure)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_PREVMEASURE, OnUpdateControlPrevMeasureUI)
	ON_COMMAND (ID_CONTROL_NEXTMEASURE, OnControlNextMeasure)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_NEXTMEASURE, OnUpdateControlNextMeasureUI)
	ON_COMMAND (ID_CONTROL_PREVBEAT, OnControlPrevBeat)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_PREVBEAT, OnUpdateControlPrevBeatUI)
	ON_COMMAND (ID_CONTROL_NEXTBEAT, OnControlNextBeat)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_NEXTBEAT, OnUpdateControlNextBeatUI)
	ON_COMMAND (ID_CONTROL_SPEEDNONE, OnControlSpeedNone)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDNONE, OnUpdateControlSpeedNoneUI)
	ON_COMMAND (ID_CONTROL_SPEEDSLOW, OnControlSpeedSlow)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDSLOW, OnUpdateControlSpeedSlowUI)
	ON_COMMAND (ID_CONTROL_SPEEDNORMAL, OnControlSpeedNormal)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDNORMAL, OnUpdateControlSpeedNormalUI)
	ON_COMMAND (ID_CONTROL_SPEEDFAST, OnControlSpeedFast)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDFAST, OnUpdateControlSpeedFastUI)
	ON_COMMAND (ID_CONTROL_SPEEDSLAVE, OnControlSpeedSlave)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_SPEEDSLAVE, OnUpdateControlSpeedSlaveUI)
	ON_COMMAND (ID_CONTROL_AUTOREPEAT, OnControlAutoRepeat)
	ON_UPDATE_COMMAND_UI (ID_CONTROL_AUTOREPEAT, OnUpdateControlAutoRepeatUI)
	ON_COMMAND (ID_SETUP_MIDIDEVICE, OnSetupMIDIDevice)
	ON_COMMAND (ID_SETUP_MIDISYNCMODE, OnSetupMIDISyncMode)
	ON_COMMAND (ID_SETUP_INSTRUMENT, OnSetupInstrument)
	ON_COMMAND (ID_SETUP_METRONOME, OnSetupMetronome)
	ON_COMMAND (ID_SETUP_AUTOSAVE, OnSetupAutoSave)
	ON_COMMAND (ID_SETUP_LANGUAGE, OnSetupLanguage)
	ON_COMMAND (ID_SETUP_OPTIONS, OnSetupOptions)
	ON_COMMAND (ID_HELP_README, OnHelpReadMe)
	ON_COMMAND (ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND (ID_HELP_MANUAL, OnHelpManual)
	ON_COMMAND (ID_HELP_PROJECTHOMEPAGE, OnHelpProjectHomePage)
	ON_COMMAND (ID_HELP_ABOUT, OnHelpAbout)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CSekaijuApp::CSekaijuApp () {
	m_pPlayRecordThread = NULL;
	m_bPlayRecordThreadRunning = FALSE;	
	m_pCurChildWnd = NULL;
	m_pCurDocument = NULL;
	m_pOldChildWnd = NULL;
	m_pOldDocument = NULL;
	m_bPlaying = FALSE;
	m_bRecording = FALSE;
	m_lCurSpeedIndex = 2;
	m_lOldSpeedIndex = 2;
	long i;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_pMIDIIn[i] = NULL;
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_pMIDIOut[i] = NULL;
		m_pMIDIStatus[i] = NULL;
		m_pTempMIDIStatus[i] = NULL;
	}
	m_bIgnoreNoteEvent = FALSE;
	m_bFirstMetronome = FALSE;
	m_bInplaceEditing = FALSE;
	m_bInplaceListing = FALSE;
	m_bValueUpDowning = FALSE;
	// �f�t�H���g�̐F���w�肵�Ă���
	m_lDefTrackColor[0] = 0x00FF0000;
	m_lDefTrackColor[1] = 0x00808000;
	m_lDefTrackColor[2] = 0x00008000;
	m_lDefTrackColor[3] = 0x00008080;
	m_lDefTrackColor[4] = 0x000000F0;
	m_lDefTrackColor[5] = 0x008000F0;
	m_lDefTrackColor[6] = 0x00800080;
	m_lDefTrackColor[7] = 0x00FF0080;
	m_lDefTrackColor[8] = 0x00FF0000;
	m_lDefTrackColor[9] = 0x00808000;
	m_lDefTrackColor[10] = 0x00008000;
	m_lDefTrackColor[11] = 0x00008080;
	m_lDefTrackColor[12] = 0x000000F0;
	m_lDefTrackColor[13] = 0x008000F0;
	m_lDefTrackColor[14] = 0x00800080;
	m_lDefTrackColor[15] = 0x00FF0080;
}

//------------------------------------------------------------------------------
// �B��� CSekaijuApp �I�u�W�F�N�g
//------------------------------------------------------------------------------
CSekaijuApp theApp;


//------------------------------------------------------------------------------
// �I�[�o�[���C�h
//------------------------------------------------------------------------------

// CSekaijuApp �N���X�̏�����
BOOL CSekaijuApp::InitInstance () {

	// �R�����R���g���[���̏�����
	// ����InitCommonControls�́A
	// �f�o�b�O�Ŗ��̓����[�X�ł�sekaiju.exe.manufest�Ƌ��p�����Ƃ���
	// �G���[���N�����ċN���ł��Ȃ��Ȃ�ꍇ�́A�R�����g�A�E�g���邱�ƁB
	//InitCommonControls ();
	
#ifdef _AFXDLL
	Enable3dControls();			// ���LDLL�̒���MFC���g�p����ꍇ 
#else
	Enable3dControlsStatic();	// MFC�ƐÓI�Ƀ����N���Ă���ꍇ
#endif

	CString strMsg;
	CString strFileName;

	// EXE�t�@�C���̂���t���p�X�����擾
	TCHAR szPath[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	::GetModuleFileName (this->m_hInstance, szPath, TSIZEOF (szPath));
	_tsplitpath (szPath, szDrive, szDir, szFileName, szExt);
	m_strExeFilePath.Format (_T("%s%s"), szDrive, szDir);
	if (m_strExeFilePath.Right (7) == _T("\\Debug\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 6);
	}
	else if (m_strExeFilePath.Right (9) == _T("\\Release\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 8);
	}
	else if (m_strExeFilePath.Right (8) == _T("\\DebugU\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 7);
	}
	else if (m_strExeFilePath.Right (10) == _T("\\ReleaseU\\")) {
		m_strExeFilePath = m_strExeFilePath.Left (m_strExeFilePath.GetLength () - 9);
	}

	// ���������̃J�����g�f�B���N�g���擾
	memset (szPath, 0, sizeof (szPath));
	::GetCurrentDirectory (MAX_PATH, szPath);
	m_strInitialPath.Format (_T("%s"), szPath);

	// INI�t�@�C���̃t���p�X���̕ύX
	if (m_pszProfileName) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = NULL;
	}
	m_pszProfileName = new TCHAR[MAX_PATH + 1]; // 20120422�C��
	if (!m_pszProfileName) {
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_FOR_EXEFILENAME);
		AfxMessageBox (strMsg, MB_ICONSTOP);
		return FALSE;
	}
	memset ((void*)m_pszProfileName, 0, sizeof (TCHAR) * (MAX_PATH + 1)); // 20120422�C��
	_sntprintf ((TCHAR*)m_pszProfileName, MAX_PATH, _T("%s%s"), m_strExeFilePath, _T("Sekaiju.ini"));

	// �W����INI�t�@�C���̃I�v�V���������[�h����(�ŋߎg�����t�@�C�����܂�)
	LoadStdProfileSettings (16);  

	// INI�t�@�C������ݒ�̓ǂݍ���
	LoadIniFile ();

	// ���\�[�XDLL�̓ǂݍ���
	if (LoadResourceDLL () == FALSE) {
		return FALSE;
		// ���̊֐��̌��CSekaijuApp::ExitInstance���Ăяo�����B
	}

	// �A�v���P�[�V�������̕ύX(20100128:���\�[�X��DLL���ɔ����ǉ�)
	TCHAR szAppName[256];
	memset (szAppName, 0, sizeof (szAppName));
	VERIFY (::LoadString (m_hResourceDLL, AFX_IDS_APP_TITLE, szAppName, 255));
	free ((void*)m_pszAppName );
	m_pszAppName = NULL;
	m_pszAppName = _tcsdup (szAppName);
	if (m_pszAppName == NULL) {
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_FOR_EXEFILENAME);
		::MessageBox (NULL, strMsg, _T("Sekaiju"), MB_ICONSTOP);
		return FALSE;
	}
	AfxGetModuleState()->m_lpszCurrentAppName = m_pszAppName;

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������́B
	CCommandLineInfo cmdInfo;
	ParseCommandLine (cmdInfo); //20081019�ʒu�ύX

	// ��d�N���̖h�~
	if (CheckMultiExec (&cmdInfo) == FALSE) {
		return FALSE;
		// ���̊֐��̌��CSekaijuApp::ExitInstance���Ăяo�����B
	}

	// �Ō�ɊJ�����t�@�C���������I�ɊJ���ꍇ�AcmdInfo������������B
	if (m_theGeneralOption.m_bExecOpen == TRUE &&
		m_strLastOpenFileName[0] != _T("") &&
		cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
		// �Ō�ɊJ�����t�@�C�������݂���ꍇ�̂�(20081019)
		if (::GetFileAttributes (m_strLastOpenFileName[0]) != -1) {
			cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
			cmdInfo.m_strFileName = m_strLastOpenFileName[0];
		}
	}

	// ���ׂĂ�MIDI���̓f�o�C�X���J��
	OpenAllMIDIInDevice ();

	// ���ׂĂ�MIDI�o�̓f�o�C�X���J��
	OpenAllMIDIOutDevice ();

	// ���ׂĂ�MIDI�C���X�g�D�������g��`�t�@�C��(*.ins)��ǂݍ���
	LoadAllMIDIInstrument ();
	
	SelectAllMIDIInstDefNorm ();
	SelectAllMIDIInstDefDrum ();

	// �J�[�\���̓ǂݍ���
	m_hCursorArrow = LoadStandardCursor (IDC_ARROW);
	m_hCursorCross = LoadStandardCursor (IDC_CROSS);
	m_hCursorSizeWE = LoadStandardCursor (IDC_SIZEWE);
	m_hCursorSizeNS = LoadStandardCursor (IDC_SIZENS);
	m_hCursorSizeAll = LoadStandardCursor (IDC_SIZEALL);
	m_hCursorSizeAllCopy = LoadCursor (IDC_SIZEALLCOPY);
	m_hCursorNo = LoadStandardCursor (IDC_NO);
	m_hCursorResizeWE = LoadCursor (IDC_RESIZEWE);
	m_hCursorResizeNS = LoadCursor (IDC_RESIZENS);
	m_hCursorResizeAll = LoadCursor (IDC_RESIZEALL);
	m_hCursorDraw = LoadCursor (IDC_DRAW);
	m_hCursorLine = LoadCursor (IDC_LINE);
	m_hCursorEraser = LoadCursor (IDC_ERASER);
	m_hCursorSelect = LoadCursor (IDC_SELECT);
	m_hCursorSelectAdd = LoadCursor (IDC_SELECTADD);
	m_hCursorSelect2 = LoadCursor (IDC_SELECT2);
	m_hCursorSelectAdd2 = LoadCursor (IDC_SELECTADD2);
	m_hCursorSpeaker = LoadCursor (IDC_SPEAKER);

	// �C�x���g�̎�ޖ��̓ǂݍ���
	long i = 0;
	for (i = 0; i < 256; i++) {
		m_strEventKindName[i].LoadString (IDS_EVENTKIND_00 + i);
		if (m_strEventKindName[i] == _T("")) {
			m_strEventKindName[i].Format (_T("0x%02X"), i);
		}
	}

	// �L�[���̓ǂݍ���
	for (i = 0; i < 256; i++) {
		m_strNoteKeyName[i].LoadString (IDS_NOTEKEY_0S00 + i);
		if (m_strNoteKeyName[i] == _T("")) {
			m_strNoteKeyName[i].Format (_T("ERR"));
		}
	}

	// �f�t�H���g�t�H���g�̓ǂݍ���
	CString strDefaultFontName;
	VERIFY (strDefaultFontName.LoadString (IDS_DEFAULTFONTNAME));
	m_theDefaultFont.CreateFont (12, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strDefaultFontName);

	// �ꎞ�ϐ��̏�����
	m_bInplaceEditing = 0;
	m_bInplaceListing = 0;
	m_bValueUpDowning = 0;

	// �����ۑ��p�f�B���N�g���̍쐬
	CString strDirName1;
	CString strDirName2;
	strDirName1.LoadString (IDS_AUTOSAVEDIRNAME);
	strDirName2 = m_strExeFilePath + strDirName1;
	if (strDirName2.Right (1) == _T("\\")) {
		strDirName2 = strDirName2.Left (strDirName2.GetLength () - 1);
	}
	::CreateDirectory ((LPCTSTR)strDirName2, NULL);

	// MIDI�f�[�^�p�̃h�L�������g�e���v���[�g���쐬
	CSekaijuDocTemplate* pDocTemplate;
	pDocTemplate = new CSekaijuDocTemplate(
		IDR_SEKAIJUTYPE,
		RUNTIME_CLASS(CSekaijuDoc),
		RUNTIME_CLASS(CTrackListFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CSekaijuView));
	AddDocTemplate(pDocTemplate);
	m_pSekaijuDocTemplate = pDocTemplate;

	//DragAcceptFiles (TRUE); // �Ӗ��Ȃ�
	//EnableShellOpen(); // �Ӗ��Ȃ�

	// ���C��MDI�t���[���E�B���h�E���쐬
	// �E�B���h�E�N���X�����I���W�i���Ȃ��̂ɂ��邽�߂�LoadFrame�ł͂Ȃ�Create���g���B
	// ���ӁFLoadFrame�ō\�z�����CMainFrame::PreCreateWindow�ł̐ݒ肪���f����Ȃ��B
	// ���ӁFCreate�ō\�z�����ꍇ�A�f�t�H���g���j���[�ƃA�N�Z�����[�^�͕ʓr�ݒ肵�Ȃ���΂Ȃ�Ȃ��B
	CMainFrame* pMainFrame = new CMainFrame;
	if (pMainFrame->Create (
		NULL, // �E�B���h�E�N���X��(CMainFrame::PreCreateWindow���Őݒ肷��)
		this->m_pszAppName,  // �^�C�g��
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,  // �E�B���h�E�X�^�C��
		CRect (0, 0, 640, 400), // ���z�u(CMainFrame::PreCreateWindow���ōĐݒ肷��)
		NULL, // �e
		MAKEINTRESOURCE (IDR_MAINFRAME), // ���j���[
		0, // ExStyle 
		NULL) == FALSE) {
		return FALSE;
	}

	// �f�t�H���g���j���[�̐ݒ�(���C��MDI�t���[����Create�Ő�����������)
	ASSERT (pMainFrame->m_hWnd != NULL);
	pMainFrame->m_hMenuDefault = ::GetMenu (pMainFrame->m_hWnd);	
	
	// �A�N�Z�����[�^�̐ݒ�(���C��MDI�t���[����Create�Ő�����������)
	VERIFY (pMainFrame->LoadAccelTable (MAKEINTRESOURCE(IDR_MAINFRAME)));
	m_pMainWnd = pMainFrame;

	// ��͍ς݃R�}���h���C���𔽉f
	if (!ProcessShellCommand (cmdInfo)) {
		// �����I�ɍŌ�Ɏg�����t�@�C�����J���������s�����ꍇ
		if (m_theGeneralOption.m_bExecOpen == TRUE &&
			cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen &&
			cmdInfo.m_strFileName == m_strLastOpenFileName[0]) {
			m_strLastOpenFileName[0] = _T("");
			//AfxGetMainWnd ()->PostMessage (WM_COMMAND, ID_FILE_NEW, 0);
		}
		// �ʏ�̎��s�̏ꍇ
		else {
			return FALSE;
		}
	}

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���B
	if (m_theGeneralOption.m_bRestoreWindowPlacement) {
		if (m_theWindowPlacement.m_bIconic) {
			m_nCmdShow = SW_SHOWMINIMIZED;
		}
		else if (m_theWindowPlacement.m_bZoomed) {
			m_nCmdShow = SW_SHOWMAXIMIZED;
		}
	}
	pMainFrame->ShowWindow (m_nCmdShow);
	pMainFrame->UpdateWindow();

	// ���j���[�̍X�V
	UpdateMenu ();
	
	// �^�����t�p�X���b�h�̊J�n
	m_bPlayRecordThreadRunning = TRUE;
	m_pPlayRecordThread = AfxBeginThread (PlayRecordThread, this);
	if (m_pPlayRecordThread == NULL) {
		_RPTF1 (_CRT_WARN, "PlayRecordThread Create Failed, Address=0x%p.\n", m_pPlayRecordThread);
		// "�^���Đ��X���b�h���쐬���邱�Ƃ��ł��܂���B"
		strMsg.LoadString (IDS_RECPLAYTHREAD_CREATE_ERROR);
		AfxMessageBox (strMsg, MB_ICONSTOP);
	}
	else {
		_RPTF1 (_CRT_WARN, "PlayRecordThread Create Successful, Address=0x%p.\n", m_pPlayRecordThread);
	}
	
	return TRUE;
}

// �A�v���P�[�V�����̌㏈��
int CSekaijuApp::ExitInstance () {
	m_theCriticalSection.Lock ();
	m_bPlayRecordThreadRunning = FALSE;
	m_theCriticalSection.Unlock ();

	::Sleep (500);
	int nRet = CWinApp::ExitInstance ();

	// INI�t�@�C���֐ݒ��ۑ�
	SaveIniFile ();

	long i;
	
	// MIDI���̓f�o�C�X�����
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		if (m_pMIDIIn[i] != NULL) {
			MIDIIn_Close (m_pMIDIIn[i]);
			m_pMIDIIn[i] = NULL;
		}
	}

	// MIDI�o�̓f�o�C�X�����
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_pMIDIOut[i] != NULL) {
			MIDIOut_Close (m_pMIDIOut[i]);
			m_pMIDIOut[i] = NULL;
		}
		if (m_pMIDIStatus[i]) {
			MIDIStatus_Delete (m_pMIDIStatus[i]);
			m_pMIDIStatus[i] = NULL;
		}
		if (m_pTempMIDIStatus[i]) {
			MIDIStatus_Delete (m_pTempMIDIStatus[i]);
			m_pTempMIDIStatus[i] = NULL;
		}
	}

	// ���\�[�XDLL�̊J��
	FreeResourceDLL ();

	// INI�t�@�C���̃t���p�X�p�Ɋ��蓖�Ă�ꂽ���������������B
	if (m_pszProfileName) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = NULL;
	}

	// �A�v���P�[�V�������p�Ɋ��蓖�Ă�ꂽ���������������(20100202:���\�[�X��DLL���ɔ����ǉ�)
	if (m_pszAppName) {
		free ((void*)m_pszAppName);
		m_pszAppName = NULL;
	}

	::Sleep (500);

	return nRet;

}

// CWinApp::AddDocTemplate�̃I�[�o�[���C�h
void CSekaijuApp::AddDocTemplate (CDocTemplate* pTemplate) {
	if (m_pDocManager == NULL) {
		m_pDocManager = new CSekaijuDocManager;
	}
	m_pDocManager->AddDocTemplate (pTemplate);
}

// CWinApp::PreTranslateMessage�̃I�[�o�[���C�h20080809,20091222
BOOL CSekaijuApp::PreTranslateMessage (MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) {
		// �C���v���[�X�e�L�X�g���͎���������
		// �C���v���[�X���X�g�I������
		// �A�N�Z�����[�^�[�L�[�������Ȃ�����B
		if (m_bInplaceEditing || m_bInplaceListing) {
			return 0; // �ʏ��(�A�N�Z�����[�^�[����Ȃ�)����
		}
		// ����̃E�B���h�E��Ƀt�H�[�J�X������ꍇ�́A
		// ���̃E�B���h�E�̃L�[�����D�悷��(20091222�ǉ�)
		CWnd* pFocusedWnd = AfxGetMainWnd ()->GetFocus ();
		if (pFocusedWnd != NULL) {
			// �R���{�{�b�N�X�̏ꍇ��GetFocus()�ɂ���āA
			// �R���{�{�b�N�X����"Edit"�ւ̃|�C���^���擾�ł���̂ŁA
			// ���̐e�̃R���g���[��ID�𒲂ׂȂ���΂Ȃ�Ȃ��B
			//char szClassName[256];
			//memset (szClassName, 0, sizeof (szClassName));
			//HWND hFocusedWnd = pFocusedWnd->GetSafeHwnd ();
			//GetClassName (hFocusedWnd, szClassName, sizeof (szClassName));
			int nCtrlID = pFocusedWnd->GetDlgCtrlID ();
			CWnd* pParentWnd = pFocusedWnd->GetParent ();
			int nParentCtrlID = 0;
			if (pParentWnd) {
				nParentCtrlID = pParentWnd->GetDlgCtrlID ();
			}
			// �s�A�m���[���E�B���h�E�̃c�[���o�[�̃x���V�e�B�R���{�{�b�N�X�̏ꍇ
			// �s�A�m���[���E�B���h�E�̃c�[���o�[�̉������R���{�{�b�N�X�̏ꍇ
			// �C�x���g���X�g�E�B���h�E�̃c�[���o�[�̃^�C���G�f�B�b�g�̏ꍇ
			if (nParentCtrlID == IDC_VELOCITYCOMBO ||
				nParentCtrlID == IDC_DURATIONCOMBO ||
				nCtrlID == IDC_EVENTTIMEEDIT) {
				return 0; // �ʏ��(�A�N�Z�����[�^�[����Ȃ�)����
			}
			return CWinApp::PreTranslateMessage (pMsg); // �A�N�Z�����[�^�[���������
		}
		return CWinApp::PreTranslateMessage (pMsg); // �A�N�Z�����[�^�[���������
	}
	return CWinApp::PreTranslateMessage (pMsg); // �A�N�Z�����[�^�[���������
}

// �A�C�h�����O���̃I�[�o�[���C�h
BOOL CSekaijuApp::OnIdle (LONG lCount) {
	// �ʏ�̃A�C�h�����O����(�c�[���o�[�̕`��Ȃ�)
	BOOL bRet = CWinApp::OnIdle (lCount);
	// �c�[���o�[�̍X�V(�A�C�h�����O256���1��̊�����)
	if ((lCount & 0x000000FF) == 0) {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
		if (pMainFrame) {
			CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
			if (pSekaijuDoc) {
				if (pMainFrame->m_wndPositionScroll.GetSafeHwnd ()) {
					if (pMainFrame->m_wndPositionScroll.IsWindowEnabled () == 0) {
						//pMainFrame->m_wndPositionScroll.EnableWindow (1);
					}
				}
			}
			else {
				if (pMainFrame->m_wndPositionScroll.GetSafeHwnd ()) {
					if (pMainFrame->m_wndPositionScroll.IsWindowEnabled () != 0) {
						//pMainFrame->m_wndPositionScroll.EnableWindow (0);
					}
				}
			}
		}
	}
	// �����ۑ�(�A�C�h�����O256���1��̊�����)
	if (m_theAutoSaveDlgStatus.m_nOn && (lCount & 0x000000FF) == 0) {
		POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
		while (docpos) {
			CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
			// �h�L�������g���ύX����Ă���ꍇ�݂̂��ۑ����b�N����Ă��Ȃ��ꍇ�̂�
			if (pSekaijuDoc->IsModified () && !(pSekaijuDoc->m_bSaveLocked)) {
				// �����ۑ�����ׂ����������Ȃ��
				time_t tmCurTime;
				time (&tmCurTime);
				if (tmCurTime - pSekaijuDoc->m_tmLastAutoSaveTime > m_theAutoSaveDlgStatus.m_lInterval &&
					(m_theAutoSaveDlgStatus.m_nDisableWhilePlaying & m_bPlaying) == 0 &&
					(m_theAutoSaveDlgStatus.m_nDisableWhileRecording & m_bRecording) == 0) {
					// ����������ۑ�
					struct tm* pLocalTime = localtime (&tmCurTime);
					CString strFileName1;
					CString strFileName2;
					CString strFileName3;
					VERIFY (strFileName1.LoadString (IDS_AUTOSAVEFILEFORMAT));
					strFileName2.Format (
						strFileName1,
						pLocalTime->tm_year + 1900,
						pLocalTime->tm_mon + 1,
						pLocalTime->tm_mday,
						pLocalTime->tm_hour,
						pLocalTime->tm_min,
						pLocalTime->tm_sec,
						(ULONG)pSekaijuDoc);
					strFileName3 = m_strExeFilePath + strFileName2;
					long lRet = pSekaijuDoc->OnSaveDocument (strFileName3);
					// �����ۑ����s
					if (lRet == FALSE) {
						_RPTF1 (_CRT_WARN, "AutoSaveFile Failed. - \"%s\"\n", strFileName3);
					}
					// �����ۑ�����
					else {
						_RPTF1 (_CRT_WARN, "AutoSaveFile Successful. - \"%s\"\n", strFileName3);
						// �O�񎩓��ۑ������t�@�C�����폜(�ۑ��������őO��ۑ������t�@�C��������ꍇ�̂�)
						if (pSekaijuDoc->m_tmLastAutoSaveTime != 0) {
							pLocalTime = localtime (&(pSekaijuDoc->m_tmLastAutoSaveTime));
							VERIFY (strFileName1.LoadString (IDS_AUTOSAVEFILEFORMAT));
							strFileName2.Format (
								strFileName1,
								pLocalTime->tm_year + 1900,
								pLocalTime->tm_mon + 1,
								pLocalTime->tm_mday,
								pLocalTime->tm_hour,
								pLocalTime->tm_min,
								pLocalTime->tm_sec,
								(ULONG)pSekaijuDoc);
							strFileName3 = m_strExeFilePath + strFileName2;
							BOOL bRet = _tremove (strFileName3);
							if (bRet == FALSE) {
								_RPTF1 (_CRT_WARN, "AutoSaveFile Delete Failed. - \"%s\"\n", strFileName3);
							}
							else {
								_RPTF1 (_CRT_WARN, "AutoSaveFile Delete Successful. - \"%s\"\n", strFileName3);				
							}
						}
						// �O��ۑ������������X�V
						pSekaijuDoc->m_tmLastAutoSaveTime = tmCurTime;
						pSekaijuDoc->SetModifiedFlag (TRUE);
					}
				}
			}
		}
	}
	return bRet;
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// ���\�[�XDLL�̓ǂݍ���
BOOL CSekaijuApp::LoadResourceDLL () {
	if (m_strLanguage == _T("English")) {
		m_hResourceDLL = ::LoadLibrary (_T("SekaijuEnu.dll"));
		if (m_hResourceDLL == NULL) {
			::MessageBox (NULL, _T("SekaijuEnu.dll Load failed!!"), _T("Sekaiju"), MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	else {
		m_hResourceDLL = ::LoadLibrary (_T("SekaijuJpn.dll"));
		if (m_hResourceDLL == NULL) {
			::MessageBox (NULL, _T("SekaijuJpn.dll Load failed!!"), _T("Sekaiju"), MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	AfxSetResourceHandle (m_hResourceDLL);
	return TRUE;
}

// ���\�[�XDLL�̊J��
BOOL CSekaijuApp::FreeResourceDLL () {
	if (m_hResourceDLL) {
		::FreeLibrary (m_hResourceDLL);
	}
	return TRUE;
}

// ��d�N���̖h�~(�N���s�\�̏ꍇFALSE��Ԃ�)
BOOL CSekaijuApp::CheckMultiExec (CCommandLineInfo* pCmdInfo) {
	// ��d�N����������Ă��Ȃ��Ȃ��
	if (!m_theGeneralOption.m_bEnableMultiExec) {
		HWND hFindWnd = ::FindWindow (AfxGetAppName (), NULL);
		// ��d�N���ł���Ȃ��
		if (hFindWnd) {
			::SendMessage (hFindWnd, WM_COMMANDWAKEUP, NULL, NULL);
			// �t�@�C�������w�肳��Ă���ꍇ�A
			// ���ɋN�����Ă�����̃A�v���Ƀt�@�C������n���B
			if (pCmdInfo->m_strFileName != _T("")) {
				HANDLE hShare = CreateFileMapping
					 (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHMSIZE, AfxGetAppName ());
				if (hShare == NULL) {
					// ���L������(���M��)�I�[�v���G���[
					CString strMsg;
					VERIFY (strMsg.LoadString (IDS_SHAREMEMORY_FOR_SEND_OPEN_ERROR));
					AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
					return FALSE;
				}
				TCHAR* pShareMem = 
					(TCHAR*)::MapViewOfFile (hShare, FILE_MAP_WRITE, 0, 0, SHMSIZE);
				if (pShareMem == NULL) {
					// ���L������(���M��)�}�b�s���O�G���[
					::CloseHandle (hShare);
					CString strMsg;
					VERIFY (strMsg.LoadString (IDS_SHAREMEMORY_FOR_SEND_MAPPING_ERROR));
					AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
					return FALSE;
				}
				memset (pShareMem, 0, SHMSIZE);
				TCSNCPY (pShareMem, (LPCTSTR)(pCmdInfo->m_strFileName), SHMSIZE / sizeof(TCHAR) - 1);
				::UnmapViewOfFile (pShareMem);
				::SendMessage (hFindWnd, WM_COMMANDREADSHM, NULL, NULL);
				::CloseHandle (hShare);
				::PostMessage (hFindWnd, WM_COMMANDFILEOPEN, NULL, NULL);
			}
			return FALSE; 
			// TODO:MFC�̃��@�[�W�����ɂ���ẮAInitInstance����
			// return FALSE������ƃ��b�Z�[�W���o�Ă��܂��Ƃ�����������B
		}
		// ��d�N���łȂ�
		else {
			return TRUE;
		}
	}
	// ��d�N����������Ă���
	else {
		return TRUE;
	}
}

// INI�t�@�C������ݒ��ǂݍ���
BOOL CSekaijuApp::LoadIniFile () {
	long i;
	TCHAR szTemp[256];
	// [MIDIInDevice]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIInName[i] = GetProfileString (_T("MIDIInDevice"), szTemp, _T(""));
	}
	// [MIDIOutDevice]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIOutName[i] = GetProfileString (_T("MIDIOutDevice"), szTemp, _T(""));
	}
	if (m_strMIDIOutName[0] == _T("")) {
		m_strMIDIOutName[0] = _T("MIDI Mapper");
	}
	// [MIDIInstDefNorm]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIInstDefNormName[i] = GetProfileString (_T("MIDIInstDefNorm"), szTemp, 
			i == 0 ? _T("Microsoft GS Wavetable Synth") : _T(""));
	}
	// [MIDIInstDefDrum]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_strMIDIInstDefDrumName[i] = GetProfileString (_T("MIDIInstDefDrum"), szTemp,
			i == 0 ? _T("Microsoft GS Wavetable Synth Drumsets") : _T(""));
	}
	// [MIDIInSyncMode]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_lMIDIInSyncMode[i] = GetProfileInt (_T("MIDIInSyncMode"), szTemp, i == 0 ? 1 : 0);
	}
	// [MIDIOutSyncMode]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		m_lMIDIOutSyncMode[i] = GetProfileInt (_T("MIDIOutSyncMode"), szTemp, i == 0 ? 1 : 0);
	}

	// [WindowPlacement]
	m_theWindowPlacement.m_bIconic = (BOOL)GetProfileInt (_T("WindowPlacement"), _T("Iconic"), FALSE);
	m_theWindowPlacement.m_bZoomed = (BOOL)GetProfileInt (_T("WindowPlacement"), _T("Zoomed"), FALSE);
	m_theWindowPlacement.m_nX = GetProfileInt (_T("WindowPlacement"), _T("X"), CW_USEDEFAULT);
	m_theWindowPlacement.m_nY = GetProfileInt (_T("WindowPlacement"), _T("Y"), CW_USEDEFAULT);
	m_theWindowPlacement.m_nWidth = GetProfileInt (_T("WindowPlacement"), _T("Width"), CW_USEDEFAULT);
	m_theWindowPlacement.m_nHeight =GetProfileInt (_T("WindowPlacement"), _T("Height"), CW_USEDEFAULT);

	// [GeneralOption]
	m_theGeneralOption.m_bEnableMultiExec = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableMultiExec"), FALSE);
	m_theGeneralOption.m_bEnableMultiOpen = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableMultiOpen"), TRUE);
	m_theGeneralOption.m_bRestoreWindowPlacement = (BOOL)GetProfileInt (_T("GeneralOption"), _T("RestoreWindowPlacement"), TRUE);
	m_theGeneralOption.m_bExecOpen = (BOOL)GetProfileInt (_T("GeneralOption"), _T("ExecOpen"), FALSE);
	m_theGeneralOption.m_bOpenPlay = (BOOL)GetProfileInt (_T("GeneralOption"), _T("OpenPlay"), FALSE);
	m_theGeneralOption.m_bPlayUpdate = (BOOL)GetProfileInt (_T("GeneralOption"), _T("PlayUpdate"), TRUE);
	m_theGeneralOption.m_bSearchUpdate = (BOOL)GetProfileInt (_T("GeneralOption"), _T("SearchUpdate"), TRUE);
	m_theGeneralOption.m_bSearchSysx = (BOOL)GetProfileInt (_T("GeneralOption"), _T("SearchSysx"), TRUE);
	m_theGeneralOption.m_bEnableCC111Loop = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableCC111Loop"), TRUE);
	m_theGeneralOption.m_bPatchSearch = (BOOL)GetProfileInt (_T("GeneralOption"), _T("PatchSearch"), TRUE);
	m_theGeneralOption.m_bInvertCtrlMouseWheel = (BOOL)GetProfileInt (_T("GeneralOption"), _T("InvertCtrlMouseWheel"), FALSE);
	m_theGeneralOption.m_bTrackZeroOrigin = (BOOL)GetProfileInt (_T("GeneralOption"), _T("TrackZeroOrigin"), FALSE);
	m_theGeneralOption.m_bEventZeroOrigin = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EventZeroOrigin"), FALSE);
	m_theGeneralOption.m_bEnableAutoPageUpdate = (BOOL)GetProfileInt (_T("GeneralOption"), _T("EnableAutoPageUpdate"), TRUE);
	m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd = (BOOL)GetProfileInt (_T("GeneralOption"), _T("SendNoteOffHoldOffAtEnd"), TRUE);
	m_theGeneralOption.m_lUpDownDelta1 = GetProfileInt (_T("GeneralOption"), _T("UpDownDelta1"), 1);
	m_theGeneralOption.m_lUpDownDelta2 = GetProfileInt (_T("GeneralOption"), _T("UpDownDelta2"), 10);
	m_theGeneralOption.m_lKeyVelocity1 = GetProfileInt (_T("GeneralOption"), _T("KeyVelocity1"), 80);
	m_theGeneralOption.m_lKeyVelocity2 = GetProfileInt (_T("GeneralOption"), _T("KeyVelocity2"), 120);
	m_theGeneralOption.m_lSpeedSlow = GetProfileInt (_T("GeneralOption"), _T("SpeedSlow"), 5000);
	m_theGeneralOption.m_lSpeedNormal = GetProfileInt (_T("GeneralOption"), _T("SpeedNormal"), 10000);
	m_theGeneralOption.m_lSpeedFast = GetProfileInt (_T("GeneralOption"), _T("SpeedFast"), 20000);
	m_theGeneralOption.m_lPlayRecordInterval = GetProfileInt (_T("GeneralOption"), _T("PlayRecordInterval"), 5);
	m_theGeneralOption.m_lOctaveSignature = GetProfileInt (_T("GeneralOption"), _T("OctaveSignature"), 3);

	// [TrackListOption1]
	m_theTrackListOption1.m_lDefRowZoom = GetProfileInt (_T("TrackListOption1"), _T("DefRowZoom"), 24);
	m_theTrackListOption1.m_lDefColumnZoom = GetProfileInt (_T("TrackListOption1"), _T("DefColumnZoom"), 6);
	m_theTrackListOption1.m_lDefTimeZoom = GetProfileInt (_T("TrackListOption1"), _T("DefTimeZoom"), 6);
	m_theTrackListOption1.m_lDefNameWidth = GetProfileInt (_T("TrackListOption1"), _T("DefNameWidth"), 12);
	m_theTrackListOption1.m_lDefColorWidth = GetProfileInt (_T("TrackListOption1"), _T("DefColorWidth"), 2);
	m_theTrackListOption1.m_lDefInputOnWidth = GetProfileInt (_T("TrackListOption1"), _T("DefInputOnWidth"), 4);
	m_theTrackListOption1.m_lDefInputPortWidth = GetProfileInt (_T("TrackListOption1"), _T("DefInputPortWidth"), 16);
	m_theTrackListOption1.m_lDefInputChWidth = GetProfileInt (_T("TrackListOption1"), _T("DefInputChWidth"), 5);
	m_theTrackListOption1.m_lDefOutputOnWidth = GetProfileInt (_T("TrackListOption1"), _T("DefOutputOnWidth"), 4);
	m_theTrackListOption1.m_lDefOutputPortWidth = GetProfileInt (_T("TrackListOption1"), _T("DefOutputPortWidth"), 16);
	m_theTrackListOption1.m_lDefOutputChWidth = GetProfileInt (_T("TrackListOption1"), _T("DefOutputChWidth"), 5);
	m_theTrackListOption1.m_lDefViewModeWidth = GetProfileInt (_T("TrackListOption1"), _T("DefViewModeWidth"), 5);

	// [TrackListOption2]
	m_theTrackListOption2.m_lDefCC000Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC000Width"), 5);
	m_theTrackListOption2.m_lDefCC032Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC032Width"), 5);
	m_theTrackListOption2.m_lDefPCWidth = GetProfileInt (_T("TrackListOption2"), _T("DefPCWidth"), 16);
	m_theTrackListOption2.m_lDefCC007Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC007Width"), 5);
	m_theTrackListOption2.m_lDefCC010Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC010Width"), 5);
	m_theTrackListOption2.m_lDefCC091Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC091Width"), 5);
	m_theTrackListOption2.m_lDefCC093Width = GetProfileInt (_T("TrackListOption2"), _T("DefCC093Width"), 5);
	m_theTrackListOption2.m_lDefCC094Width = GetProfileInt (_T("(TrackListOption2"), _T("DefCC094Width"), 5);
	m_theTrackListOption2.m_lDefKeyShiftWidth = GetProfileInt (_T("TrackListOption2"), _T("DefKeyShiftWidth"), 5);
	m_theTrackListOption2.m_lDefVelShiftWidth = GetProfileInt (_T("TrackListOption2"), _T("DefVelShiftWidth"), 5);
	m_theTrackListOption2.m_lDefTimeShiftWidth = GetProfileInt (_T("TrackListOption2"), _T("DefTimeShiftWidth"), 5);
	m_theTrackListOption2.m_lDefNumEventWidth = GetProfileInt (_T("TrackListOption2"), _T("DefNumEventWidth"), 8);

	// [PianoRollOption]
	m_thePianoRollOption.m_lDefKeyZoom = GetProfileInt (_T("PianoRollOption"), _T("DefKeyZoom"), 6);
	m_thePianoRollOption.m_lDefVelZoom = GetProfileInt (_T("PianoRollOption"), _T("DefVelZoom"), 1);
	m_thePianoRollOption.m_lDefTimeZoom = GetProfileInt (_T("PianoRollOption"), _T("DefTimeZoom"), 6);

	// [EventListOption]
	m_theEventListOption.m_lDefRowZoom = GetProfileInt (_T("EventListOption"), _T("DefRowZoom"), 20);
	m_theEventListOption.m_lDefColumnZoom = GetProfileInt (_T("EventListOption"), _T("DefColumnZoom"), 6);
	m_theEventListOption.m_lDefTrackWidth = GetProfileInt (_T("EventListOption"), _T("DefTrackWidth"), 16);
	m_theEventListOption.m_lDefMillisecWidth = GetProfileInt (_T("EventListOption"), _T("DefMillisecWidth"), 16);
	m_theEventListOption.m_lDefTimeWidth = GetProfileInt (_T("EventListOption"), _T("DefTimeWidth"), 16);
	m_theEventListOption.m_lDefKindWidth = GetProfileInt (_T("EventListOption"), _T("DefKindWidth"), 16);
	m_theEventListOption.m_lDefChWidth = GetProfileInt (_T("EventListOption"), _T("DefChWidth"), 8);
	m_theEventListOption.m_lDefVal1Width = GetProfileInt (_T("EventListOption"), _T("DefVal1Width"), 16);
	m_theEventListOption.m_lDefVal2Width = GetProfileInt (_T("EventListOption"), _T("DefVal2Width"), 16);
	m_theEventListOption.m_lDefVal3Width = GetProfileInt (_T("EventListOption"), _T("DefVal3Width"), 16);
	m_theEventListOption.m_bInsertEventAfter = GetProfileInt (_T("EventListOption"), _T("InsertEventAfter"), 0);
	m_theEventListOption.m_bDuplicateEventAfter = GetProfileInt (_T("EventListOption"), _T("DuplicateEventAfter"), 0);
	m_theEventListOption.m_bDeleteEventAfter = GetProfileInt (_T("EventListOption"), _T("DeleteEventAfter"), 1);

	// [MusicalScoreOption]
	m_theMusicalScoreOption.m_lDefTrackZoom = GetProfileInt (_T("MusicalScoreOption"), _T("DefTrackZoom"), 4);
	m_theMusicalScoreOption.m_lDefTimeZoom = GetProfileInt (_T("MusicalScoreOption"), _T("DefTimeZoom"), 8);

	// [Control]
	m_bPlaying = FALSE;
	m_bRecording = FALSE;
	m_lCurSpeedIndex = GetProfileInt (_T("Control"), _T("CurSpeedIndex"), 2);
	m_bAutoRepeat = GetProfileInt (_T("Control"), _T("AutoRepeat"), TRUE);

	// [EditTrackDlgStatus]
	m_theEditTrackDlgStatus.m_nAmount = GetProfileInt (_T("EditTrackDlgStatus"), _T("Amount"), 0);
	m_theEditTrackDlgStatus.m_nUnit = GetProfileInt (_T("EditTrackDlgStatus"), _T("Unit"), 0);

	// [EditTimeDlgStatus]
	m_theEditTimeDlgStatus.m_nAmount = GetProfileInt (_T("EditTimeDlgStatus"), _T("Amount"), 0);
	m_theEditTimeDlgStatus.m_nUnit = GetProfileInt (_T("EditTimeDlgStatus"), _T("Unit"), 0);

	// [EditTimeSmpDlgStatus]
	m_theEditTimeSmpDlgStatus.m_nAmount = GetProfileInt (_T("EditTimeSmpDlgStatus"), _T("Amount"), 0);
	m_theEditTimeSmpDlgStatus.m_nUnit = GetProfileInt (_T("EditTimeSmpDlgStatus"), _T("Unit"), 0);

	// [EditChannelDlgStatus]
	m_theEditChannelDlgStatus.m_nAmount = GetProfileInt (_T("EditChannelDlgStatus"), _T("Amount"), 0);
	m_theEditChannelDlgStatus.m_nUnit = GetProfileInt (_T("EditChannelDlgStatus"), _T("Unit"), 0);

	// [EditKeyDlgStatus]
	m_theEditKeyDlgStatus.m_nAmount = GetProfileInt (_T("EditKeyDlgStatus"), _T("Amount"), 0);
	m_theEditKeyDlgStatus.m_nUnit = GetProfileInt (_T("EditKeyDlgStatus"), _T("Unit"), 0);
	m_theEditKeyDlgStatus.m_nTargetNote = GetProfileInt (_T("EditKeyDlgStatus"), _T("TargetNote"), 1);
	m_theEditKeyDlgStatus.m_nTargetKeyAfter = GetProfileInt (_T("EditKeyDlgStatus"), _T("TargetKeyAfter"), 1);
	
	// [EditVelocityDlgStatus]
	m_theEditVelocityDlgStatus.m_nAmount = GetProfileInt (_T("EditVelocityDlgStatus"), _T("Amount"), 0);
	m_theEditVelocityDlgStatus.m_nUnit = GetProfileInt (_T("EditVelocityDlgStatus"), _T("Unit"), 0);
	m_theEditVelocityDlgStatus.m_nTargetNoteOn = GetProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOn"), 1);
	m_theEditVelocityDlgStatus.m_nTargetNoteOff = GetProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOff"), 1);

	// [EditDurationDlgStatus]
	m_theEditDurationDlgStatus.m_nAmount = GetProfileInt (_T("EditDurationDlgStatus"), _T("Amount"), 0);
	m_theEditDurationDlgStatus.m_nUnit = GetProfileInt (_T("EditDurationDlgStatus"), _T("Unit"), 0);

	// [EditValueDlgStatus]
	m_theEditValueDlgStatus.m_nAmount = GetProfileInt (_T("EditValueDlgStatus"), _T("Amount"), 0);
	m_theEditValueDlgStatus.m_nUnit = GetProfileInt (_T("EditValueDlgStatus"), _T("Unit"), 0);
	m_theEditValueDlgStatus.m_nTargetKeyAfter = GetProfileInt (_T("EditValueDlgStatus"), _T("TargetKeyAfter"), 1);
	m_theEditValueDlgStatus.m_nTargetControlChange = GetProfileInt (_T("EditValueDlgStatus"), _T("ControlChange"), 1);
	m_theEditValueDlgStatus.m_nTargetChannelAfter = GetProfileInt (_T("EditValueDlgStatus"), _T("ChannelAfter"), 1);
	m_theEditValueDlgStatus.m_nTargetPitchBend = GetProfileInt (_T("EditValueDlgStatus"), _T("PitchBend"), 1);

	// [EditBreakupAndTrillDlgStatus]
	m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex = GetProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("DurationIndex"), 3);
	m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill = GetProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("EnableTrill"), 1);
	m_theEditBreakupAndTrillDlgStatus.m_nKeyShift = GetProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("KeyShift"), 2);

	// [EditQuantizeDlgStatus]
	m_theEditQuantizeDlgStatus.m_nSnapTimeIndex = GetProfileInt (_T("EditQuantizeDlgStatus"), _T("SnapTimeIndex"), 3);
	m_theEditQuantizeDlgStatus.m_nStrength = GetProfileInt (_T("EditQuantizeDlgStatus"), _T("Strength"), 100);
	m_theEditQuantizeDlgStatus.m_nTargetNoteOn = GetProfileInt (_T("EditQuantizeDlgStatus"), _T("TargetNoteOn"), 1);
	m_theEditQuantizeDlgStatus.m_nTargetNoteOff = GetProfileInt (_T("EditQuantizeyDlgStatus"), _T("TargetNoteOff"), 0);

	// [EditBeatScanDlgStatus]
	m_theEditBeatScanDlgStatus.m_nBeatTrackIndex = GetProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatTrackIndex"), 1);
	m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex = GetProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatIntervalIndex"), 0);
	m_theEditBeatScanDlgStatus.m_nInsertTempo = GetProfileInt (_T("EditBeatScanDlgStatus"), _T("InsertTempo"), 1);

	// [EditInsertMeasureDlgStatus]
	m_theEditInsertMeasureDlgStatus.m_nPosition = GetProfileInt (_T("EditInsertMeasureDlgStatus"), _T("Position"), 1);
	m_theEditInsertMeasureDlgStatus.m_nNumMeasure = GetProfileInt (_T("EditInsertMeasureDlgStatus"), _T("NumMeasure"), 1);

	// [EditRemoveMeasureDlgStatus]
	m_theEditRemoveMeasureDlgStatus.m_nPosition = GetProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("Position"), 1);
	m_theEditRemoveMeasureDlgStatus.m_nNumMeasure = GetProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("NumMeasure"), 1);

	// [MetronomeDlgStatus]
	m_theMetronomeDlgStatus.m_nOn = GetProfileInt (_T("MetronomeDlgStatus"), _T("On"), 1);
	m_theMetronomeDlgStatus.m_nOutputPort = GetProfileInt (_T("MetronomeDlgStatus"), _T("OutputPort"), 0);
	m_theMetronomeDlgStatus.m_nOutputChannel = GetProfileInt (_T("MetronomeDlgStatus"), _T("OutputChannel"), 9);
	m_theMetronomeDlgStatus.m_nNoteKey1 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey1"), 60);
	m_theMetronomeDlgStatus.m_nNoteVel1 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel1"), 120);
	m_theMetronomeDlgStatus.m_nNoteKey2 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey2"), 61);
	m_theMetronomeDlgStatus.m_nNoteVel2 = GetProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel2"), 120);

	// [AutoSaveDlgStatus]
	m_theAutoSaveDlgStatus.m_nOn = GetProfileInt (_T("AutoSaveDlgStatus"), _T("On"), 1);
	m_theAutoSaveDlgStatus.m_lInterval = GetProfileInt (_T("AutoSaveDlgStatus"), _T("Interval"), 600);
	m_theAutoSaveDlgStatus.m_nDisableWhilePlaying = GetProfileInt (_T("AutoSaveDlgStatus"), _T("DisableWhilePlaying"), 1);
	m_theAutoSaveDlgStatus.m_nDisableWhileRecording = GetProfileInt (_T("AutoSaveDlgStatus"),_T( "DisableWhileRecording"), 1);


	// [LastOpenFileName]
	m_strLastOpenFileName[0] = GetProfileString (_T("LastOpenFileName"), _T("File1"), _T(""));

	// [Language]
	m_strLanguage = GetProfileString (_T("Language"), _T("Language"), _T("Japanese"));
	return TRUE;
}

// INI�t�@�C���ɐݒ��ۑ�
BOOL CSekaijuApp::SaveIniFile () {
	long i;
	TCHAR szTemp[256];
	// [MIDIInDevice]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIInDevice"), szTemp, m_strMIDIInName[i]);
	}
	// [MIDIOutDevice]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIOutDevice"), szTemp, m_strMIDIOutName[i]);
	}
	// [MIDIInstDefNorm]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIInstDefNorm"), szTemp, m_strMIDIInstDefNormName[i]);
	}
	// [MIDIInstDefDrum]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileString (_T("MIDIInstDefDrum"), szTemp, m_strMIDIInstDefDrumName[i]);
	}
	// [MIDIInSyncMode]
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileInt (_T("MIDIInSyncMode"), szTemp, m_lMIDIInSyncMode[i]);
	}
	// [MIDIOutSyncMode]
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		memset (szTemp, 0, sizeof (szTemp));
		_sntprintf (szTemp, 255, _T("%03d"), i);
		WriteProfileInt (_T("MIDIOutSyncMode"), szTemp, m_lMIDIOutSyncMode[i]);
	}

	// [WindowPlacement]
	WriteProfileInt (_T("WindowPlacement"), _T("Iconic"),  m_theWindowPlacement.m_bIconic);
	WriteProfileInt (_T("WindowPlacement"), _T("Zoomed"),  m_theWindowPlacement.m_bZoomed);
	WriteProfileInt (_T("WindowPlacement"), _T("X"),  m_theWindowPlacement.m_nX);
	WriteProfileInt (_T("WindowPlacement"), _T("Y"),  m_theWindowPlacement.m_nY);
	WriteProfileInt (_T("WindowPlacement"), _T("Width"),  m_theWindowPlacement.m_nWidth);
	WriteProfileInt (_T("WindowPlacement"), _T("Height"),  m_theWindowPlacement.m_nHeight);

	// [GeneralOption]
	WriteProfileInt (_T("GeneralOption"), _T("EnableMultiExec"),  m_theGeneralOption.m_bEnableMultiExec);
	WriteProfileInt (_T("GeneralOption"), _T("EnableMultiOpen"), m_theGeneralOption.m_bEnableMultiOpen);
	WriteProfileInt (_T("GeneralOption"), _T("RestoreWindowPlacement"),  m_theGeneralOption.m_bRestoreWindowPlacement);
	WriteProfileInt (_T("GeneralOption"), _T("ExecOpen"),  m_theGeneralOption.m_bExecOpen);
	WriteProfileInt (_T("GeneralOption"), _T("OpenPlay"),  m_theGeneralOption.m_bOpenPlay);
	WriteProfileInt (_T("GeneralOption"), _T("PlayUpdate"), m_theGeneralOption.m_bPlayUpdate);
	WriteProfileInt (_T("GeneralOption"), _T("SearchUpdate"), m_theGeneralOption.m_bSearchUpdate);
	WriteProfileInt (_T("GeneralOption"), _T("SearchSysx"), m_theGeneralOption.m_bSearchSysx);
	WriteProfileInt (_T("GeneralOption"), _T("EnableCC111Loop"), m_theGeneralOption.m_bEnableCC111Loop);
	WriteProfileInt (_T("GeneralOption"), _T("PatchSearch"), m_theGeneralOption.m_bPatchSearch);
	WriteProfileInt (_T("GeneralOption"), _T("InvertCtrlMouseWheel"), m_theGeneralOption.m_bInvertCtrlMouseWheel);
	WriteProfileInt (_T("GeneralOption"), _T("TrackZeroOrigin"), m_theGeneralOption.m_bTrackZeroOrigin);
	WriteProfileInt (_T("GeneralOption"), _T("EventZeroOrigin"), m_theGeneralOption.m_bEventZeroOrigin);
	WriteProfileInt (_T("GeneralOption"), _T("EnableAutoPageUpdate"), m_theGeneralOption.m_bEnableAutoPageUpdate);
	WriteProfileInt (_T("GeneralOption"), _T("SendNoteOffHoldOffAtEnd"), m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd);
	WriteProfileInt (_T("GeneralOption"), _T("UpDownDelta1"), m_theGeneralOption.m_lUpDownDelta1);
	WriteProfileInt (_T("GeneralOption"), _T("UpDownDelta2"), m_theGeneralOption.m_lUpDownDelta2);
	WriteProfileInt (_T("GeneralOption"), _T("KeyVelocity1"), m_theGeneralOption.m_lKeyVelocity1);
	WriteProfileInt (_T("GeneralOption"), _T("KeyVelocity2"), m_theGeneralOption.m_lKeyVelocity2);
	WriteProfileInt (_T("GeneralOption"), _T("SpeedSlow"), m_theGeneralOption.m_lSpeedSlow);
	WriteProfileInt (_T("GeneralOption"), _T("SpeedNormal"), m_theGeneralOption.m_lSpeedNormal);
	WriteProfileInt (_T("GeneralOption"), _T("SpeedFast"), m_theGeneralOption.m_lSpeedFast);
	WriteProfileInt (_T("GeneralOption"), _T("PlayRecordInterval"), m_theGeneralOption.m_lPlayRecordInterval);
	WriteProfileInt (_T("GeneralOption"), _T("OctaveSignature"), m_theGeneralOption.m_lOctaveSignature);

	// [TrackListOption1]
	WriteProfileInt (_T("TrackListOption1"), _T("DefRowZoom"), m_theTrackListOption1.m_lDefRowZoom);
	WriteProfileInt (_T("TrackListOption1"), _T("DefColumnZoom"), m_theTrackListOption1.m_lDefColumnZoom);
	WriteProfileInt (_T("TrackListOption1"), _T("DefTimeZoom"), m_theTrackListOption1.m_lDefTimeZoom);
	WriteProfileInt (_T("TrackListOption1"), _T("DefNameWidth"), m_theTrackListOption1.m_lDefNameWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefColorWidth"), m_theTrackListOption1.m_lDefColorWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefInputOnWidth"), m_theTrackListOption1.m_lDefInputOnWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefInputPortWidth"), m_theTrackListOption1.m_lDefInputPortWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefInputChWidth"), m_theTrackListOption1.m_lDefInputChWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefOutputOnWidth"), m_theTrackListOption1.m_lDefOutputOnWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefOutputPortWidth"), m_theTrackListOption1.m_lDefOutputPortWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefOutputChWidth"), m_theTrackListOption1.m_lDefOutputChWidth);
	WriteProfileInt (_T("TrackListOption1"), _T("DefViewModeWidth"), m_theTrackListOption1.m_lDefViewModeWidth);

	// [TrackListOption2]
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC000Width"), m_theTrackListOption2.m_lDefCC000Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC032Width"), m_theTrackListOption2.m_lDefCC032Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefPCWidth"), m_theTrackListOption2.m_lDefPCWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC007Width"), m_theTrackListOption2.m_lDefCC007Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC010Width"), m_theTrackListOption2.m_lDefCC010Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC091Width"), m_theTrackListOption2.m_lDefCC091Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC093Width"), m_theTrackListOption2.m_lDefCC093Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefCC094Width"), m_theTrackListOption2.m_lDefCC094Width);
	WriteProfileInt (_T("TrackListOption2"), _T("DefKeyShiftWidth"), m_theTrackListOption2.m_lDefKeyShiftWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefVelShiftWidth"), m_theTrackListOption2.m_lDefVelShiftWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefTimeShiftWidth"), m_theTrackListOption2.m_lDefTimeShiftWidth);
	WriteProfileInt (_T("TrackListOption2"), _T("DefNumEventWidth"), m_theTrackListOption2.m_lDefNumEventWidth);

	// [PianoRollOption]
	WriteProfileInt (_T("PianoRollOption"), _T("DefKeyZoom"), m_thePianoRollOption.m_lDefKeyZoom);
	WriteProfileInt (_T("PianoRollOption"), _T("DefVelZoom"), m_thePianoRollOption.m_lDefVelZoom);
	WriteProfileInt (_T("PianoRollOption"), _T("DefTimeZoom"), m_thePianoRollOption.m_lDefTimeZoom);

	// [EventListOption]
	WriteProfileInt (_T("EventListOption"), _T("DefRowZoom"), m_theEventListOption.m_lDefRowZoom);
	WriteProfileInt (_T("EventListOption"), _T("DefColumnZoom"), m_theEventListOption.m_lDefColumnZoom);
	WriteProfileInt (_T("EventListOption"), _T("DefTrackWidth"), m_theEventListOption.m_lDefTrackWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefMillisecWidth"), m_theEventListOption.m_lDefMillisecWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefTimeWidth"), m_theEventListOption.m_lDefTimeWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefKindWidth"), m_theEventListOption.m_lDefKindWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefChWidth"), m_theEventListOption.m_lDefChWidth);
	WriteProfileInt (_T("EventListOption"), _T("DefVal1Width"), m_theEventListOption.m_lDefVal1Width);
	WriteProfileInt (_T("EventListOption"), _T("DefVal2Width"), m_theEventListOption.m_lDefVal2Width);
	WriteProfileInt (_T("EventListOption"), _T("DefVal3Width"), m_theEventListOption.m_lDefVal3Width);
	WriteProfileInt (_T("EventListOption"), _T("InsertEventAfter"), m_theEventListOption.m_bInsertEventAfter);
	WriteProfileInt (_T("EventListOption"), _T("DuplicateEventAfter"), m_theEventListOption.m_bDuplicateEventAfter);
	WriteProfileInt (_T("EventListOption"), _T("DeleteEventAfter"), m_theEventListOption.m_bDeleteEventAfter);
	
	// [MusicalScoreOption]
	WriteProfileInt (_T("MusicalScoreOption"), _T("DefTrackZoom"), m_theMusicalScoreOption.m_lDefTrackZoom);
	WriteProfileInt (_T("MusicalScoreOption"), _T("DefTimeZoom"), m_theMusicalScoreOption.m_lDefTimeZoom);

	// [Control]
	WriteProfileInt (_T("Control"), _T("Playing"),m_bPlaying);
	WriteProfileInt (_T("Control"), _T("Recording"), m_bRecording);
	WriteProfileInt (_T("Control"), _T("CurSpeedIndex"), m_lCurSpeedIndex);
	WriteProfileInt (_T("Control"), _T("AutoRepeat"), m_bAutoRepeat);
	
	// [EditTrackDlgStatus]
	WriteProfileInt (_T("EditTrackDlgStatus"), _T("Amount"), m_theEditTrackDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditTrackDlgStatus"), _T("Unit"), m_theEditTrackDlgStatus.m_nUnit);
	
	// [EditTimeDlgStatus]
	WriteProfileInt (_T("EditTimeDlgStatus"), _T("Amount"), m_theEditTimeDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditTimeDlgStatus"), _T("Unit"), m_theEditTimeDlgStatus.m_nUnit);

	// [EditTimeSmpDlgStatus]
	WriteProfileInt (_T("EditTimeSmpDlgStatus"), _T("Amount"), m_theEditTimeSmpDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditTimeSmpDlgStatus"), _T("Unit"), m_theEditTimeSmpDlgStatus.m_nUnit);

	// [EditChannelDlgStatus]
	WriteProfileInt (_T("EditChannelDlgStatus"), _T("Amount"), m_theEditChannelDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditChannelDlgStatus"), _T("Unit"), m_theEditChannelDlgStatus.m_nUnit);

	// [EditKeyDlgStatus]
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("Amount"), m_theEditKeyDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("Unit"), m_theEditKeyDlgStatus.m_nUnit);
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("TargetNote"), m_theEditKeyDlgStatus.m_nTargetNote);
	WriteProfileInt (_T("EditKeyDlgStatus"), _T("TargetKeyAfter"), m_theEditKeyDlgStatus.m_nTargetKeyAfter);
	
	// [EditVelocityDlgStatus]
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("Amount"), m_theEditVelocityDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("Unit"), m_theEditVelocityDlgStatus.m_nUnit);
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOn"), m_theEditVelocityDlgStatus.m_nTargetNoteOn);
	WriteProfileInt (_T("EditVelocityDlgStatus"), _T("TargetNoteOff"), m_theEditVelocityDlgStatus.m_nTargetNoteOff);

	// [EditDurationDlgStatus]
	WriteProfileInt (_T("EditDurationDlgStatus"), _T("Amount"), m_theEditDurationDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditDurationDlgStatus"), _T("Unit"), m_theEditDurationDlgStatus.m_nUnit);

	// [EditValueDlgStatus]
	WriteProfileInt (_T("EditValueDlgStatus"), _T("Amount"), m_theEditValueDlgStatus.m_nAmount);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("Unit"), m_theEditValueDlgStatus.m_nUnit);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("TargetKeyAfter"), m_theEditValueDlgStatus.m_nTargetKeyAfter);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("ControlChange"), m_theEditValueDlgStatus.m_nTargetControlChange);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("ChannelAfter"), m_theEditValueDlgStatus.m_nTargetChannelAfter);
	WriteProfileInt (_T("EditValueDlgStatus"), _T("PitchBend"), m_theEditValueDlgStatus.m_nTargetPitchBend);
	
	// [EditBreakupAndTrillDlgStatus]
	WriteProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("DurationIndex"), m_theEditBreakupAndTrillDlgStatus.m_nDurationIndex);
	WriteProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("EnableTrill"), m_theEditBreakupAndTrillDlgStatus.m_nEnableTrill);
	WriteProfileInt (_T("EditBreakupAndTrillDlgStatus"), _T("KeyShift"), m_theEditBreakupAndTrillDlgStatus.m_nKeyShift);

	// [EditQuantizeDlgStatus]
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("SnapTimeIndex"), m_theEditQuantizeDlgStatus.m_nSnapTimeIndex);
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("Strength"), m_theEditQuantizeDlgStatus.m_nStrength);
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("TargetNoteOn"), m_theEditQuantizeDlgStatus.m_nTargetNoteOn);
	WriteProfileInt (_T("EditQuantizeDlgStatus"), _T("TargetNoteOff"), m_theEditQuantizeDlgStatus.m_nTargetNoteOff);

	// [EditBeatScanDlgStatus]
	WriteProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatTrackIndex"), m_theEditBeatScanDlgStatus.m_nBeatTrackIndex);
	WriteProfileInt (_T("EditBeatScanDlgStatus"), _T("BeatIntervalIndex"), m_theEditBeatScanDlgStatus.m_nBeatIntervalIndex);
	WriteProfileInt (_T("EditBeatScanDlgStatus"), _T("InsertTempo"), m_theEditBeatScanDlgStatus.m_nInsertTempo);

	// [EditInsertMeasureDlgStatus]
	WriteProfileInt (_T("EditInsertMeasureDlgStatus"), _T("Position"), m_theEditInsertMeasureDlgStatus.m_nPosition);
	WriteProfileInt (_T("EditInsertMeasureDlgStatus"), _T("NumMeasure"), m_theEditInsertMeasureDlgStatus.m_nNumMeasure);

	// [EditRemoveMeasureDlgStatus]
	WriteProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("Position"), m_theEditRemoveMeasureDlgStatus.m_nPosition);
	WriteProfileInt (_T("EditRemoveMeasureDlgStatus"), _T("NumMeasure"), m_theEditRemoveMeasureDlgStatus.m_nNumMeasure);

	// [MetronomeDlgStatus]
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("On"), m_theMetronomeDlgStatus.m_nOn);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("OutputPort"), m_theMetronomeDlgStatus.m_nOutputPort);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("OutputChannel"), m_theMetronomeDlgStatus.m_nOutputChannel);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey1"), m_theMetronomeDlgStatus.m_nNoteKey1);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel1"), m_theMetronomeDlgStatus.m_nNoteVel1);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteKey2"), m_theMetronomeDlgStatus.m_nNoteKey2);
	WriteProfileInt (_T("MetronomeDlgStatus"), _T("NoteVel2"), m_theMetronomeDlgStatus.m_nNoteVel2);
	
	// [AutoSaveDlgStatus]
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("On"), m_theAutoSaveDlgStatus.m_nOn);
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("Interval"), m_theAutoSaveDlgStatus.m_lInterval);
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("DisableWhilePlaying"), m_theAutoSaveDlgStatus.m_nDisableWhilePlaying);
	WriteProfileInt (_T("AutoSaveDlgStatus"), _T("DisableWhileRecording"), m_theAutoSaveDlgStatus.m_nDisableWhileRecording);

	// [LastOpenFileName]
	WriteProfileString (_T("LastOpenFileName"), _T("File1"), m_strLastOpenFileName[0]);

	// [Language]
	WriteProfileString (_T("Language"), _T("Language"), m_strLanguage);

	return TRUE;
}


// ���ׂĂ�MIDI�C���X�g�D�������g��`�t�@�C��(*.ins)��ǂݍ���
long CSekaijuApp::LoadAllMIDIInstrument () {
	long i = 0;
	// ����MIDI�C���X�g�D�������g���ǂݍ��܂ꂢ�Ă���ꍇ�͍폜
	for (i = 0; i < MAXMIDIINSTRUMENTNUM; i++) {
		if (m_pMIDIInstrument[i]) {
			MIDIInstrument_Delete (m_pMIDIInstrument[i]);
			m_pMIDIInstrument[i] = NULL;
		}
	}
	// ���MIDI�C���X�g�D�������g�𐶐� // 20100220�p�~
	//m_pMIDIInstrument = MIDIInstrument_Create ();
	//if (m_pMIDIInstrument == NULL) {
	//	//_RPTF0 (_CRT_WARN, "�G���[�FOnCreate����MIDI�C���X�g�D�������g��ǂݍ��߂܂���B\n");
	//	//MessageBox (g_hMainWnd, MIDIINSTRUMENT_CANT_LOAD, APPNAME, MB_ICONSTOP);
	//	return 0;
	//}
	// �g���q(*.ins)�̃t�@�C����S�ēǂݍ���
	i = 0;
	CString strFileName1;
	CString strFileName2;
	VERIFY (strFileName1.LoadString (IDS_INSTRUMENTFILENAME));
	strFileName2 = m_strExeFilePath + strFileName1;
	HANDLE hFind = NULL;
	WIN32_FIND_DATA fd;
	hFind = ::FindFirstFile ((LPCTSTR)strFileName2, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (_tcscmp (fd.cFileName, _T(".")) != 0 &&
				_tcscmp (fd.cFileName, _T("..")) != 0) {
				CString strFileName3;
				CString strFileName4;
				VERIFY (strFileName3.LoadString (IDS_INSTRUMENTDIRNAME));
				strFileName4 = m_strExeFilePath + strFileName3 + fd.cFileName;
				m_pMIDIInstrument[i] = MIDIInstrument_Load ((LPCTSTR)strFileName4); // 20100220�ύX
				if (m_pMIDIInstrument[i] == NULL) {
					_RPTF1 (_CRT_WARN, "MIDIInstDefFile Load Failed. - \"%s\"\n", strFileName4);
				}
				else {
					_RPTF1 (_CRT_WARN, "MIDIInstDefFile Load Successful. - \"%s\"\n", strFileName4);
				}
				i++;
			}
		} while (::FindNextFile (hFind, &fd) && i < MAXMIDIINSTRUMENTNUM); // 20100220�����ǉ�
		::FindClose (hFind);
	}
	return i;
}


long CSekaijuApp::SelectAllMIDIInstDefNorm () {
	long lPort = 0;
	long j = 0;
	CString strMsg;
	CString strMsg2;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		// �C���X�g�D�������g��`���̂��C���X�g�D�������g��`�ւ̃|�C���^���擾
		for (j = 0; j < MAXMIDIINSTRUMENTNUM; j++) {
			if (m_pMIDIInstrument[j] != NULL) {
				m_pMIDIInstDefNorm[lPort] = MIDIInstrument_GetInstrumentDefinitionFromTitle 
					(m_pMIDIInstrument[j], m_strMIDIInstDefNormName[lPort]);
				if (m_pMIDIInstDefNorm[lPort] != NULL) {
					break;
				}
			}
		}
		// �C���X�g�D�������g��`����������
		if (m_pMIDIInstDefNorm[lPort] != NULL) {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Successful. - \"%s\"\n", 
				lPort, m_strMIDIInstDefNormName[lPort]);
		}
		// �C���X�g�D�������g��`��������Ȃ�����
		else {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Failed. - \"%s\"\n", 
				lPort, m_strMIDIInstDefNormName[lPort]);
			if (m_strMIDIInstDefNormName[lPort] != _T("")) { // 20080829�C��
				// %s\n-MIDI�C���X�g�D�������g��`[%d]��������܂���B
				strMsg.LoadString (IDS_S_N_MIDIINSTDEF_D_FIND_FAILED);
				strMsg2.Format (strMsg, m_strMIDIInstDefNormName[lPort], lPort + 1);
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
			}
			m_strMIDIInstDefNormName[lPort] = _T("");
		}
	}
	return 1;
}

long CSekaijuApp::SelectAllMIDIInstDefDrum () {
	long lPort = 0;
	long j = 0;
	CString strMsg;
	CString strMsg2;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		// �C���X�g�D�������g��`���̂��C���X�g�D�������g��`�ւ̃|�C���^���擾
		for (j = 0; j < MAXMIDIINSTRUMENTNUM; j++) {
			if (m_pMIDIInstrument[j] != NULL) {
				m_pMIDIInstDefDrum[lPort] = MIDIInstrument_GetInstrumentDefinitionFromTitle 
					(m_pMIDIInstrument[j], m_strMIDIInstDefDrumName[lPort]);
				if (m_pMIDIInstDefDrum[lPort] != NULL) {
					break;
				}
			}
		}
		// �C���X�g�D�������g��`����������
		if (m_pMIDIInstDefDrum[lPort] != NULL) {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Successful. - \"%s\"\n", 
				lPort, m_strMIDIInstDefDrumName[lPort]);
		}
		// �C���X�g�D�������g��`��������Ȃ�����
		else {
			_RPTF2 (_CRT_WARN, "MIDIInstDef[%d] Find Failed. - \"%s\"\n", 
				lPort, m_strMIDIInstDefDrumName[lPort]);
			if (m_strMIDIInstDefDrumName[lPort] != _T("")) { // 20080829�C��
				// %s\n-MIDI�C���X�g�D�������g��`[%d]��������܂���B
				strMsg.LoadString (IDS_S_N_MIDIINSTDEF_D_FIND_FAILED);
				strMsg2.Format (strMsg, m_strMIDIInstDefDrumName[lPort], lPort + 1);
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
			}
			m_strMIDIInstDefDrumName[lPort] = _T("");
		}
	}
	return 1;
}

//-----------------------------------------------------------------------------
// �I�y���[�V����
//-----------------------------------------------------------------------------

// ���j���[�̍X�V
void CSekaijuApp::UpdateMenu () {
	CMenu* pMenu = AfxGetMainWnd ()->GetMenu ();
	CString str1;
	CString str2;
	str1.LoadString (IDS_CONTROL_SPEEDSLOW);
	str2.Format (str1, m_theGeneralOption.m_lSpeedSlow / 100);
	pMenu->ModifyMenu (ID_CONTROL_SPEEDSLOW, MF_BYCOMMAND, ID_CONTROL_SPEEDSLOW, (LPCTSTR)str2);
	str1.LoadString (IDS_CONTROL_SPEEDNORMAL);
	str2.Format (str1, m_theGeneralOption.m_lSpeedNormal / 100);
	pMenu->ModifyMenu (ID_CONTROL_SPEEDNORMAL, MF_BYCOMMAND, ID_CONTROL_SPEEDNORMAL, (LPCTSTR)str2);
	str1.LoadString (IDS_CONTROL_SPEEDFAST);
	str2.Format (str1, m_theGeneralOption.m_lSpeedFast / 100);
	pMenu->ModifyMenu (ID_CONTROL_SPEEDFAST, MF_BYCOMMAND, ID_CONTROL_SPEEDFAST, (LPCTSTR)str2);
}

// ���݂̃E�B���h�E�ƌ��݂̃h�L�������g�ւ̃|�C���^���X�V
void CSekaijuApp::UpdateCurWndAndDocPtr () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	// �J�����g�q�E�B���h�E�̍X�V
	m_pCurChildWnd = (CMDIChildWnd*)(pMainFrame->GetActiveFrame ());
	// �ȑO�̃h�L�������g�ƃJ�����g�h�L�������g�̍X�V
	if (m_pCurChildWnd) {
		m_pOldDocument = m_pCurDocument;
		m_pCurDocument = m_pCurChildWnd->GetActiveDocument ();
	}
	else {
		m_pOldDocument = m_pCurDocument;
		m_pCurDocument = NULL;
	}
	// �J�����g�h�L�������g���ύX���ꂽ�ꍇ
	if (m_pCurDocument != m_pOldDocument) {
		CSekaijuDoc* pOldSekaijuDoc = (CSekaijuDoc*)m_pOldDocument;
		CSekaijuDoc* pCurSekaijuDoc = (CSekaijuDoc*)m_pCurDocument;
		// �h�L�������g������ꍇ�N���e�B�J���Z�N�V�������b�N
		if (pOldSekaijuDoc) {
			pOldSekaijuDoc->m_theCriticalSection.Lock ();
		}
		if (pCurSekaijuDoc) {
			pCurSekaijuDoc->m_theCriticalSection.Lock ();
		}
		this->m_theCriticalSection.Lock ();
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
		//CScrollBar* pPositionScroll = &(pMainFrame->m_wndPositionScroll);
		// ���t�p�h�L�������g�E�L�^�p�h�L�������g�̕ύX����
		// �Â��h�L�������g������ꍇ
		if (pOldSekaijuDoc) {
			// �^�����̏ꍇ�͋�����~(20080709�ǉ�)(20081003�C��)
			if (m_bRecording) {
				// ���ӁF�^�����ɓˑR����ꂽ�ꍇpCurHistoryUnit�擾�s�B
				// ���ӁF�����PostNcDestroy����Ăяo�����BCSekaijuDoc::OnCloseDocument����B
				// ���ӁF�����CSekaijuDoc::OnCloseDocument��StopRecording���Ă������ƁB
				StopRecording (pOldSekaijuDoc);
			}
			// ���h�L�������g��MIDI�N���b�N�̋�����~
			if (pOldSekaijuDoc->m_pMIDIClock) {
				MIDIClock_Stop (pOldSekaijuDoc->m_pMIDIClock);
			}
			// �����̋�����~
			SendAllNoteOff ();
			SendAllHold1Off ();
			SendAllSostenutoOff ();
			SendAllHold2Off ();
			SendAllSoundOff ();
			// �V�����h�L�������g������ꍇ
			if (pCurSekaijuDoc) {
				// �|�W�V�����X�N���[���o�[�͈̔͐ݒ�
				long lEndTime = MIDIData_GetEndTime (pCurSekaijuDoc->m_pMIDIData);
				pMainFrame->SetPositionScrollRange (0, lEndTime, TRUE);
				// MIDI�N���b�N��s�X�s�[�h�Ɠ������[�h���A�v���P�[�V�����ɍ��킹��(20090630�p�~)
				// pCurSekaijuDoc->ApplyAppCurSpeedIndex ();
				// ���t�����͘^�����̏ꍇ�́AMIDIStatus���X�V����B
				if (m_bPlaying || m_bRecording) {
					ResetTempMIDIStatusArray ();
					pCurSekaijuDoc->TimeMIDIStatus 
						(pCurSekaijuDoc->m_lOldTime, m_pTempMIDIStatus);
					long lFlags = m_bPlaying ? SDS_ALL : SDS_ALL & ~SDS_NOTE;
					m_bIgnoreNoteEvent = TRUE; // 20090625�ǉ�
					SendDifferentStatus (lFlags);
				}
				// ���t���̏ꍇ�́A�V�h�L�������g�̉��t���J�n����B
				// �^�����̏ꍇ�́A�V�h�L�������g�̘^�����J�n����B
				if (m_bPlaying || m_bRecording) {
					MIDIClock_Start (pCurSekaijuDoc->m_pMIDIClock);
				}
			}
			// �V�����h�L�������g���Ȃ��ꍇ
			else {
				m_bPlaying = FALSE;
				m_bRecording = FALSE;
			}
		}
		this->m_theCriticalSection.Unlock ();
		// �h�L�������g������ꍇ�N���e�B�J���Z�N�V�������
		if (m_pOldDocument) {
			((CSekaijuDoc*)m_pOldDocument)->m_theCriticalSection.Unlock ();
		}
		if (m_pCurDocument) {
			((CSekaijuDoc*)m_pCurDocument)->m_theCriticalSection.Unlock ();
		}
	}
	_RPTF2 (_CRT_WARN, 
		"CMainFrame::UpdateCurWndAndDocPtr : m_pCurDocument=%p, m_pOldDocument=%p.\n",
		m_pCurDocument, m_pOldDocument);
}

// MIDI�������̓|�[�g�ԍ��Ɠ������̓��[�h���擾����(�Ȃ����FALSE��Ԃ�)
BOOL CSekaijuApp::GetCurSyncInputPortAndMode (long* pPort, long* pMode) {
	long lSyncPort = 0;
	long lSyncMode = 0;
	for (lSyncPort = 0; lSyncPort < MAXMIDIINDEVICENUM; lSyncPort++) {
		if (m_lMIDIInSyncMode[lSyncPort] == 1 || m_lMIDIInSyncMode[lSyncPort] == 2) {
			*pPort = lSyncPort;
			*pMode = m_lMIDIInSyncMode[lSyncPort];
			return TRUE;
		}
	}
	*pPort = 0;
	*pMode = 0;
	return FALSE;
}



// PlayRecordThread-���t�E�L�^�p�X���b�h 
UINT CSekaijuApp::PlayRecordThread (LPVOID pInfo) {
	// ����:�ÓI�ȃ����o�֐���this�|�C���^�ɃA�N�Z�X���邱�Ƃ��ł��Ȃ��B
	CString strMsg;
	strMsg = ("CSekaijuApp::PlayRecordThread Started.\n");
	_RPTF0 (_CRT_WARN, strMsg);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)pInfo;
	while (pSekaijuApp->m_bPlayRecordThreadRunning != FALSE) {
		pSekaijuApp->PlayRecordProc (pInfo);
		::Sleep (pSekaijuApp->m_theGeneralOption.m_lPlayRecordInterval);
	}
	strMsg = ("CSekaijuApp::PlayRecordThread Finished.\n");
	_RPTF0 (_CRT_WARN, strMsg);
	return 1;
}


// PlayRecordProc(��5�~���b������CSekaijuApp::PlayRecordThread����Ăяo�����B)
BOOL CSekaijuApp::PlayRecordProc (LPVOID pInfo) {
	BYTE byMsg[1024];
	long lLen;
	long i, j;
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)pInfo;
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)m_pCurDocument;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pSekaijuDoc == NULL || pSekaijuApp == NULL) {
		return FALSE;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return FALSE;
	}
	// �N���e�B�J���Z�N�V�������b�N
	pSekaijuDoc->m_theCriticalSection.Lock ();
	pSekaijuApp->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	MIDIClock* pMIDIClock = pSekaijuDoc->m_pMIDIClock;
	if (pMIDIData == NULL || pMIDIClock == NULL) {
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		pSekaijuApp->m_theCriticalSection.Unlock ();
		return FALSE;
	}
	long lCurTime = 0;
	long lCurMillisec = 0;
	lCurTime = MIDIClock_GetTickCount (pMIDIClock);
	lCurMillisec = MIDIClock_GetMillisec (pMIDIClock);
	pSekaijuDoc->m_lNewTime = lCurTime;
	long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
	long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);

	// �f�[�^�̓��͏���(�L�^�p)
	// �eMIDI���̓f�o�C�X�ɂ���
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		// MIDI���̓f�o�C�X���L���Ȃ��
		if (m_pMIDIIn[i] != NULL) {
			long lMIDIInSyncMode = pSekaijuApp->m_lMIDIInSyncMode[i];
			// MIDI���b�Z�[�W�̎擾�ł������
			while (1) {
				// MIDI���b�Z�[�W�̎擾
				memset (byMsg, 0, sizeof (byMsg));
				lLen = MIDIIn_GetMIDIMessage (m_pMIDIIn[i], byMsg, sizeof (byMsg));
				if (lLen <= 0) {
					break;
				}
				// �A�N�e�B�u�Z���V���O�̏ꍇ
				if (byMsg[0] == 0xFE) {
					;
				}
				// MIDI�^�C���R�[�h�N�H�[�^�[�t���[���̏ꍇ
				else if (byMsg[0] == 0xF1 || byMsg[0] == 0xF0) {
					if (lMIDIInSyncMode >= 2) {
						if (pMIDIClock) {
							MIDIClock_PutMIDIMessage (pMIDIClock, byMsg, lLen);
						}
					}
				}
				// MIDI�^�C�~���O�N���b�N�̏ꍇ
				else if (byMsg[0] == 0xF8) {
					if (lMIDIInSyncMode == 1) {
						if (pMIDIClock) {
							MIDIClock_PutMIDIMessage (pMIDIClock, byMsg, lLen);
						}
					}
				}
				// �\���O�|�W�V�����Z���N�^�̏ꍇ
				else if (byMsg[0] == 0xF2) {
					if (lMIDIInSyncMode == 1) {
						long lVal = ((byMsg[2] & 0x7F) << 7) | (byMsg[1] & 0x7F);
						long lTickCount = lVal * lTimeResolution / 4;
						SetPlayPosition (pSekaijuDoc, lTickCount);
					}
				}
				// �X�^�[�g�̏ꍇ
				else if (byMsg[0] == 0xFA && pSekaijuDoc) {
					if (pMIDIClock && !m_bPlaying && !m_bRecording) {
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_TOBEGIN, 0);
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
					else if (pMIDIClock && m_bPlaying && !m_bRecording) { // 20090704�ǉ�
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_TOBEGIN, 0);
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
				}
				// �R���e�B�j���[�̏ꍇ
				else if (byMsg[0] == 0xFB && pSekaijuDoc) {
					if (pSekaijuDoc->m_pMIDIClock && !m_bPlaying && !m_bRecording) {
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
				}
				// �X�g�b�v�̏ꍇ
				else if (byMsg[0] == 0xFC && pSekaijuDoc) {
					if (pSekaijuDoc->m_pMIDIClock && (m_bPlaying || m_bRecording)) {
						pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_PLAY, 0);
					}
				}
				// MIDI�`�����l�����b�Z�[�W�̏ꍇ
				else if (0x80 <= byMsg[0] && byMsg[0] <= 0xEF && pSekaijuDoc) {
					// �V���N���X�^�[�g����
					if (m_bRecording && m_lCurSpeedIndex == 0) {
						m_lCurSpeedIndex = m_lOldSpeedIndex;
						MIDIClock_Stop (pMIDIClock);
						pSekaijuDoc->ApplyAppCurSpeedIndex ();
						MIDIClock_Start (pMIDIClock);
					}
					// ���A���^�C�����͏���
					MIDITrack* pMIDITrack = NULL;
					forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
						long lInputOn = MIDITrack_GetInputOn (pMIDITrack);
						long lInputPort = MIDITrack_GetInputPort (pMIDITrack);
						long lInputChannel = MIDITrack_GetInputChannel (pMIDITrack);
						long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
						long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
						long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
						if (lInputOn && lInputPort == i && lInputChannel == (byMsg[0] & 0x0F)) {
							// �o�̓`�����l���̕ϊ�
							BYTE byMsg2[sizeof(byMsg)];
							memset (byMsg2, 0, sizeof (byMsg));
							memcpy (byMsg2, byMsg, sizeof (byMsg));
							if (0 <= lOutputChannel && lOutputChannel < 16) {
								byMsg2[0] &= 0xF0;
								byMsg2[0] |= (BYTE)lOutputChannel;
							}
							// ���o������(���t���łȂ����^�����łȂ��ꍇ�̂�)
							if (!(m_bPlaying && m_bRecording)) {
								MIDIOut* pMIDIOut = NULL;
								if (0 <= lOutputPort && lOutputPort < MAXMIDIOUTDEVICENUM) {
									pMIDIOut = m_pMIDIOut[lOutputPort];
								}
								if (pMIDIOut) {
									//TODO:MIDI���b�Z�[�W�����[�v�o�b�N����ꍇ���L�𖳌��ɂ��Ă��������B
									MIDIOut_PutMIDIMessage (pMIDIOut, byMsg2, lLen);
								}
							}
							// ����MIDI���b�Z�[�W��MIDI�f�[�^�ɋL�^
							if (m_bRecording) {
								MIDIEvent* pMIDIEvent = MIDIEvent_Create
									(pSekaijuDoc->m_lNewTime, (byMsg[0] & 0xF0), byMsg2, lLen);
								if (pMIDIEvent) {
									//pMIDIEvent->m_lUserFlag |= MIDIEVENT_REALTIMEGENERATE;
									MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent);
									// �m�[�g�I�t�̏ꍇ�̃m�[�g�I���Ƃ̌�������
									if (MIDIEvent_IsNoteOff (pMIDIEvent)) {
										long lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
										long j;
										for (j = lTempRecordedEventCount - 1; j >= 0; j--) {
											MIDIEvent* pTempEvent = 
												(MIDIEvent*)(m_theTempRecordedEventArray.GetAt (j));
											if (MIDIEvent_IsNoteOn (pTempEvent) &&
												pTempEvent->m_pNextCombinedEvent == NULL) {
												if (MIDIEvent_GetChannel (pTempEvent) == 
													MIDIEvent_GetChannel (pMIDIEvent)) {
													if (MIDIEvent_GetKey (pTempEvent) ==
														MIDIEvent_GetKey (pMIDIEvent)) {
														pTempEvent->m_pNextCombinedEvent = pMIDIEvent;
														pMIDIEvent->m_pPrevCombinedEvent = pTempEvent;
													}
												}
											}
										}
										//MIDIEvent_Combine (pMIDIEvent);
									}
									m_theTempRecordedEventArray.Add (pMIDIEvent);
								}
							}
						}
					}
				}
				// �V�X�e���G�N�X�N���[�V�u���b�Z�[�W�̏ꍇ
				else if (byMsg[0] == 0xF0 || byMsg[0] == 0xF7) {
					// �V���N���X�^�[�g����
					if (m_bRecording && m_lCurSpeedIndex == 0) {
						m_lCurSpeedIndex = m_lOldSpeedIndex;
						MIDIClock_Stop (pMIDIClock);
						pSekaijuDoc->ApplyAppCurSpeedIndex ();
						MIDIClock_Start (pMIDIClock);
					}
					// ���A���^�C�����͏���
					MIDITrack* pMIDITrack = NULL;
					forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
						long lInputOn = MIDITrack_GetInputOn (pMIDITrack);
						long lInputPort = MIDITrack_GetInputPort (pMIDITrack);
						long lInputChannel = MIDITrack_GetInputChannel (pMIDITrack);
						long lOutputOn = MIDITrack_GetOutputOn (pMIDITrack);
						long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
						long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
						if (lInputOn && lInputPort == i && lInputChannel == -1) {
							// ���o������(���t���łȂ����^�����łȂ��ꍇ�̂�)
							if (!(m_bPlaying && m_bRecording)) {
								MIDIOut* pMIDIOut = NULL;
								if (0 <= lOutputPort && lOutputPort < MAXMIDIOUTDEVICENUM) {
									pMIDIOut = m_pMIDIOut[lOutputPort];
								}
								if (pMIDIOut) {
									//TODO:MIDI���b�Z�[�W�����[�v�o�b�N����ꍇ���L�𖳌��ɂ��Ă��������B
									MIDIOut_PutMIDIMessage (pMIDIOut, byMsg, lLen);
								}
							}
							// ����MIDI���b�Z�[�W��MIDI�f�[�^�ɋL�^
							if (m_bRecording) {
								MIDITrack_InsertSysExEvent
									(pMIDITrack, pSekaijuDoc->m_lNewTime, byMsg, lLen);
							}
						}
					}
				}
			}
		}
	}

	// MIDI�^�C�~���O�N���b�N��SMPTE/MTC�̑��o����(20090624�ǉ�)
	if (m_bPlaying) {
		long lOutputPort;
		// MIDI�^�C�~���O�N���b�N���v�Z(20090620�ǉ�)
		long lNumMIDITimingClock = 0;
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			long lOldClock = pSekaijuDoc->m_lOldTime * 24 / lTimeResolution;
			long lNewClock = pSekaijuDoc->m_lNewTime * 24 / lTimeResolution;
			lNumMIDITimingClock = lNewClock - lOldClock;
			lNumMIDITimingClock = MAX (0, lNumMIDITimingClock);
		}
		// MIDI�^�C�~���O�N���b�N�̍쐬(20090620�ǉ�)
		memset (byMsg, 0, sizeof (byMsg));
		byMsg[0] = 0xF8;
		// MIDI�^�C�~���O�N���b�N�o�͏���(20090620�ǉ�)
		for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
			if (pSekaijuApp->m_lMIDIOutSyncMode[lOutputPort] == 1) {
				if (m_pMIDIOut[lOutputPort]) {
					for (j = 0; j < lNumMIDITimingClock; j++) {
						MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
					}
				}
				if (m_pMIDIStatus[lOutputPort]) {
					for (j = 0; j < lNumMIDITimingClock; j++) {
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
					}
				}
			}
		}
		// �O��̎��F���F�b�F�t���[���i�[�p�ϐ�
		static long lOldSec;
		static long lOldMinute;
		static long lOldHour;
		static long lOldFrame[4];
		// ���݂̎��F���F�b�F�t���[���v�Z(20090624�ǉ�)
		long lMillisec = lCurMillisec % 1000;
		long lSec = (lCurMillisec / 1000) % 60;
		long lMinute = (lCurMillisec / 60000) % 60;
		long lHour = (lCurMillisec / 3600000);
		long lFrame[4] = {
			lMillisec * 24 / 1000, lMillisec * 25 / 1000, 
			lMillisec * 2997 / 100000, lMillisec * 30 / 1000}; 
		// SMPTE24,SMPTE25,SMPTE29.97,SMPTE30�p��MIDI�^�C���R�[�h�N�H�[�^�[�t���[�����쐬
		memset (byMsg, 0, sizeof (byMsg));
		long lFrameRate;
		for (lFrameRate = 0; lFrameRate < 4; lFrameRate++) {
			byMsg[lFrameRate * 16 + 0] = 0xF1;
			byMsg[lFrameRate * 16 + 1] = 0x00 | (lFrame[lFrameRate] & 0x0F);
			byMsg[lFrameRate * 16 + 2] = 0xF1;
			byMsg[lFrameRate * 16 + 3] = 0x10 | ((lFrame[lFrameRate] >> 4) & 0x0F);
			byMsg[lFrameRate * 16 + 4] = 0xF1;
			byMsg[lFrameRate * 16 + 5] = 0x20 | (lSec & 0x0F);
			byMsg[lFrameRate * 16 + 6] = 0xF1;
			byMsg[lFrameRate * 16 + 7] = 0x30 | ((lSec >> 4) & 0x0F);
			byMsg[lFrameRate * 16 + 8] = 0xF1;
			byMsg[lFrameRate * 16 + 9] = 0x40 | (lMinute & 0x0F);
			byMsg[lFrameRate * 16 + 10] = 0xF1;
			byMsg[lFrameRate * 16 + 11] = 0x50 | ((lMinute >> 4) & 0x0F);
			byMsg[lFrameRate * 16 + 12] = 0xF1;
			byMsg[lFrameRate * 16 + 13] = 0x60 | (lHour & 0x0F);
			byMsg[lFrameRate * 16 + 14] = 0xF1;
			byMsg[lFrameRate * 16 + 15] = 0x70 | ((lFrameRate & 0x03) << 1) | ((lHour >> 4) & 0x01); 
		}
		// MIDI�^�C���R�[�h�N�H�[�^�[�t���[���o�͏���(20090624�ǉ�)
		for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
			lFrameRate = pSekaijuApp->m_lMIDIOutSyncMode[lOutputPort] - 2;
			if (0 <= lFrameRate && lFrameRate <= 3) { 
				// ���݂̎��F���F�b�F�t���[�����O��̎��F���F�b�F�t���[���ƈقȂ�ꍇ�̂�
				if (lFrameRate == 0 && (lFrame[0] != lOldFrame[0] || lSec != lOldSec || lMinute != lOldMinute) || 
					lFrameRate == 1 && (lFrame[1] != lOldFrame[1] || lSec != lOldSec || lMinute != lOldMinute) ||
					lFrameRate == 2 && (lFrame[2] != lOldFrame[2] || lSec != lOldSec || lMinute != lOldMinute) ||
					lFrameRate == 3 && (lFrame[3] != lOldFrame[3] || lSec != lOldSec || lMinute != lOldMinute)) {
					if (m_pMIDIOut[lOutputPort]) {
						for (j = 0; j < 8; j++) {
							long lPos = lFrameRate * 16 + j * 2;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], &byMsg[lPos], 2);
						}
					}
					if (m_pMIDIStatus[lOutputPort]) {
						for (j = 0; j < 8; j++) {
							long lPos = lFrameRate * 16 + j * 2;
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], &byMsg[lPos], 2);
						}
					}
				}
			}
		}
		// �O��̎��F���F�b�F�t���[���Ɍ��݂̎��F���F�b�F�t���[�����i�[
		lOldSec = lSec;
		lOldMinute = lMinute;
		lOldHour = lHour;
		lOldFrame[0] = lFrame[0];
		lOldFrame[1] = lFrame[1];
		lOldFrame[2] = lFrame[2];
		lOldFrame[3] = lFrame[3];
	}

	// ���g���m�[���o�͏���(20080825�ǉ�)
	if (m_bRecording && m_theMetronomeDlgStatus.m_nOn) {
		if (lTimeMode == MIDIDATA_TPQNBASE) {
			long lOldMeasure, lOldBeat, lOldTick;
			MIDIData_BreakTime (pMIDIData, 
				pSekaijuDoc->m_lOldTime, &lOldMeasure, &lOldBeat, &lOldTick);
			long lNewMeasure, lNewBeat, lNewTick;
			MIDIData_BreakTime (pMIDIData, 
				pSekaijuDoc->m_lNewTime, &lNewMeasure, &lNewBeat, &lNewTick);
			long lOutputPort = CLIP (0, m_theMetronomeDlgStatus.m_nOutputPort, 15);
			long lOutputChannel = CLIP (0, m_theMetronomeDlgStatus.m_nOutputChannel, 15);
			// �ŏ���1���ߖڂ̋��ł�炷���߂̃g���b�N
			if (pSekaijuDoc->m_lOldTime == pSekaijuDoc->m_lNewTime && 
				lOldBeat == 0 && lOldTick == 0 && this->m_bFirstMetronome == TRUE) {
				lOldMeasure --;
			}
			m_bFirstMetronome = FALSE;
			long lMeasure = 0;
			for (lMeasure = lOldMeasure; lMeasure < lNewMeasure; lMeasure++) {
				memset (byMsg, 0, sizeof (byMsg));
				byMsg[0] = 0x90 | (BYTE)lOutputChannel;
				byMsg[1] = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey1, 127);
				byMsg[2] = CLIP (1, m_theMetronomeDlgStatus.m_nNoteVel1, 127);
				if (m_pMIDIOut[lOutputPort]) {
					MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
				}
				byMsg[2] = 0;
				if (m_pMIDIOut[lOutputPort]) {
					MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
				}
			}
			long lBeat = 0;
			for (lBeat = lOldBeat; lBeat < lNewBeat; lBeat++) {
				if (1) {
					memset (byMsg, 0, sizeof (byMsg));
					byMsg[0] = 0x90 | (BYTE)lOutputChannel;
					byMsg[1] = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey2, 127);
					byMsg[2] = CLIP (1, m_theMetronomeDlgStatus.m_nNoteVel2, 127);
					if (m_pMIDIOut[lOutputPort]) {
						MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
					}
					byMsg[2] = 0;
					if (m_pMIDIOut[lOutputPort]) {
						MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
					}
				}
			}
		}
	}

	// MIDI�f�[�^�̏o�͏���(���t�p)
	if (m_bPlaying) {
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		// �e�g���b�N�ɂ���
		forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
			// ���̃g���b�N�̏o�͂�ON�Ȃ��
			if (MIDITrack_GetOutputOn (pMIDITrack)) {
				long lOutputPort = MIDITrack_GetOutputPort (pMIDITrack);
				long lOutputChannel = MIDITrack_GetOutputChannel (pMIDITrack);
				forEachEvent (pMIDITrack, pMIDIEvent) {
					long lTime = MIDIEvent_GetTime (pMIDIEvent);
					lTime = CLIP (0, lTime + MIDITrack_GetTimePlus (pMIDITrack), 0x7FFFFFFF);
					if (pSekaijuDoc->m_lOldTime <= lTime && lTime < pSekaijuDoc->m_lNewTime) {
						// MIDI�`�����l���C�x���g�̏ꍇ
						if (MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
							memset (byMsg, 0, sizeof (byMsg));
							lLen = MIDIEvent_GetLen (pMIDIEvent);
							MIDIEvent_GetData (pMIDIEvent, byMsg, lLen);
							// �o�̓`�����l�����ݒ肳��Ă���ꍇ�͂��̃`�����l���ɏ]��
							if (0 <= lOutputChannel && lOutputChannel < 16) {
								byMsg[0] &= 0xF0;
								byMsg[0] |= (BYTE)lOutputChannel;
							}
							// �L�[+�ƃx���V�e�B+�̔��f
							if (MIDIEvent_IsNoteOn (pMIDIEvent)) {
								byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
								byMsg[2] = (BYTE)CLIP (1, byMsg[2] + MIDITrack_GetVelocityPlus (pMIDITrack), 127);
							}
							else if (MIDIEvent_IsNoteOff (pMIDIEvent) || MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
								byMsg[1] = (BYTE)CLIP (0, byMsg[1] + MIDITrack_GetKeyPlus (pMIDITrack), 127);
							}
							// ���t�ʒu�𓮂����Ĕ�����Ԃ𕜌���������͓�d�����E��d������h�~����
							if (m_bIgnoreNoteEvent && 
								!(MIDIEvent_IsNoteOn (pMIDIEvent) && lTime == pSekaijuDoc->m_lOldTime)  &&
								!(MIDIEvent_IsNoteOff (pMIDIEvent) && lTime == pSekaijuDoc->m_lOldTime) ||
								m_bIgnoreNoteEvent == FALSE) {
								if (m_pMIDIOut[lOutputPort]) {
									MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, lLen);
								}
								if (m_pMIDIStatus[lOutputPort]) {
									MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, lLen);
								}
							}
							else {
								//_RPTF3 (_CRT_WARN, "PutMIDIMessage Ignored (%02x%02x%02x...)\n", 
								//	byMsg[0], byMsg[1], byMsg[2]);
							}
						}
						// �V�X�e���G�N�X�N���[�V�u�C�x���g�̏ꍇ
						else if (MIDIEvent_IsSysExEvent (pMIDIEvent)) {
							memset (byMsg, 0, sizeof (byMsg));
							lLen = MIDIEvent_GetLen (pMIDIEvent);
							MIDIEvent_GetData (pMIDIEvent, byMsg, lLen);
							if (m_pMIDIOut[lOutputPort]) {
								MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, lLen);
							}
							if (m_pMIDIStatus[lOutputPort]) {
								MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, lLen);
							}
						}
						// �e���|�C�x���g�̏ꍇ
						else if (MIDIEvent_IsTempo (pMIDIEvent)) {
							long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
							if (pMIDIClock) {
								MIDIClock_SetTempo (pMIDIClock, lTempo);
							}
						}
					}
				}
			}
		}
		// ��\�����h�~�p��m_bIngoreNoteEvent�t���O�̃N���A
		if (pSekaijuDoc->m_lOldTime != pSekaijuDoc->m_lNewTime) {
			m_bIgnoreNoteEvent = FALSE;
		}
		// ���t�͈̓^�C���̍X�V
		pSekaijuDoc->m_lOldTime = pSekaijuDoc->m_lNewTime;
		// �^�����łȂ��ꍇ���I�[�ɒB�����ꍇ
		long lEndTime = MIDIData_GetEndTime (pSekaijuDoc->m_pMIDIData);
		if (!m_bRecording &&
			pSekaijuDoc->m_lOldTime > lEndTime) {
			// �I�[�g���s�[�g����
			if (m_bAutoRepeat) {
				if (m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd) {
					SendAllNoteOff (); // 20081027�ǉ�
					SendAllHold1Off (); // 20081219�ǉ�
					SendAllSostenutoOff (); // 20081219�ǉ�
					SendAllHold2Off (); // 20081219�ǉ�
				}
				// CC#111�̈ʒu���烋�[�v�J�n����ꍇ(RPG�c�N�[������) // 20081211�ǉ�
				if (m_theGeneralOption.m_bEnableCC111Loop) {
					// �Ō��CC#111�C�x���g�̃^�C����T��
					long lCC111Time = 0;
					long lCC111Millisec = 0;
					long lTempo = 0;
					forEachTrack (pMIDIData, pMIDITrack) {
						forEachEvent (pMIDITrack, pMIDIEvent) {
							if (MIDIEvent_IsControlChange (pMIDIEvent)) {
								if (MIDIEvent_GetNumber (pMIDIEvent) == 111) {
									long lTime = MIDIEvent_GetTime (pMIDIEvent);
									if (lTime > lCC111Time) {
										lCC111Time = lTime;
									}
								}
							}
						}
					}
					lCC111Millisec = MIDIData_TimeToMillisec (pMIDIData, lCC111Time);
					MIDIData_FindTempo (pMIDIData, lCC111Time, &lTempo);
					MIDIClock_Stop (pMIDIClock);
					MIDIClock_SetTickCount (pMIDIClock, lCC111Time);
					MIDIClock_SetMillisec (pMIDIClock, lCC111Millisec);
					if (lTempo != 0) {
						MIDIClock_SetTempo (pMIDIClock, lTempo);
					}
					pSekaijuDoc->m_lOldTime = lCC111Time;
					pSekaijuDoc->m_lNewTime = lCC111Time;
					MIDIClock_Start (pMIDIClock);
				}
				// �ŏ����烋�[�v�J�n����ꍇ
				else {
					MIDIClock_Reset (pSekaijuDoc->m_pMIDIClock);
					pSekaijuDoc->m_lOldTime = 0;
					pSekaijuDoc->m_lNewTime = 0;
				}
			}
			// ������~����
			else {
				if (m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd) {
					SendAllNoteOff (); // 20081027�ǉ�
					SendAllHold1Off (); // 20081218�ǉ�
					SendAllSostenutoOff (); // 20081218�ǉ�
					SendAllHold2Off (); // 20081218�ǉ�
				}
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
				pSekaijuDoc->m_lOldTime = lEndTime;
				pSekaijuDoc->m_lNewTime = lEndTime;
			}
		}
	}
	
	// �N���e�B�J���Z�N�V�������
	pSekaijuApp->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	return TRUE;
}

// �S�|�[�g�ɃI�[���z�[���h1�I�t(CC#64-0)�𑗐M
void CSekaijuApp::SendAllHold1Off () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 64, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// �S�|�[�g�ɃI�[���\�X�e�k�[�g�I�t(CC#66-0)�𑗐M
void CSekaijuApp::SendAllSostenutoOff () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 66, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}


// �S�|�[�g�ɃI�[���z�[���h2�I�t(CC#69-0)�𑗐M
void CSekaijuApp::SendAllHold2Off () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 69, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// �S�|�[�g�ɃI�[���T�E���h�I�t(CC#120)�𑗐M
void CSekaijuApp::SendAllSoundOff () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 120, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// �S�|�[�g�Ƀ��Z�b�g�I�[���R���g���[���[(CC#121)�𑗐M
void CSekaijuApp::SendResetAllController () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 121, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// �S�|�[�g�ɃI�[���m�[�g�I�t(CC#123)�𑗐M
void CSekaijuApp::SendAllNoteOff () {
	int i, j;
	BYTE byMsg[3] = {0xB0, 123, 0};
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		for (j = 0; j < 16; j++) {
			byMsg[0] = 0xB0 | j;
			if (m_pMIDIOut[i]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[i], byMsg, 3);
			}
			if (m_pMIDIStatus[i]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[i], byMsg, 3);
			}
		}
	}
}

// m_theMIDIStatusArray�̓��e�����Z�b�g
void CSekaijuApp::ResetMIDIStatusArray () {
	for (int nPort = 0; nPort < MAXMIDIOUTDEVICENUM; nPort++) {
		if (m_pMIDIStatus[nPort]) {
			MIDIStatus_PutReset (m_pMIDIStatus[nPort]);
		}
	}
}

// m_theTempMIDIStatusArray�̓��e�����Z�b�g���܂��B
void CSekaijuApp::ResetTempMIDIStatusArray () {
	for (int nPort = 0; nPort < MAXMIDIOUTDEVICENUM; nPort++) {
		if (m_pTempMIDIStatus[nPort]) {
			MIDIStatus_PutReset (m_pTempMIDIStatus[nPort]);
		}
	}
}


// �S�|�[�g��MIDIStatusArray��TempMIDIStatusArray�̍����݂̂𑗐M
// lFlags : ��������C�x���g�̎�ނ������t���O�̃r�b�g�a�B
//          Sekaiju.h�Ɏ�����SDS_�n�t���O�𑫂����킹��B
void CSekaijuApp::SendDifferentStatus (long lFlags) {
	int nPort, i, j;
	long lCurVal;
	long lTempVal;
	BYTE byCurBuf[128];
	BYTE byTempBuf[128];
	BYTE byMsg[256];
	BYTE byCC101[] = {   0,    0,    0,    0};
	BYTE byCC100[] = {   0,    1,    2,    5};
	BYTE byCC99[] = {   1,    1,    1,    1,    1,    1,    1,    1,   
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24,  24, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28,  28, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29,  29, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30,  30, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
		 
	};
	BYTE byCC98[] = {   8,    9,   10,   32,   33,   99,  100,  102,  
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
		  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
		 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
		 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
		 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	};
	// �e�|�[�g����
 	for (nPort = 0; nPort < MAXMIDIOUTDEVICENUM; nPort++) {
		if (m_pMIDIStatus[nPort] == NULL || m_pMIDIOut[nPort] == NULL || 
			m_pTempMIDIStatus[nPort] == NULL) {
			continue;
		}
		// �}�X�^�[�{�����[��
		if (lFlags & SDS_MASTERVOLUME) {
			long lCurMasterVolume = MIDIStatus_GetMasterVolume (m_pMIDIStatus[nPort]);
			long lTempMasterVolume = MIDIStatus_GetMasterVolume (m_pTempMIDIStatus[nPort]);
			if (lCurMasterVolume != lTempMasterVolume) {
				// �}�X�^�[�{�����[�� {0xF0, 0x7F, 0xid, 0x04, 0x01, 0xll, 0xmm, 0xF7}
				byMsg[0] = 0xF0;
				byMsg[1] = 0x7F;
				byMsg[2] = 0x7F;
				byMsg[3] = 0x04;
				byMsg[4] = 0x01; 
				byMsg[5] = 0x00;
				byMsg[6] = (BYTE)CLIP (0, lTempMasterVolume, 127);
				byMsg[7] = 0xF7;
				MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 8);
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 8);
			}
		}
		// �e�p�[�g����
		for (i = 0; i < 16; i++) {
			MIDIPart* pMIDIPart = MIDIStatus_GetMIDIPart (m_pMIDIStatus[nPort], i);
			MIDIPart* pTempPart = MIDIStatus_GetMIDIPart (m_pTempMIDIStatus[nPort], i);
			// �L�[�A�t�^�[�^�b�`
			if (lFlags & SDS_KEYAFTER) {
				MIDIPart_GetKeyAfterTouchEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetKeyAfterTouchEx (pTempPart, byTempBuf, 128);
				for (j = 0; j < 128; j++) {
					if (byCurBuf[j] != byTempBuf[j]) {
						byMsg[0] = 0xA0 + (BYTE)i;
						byMsg[1] = (BYTE)j;
						byMsg[2] = byTempBuf[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					}
				}
			}
			// �R���g���[���`�F���W
			if (lFlags & SDS_CONTROLCHANGE) {
				MIDIPart_GetControlChangeEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetControlChangeEx (pTempPart, byTempBuf, 128);
				for (j = 0; j < 120; j++) { // 20081218 128��120�ɕύX
					if (byCurBuf[j] != byTempBuf[j] && j != 0 && j != 32) { // 20111016�p�b�`�`�F���W�͏���
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = (BYTE)j;
						byMsg[2] = byTempBuf[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					}
				}
			}
			// RPN
			if (lFlags & SDS_RPN) {
				for (j = 0; j < sizeof (byCC101); j++) {
					lCurVal = 
						(MIDIPart_GetRPNMSB (pMIDIPart, byCC101[j], byCC100[j]) << 7) |
						(MIDIPart_GetRPNLSB (pMIDIPart, byCC101[j], byCC100[j]) & 0x7F);
					lTempVal = 
						(MIDIPart_GetRPNMSB (pTempPart, byCC101[j], byCC100[j]) << 7) |
						(MIDIPart_GetRPNLSB (pTempPart, byCC101[j], byCC100[j]) & 0x7F);
					if (lCurVal != lTempVal) {
						// CC#99=127(NRPN=NULL)(����̂�)
						if (byCurBuf[99] != 127) {
							byCurBuf[99] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 99;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#98=127(NRPN=NULL)(����̂�)
						if (byCurBuf[98] != 127) {
							byCurBuf[98] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 98;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#101
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 101;
						byMsg[2] = byCC101[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#100
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 100;
						byMsg[2] = byCC100[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#6
						if ((lCurVal >> 7) != (lTempVal >> 7)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 6;
							byMsg[2] = (BYTE)(lTempVal >> 7);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#38
						if ((lCurVal & 0x7F) != (lTempVal & 0x7F)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 38;
							byMsg[2] = (BYTE)(lTempVal & 0x7F);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
					}
				}
			}
			// NRPN
			if (lFlags & SDS_NRPN) {
				for (j = 0; j < sizeof (byCC99); j++) {
					lCurVal = 
						(MIDIPart_GetRPNMSB (pMIDIPart, byCC99[j], byCC98[j]) << 7) |
						(MIDIPart_GetRPNLSB (pMIDIPart, byCC99[j], byCC98[j]) & 0x7F);
					lTempVal = 
						(MIDIPart_GetRPNMSB (pTempPart, byCC99[j], byCC98[j]) << 7) |
						(MIDIPart_GetRPNLSB (pTempPart, byCC99[j], byCC98[j]) & 0x7F);
					if (lCurVal != lTempVal) {
						// CC#101=127(RPN=NULL)(����̂�)
						if (byCurBuf[101] != 127) {
							byCurBuf[101] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 101;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#100=127(NRPN=NULL)(����̂�)
						if (byCurBuf[100] != 127) {
							byCurBuf[100] = 127;
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 100;
							byMsg[2] = 127;
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#99
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 99;
						byMsg[2] = byCC99[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#98
						byMsg[0] = 0xB0 + (BYTE)i;
						byMsg[1] = 98;
						byMsg[2] = byCC98[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						// CC#6
						if ((lCurVal >> 7) != (lTempVal >> 7)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 6;
							byMsg[2] = (BYTE)(lTempVal >> 7);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
						// CC#38
						if ((lCurVal & 0x7F) != (lTempVal & 0x7F)) {
							byMsg[0] = 0xB0 + (BYTE)i;
							byMsg[1] = 38;
							byMsg[2] = (BYTE)(lTempVal & 0x7F);
							MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
							MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
						}
					}
				}
			}
			// �p�b�`�`�F���W(20111016�ǉ��C��)
			if (lFlags & (SDS_CONTROLCHANGE | SDS_PROGRAMCHANGE)) {
				MIDIPart_GetControlChangeEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetControlChangeEx (pTempPart, byTempBuf, 128);
				lCurVal = MIDIPart_GetProgramChange (pMIDIPart);
				lTempVal = MIDIPart_GetProgramChange (pTempPart);
				if (byCurBuf[0] != byTempBuf[0] ||
					byCurBuf[32] != byTempBuf[32] ||
					lCurVal != lTempVal) {
					byMsg[0] = 0xB0 + (BYTE)i;
					byMsg[1] = (BYTE)0;
					byMsg[2] = byTempBuf[0];
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					byMsg[0] = 0xB0 + (BYTE)i;
					byMsg[1] = (BYTE)32;
					byMsg[2] = byTempBuf[32];
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					byMsg[0] = 0xC0 + (BYTE)i;
					byMsg[1] = (BYTE)lTempVal;
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 2);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 2);
				}
			}
			// �`�����l���A�t�^�[�^�b�`
			if (lFlags & SDS_CHANNELAFTER) {
				lCurVal = MIDIPart_GetChannelAfterTouch (pMIDIPart);
				lTempVal = MIDIPart_GetChannelAfterTouch (pTempPart);
				if (lCurVal != lTempVal) {
					memset (byMsg, 0, sizeof (byMsg));
					byMsg[0] = 0xD0 + (BYTE)i;
					byMsg[1] = (BYTE)lTempVal;
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 2);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 2);
				}
			}
			// �s�b�`�x���h
			if (lFlags & SDS_PITCHBEND) {
				lCurVal = MIDIPart_GetPitchBend (pMIDIPart);
				lTempVal = MIDIPart_GetPitchBend (pTempPart);
				if (lCurVal != lTempVal) {
					memset (byMsg, 0, sizeof (byMsg));
					byMsg[0] = 0xE0 + (BYTE)i;
					byMsg[1] = (BYTE)(lTempVal & 0x007F);
					byMsg[2] = (BYTE)(lTempVal >> 7);
					MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
				}
			}
			// �m�[�g
			if (lFlags & SDS_NOTE) {
				MIDIPart_GetNoteKeepEx (pMIDIPart, byCurBuf, 128);
				MIDIPart_GetNoteKeepEx (pTempPart, byTempBuf, 128);
				for (j = 0; j < 128; j++) {
					if (byCurBuf[j] != byTempBuf[j]) {
						byMsg[0] = 0x90 + (BYTE)i;
						byMsg[1] = (BYTE)j;
						byMsg[2] = byTempBuf[j];
						MIDIOut_PutMIDIMessage (m_pMIDIOut[nPort], byMsg, 3);
						MIDIStatus_PutMIDIMessage (m_pMIDIStatus[nPort], byMsg, 3);
					}
				}
			}
			//TRACE0 ("restored.\n");
		}
	}
}

// MIDI���̓f�o�C�X���J��
void CSekaijuApp::OpenAllMIDIInDevice () {
	long i;
	CString strMsg;
	CString strMsg2;
	m_theCriticalSection.Lock ();
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_pMIDIIn[i] = MIDIIn_Reopen (m_pMIDIIn[i], m_strMIDIInName[i]);
		if (m_pMIDIIn[i]) {
			_RPTF2 (_CRT_WARN, "MIDIInDevice[%d] Open Successful. - \"%s\"\n", 
				i, m_strMIDIInName[i]);
		}
		else {
			if (m_strMIDIInName[i] != _T("") && m_strMIDIInName[i] != MIDIIO_NONE && 
				m_strMIDIInName[i] != MIDIIO_NONEJ) {
				_RPTF2 (_CRT_WARN, "MIDIInDevice[%d] Open Failed. - \"%s\"\n", 
					i, m_strMIDIInName[i]);
				// "%s\nMIDIIn�f�o�C�X[%d]���J���܂���B"
				strMsg.LoadString (IDS_S_N_MIDIINDEVICE_D_OPEN_FAILED);
				strMsg2.Format (strMsg, m_strMIDIInName[i], i + 1);
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
			}
			m_strMIDIInName[i] = _T("");
		}
	}
	m_theCriticalSection.Unlock ();
}

// MIDI�o�̓f�o�C�X���J��
void CSekaijuApp::OpenAllMIDIOutDevice () {
	long i;
	CString strMsg;
	CString strMsg2;
	m_theCriticalSection.Lock ();
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		MIDIStatus_Delete (m_pMIDIStatus[i]);
		m_pMIDIStatus[i] = NULL;
		MIDIStatus_Delete (m_pTempMIDIStatus[i]);
		m_pTempMIDIStatus[i] = NULL;
		m_pMIDIOut[i] = MIDIOut_Reopen (m_pMIDIOut[i], m_strMIDIOutName[i]);
		if (m_pMIDIOut[i]) {
			_RPTF2 (_CRT_WARN, "MIDIOutDevice[%d] Open Successful. - \"%s\"\n",
				i, m_strMIDIOutName[i]);
			m_pMIDIStatus[i] = MIDIStatus_Create (MIDISTATUS_MODENATIVE, 16, 2);
			m_pTempMIDIStatus[i] = MIDIStatus_Create (MIDISTATUS_MODENATIVE, 16, 2);
		}
		else {
			if (m_strMIDIOutName[i] != _T("") && m_strMIDIOutName[i] != MIDIIO_NONE &&
				m_strMIDIOutName[i] != MIDIIO_NONEJ) {
				_RPTF2 (_CRT_WARN, "MIDIOutDevice[%d] Open Failed. - \"%s\"\n",
					i, m_strMIDIOutName[i]);
				// "%s\nMIDIIn�f�o�C�X[%d]���J���܂���B"
				strMsg.LoadString (IDS_S_N_MIDIOUTDEVICE_D_OPEN_FAILED);
				strMsg2.Format (strMsg, m_strMIDIOutName[i], i + 1);
				m_theCriticalSection.Unlock ();
				AfxMessageBox (strMsg2, MB_OK | MB_ICONEXCLAMATION);
				m_theCriticalSection.Lock ();
			}
			m_strMIDIOutName[i] = _T("");
		}
	}
	m_theCriticalSection.Unlock ();
}

// ���t�ʒu�w��(��΃e�B�b�N�w��)
long CSekaijuApp::SetPlayPosition (CDocument* pDocument, long lTargetTime) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	//CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame ()->GetActiveDocument ());
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	if (pSekaijuDoc == NULL) {
		return 0;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return 0;
	}
	long lTempo = MIDIEVENT_DEFTEMPO;
	long lMillisec = 0;
	long lEndTime = MIDIData_GetEndTime (pSekaijuDoc->m_pMIDIData);
	lTargetTime = CLIP (0, lTargetTime, lEndTime);
	pSekaijuDoc->m_lOldTime = lTargetTime;
	lMillisec = MIDIData_TimeToMillisec (pSekaijuDoc->m_pMIDIData, lTargetTime);
	// �\���O�|�W�V�����Z���N�^(0xF2)�̑��o
	long lTimeMode, lTimeResolution;
	MIDIData_GetTimeBase (pSekaijuDoc->m_pMIDIData, &lTimeMode, &lTimeResolution);
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = 0xF2;
	byMsg[1] = (lTargetTime * 4 / lTimeResolution) & 0x7F;
	byMsg[2] = ((lTargetTime * 4 / lTimeResolution) >> 7) & 0x7F;
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] == 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 3);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 3);
			}
		}
	}
	/* lTargetTime���̃e���|��lTempo�Ɏ擾 */
	VERIFY (MIDIData_FindTempo (pSekaijuDoc->m_pMIDIData, lTargetTime, &lTempo));
	/* MIDI�X�e�[�^�X��lTargetTime���̏�Ԃɍ��킹�� */
	ResetTempMIDIStatusArray ();
	pSekaijuDoc->TimeMIDIStatus (lTargetTime, m_pTempMIDIStatus);
	long lFlags;
	/* ���t���Ȃ��(a)���ׂĖ���(b)�m�[�g�݂̂𕜌����� */
	if (m_bPlaying) {
		lFlags = m_theGeneralOption.m_bSearchUpdate ? SDS_ALL : SDS_NOTE;
		m_bIgnoreNoteEvent = 1;
	}
	/* ��~���Ȃ��(a)�m�[�g���������ׂĂ𕜌����邩(b)�����������Ȃ� */
	else {
		lFlags = m_theGeneralOption.m_bSearchUpdate ? (SDS_ALL & ~SDS_NOTE) : 0;
		m_bIgnoreNoteEvent = 0;
	}
	SendDifferentStatus (lFlags);
	/* MIDI�N���b�N�̍X�V */
	if (m_bPlaying) {
		MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
	}
	MIDIClock_Reset (pSekaijuDoc->m_pMIDIClock);
	MIDIClock_SetTickCount (pSekaijuDoc->m_pMIDIClock, lTargetTime);
	MIDIClock_SetMillisec (pSekaijuDoc->m_pMIDIClock, lMillisec);
	MIDIClock_SetTempo (pSekaijuDoc->m_pMIDIClock, lTempo);
	if (m_bPlaying) {
		MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
	}
	return 1;
}

// ���t�̊J�n
long CSekaijuApp::StartPlaying (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = (lCurTime == 0 ? 0xFA : 0xFB);
	// �X�^�[�g(0xFA)���̓R���e�B�j���[(0xFB)���b�Z�[�W�̑��o
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
			}
		}
	}
	// ���݂̏�Ԃ𕜌�����
	ResetTempMIDIStatusArray ();
	pSekaijuDoc->TimeMIDIStatus (lCurTime, m_pTempMIDIStatus); // 20090625�C��
	SendDifferentStatus (m_theGeneralOption.m_bPlayUpdate ? SDS_ALL : SDS_NOTE);
	m_bIgnoreNoteEvent = TRUE; // 20090625�C��
	// ���t��Ԃɂ���
	pSekaijuDoc->m_lOldTime = lCurTime;
	if (!m_bRecording) {
		MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
	}
	m_bPlaying = TRUE;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_PLAYSTARTED);
	return 1;
}

// ���t�̒�~(�^�����̏ꍇ��StopRecording���g������)
long CSekaijuApp::StopPlaying (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = 0xFC;
	// �X�g�b�v���b�Z�[�W(0xFC)�̑��o
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
			}
		}
	}

	// ���t�E�^���̒�~
	m_bRecording = FALSE;
	m_bPlaying = FALSE;
	MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_PLAYSTOPED);
	return 1;
}


// �^���̊J�n
long CSekaijuApp::StartRecording (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	// ���t���łȂ��ꍇ
	if (!m_bPlaying) {
		long lOutputPort;
		BYTE byMsg[256];
		memset (byMsg, 0, sizeof (byMsg));
		byMsg[0] = (lCurTime == 0 ? 0xFA : 0xFB);
		// �X�^�[�g(0xFA)���̓R���e�B�j���[(0xFB)���b�Z�[�W�̑��o
		for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
			if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
				if (m_pMIDIOut[lOutputPort]) {
					MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
				}
				if (m_pMIDIStatus[lOutputPort]) {
					MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
				}
			}
		}
		// ���݂̏�Ԃ𕜌�����
		ResetTempMIDIStatusArray ();
		pSekaijuDoc->TimeMIDIStatus (lCurTime, m_pTempMIDIStatus); // 20090626�C��
		SendDifferentStatus (m_theGeneralOption.m_bPlayUpdate ? SDS_ALL : SDS_NOTE);
		m_bIgnoreNoteEvent = TRUE; // 20090626�C��
		pSekaijuDoc->m_lOldTime = lCurTime;
		MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		m_bPlaying = TRUE;
	}
	// �����L�^
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pLastEvent = NULL;
	MIDIEvent* pCloneEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	pSekaijuDoc->AddHistoryUnit (_T("���A���^�C������"));
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	// EOT�̗����L�^
	forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pLastEvent));
				VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pLastEvent));
			}
		}
	}
	// �L�^�J�n
	pSekaijuDoc->SetModifiedFlag (TRUE);
	m_theTempRecordedEventArray.RemoveAll ();
	m_bFirstMetronome = TRUE;
	m_bRecording = TRUE;
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_RECORDSTARTED | SEKAIJUDOC_MIDIEVENTCHANGED);
	return 1;
}

// �^���̒�~
long CSekaijuApp::StopRecording (CDocument* pDocument) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)pDocument;
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lOutputPort;
	BYTE byMsg[256];
	memset (byMsg, 0, sizeof (byMsg));
	byMsg[0] = 0xFC;
	// �X�g�b�v���b�Z�[�W�̑��o
	for (lOutputPort = 0; lOutputPort < MAXMIDIOUTDEVICENUM; lOutputPort++) {
		if (m_lMIDIOutSyncMode[lOutputPort] >= 1) {
			if (m_pMIDIOut[lOutputPort]) {
				MIDIOut_PutMIDIMessage (m_pMIDIOut[lOutputPort], byMsg, 1);
			}
			if (m_pMIDIStatus[lOutputPort]) {
				MIDIStatus_PutMIDIMessage (m_pMIDIStatus[lOutputPort], byMsg, 1);
			}
		}
	}
	// ���t�E�L�^��~
	m_bRecording = FALSE;
	m_bPlaying = FALSE;
	MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllHold1Off ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	// �����L�^
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempEvent = NULL;
	MIDIEvent* pLastEvent = NULL;
	CHistoryUnit* pCurHistoryUnit = NULL;
	VERIFY (pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ());
	long lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
	long j;
	// �^�����ꂽ���ׂẴm�[�g�C�x���g�̌����m�F
	for (j = 0; j < lTempRecordedEventCount; j++) {
		pTempEvent = (MIDIEvent*)m_theTempRecordedEventArray.GetAt (j);
		// �m�[�g�I�����Ȃ��m�[�g�I�t�C�x���g�̏ꍇ�A�m�[�g�I�t�C�x���g�폜
		if (MIDIEvent_IsNoteOff (pTempEvent) && pTempEvent->m_pPrevCombinedEvent == NULL) {
			MIDIEvent_Delete (pTempEvent);
			pTempEvent = NULL;
			m_theTempRecordedEventArray.RemoveAt (j);
			j--;
			lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
		}
		// �m�[�g�I�t���Ȃ��m�[�g�I���C�x���g�̏ꍇ�A�m�[�g�I���C�x���g�ǉ�
		else if (MIDIEvent_IsNoteOn (pTempEvent) && pTempEvent->m_pNextCombinedEvent == NULL) {
			long lTime = MIDIEvent_GetTime (pTempEvent);
			long lChannel = MIDIEvent_GetChannel (pTempEvent);
			long lKey = MIDIEvent_GetKey (pTempEvent);
			MIDITrack* pTempTrack = MIDIEvent_GetParent (pTempEvent);
			ASSERT (pTempTrack);
			MIDIEvent* pNoteOffEvent = NULL;
			VERIFY (pNoteOffEvent = MIDIEvent_CreateNoteOff (lCurTime, lChannel, lKey, 0));
			VERIFY (MIDITrack_InsertEvent (pTempTrack, pNoteOffEvent));
			//VERIFY (MIDIEvent_Combine (pTempEvent));
			pTempEvent->m_pNextCombinedEvent = pNoteOffEvent;
			pNoteOffEvent->m_pPrevCombinedEvent = pTempEvent;
			m_theTempRecordedEventArray.Add (pNoteOffEvent);
			lTempRecordedEventCount = m_theTempRecordedEventArray.GetSize ();
		}
	}

	// �^�����ꂽ���ׂẴC�x���g�𗚗��ɋL�^(�����m�[�g�̓m�[�g�I���̂�)
	for (j = 0; j < lTempRecordedEventCount; j++) {
		pTempEvent = (MIDIEvent*)m_theTempRecordedEventArray.GetAt (j);
		if (pTempEvent->m_pPrevCombinedEvent == NULL) {
			pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pTempEvent);
		}
	}
	// EOT�̗����L�^
	forEachTrack (pSekaijuDoc->m_pMIDIData, pMIDITrack) {
		pLastEvent = MIDITrack_GetLastEvent (pMIDITrack);
		if (pLastEvent) {
			if (MIDIEvent_IsEndofTrack (pLastEvent)) {
				pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pLastEvent);
			}
		}
	}
	m_theTempRecordedEventArray.RemoveAll ();
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_RECORDSTOPED | SEKAIJUDOC_MIDIEVENTCHANGED);
	return 1;
}


//------------------------------------------------------------------------------
// ���b�Z�[�W�}�b�v
//------------------------------------------------------------------------------

// �w�t�@�C��(&F)�x-�w�V�K�쐬(&N)�x
void CSekaijuApp::OnUpdateFileNewUI (CCmdUI* pCmdUI) {
	//if (m_bRecording) {
	//	pCmdUI->Enable (FALSE);
	//}
}

// �w�t�@�C��(&F)�x-�w�J��(&O)�x
void CSekaijuApp::OnUpdateFileOpenUI (CCmdUI* pCmdUI) {
	//if (m_bRecording) {
	//	pCmdUI->Enable (FALSE);
	//}
}

// �w�R���g���[��(&C)�x-�w�ŏ��ɖ߂�(&W)�x
void CSekaijuApp::OnControlToBegin () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());

	// �h�L�������g���Ȃ��ꍇ
	if (pSekaijuDoc == NULL) {
		m_bRecording = FALSE;
		m_bPlaying = FALSE;
		return;
	}
	
	// �h�L�������g������ꍇ
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// �L�^���̏ꍇ
	if (m_bRecording) {
		StopRecording (pSekaijuDoc);
	}
	// �L�^���łȂ��ꍇ
	else {
		StopPlaying (pSekaijuDoc);
	}
	//MIDIClock_Reset (pSekaijuDoc->m_pMIDIClock); // 20080825�ړ� // 20090627�p�~
	SetPlayPosition (pSekaijuDoc, 0); // 20090627�ǉ�
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED); // 20080922�ǉ�
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w�ŏ��ɖ߂�(&W)�x��UI�X�V
void CSekaijuApp::OnUpdateControlToBeginUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// �w�R���g���[��(&C)�x-�w�Ō�ɐi�ށx
void CSekaijuApp::OnControlToEnd () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	
	// �h�L�������g���Ȃ��ꍇ
	if (pSekaijuDoc == NULL) {
		m_bRecording = FALSE;
		m_bPlaying = FALSE;
		return;
	}

	// �h�L�������g������ꍇ
	pSekaijuDoc->m_theCriticalSection.Lock ();
	// �L�^���̏ꍇ
	if (m_bRecording) {
		StopRecording (pSekaijuDoc);
	}
	// �L�^���łȂ��ꍇ
	else {
		StopPlaying (pSekaijuDoc);
	}
	long lEndTime = MIDIData_GetEndTime (pSekaijuDoc->m_pMIDIData);
	SetPlayPosition (pSekaijuDoc, lEndTime);
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED); // 20080922�ǉ�
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w�Ō�ɐi�ށx��UI�X�V
void CSekaijuApp::OnUpdateControlToEndUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

	
// �w�R���g���[��(&C)�x-�w���t(&P)�x
void CSekaijuApp::OnControlPlay () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	// ��~���̏ꍇ
	if (!m_bPlaying) {
		StartPlaying (pSekaijuDoc);
	}
	// ���t���̏ꍇ
	else {
		// ���t�����^�����̏ꍇ
		if (m_bRecording) {
			StopRecording (pSekaijuDoc);
		}
		// ���t���ł��邪�^�����łȂ��ꍇ
		else {
			StopPlaying (pSekaijuDoc);
		}
	}
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w���t(&P)�x��UI�X�V
void CSekaijuApp::OnUpdateControlPlayUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->SetCheck (m_bPlaying);
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->SetCheck (0);
		pCmdUI->Enable (0);
	}
}

// �w�R���g���[��(&C)�x-�w�^��(&R)�x
void CSekaijuApp::OnControlRecord () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (this->m_bInplaceEditing || this->m_bInplaceListing || this->m_bValueUpDowning) {
		return;
	}
	if (pSekaijuDoc->m_bEditLocked) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	// ��~���̏ꍇ
	if (!m_bRecording) {
		StartRecording (pSekaijuDoc);
	}
	// �L�^���̏ꍇ
	else {
		StopRecording (pSekaijuDoc);
	}
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w�^��(&R)�xUI�X�V
void CSekaijuApp::OnUpdateControlRecordUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->SetCheck (m_bRecording);
		if (this->m_bInplaceEditing || this->m_bInplaceListing || this->m_bValueUpDowning) {
			pCmdUI->Enable (0);
		}
		else if (pSekaijuDoc->m_bEditLocked) {
			pCmdUI->Enable (0);
		}
		else {
			pCmdUI->Enable (1);
		}
	}
	else {
		pCmdUI->SetCheck (0);
		pCmdUI->Enable (0);
	}
}



// �w�R���g���[��(&C)�x-�w�O�̏��߁x
void CSekaijuApp::OnControlPrevMeasure () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		if (lTick * 2 / lTimeResolution < 1) { 
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (0, lMeasure - 1), 0, 0, &lCurTime);
		}
		else {
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (0, lMeasure), 0, 0, &lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		if (lCurTime % lTimeResolution < lTimeResolution / 2) {
			lCurTime = (lCurTime - lTimeResolution * 10) / lTimeResolution * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		else {
			lCurTime = (lCurTime - lTimeResolution * 10) / lTimeResolution * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w�O�̏��߁x��UI�X�V
void CSekaijuApp::OnUpdateControlPrevMeasureUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// �w�R���g���[��(&C)�x-�w���̏��߁x
void CSekaijuApp::OnControlNextMeasure () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __min (65535, lMeasure + 1), 0, 0, &lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		lCurTime = (lCurTime + (lTimeResolution * 10)) / lTimeResolution * lTimeResolution;
		lCurTime = __min (0x7FFFFFFF, lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w���̏��߁x��UI�X�V
void CSekaijuApp::OnUpdateControlNextMeasureUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// �w�R���g���[��(&C)�x-�w�O�̔��x
void CSekaijuApp::OnControlPrevBeat () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		if (lTick * 2 / lTimeResolution < 1) { 
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (lMeasure, 0), lBeat - 1, 0, &lCurTime);
		}
		else {
			MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __max (lMeasure, 0), lBeat - 1, 0, &lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		if (lCurTime % lTimeResolution < lTimeResolution / 2) {
			lCurTime = ((lCurTime - lTimeResolution) / lTimeResolution) * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		else {
			lCurTime = lCurTime / lTimeResolution * lTimeResolution;
			lCurTime = __max (0, lCurTime);
		}
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w�O�̔��x��UI�X�V
void CSekaijuApp::OnUpdateControlPrevBeatUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}

// �w�R���g���[��(&C)�x-�w���̔��x
void CSekaijuApp::OnControlNextBeat () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pSekaijuDoc->m_pMIDIData == NULL || pSekaijuDoc->m_pMIDIClock == NULL) {
		return;
	}
	if (m_bRecording) {
		return;
	}
	pSekaijuDoc->m_theCriticalSection.Lock ();
	this->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	SendAllNoteOff ();
	SendAllHold1Off ();
	SendAllSostenutoOff ();
	SendAllHold2Off ();
	SendAllSoundOff ();
	long lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
	long lTimeMode = 0;
	long lTimeResolution = 120;
	MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
	if (lTimeMode == MIDIDATA_TPQNBASE) {	
		long lMeasure, lBeat, lTick;
		MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick);
		MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData, __min (65535, lMeasure), lBeat + 1, 0, &lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	else {
		lCurTime = ((lCurTime + lTimeResolution) / lTimeResolution) * lTimeResolution;
		lCurTime = __min (0x7FFFFFFF, lCurTime);
		SetPlayPosition (pSekaijuDoc, lCurTime);
	}
	pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
	this->m_theCriticalSection.Unlock ();
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}

// �w�R���g���[��(&C)�x-�w���̔��x��UI�X�V
void CSekaijuApp::OnUpdateControlNextBeatUI (CCmdUI* pCmdUI) {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame()->GetActiveDocument ());
	if (pSekaijuDoc) {
		pCmdUI->Enable (1);
	}
	else {
		pCmdUI->Enable (0);
	}
}


// �w�R���g���[��(&C)�x-�w�X�s�[�h=�Î~�x
void CSekaijuApp::OnControlSpeedNone () {
	if (m_bRecording) {
		return;
	}
	if (m_lCurSpeedIndex != 0) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 0;

	// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�Î~�x
void CSekaijuApp::OnUpdateControlSpeedNoneUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 0 ? 1 : 0);
	pCmdUI->Enable (m_bRecording ? 0 : 1);
	//CToolBar* pToolBar = (CToolBar*)pCmdUI;
	//pToolBar->Invalidate (TRUE);
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�ᑬ�x
void CSekaijuApp::OnControlSpeedSlow () {
	if (m_lCurSpeedIndex != 1) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 1;

	// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�ᑬ�x
void CSekaijuApp::OnUpdateControlSpeedSlowUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 1 ? 1 : 0);
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�W���x
void CSekaijuApp::OnControlSpeedNormal () {
	if (m_lCurSpeedIndex != 2) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 2;

	// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�W���x
void CSekaijuApp::OnUpdateControlSpeedNormalUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 2 ? 1 : 0);
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�����x
void CSekaijuApp::OnControlSpeedFast () {
	if (m_lCurSpeedIndex != 3) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 3;

	// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}

}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=�����x
void CSekaijuApp::OnUpdateControlSpeedFastUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 3 ? 1 : 0);
}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=���@��ɃX���[�u�x
void CSekaijuApp::OnControlSpeedSlave () {
	long lSyncInputPort = 0;
	long lSyncInputMode = 0;
	GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
	// �X���[�u������̂�������Ȃ��ꍇ
	if (lSyncInputMode == 0) {
		return;
	}
	
	if (m_lCurSpeedIndex != 4) {
		m_lOldSpeedIndex = m_lCurSpeedIndex;
	}
	m_lCurSpeedIndex = 4;

	// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		//pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}

}

// �w�R���g���[��(&C)�x-�w�X�s�[�h=���@��ɃX���[�u�x
void CSekaijuApp::OnUpdateControlSpeedSlaveUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_lCurSpeedIndex == 4 ? 1 : 0);
}


// �w�R���g���[��(&C)�x-�w�I�[�g���s�[�g(&A)�x
void CSekaijuApp::OnControlAutoRepeat () {
	m_bAutoRepeat = !m_bAutoRepeat;
}

// �w�R���g���[��(&C)�x-�w�I�[�g���s�[�g(&A)�x
void CSekaijuApp::OnUpdateControlAutoRepeatUI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck (m_bAutoRepeat);
}


// �w�ݒ�(&S)�x-�wMIDI�f�o�C�X�ƃC���X�g�D�������g(&D)...�x
void CSekaijuApp::OnSetupMIDIDevice () {
	long i;
	CString strNone;
	VERIFY (strNone.LoadString (IDS_NONE));
	CMIDIDeviceSheet theSheet (AfxGetMainWnd ());
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		if (m_strMIDIInName[i] == _T("")) {
			theSheet.m_theMIDIInDevicePage.m_strMIDIInName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIInDevicePage.m_strMIDIInName[i] = m_strMIDIInName[i];
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_strMIDIOutName[i] == _T("")) {
			theSheet.m_theMIDIOutDevicePage.m_strMIDIOutName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIOutDevicePage.m_strMIDIOutName[i] = m_strMIDIOutName[i];
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_strMIDIInstDefNormName[i] == _T("")) {
			theSheet.m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i] = m_strMIDIInstDefNormName[i];
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (m_strMIDIInstDefDrumName[i] == _T("")) {
			theSheet.m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i] = _T("");
		}
		else {
			theSheet.m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i] = m_strMIDIInstDefDrumName[i];
		}
	}
	if (theSheet.DoModal () == IDOK) {
		for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
			m_strMIDIInName[i] = theSheet.m_theMIDIInDevicePage.m_strMIDIInName[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_strMIDIOutName[i] = theSheet.m_theMIDIOutDevicePage.m_strMIDIOutName[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_strMIDIInstDefNormName[i] = theSheet.m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_strMIDIInstDefDrumName[i] = theSheet.m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i];
		}
		OpenAllMIDIInDevice ();
		OpenAllMIDIOutDevice ();
		SelectAllMIDIInstDefNorm ();
		SelectAllMIDIInstDefDrum ();
		((CSekaijuDocTemplate*)m_pSekaijuDocTemplate)->UpdateAllViews 
			(NULL, SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
	}
}

// �w�ݒ�(&S)�x-�wMIDI�������[�h(&S)...�x
void CSekaijuApp::OnSetupMIDISyncMode () {
	long i;
	CMIDISyncModeSheet theSheet (AfxGetMainWnd ());
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		if (0 <= m_lMIDIInSyncMode[i] && m_lMIDIInSyncMode[i] <= 2) {
			theSheet.m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i] = m_lMIDIInSyncMode[i];
		}
		else {
			theSheet.m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i] = 0;
		}
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		if (0 <= m_lMIDIOutSyncMode[i] && m_lMIDIOutSyncMode[i] <= 5) {
			theSheet.m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i] = m_lMIDIOutSyncMode[i];
		}
		else {
			theSheet.m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i] = 0;
		}
	}
	if (theSheet.DoModal () == IDOK) {
		for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
			m_lMIDIInSyncMode[i] = theSheet.m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i];
		}
		for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
			m_lMIDIOutSyncMode[i] = theSheet.m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i];
		}

		// �X���[�u������̂�������Ȃ��ꍇ�}�X�^�[�W�����x�Ƃ���B
		long lSyncInputPort = 0;
		long lSyncInputMode = 0;
		GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
		if (lSyncInputMode == 0 && m_lCurSpeedIndex == 4) {
			m_lCurSpeedIndex = 2;
		}

		// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
		POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
		while (docpos) {
			CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
			long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			if (lRunning) {
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->ApplyAppCurSpeedIndex ();
			if (lRunning) {
				MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);		
		}
	}
}


// MIDI�f�o�C�X�ƃC���X�g�D�������g�Łw�K�p�x���������Ƃ�
void CSekaijuApp::ApplyMIDIDeviceSheet (CPropertySheet* pSheet) {
	long i;
	CMIDIDeviceSheet* pMIDIDeviceSheet = (CMIDIDeviceSheet*)pSheet;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_strMIDIInName[i] = pMIDIDeviceSheet->m_theMIDIInDevicePage.m_strMIDIInName[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_strMIDIOutName[i] = pMIDIDeviceSheet->m_theMIDIOutDevicePage.m_strMIDIOutName[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_strMIDIInstDefNormName[i] = pMIDIDeviceSheet->m_theMIDIInstDefNormPage.m_strMIDIInstDefNormName[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_strMIDIInstDefDrumName[i] = pMIDIDeviceSheet->m_theMIDIInstDefDrumPage.m_strMIDIInstDefDrumName[i];
	}
	OpenAllMIDIInDevice ();
	OpenAllMIDIOutDevice ();
	SelectAllMIDIInstDefNorm ();
	SelectAllMIDIInstDefDrum ();
	((CSekaijuDocTemplate*)m_pSekaijuDocTemplate)->UpdateAllViews 
		(NULL, SEKAIJUDOC_MIDIDATACHANGED | SEKAIJUDOC_MIDITRACKCHANGED | SEKAIJUDOC_MIDIDATACHANGED);
}

// MIDI�������[�h�v���p�e�B�V�[�g�Łw�K�p�x���������Ƃ�
void CSekaijuApp::ApplyMIDISyncModeSheet (CPropertySheet* pSheet) {
	long i;
	CMIDISyncModeSheet* pMIDISyncModeSheet = (CMIDISyncModeSheet*)pSheet;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		m_lMIDIInSyncMode[i] = pMIDISyncModeSheet->m_theMIDIInSyncModePage.m_nMIDIInSyncMode[i];
	}
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		m_lMIDIOutSyncMode[i] = pMIDISyncModeSheet->m_theMIDIOutSyncModePage.m_nMIDIOutSyncMode[i];
	}

	// �X���[�u������̂�������Ȃ��ꍇ�}�X�^�[�W�����x�Ƃ���B
	long lSyncInputPort = 0;
	long lSyncInputMode = 0;
	GetCurSyncInputPortAndMode (&lSyncInputPort, &lSyncInputMode);
	if (lSyncInputMode == 0 && m_lCurSpeedIndex == 4) {
		m_lCurSpeedIndex = 2;
	}

	// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
	POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
	while (docpos) {
		CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
		long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
		pSekaijuDoc->m_theCriticalSection.Lock ();
		if (lRunning) {
			MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->ApplyAppCurSpeedIndex ();
		if (lRunning) {
			MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
		}
		pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		::Sleep (0);		
	}
}


// �w�ݒ�(&S)�x-�w�C���X�g�D�������g(&I)...�x
void CSekaijuApp::OnSetupInstrument () {

}

// �w�ݒ�(&S)�x-�w���g���m�[��(&M)...�x
void CSekaijuApp::OnSetupMetronome () {
	CMetronomeDlg theDlg;
	theDlg.m_nOn = m_theMetronomeDlgStatus.m_nOn;
	theDlg.m_nOutputPort = CLIP (0, m_theMetronomeDlgStatus.m_nOutputPort, 15);
	theDlg.m_nOutputChannel = CLIP (0, m_theMetronomeDlgStatus.m_nOutputChannel, 15) + 1;
	theDlg.m_nNoteKey1 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey1, 127);
	theDlg.m_nNoteVel1 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteVel1, 127);
	theDlg.m_nNoteKey2 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteKey2, 127);
	theDlg.m_nNoteVel2 = CLIP (0, m_theMetronomeDlgStatus.m_nNoteVel2, 127);
	if (theDlg.DoModal () == IDOK) {
		m_theMetronomeDlgStatus.m_nOn =theDlg.m_nOn; 
		m_theMetronomeDlgStatus.m_nOutputPort = theDlg.m_nOutputPort;
		m_theMetronomeDlgStatus.m_nOutputChannel = theDlg.m_nOutputChannel - 1;
		m_theMetronomeDlgStatus.m_nNoteKey1 = theDlg.m_nNoteKey1;
		m_theMetronomeDlgStatus.m_nNoteVel1 = theDlg.m_nNoteVel1;
		m_theMetronomeDlgStatus.m_nNoteKey2 = theDlg.m_nNoteKey2;
		m_theMetronomeDlgStatus.m_nNoteVel2 = theDlg.m_nNoteVel2;
	}
}

// �w�ݒ�(&S)�x-�w�����ۑ�(&A)...�x
void CSekaijuApp::OnSetupAutoSave () {
	CAutoSaveDlg theDlg;
	theDlg.m_strExeFilePath = m_strExeFilePath;
	theDlg.m_nOn = m_theAutoSaveDlgStatus.m_nOn;
	theDlg.m_lInterval = CLIP (1, m_theAutoSaveDlgStatus.m_lInterval / 60, 120);
	theDlg.m_nDisableWhilePlaying = m_theAutoSaveDlgStatus.m_nDisableWhilePlaying;
	theDlg.m_nDisableWhileRecording = m_theAutoSaveDlgStatus.m_nDisableWhileRecording;
	if (theDlg.DoModal () == IDOK) {
		m_theAutoSaveDlgStatus.m_nOn = theDlg.m_nOn;
		m_theAutoSaveDlgStatus.m_lInterval =CLIP (1,  theDlg.m_lInterval, 120) * 60;
		m_theAutoSaveDlgStatus.m_nDisableWhilePlaying = theDlg.m_nDisableWhilePlaying;
		m_theAutoSaveDlgStatus.m_nDisableWhileRecording = theDlg.m_nDisableWhileRecording;
	}
}

// �w�ݒ�(&S)�x-�w����(&L)...�x
void CSekaijuApp::OnSetupLanguage () {
	CLanguageDlg theDlg;
	theDlg.m_strLanguage = m_strLanguage;
	if (theDlg.DoModal () == IDOK) {
		m_strLanguage = theDlg.m_strLanguage;
		CString strMsg;
		VERIFY (strMsg.LoadString (IDS_LANGUAGE_WILL_BE_CHANGED_AT_THE_NEXT_STARTUP));
		AfxMessageBox (strMsg, MB_ICONINFORMATION);
	}
}


// �w�ݒ�(&S)�x-�w�I�v�V����(&O)...�x
void CSekaijuApp::OnSetupOptions () {
	COptionSheet theSheet (AfxGetMainWnd ());
	theSheet.m_theGeneralOptionPage.m_bEnableMultiExec = m_theGeneralOption.m_bEnableMultiExec;
	theSheet.m_theGeneralOptionPage.m_bEnableMultiOpen = m_theGeneralOption.m_bEnableMultiOpen;
	theSheet.m_theGeneralOptionPage.m_bRestoreWindowPlacement = m_theGeneralOption.m_bRestoreWindowPlacement;
	theSheet.m_theGeneralOptionPage.m_bExecOpen = m_theGeneralOption.m_bExecOpen;
	theSheet.m_theGeneralOptionPage.m_bOpenPlay = m_theGeneralOption.m_bOpenPlay;
	theSheet.m_theGeneralOptionPage.m_bPlayUpdate = m_theGeneralOption.m_bPlayUpdate;
	theSheet.m_theGeneralOptionPage.m_bSearchUpdate = m_theGeneralOption.m_bSearchUpdate;
	theSheet.m_theGeneralOptionPage.m_bEnableCC111Loop = m_theGeneralOption.m_bEnableCC111Loop;
	theSheet.m_theGeneralOptionPage.m_bPatchSearch = m_theGeneralOption.m_bPatchSearch;
	theSheet.m_theGeneralOptionPage.m_bInvertCtrlMouseWheel = m_theGeneralOption.m_bInvertCtrlMouseWheel;
	theSheet.m_theGeneralOptionPage.m_bTrackZeroOrigin = m_theGeneralOption.m_bTrackZeroOrigin;
	theSheet.m_theGeneralOptionPage.m_bEventZeroOrigin = m_theGeneralOption.m_bEventZeroOrigin;
	theSheet.m_theGeneralOptionPage.m_bEnableAutoPageUpdate = m_theGeneralOption.m_bEnableAutoPageUpdate;
	theSheet.m_theGeneralOptionPage.m_bSendNoteOffHoldOffAtEnd = m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd;
	theSheet.m_theGeneralOptionPage.m_lUpDownDelta1 = CLIP (1, m_theGeneralOption.m_lUpDownDelta1, 16);
	theSheet.m_theGeneralOptionPage.m_lUpDownDelta2 = CLIP (1, m_theGeneralOption.m_lUpDownDelta2, 16);
	theSheet.m_theGeneralOptionPage.m_lKeyVelocity1 = CLIP (1, m_theGeneralOption.m_lKeyVelocity1, 127);
	theSheet.m_theGeneralOptionPage.m_lKeyVelocity2 = CLIP (1, m_theGeneralOption.m_lKeyVelocity2, 127);
	theSheet.m_theGeneralOptionPage.m_lSpeedSlow = CLIP (1, m_theGeneralOption.m_lSpeedSlow / 100, 1000);
	theSheet.m_theGeneralOptionPage.m_lSpeedNormal = CLIP (1, m_theGeneralOption.m_lSpeedNormal / 100, 1000);
	theSheet.m_theGeneralOptionPage.m_lSpeedFast = CLIP (1, m_theGeneralOption.m_lSpeedFast / 100, 1000);
	theSheet.m_theGeneralOptionPage.m_lPlayRecordInterval = CLIP (1, m_theGeneralOption.m_lPlayRecordInterval, 1000);
	theSheet.m_theGeneralOptionPage.m_lOctaveSignature = CLIP (3, m_theGeneralOption.m_lOctaveSignature, 5);
	
	theSheet.m_theTrackListOption1Page.m_lDefRowZoom = CLIP (16, m_theTrackListOption1.m_lDefRowZoom, 64);
	theSheet.m_theTrackListOption1Page.m_lDefColumnZoom = CLIP (2, m_theTrackListOption1.m_lDefColumnZoom, 16);
	theSheet.m_theTrackListOption1Page.m_lDefTimeZoom = CLIP (2, m_theTrackListOption1.m_lDefTimeZoom, 16);
	theSheet.m_theTrackListOption1Page.m_lDefNameWidth = CLIP (1, m_theTrackListOption1.m_lDefNameWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefColorWidth = CLIP (1, m_theTrackListOption1.m_lDefColorWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefInputOnWidth = CLIP (1, m_theTrackListOption1.m_lDefInputOnWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefInputPortWidth = CLIP (1, m_theTrackListOption1.m_lDefInputPortWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefInputChWidth = CLIP (1, m_theTrackListOption1.m_lDefInputChWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefOutputOnWidth = CLIP (1, m_theTrackListOption1.m_lDefOutputOnWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefOutputPortWidth = CLIP (1, m_theTrackListOption1.m_lDefOutputPortWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefOutputChWidth = CLIP (1, m_theTrackListOption1.m_lDefOutputChWidth, 32);
	theSheet.m_theTrackListOption1Page.m_lDefViewModeWidth = CLIP (1, m_theTrackListOption1.m_lDefViewModeWidth, 32);

	theSheet.m_theTrackListOption2Page.m_lDefCC000Width = CLIP (1, m_theTrackListOption2.m_lDefCC000Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC032Width = CLIP (1, m_theTrackListOption2.m_lDefCC032Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefPCWidth = CLIP (1, m_theTrackListOption2.m_lDefPCWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC007Width = CLIP (1, m_theTrackListOption2.m_lDefCC007Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC010Width = CLIP (1, m_theTrackListOption2.m_lDefCC010Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC091Width = CLIP (1, m_theTrackListOption2.m_lDefCC091Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC093Width = CLIP (1, m_theTrackListOption2.m_lDefCC093Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefCC094Width = CLIP (1, m_theTrackListOption2.m_lDefCC094Width, 32);
	theSheet.m_theTrackListOption2Page.m_lDefKeyShiftWidth = CLIP (1, m_theTrackListOption2.m_lDefKeyShiftWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefVelShiftWidth = CLIP (1, m_theTrackListOption2.m_lDefVelShiftWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefTimeShiftWidth = CLIP (1, m_theTrackListOption2.m_lDefTimeShiftWidth, 32);
	theSheet.m_theTrackListOption2Page.m_lDefNumEventWidth = CLIP (1, m_theTrackListOption2.m_lDefNumEventWidth, 32);

	theSheet.m_thePianoRollOptionPage.m_lDefKeyZoom = CLIP (4, m_thePianoRollOption.m_lDefKeyZoom, 16);
	theSheet.m_thePianoRollOptionPage.m_lDefVelZoom = CLIP (1, m_thePianoRollOption.m_lDefVelZoom, 4);
	theSheet.m_thePianoRollOptionPage.m_lDefTimeZoom = CLIP (1, m_thePianoRollOption.m_lDefTimeZoom, 16);

	theSheet.m_theEventListOptionPage.m_lDefRowZoom = CLIP (16, m_theEventListOption.m_lDefRowZoom, 32);
	theSheet.m_theEventListOptionPage.m_lDefColumnZoom = CLIP (2, m_theEventListOption.m_lDefColumnZoom, 16);
	theSheet.m_theEventListOptionPage.m_lDefTrackWidth = CLIP (1, m_theEventListOption.m_lDefTrackWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefMillisecWidth = CLIP (1, m_theEventListOption.m_lDefMillisecWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefTimeWidth = CLIP (1, m_theEventListOption.m_lDefTimeWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefKindWidth = CLIP (1, m_theEventListOption.m_lDefKindWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefChWidth = CLIP (1, m_theEventListOption.m_lDefChWidth, 32);
	theSheet.m_theEventListOptionPage.m_lDefVal1Width = CLIP (1, m_theEventListOption.m_lDefVal1Width, 32);
	theSheet.m_theEventListOptionPage.m_lDefVal2Width = CLIP (1, m_theEventListOption.m_lDefVal2Width, 32);
	theSheet.m_theEventListOptionPage.m_lDefVal3Width = CLIP (1, m_theEventListOption.m_lDefVal3Width, 32);

	theSheet.m_theEventListOptionPage.m_bInsertEventAfter = m_theEventListOption.m_bInsertEventAfter;
	theSheet.m_theEventListOptionPage.m_bDuplicateEventAfter = m_theEventListOption.m_bDuplicateEventAfter;
	theSheet.m_theEventListOptionPage.m_bDeleteEventAfter = m_theEventListOption.m_bDeleteEventAfter;

	theSheet.m_theMusicalScoreOptionPage.m_lDefTrackZoom = CLIP (1, m_theMusicalScoreOption.m_lDefTrackZoom, 8);
	theSheet.m_theMusicalScoreOptionPage.m_lDefTimeZoom = CLIP (1, m_theMusicalScoreOption.m_lDefTimeZoom, 16);

	if (theSheet.DoModal () == IDOK) {
		m_theGeneralOption.m_bEnableMultiExec = theSheet.m_theGeneralOptionPage.m_bEnableMultiExec;
		m_theGeneralOption.m_bEnableMultiOpen = theSheet.m_theGeneralOptionPage.m_bEnableMultiOpen;
		m_theGeneralOption.m_bRestoreWindowPlacement = theSheet.m_theGeneralOptionPage.m_bRestoreWindowPlacement;
		m_theGeneralOption.m_bExecOpen = theSheet.m_theGeneralOptionPage.m_bExecOpen;
		m_theGeneralOption.m_bOpenPlay = theSheet.m_theGeneralOptionPage.m_bOpenPlay;
		m_theGeneralOption.m_bPlayUpdate = theSheet.m_theGeneralOptionPage.m_bPlayUpdate;
		m_theGeneralOption.m_bSearchUpdate = theSheet.m_theGeneralOptionPage.m_bSearchUpdate;
		m_theGeneralOption.m_bEnableCC111Loop = theSheet.m_theGeneralOptionPage.m_bEnableCC111Loop;
		m_theGeneralOption.m_bPatchSearch = theSheet.m_theGeneralOptionPage.m_bPatchSearch;
		m_theGeneralOption.m_bInvertCtrlMouseWheel = theSheet.m_theGeneralOptionPage.m_bInvertCtrlMouseWheel;
		m_theGeneralOption.m_bTrackZeroOrigin = theSheet.m_theGeneralOptionPage.m_bTrackZeroOrigin;
		m_theGeneralOption.m_bEventZeroOrigin = theSheet.m_theGeneralOptionPage.m_bEventZeroOrigin;
		m_theGeneralOption.m_bEnableAutoPageUpdate = theSheet.m_theGeneralOptionPage.m_bEnableAutoPageUpdate;
		m_theGeneralOption.m_bSendNoteOffHoldOffAtEnd = theSheet.m_theGeneralOptionPage.m_bSendNoteOffHoldOffAtEnd;

		m_theGeneralOption.m_lUpDownDelta1 = theSheet.m_theGeneralOptionPage.m_lUpDownDelta1;
		m_theGeneralOption.m_lUpDownDelta2 = theSheet.m_theGeneralOptionPage.m_lUpDownDelta2;
		m_theGeneralOption.m_lKeyVelocity1 = theSheet.m_theGeneralOptionPage.m_lKeyVelocity1;
		m_theGeneralOption.m_lKeyVelocity2 = theSheet.m_theGeneralOptionPage.m_lKeyVelocity2;
		m_theGeneralOption.m_lSpeedSlow = theSheet.m_theGeneralOptionPage.m_lSpeedSlow * 100;
		m_theGeneralOption.m_lSpeedNormal = theSheet.m_theGeneralOptionPage.m_lSpeedNormal * 100;
		m_theGeneralOption.m_lSpeedFast = theSheet.m_theGeneralOptionPage.m_lSpeedFast * 100;
		m_theGeneralOption.m_lPlayRecordInterval = theSheet.m_theGeneralOptionPage.m_lPlayRecordInterval;
		m_theGeneralOption.m_lOctaveSignature = theSheet.m_theGeneralOptionPage.m_lOctaveSignature;

		m_theTrackListOption1.m_lDefRowZoom = theSheet.m_theTrackListOption1Page.m_lDefRowZoom;
		m_theTrackListOption1.m_lDefColumnZoom = theSheet.m_theTrackListOption1Page.m_lDefColumnZoom;
		m_theTrackListOption1.m_lDefTimeZoom = theSheet.m_theTrackListOption1Page.m_lDefTimeZoom;
		m_theTrackListOption1.m_lDefNameWidth = theSheet.m_theTrackListOption1Page.m_lDefNameWidth;
		m_theTrackListOption1.m_lDefColorWidth = theSheet.m_theTrackListOption1Page.m_lDefColorWidth;
		m_theTrackListOption1.m_lDefInputOnWidth = theSheet.m_theTrackListOption1Page.m_lDefInputOnWidth;
		m_theTrackListOption1.m_lDefInputPortWidth = theSheet.m_theTrackListOption1Page.m_lDefInputPortWidth;
		m_theTrackListOption1.m_lDefInputChWidth = theSheet.m_theTrackListOption1Page.m_lDefInputChWidth;
		m_theTrackListOption1.m_lDefOutputOnWidth = theSheet.m_theTrackListOption1Page.m_lDefOutputOnWidth;
		m_theTrackListOption1.m_lDefOutputPortWidth = theSheet.m_theTrackListOption1Page.m_lDefOutputPortWidth;
		m_theTrackListOption1.m_lDefOutputChWidth = theSheet.m_theTrackListOption1Page.m_lDefOutputChWidth;
		m_theTrackListOption1.m_lDefViewModeWidth = theSheet.m_theTrackListOption1Page.m_lDefViewModeWidth;

		m_theTrackListOption2.m_lDefCC000Width = theSheet.m_theTrackListOption2Page.m_lDefCC000Width;
		m_theTrackListOption2.m_lDefCC032Width = theSheet.m_theTrackListOption2Page.m_lDefCC032Width;
		m_theTrackListOption2.m_lDefPCWidth = theSheet.m_theTrackListOption2Page.m_lDefPCWidth;
		m_theTrackListOption2.m_lDefCC007Width = theSheet.m_theTrackListOption2Page.m_lDefCC007Width;
		m_theTrackListOption2.m_lDefCC010Width = theSheet.m_theTrackListOption2Page.m_lDefCC010Width;
		m_theTrackListOption2.m_lDefCC091Width = theSheet.m_theTrackListOption2Page.m_lDefCC091Width;
		m_theTrackListOption2.m_lDefCC093Width = theSheet.m_theTrackListOption2Page.m_lDefCC093Width;
		m_theTrackListOption2.m_lDefCC094Width = theSheet.m_theTrackListOption2Page.m_lDefCC094Width;
		m_theTrackListOption2.m_lDefKeyShiftWidth = theSheet.m_theTrackListOption2Page.m_lDefKeyShiftWidth;
		m_theTrackListOption2.m_lDefVelShiftWidth = theSheet.m_theTrackListOption2Page.m_lDefVelShiftWidth;
		m_theTrackListOption2.m_lDefTimeShiftWidth = theSheet.m_theTrackListOption2Page.m_lDefTimeShiftWidth;
		m_theTrackListOption2.m_lDefNumEventWidth= theSheet.m_theTrackListOption2Page.m_lDefNumEventWidth;

		m_thePianoRollOption.m_lDefKeyZoom = theSheet.m_thePianoRollOptionPage.m_lDefKeyZoom;
		m_thePianoRollOption.m_lDefVelZoom = theSheet.m_thePianoRollOptionPage.m_lDefVelZoom;
		m_thePianoRollOption.m_lDefTimeZoom = theSheet.m_thePianoRollOptionPage.m_lDefTimeZoom;

		m_theEventListOption.m_lDefRowZoom = theSheet.m_theEventListOptionPage.m_lDefRowZoom;
		m_theEventListOption.m_lDefColumnZoom = theSheet.m_theEventListOptionPage.m_lDefColumnZoom;
		m_theEventListOption.m_lDefTrackWidth = theSheet.m_theEventListOptionPage.m_lDefTrackWidth;
		m_theEventListOption.m_lDefMillisecWidth = theSheet.m_theEventListOptionPage.m_lDefMillisecWidth;
		m_theEventListOption.m_lDefTimeWidth = theSheet.m_theEventListOptionPage.m_lDefTimeWidth;
		m_theEventListOption.m_lDefKindWidth = theSheet.m_theEventListOptionPage.m_lDefKindWidth;
		m_theEventListOption.m_lDefChWidth = theSheet.m_theEventListOptionPage.m_lDefChWidth;
		m_theEventListOption.m_lDefVal1Width = theSheet.m_theEventListOptionPage.m_lDefVal1Width;
		m_theEventListOption.m_lDefVal2Width = theSheet.m_theEventListOptionPage.m_lDefVal2Width;
		m_theEventListOption.m_lDefVal3Width = theSheet.m_theEventListOptionPage.m_lDefVal3Width;

		m_theEventListOption.m_bInsertEventAfter = 	theSheet.m_theEventListOptionPage.m_bInsertEventAfter;
		m_theEventListOption.m_bDuplicateEventAfter = theSheet.m_theEventListOptionPage.m_bDuplicateEventAfter;
		m_theEventListOption.m_bDeleteEventAfter = theSheet.m_theEventListOptionPage.m_bDeleteEventAfter;
		
		m_theMusicalScoreOption.m_lDefTrackZoom = theSheet.m_theMusicalScoreOptionPage.m_lDefTrackZoom;
		m_theMusicalScoreOption.m_lDefTimeZoom = theSheet.m_theMusicalScoreOptionPage.m_lDefTimeZoom;

		
		// �J���Ă���S�h�L�������g��MIDI�N���b�N�̃X�s�[�h�Ɠ������[�h�Đݒ�
		POSITION docpos = m_pSekaijuDocTemplate->GetFirstDocPosition ();
		while (docpos) {
			CSekaijuDoc* pSekaijuDoc =(CSekaijuDoc*)(m_pSekaijuDocTemplate->GetNextDoc (docpos));
			long lRunning = MIDIClock_IsRunning (pSekaijuDoc->m_pMIDIClock);
			pSekaijuDoc->m_theCriticalSection.Lock ();
			if (lRunning) {
				MIDIClock_Stop (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->ApplyAppCurSpeedIndex ();
			if (lRunning) {
				MIDIClock_Start (pSekaijuDoc->m_pMIDIClock);
			}
			pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
			pSekaijuDoc->m_theCriticalSection.Unlock ();
			::Sleep (0);		
		}
		
		// ���j���[�̍X�V
		UpdateMenu ();
	}
}

// �I�v�V�����v���p�e�B�V�[�g�Łw�K�p�x���������Ƃ�
void CSekaijuApp::ApplyOptionSheet (CPropertySheet* pSheet) {

}

// �w�w���v(&H)�x-�wreadme(&R)...�x
void CSekaijuApp::OnHelpReadMe () {
	// ShellExecute��GetOpenFileName��GetSaveFileName��
	// �f�o�b�O�łł̓A�N�Z�X�ᔽ���������邪�A
	// �����[�X�łł͖��Ȃ��Ƃ̏�񂠂�B
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	CString strFileName2;
	strFileName1.LoadString (IDS_READMEFILENAME);
	strFileName2 = m_strExeFilePath + strFileName1;
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName2, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "�������s�����̓��\�[�X�s���ł��B"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-�t�@�C�����J���܂���B"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName2);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}

// �w�w���v(&H)�x-�w���C�Z���X(&L)...�x
void CSekaijuApp::OnHelpLicense () {
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	CString strFileName2;
	strFileName1.LoadString (IDS_LICENSEFILENAME);
	strFileName2 = m_strExeFilePath + strFileName1;
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName2, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "�������s�����̓��\�[�X�s���ł��B"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-�t�@�C�����J���܂���B"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName2);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}

// �w�w���v(&H)�x-�w�戵������(PDF)(&M)...�x
void CSekaijuApp::OnHelpManual () {
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	CString strFileName2;
	strFileName1.LoadString (IDS_MANUALFILENAME);
	strFileName2 = m_strExeFilePath + strFileName1;
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName2, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "�������s�����̓��\�[�X�s���ł��B"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-�t�@�C�����J���܂���B"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName2);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}


// �w�w���v(&H)�x-�w�v���W�F�N�g�z�[���y�[�W(&P)...�x
void CSekaijuApp::OnHelpProjectHomePage () {
	CString strMsg;
	CString strMsg2;
	CString strFileName1;
	strFileName1.LoadString (IDS_HOMEPAGEADDRESS);
	long lRet = (long)::ShellExecute (this->m_pMainWnd->GetSafeHwnd (), 
		_T("open"), (LPCTSTR)strFileName1, NULL, NULL, SW_SHOW);
	if (lRet == 0) {
		// "�������s�����̓��\�[�X�s���ł��B"
		strMsg.LoadString (IDS_INSUFFICIENT_MEMORY_OR_INSUFFICIENT_RESOURCE);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
	}
	else if (1 <= lRet && lRet < 32) {
		// "%s\n-�t�@�C�����J���܂���B"
		strMsg.LoadString (IDS_S_N_FILE_OPEN_FAILED);
		strMsg2.Format (strMsg, strFileName1);
		AfxMessageBox (strMsg2, MB_ICONEXCLAMATION);
	}
}


// �w�w���v(&H)�x-�w���@�[�W�������(&A)...�x
void CSekaijuApp::OnHelpAbout () {
	CAboutDlg theDlg;
	theDlg.DoModal();
}
