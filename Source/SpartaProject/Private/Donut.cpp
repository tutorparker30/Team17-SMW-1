#include "Donut.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ADonut::ADonut()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Shapes/Shape_Torus.Shape_Torus"));
    if (MeshAsset.Succeeded())
    {
        StaticMeshComp->SetStaticMesh(MeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile"));
    if (MaterialAsset.Succeeded())
    {
        StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
    }

    PrimaryActorTick.bCanEverTick = true;
    MoveSpeed = 40.0f;
    MaxRange = 50.0f;
}

void ADonut::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(ChangeMeshTimerHandle, this, &ADonut::ChangeMesh, 2.0f, true);
    }

    StartLocation = GetActorLocation();
    MoveDirection = 1;

    // SpawnDonuts(); // 이 호출을 주석 처리하여 무한 재귀를 방지
}


void ADonut::SpawnDonuts()
{
    const int32 NumDonuts = 6;
    const float Radius = 200.0f;  // 원형으로 배치할 반경
    const float SizeIncrement = 100.0f;  // 크기 증가량

    for (int32 i = 0; i < NumDonuts; ++i)
    {
        // 각도 계산
        float Angle = (360.0f / NumDonuts) * i;
        FVector SpawnLocation = GetActorLocation() + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0) * Radius;

        // Donut 복제
        ADonut* NewDonut = GetWorld()->SpawnActor<ADonut>(GetClass(), SpawnLocation, FRotator::ZeroRotator);

        if (NewDonut)
        {
            // 로그 출력
            UE_LOG(LogTemp, Warning, TEXT("Donut spawned at: %s"), *SpawnLocation.ToString());

            // 크기를 100씩 증가
            FVector NewScale = NewDonut->StaticMeshComp->GetComponentScale() + FVector(SizeIncrement * (i + 1), SizeIncrement * (i + 1), SizeIncrement * (i + 1));
            NewDonut->StaticMeshComp->SetWorldScale3D(NewScale);
        }
        else
        {
            // 실패 로그 출력
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn Donut"));
        }
    }
}

void ADonut::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(MoveSpeed))
    {
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, MoveDirection * MoveSpeed * DeltaTime);

        if (NewLocation.Z >= StartLocation.Z + MaxRange)
        {
            NewLocation.Z = StartLocation.Z + MaxRange;
            MoveDirection = -1;
        }
        else if (NewLocation.Z <= StartLocation.Z - MaxRange)
        {
            NewLocation.Z = StartLocation.Z - MaxRange;
            MoveDirection = 1;
        }

        SetActorLocation(NewLocation);
    }
}

void ADonut::ChangeMesh()
{
    if (StaticMeshComp)
    {
        bool bIsVisible = StaticMeshComp->IsVisible();
        StaticMeshComp->SetVisibility(!bIsVisible);

        if (!bIsVisible)
        {
            GetWorld()->GetTimerManager().SetTimer(ChangeMeshTimerHandle, this, &ADonut::ChangeMesh, 1.0f, false);
        }
        else
        {
            GetWorld()->GetTimerManager().SetTimer(ChangeMeshTimerHandle, this, &ADonut::ChangeMesh, 2.0f, false);
        }
    }
}

void ADonut::ResetActorPosition()
{
    SetActorLocation(FVector::ZeroVector);
}

float ADonut::GetRotationSpeed() const
{
    return MoveSpeed;
}

void ADonut::DestroyActor()
{
    Destroy();
}
