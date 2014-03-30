//
//  WHWorld.h
//  The White House
//
//  Created by Nils Daumann on 28.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __WH_WORLD_H__
#define __WH_WORLD_H__

#include <Rayne/Rayne.h>
#include "RBPhysicsWorld.h"
#include "WHStaticEntity.h"
#include "WHPlayer.h"
#include "WHCritter.h"
#include "WHBalloon.h"
#include "WHDoor.h"
#include "WHNavigation.h"
#include "WHDecoy.h"

#define kWHWorldSpawningStoppedMessage RNCSTR("kWHWorldSpawningStoppedMessage")

namespace WH
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void Update(float delta) override;
		void TrackDecoy(Decoy *decoy);
		
		void HandleInputEvent(RN::Event *event);
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		
		void SetLevel(uint32 level);
		uint32 GetLevel() { return _level; }
		
	private:
		void LoadLevel1();
		void LoadLevel2();
		void LoadLevel3();
		void LoadLevel4();
		
		void SetSpawning(bool enabled);
		
		RN::Camera *_camera;
		RN::bullet::PhysicsWorld *_physicsWorld;
		
		Decoy *_decoy;
		RN::Timer *_decoyTimer;
		
		uint32 _level;
		
		Player *_player;
	};
}

#endif /* __WH_WORLD_H__ */
