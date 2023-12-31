// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JigsawWidget.generated.h"

/**
 * 
 */
class UJigsawInventoryComponent;
class UJigsawItem;
class UGridPanel;
class UImage;
class UTextBlock;
class UButton;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemWidgetAction, UJigsawItemWidget*, ItemWidget);

UCLASS(Blueprintable, BlueprintType)
class JIGSAWINVEN_API UJigsawWidget : public UUserWidget
{
	GENERATED_BODY()
public:	

    // Class of the item widget to be used in the inventory
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UJigsawItemWidget> ItemWidgetClass;

    // Reference to the grid panel in the widget
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UGridPanel* GridPanel = nullptr;

    // Reference to the inventory component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
    UJigsawInventoryComponent* InventoryComponent = nullptr;

    // Function to handle native construction of the widget
    virtual void NativeConstruct() override;

    // Map to store item widgets with their corresponding slots
    TMap<FIntPoint, class UJigsawItemWidget*> ItemWidgets;

    // Function to update the inventory display
    UFUNCTION()
    void UpdateInventory(TSet<FIntPoint> ChangedSlots);

    // Reference to the background image of the inventory
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UImage* InventoryBackGround = nullptr;

    // Function to handle item click in the inventory
    UFUNCTION()
    void OnInventoryItemStartMoving(UJigsawItemWidget* Item);

    // Variables to store information about the moving item
    FIntPoint SlotWhenClickedOnItem = FIntPoint::ZeroValue;
    FIntPoint MovingDestinationSlot = FIntPoint::ZeroValue;
	UPROPERTY()
    UJigsawItemWidget* ItemWhenClickedOnItem = nullptr;
    bool bIsMovingItem = false;
    bool bIsMovingItemRotated = false;

    // Function to preview moving the item in a specific direction
    UFUNCTION(BlueprintCallable)
    void PreviewMoveItem(FIntPoint Direction);

    // Function to refresh the preview of moving the item
    void RefreshPreviewMovingItem();

    // Function to rotate the moving item
    UFUNCTION(BlueprintCallable)
    void RotateMovingItem();

    // Function to finish moving the item
    UFUNCTION(BlueprintCallable)
    void FinishMovingItem();

    // Function to cancel moving the item
    UFUNCTION(BlueprintCallable)
    void CancelMovingItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMovingItem() const { return bIsMovingItem; }
	
};


UCLASS(Blueprintable, BlueprintType)
class JIGSAWINVEN_API UJigsawItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Reference to the Jigsaw item associated with the widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UJigsawItem* Item = nullptr;

	// Reference to the item image in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage = nullptr;

	// Reference to the item name text in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemName = nullptr;

	// Reference to the item count text in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemCount = nullptr;

	// Reference to the item button in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemButton = nullptr;

	// Reference to the item background image in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemBackground = nullptr;

	// Reference to the selected item background image in the widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemBackgroundSelected = nullptr;

	// Delegate for the item clicked event
	FItemWidgetAction OnItemStartMoving;	

	// Function to handle native construction of the widget
	virtual void NativeConstruct() override;

	// Function to handle item clicked event
	UFUNCTION()
	void OnItemClickedNative();
};
