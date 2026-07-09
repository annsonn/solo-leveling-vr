#include "Player/ArisePawnBase.h"
#include "Components/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AArisePawnBase::ARisePawnBase()
{
	PrimaryActorTick.bCanEverTick = false;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	RootComponent = VROrigin;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerLeft->SetupAttachment(VROrigin);
	MotionControllerLeft->SetTrackingMotionSource(FName("Left"));

	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	MotionControllerRight->SetupAttachment(VROrigin);
	MotionControllerRight->SetTrackingMotionSource(FName("Right"));

	MeshLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeft"));
	MeshLeft->SetupAttachment(MotionControllerLeft);

	MeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshRight"));
	MeshRight->SetupAttachment(MotionControllerRight);
}

void AArisePawnBase::BeginPlay()
{
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);
	UE_LOG(LogArise, Log, TEXT("ArisePawnBase ready"));
}

FVector AArisePawnBase::GetHMDLocationLocal() const
{
	return Camera->GetRelativeLocation();
}
