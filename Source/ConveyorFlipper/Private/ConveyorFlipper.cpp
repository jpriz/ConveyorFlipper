// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConveyorFlipper.h"

#include "AFGMyConveyorLift.h"
#include "FGColoredInstanceMeshProxy.h"
#include "Buildables/FGBuildableConveyorBase.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Equipment/FGBuildGunDismantle.h"
#include "Patching/NativeHookManager.h"
#include "FGFactoryConnectionComponent.h"
#include "Buildables/FGBuildableConveyorLift.h"
#include "InputCoreTypes.h"
#include "Registry/ModKeyBindRegistry.h"
#include "Algo/Reverse.h"
#include "Equipment/FGBuildGunBuild.h"
#include "Hologram/FGConveyorLiftHologram.h"

DEFINE_LOG_CATEGORY(LogConveyorFlipperLog);

IMPLEMENT_MODULE(FConveyorFlipperModule, ConveyorFlipper)

#define LOCTEXT_NAMESPACE "FConveyorFlipperModule"

FVector& FlipVector(FVector& Vector)
{
	Vector.X = -Vector.X;
	Vector.Y = -Vector.Y;
	return Vector;
}



void FConveyorFlipperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	SUBSCRIBE_METHOD(UFGBuildGunStateDismantle::SecondaryFire,[] (CallScope<void(*)(UFGBuildGunState*)>& CallScope2, UFGBuildGunState* UfgBuildGunState)
	{
	//	UModKeyBindRegistry::RegisterModKeyBind("ConveyorFlipper", FInputActionKeyMapping("ConveyorFlipper.FlipConveyor","E"),FText::FromString("Conveyor Flipper"));
		APlayerController* PlayerController = UfgBuildGunState->GetWorld()->GetFirstPlayerController();
		FHitResult HitResult;
		if (PlayerController)//r && LocalPlayer->ViewportClient)
		{
			{
				if (UfgBuildGunState->GetBuildGun()->IsInState(EBuildGunState::BGS_DISMANTLE)){
					AActor* Actor = Cast<UFGBuildGunStateDismantle>(UfgBuildGunState)->GetSelectedActor();  //HitResult.GetActor();
					if (Actor != NULL && Actor != nullptr){
						const FString ActorName  = Actor->GetName();
						UE_LOG(LogConveyorFlipperLog, Warning, TEXT("The Actor's name is %s"), *ActorName);

						if (Actor->GetClass()->IsChildOf(AFGBuildableConveyorBelt::StaticClass()))
						{
							//UE_LOG(LogCppTestModLog, Warning, TEXT("found conveyor!"));
							AFGBuildableConveyorBelt* Conveyor = Cast<AFGBuildableConveyorBelt>(Actor);
							UFGFactoryConnectionComponent* Connection0 = Conveyor->GetConnection0();
							UFGFactoryConnectionComponent* Connection1 = Conveyor->GetConnection1();

							if (!Connection0->IsConnected() && !Connection1->IsConnected())
							{
								TArray< FSplinePointData > OldSplineData = Conveyor->GetSplineData();
								const uint32 ArraySize = OldSplineData.Num();
								TArray< FSplinePointData > NewSplineData;
								//UE_LOG(LogCppTestModLog, Warning, TEXT("array size %d"), ArraySize);

								for(uint32 i= ArraySize - 1 ; i > 0 ; i--)
								{
									NewSplineData.Add(FSplinePointData(OldSplineData[i].Location, FlipVector(OldSplineData[i].LeaveTangent), FlipVector(OldSplineData[i].ArriveTangent)));
								}
								NewSplineData.Add(FSplinePointData(OldSplineData[0].Location, FlipVector(OldSplineData[0].LeaveTangent), FlipVector(OldSplineData[0].ArriveTangent)));
								AFGBuildableConveyorBelt::Respline(Conveyor, NewSplineData);
							}
						}

						
					}
				}
			}
		}
		//return bHit;
		
	});     		
}

void FConveyorFlipperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



#undef LOCTEXT_NAMESPACE