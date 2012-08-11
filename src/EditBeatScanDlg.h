//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �r�[�g���o�ƃe���|�����}���_�C�A���O�N���X
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

#ifndef _EDITBEATSCANDLG_H_
#define _EDITBEATSCANDLG_H_

class CEditBeatScanDlg : public CDialog {
	//--------------------------------------------------------------------------
	// �A�g���r���[�g
	//--------------------------------------------------------------------------
public:
	long m_lTimeMode;                   // �^�C�����[�h
	long m_lTimeResolution;             // ����\[tick/4������]����[�T�u�t���[��/1�t���[��]
	CStringArray m_theTrackNameArray;   // �g���b�N���̔z��
	int m_nBeatTrackIndex;              // �r�[�g�̋L�^���ꂽ�g���b�N�ԍ�(0�`65535)
	int m_nBeatIntervalIndex;           // �r�[�g�̊Ԋu�C���f�b�N�X(0=4�������`)
	int m_nInsertTempo;                 // �e���|�������}�����邩�H

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CEditBeatScanDlg();                 // �R���X�g���N�^
	enum {IDD = IDD_EDITBEATSCAN};

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	BOOL FillBeatTrackCombo ();
	BOOL FillBeatIntervalCombo ();

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()

};

#endif
