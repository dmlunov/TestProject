// Test Game,  All Rights Reserved.


#include "Menu/PGMenuPlayerController.h"
#include "PGGameInstance.h"

void APGMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());//���� ��������� ������ ��� ����������
    bShowMouseCursor = true;

    // GetWorld()->GetGameInstance<USTUGameInstance>()->TestString = "Menu says Hello";
}