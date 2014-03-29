//
//  WHBalloon.cpp
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHBalloon.h"
#include "RBPhysicsWorld.h"
#include "WHSplatter.h"

namespace WH
{
	RNDefineMeta(Balloon, RN::Entity)
	
	Balloon::Balloon()
	{
		SetModel(RN::Model::WithFile("Models/balloon/balloon_01.sgm")->Copy());
		
		RN::RandomNumberGenerator *rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
		_splatterColor = RN::Color(rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f));
		rng->Release();
		GetModel()->GetMaterialAtIndex(0, 1)->SetDiffuseColor(_splatterColor);
		
		SetShape(RN::bullet::SphereShape::WithRadius(0.2f));
	}
	
	Balloon::Balloon(RN::Deserializer *deserializer) :
		RN::Entity(deserializer)
	{
		SetModel(RN::Model::WithFile("Models/balloon/balloon_01.sgm")->Copy());
		RN::RandomNumberGenerator *rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
		_splatterColor = RN::Color(rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f), rng->RandomFloatRange(0.0f, 1.0f));
		rng->Release();
		GetModel()->GetMaterialAtIndex(0, 1)->SetDiffuseColor(_splatterColor);
		
		SetShape(RN::bullet::SphereShape::WithRadius(0.2f));
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
		
		class Splatter splatter(GetWorldPosition(), _splatterColor);
		splatter.Activate();
		
		Release();
	}
}
