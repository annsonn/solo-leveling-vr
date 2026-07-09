#include "Arise.h"

DEFINE_LOG_CATEGORY(LogArise);

void FAriseModule::StartupModule()
{
	UE_LOG(LogArise, Log, TEXT("Arise module started"));
}

void FAriseModule::ShutdownModule()
{
	UE_LOG(LogArise, Log, TEXT("Arise module shut down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FAriseModule, Arise, "Arise");
