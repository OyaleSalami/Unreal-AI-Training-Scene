// Fill out your copyright notice in the Description page of Project Settings.

#include "MapManager.h"

// Sets default values for this component's properties
UMapManager::UMapManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMapManager::BeginPlay()
{
	Super::BeginPlay();

	if (FParse::Value(FCommandLine::Get(), TEXT("OutputDir="), OutputDir))
	{
		// Use the directory path here
		printf("Output directory set to: %s\n", TCHAR_TO_ANSI(*OutputDir));
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("CamHeight="), CamHeight))
	{
		// Set your camera height
	}
}

// Called every frame
void UMapManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

