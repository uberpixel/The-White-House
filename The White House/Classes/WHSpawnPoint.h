//
//  WHSpawnPoint.h
//  The White House
//
//  Created by Sidney Just on 30/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __The_White_House__WHSpawnPoint__
#define __The_White_House__WHSpawnPoint__

#include <Rayne/Rayne.h>

#define kWHSpawnPointTag 0xdeadf00d

namespace WH
{
	class SpawnPoint : public RN::SceneNode
	{
	public:
		SpawnPoint(const RN::Vector3 &position);
		
		void SetSpawningEnabled(bool enabled);
		bool IsSpawningEnabled() const { return _spawnCritter; }
		
		void Update(float delta) override;
		
	private:
		bool _spawnCritter;
		float _critterCoolDown;
		
		RN::Random::MersenneTwister _random;
	};
}

#endif /* defined(__The_White_House__WHSpawnPoint__) */
