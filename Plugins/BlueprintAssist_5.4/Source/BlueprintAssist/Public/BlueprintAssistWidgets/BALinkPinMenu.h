// Copyright fpwong. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BAFilteredList.h"

class UEdGraphPin;
class FBAGraphHandler;

struct FPinLinkerStruct final : IBAFilteredListItem
{
	UEdGraphPin* Pin;
	FString PinName;
	FString PinFullString;

	FPinLinkerStruct(UEdGraphPin* InPin, const FString& InPinName);

	virtual FString GetKeySearchText() const override;
	virtual FString ToString() const override;
};

class BLUEPRINTASSIST_API SBALinkPinMenu final : public SCompoundWidget
{
	// @formatter:off
	SLATE_BEGIN_ARGS(SBALinkPinMenu)
		: _GraphHandler(TSharedPtr<FBAGraphHandler>())
		, _SourcePin(nullptr) { }
		SLATE_ARGUMENT(TSharedPtr<FBAGraphHandler>, GraphHandler)
		SLATE_ARGUMENT(UEdGraphPin*, SourcePin)
	SLATE_END_ARGS()
	// @formatter:on

	static FVector2D GetWidgetSize() { return FVector2D(400, 200); }

	void Construct(const FArguments& InArgs);
	virtual ~SBALinkPinMenu() override;

	void InitListItems(TArray<TSharedPtr<FPinLinkerStruct>>& Items);

	TSharedRef<ITableRow> CreateItemWidget(TSharedPtr<FPinLinkerStruct> Item, const TSharedRef<STableViewBase>& OwnerTable) const;

	void SelectItem(TSharedPtr<FPinLinkerStruct> Item);

	void MarkActiveSuggestion(TSharedPtr<FPinLinkerStruct> Item);

protected:
	TSharedPtr<SBAFilteredList<TSharedPtr<FPinLinkerStruct>>> FilteredList;
	TSharedPtr<FBAGraphHandler> GraphHandler;
	UEdGraphPin* SourcePin = nullptr;
	FVector2D SavedLocation;
	TSharedPtr<FPinLinkerStruct> LastSelectedItem;

	bool CanConnectSourceToPin(UEdGraphPin* Pin);

	virtual void Tick(
		const FGeometry& AllottedGeometry,
		const double InCurrentTime,
		const float InDeltaTime) override;
};
