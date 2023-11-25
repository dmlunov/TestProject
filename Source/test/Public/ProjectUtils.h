#pragma once


class ProjectUtils
{

public:
	template<typename T>
    static T* GetProjectPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

};
