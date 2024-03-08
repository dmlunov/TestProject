// Test Game,  All Rights Reserved.


#include "Menu/UI/PGMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PGGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"  //� ��� ������ ��� ������ �� ����
#include "Components/HorizontalBox.h"
//#include "UI/PGLevelItemWidget.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogPGMenuWidget, All, All);

void UPGMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UPGMenuWidget::OnStartGame);
    }
    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UPGMenuWidget::OnQuitGame);
    }
    //InitLevelItems();
}

//void UPGMenuWidget::InitLevelItems()
//{
//   /* const auto PGGameInstance = GetPGGameInstans();
//    if (!PGGameInstance) return;
//    checkf(PGGameInstance->GetLevelsData().Num() != 0, TEXT("Levels Data must not be empty!"));
//    if (!LevelItemsBox) return;
//    LevelItemsBox->ClearChildren();
//
//    for (auto LevelData : PGGameInstance->GetLevelsData())
//    {
//        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
//        if (!LevelItemWidget) continue;
//
//        LevelItemWidget->SetLevelData(LevelData);
//
//        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);
//
//        LevelItemsBox->AddChild(LevelItemWidget);
//
//        LevelItemWidgets.Add(LevelItemWidget);
//    }
//
//    if (PGGameInstance->GetStartupLevel().LevelName.IsNone())
//    {
//        OnLevelSelected(PGGameInstance->GetLevelsData()[0]);
//    }
//    else
//    {
//        OnLevelSelected(PGGameInstance->GetStartupLevel());
//    }*/
//}

//void UPGMenuWidget::OnLevelSelected(const FLevelData& Data)
//{
//    const auto PGGameInstance = GetPGGameInstans();
//    if (!PGGameInstance) return;
//    PGGameInstance->SetStartupLevel(Data);
//    for (auto LevelitemWidget : LevelItemWidgets)
//    {
//        if (LevelitemWidget)
//        {
//            // IsSelected ���������� bool
//            const auto IsSelected = Data.LevelName == LevelitemWidget->GetLevelData().LevelName;
//            // ��������� ��������� ����� ������ ���� �� �� ��� ��������
//            LevelitemWidget->SetSelected(IsSelected);
//        }
//    }
//}

void UPGMenuWidget::OnStartGame()
{ 
    FName StartupLevelName = "ProjectGameLevel_00";
    UGameplayStatics::OpenLevel(this, StartupLevelName);

   //// PlayAnimation(HideAnimation);
   // // ������ � �������  OnAnimationFinished_Implementation()
   // 
   // const auto PGGameInstance = GetPGGameInstans();
   // if (!PGGameInstance) return;

    
    //// ������ ������ ��� �������� ������ � ������ ������ ����
    //if (!GetWorld()) return;
    //const auto PGGameInstance = GetWorld()->GetGameInstance<UPGGameInstance>();
    //if (!PGGameInstance) return;

    //// � ������ FName ������� .IsNone() ���������� true ���� ��� == None
    //if (PGGameInstance->GetStartupLevelName().IsNone())
    //{
    //    UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is None"));
    //    return;
    //}


    //// �� �������� �������� �� IsNone()  �� ������������� ���� ������
    //UGameplayStatics::OpenLevel(this, PGGameInstance->GetStartupLevel().LevelName);
    //
    //UGameplayStatics::PlaySound2D(GetWorld(), StaptGameSound);
}

//void UPGMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
//{
//    // Super::OnAnimationFinished_Implementation(Animation);
//    // ������ ��� ������ ������� ����� ����������� ��� ����� �������
//    if (Animation != HideAnimation) return;
//    const auto PGGameInstance = GetPGGameInstans();
//    if (!PGGameInstance) return;
//    // �� �������� �������� �� IsNone()  �� ������������� ���� ������
//    UGameplayStatics::OpenLevel(this, PGGameInstance->GetStartupLevel().LevelName);
//}

void UPGMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UPGGameInstance* UPGMenuWidget::GetPGGameInstans() const
{
    if (!GetWorld()) return nullptr;

    return GetWorld()->GetGameInstance<UPGGameInstance>();
}
