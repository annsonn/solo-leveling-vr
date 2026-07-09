#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SceneComponent.h"
#include "Components/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Arise.h"
#include "ArisePawnBase.generated.h"

UCLASS()
class ARISE_API AArisePawnBase : public APawn
{
	GENERATED_BODY()

public:
	AArisePawnBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arise|VR")
	TObjectPtr<USceneComponent> VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arise|VR")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arise|VR")
	TObjectPtr<UMotionControllerComponent> MotionControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arise|VR")
	TObjectPtr<UMotionControllerComponent> MotionControllerRight;

	// Visual representations for the hands
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arise|VR")
	TObjectPtr<UStaticMeshComponent> MeshLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arise|VR")
	TObjectPtr<UStaticMeshComponent> MeshRight;

	UFUNCTION(BlueprintCallable, Category = "Arise|VR")
	FVector GetHMDLocationLocal() const;
};
