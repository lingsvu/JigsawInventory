// Fill out your copyright notice in the Description page of Project Settings.

#include "JigsawWidget.h"
#include "JigsawInventoryComponent.h"
#include "JigsawItem.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


// Function to handle native construction of the widget
void UJigsawWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (InventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is not null"));
        UGridSlot* BackGroundAsSlot = GridPanel->AddChildToGrid(InventoryBackGround, 0, 0);
        BackGroundAsSlot->SetColumnSpan(InventoryComponent->InventorySize.X);
        BackGroundAsSlot->SetRowSpan(InventoryComponent->InventorySize.Y);
        InventoryComponent->OnInventoryChanged.AddDynamic(this, &UJigsawWidget::UpdateInventory);
        for (uint8 i = 0; i <= InventoryComponent->InventorySize.X; i++)
        {
            GridPanel->SetRowFill(i, 1.0);
        }
        for (uint8 i = 0; i <= InventoryComponent->InventorySize.Y; i++)
        {
            GridPanel->SetColumnFill(i, 1.0);
        }
    }
}

// Function to handle item click in the inventory
void UJigsawWidget::OnInventoryItemStartMoving(UJigsawItemWidget* Item)
{
    if (bIsMovingItem)
    {
        return;
    }
    //CRASH CHO NAY? KHI FINISH ROI LAI CANCEL
    SlotWhenClickedOnItem = *ItemWidgets.FindKey(Item);
    ItemWhenClickedOnItem = Item;    
    MovingDestinationSlot = SlotWhenClickedOnItem;
    bIsMovingItem = true;
}

// Function to preview moving the item in a specific direction
void UJigsawWidget::PreviewMoveItem(FIntPoint Direction)
{
    MovingDestinationSlot.X += Direction.X;
    MovingDestinationSlot.Y -= Direction.Y;          
    
    if (bIsMovingItemRotated)
    {        
        MovingDestinationSlot.Y = FMath::Clamp(MovingDestinationSlot.Y, 0, InventoryComponent->InventorySize.X - ItemWhenClickedOnItem->Item->ItemSize.X);
        MovingDestinationSlot.X = FMath::Clamp(MovingDestinationSlot.X, 0, InventoryComponent->InventorySize.Y - ItemWhenClickedOnItem->Item->ItemSize.Y);
    }
    else
    {        
        MovingDestinationSlot.X = FMath::Clamp(MovingDestinationSlot.X, 0, InventoryComponent->InventorySize.X - ItemWhenClickedOnItem->Item->ItemSize.X);
        MovingDestinationSlot.Y = FMath::Clamp(MovingDestinationSlot.Y, 0, InventoryComponent->InventorySize.Y - ItemWhenClickedOnItem->Item->ItemSize.Y);
    }
    RefreshPreviewMovingItem();
}

// Function to refresh the preview of moving the item
void UJigsawWidget::RefreshPreviewMovingItem()
{    
    if (bIsMovingItemRotated)
    {
        if (MovingDestinationSlot.Y + ItemWhenClickedOnItem->Item->ItemSize.X > InventoryComponent->InventorySize.X)
        {
            MovingDestinationSlot.Y = InventoryComponent->InventorySize.X - ItemWhenClickedOnItem->Item->ItemSize.X;
        }
        GridPanel->RemoveChild(ItemWhenClickedOnItem);
        UGridSlot* NewSlotAsGridSlot = GridPanel->AddChildToGrid(ItemWhenClickedOnItem, MovingDestinationSlot.Y, MovingDestinationSlot.X);
        NewSlotAsGridSlot->SetRowSpan(ItemWhenClickedOnItem->Item->ItemSize.X);
        NewSlotAsGridSlot->SetColumnSpan(ItemWhenClickedOnItem->Item->ItemSize.Y);
        ItemWhenClickedOnItem->ItemImage->SetRenderTransformAngle(90);
        
    }
    else
    {
        if (MovingDestinationSlot.X + ItemWhenClickedOnItem->Item->ItemSize.X > InventoryComponent->InventorySize.X)
        {
            MovingDestinationSlot.X = InventoryComponent->InventorySize.X - ItemWhenClickedOnItem->Item->ItemSize.X;
        }
        GridPanel->RemoveChild(ItemWhenClickedOnItem);
        UGridSlot* NewSlotAsGridSlot = GridPanel->AddChildToGrid(ItemWhenClickedOnItem, MovingDestinationSlot.Y, MovingDestinationSlot.X);
        NewSlotAsGridSlot->SetRowSpan(ItemWhenClickedOnItem->Item->ItemSize.Y);
        NewSlotAsGridSlot->SetColumnSpan(ItemWhenClickedOnItem->Item->ItemSize.X);
        ItemWhenClickedOnItem->ItemImage->SetRenderTransformAngle(0);
    }
}

// Function to rotate the moving item
void UJigsawWidget::RotateMovingItem()
{
    if (!bIsMovingItem)
    {
        return;
    }
    bIsMovingItemRotated = !bIsMovingItemRotated;
    RefreshPreviewMovingItem();
}

// Function to finish moving the item
void UJigsawWidget::FinishMovingItem()
{
    if (!bIsMovingItem)
    {
        return;
    }
    bIsMovingItem = false;
    RefreshPreviewMovingItem();
    if (InventoryComponent->IsItemFit(MovingDestinationSlot, ItemWhenClickedOnItem->Item, bIsMovingItemRotated))
    {
        InventoryComponent->MoveItem(SlotWhenClickedOnItem, MovingDestinationSlot, bIsMovingItemRotated);
    }
    else
    {
        InventoryComponent->MoveItem(SlotWhenClickedOnItem, SlotWhenClickedOnItem, bIsMovingItemRotated);
    }
}

// Function to cancel moving the item
void UJigsawWidget::CancelMovingItem()
{
    bIsMovingItem = false;
    MovingDestinationSlot = SlotWhenClickedOnItem;
    RefreshPreviewMovingItem();
}

// Function to update the inventory display
void UJigsawWidget::UpdateInventory(TSet<FIntPoint> ChangedSlots)
{
    UE_LOG(LogTemp, Warning, TEXT("Updating Inventory with %d slots"), ChangedSlots.Num());
    if (!InventoryComponent)
    {
        return;
    }
    for (auto& ChangedSlot : ChangedSlots)
    {
        if (ItemWidgets.Contains(ChangedSlot) && InventoryComponent->Inventory.Contains(ChangedSlot))
        {
            bool IsRotated = false;
            if (InventoryComponent->IsItemRotated.Contains(ChangedSlot))
            {
                IsRotated = InventoryComponent->IsItemRotated[ChangedSlot];
            };
            if (IsRotated)
            {
                ItemWidgets[ChangedSlot]->ItemImage->SetRenderTransformAngle(90);
            }
            ItemWidgets[ChangedSlot]->Item = InventoryComponent->Inventory[ChangedSlot];
            ItemWidgets[ChangedSlot]->ItemImage->SetBrushFromTexture(
                InventoryComponent->Inventory[ChangedSlot]->ItemImage.Get());
            ItemWidgets[ChangedSlot]->ItemName->SetText(
                FText::FromString(InventoryComponent->Inventory[ChangedSlot]->Name));
            ItemWidgets[ChangedSlot]->ItemCount->SetText(
                FText::FromString(FString::FromInt(InventoryComponent->Inventory[ChangedSlot]->CurrentStackSize)));
        }
        else if (InventoryComponent->Inventory.Contains(ChangedSlot))
        {
            UE_LOG(LogTemp, Warning, TEXT("Adding new item widget"));
            UJigsawItemWidget* NewItemWidget = CreateWidget<UJigsawItemWidget>(this, ItemWidgetClass);
            bool IsRotated = false;
            if (InventoryComponent->IsItemRotated.Contains(ChangedSlot))
            {
                IsRotated = InventoryComponent->IsItemRotated[ChangedSlot];
            };

            NewItemWidget->Item = InventoryComponent->Inventory[ChangedSlot];
            NewItemWidget->ItemImage->SetBrushFromSoftTexture(InventoryComponent->Inventory[ChangedSlot]->ItemImage);
            NewItemWidget->ItemName->SetText(FText::FromString(InventoryComponent->Inventory[ChangedSlot]->Name));
            NewItemWidget->ItemCount->SetText(
                FText::FromString(FString::FromInt(InventoryComponent->Inventory[ChangedSlot]->CurrentStackSize)));
            ItemWidgets.Add(ChangedSlot, NewItemWidget);
            NewItemWidget->OnItemStartMoving.AddDynamic(this, &UJigsawWidget::OnInventoryItemStartMoving);
            UGridSlot* NewSlot = GridPanel->AddChildToGrid(NewItemWidget, ChangedSlot.Y, ChangedSlot.X);
            if (IsRotated)
            {
                NewItemWidget->ItemImage->SetRenderTransformAngle(90);
                NewSlot->SetRowSpan(InventoryComponent->Inventory[ChangedSlot]->ItemSize.X);
                NewSlot->SetColumnSpan(InventoryComponent->Inventory[ChangedSlot]->ItemSize.Y);
            }
            else
            {
                NewSlot->SetRowSpan(InventoryComponent->Inventory[ChangedSlot]->ItemSize.Y);
                NewSlot->SetColumnSpan(InventoryComponent->Inventory[ChangedSlot]->ItemSize.X);
            }
            NewSlot->SetLayer(1);
        }
        else
        {
            if (ItemWidgets.Contains(ChangedSlot))
            {
                ItemWidgets[ChangedSlot]->RemoveFromParent();
                ItemWidgets.Remove(ChangedSlot);
            }
        }
    }
}

// Function to handle item clicked event
void UJigsawItemWidget::OnItemClickedNative()
{
    OnItemStartMoving.Broadcast(this);
}

// Function to handle native construction of the widget
void UJigsawItemWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (ItemButton)
    {
        ItemButton->OnClicked.AddDynamic(this, &UJigsawItemWidget::OnItemClickedNative);
    }
}
