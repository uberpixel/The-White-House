//
//  WHDecoy.cpp
//  The White House
//
//  Created by Sidney Just on 30/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHDecoy.h"
#include "RBRigidBody.h"

namespace WH
{
	Decoy::Decoy(const RN::Vector3 &position, const RN::Quaternion &rotation)
	{
		SetModel(RN::Model::WithFile("Models/foodbowl/obstschale_02.sgm"));
		SetPosition(position + rotation.GetRotatedVector(RN::Vector3(0.0f, 0.0f, -2.0f)));
		SetRenderGroup(31);
		
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(RN::bullet::CylinderShape::WithHalfExtents(RN::Vector3(0.3f, 0.3f, 0.1f)), 1.0f);
		
		body->ApplyImpulse(rotation.GetRotatedVector(RN::Vector3(0.0f, 0.8f, -5.0f)));
		
		AddAttachment(body->Autorelease());
	}
	
	Decoy::~Decoy()
	{
	}
}
