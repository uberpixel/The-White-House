//
//  WHCritter.cpp
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHCritter.h"
#include "RBPhysicsWorld.h"

namespace WH
{
	RNDefineMeta(Critter, RN::Entity)
	
	Critter::Critter(Type type, const RN::Vector3 &position)
	{
		SetPosition(position);
		SetType(type);
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
				break;
			}
		}
	}
	
	void Critter::SetModel(RN::Model *model)
	{
		RN::Entity::SetModel(model);
		SetScale(RN::Vector3(0.4f));
		
		size_t count = model->GetMeshCount(0);
		for(size_t i = 0;  i < count; i ++)
		{
			RN::Material *material = model->GetMaterialAtIndex(0, i);
			material->SetCullMode(RN::Material::CullMode::None);
		}
	}
	
	void Critter::SetShape(RN::bullet::Shape *shape)
	{
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 1.0f);
		AddAttachment(body->Autorelease());
	}
	
	void Critter::Splatter()
	{
		Retain();
		RemoveFromWorld();
		
		RN::GenericParticleEmitter *emitter = new RN::GenericParticleEmitter();
		emitter->GetMaterial()->AddTexture(RN::Texture::WithFile("Textures/spatter/3.png"));
		emitter->SetWorldPosition(GetWorldPosition());
		emitter->GetMaterial()->SetDepthTest(true);
		emitter->SetStartColor(_splatterColor);
		emitter->SetEndColor(_splatterColor);
		emitter->SetLifeSpan(RN::Vector2(0.3f, 0.8f));
		emitter->SetStartSize(RN::Vector2(0.1f));
		emitter->SetEndSize(RN::Vector2(2.0f));
		emitter->SetVelocity(RN::Vector3());
		emitter->SetVelocityRandomizeMin(RN::Vector3(-20.0f));
		emitter->SetVelocityRandomizeMax(RN::Vector3(20.0f));
		emitter->SetGravity(RN::Vector3(0.0f, -9.81f, 0.0f));
		emitter->SetFlags(emitter->GetFlags() | RN::SceneNode::Flags::DrawLate);
		emitter->Release();
		
		RN::Vector3 from = GetWorldPosition();
		RN::Color color = _splatterColor;
		std::chrono::milliseconds blubb(300);
		RN::Timer::ScheduledTimerWithDuration(blubb, [from, color, emitter]{
			emitter->Release();
			
			RN::RandomNumberGenerator *rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
			
			for(int i = 0; i < 20; i++)
			{
				RN::Vector3 dir = -rng->RandomVector3Range(RN::Vector3(-1.0f), RN::Vector3(1.0f));
				dir.Normalize();
				RN::Vector3 to = from + dir * 100.0f;
				RN::Hit hit = RN::bullet::PhysicsWorld::GetSharedInstance()->CastRay(from, to);
				if(hit.distance > 0)
				{
					RN::Decal *decal = new RN::Decal(RN::Texture::WithFile("Textures/spatter/3.png"));
					decal->SetPosition(hit.position);
					decal->SetScale(RN::Vector3(rng->RandomFloatRange(1.0f, 3.0f)));
					decal->SetRotation(RN::Quaternion::WithLookAt(-dir));
					decal->GetMaterial()->SetDiffuseColor(color);
					decal->Release();
				}
			}
			
			rng->Release();
		}, false);
		
		Release();
	}
}
