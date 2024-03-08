// Test Game,  All Rights Reserved.


#include "UI/GameOverWidget.h"

#include "TestGameModeBase.h"
#include "Player/PGPlayerState.h"
#include "UI/PGPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "ProjectUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"  // в этой библиотеке находится функция для перезагрузки уровня

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
        // добалляем делегат к кнопке
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
    // удаление ранее добавленных виждетов в редакторе UE
    PlayerStateBox->ClearChildren();

    // создание цикла итерирования по всем контролерам
    for (auto It = GetWorld()->GetControllerIterator(); It; It++)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<APGPlayerState>(Controller->PlayerState);

        if (!PlayerState) continue;
        // создаем свой собственный виджет для каждого PlayerState
        const auto PlayerStatRowWidget = CreateWidget<UPGPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);

        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        // PlayerState->GetPlayerName() передает имя игрока , обычно берет имя компьютера , имена AI обычно пустые
        PlayerStatRowWidget->SetKills(ProjectUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(ProjectUtils::TextFromInt(PlayerState->GetDeath()));
        PlayerStatRowWidget->SetTeam(ProjectUtils::TextFromInt(PlayerState->GetTiamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

        // Controller->IsPlayerController() проверка является ли контролер пользовательским

        // передаем в UVerticalBox* PlayerStateBox виджет
        PlayerStateBox->AddChild(PlayerStatRowWidget);
    }
}

void UGameOverWidget::OnResetLevel()
{
    // const FName CurrentLevelName = "TestLevel";
    // получение имени текущего уровня
    const FString CurrentLevelName = "ProjectGameLevel_00"; // UGameplayStatics::GetCurrentLevelName(this);
    // функция для загрузки уровня по имени. первый параметр this или GetWorld()
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
    // преобразуем string в FName: FName(CurrentLevelName)
}