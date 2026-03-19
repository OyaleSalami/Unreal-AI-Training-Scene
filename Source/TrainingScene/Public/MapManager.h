// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include<cstdint>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "MapManager.generated.h"

UENUM(BlueprintType)
enum class EWeather : uint8
{
	Clear = 0,
	Cloudy = 1,
	Rainy = 2,
	Foggy = 3,
	Snowy = 4
};

USTRUCT(BlueprintType)
struct FBoundingBoxData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString ActorName;

	UPROPERTY(BlueprintReadOnly)
	FVector2D Min;

	UPROPERTY(BlueprintReadOnly)
	FVector2D Max;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGSCENE_API UMapManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Command line parameters
	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	FString OutputDir;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	float CamHeight;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	int32 TimeOfDay;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	float FOV;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	EWeather Weather;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	float CameraBlur;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	float AtmosphericHaze;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	int32 CharacterCount;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	float ArrivalTime;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	int32 ArmedCount;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	float RotationSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	int32 VehicleID;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	int32 BuildingID;

	UPROPERTY(BlueprintReadOnly, Category = "Parameters")
	int32 Mode;

	// Sets default values for this component's properties
	UMapManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Forces the resolution to 640x512 */
	UFUNCTION(BlueprintCallable, Category = "System")
	void ForceResolution();

	/** Calculates 2D bounding boxes for specific actors */
	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FBoundingBoxData> CalculateBoundingBoxes();

	/** Saves all parameters and bounding box data to a JSON file */
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SaveDataToJSON(const TArray<FBoundingBoxData>& BBoxData);
};
