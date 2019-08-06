// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    if (Owner == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Owning door is not attached."));
    }
    
    if (PressurePlate == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Pressure plate not attached."));
    }
}

void UOpenDoor::OpenDoor()
{
    if (!Owner) { return; }
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
    if (!Owner) { return; }
    Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the ActorThatOpens is in the trigger volume
    if (GetTotalMassOfActorsOnPlate() > 50.f) // TODO: make into a parameter
    {
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    // check if it's time to close the door
    if (GetWorld()->GetTimeSeconds() > LastDoorOpenTime + DoorCloseDelay)
    {
        CloseDoor();
    }

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    TArray<AActor*> OverlappingActors;
    
    if (!PressurePlate) { return TotalMass; }
    // find all overlapping actors
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    // iterate through them using their masses
    for (const auto* Actor:OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate."), *Actor->GetName());
    }
    return TotalMass;
}
