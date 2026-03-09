// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGSCENE_API UMapManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	FString OutputDir;
	float CamHeight;
	long TimeOfDay;
	EWeather Weather;

	// Sets default values for this component's properties
	UMapManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

enum class EWeather : uint8
{
	Clear = 0,
	Cloudy = 1,
	Rainy = 2,
	Foggy = 3,
	Snowy = 4
};
