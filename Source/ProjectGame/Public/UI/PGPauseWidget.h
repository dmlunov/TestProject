// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGPauseWidget.generated.h"

class UButton;


UCLASS()
class PROJECTGAME_API UPGPauseWidget : public UUserWidget
{
	GENERATED_BODY()
public:

protected:
    // ������� ������ , � ��������� ���� ���������� ������� ���������� ������ � ��������� � ������� ������� ����� ������������ � �++
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;  // ���� �� ��� ������ ���� � ������ � �������
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()  // ������ UFUNCTION() ������ ��� ������������� � ������������� ��������
    void OneClearPause();
};
