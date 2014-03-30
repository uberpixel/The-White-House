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
		SetTitle("The White(st) House (you know)");
		
		const RN::Array *configs = RN::Window::GetSharedInstance()->GetMainScreen()->GetConfigurations();
		RN::Window::GetSharedInstance()->ActivateConfiguration(configs->GetObjectAtIndex<RN::WindowConfiguration>(configs->GetCount() - 2), 0);

		// Pre-load the splatter textures
		RN::Texture::WithFile("Textures/spatter/1.png")->Retain();
		RN::Texture::WithFile("Textures/spatter/2.png")->Retain();
		RN::Texture::WithFile("Textures/spatter/3.png")->Retain();
		RN::Texture::WithFile("Textures/spatter/4.png")->Retain();
		RN::Texture::WithFile("Textures/spatter/5.png")->Retain();
		RN::Texture::WithFile("Textures/spatter/6.png")->Retain();
		
		RN::World *world = new World();
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
		
		_crosshair = new RN::UI::ImageView(RN::UI::Image::WithFile("Textures/shottyiron.png"));
		_crosshair->SetScaleMode(RN::UI::ScaleMode::ProportionallyDown);
		
		_crosshairWidget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless);
		_crosshairWidget->GetContentView()->AddSubview(_crosshair);
		_crosshairWidget->SetCanBecomeKeyWidget(false);
		_crosshairWidget->SetWidgetLevel(-10000);
		
		_crosshairWidget->Open();
		
		UpdateCrosshair();
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNUIServerDidResizeMessage, std::bind(&Application::UpdateCrosshair, this), nullptr);
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
