//
//  WHStaticEntity.h
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHStaticEntity__
#define __The_White_House__WHStaticEntity__

#include <Rayne/Rayne.h>
#include "RBRigidBody.h"

namespace WH
{
	class StaticEntity : public RN::Entity
	{
	public:
		StaticEntity(RN::Model *model);
		~StaticEntity() override;
		
	private:
		RN::bullet::RigidBody *_body;
	};
}

#endif /* defined(__The_White_House__WHStaticEntity__) */
