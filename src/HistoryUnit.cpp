//******************************************************************************
// MIDI�V�[�P���T�[�\�t�g�w���E���x
// �������j�b�g�N���X
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

#include "winver.h"
#include <afxwin.h>
#include <afxmt.h>
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuDoc.h"

//------------------------------------------------------------------------------
// �\�z�Ɣj��
//------------------------------------------------------------------------------

// �R���X�g���N�^
CHistoryUnit::CHistoryUnit () {
	m_strName = _T("");
}

// �f�X�g���N�^
CHistoryUnit::~CHistoryUnit () {
	long i;
	long lHistoryRecordCount = m_theHistoryRecordArray.GetSize ();
	for (i = lHistoryRecordCount - 1; i >= 0; i--) {
		MIDIEvent* pMIDIEvent = NULL;
		MIDITrack * pMIDITrack = NULL;
		MIDIData* pMIDIData = NULL;
		CHistoryRecord* pHistoryRecord = (CHistoryRecord*)(m_theHistoryRecordArray.GetAt(i));
		switch (pHistoryRecord->m_lType) {
		case HISTORYRECORD_INSERTEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			// �h�L�������g�Ŏg���Ă��Ȃ��C�x���g�͎��̂��폜
			if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_ALIVE) == 0) {
				MIDIEvent_DeleteSingle (pMIDIEvent);
				pMIDIEvent = NULL;
			}
			break; // 20080714break�Y��C��
		case HISTORYRECORD_REMOVEEVENT:
			pMIDIEvent = (MIDIEvent*)(pHistoryRecord->m_pObject);
			// �h�L�������g�Ŏg���Ă��Ȃ��C�x���g�͎��̂��폜
			if ((pMIDIEvent->m_lUserFlag & MIDIEVENT_ALIVE) == 0 &&
				(pMIDIEvent->m_lUserFlag & MIDIEVENT_RESISTEREDASINSERTED) == 0) {
				MIDIEvent_DeleteSingle (pMIDIEvent);
				pMIDIEvent = NULL;
			}
			break;
		case HISTORYRECORD_INSERTTRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			// �h�L�������g�Ŏg���Ă��Ȃ��g���b�N�͎��̂��폜
			if ((pMIDITrack->m_lUserFlag & MIDITRACK_ALIVE) == 0) {
				MIDITrack_Delete (pMIDITrack);
				pMIDITrack = NULL;
			}
			break; // 20080714break�Y��C��
		case HISTORYRECORD_REMOVETRACK:
			pMIDITrack = (MIDITrack*)(pHistoryRecord->m_pObject);
			// �h�L�������g�Ŏg���Ă��Ȃ��g���b�N�͎��̂��폜
			if ((pMIDITrack->m_lUserFlag & MIDITRACK_ALIVE) == 0 &&
				(pMIDITrack->m_lUserFlag & MIDITRACK_RESISTEREDASINSERTED) == 0) {
				MIDITrack_Delete (pMIDITrack);
				pMIDITrack = NULL;
			}
			break;
		case HISTORYRECORD_INSERTDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			// �h�L�������g�Ŏg���Ă��Ȃ�MIDIData�I�u�W�F�N�g�͎��̂��폜
			if ((pMIDIData->m_lUserFlag & MIDIDATA_ALIVE) == 0) {
				MIDIData_Delete (pMIDIData);
				pMIDIData = NULL;
			}
			break;
		case HISTORYRECORD_REMOVEDATA:
			pMIDIData = (MIDIData*)(pHistoryRecord->m_pObject);
			// �h�L�������g�Ŏg���Ă��Ȃ�MIDIData�I�u�W�F�N�g�͎��̂��폜
			if ((pMIDIData->m_lUserFlag & MIDIDATA_ALIVE) == 0 &&
				(pMIDIData->m_lUserFlag & MIDIDATA_RESISTEREDASINSERTED) == 0) {
				MIDIData_Delete (pMIDIData);
				pMIDIData = NULL;
			}
			break;
		}
		delete pHistoryRecord;
	}
}

//------------------------------------------------------------------------------
// �I�y���[�V����
//------------------------------------------------------------------------------

// ���̗������j�b�g�ɗ����L�^���ЂƂǉ�
long CHistoryUnit::AddHistoryRecord (long lType, void* pObject) {
	long lCount = 0;
	MIDIEvent* pMIDIEvent = NULL;
	MIDIEvent* pNextEvent = NULL;
	MIDIEvent* pPrevEvent = NULL;
	MIDITrack* pMIDITrack = NULL;
	MIDIEvent* pTempTrack = NULL;
	MIDIData* pMIDIData = NULL;
	CHistoryRecord* pHistoryRecord = NULL;
	switch (lType) {
	// MIDIEvent�I�u�W�F�N�g�}���̋L�^
	case HISTORYRECORD_INSERTEVENT:
		pMIDIEvent = (MIDIEvent*)pObject;
		pMIDIEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pNextEvent = pMIDIEvent->m_pNextEvent;
			while (pNextEvent) {
				if (pNextEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
					break;
				}
				pNextEvent = pNextEvent->m_pNextEvent;
			}
			pHistoryRecord->m_pNextObject = pNextEvent;
			pPrevEvent = pMIDIEvent->m_pPrevEvent;
			while (pPrevEvent) {
				if (pPrevEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
					break;
				}
				pPrevEvent = pPrevEvent->m_pPrevEvent;
			}
			pHistoryRecord->m_pPrevObject = pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASINSERTED;
			//_RPTF4 (_CRT_WARN, "HISTORYRECORD_INSERTEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//		(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//FILE* pFile = fopen ("history.txt", "at");
			//fprintf (pFile, "HISTORYRECORD_INSERTEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//	(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//fclose (pFile);
			
			m_theHistoryRecordArray.Add (pHistoryRecord);
			pMIDIEvent = pMIDIEvent->m_pNextCombinedEvent;
			lCount++;
		}
		break;
	// MIDIEvent�I�u�W�F�N�g�����̋L�^
	case HISTORYRECORD_REMOVEEVENT:
		pMIDIEvent = (MIDIEvent*)pObject;
		pMIDIEvent = MIDIEvent_GetFirstCombinedEvent (pMIDIEvent);
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pNextEvent = pMIDIEvent->m_pNextEvent;
			while (pNextEvent) {
				//TODO ���̔��莮�����K�v���s���B
				if (pNextEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
				//if (!(pNextEvent->m_lUserFlag & MIDIEVENT_DEAD)) {
					break;
				}
				pNextEvent = pNextEvent->m_pNextEvent;
			}
			pHistoryRecord->m_pNextObject = pNextEvent;
			pPrevEvent = pMIDIEvent->m_pPrevEvent;
			while (pPrevEvent) {
				//TODO ���̔��莮�����K�v���s���B
				if (pPrevEvent->m_lUserFlag & MIDIEVENT_ALIVE) {
				//if (!(pPrevEvent->m_lUserFlag & MIDIEVENT_DEAD)) {
					break;
				}
				pPrevEvent = pPrevEvent->m_pPrevEvent;
			}
			pHistoryRecord->m_pPrevObject = pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASREMOVED;
			m_theHistoryRecordArray.Add (pHistoryRecord);
			//_RPTF4 (_CRT_WARN, "HISTORYRECORD_REMOVEEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//		(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//FILE* pFile = fopen ("history.txt", "at");
			//fprintf (pFile, "HISTORYRECORD_REMOVEEVENT (%08x(%08x), prev=%08x, next=%08x)\n",
			//	(long)pMIDIEvent, pMIDIEvent->m_lData, (long)pPrevEvent, (long)pNextEvent);
			//fclose (pFile);
			pMIDIEvent = pMIDIEvent->m_pNextCombinedEvent;
			lCount++;
		}
		break;
	// MIDIEvent�I�u�W�F�N�g�S�}���̋L�^(20090116�ǉ�)
	case HISTORYRECORD_INSERTEVENTALL:
		pMIDIEvent = (MIDIEvent*)pObject;
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pHistoryRecord->m_pNextObject = pMIDIEvent->m_pNextEvent;
			pHistoryRecord->m_pPrevObject = pMIDIEvent->m_pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASINSERTED;
			m_theHistoryRecordArray.Add (pHistoryRecord);
			pMIDIEvent = pMIDIEvent->m_pNextEvent;
			lCount++;
		}
		break;
	// MIDIEvent�I�u�W�F�N�g�S�����̋L�^(20090116�ǉ�)
	case HISTORYRECORD_REMOVEEVENTALL:
		pMIDIEvent = (MIDIEvent*)pObject;
		ASSERT (pMIDIEvent);
		while (pMIDIEvent) {
			ASSERT (pMIDIEvent->m_pParent);
			pHistoryRecord = new CHistoryRecord ();
			pHistoryRecord->m_lType = lType;
			pHistoryRecord->m_pObject = pMIDIEvent;
			pHistoryRecord->m_pFirstChild = NULL;
			pHistoryRecord->m_pLastChild = NULL;
			pHistoryRecord->m_pNextObject = pMIDIEvent->m_pNextEvent;
			pHistoryRecord->m_pPrevObject =pMIDIEvent->m_pPrevEvent;
			pHistoryRecord->m_pParent = pMIDIEvent->m_pParent;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_RESISTEREDASREMOVED;
			m_theHistoryRecordArray.Add (pHistoryRecord);
			pMIDIEvent = pMIDIEvent->m_pNextEvent;
			lCount++;
		}
		break;
	// MIDITrack�I�u�W�F�N�g�}���̋L�^
	case HISTORYRECORD_INSERTTRACK:
		pMIDITrack = (MIDITrack*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDITrack;
		pHistoryRecord->m_pFirstChild = pMIDITrack->m_pFirstEvent;
		pHistoryRecord->m_pLastChild = pMIDITrack->m_pLastEvent;
		pHistoryRecord->m_pPrevObject = pMIDITrack->m_pPrevTrack;
		pHistoryRecord->m_pNextObject = pMIDITrack->m_pNextTrack;
		pHistoryRecord->m_pParent = pMIDITrack->m_pParent;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
		pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
		pMIDITrack->m_lUserFlag |= MIDITRACK_RESISTEREDASINSERTED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;
	// MIDITrack�I�u�W�F�N�g�����̋L�^
	case HISTORYRECORD_REMOVETRACK:
		pMIDITrack = (MIDITrack*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDITrack;
		pHistoryRecord->m_pFirstChild = pMIDITrack->m_pFirstEvent;
		pHistoryRecord->m_pLastChild = pMIDITrack->m_pLastEvent;
		pHistoryRecord->m_pPrevObject = pMIDITrack->m_pPrevTrack;
		pHistoryRecord->m_pNextObject = pMIDITrack->m_pNextTrack;
		pHistoryRecord->m_pParent = pMIDITrack->m_pParent;
		pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
		pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
		pMIDITrack->m_lUserFlag |= MIDITRACK_RESISTEREDASREMOVED;
		forEachEvent (pMIDITrack, pMIDIEvent) {
			pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
			pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;

	// MIDIData�I�u�W�F�N�g�}���̋L�^
	case HISTORYRECORD_INSERTDATA:
		pMIDIData = (MIDIData*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDIData;
		pHistoryRecord->m_pFirstChild = pMIDIData->m_pFirstTrack;
		pHistoryRecord->m_pLastChild = pMIDIData->m_pLastTrack;
		pHistoryRecord->m_pPrevObject = NULL;
		pHistoryRecord->m_pNextObject = NULL;
		pHistoryRecord->m_pParent = NULL;
		pMIDIData->m_lUserFlag &= ~MIDIDATA_DEAD;
		pMIDIData->m_lUserFlag |= MIDIDATA_ALIVE;
		pMIDIData->m_lUserFlag |= MIDIDATA_RESISTEREDASINSERTED;
		forEachTrack (pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag |= MIDITRACK_ALIVE;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_ALIVE;
			}
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;
	// MIDIData�I�u�W�F�N�g�����̋L�^
	case HISTORYRECORD_REMOVEDATA:
		pMIDIData = (MIDIData*)pObject;
		pHistoryRecord = new CHistoryRecord ();
		pHistoryRecord->m_lType = lType;
		pHistoryRecord->m_pObject = pMIDIData;
		pHistoryRecord->m_pFirstChild = pMIDIData->m_pFirstTrack;
		pHistoryRecord->m_pLastChild = pMIDIData->m_pLastTrack;
		pHistoryRecord->m_pPrevObject = NULL;
		pHistoryRecord->m_pNextObject = NULL;
		pHistoryRecord->m_pParent = NULL;
		pMIDIData->m_lUserFlag |= MIDIDATA_DEAD;
		pMIDIData->m_lUserFlag &= ~MIDIDATA_ALIVE;
		pMIDIData->m_lUserFlag |= MIDIDATA_RESISTEREDASREMOVED;
		forEachTrack (pMIDIData, pMIDITrack) {
			pMIDITrack->m_lUserFlag |= MIDITRACK_DEAD;
			pMIDITrack->m_lUserFlag &= ~MIDITRACK_ALIVE;
			forEachEvent (pMIDITrack, pMIDIEvent) {
				pMIDIEvent->m_lUserFlag |= MIDIEVENT_DEAD;
				pMIDIEvent->m_lUserFlag &= ~MIDIEVENT_ALIVE;
			}
		}
		m_theHistoryRecordArray.Add (pHistoryRecord);
		lCount = 1;
		break;
	}
	return lCount;
}
