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

namespace WH
{
	class Player : public RN::Entity
	{
	public:
		Player();
		~Player() override;
		
		void Update(float delta) override;
		
	private:
		
		RN::bullet::KinematicController *_controller;
		
		RNDeclareMeta(Player)
	};
}

#endif /* defined(__The_White_House__WHPlayer__) */
