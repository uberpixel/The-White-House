//
//  WHPlayer.cpp
//  The White House
//
//  Created by Sidney Just on 28/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHPlayer.h"

namespace WH
{
	RNDefineMeta(Player, RN::Entity)
	
	Player::Player()
	{
		_controller = new RN::bullet::KinematicController(RN::bullet::CapsuleShape::WithRadius(0.5f, 1.8f), 0.7f);
		AddAttachment(_controller);
	}
	
	Player::~Player()
	{
		_controller->Release();
	}
	
	
	void Player::Update(float delta)
	{
		RN::Entity::Update(delta);
		RN::Input *input = RN::Input::GetSharedInstance();
		
		
		
		RN::Vector3 direction(input->IsKeyPressed('d')-input->IsKeyPressed('a'), 0.0f, input->IsKeyPressed('s')-input->IsKeyPressed('w'));
	
		direction = GetWorldRotation().GetRotatedVector(direction);
		direction *= 5.0f;
		
		_controller->SetWalkDirection(direction);
	}
}
