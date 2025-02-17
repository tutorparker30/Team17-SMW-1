#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Donut.generated.h"

UCLASS()
class SPARTAPROJECT_API ADonut : public AActor
{
    GENERATED_BODY()

public:
    ADonut();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Donut|Components")
    USceneComponent* SceneRoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Donut|Components")
    UStaticMeshComponent* StaticMeshComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Donut|Properties")
    float MoveSpeed;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Donut|Actions")
    void ResetActorPosition();

    UFUNCTION(BlueprintPure, Category = "Donut|Properties")
    float GetRotationSpeed() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Donut|Event")
    void OnItemPickedUp();

    UFUNCTION(BlueprintCallable, Category = "Donut|Actions")
    void SpawnDonuts(); 

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
    float ActorMoveSpeed;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MaxRange;

    FVector StartLocation;
    int32 MoveDirection;

    FTimerHandle ChangeMeshTimerHandle;
    FTimerHandle DestroyTimerHandle;

    UFUNCTION()
    void ChangeMesh();

    UFUNCTION()
    void DestroyActor();
};