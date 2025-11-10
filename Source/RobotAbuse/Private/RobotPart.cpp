// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotPart.h"

// Sets default values
ARobotPart::ARobotPart()
{
	MeshRef = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshRef;
	MeshRef->SetSimulatePhysics(true);
}


void ARobotPart::TryAttach()
{

}
