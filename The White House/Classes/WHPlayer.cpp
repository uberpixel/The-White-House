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
#include "WHDefinition.h"
#include "WHSplatter.h"
#include "WHDecoy.h"
#include "WHWorld.h"
#include "RBPhysicsWorld.h"

namespace WH
{
	RNDefineMeta(Player, RN::Entity)
	RNDefineSingleton(Player)
	
	Player::Player(RN::Camera *camera) :
		_camera(camera),
		_mouseDown(false),
		_diedBrutally(false),
		_noseBroken(false),
		_noseTouch(false),
		_virginObstschale(true),
		_attackCooldown(0.0f),
		_stepCooldown(0.0f),
		_pushBack(0.0f),
		_dyingHeight(-0.75f)
	{
		MakeShared();
		SetTag(kWHPlayerTag);
		
		_controller = new RN::bullet::KinematicController(RN::bullet::CapsuleShape::WithRadius(0.5f, 1.8f), 0.7f);
		_controller->SetJumpSpeed(3.4f);
		
		AddAttachment(_controller);
		AddChild(_camera);
		
		_camera->SetPosition(RN::Vector3(0.0f, 1.6f, 0.0f));
		
		_shotgun = new RN::Entity(RN::Model::WithFile("Models/shotgun/shotgun.sgm"));
		AddChild(_shotgun);
		_shotgun->SetPosition(RN::Vector3(0.3f, 1.4f, 0.2f));
		
		_muzzle1 = new RN::Entity(RN::Model::WithFile("Models/muzzleflash/muzzleflash.sgm"));
		_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(false);
		_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetDepthWrite(false);
		_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetBlending(true);
		_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetBlendMode(RN::Material::BlendMode::SourceAlpha, RN::Material::BlendMode::OneMinusSourceAlpha);
		_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetDiffuseColor(RN::Color(2.0f, 2.0f, 2.0f, 0.5f));
		_shotgun->AddChild(_muzzle1);
		_muzzle1->SetPosition(RN::Vector3(0.05f, 0.0f, -1.2f));
		_muzzle1->SetScale(RN::Vector3(0.4f));
		
		_muzzle2 = new RN::Entity(RN::Model::WithFile("Models/muzzleflash/muzzleflash.sgm"));
		_shotgun->AddChild(_muzzle2);
		_muzzle2->SetPosition(RN::Vector3(-0.075f, 0.0f, -1.2f));
		_muzzle2->SetScale(RN::Vector3(0.4f));
		
		_muzzle1->SetFlags(_muzzle1->GetFlags() | RN::SceneNode::Flags::Hidden | RN::SceneNode::Flags::DrawLate);
		_muzzle2->SetFlags(_muzzle2->GetFlags() | RN::SceneNode::Flags::Hidden | RN::SceneNode::Flags::DrawLate);
	}
	
	Player::~Player()
	{
		ResignShared();
		_controller->Release();
	}
	
	
	void Player::Attack()
	{
		RN::MessageCenter::GetSharedInstance()->PostMessage(kOAPlaySoundMessage, RNCSTR("/Sounds/shotty.ogg"), nullptr);
		RN::Vector3 source = _camera->ToWorld(RN::Vector3(0.0f, 0.0f, 1.8f));
		
		for(int i = 0; i < 5; i++)
		{
			RN::Vector3 target = _camera->ToWorld(RN::Vector3(_random.RandomFloatRange(-0.1f, 0.1f), _random.RandomFloatRange(-0.1f, 0.1f), 120.0f));
			RN::Hit hit = RN::bullet::PhysicsWorld::GetSharedInstance()->CastRay(source, target);
			
			if(hit.node)
			{
				if(hit.node->IsKindOfClass(Critter::GetMetaClass()))
				{
					Critter *critter = static_cast<Critter *>(hit.node);
					critter->Splatter();
				}
				
				if(hit.node->IsKindOfClass(Balloon::GetMetaClass()))
				{
					Balloon *balloon = static_cast<Balloon *>(hit.node);
					balloon->Splatter();
				}
			}
		}
		
		_muzzle1->SetFlags(_muzzle1->GetFlags() & ~RN::SceneNode::Flags::Hidden);
		_muzzle2->SetFlags(_muzzle2->GetFlags() & ~RN::SceneNode::Flags::Hidden);
		_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetDiffuseColor(RN::Color(2.0f, 2.0f, 2.0f, 0.5f));
		_pushBack = 1.0f;
		
		_attackCooldown = 1.1f;
	}
	
	void Player::Update(float delta)
	{
		RN::Entity::Update(delta);
		if(_diedBrutally)
			return;
		
		RN::Input *input = RN::Input::GetSharedInstance();
		
		RN::Vector3 direction(input->IsKeyPressed('d')-input->IsKeyPressed('a'), 0.0f, input->IsKeyPressed('s')-input->IsKeyPressed('w'));
		
		RN::Vector3 rotationX(input->GetMouseDelta().x, 0.0f, 0.0f);
		RN::Vector3 rotationY(0.0f, input->GetMouseDelta().y, 0.0f);
		
		rotationY += _camera->GetRotation().GetEulerAngle();
		rotationY.y += _pushBack * 10.0f;
		rotationY.y = std::max(-80.0f, std::min(65.0f, rotationY.y));
		
		Rotate(rotationX);
		_camera->SetRotation(rotationY);
		_shotgun->SetRotation(rotationY);
		
		direction = GetRotation().GetRotatedVector(direction);
		direction *= 0.1f;
		
		_stepCooldown -= delta;
		
		if(direction.GetLength() > RN::k::EpsilonFloat)
		{
			if(_stepCooldown <= 0.0f)
			{
				RN::MessageCenter::GetSharedInstance()->PostMessage(kOAPlaySoundMessage, RNSTR("/Sounds/Schritte%i.ogg", _random.RandomInt32Range(1, 14)), nullptr);
				_stepCooldown = 0.41f;
			}
		}
		
		_controller->SetWalkDirection(direction - GetForward() * _pushBack * 0.3f);
		_pushBack = false;
		
		_attackCooldown = std::max(0.0f, _attackCooldown - delta);
		
		if(_attackCooldown < 0.9f)
		{
			_muzzle1->SetFlags(_muzzle1->GetFlags() | RN::SceneNode::Flags::Hidden);
			_muzzle2->SetFlags(_muzzle2->GetFlags() | RN::SceneNode::Flags::Hidden);
		}
		else
		{
			_muzzle1->GetModel()->GetMaterialAtIndex(0, 0)->SetDiffuseColor(RN::Color(2.0f, 2.0f, 2.0f, (_attackCooldown - 0.9f) * 2.5));
		}
		
		if(input->IsKeyPressed(' ') && _controller->IsOnGround())
		{
			RN::MessageCenter::GetSharedInstance()->PostMessage(kOAPlaySoundMessage, RNCSTR("/Sounds/Jump.ogg"), nullptr);
			_controller->Jump();
		}
		
		if(input->IsMousePressed(0) || input->IsMousePressed(1))
		{
			if(input->IsMousePressed(0))
			{
				if(_attackCooldown < RN::k::EpsilonFloat && !_diedBrutally && !_mouseDown)
				{
					Attack();
					_mouseDown = true;
				}
			}
			
			if(input->IsMousePressed(1))
			{
				if(!_diedBrutally && !_mouseDown)
				{
					float chance = _random.RandomFloat();
					if(chance >= 0.9 || _virginObstschale)
					{
						RN::MessageCenter::GetSharedInstance()->PostMessage(kOAPlaySoundMessage, RNCSTR("/Sounds/Obstschale.ogg"), nullptr);
						_virginObstschale = false;
					}
					
					Decoy *decoy = new Decoy(_camera->GetWorldPosition(), _camera->GetWorldRotation());
					
					static_cast<World *>(GetWorld())->TrackDecoy(decoy);
					
					decoy->Release();
					
					_mouseDown = true;
				}
			}
		}
		else
		{
			_mouseDown = false;
		}
		
		if(!_diedBrutally)
		{
			RN::Vector3 source = _camera->ToWorld(RN::Vector3(0.0f, -0.3f, 0.8f));
			RN::Vector3 target = _camera->ToWorld(RN::Vector3(0.0f, -0.3f, 0.9f));
			
			RN::Hit hit = RN::bullet::PhysicsWorld::GetSharedInstance()->CastRay(source, target);
			
			if(hit.node)
			{
				if(!_noseTouch)
				{
					RN::Decal *decal = new RN::Decal(RN::Texture::WithFile("Textures/spatter/Nose.png"));
					decal->SetPosition(hit.position);
					decal->SetRotation(GetWorldRotation());
					decal->SetScale(RN::Vector3(0.7f));
					decal->GetMaterial()->SetDiffuseColor(RN::Color(0.629f, 0.007f, 0.049f));
					decal->Release();
					
					if(_noseBroken)
						decal->SetScale(RN::Vector3(0.3f));
					else
						RN::MessageCenter::GetSharedInstance()->PostMessage(kOAPlaySoundMessage, RNCSTR("/Sounds/Nase.ogg"), nullptr);
					
					_noseBroken = true;
					_noseTouch  = true;
				}
			}
			else
				_noseTouch = false;
		}
		
		RN::Vector3 position = GetWorldPosition();
		if(position.y <= _dyingHeight && !_diedBrutally)
		{
			_diedBrutally = true;
			
			
			RN::MessageCenter::GetSharedInstance()->PostMessage(kOAPlaySoundMessage, RNSTR("/Sounds/BrutalDeath%i.ogg", _random.RandomInt32Range(1, 4)), nullptr);
			
			Splatter splatter(position, RN::Color(0.629f, 0.007f, 0.049f));
			splatter.Activate(130);
			
			RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(2500), []() {
				RN::World::GetActiveWorld()->DropSceneNodes();
				RN::World::GetActiveWorld()->LoadOnThread(RN::Thread::GetCurrentThread(), nullptr);
			}, false);
		}
	}
	
	void Player::setDyingHeight (float dyingHeight)
	{
		 _dyingHeight = dyingHeight;
	}
}
