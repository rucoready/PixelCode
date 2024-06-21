// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TestDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health;


   


};


UCLASS()
class PIXELCODE_API UTestDataTable : public UDataTable
{
	GENERATED_BODY()
    
     
    UFUNCTION(BlueprintCallable, Category = "Character Data")
    static TArray<FCharacterData> LoadCharacterDataFromFile(const FString& FilePath);

  
};
