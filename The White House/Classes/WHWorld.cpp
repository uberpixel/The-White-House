//
//  WHWorld.cpp
//  The White House
//
//  Created by Nils Daumann on 28.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHWorld.h"

namespace WH
{
	World::World() :
		RN::World("GenericSceneManager")
	{
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [&](RN::Message *message) {
			
			RN::Event *event = static_cast<RN::Event *>(message);
			HandleInputEvent(event);
		}, this);
		
		_physicsWorld = new RN::bullet::PhysicsWorld();
		
		AddAttachment(_physicsWorld);
	}
	
	World::~World()
	{
		_physicsWorld->Release();
	}
	
	void World::HandleInputEvent(RN::Event *event)
	{
		if(event->GetType() == RN::Event::Type::KeyDown)
		{
			switch(event->GetCharacter())
			{
				case '0':
				{
					RN::MessageCenter::GetSharedInstance()->PostMessage(RNCSTR("DPToggle"), nullptr, nullptr);
					break;
				}
					
				default:
					break;
			}
		}
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB888, RN::Camera::Flags::Defaults);
		_player = new Player(_camera);
		
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_01.sgm"));
		level->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f, 2.0f, -1.0f));
		light->SetIntensity(1.0f);
		light->Release();
		
		RN::Entity *leuchter = new RN::Entity(RN::Model::WithFile("Models/kronleuchter/kronleuchter_01.sgm"));
		leuchter->SetPosition(RN::Vector3(0.0f, 3.0f, -1.0f));
		leuchter->Release();
	}

	void World::Update(float delta)
	{
		// Do something...
	}
}
