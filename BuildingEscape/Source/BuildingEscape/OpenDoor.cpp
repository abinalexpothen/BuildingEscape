// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

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
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the ActorThatOpens is in the trigger volume
    if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens)) // null check included
    {
        OpenDoor();
    }
}

void UOpenDoor::OpenDoor()
{
    // Find the owning object and store it in a variable
    AActor* Owner = GetOwner();
    
    FRotator NewRotation = FRotator(0.f, -60.f, 0.f);
    
    // Set actor rotation
    Owner->SetActorRotation(NewRotation);
    
    return;
}
