//
//  WHApplication.h
//  The White House
//
//  Created by Nils Daumann on 28.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __WH_APPLICATION_H__
#define __WH_APPLICATION_H__

#include <Rayne/Rayne.h>
#include "WHWorld.h"

namespace WH
{
	class Application : public RN::Application
	{
	public:
		Application();
		~Application() override;
		
		void Start() override;
		void WillExit() override;
	};
}

#endif /* __WH_APPLICATION_H__ */
