//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// ���E���A�v���P�[�V�����N���X
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

#ifndef _AFXWIN_H_
#define _AFXWIN_H_

#include <afxmt.h>

#define MAXMIDIINDEVICENUM 16           // �ő�MIDI���̓f�o�C�X��
#define MAXMIDIOUTDEVICENUM 16          // �ő�MIDI�o�̓f�o�C�X��
#define MAXMIDITRACKNUM 65536           // �ő�MIDI�g���b�N��(1��MIDI�f�[�^�ɕt��)
#define MAXMIDIINSTRUMENTNUM 256        // �ő�MIDI�C���X�g�D�������g(*.ins)��

#define WM_COMMANDWAKEUP (WM_USER + 0)
#define WM_COMMANDREADSHM (WM_USER + 1)
#define WM_COMMANDFILEOPEN (WM_USER + 2)
#define SHMSIZE 1024

// �ėp�}�N��(�ŏ��A�ő�A���ݍ���)
#ifndef MIN
#define MIN(A,B) ((A)>(B)?(B):(A))      // A��B�̏������ق���Ԃ�
#endif
#ifndef MAX
#define MAX(A,B) ((A)>(B)?(A):(B))      // A��B�̑傫���ق���Ԃ�
#endif
#ifndef CLIP
#define CLIP(A,B,C) ((A)>(B)?(A):((B)>(C)?(C):(B))) //B��A��C�̊ԂŊۂ߂��l��Ԃ�
#endif

// TSIZEOF�}�N�� //20120211�ǉ�
#ifndef TSIZEOF
#define TSIZEOF(STR) (sizeof(STR)/sizeof(TCHAR))
#endif
#ifndef TCSLEN
#ifdef UNICODE
#define TCSLEN(STRING) wcslen(STRING)
#else
#define TCSLEN(STRING) strlen(STRING)
#endif
#endif
#ifndef TCSNCPY
#ifdef UNICODE
#define TCSNCPY(STRING1,STRING2,N) wcsncpy(STRING1,STRING2,N)
#else
#define TCSNCPY(STRING1,STRING2,N) strncpy(STRING1,STRING2,N)
#endif
#endif

// SendDiffrentStatus�̃t���O�p
#define SDS_NOTEOFF 0x0001              // �m�[�g�I�t�̏�Ԃ��ŐV��Ԃɕ�������
#define SDS_NOTEON 0x0002               // �m�[�g�I���̏�Ԃ��ŐV��Ԃɕ�������
#define SDS_NOTE (SDS_NOTEOFF | SDS_NOTEON) // �m�[�g�̏�Ԃ��ŐV��Ԃɕ�������
#define SDS_KEYAFTER 0x0004             // �L�[�A�t�^�[�^�b�`�̒l�����ŐV��Ԃɕ�������
#define SDS_CONTROLCHANGE 0x0008        // �R���g���[���`�F���W�̒l���ŐV��Ԃɕ�������
#define SDS_PROGRAMCHANGE 0x0010        // �v���O�����`�F���W�̒l���ŐV��Ԃɕ�������
#define SDS_CHANNELAFTER 0x0020         // �`�����l���A�t�^�[�^�b�`�̒l���ŐV��Ԃɕ�������
#define SDS_PITCHBEND 0x0040            // �s�b�`�x���h�̒l���ŐV��Ԃɕ�������
#define SDS_RPN 0x0100                  // RPN�̒l���ŐV��Ԃɕ�������
#define SDS_NRPN 0x0200                 // NRPN�̒l���ŐV��Ԃɕ�������
#define SDS_MASTERVOLUME 0x0400         // �}�X�^�[�{�����[���̒l���ŐV��Ԃɕ�������
#define SDS_ALL 0xFFFF                  // ���ׂĂ̒l���ŐV��Ԃɕ�������

#include "resource.h"                   // ���C�� �V���{��

// �E�B���h�E�̔z�u
typedef struct {
	int m_bIconic;                      // �A�C�R��������Ă��邩�H
	int m_bZoomed;                      // �ő剻����Ă��邩�H
	int m_nX;                           // ���C���E�B���h�E�����X���W[pixel]
	int m_nY;                           // ���C���E�B���h�E�����Y���W[pixel]
	int m_nWidth;                       // ���C���E�B���h�E�̕�[pixel]
	int m_nHeight;                      // ���C���E�B���h�E�̍���[pixel]
} WindowPlacement;

// �g���b�N�̕ύX�_�C�A���O���
typedef struct {
	int m_nAmount;                      // �g���b�N�ԍ����͕ύX��
	int m_nUnit;                        // �P��(0=��Ύw��,1=���Εω�)
} EditTrackDlgStatus;

// �^�C���̕ύX(TPQN�x�[�X�p)�_�C�A���O���
typedef struct {
	int m_nAmount;                      // �^�C���̕ύX��
	int m_nUnit;                        // �P��(0=����,1=��,2=�e�B�b�N,3=�p�[�Z���g)
} EditTimeDlgStatus;

// �^�C���̕ύX(SMPTE�x�[�X�p)�_�C�A���O���
typedef struct {
	int m_nAmount;                      // �^�C���̕ύX��
	int m_nUnit;                        // �P��(0=�T�u�t���[��,1=�t���[��,2=�p�[�Z���g)
} EditTimeSmpDlgStatus;

// �`�����l���̕ύX�_�C�A���O�̏��
typedef struct {
	int m_nAmount;                      // �g���b�N�ԍ����͕ύX��
	int m_nUnit;                        // �P��(0=��Ύw��,1=���Εω�)
} EditChannelStatus;

// �L�[�̕ύX�_�C�A���O�̏��
typedef struct {
	int m_nAmount;                      // �����̕ύX��
	int m_nUnit;                        // �P��(0=����, 1=�I�N�^�[�u)
	int m_nTargetNote;                  // �m�[�g�I���E�m�[�g�I�t��ΏۂƂ���(0/1)
	int m_nTargetKeyAfter;              // �L�[�A�t�^�[�^�b�`��ΏۂƂ���(0/1)
} EditKeyDlgStatus;

// �x���V�e�B�̕ύX�_�C�A���O�̏��
typedef struct {
	int m_nAmount;                      // �x���V�e�B�̕ύX��
	int m_nUnit;                        // �P��(0=����,1=�p�[�Z���g)
	int m_nTargetNoteOn;                // �m�[�g�I����Ώۂɂ���(0/1)
	int m_nTargetNoteOff;               // �m�[�g�I�t��Ώۂɂ���(0/1)
} EditVelocityDlgStatus;

// �������̕ύX�_�C�A���O�̏��
typedef struct {
	int m_nAmount;                      // �������̕ύX��
	int m_nUnit;                        // �P��(0=�e�B�b�N, 1=�p�[�Z���g)
} EditDurationDlgStatus;

// �l�̕ύX�_�C�A���O�̏��
typedef struct {
	int m_nAmount;                      // �l�̕ύX��
	int m_nUnit;                        // �P��(0=����,1=�p�[�Z���g)
	int m_nTargetKeyAfter;              // �L�[�A�t�^�[�^�b�`��ΏۂƂ���(0/1)
	int m_nTargetControlChange;         // �R���g���[���`�F���W��ΏۂƂ���(0/1)
	int m_nTargetChannelAfter;          // �`�����l���A�t�^�[�^�b�`��ΏۂƂ���(0/1)
	int m_nTargetPitchBend;             // �s�b�`�x���h��ΏۂƂ���(0/1)
} EditValueDlgStatus;

// �����̍ו����ƃg�������_�C�A���O�̏��
typedef struct {
	int m_nDurationIndex;               // �ו������1���̉������C���f�b�N�X(0=4�������`)
	int m_nEnableTrill;                 // �g������L���ɂ��邩�H
	int m_nKeyShift;                    // �g�������̃L�[�V�t�g
} EditBreakupAndTrillDlgStatus;

// �N�H���^�C�Y�_�C�A���O�̏��
typedef struct {
	int m_nSnapTimeIndex;               // �X�i�b�v�^�C���C���f�b�N�X(0=4�������`)
	int m_nStrength;                    // ���x[%]
	int m_nTargetNoteOn;                // �^�[�Q�b�g�m�[�g�I��
	int m_nTargetNoteOff;               // �^�[�Q�b�g�m�[�g�I�t
} EditQuantizeDlgStatus;

// �r�[�g���o�ƃe���|�����}���_�C�A���O�̏��
typedef struct {
	int m_nBeatTrackIndex;              // �r�[�g���L�q���ꂽ�g���b�N�ԍ�(0�`65535)
	int m_nBeatIntervalIndex;           // �r�[�g�Ԋu(1�`)[tick]
	int m_nInsertTempo;                 // �e���|�������}�����邩�H
} EditBeatScanDlgStatus;

// ���߂̑}���_�C�A���O�̏��
typedef struct {
	int m_nPosition;                    // �ʒu
	int m_nNumMeasure;                  // ���ߐ�
} EditInsertMeasureDlgStatus;

// ���߂̏����_�C�A���O�̏��
typedef struct {
	int m_nPosition;                    // �ʒu
	int m_nNumMeasure;                  // ���ߐ�
} EditRemoveMeasureDlgStatus;

// ���g���m�[���_�C�A���O�̏��
typedef struct {
	int m_nOn;                          // ���g���m�[���I��
	int m_nOutputPort;                  // �o�̓|�[�g(0�`15)
	int m_nOutputChannel;               // �o�̓`�����l��(0�`15)
	int m_nNoteKey1;                    // ���Ńm�[�g�L�[(0�`127)
	int m_nNoteVel1;                    // ���Ńx���V�e�B(1�`127)
	int m_nNoteKey2;                    // ��Ńm�[�g�L�[(0�`127)
	int m_nNoteVel2;                    // ��Ńx���V�e�B(1�`127)
} MetronomeDlgStatus;

// �����ۑ��_�C�A���O�̏��
typedef struct {
	int m_nOn;                          // �����ۑ��I��
	long m_lInterval;                   // �����ۑ��Ԋu(1�`7200)[�b]
	int m_nDisableWhilePlaying;         // ���t���͎����ۑ����Ȃ�
	int m_nDisableWhileRecording;       // ���A���^�C�����͒��͎����ۑ����Ȃ�
} AutoSaveDlgStatus;

// �I�v�V����(���)�y�[�W�̏��
typedef struct {
	BOOL m_bEnableMultiExec;            // �����̐��E�����N�����邱�Ƃ�������
	BOOL m_bEnableMultiOpen;            // ������MIDI�f�[�^���J�����Ƃ�������
	BOOL m_bRestoreWindowPlacement;     // �N�����ɑO��̃E�B���h�E�ʒu�𕜌�����
	BOOL m_bExecOpen;                   // �N�����ɍŌ�ɊJ����MIDI�f�[�^�������I�ɊJ��
	BOOL m_bOpenPlay;                   // MIDI�f�[�^���J���Ǝ����I�ɉ��t���J�n����
	BOOL m_bPlayUpdate;                 // ���t�ʒu�ړ����Ƀp�b�`��R���g���[����s�b�`�x���h�RPN�NRPN���ŐV�l�ɍX�V����
	BOOL m_bSearchUpdate;               // ���t�J�n���Ƀp�b�`��R���g���[����s�b�`�x���h�RPN�NRPN���ŐV�l�ɍX�V���� 
	BOOL m_bSearchSysx;                 // ���t�J�n���ɃV�X�e���G�N�X�N���[�V��(�}�X�^�[���H�����[���Ȃ�)���ŐV�l�ɍX�V����
	BOOL m_bEnableCC111Loop;            // �I�[�g���s�[�g����CC#111�̈ʒu���烋�[�v�J�n����
	BOOL m_bInvertCtrlMouseWheel;       // Ctrl+�}�E�X�z�C�[����]���̉��t�ʒu�ړ������𔽓]�ɂ���
	BOOL m_bTrackZeroOrigin;            // �g���b�N�ԍ���0���琔����
	BOOL m_bEventZeroOrigin;            // �C�x���g�ԍ���0���琔����
	BOOL m_bPatchSearch;                // CC#0�CC#32��v���O�����`�F���W���쎞�ɗL���ȉ��F��������������
	BOOL m_bEnableAutoPageUpdate;       // ���t�J�n���E�ʒu�ړ����Ɏ����I�y�[�W�X�V���I���ɂ���
	BOOL m_bSendNoteOffHoldOffAtEnd;    // �Ȃ̏I�[�ɒB�����Ƃ��m�[�g�I�t��z�[���h�I�t�𑗐M����
	long m_lUpDownDelta1;               // �����̍��N���b�N����[+][-]�L�[�ő��������(1�`127)
	long m_lUpDownDelta2;               // �����̉E�N���b�N����[Shift]+[+][-]�L�[�ő��������(1�`127)
	long m_lKeyVelocity1;               // ���Ղ̍��N���b�N����[Z]-[\]�L�[�Ŕ�������x���V�e�B(1�`127)
	long m_lKeyVelocity2;               // ���Ղ̉E�N���b�N����[Shift]+[Z]-[\]�L�[�Ŕ�������x���V�e�B(1�`127)
	long m_lSpeedSlow;                  // �X�s�[�h=�ᑬ�ŉ��t���̃e���|�{��(1�`<5000>�`100000)[*1/100%]
	long m_lSpeedNormal;                // �X�s�[�h=�W���ŉ��t���̃e���|�{��(1�`<10000>�`100000)[*1/100%]
	long m_lSpeedFast;                  // �X�s�[�h=�����ŉ��t���̃e���|�{��(1�`<20000>�`100000)[*1/100%]
	long m_lPlayRecordInterval;         // MIDI�f�[�^�^�����t����while���[�v�Ăяo���Ԋu(1�`1000)[�~���b]
	long m_lOctaveSignature;            // �����̃h(��60)�̵����ޔԍ��\�L(3�`5)
} GeneralOption;

// �g���b�N���X�g�I�v�V�����y�[�W1�̐ݒ�
typedef struct TTrackListOption1 {
	long m_lDefRowZoom;                 // �f�t�H���g�̍s�����g��{��[�{]
	long m_lDefColumnZoom;              // �f�t�H���g�̗�����g��{��[�{]
	long m_lDefTimeZoom;                // �f�t�H���g�̎��ԕ����g��{��[�{]
	long m_lDefNameWidth;               // �f�t�H���g�̖��O�̕�[pixel]
	long m_lDefColorWidth;              // �f�t�H���g�̐F�̕�[pixel]
	long m_lDefInputOnWidth;            // �f�t�H���g�̓���ON�̕�[pixel]
	long m_lDefInputPortWidth;          // �f�t�H���g�̓��̓|�[�g�̕�[pixel]
	long m_lDefInputChWidth;            // �f�t�H���g�̓���CH�̕�[pixel]
	long m_lDefOutputOnWidth;           // �f�t�H���g�̏o��ON�̕�[pixel]
	long m_lDefOutputPortWidth;         // �f�t�H���g�̏o�̓|�[�g�̕�[pixel]
	long m_lDefOutputChWidth;           // �f�t�H���g�̏o��CH�̕�[pixel]
	long m_lDefViewModeWidth;           // �f�t�H���g�̕\�����[�h�̕�[pixel]
} TrackListOption1;

// �g���b�N���X�g�I�v�V�����y�[�W2�̐ݒ�
typedef struct TTrackListOption2 {
	long m_lDefCC000Width;              // �f�t�H���g��CC#0�̕�[pixel]
	long m_lDefCC032Width;              // �f�t�H���g��CC#32�̕�[pixel]
	long m_lDefPCWidth;                 // �f�t�H���g�̃v���O�����i���o�[�̕�[pixel]
	long m_lDefCC007Width;              // �f�t�H���g�̃{�����[���̕�[pixel]
	long m_lDefCC010Width;              // �f�t�H���g�̃p���̕�[pixel]
	long m_lDefCC091Width;              // �f�t�H���g�̃��o�[�u�̕�[pixel]
	long m_lDefCC093Width;              // �f�t�H���g�̃R�[���X�̕�[pixel]
	long m_lDefCC094Width;              // �f�t�H���g�̃f�B���C�̕�[pixel]
	long m_lDefKeyShiftWidth;           // �f�t�H���g�̃L�[�V�t�g�̕�[pixel]
	long m_lDefVelShiftWidth;           // �f�t�H���g�̃x���V�e�B�V�t�g�̕�[pixel]
	long m_lDefTimeShiftWidth;          // �f�t�H���g�̃^�C���V�t�g�̕�[pixel]
	long m_lDefNumEventWidth;           // �f�t�H���g�̃C�x���g���̕�[pixel]
} TrackListOption2;

// �s�A�m���[���I�v�V�����y�[�W�̐ݒ�
typedef struct {
	long m_lDefKeyZoom;                 // �f�t�H���g�̃L�[�����g��{��[�{]
	long m_lDefVelZoom;                 // �f�t�H���g�̃x���V�e�B�����g��{��[�{]
	long m_lDefTimeZoom;                // �f�t�H���g�̎��ԕ����g��{��[�{]
} PianoRollOption;

// �C�x���g���X�g�I�v�V�����y�[�W�̐ݒ�
typedef struct {
	long m_lDefRowZoom;                 // �f�t�H���g�̍s�����g��{��[�{]
	long m_lDefColumnZoom;              // �f�t�H���g�̗�����g��{��[�{]
	long m_lDefTrackWidth;              // �f�t�H���g�̃g���b�N�̕�[pixel]
	long m_lDefMillisecWidth;           // �f�t�H���g�̃~���b�̕�[pixel]
	long m_lDefTimeWidth;               // �f�t�H���g�̃^�C���̕�[pixel]
	long m_lDefKindWidth;               // �f�t�H���g�̃C�x���g�̎�ނ̕�[pixel]
	long m_lDefChWidth;                 // �f�t�H���g�̃`�����l���̕�[pixel]
	long m_lDefVal1Width;               // �f�t�H���g�̒l1�̕�[pixel]
	long m_lDefVal2Width;               // �f�t�H���g�̒l2�̕�[pixel]
	long m_lDefVal3Width;               // �f�t�H���g�̒l3�̕�[pixel]
	BOOL m_bInsertEventAfter;           // �������̃C�x���g�̒���ɑ}������
	BOOL m_bDuplicateEventAfter;        // ���݂̃C�x���g�̒���ɑ}������
	BOOL m_bDeleteEventAfter;           // �폜�����C�x���g�̎��̃C�x���g�Ƀt�H�[�J�X�����킹��
} EventListOption;

// ���ʃI�v�V�����y�[�W�̐ݒ�
typedef struct {
	long m_lDefTrackZoom;               // �f�t�H���g�̃g���b�N�����g��{��[�{]
	long m_lDefTimeZoom;                // �f�t�H���g�̎��ԕ����g��{��[�{]
} MusicalScoreOption;




class CSekaijuApp : public CWinApp {

	
	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CSekaijuApp();                      // �R���X�g���N�^


	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	HINSTANCE m_hResourceDLL;
public:
	// �h�L�������g�e���v���[�g�ւ̃|�C���^��ێ�
	CDocTemplate* m_pSekaijuDocTemplate;
public:
	// MIDI�f�o�C�X�I�u�W�F�N�g��MIDI�X�e�[�^�X�I�u�W�F�N�g
	MIDIIn*     m_pMIDIIn[MAXMIDIINDEVICENUM];
	MIDIOut*    m_pMIDIOut[MAXMIDIOUTDEVICENUM];
	MIDIStatus* m_pMIDIStatus[MAXMIDIOUTDEVICENUM];
	MIDIStatus* m_pTempMIDIStatus[MAXMIDIOUTDEVICENUM];
	MIDIInstrument* m_pMIDIInstrument[MAXMIDIINSTRUMENTNUM]; // 20100220�C��
	MIDIInstrumentDefinition* m_pMIDIInstDefNorm[MAXMIDIOUTDEVICENUM];
	MIDIInstrumentDefinition* m_pMIDIInstDefDrum[MAXMIDIOUTDEVICENUM];
public:
	// MIDI���o�͂Ȃǂɑ΂���N���e�B�J���Z�N�V����
	CCriticalSection m_theCriticalSection;
public:
	// �p�X��
	CString m_strExeFilePath;           // ���s�t�@�C���̂���p�X��
	CString m_strInitialPath;           // ini�t�@�C���̂���p�X��
	CString m_strFileOpenPath;          // �t�@�C���I�[�v���_�C�A���O�̃p�X��
	CString m_strSysxOpenPath;          // SYSX�I�[�v���_�C�A���O�̃p�X��

	// MIDI���o�̓f�o�C�X�̖��O
	CString m_strMIDIInName[MAXMIDIINDEVICENUM];
	CString m_strMIDIOutName[MAXMIDIOUTDEVICENUM];

	// MIDI�C���X�g�D�������g��`�̖��O
	CString m_strMIDIInstDefNormName[MAXMIDIOUTDEVICENUM];
	CString m_strMIDIInstDefDrumName[MAXMIDIOUTDEVICENUM];

	// MIDI�������[�h
	long m_lMIDIInSyncMode[MAXMIDIINDEVICENUM];
	long m_lMIDIOutSyncMode[MAXMIDIOUTDEVICENUM];

	// �R���g���[�����
	BOOL m_bPlaying;       // ���t�t���O (0=��~��,1=���t��)
	BOOL m_bRecording;     // �^���t���O (0=��~��,1=�^����)
	BOOL m_bAutoRepeat;    // �I�[�g���s�[�g(0=OFF,1=ON)
	long m_lCurSpeedIndex; // �X�s�[�h (0=�Î~,1=�ᑬ,2=�W��,3=����,4=���@��ɃX���[�u)
	long m_lOldSpeedIndex; // �X�s�[�h (0=�Î~,1=�ᑬ,2=�W��,3=����,4=���@��ɃX���[�u)

	// �S�ʃI�v�V�������
	GeneralOption m_theGeneralOption;

	// �g���b�N���X�g�I�v�V����1���
	TrackListOption1 m_theTrackListOption1;

	// �g���b�N���X�g�I�v�V����2���
	TrackListOption2 m_theTrackListOption2;

	// �s�A�m���[���I�v�V�������
	PianoRollOption m_thePianoRollOption;

	// �C�x���g���X�g�I�v�V�������
	EventListOption m_theEventListOption;

	// ���ʃI�v�V�������
	MusicalScoreOption m_theMusicalScoreOption;
	
	// �E�B���h�E�z�u���
	WindowPlacement m_theWindowPlacement;

	// EditTrack�_�C�A���O���
	EditTrackDlgStatus m_theEditTrackDlgStatus;
	
	// EditTime�_�C�A���O���
	EditTimeDlgStatus m_theEditTimeDlgStatus;

	// EditTimeSmp�_�C�A���O���
	EditTimeSmpDlgStatus m_theEditTimeSmpDlgStatus;

	// EditChannel�_�C�A���O���
	EditChannelStatus m_theEditChannelDlgStatus;

	// EditKey�_�C�A���O���
	EditKeyDlgStatus m_theEditKeyDlgStatus;

	// EditVelocity�_�C�A���O���
	EditVelocityDlgStatus m_theEditVelocityDlgStatus;

	// EditDuration�_�C�A���O���
	EditDurationDlgStatus m_theEditDurationDlgStatus;

	// EditValue�_�C�A���O���
	EditValueDlgStatus m_theEditValueDlgStatus;

	// EditBreakupAndTrill�_�C�A���O���
	EditBreakupAndTrillDlgStatus m_theEditBreakupAndTrillDlgStatus;

	// EditQuantize�_�C�A���O���
	EditQuantizeDlgStatus m_theEditQuantizeDlgStatus;

	// EditBeatScan�_�C�A���O���
	EditBeatScanDlgStatus m_theEditBeatScanDlgStatus;

	// EditInsertMeasure�_�C�A���O���
	EditInsertMeasureDlgStatus m_theEditInsertMeasureDlgStatus;

	// EditRemoveMeasure�_�C�A���O���
	EditRemoveMeasureDlgStatus m_theEditRemoveMeasureDlgStatus;

	// Metronome�_�C�A���O���
	MetronomeDlgStatus m_theMetronomeDlgStatus;

	// �����ۑ��_�C�A���O���
	AutoSaveDlgStatus m_theAutoSaveDlgStatus;

	// �Ō�ɊJ�����t�@�C����
	CString m_strLastOpenFileName[16];

	// ����
	CString m_strLanguage;

	// �f�t�H���g�t�H���g
	CFont m_theDefaultFont;

	// �f�t�H���g�g���b�N�J���[
	long m_lDefTrackColor[16];

	// �J�[�\��
	HCURSOR m_hCursorArrow;
	HCURSOR m_hCursorCross;
	HCURSOR m_hCursorSizeWE;
	HCURSOR m_hCursorSizeNS;
	HCURSOR m_hCursorSizeAll;
	HCURSOR m_hCursorSizeAllCopy;
	HCURSOR m_hCursorNo;
	HCURSOR m_hCursorResizeWE;
	HCURSOR m_hCursorResizeNS;
	HCURSOR m_hCursorResizeAll;
	HCURSOR m_hCursorDraw;
	HCURSOR m_hCursorLine;
	HCURSOR m_hCursorEraser;
	HCURSOR m_hCursorSelect;
	HCURSOR m_hCursorSelectAdd;
	HCURSOR m_hCursorSelect2;
	HCURSOR m_hCursorSelectAdd2;
	HCURSOR m_hCursorSpeaker;

	// �C�x���g�̎�ޖ�
	CString m_strEventKindName[256];

	// �m�[�g�L�[��
	CString m_strNoteKeyName[256];

	// �ꎞ�I�ȕϐ�
	BOOL m_bIgnoreNoteEvent;
	BOOL m_bFirstMetronome;
	CPtrArray m_theTempRecordedEventArray;
	BOOL m_bInplaceEditing;
	BOOL m_bInplaceListing;
	BOOL m_bValueUpDowning;

	// ���݂̎q�E�B���h�E�ƌ��݂̃h�L�������g
	CMDIChildWnd* m_pCurChildWnd;
	CDocument* m_pCurDocument;
	CMDIChildWnd* m_pOldChildWnd;
	CDocument* m_pOldDocument;

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual BOOL InitInstance ();
	virtual int  ExitInstance ();
	virtual void AddDocTemplate (CDocTemplate* pTemplate);
	virtual BOOL PreTranslateMessage (MSG* pMsg);
  	virtual BOOL OnIdle (LONG lCount);

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	virtual BOOL LoadResourceDLL ();
	virtual BOOL FreeResourceDLL ();
	virtual BOOL CheckMultiExec (CCommandLineInfo* pCmdInfo);
	virtual BOOL LoadIniFile ();
	virtual BOOL SaveIniFile ();
	virtual void UpdateMenu ();
	virtual void UpdateCurWndAndDocPtr ();
	virtual void ResetMIDIStatusArray ();
	virtual void ResetTempMIDIStatusArray ();
	virtual void SendAllHold1Off ();
	virtual void SendAllSostenutoOff ();
	virtual void SendAllHold2Off ();
	virtual void SendAllSoundOff ();
	virtual void SendResetAllController ();
	virtual void SendAllNoteOff ();
	virtual void SendDifferentStatus (long lFlags);
	virtual void OpenAllMIDIInDevice ();
	virtual void OpenAllMIDIOutDevice ();
	virtual long LoadAllMIDIInstrument ();
	virtual long SelectAllMIDIInstDefNorm ();
	virtual long SelectAllMIDIInstDefDrum ();
	virtual long SetPlayPosition (CDocument* pDocument, long lTargetTime);
	virtual long StartPlaying (CDocument* pDocument);
	virtual long StopPlaying (CDocument* pDocument);
	virtual long StartRecording (CDocument* pDocument);
	virtual long StopRecording (CDocument* pDocument);
	virtual void ApplyMIDIDeviceSheet (CPropertySheet* pSheet);
	virtual void ApplyMIDISyncModeSheet (CPropertySheet* pSheet);
	virtual void ApplyOptionSheet (CPropertySheet* pSheet);
	virtual BOOL GetCurSyncInputPortAndMode (long* pPort, long* pMode);

public:
	// ���t�E�^���p�X���b�h�֘A
	static UINT PlayRecordThread (LPVOID pInfo);
	virtual BOOL PlayRecordProc (LPVOID pInfo);
	CWinThread* m_pPlayRecordThread;
	BOOL m_bPlayRecordThreadRunning;

	//--------------------------------------------------------------------------
	// �C���v�������e�[�V����
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnUpdateFileNewUI (CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpenUI (CCmdUI* pCmdUI);
	afx_msg void OnControlToBegin ();
	afx_msg void OnUpdateControlToBeginUI (CCmdUI* pCmdUI);
	afx_msg void OnControlToEnd ();
	afx_msg void OnUpdateControlToEndUI (CCmdUI* pCmdUI);
	afx_msg void OnControlPlay ();
	afx_msg void OnUpdateControlPlayUI (CCmdUI* pCmdUI);
	afx_msg void OnControlRecord ();
	afx_msg void OnUpdateControlRecordUI (CCmdUI* pCmdUI);
	afx_msg void OnControlPrevMeasure ();
	afx_msg void OnUpdateControlPrevMeasureUI (CCmdUI* pCmdUI);
	afx_msg void OnControlNextMeasure ();
	afx_msg void OnUpdateControlNextMeasureUI (CCmdUI* pCmdUI);
	afx_msg void OnControlPrevBeat ();
	afx_msg void OnUpdateControlPrevBeatUI (CCmdUI* pCmdUI);
	afx_msg void OnControlNextBeat ();
	afx_msg void OnUpdateControlNextBeatUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedNone ();
	afx_msg void OnUpdateControlSpeedNoneUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedSlow ();
	afx_msg void OnUpdateControlSpeedSlowUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedNormal ();
	afx_msg void OnUpdateControlSpeedNormalUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedFast ();
	afx_msg void OnUpdateControlSpeedFastUI (CCmdUI* pCmdUI);
	afx_msg void OnControlSpeedSlave ();
	afx_msg void OnUpdateControlSpeedSlaveUI (CCmdUI* pCmdUI);
	afx_msg void OnControlAutoRepeat ();
	afx_msg void OnUpdateControlAutoRepeatUI (CCmdUI* pCmdUI);
	afx_msg void OnSetupMIDIDevice ();
	afx_msg void OnSetupMIDISyncMode ();
	afx_msg void OnSetupInstrument ();
	afx_msg void OnSetupMetronome ();
	afx_msg void OnSetupAutoSave ();
	afx_msg void OnSetupLanguage ();
	afx_msg void OnSetupOptions ();
	afx_msg void OnHelpReadMe ();
	afx_msg void OnHelpLicense ();
	afx_msg void OnHelpManual ();
	afx_msg void OnHelpProjectHomePage ();
	afx_msg void OnHelpAbout ();
	DECLARE_MESSAGE_MAP()
};

#endif
