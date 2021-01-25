
#include "main.h"

CActorPool::CActorPool()
{
	memset(m_pActors, 0, sizeof(CActor*) * MAX_ACTORS);
	memset(m_bSlotState, 0, sizeof(bool) * MAX_ACTORS);
	memset(m_iVirtualWorld, 0, sizeof(int) * MAX_ACTORS);

	m_iLastActorID = -1;
}

CActorPool::~CActorPool()
{
	for (unsigned short i = 0; i < MAX_ACTORS; i++)
	{
		//Destroy(i);
		
		SAFE_DELETE(m_pActors[i]);
		m_bSlotState[i] = false;
	}

	m_iLastActorID = -1;
}

CActor* CActorPool::GetAt(int iActorID)
{
	if (iActorID >= 0 && iActorID < MAX_ACTORS)
	{
		return  m_pActors[iActorID];
	}
	return NULL;
}

bool CActorPool::GetSlotState(int iActorID)
{
	if (iActorID >= 0 && iActorID < MAX_ACTORS)
	{
		return  m_bSlotState[iActorID];
	}
	return false;
}

void CActorPool::UpdateLastActorID()
{
	m_iLastActorID = -1;

	for (int i = 0; i < MAX_ACTORS; i++)
	{
		if (m_bSlotState[i])
		{
			m_iLastActorID = i;
		}
	}
}

unsigned short CActorPool::New(int iModelID, VECTOR vecPos, float fAngle)
{
	CActor* pActor;
	unsigned short ActorID;

	for (ActorID = 0; ActorID < MAX_ACTORS; ActorID++)
	{
		if (!m_bSlotState[ActorID])
		{
			break;
		}
	}

	if (ActorID == MAX_ACTORS) return INVALID_ACTOR_ID;

	pActor = new CActor(ActorID, iModelID, vecPos, fAngle);
	if (pActor)
	{
		m_pActors[ActorID] = pActor;
		m_bSlotState[ActorID] = true;
		m_iVirtualWorld[ActorID] = 0;

		UpdateLastActorID();

		return ActorID;
	}
	return INVALID_ACTOR_ID;
}

void CActorPool::SetActorVirtualWorld(unsigned short ActorID, int iVirtualWorld)
{
	if (ActorID < MAX_ACTORS && m_bSlotState[ActorID])
	{
		m_iVirtualWorld[ActorID] = iVirtualWorld;
	}
}

bool CActorPool::Destroy(int iActorID)
{
	if ((iActorID >= 0 && iActorID < MAX_ACTORS) && m_bSlotState[iActorID])
	{
		SAFE_DELETE(m_pActors[iActorID]);
		m_bSlotState[iActorID] = false;
		UpdateLastActorID();
		return true;
	}
	return false;
}

