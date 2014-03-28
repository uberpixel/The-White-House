//
//  WHStaticEntity.cpp
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHStaticEntity.h"

namespace WH
{
	StaticEntity::StaticEntity(RN::Model *model) :
		RN::Entity(model)
	{
		size_t count = model->GetMeshCount(0);
		for(size_t i = 0;  i < count; i ++)
		{
			RN::Material *material = model->GetMaterialAtIndex(0, i);
			material->SetCullMode(RN::Material::CullMode::None);
		}
		
		RN::bullet::Shape *shape = RN::bullet::TriangleMeshShape::WithModel(model);
		
		_body = new RN::bullet::RigidBody(shape, 0.0f);
		
		AddAttachment(_body);
	}
	
	StaticEntity::~StaticEntity()
	{
		_body->Release();
	}
}
