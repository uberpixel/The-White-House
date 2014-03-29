//
//  WHPlayer.cpp
//  The White House
//
//  Created by Sidney Just on 28/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHPlayer.h"
#include "WHCritter.h"
#include "WHBalloon.h"
#include "RBPhysicsWorld.h"

namespace WH
{
	RNDefineMeta(Player, RN::Entity)
	
	Player::Player(RN::Camera *camera) :
		_camera(camera),
		_mouseDown(false)
	{
		_controller = new RN::bullet::KinematicController(RN::bullet::CapsuleShape::WithRadius(0.5f, 1.8f), 0.7f);
		_controller->SetJumpSpeed(3.4f);
		
		AddAttachment(_controller);
		AddChild(_camera);
		
		_camera->SetPosition(RN::Vector3(0.0f, 1.6f, 0.0f));
	}
	
	Player::~Player()
	{
		_controller->Release();
	}
	
	
	void Player::Attack()
	{
		RN::Vector3 source = _camera->ToWorld(RN::Vector3(0.0f, 0.0f, 1.8f));
		RN::Vector3 target = _camera->ToWorld(RN::Vector3(0.0f, 0.0f, 120.0f));
		
		RN::Hit hit = RN::bullet::PhysicsWorld::GetSharedInstance()->CastRay(source, target);
		hit = RN::bullet::PhysicsWorld::GetSharedInstance()->CastRay(source, target);
		
		if(hit.node && hit.node->IsKindOfClass(Critter::MetaClass()))
		{
			Critter *critter = static_cast<Critter *>(hit.node);
			critter->Splatter();
		}
		
		if(hit.node && hit.node->IsKindOfClass(Balloon::MetaClass()))
		{
			Balloon *balloon = static_cast<Balloon *>(hit.node);
			balloon->Splatter();
		}
	}
	
	void Player::Update(float delta)
	{
		RN::Entity::Update(delta);
		RN::Input *input = RN::Input::GetSharedInstance();
		
		RN::Vector3 direction(input->IsKeyPressed('d')-input->IsKeyPressed('a'), 0.0f, input->IsKeyPressed('s')-input->IsKeyPressed('w'));
		
		RN::Vector3 rotationX(input->GetMouseDelta().x, 0.0f, 0.0f);
		RN::Vector3 rotationY(0.0f, input->GetMouseDelta().y, 0.0f);
		
		rotationY += _camera->GetRotation().GetEulerAngle();
		rotationY.y = std::max(-80.0f, std::min(65.0f, rotationY.y));
		
		_camera->SetRotation(rotationY);
		
		
		Rotate(rotationX);
		
		direction = GetRotation().GetRotatedVector(direction);
		direction *= 0.1f;
		
		_controller->SetWalkDirection(direction);
		
		if(input->IsKeyPressed(' ') && _controller->IsOnGround())
			_controller->Jump();
		
		if(input->IsMousePressed(0))
		{
			if(!_mouseDown)
			{
				Attack();
				_mouseDown = true;
			}
		}
		else
		{
			_mouseDown = false;
		}
	}
}
