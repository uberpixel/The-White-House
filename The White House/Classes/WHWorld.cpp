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
		RN::World("GenericSceneManager"),
		_level(1)
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
	
	void World::SetLevel(uint32 level)
	{
		_level = level;
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		RN::World::LoadOnThread(thread, deserializer);
		
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults);
		_player = new Player(_camera);
		_player->Release();
		
		_camera->SetClearColor(RN::Color::White());
		
		switch(_level)
		{
			case 1:
				LoadLevel1();
				break;
				
			case 2:
				LoadLevel2();
				break;
				
			case 3:
				LoadLevel3();
				break;
				
			case 4:
				LoadLevel4();
				break;
				
			default:
				break;
		}
	}
	
	void World::LoadLevel1()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_01.sgm"));
		level->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(false);
		level->Release();
		
		Door *door = new Door(RN::Vector3(-0.95f, 1.0f, -14.5), RN::Vector3(90.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f, 3.0f, -5.0f));
		light->SetIntensity(1.0f);
		light->SetRange(20.0f);
		light->Release();
		
		RN::Entity *leuchter = new RN::Entity(RN::Model::WithFile("Models/kronleuchter/kronleuchter_01.sgm"));
		leuchter->SetPosition(RN::Vector3(0.0f, 3.0f, -5.0f));
		leuchter->Release();
		
		Balloon *balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(0.58193, -0.98537, 0.60694));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(0.2197, -0.98537, 0.81709));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(-0.2006, -0.98537, 0.81709));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(-0.57112, -0.98537, 0.60141));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(-0.80771, -0.98537, 0.24377));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(-0.7868, -0.98537, -0.18941));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(-0.56559, -0.98537, -0.54334));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(-0.20613, -0.98537, -0.77008));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(0.22246, -0.98537, -0.76455));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(0.59299, -0.98537, -0.55993));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(0.7976, -0.98537, -0.18664));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		balloon = new Balloon();
		balloon->SetPosition(RN::Vector3(0.79484, -0.98537, 0.23918));
		balloon->Translate(leuchter->GetWorldPosition());
		balloon->Release();
		
		
		Critter *critter = new Critter(Critter::Type::Apple, RN::Vector3(0.0f));
		critter->Release();
	}
	
	void World::LoadLevel2()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_02.sgm"));
		level->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(true);
		level->Release();
		
		Door *door = new Door(RN::Vector3(5.41865f, 1.0f, -28.85), RN::Vector3(0.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f, 3.0f, -5.0f));
		light->SetIntensity(1.0f);
		light->SetRange(20.0f);
		light->Release();
		
		
		Critter *critter = new Critter(Critter::Type::Apple, RN::Vector3(0.0f));
		critter->Release();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [&](RN::Message *message) {
			
			RN::Event *event = static_cast<RN::Event *>(message);
			HandleInputEvent(event);
		}, this);
	}
	
	void World::LoadLevel3()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_03.sgm"));
		level->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(true);
		level->Release();
		
		Door *door = new Door(RN::Vector3(7.01063f, 0.32514f, 40.52977f), RN::Vector3(0.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f, 3.0f, -5.0f));
		light->SetIntensity(1.0f);
		light->SetRange(20.0f);
		light->Release();
		
		
		Critter *critter = new Critter(Critter::Type::Apple, RN::Vector3(0.0f));
		critter->Release();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [&](RN::Message *message) {
			
			RN::Event *event = static_cast<RN::Event *>(message);
			HandleInputEvent(event);
		}, this);
	}
	
	void World::LoadLevel4()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_04.sgm"));
		level->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(true);
		level->Release();
		
		Door *door = new Door(RN::Vector3(-20.5f, 0.5f, -20.0f), RN::Vector3(0.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f, 3.0f, -5.0f));
		light->SetIntensity(1.0f);
		light->SetRange(20.0f);
		light->Release();
		
		
		Critter *critter = new Critter(Critter::Type::Apple, RN::Vector3(0.0f));
		critter->Release();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [&](RN::Message *message) {
			
			RN::Event *event = static_cast<RN::Event *>(message);
			HandleInputEvent(event);
		}, this);
	}

	void World::Update(float delta)
	{
		// Do something...
	}
}
