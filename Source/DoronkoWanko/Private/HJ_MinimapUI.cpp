// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_MinimapUI.h"
#include "Components/CanvasPanelSlot.h"

//void UHJ_MinimapUI::ShowFloor(int32 Floor)
//{
//	if (!Image_Base || !Image_First || !Image_Second)
//	{
//		return;
//	}
//
//	BaseZOrder = 0;
//	FirstZOrder = 0;
//	SecondZOrder = 0;
//	fourZOrder = 0;
//	FiveZOrder = 0;
//
//	switch (Floor)
//	{
//	case 0:
//		BaseZOrder = 3;
//		FirstZOrder = 1;
//		SecondZOrder = 0;
//		break;
//	case 1:
//		BaseZOrder = 0;
//		FirstZOrder = 3;
//		SecondZOrder = 1;
//		break;
//	case 2:
//		BaseZOrder = 1;
//		FirstZOrder = 0;
//		SecondZOrder = 3;
//		break;
//
//		return;
//	}
//
//	BaseSlot = Cast<UCanvasPanelSlot>(Image_Base->Slot);
//	FirstSlot = Cast<UCanvasPanelSlot>(Image_First->Slot);
//	SecondSlot = Cast<UCanvasPanelSlot>(Image_Second->Slot);
//	fourSlot = Cast<UCanvasPanelSlot>(Image_four->Slot);
//	FiveSlot = Cast<UCanvasPanelSlot>(Image_Five->Slot);
//
//	if (BaseSlot && FirstSlot && SecondSlot && fourSlot && FiveSlot)
//	{
//		BaseSlot->SetZOrder(BaseZOrder);
//		FirstSlot->SetZOrder(FirstZOrder);
//		SecondSlot->SetZOrder(SecondZOrder);
//		fourSlot->SetZOrder(fourZOrder);
//		FiveSlot->SetZOrder(FiveZOrder);
//	}
//
//}
//
//void UHJ_MinimapUI::MiniMapUpdate(int32 StampID)
//{
//	UE_LOG(LogTemp, Warning, TEXT("stampID : %d"), StampID);
//	switch (StampID)
//	{
//	case 1:
//		fourZOrder = 3;
//		fourSlot->SetZOrder(fourZOrder);
//		break;
//	case 2:
//		FiveZOrder = 3;
//		FiveSlot->SetZOrder(FiveZOrder);
//		break;
//		//case 3:
//		//	break;
//		//case 4:
//		//	break;
//		//case 5:
//		//	break;
//	default:
//		break;
//	}
//}
