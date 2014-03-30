//
//  WHCritter.cpp
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHCritter.h"
#include "RBPhysicsWorld.h"
#include "WHSplatter.h"
#include "WHNavigation.h"
#include "WHWorld.h"

namespace WH
{
	RNDefineMeta(Critter, RN::Entity)
	
	static RN::SpinLock _pestLock;
	static size_t _pestControl(0);
	
	bool Critter::CanSpawnCritter()
	{
		_pestLock.Lock();
		
		bool allowed = (_pestControl < 25);
		if(allowed)
			_pestControl ++;
		
		_pestLock.Unlock();
		
		return allowed;
	}
	
	
	
	Critter::Critter(Type type, const RN::Vector3 &position) :
		_isFixedAndCantHaveChildren(false)
	{
		SetPosition(position);
		SetRenderGroup(31);
		SetTag(kWHCritterTag);
		
		SetType(type);
		
		NavigationAgent *agent = new NavigationAgent();
		AddAttachment(agent);
		
		agent->Enable();
		agent->Release();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kWHWorldSpawningStoppedMessage, [this](RN::Message *message) {
			RemoveFromWorld();
		}, this);
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(RNCSTR("fuckoff"), [this](RN::Message *message) {
			
			RN::Value *value = static_cast<RN::Value *>(message->GetObject());
			SetTarget(value->GetValue<RN::Vector3>());
			
		}, this);
	}
	
	Critter::~Critter()
	{
		_pestLock.Lock();
		_pestControl --;
		_pestLock.Unlock();
		
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this, kWHWorldSpawningStoppedMessage);
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this, RNCSTR("fuckoff"));
	}
	
	
	void Critter::SetType(Type type)
	{
		switch(type)
		{
			case Type::Apple:
			{
				_splatterColor = RN::Color(0.849f, 0.009f, 0.066f);
				
				SetModel(RN::Model::WithFile("Models/critter/apple_01.sgm"));
				SetShape(RN::bullet::SphereShape::WithRadius(0.4f));
				
				SetScale(RN::Vector3(0.4f));
				break;
			}
				
			case Type::Banana:
			{
				_splatterColor = RN::Color(1.000f, 0.851f, 0.100f);
				
				SetModel(RN::Model::WithFile("Models/critter/banana_01.sgm"));
				SetShape(RN::bullet::SphereShape::WithRadius(0.4f));
				
				SetScale(RN::Vector3(1.2f));
				break;
			}
				
				
			case Type::Cucumber:
			{
				_splatterColor = RN::Color(0.082f, 0.657f, 0.000f);
				
				SetModel(RN::Model::WithFile("Models/critter/cucumber_01.sgm"));
				SetShape(RN::bullet::SphereShape::WithRadius(0.4f));
				
				SetScale(RN::Vector3(0.3));
				break;
			}
				
			default:
				break;
		}
	}
	
	void Critter::SetModel(RN::Model *model)
	{
		RN::Entity::SetModel(model);
		
		size_t count = model->GetMeshCount(0);
		for(size_t i = 0;  i < count; i ++)
		{
			RN::Material *material = model->GetMaterialAtIndex(0, i);
			material->SetCullMode(RN::Material::CullMode::None);
		}
	}
	
	void Critter::SetFixed(bool fixed)
	{
		_isFixedAndCantHaveChildren = fixed;
	}
	
	void Critter::SetShape(RN::bullet::Shape *shape)
	{
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
		AddAttachment(body->Autorelease());
	}
	
	void Critter::SetTarget(const RN::Vector3 &target)
	{
		_target = target;
	}
	
	void Critter::Update(float delta)
	{
		RN::Entity::Update(delta);
		
		if(_isFixedAndCantHaveChildren)
			return;
		
		if(GetWorldPosition().GetDistance(_target) >= 1.5f)
		{
			LookAt(_target);
			TranslateLocal(RN::Vector3(0.0f, 0.0f, -6.0f) * delta);
		}
	}
	
	void Critter::Splatter()
	{
		Retain();
		RemoveFromWorld();
		
		class Splatter splatter(GetWorldPosition(), _splatterColor);
		splatter.Activate();
		
		Release();
	}
}
