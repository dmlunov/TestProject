// Test Game,  All Rights Reserved.


#include "UI/GameOverWidget.h"

#include "TestGameModeBase.h"
#include "Player/PGPlayerState.h"
#include "UI/PGPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "ProjectUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"  // � ���� ���������� ��������� ������� ��� ������������ ������

void UGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<ATestGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &UGameOverWidget::OnMatchStateChanged);
        }
    }
    if (ResetLevelButton)
    {
        // ��������� ������� � ������
        ResetLevelButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnResetLevel);
    }
}

void UGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}
void UGameOverWidget::UpdatePlayerStat()
{
    if (!GetWorld() || !PlayerStateBox) return;
    // �������� ����� ����������� �������� � ��������� UE
    PlayerStateBox->ClearChildren();

    // �������� ����� ������������ �� ���� �����������
    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<APGPlayerState>(Controller->PlayerState);

        if (!PlayerState) continue;
        // ������� ���� ����������� ������ ��� ������� PlayerState
        const auto PlayerStatRowWidget = CreateWidget<UPGPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);

        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        // PlayerState->GetPlayerName() �������� ��� ������ , ������ ����� ��� ���������� , ����� AI ������ ������
        PlayerStatRowWidget->SetKills(ProjectUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(ProjectUtils::TextFromInt(PlayerState->GetDeath()));
        PlayerStatRowWidget->SetTeam(ProjectUtils::TextFromInt(PlayerState->GetTiamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

        // Controller->IsPlayerController() �������� �������� �� ��������� ����������������

        // �������� � UVerticalBox* PlayerStateBox ������
        PlayerStateBox->AddChild(PlayerStatRowWidget);
    }
}

void UGameOverWidget::OnResetLevel()
{
    // const FName CurrentLevelName = "TestLevel";
    // ��������� ����� �������� ������
    const FString CurrentLevelName = "ProjectGameLevel_00"; // UGameplayStatics::GetCurrentLevelName(this);
    // ������� ��� �������� ������ �� �����. ������ �������� this ��� GetWorld()
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
    // ����������� string � FName: FName(CurrentLevelName)
}