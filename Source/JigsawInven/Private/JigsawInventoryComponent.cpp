// Fill out your copyright notice in the Description page of Project Settings.

#include "JigsawInventoryComponent.h"
#include "JigsawItem.h"

// Constructor
UJigsawInventoryComponent::UJigsawInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UJigsawInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UJigsawInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Function to get free slots in the inventory
TSet<FIntPoint> UJigsawInventoryComponent::GetFreeSlots()
{
    TSet<FIntPoint> FreeSlots;

    // Iterate through all slots in the inventory grid
    for (int i = 0; i < InventorySize.X; i++)
    {
        for (int j = 0; j < InventorySize.Y; j++)
        {
            FreeSlots.Add(FIntPoint(i, j));
        }
    }

    // Remove slots that are occupied by items
    for (auto& Elem : Inventory)
    {
        // Check if the item is rotated
        if (IsItemRotated.Contains(Elem.Key) && IsItemRotated[Elem.Key])
        {
            for (int i = 0; i < Elem.Value->ItemSize.Y; i++)
            {
                for (int j = 0; j < Elem.Value->ItemSize.X; j++)
                {
                    FreeSlots.Remove(Elem.Key + FIntPoint(i, j));
                }
            }
            continue;
        }

        // Remove slots occupied by non-rotated items
        for (int i = 0; i < Elem.Value->ItemSize.X; i++)
        {
            for (int j = 0; j < Elem.Value->ItemSize.Y; j++)
            {
                FreeSlots.Remove(Elem.Key + FIntPoint(i, j));
            }
        }
    }

    return FreeSlots;
}

// Function to add an item to the inventory
bool UJigsawInventoryComponent::AddItem(UJigsawItem* Item)
{
    TSet<FIntPoint> FreeSlots = GetFreeSlots();

    // Iterate through free slots to find a suitable location for the item
    for (auto FreeSlot : FreeSlots)
    {
        bool bCanFit = true;

        // Check if the item fits in the current orientation
        for (int i = 0; i < Item->ItemSize.X; i++)
        {
            for (int j = 0; j < Item->ItemSize.Y; j++)
            {
                if (!FreeSlots.Contains(FreeSlot + FIntPoint(i, j)))
                {
                    bCanFit = false;
                }
            }
        }

        // If the item doesn't fit, try the rotated orientation
        if (!bCanFit)
        {
            bCanFit = true;
            for (int i = 0; i < Item->ItemSize.Y; i++)
            {
                for (int j = 0; j < Item->ItemSize.X; j++)
                {
                    if (!FreeSlots.Contains(FreeSlot + FIntPoint(i, j)))
                    {
                        bCanFit = false;
                    }
                }
            }

            // If the item fits in the rotated orientation, add it to the inventory
            if (bCanFit)
            {
                IsItemRotated.Add(FreeSlot, true);
                Inventory.Add(FreeSlot, Item);
                OnInventoryChanged.Broadcast(TSet<FIntPoint>({ FreeSlot }));
                return true;
            }
        }

        // If the item fits in the current orientation, add it to the inventory
        IsItemRotated.Add(FreeSlot, false);
        Inventory.Add(FreeSlot, Item);
        OnInventoryChanged.Broadcast(TSet<FIntPoint>({ FreeSlot }));
        return true;
    }

    // If no suitable slot is found, return false
    return false;
}

// Function to check if an item fits in a specific slot
bool UJigsawInventoryComponent::IsItemFit(FIntPoint Slot, UJigsawItem* Item, bool bIsRotated)
{
    TSet<FIntPoint> FreeSlots = GetFreeSlots();

    // Check if the item fits in the specified slot and orientation
    if (bIsRotated)
    {
        for (int i = 0; i < Item->ItemSize.Y; i++)
        {
            for (int j = 0; j < Item->ItemSize.X; j++)
            {
                if (!FreeSlots.Contains(Slot + FIntPoint(i, j)))
                {
                    return false;
                }
            }
        }
        return true;
    }

    for (int i = 0; i < Item->ItemSize.X; i++)
    {
        for (int j = 0; j < Item->ItemSize.Y; j++)     
		{
			if (!FreeSlots.Contains(Slot + FIntPoint(i, j)))
			{
				return false;
			}
		}
	}
	return true;
}

void UJigsawInventoryComponent::MoveItem(FIntPoint From, FIntPoint To, bool bIsRotated)
{
	if (!Inventory.Contains(From))
	{
		return;
	}
	UJigsawItem* Item = Inventory[From];
	bool bIsOldItemRotated = false;
	if (IsItemRotated.Contains(From))
	{
		bIsOldItemRotated = IsItemRotated[From];
	}
	Inventory.Remove(From);
	IsItemRotated.Remove(From);
	if (IsItemFit(To, Item, bIsRotated))
	{
		Inventory.Add(To, Item);
		IsItemRotated.Add(To, bIsRotated);
		OnInventoryChanged.Broadcast(TSet<FIntPoint>({From, To}));
		return;
	}
	Inventory.Add(From, Item);
	IsItemRotated.Add(From, bIsOldItemRotated);
	
}

UJigsawItem* UJigsawInventoryComponent::GetItem(FIntPoint Slot)
{
	for (auto& Elem : Inventory)
	{
		if (Slot.X >= Elem.Key.X && Slot.X < Elem.Key.X + Elem.Value->ItemSize.X && Slot.Y >= Elem.Key.Y && Slot.Y < Elem.Key.Y + Elem.Value->ItemSize.Y)
		{
			return Elem.Value;
		}		
	}
	return nullptr;
}

