// Test Game,  All Rights Reserved.


#include "UI/PGPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

void UPGPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();  // �������� ��������� �� ��������� � ������ ����� ��������� ����� �������
    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UPGPauseWidget::OneClearPause);
    }
}

void UPGPauseWidget::OneClearPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
   
}