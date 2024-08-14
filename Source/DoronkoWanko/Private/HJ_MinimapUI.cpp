// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_MinimapUI.h"
#include "Components/CanvasPanelSlot.h"

void UHJ_MinimapUI::ShowFloor(int32 Floor)
{
	if (!Image_Base || !Image_First || !Image_Second)
	{
		return;
	}

	int32 BaseZOrder = 0;
	int32 FirstZOrder = 0;
	int32 SecondZOrder = 0;
	int32 fourZOrder = 0;
	int32 FiveZOrder = 0;

	switch (Floor)
	{
	case 0:
		BaseZOrder = 3;
		FirstZOrder = 1;
		SecondZOrder = 0;
		break;
	case 1:
		BaseZOrder = 0;
		FirstZOrder = 3;
		SecondZOrder = 1;
		break;
	case 2:
		BaseZOrder = 1;
		FirstZOrder = 0;
		SecondZOrder = 3;
		break;

		return;
	}

	UCanvasPanelSlot* BaseSlot = Cast<UCanvasPanelSlot>(Image_Base->Slot);
	UCanvasPanelSlot* FirstSlot = Cast<UCanvasPanelSlot>(Image_First->Slot);
	UCanvasPanelSlot* SecondSlot = Cast<UCanvasPanelSlot>(Image_Second->Slot);
	UCanvasPanelSlot* fourSlot = Cast<UCanvasPanelSlot>(Image_four->Slot);
	UCanvasPanelSlot* FiveSlot = Cast<UCanvasPanelSlot>(Image_Five->Slot);

	if (BaseSlot && FirstSlot && SecondSlot && fourSlot && FiveSlot)
	{
		BaseSlot->SetZOrder(BaseZOrder);
		FirstSlot->SetZOrder(FirstZOrder);
		SecondSlot->SetZOrder(SecondZOrder);
		fourSlot->SetZOrder(fourZOrder);
		FiveSlot->SetZOrder(FiveZOrder);
	}

}
