// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DemoLoadingScreen.h"

//#include "Engine.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"
#include "SThrobber.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

struct FDemoLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FDemoLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(nullptr, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		FSlateBrush::AddReferencedObjects(Collector);
	}
};

class SDemoLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDemoLoadingScreen) {}
	SLATE_END_ARGS()


		void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(TEXT("/Game/UI/MainMenu/Demo_LoadingScreen.Demo_LoadingScreen"));

		LoadingScreenBrush = MakeShareable(new FDemoLoadingScreenBrush(LoadingScreenName, FVector2D(1920, 1080)));

		ChildSlot
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			.Padding(FMargin(10.0f))
			[
				SNew(SThrobber)
				.Visibility(this, &SDemoLoadingScreen::GetLoadIndicatorVisibility)
			]
			]
			];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis = GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}

	/** loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FDemoLoadingScreenModule : public IDemoLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		//force load for cooker reference
		LoadObject<UObject>(NULL, TEXT("/Game/UI/MainMenu/Demo_LoadingScreen.Demo_LoadingScreen"));

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen() override
	{
		CreateScreen();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.f;
		LoadingScreen.WidgetLoadingScreen = SNew(SDemoLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

};

IMPLEMENT_GAME_MODULE(FDemoLoadingScreenModule, DemoLoadingScreen);
