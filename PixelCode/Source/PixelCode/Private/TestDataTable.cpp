// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDataTable.h"
#include <../../../../../../../Source/Runtime/Core/Public/Misc/FileHelper.h>
#include "Containers/UnrealString.h"



TArray<FCharacterData> UTestDataTable::LoadCharacterDataFromFile(const FString& FilePath)
{
    TArray<FCharacterData> LoadedData;

    // 파일 경로 설정
    FString FullPath = FPaths::ProjectContentDir() + FilePath;

    // 파일 읽기
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FullPath);
        return LoadedData;
    }

    // CSV 파싱
    TArray<FString> Lines;
    FileContent.ParseIntoArrayLines(Lines);

    for (int32 i = 1; i < Lines.Num(); ++i) // 첫 번째 줄은 헤더이므로 무시
    {
        FString Line = Lines[i];
        TArray<FString> Tokens;
        Line.ParseIntoArray(Tokens, TEXT(","), true);

        if (Tokens.Num() != 3)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid data format in line %d"), i);
            continue;
        }

        FCharacterData CharacterData;
        CharacterData.Name = Tokens[0];
        CharacterData.Level = FCString::Atoi(*Tokens[1]);
        CharacterData.Health = FCString::Atof(*Tokens[2]);

        LoadedData.Add(CharacterData);
    }

    return LoadedData;
}

// 데이터 유효성 검사

//TArray<FCharacterData> UCharacterDataManager::LoadCharacterDataFromFile(const FString& FilePath)
//{
//    TArray<FCharacterData> LoadedData;
//
//    // 파일 경로 설정
//    FString FullPath = FPaths::ProjectContentDir() + FilePath;
//
//    // 파일 읽기
//    TArray<FString> FileLines;
//    if (!FFileHelper::LoadFileToStringArray(FileLines, *FullPath))
//    {
//        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FullPath);
//        return LoadedData;
//    }
//
//    // 데이터 파싱
//    for (int32 i = 1; i < FileLines.Num(); ++i) // 첫 번째 라인은 헤더이므로 무시
//    {
//        FString Line = FileLines[i];
//        TArray<FString> Tokens;
//        Line.ParseIntoArray(Tokens, TEXT(","), true);
//
//        if (Tokens.Num() != 3)
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Invalid data format in line %d"), i);
//            continue;
//        }
//
//        // 유효성 검사
//        FString Name = Tokens[0];
//        if (Name.IsEmpty())
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Empty name in line %d"), i);
//            continue;
//        }
//
//        int32 Level = FCString::Atoi(*Tokens[1]);
//        if (Level <= 0)
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Invalid level in line %d"), i);
//            continue;
//        }
//
//        float Health = FCString::Atof(*Tokens[2]);
//        if (Health <= 0.0f)
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Invalid health in line %d"), i);
//            continue;
//        }
//
//        // 데이터 구조에 저장
//        FCharacterData CharacterData;
//        CharacterData.Name = Name;
//        CharacterData.Level = Level;
//        CharacterData.Health = Health;
//
//        LoadedData.Add(CharacterData);
//    }
//
//    return LoadedData;
//}