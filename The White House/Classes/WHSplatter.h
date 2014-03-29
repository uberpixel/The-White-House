//
//  WHSplatter.h
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHSplatter__
#define __The_White_House__WHSplatter__

#include <Rayne/Rayne.h>

namespace WH
{
	class Splatter
	{
	public:
		Splatter(const RN::Vector3 &position, const RN::Color &color);
		void Activate();
		
	private:
		RN::Vector3 _position;
		RN::Color _color;
	};
}

#endif /* defined(__The_White_House__WHSplatter__) */
