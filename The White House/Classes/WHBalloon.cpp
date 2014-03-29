//
//  WHBalloon.cpp
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHBalloon.h"
#include "RBPhysicsWorld.h"

namespace WH
{
	RNDefineMeta(Balloon, RN::Entity)
	
	Balloon::Balloon()
	{
		SetModel(RN::Model::WithFile("Models/balloon/balloon_01.sgm")->Copy());
		
		RN::RandomNumberGenerator *rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
		_splatterColor = RN::Color(rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f));
		rng->Release();
		GetModel()->GetMaterialAtIndex(0, 0)->SetDiffuseColor(_splatterColor);
		
		SetShape(RN::bullet::SphereShape::WithRadius(0.4f));
	}
	
	Balloon::Balloon(RN::Deserializer *deserializer) :
		RN::Entity(deserializer)
	{
		SetModel(RN::Model::WithFile("Models/balloon/balloon_01.sgm")->Copy());
		RN::RandomNumberGenerator *rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
		_splatterColor = RN::Color(rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f));
		rng->Release();
		GetModel()->GetMaterialAtIndex(0, 0)->SetDiffuseColor(_splatterColor);
		
		SetShape(RN::bullet::SphereShape::WithRadius(0.4f));
	}
	
	void Balloon::SetShape(RN::bullet::Shape *shape)
	{
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
		AddAttachment(body->Autorelease());
	}
	
	void Balloon::Splatter()
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
		
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(500), [from, color, emitter]{
			emitter->Release();
		}, false);
		
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(300), [from, color, emitter]{
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
