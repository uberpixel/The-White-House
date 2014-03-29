//
//  WHApplication.cpp
//  The White House
//
//  Created by Nils Daumann on 28.03.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHApplication.h"

namespace WH
{
	Application::Application()
	{}
	Application::~Application()
	{}
	
	
	void Application::Start()
	{
		SetTitle("The White House");

		RN::World *world = new World();
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
		
		_crosshair = new RN::UI::ImageView(RN::UI::Image::WithFile("Textures/shottyiron.png"));
		_crosshair->SetScaleMode(RN::UI::ScaleMode::ProportionallyDown);
		
		_crosshairWidget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless);
		_crosshairWidget->GetContentView()->AddSubview(_crosshair);
		
		_crosshairWidget->Open();
		
		UpdateCrosshair();
	}
	
	void Application::WillExit()
	{}
	
	
	void Application::UpdateCrosshair()
	{
		RN::UI::Server *server = RN::UI::Server::GetSharedInstance();
		
		_crosshairWidget->SetFrame(RN::Rect(0.0f, 0.0f, server->GetWidth(), server->GetHeight()));
		_crosshair->SetFrame(RN::Rect(0.0f, 0.0f, server->GetWidth(), server->GetHeight()));
	}
}
