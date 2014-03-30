//
//  WHWorld.cpp
//  The White House
//
//  Created by Nils Daumann on 28.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHWorld.h"
#include "WHSpawnPoint.h"

namespace WH
{
	World::World() :
		RN::World("GenericSceneManager"),
		_level(1),
		_decoyTimer(nullptr),
		_decoy(nullptr)
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
		RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
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
					
				case 'p':
				{
					RN::Vector3 position = _player->GetWorldPosition();
					
					RNInfo("{%f, %f, %f}", position.x, position.y, position.z);
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
	
	void World::Update(float delta)
	{
		if(_decoy)
		{
			RN::Vector3 target = _decoy->GetWorldPosition();
			RN::MessageCenter::GetSharedInstance()->PostMessage(RNCSTR("fuckoff"), RN::Value::WithVector3(target), nullptr);
		}
	}
	
	void World::TrackDecoy(Decoy *decoy)
	{
		if(_decoyTimer)
		{
			_decoy->RemoveFromWorld();
			
			_decoyTimer->Invalidate();
			_decoyTimer = nullptr;
			
			SetSpawning(false);
		}
		
		_decoy = decoy;
		if(_decoy)
		{
			SetSpawning(true);
			
			_decoyTimer = RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(8), [this]() {
				
				_decoyTimer = nullptr;
				_decoy->RemoveFromWorld();
				_decoy = nullptr;
				
				SetSpawning(false);
				
			}, false);
		}
	}
	
	void World::SetSpawning(bool enabled)
	{
		RN::Array *spawners = GetSceneNodesWithTag<SpawnPoint>(kWHSpawnPointTag);
		if(!spawners)
			return;
		
		spawners->Enumerate<SpawnPoint>([&](SpawnPoint *point, size_t index, bool &stop) {
			point->SetSpawningEnabled(enabled);
		});
		
		if(!enabled)
			RN::MessageCenter::GetSharedInstance()->PostMessage(kWHWorldSpawningStoppedMessage, nullptr, nullptr);
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		if(_decoyTimer)
		{
			_decoyTimer->Invalidate();
			_decoyTimer = nullptr;
			
			_decoy = nullptr;
		}
		
		RN::World::LoadOnThread(thread, deserializer);
		
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults);
		_camera->SetClearColor(RN::Color::White());
		_camera->SetRenderGroups(_camera->GetRenderGroups() | (1 << 31));
		_camera->SetClipFar(80.0f);
		_camera->Release();
		
		_player = new Player(_camera);
		_player->Release();
		
		NavigationManager::GetSharedInstance()->SetNavMesh(nullptr);
		
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
			case 5:
				LoadLevel5();
				break;
			default:
				break;
		}
		
		RN::Kernel::GetSharedInstance()->DidSleepForSignificantTime();
	}
	
	void World::LoadLevel1()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_01.sgm"));
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
		
		
		if(Critter::CanSpawnCritter())
		{
			Critter *critter = new Critter(Critter::Type::Apple, RN::Vector3(0.0f, 0.0f, -10.5));
			critter->SetFixed(true);
			critter->Release();
		}
	}
	
	void World::LoadLevel2()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_02.sgm"));
		level->Release();
		
		Door *door = new Door(RN::Vector3(5.41865f, 1.0f, -28.85), RN::Vector3(0.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-9.20f, 7.36f, -7.13f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		
		SpawnPoint *point;
		
		point = new SpawnPoint(RN::Vector3(-17.027275, 1.205256, -9.622817));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(4.309178, 0.635908, -0.264022));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(0.960735, 0.618220, -19.294271));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-11.842712, 1.181179, -18.227867));
		point->Release();
	}
	
	void World::LoadLevel3()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_03.sgm"));
		level->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(true);
		level->Release();
		
		Door *door = new Door(RN::Vector3(7.92063f, 0.32514f, 40.52977f), RN::Vector3(270.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.24f, 12.31f, 34.69f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(31.97f, 12.31f, 18.88f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(8.52f, 12.30f, 7.22f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		
		SpawnPoint *point;
		
		point = new SpawnPoint(RN::Vector3(15.765354, 0.006236, 5.233837));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(25.208176, 0.006232, 20.674175));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(14.571447, 0.006226, 23.914522));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(4.715867, 0.006213, 30.811022));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-4.085171, 0.006244, 38.585899));
		point->Release();
	}
	
	void World::LoadLevel4()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_04.sgm"));
		level->GetModel()->GetMaterialAtIndex(0, 0)->SetLighting(true);
		level->Release();
		
		Door *door = new Door(RN::Vector3(-20.67f, 0.45f, 21.26f), RN::Vector3(180.0f, 0.0f, 0.0f), RN::Vector3(0.75f));
		door->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-37.7f, 3.1f, 34.7f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-21.7f, 3.1f, 34.7f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-1.1f, 3.1f, 40.5f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-1.1f, 3.1f, 24.5f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-20.3f, 3.1f, 20.3f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-37.7f, 3.1f, 9.3f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(-9.6, 3.1f, 6.5f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f, 3.1f, 0.0f));
		light->SetIntensity(1.0f);
		light->SetRange(25.0f);
		light->Release();
		
		SpawnPoint *point;
		
		point = new SpawnPoint(RN::Vector3(-12.758311, 0.047873, -0.950561));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-18.231022, 0.047873, 0.374814));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(4.241591, 0.006198, 1.289377));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-37.768707, 0.047848, 7.032582));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-27.825808, 0.047873, 24.195520));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-13.372080, 0.047873, 37.381832));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(1.284830, 0.047874, 21.855516));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-18.686232, 0.047873, 5.904874));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-22.239698, 0.047862, 44.610992));
		point->Release();
		
		point = new SpawnPoint(RN::Vector3(-28.667234, 0.047874, 26.162754));
		point->Release();
	}
	
	void World::LoadLevel5()
	{
		StaticEntity *level = new StaticEntity(RN::Model::WithFile("Models/levels/level_05.sgm"));
		level->GetModel()->GetMaterialAtIndex(0,0)->SetLighting(true);
		level->Release();
		
		RN::Light *light = new RN::Light(RN::Light::Type::PointLight);
		light->SetPosition(RN::Vector3(0.0f));
		
		_player->setDyingHeight(-10.0f);
	}
}
