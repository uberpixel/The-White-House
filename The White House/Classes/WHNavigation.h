//
//  WHNavigation.h
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHNavigation__
#define __The_White_House__WHNavigation__

#include <Rayne/Rayne.h>

#include "DetourAlloc.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"

#define kWHCrowdMaxAgents 30

namespace WH
{
	class NavigationAgent : public RN::SceneNodeAttachment
	{
	public:
		friend class NavigationManager;
		
		NavigationAgent();
		~NavigationAgent();
		
		void Enable();
		void Disable();
		
		void SetStandStill(bool standStill);
		void GetData(RN::Vector3 &position, RN::Vector3 &target);
		
		void Update(float delta) override;
		
	private:
		bool _standStill;
		int _index;
	};
	
	class NavigationManager : public RN::ISingleton<NavigationManager>
	{
	public:
		friend class NavigationAgent;
		
		NavigationManager();
		~NavigationManager();
		
		void Lock();
		void Unlock();
		
		void Update(float delta);
		void Clear();
		
		void SetNavMesh(RN::Mesh *mesh);
		void SetTarget(const RN::Vector3 &target);
		
		void AddAgent(NavigationAgent *agent);
		void RemoveAgent(NavigationAgent *agent);
		
	protected:
		float cellSize;
		float cellHeight;
		
		float agentHeight;
		float agentRadius;
		float agentMaxClimb;
		float agentMaxSlope;
		
		float regionMinSize;
		float regionMergeSize;
		
		float edgeMaxLength;
		float edgeMaxError;
		
		float detailSampleDist;
		float detailSampleMaxError;
		
	private:
		RN::Mutex _lock;
		RN::Vector3 _target;
		
		dtNavMesh *_navMesh;
		dtNavMeshQuery *_query;
		dtCrowd *_crowd;
		
		std::vector<NavigationAgent *> _agents;
		
		RNDeclareSingleton(NavigationManager)
	};
}

#endif /* defined(__The_White_House__WHNavigation__) */
