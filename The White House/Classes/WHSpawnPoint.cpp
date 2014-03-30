//
//  WHSpawnPoint.cpp
//  The White House
//
//  Created by Sidney Just on 30/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHSpawnPoint.h"
#include "WHCritter.h"
#include "WHNavigation.h"
#include "WHPlayer.h"

namespace WH
{
	SpawnPoint::SpawnPoint(const RN::Vector3 &position) :
		_spawnCritter(false),
		_critterCoolDown(0.0f)
	{
		SetPosition(position);
		SetTag(kWHSpawnPointTag);
	}
	
	
	
	void SpawnPoint::SetSpawningEnabled(bool enabled)
	{
		_spawnCritter = enabled;
		_critterCoolDown = 0.0f;
	}
	
	void SpawnPoint::Update(float delta)
	{
		RN::SceneNode::Update(delta);
		
		_critterCoolDown = std::max(0.0f, _critterCoolDown - delta);
		
		if(_spawnCritter && _critterCoolDown <= RN::k::EpsilonFloat)
		{
			if(Player::GetSharedInstance()->GetWorldPosition().GetDistance(GetWorldPosition()) < 15.0 && Critter::CanSpawnCritter())
			{
				Critter::Type type = static_cast<Critter::Type>(_random.RandomInt32Range(0, static_cast<int32>(Critter::Type::__CritterTypeMax)));
				
				Critter *critter = new Critter(type, GetWorldPosition());
				critter->Release();
			}
			
			_critterCoolDown = 0.5f + _random.RandomFloatRange(0.0f, 1.3f);
		}
	}
}
