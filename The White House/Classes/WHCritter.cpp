//
//  WHCritter.cpp
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHCritter.h"

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
		RNDebug("Splatter");
	}
}
