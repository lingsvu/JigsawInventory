// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JigsawInventoryComponent.generated.h"


class UJigsawItem;

// Declare a delegate for the inventory change event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, TSet<FIntPoint>, ChangedSlots);

// Define the inventory component class
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JIGSAWINVEN_API UJigsawInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Default constructor to set default values
	UJigsawInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Size of the inventory grid
	FIntPoint InventorySize = FIntPoint(8, 8);

	// Map to store items in the inventory grid
	TMap<FIntPoint, UJigsawItem*> Inventory;

	// Map to store rotation status of items in the inventory
	TMap<FIntPoint, bool> IsItemRotated;

	// Function to get free slots in the inventory
	TSet<FIntPoint> GetFreeSlots();

	// Function to add an item to the inventory
	UFUNCTION(BlueprintCallable, Category = "JigSaw Inventory")
	bool AddItem(UJigsawItem* Item);

	// Event to notify when the inventory changes
	UPROPERTY(BlueprintAssignable, Category = "JigSaw Inventory")
	FOnInventoryChanged OnInventoryChanged;

	// Function to check if an item fits in a specific slot
	UFUNCTION(BlueprintCallable, Category = "JigSaw Inventory")
	bool IsItemFit(FIntPoint Slot, UJigsawItem* Item, bool bIsRotated);

	// Function to move an item from one slot to another
	UFUNCTION(BlueprintCallable, Category = "JigSaw Inventory")
	void MoveItem(FIntPoint From, FIntPoint To, bool bIsRotated);

	// Function to get the item in a specific slot
	UFUNCTION(BlueprintCallable, Category = "JigSaw Inventory")
	UJigsawItem* GetItem(FIntPoint Slot);
};
