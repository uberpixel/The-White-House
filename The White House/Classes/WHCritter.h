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

#define kWHCritterTag 0xC8177E8

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
		
		void Update(float delta) override;
		
		void Splatter();
		void SetTarget(const RN::Vector3 &target);
		void SetFixed(bool fixed);
		
		static bool CanSpawnCritter();
		
	private:
		void SetType(Type type);
		void SetModel(RN::Model *model);
		void SetShape(RN::bullet::Shape *shape);
		
		RN::Color _splatterColor;
		RN::Vector3 _target;
		bool _isFixedAndCantHaveChildren;
		
		RNDeclareMeta(Critter)
	};
}

#endif /* defined(__The_White_House__WHCritter__) */
