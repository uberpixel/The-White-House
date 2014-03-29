//
//  WHDoor.cpp
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHDoor.h"
#include "WHPlayer.h"
#include "RBRigidBody.h"

namespace WH
{
	RNDefineMeta(Door, RN::Entity)
	
	Door::Door(RN::Vector3 position, RN::Quaternion rotation, RN::Vector3 scale) :
		_active(false)
	{
		SetWorldPosition(position);
		SetWorldRotation(rotation);
		SetWorldScale(scale);
		
		SetModel(RN::Model::WithFile("Models/door/door_01.sgm"));
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(new RN::bullet::BoxShape(RN::Vector3(0.1f, 2.0f, 2.0f)), 0.0f);
		AddAttachment(body->Autorelease());
	}
	
	void Door::Update(float delta)
	{
		if(!_active)
		{
			Player *player = RN::World::GetActiveWorld()->GetSceneNodeWithTag<Player>(kWHPlayerTag);
			if(player->GetWorldPosition().GetDistance(GetWorldPosition()) < 1.0f)
			{
				_active = true;
			}
		}
		else
		{
			Rotate(RN::Vector3(delta, 0.0f, 0.0f));
		}
	}
}