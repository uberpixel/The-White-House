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
	
	Player::Player(RN::Camera *camera) :
		_camera(camera)
	{
		_controller = new RN::bullet::KinematicController(RN::bullet::CapsuleShape::WithRadius(0.5f, 1.8f), 0.7f);
		
		AddAttachment(_controller);
		AddChild(_camera);
		
		_camera->SetPosition(RN::Vector3(0.0f, 1.6f, 0.0f));
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
		
		RN::Vector3 rotationX(input->GetMouseDelta().x, 0.0f, 0.0f);
		RN::Vector3 rotationY(0.0f, input->GetMouseDelta().y, 0.0f);
		
		_camera->Rotate(rotationY);
		Rotate(rotationX);
		
		direction = GetRotation().GetRotatedVector(direction);
		direction *= 0.2f;
		
		_controller->SetWalkDirection(direction);
	}
}
