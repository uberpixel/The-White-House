//
//  WHSplatter.cpp
//  The White House
//
//  Created by Nils Daumann on 29.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHSplatter.h"
#include "RBPhysicsWorld.h"

namespace WH
{
	Splatter::Splatter(const RN::Vector3 &position, const RN::Color &color) :
		_color(color), _position(position)
	{
		
	}
	
	void Splatter::Activate(size_t fuse)
	{
		RN::GenericParticleEmitter *emitter = new RN::GenericParticleEmitter();
		emitter->GetMaterial()->AddTexture(RN::Texture::WithFile("Textures/spatter/3.png"));
		emitter->SetWorldPosition(_position);
		emitter->GetMaterial()->SetDepthTest(true);
		emitter->SetStartColor(_color);
		emitter->SetEndColor(_color);
		emitter->SetLifeSpan(RN::Vector2(0.3f, 0.8f));
		emitter->SetStartSize(RN::Vector2(0.1f));
		emitter->SetEndSize(RN::Vector2(2.0f));
		emitter->SetVelocity(RN::Vector3());
		emitter->SetVelocityRandomizeMin(RN::Vector3(-20.0f));
		emitter->SetVelocityRandomizeMax(RN::Vector3(20.0f));
		emitter->SetGravity(RN::Vector3(0.0f, -9.81f, 0.0f));
		emitter->SetFlags(emitter->GetFlags() | RN::SceneNode::Flags::DrawLate);
		emitter->Release();
		
		RN::Vector3 from = _position;
		RN::Color color = _color;
		
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(fuse), [from, color, emitter]{
			emitter->Release();
			
			RN::RandomNumberGenerator *rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
			
			for(int i = 0; i < 20; i++)
			{
				RN::Vector3 dir = -rng->RandomVector3Range(RN::Vector3(-1.0f), RN::Vector3(1.0f));
				dir.Normalize();
				RN::Vector3 to = from + dir * 100.0f;
				RN::Hit hit = RN::bullet::PhysicsWorld::GetSharedInstance()->CastRay(from, to);
				if(hit.distance > 0)
				{
					std::stringstream stream;
					stream << rng->RandomInt32Range(1, 7);
					std::string filename("Textures/spatter/");
					filename += stream.str();
					filename += ".png";
					
					RN::Decal *decal = new RN::Decal(RN::Texture::WithFile(filename));
					decal->SetPosition(hit.position);
					decal->SetScale(RN::Vector3(rng->RandomFloatRange(1.0f, 3.0f)));
					decal->SetRotation(RN::Quaternion::WithLookAt(-dir));
					decal->GetMaterial()->SetDiffuseColor(color);
					decal->Release();
				}
			}
			
			rng->Release();
		}, false);
	}
}