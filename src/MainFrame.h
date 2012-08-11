//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// MDI�e�t���[���E�B���h�E�N���X
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

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

class CMainFrame : public CMDIFrameWnd {
	DECLARE_DYNAMIC (CMainFrame)

	//--------------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------------
public:
	CString     m_strWndClass;          // �E�B���h�E�N���X��
	CString     m_strTempFileName;      // �ꎞ�I�ȃt�@�C�����i�[�ꏊ
		
	//--------------------------------------------------------------------------
	// �c�[���o�[�ƃX�e�[�^�X�o�[�Ƃ��̓����̃E�B���h�E
	//--------------------------------------------------------------------------
public:  
	CStatusBar  m_wndStatusBar;         // �X�e�[�^�X�o�[
	CSekaijuToolBar m_wndToolBar1;      // �c�[���o�[1
	CSekaijuToolBar m_wndToolBar2;      // �c�[���o�[2
	CSekaijuToolBar m_wndToolBar3;      // �c�[���o�[3(���g�p)
	CEdit       m_wndMillisecEdit;      // ��:��:�b:�~���b�p�G�f�B�b�g(�ǂݎ���p)
	CEdit       m_wndTimeEdit;          // ����:��:�e�B�b�Nor�t���[��:�T�u�t���[���p�G�f�B�b�g(�ǂݎ���p)
	CScrollBar  m_wndPositionScroll;    // �ʒu�X�N���[���o�[
	CEdit       m_wndMeasureEdit;       // ���q�L���E�����L���p�G�f�B�b�g(�ǂݎ���p)
	CEdit       m_wndTempoEdit;         // �e���|�p�G�f�B�b�g(�ǂݎ���p)

	//--------------------------------------------------------------------------
	// �\�z�Ɣj��
	//--------------------------------------------------------------------------
public:
	CMainFrame ();                      // �R���X�g���N�^
	virtual ~CMainFrame();              // �f�X�g���N�^

	//--------------------------------------------------------------------------
	// �I�y���[�V����
	//--------------------------------------------------------------------------
public:
	void SetPositionScrollRange (long lStartTime, long lEndTime, BOOL bRedraw);
	int SetWindowTextWhenDifferent (CWnd* pWnd, LPCTSTR lpszText);

	//--------------------------------------------------------------------------
	// �I�[�o�[���C�h
	//--------------------------------------------------------------------------
public:
	virtual CFrameWnd* GetActiveFrame ();
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);


	//--------------------------------------------------------------------------
	// ���b�Z�[�W�}�b�v
	//--------------------------------------------------------------------------
protected:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy ();
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	afx_msg void OnViewToolbar1 ();
	afx_msg void OnUpdateViewToolbar1UI (CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar2 ();
	afx_msg void OnUpdateViewToolbar2UI (CCmdUI* pCmdUI);
	afx_msg long OnCommandWakeUp (WPARAM wParam, LPARAM lParam);
	afx_msg long OnCommandReadShm (WPARAM wParam, LPARAM lParam);
	afx_msg long OnCommandFileOpen (WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP ()

};

#endif
