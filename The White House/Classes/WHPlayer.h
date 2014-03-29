//
//  WHPlayer.h
//  The White House
//
//  Created by Sidney Just on 28/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHPlayer__
#define __The_White_House__WHPlayer__

#include <Rayne/Rayne.h>
#include "RBKinematicController.h"

#define kWHPlayerTag 1337

namespace WH
{
	class Player : public RN::Entity
	{
	public:
		Player(RN::Camera *camera);
		~Player() override;
		
		void Update(float delta) override;
		
	private:
		void Attack();
		
		RN::Camera *_camera;
		RN::Entity *_shotgun;
		RN::bullet::KinematicController *_controller;
		
		bool _diedBrutally;
		bool _mouseDown;
		bool _noseBroken;
		bool _noseTouch;
		
		float _stepCooldown;
		float _attackCooldown;
		float _pushBack;
		
		RN::Random::MersenneTwister _random;
		
		RNDeclareMeta(Player)
	};
}

#endif /* defined(__The_White_House__WHPlayer__) */
