// Fill out your copyright notice in the Description page of Project Settings.

#include "MapManager.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include <Engine/World.h>
#include <Misc/FileHelper.h>
#include <HAL/IConsoleManager.h>

// Sets default values for this component's properties
UMapManager::UMapManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default values
	CamHeight = 500.0f;
	TimeOfDay = 1200;
	FOV = 90.0f;
	Weather = EWeather::Clear;
	RotationSpeed = 30.0f;
}

// Called when the game starts
void UMapManager::BeginPlay()
{
	Super::BeginPlay();

	// Parse all 13 parameters
	FParse::Value(FCommandLine::Get(), TEXT("OutputDir="), OutputDir);
	FParse::Value(FCommandLine::Get(), TEXT("CamHeight="), CamHeight);
	FParse::Value(FCommandLine::Get(), TEXT("TimeOfDay="), TimeOfDay);
	FParse::Value(FCommandLine::Get(), TEXT("FOV="), FOV);

	int32 WeatherInt = 0;
	if (FParse::Value(FCommandLine::Get(), TEXT("Weather="), WeatherInt))
	{
		Weather = static_cast<EWeather>(WeatherInt);
	}

	FParse::Value(FCommandLine::Get(), TEXT("CameraBlur="), CameraBlur);
	FParse::Value(FCommandLine::Get(), TEXT("AtmosphericHaze="), AtmosphericHaze);
	FParse::Value(FCommandLine::Get(), TEXT("CharacterCount="), CharacterCount);
	FParse::Value(FCommandLine::Get(), TEXT("ArrivalTime="), ArrivalTime);
	FParse::Value(FCommandLine::Get(), TEXT("ArmedCount="), ArmedCount);
	FParse::Value(FCommandLine::Get(), TEXT("RotationSpeed="), RotationSpeed);
	FParse::Value(FCommandLine::Get(), TEXT("VehicleID="), VehicleID);
	FParse::Value(FCommandLine::Get(), TEXT("BuildingID="), BuildingID);
	FParse::Value(FCommandLine::Get(), TEXT("Mode="), Mode);

	ForceResolution();
}

void UMapManager::ForceResolution()
{
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D TargetRes(640, 512);
		// Note: This is a simplified way to set resolution via console command which reliably works in unattended mode
		FString Command = FString::Printf(TEXT("r.setres 640x512w"));
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PC->ConsoleCommand(Command);
		}
	}
}

TArray<FBoundingBoxData> UMapManager::CalculateBoundingBoxes()
{
	TArray<FBoundingBoxData> Results;

	UWorld* World = GetWorld();
	if (!World) return Results;

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC) return Results;

	// Define tags or classes for actors we want to track (Characters and Vehicles)
	// For this implementation, we'll look for Actors with specific tags or shared parent classes
	TArray<AActor*> TrackedActors;
	UGameplayStatics::GetAllActorsWithTag(World, TEXT("TrackedUnit"), TrackedActors);

	for (AActor* Actor : TrackedActors)
	{
		if (!Actor) continue;

		FVector Origin;
		FVector BoxExtent;
		Actor->GetActorBounds(true, Origin, BoxExtent);

		// Calculate 8 corners of the 3D bounding box
		TArray<FVector> Corners;
		for (int32 i = 0; i < 8; ++i)
		{
			FVector Corner = Origin;
			Corner.X += (i & 1) ? BoxExtent.X : -BoxExtent.X;
			Corner.Y += (i & 2) ? BoxExtent.Y : -BoxExtent.Y;
			Corner.Z += (i & 4) ? BoxExtent.Z : -BoxExtent.Z;
			Corners.Add(Corner);
		}

		FVector2D Min2D(FLT_MAX, FLT_MAX);
		FVector2D Max2D(-FLT_MAX, -FLT_MAX);
		bool bAnyOnScreen = false;

		for (const FVector& Corner : Corners)
		{
			FVector2D ScreenPos;
			if (PC->ProjectWorldLocationToScreen(Corner, ScreenPos))
			{
				Min2D.X = FMath::Min(Min2D.X, ScreenPos.X);
				Min2D.Y = FMath::Min(Min2D.Y, ScreenPos.Y);
				Max2D.X = FMath::Max(Max2D.X, ScreenPos.X);
				Max2D.Y = FMath::Max(Max2D.Y, ScreenPos.Y);
				bAnyOnScreen = true;
			}
		}

		if (bAnyOnScreen)
		{
			FBoundingBoxData Data;
			Data.ActorName = Actor->GetName();
			Data.Min = Min2D;
			Data.Max = Max2D;
			Results.Add(Data);
		}
	}

	return Results;
}

void UMapManager::SaveDataToJSON(const TArray<FBoundingBoxData>& BBoxData)
{
	TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());

	// Add all 13 Parameters
	RootObject->SetStringField(TEXT("OutputDir"), OutputDir);
	RootObject->SetNumberField(TEXT("CamHeight"), CamHeight);
	RootObject->SetNumberField(TEXT("TimeOfDay"), TimeOfDay);
	RootObject->SetNumberField(TEXT("FOV"), FOV);
	RootObject->SetNumberField(TEXT("Weather"), static_cast<int32>(Weather));
	RootObject->SetNumberField(TEXT("CameraBlur"), CameraBlur);
	RootObject->SetNumberField(TEXT("AtmosphericHaze"), AtmosphericHaze);
	RootObject->SetNumberField(TEXT("CharacterCount"), CharacterCount);
	RootObject->SetNumberField(TEXT("ArrivalTime"), ArrivalTime);
	RootObject->SetNumberField(TEXT("ArmedCount"), ArmedCount);
	RootObject->SetNumberField(TEXT("RotationSpeed"), RotationSpeed);
	RootObject->SetNumberField(TEXT("VehicleID"), VehicleID);
	RootObject->SetNumberField(TEXT("BuildingID"), BuildingID);
	RootObject->SetNumberField(TEXT("Mode"), Mode);

	// Add Bounding Boxes
	TArray<TSharedPtr<FJsonValue>> BBoxArray;
	for (const FBoundingBoxData& Data : BBoxData)
	{
		TSharedPtr<FJsonObject> BBoxObj = MakeShareable(new FJsonObject());
		BBoxObj->SetStringField(TEXT("ActorName"), Data.ActorName);

		TSharedPtr<FJsonObject> MinObj = MakeShareable(new FJsonObject());
		MinObj->SetNumberField(TEXT("X"), Data.Min.X);
		MinObj->SetNumberField(TEXT("Y"), Data.Min.Y);
		BBoxObj->SetObjectField(TEXT("Min"), MinObj);

		TSharedPtr<FJsonObject> MaxObj = MakeShareable(new FJsonObject());
		MaxObj->SetNumberField(TEXT("X"), Data.Max.X);
		MaxObj->SetNumberField(TEXT("Y"), Data.Max.Y);
		BBoxObj->SetObjectField(TEXT("Max"), MaxObj);

		BBoxArray.Add(MakeShareable(new FJsonValueObject(BBoxObj)));
	}
	RootObject->SetArrayField(TEXT("BoundingBoxes"), BBoxArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);

	FString FilePath = FPaths::Combine(OutputDir, TEXT("simulation_data.json"));
	FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

// Called every frame
void UMapManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

