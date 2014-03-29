//
//  WHDoor.h
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHDoor__
#define __The_White_House__WHDoor__

namespace WH
{
	class Door : public RN::Entity
	{
	public:
		Door(RN::Vector3 position, RN::Quaternion rotation, RN::Vector3 scale);
		void Update(float delta);
		
	private:
		bool _active;
		
		RNDeclareMeta(Door)
	};
}

#endif /* defined(__The_White_House__WHDoor__) */
