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
    if (PhysicsHandle)
    {
        // Physics handle is found
        UE_LOG(LogTemp, Warning, TEXT("Physics handle is found for %s."), *GetOwner()->GetName());
    }
    else
    {
        // Physics handle is not found
        UE_LOG(LogTemp, Error, TEXT("Physics handle is missing for %s."), *GetOwner()->GetName());
    }
}

// Look for attached input component (appears only at runtime) and bind grab action
void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        // Input component is found
        UE_LOG(LogTemp, Warning, TEXT("Input component found."));
        // Bind the input action
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        
        // Bind the release action
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        // Input component is not found
        UE_LOG(LogTemp, Error, TEXT("Input component is missing for %s."), *GetOwner()->GetName());
    }
}



void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab key pressed."));
    
    // LINE TRACE and see if it reach any actors with physics body collision channel set
    GetFirstPhysicsBodyInReach();
    // If we hit something attach a physics handle
    
    // TODO: attach physics handle
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab key released."));
    
    // TODO: release pfysics handle
    
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // if the physics handle is attached
        // move the object we're holding
    
    
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Get the player viewpoint
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT     PlayerViewPointLocation,
        OUT     PlayerViewPointRotation
    );
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    // Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    // Line-trace (or Ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    
    // See what we hit
    AActor* ActorHit = Hit.GetActor();
    
    if(ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT(" The line-trace has hit: %s"),*(ActorHit->GetName()))
    }
    
    return Hit;
}


