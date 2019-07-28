// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
	
}

// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Physics handle is missing for %s."), *GetOwner()->GetName());
    }
}

// Look for attached input component (appears only at runtime) and bind grab action
void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        // Input component is not found
        UE_LOG(LogTemp, Error, TEXT("Input component is missing for %s."), *GetOwner()->GetName());
    }
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // if a physics ohandle is attached
    if(PhysicsHandle->GrabbedComponent)
    {
        // move the object we are holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}

void UGrabber::Grab()
{
    // Line trace and see if reach any actor with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh 
    auto ActorHit = HitResult.GetActor();
    
    // If we hit something attach a physics handle
    if (ActorHit)
    {
        PhysicsHandle->GrabComponentAtLocationWithRotation(
           ComponentToGrab,
           NAME_None, // no bones needed
           ComponentToGrab->GetOwner()->GetActorLocation(),
           ComponentToGrab->GetOwner()->GetActorRotation()
       );
    }
}

void UGrabber::Release() { PhysicsHandle->ReleaseComponent(); }

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Line-trace (or Ray-cast) out to reach distance
    FHitResult HitResult;
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(
        OUT HitResult,
        GetReachLineStart(),
        GetReachLineEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
       OUT     PlayerViewPointLocation,
       OUT     PlayerViewPointRotation
    );
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
       OUT     PlayerViewPointLocation,
       OUT     PlayerViewPointRotation
    );
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
