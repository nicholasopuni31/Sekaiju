//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �����L�^�N���X
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

#ifndef _HISTORYRECORD_H_
#define _HISTORYRECORD_H_

#define HISTORYRECORD_INSERTEVENT 0x0001        // MIDI�C�x���g�}��
#define HISTORYRECORD_REMOVEEVENT 0x0002        // MIDI�C�x���g�폜
#define HISTORYRECORD_INSERTEVENTALL 0x0003     // MIDI�C�x���g�S�}��
#define HISTORYRECORD_REMOVEEVENTALL 0x0004     // MIDI�C�x���g�S�폜
#define HISTORYRECORD_INSERTTRACK 0x0011        // MIDI�g���b�N�}��
#define HISTORYRECORD_REMOVETRACK 0x0012        // MIDI�g���b�N�폜
#define HISTORYRECORD_INSERTDATA  0x0021        // MIDI�f�[�^�}��(���g�p)
#define HISTORYRECORD_REMOVEDATA  0x0022        // MIDI�f�[�^�폜(���g�p)

class CHistoryRecord {

	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lType;           // �L�^�^�C�v
	void* m_pObject;        // �I�u�W�F�N�g�ւ̃|�C���^
	void* m_pPrevObject;    // �I�u�W�F�N�g���o���������N���X�g�v�f�̏ꍇ�̑O�̃I�u�W�F�N�g�ւ̃|�C���^
	void* m_pNextObject;    // �I�u�W�F�N�g���o���������N���X�g�v�f�̏ꍇ�̎��̃I�u�W�F�N�g�ւ̃|�C���^
	void* m_pFirstChild;    // �I�u�W�F�N�g���q�����ꍇ�̍ŏ��̎q�I�u�W�F�N�g�ւ̃|�C���^
	void* m_pLastChild;     // �I�u�W�F�N�g���q�����ꍇ�̍Ō�̎q�I�u�W�F�N�g�ւ̃|�C���^
	void* m_pParent;        // �I�u�W�F�N�g���e�����ꍇ�̐e�I�u�W�F�N�g�ւ̃|�C���^

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CHistoryRecord ();                  // �R���X�g���N�^
	virtual ~CHistoryRecord ();         // �f�X�g���N�^
};

#endif

