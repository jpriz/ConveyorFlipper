// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConveyorFlipper.h"

DEFINE_LOG_CATEGORY(LogConveyorFlipperLog);

IMPLEMENT_MODULE(FConveyorFlipperModule, ConveyorFlipper)

#define LOCTEXT_NAMESPACE "FConveyorFlipperModule"

void FConveyorFlipperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

}

void FConveyorFlipperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE