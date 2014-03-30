//
//  WHDecoy.h
//  The White House
//
//  Created by Sidney Just on 30/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHDecoy__
#define __The_White_House__WHDecoy__

#include <Rayne/Rayne.h>

namespace WH
{
	class Decoy : public RN::Entity
	{
	public:
		Decoy(const RN::Vector3 &position, const RN::Quaternion &rotation);
		~Decoy();
		
	private:
	};
}

#endif /* defined(__The_White_House__WHDecoy__) */
