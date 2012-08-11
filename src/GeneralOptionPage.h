//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �I�v�V����(�S��)�v���p�e�B�y�[�W�N���X
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

#ifndef _GENERALOPTIONPAGE_H_
#define _GENERALOPTIONPAGE_H_

class CGeneralOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	BOOL m_bEnableMultiExec;            // �����̐��E�����N�����邱�Ƌ�����
	BOOL m_bEnableMultiOpen;            // ������MIDI�f�[�^���J�����Ƃ�������
	BOOL m_bRestoreWindowPlacement;     // �N�����ɑO��̃E�B���h�E�ʒu�𕜌�����
	BOOL m_bExecOpen;                   // �N�����ɍŌ�ɊJ����MIDI�f�[�^�������I�ɊJ��
	BOOL m_bOpenPlay;                   // MIDI�f�[�^���J���Ǝ����I�ɉ��t���J�n����
	BOOL m_bPlayUpdate;                 // ���t�ʒu�ړ����Ƀp�b�`��R���g���[����s�b�`�x���h�RPN�NRPN���ŐV�l�ɍX�V���� 
	BOOL m_bSearchUpdate;               // ���t�J�n���Ƀp�b�`��R���g���[����s�b�`�x���h�RPN�NRPN���ŐV�l�ɍX�V����
	BOOL m_bEnableCC111Loop;            // �I�[�g���s�[�g����CC#111�̈ʒu���烋�[�v�J�n����
	BOOL m_bPatchSearch;                // CC#0�CC#32��v���O�����`�F���W���쎞�ɗL���ȉ��F��������������
	BOOL m_bInvertCtrlMouseWheel;       // Ctrl+�}�E�X�z�C�[���̉��t�ʒu�ړ������𔽓]
	BOOL m_bTrackZeroOrigin;            // �g���b�N�ԍ���0���琔����
	BOOL m_bEventZeroOrigin;            // �C�x���g�ԍ���0���琔����
	BOOL m_bEnableAutoPageUpdate;       // ���t�J�n���E�ʒu�ړ����Ɏ����I�y�[�W�X�V���I���ɂ���B
	BOOL m_bSendNoteOffHoldOffAtEnd;    // �Ȃ̏I�[�ɒB�����Ƃ��m�[�g�I�t��z�[���h�I�t�𑗐M����
	long m_lUpDownDelta1;               // �����̍��N���b�N����[+][-]�L�[�ő��������(1�`127)
	long m_lUpDownDelta2;               // �����̉E�N���b�N����[Shift]+[+][-]�L�[�ő��������(1�`127)
	long m_lKeyVelocity1;               // ���Ղ̍��N���b�N����[Z]-[\]�L�[�Ŕ�������x���V�e�B(1�`127)
	long m_lKeyVelocity2;               // ���Ղ̉E�N���b�N����[Shift]+[Z]-[\]�L�[�Ŕ�������x���V�e�B(1�`127)
	long m_lSpeedSlow;                  // �X�s�[�h=�ᑬ�ŉ��t���̃e���|�{��(1�`<50>�`1000)[%]
	long m_lSpeedNormal;                // �X�s�[�h=�W���ŉ��t���̃e���|�{��(1�`<100>�`1000)[%]
	long m_lSpeedFast;                  // �X�s�[�h=�����ŉ��t���̃e���|�{��(1�`<200>�`1000)[%]
	long m_lPlayRecordInterval;         // MIDI�f�[�^�^�����t����while���[�v�Ăяo���Ԋu(1�`1000)[�~���b]
	long m_lOctaveSignature;            // �����̃h(��60)�̵����ޔԍ��\�L(3�`5)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CGeneralOptionPage ();              // �R���X�g���N�^
	enum {IDD = IDD_GENERALOPTION};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
};

#endif
