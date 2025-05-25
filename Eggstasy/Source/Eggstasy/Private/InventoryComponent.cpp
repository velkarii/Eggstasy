#include "InventoryComponent.h"

void UInventoryComponent::AddItem(const FString& Item)
{
    Items.Add(Item);
}

bool UInventoryComponent::HasItem(const FString& Item) const
{
    return Items.Contains(Item);
}