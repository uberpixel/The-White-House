//
//  WHBalloon.h
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHBalloon__
#define __The_White_House__WHBalloon__

#include <Rayne/Rayne.h>
#include "RBRigidBody.h"

namespace WH
{
	class Balloon : public RN::Entity
	{
	public:
		Balloon();
		Balloon(RN::Deserializer *deserializer);
		
		void SetShape(RN::bullet::Shape *shape);
		void Splatter();
		
	private:
		RN::Color _splatterColor;
		
		RNDeclareMeta(Balloon);
	};
}

#endif /* defined(__The_White_House__WHBalloon__) */
