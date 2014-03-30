//
//  WHCritter.h
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHCritter__
#define __The_White_House__WHCritter__

#include <Rayne/Rayne.h>
#include "RBRigidBody.h"

namespace WH
{
	class Critter : public RN::Entity
	{
	public:
		enum class Type
		{
			Apple,
			Banana,
			Cucumber,
			
			__CritterTypeMax
		};
		
		Critter(Type type, const RN::Vector3 &position);
		~Critter();
		
		void Splatter();
		
		static bool CanSpawnCritter();
		
	private:
		void SetType(Type type);
		void SetModel(RN::Model *model);
		void SetShape(RN::bullet::Shape *shape);
		
		RN::Color _splatterColor;
		
		RNDeclareMeta(Critter)
	};
}

#endif /* defined(__The_White_House__WHCritter__) */
