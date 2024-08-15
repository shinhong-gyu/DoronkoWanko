// Fill out your copyright notice in the Description page of Project Settings.


#include "HJMiniMapWidget.h"

void UHJMiniMapWidget::ShowFloor(int32 Floor)
{
	if (!Image_0 || !Image_1 || !Image_2 || !Image_3 || !Image_4 || !Image_5 || !Image_6 || !Image_7)
	{
		return;
	}

	// 기본 ZOrder 값 초기화
	zaZOrder = 1;
	zbZOrder = 0;
	zcZOrder = 0;
	zdZOrder = 0;
	zeZOrder = 0;
	zfZOrder = 0;
	zgZOrder = 0;
	zhZOrder = 0;

	// Floor에 따른 ZOrder 설정
	switch (Floor)
	{
	case 0: // 지하 
		if (e > 0)
		{
			zaZOrder = 0;
			zbZOrder = 0;
			zcZOrder = 0;
			zdZOrder = 0;
			zeZOrder = 0;
			zfZOrder = 0;
			zgZOrder = 0;
			zhZOrder = 2;
		}
		else
		{
			zaZOrder = 0;
			zbZOrder = 0;
			zcZOrder = 0;
			zdZOrder = 0;
			zeZOrder = 0;
			zfZOrder = 0;
			zgZOrder = 2;
			zhZOrder = 0;
		}
		break;
	case 1:
		if (a > 0)
		{
			if (b > 0)
			{
				zaZOrder = 0;
				zbZOrder = 0;
				zcZOrder = 2;
				zdZOrder = 0;
				zeZOrder = 0;
				zfZOrder = 0;
				zgZOrder = 0;
				zhZOrder = 0;
			}
			else
			{
				zaZOrder = 0;
				zbZOrder = 2;
				zcZOrder = 0;
				zdZOrder = 0;
				zeZOrder = 0;
				zfZOrder = 0;
				zgZOrder = 0;
				zhZOrder = 0;
			}
		}
		else
		{
			zaZOrder = 2;
			zbZOrder = 0;
			zcZOrder = 0;
			zdZOrder = 0;
			zeZOrder = 0;
			zfZOrder = 0;
			zgZOrder = 0;
			zhZOrder = 0;
		}
		break;
	case 2:
		if (c > 0)
		{
			if (d > 0)
			{
				zaZOrder = 0;
				zbZOrder = 0;
				zcZOrder = 0;
				zdZOrder = 0;
				zeZOrder = 0;
				zfZOrder = 2;
				zgZOrder = 0;
				zhZOrder = 0;
			}
			else
			{
				zaZOrder = 0;
				zbZOrder = 0;
				zcZOrder = 0;
				zdZOrder = 0;
				zeZOrder = 2;
				zfZOrder = 0;
				zgZOrder = 0;
				zhZOrder = 0;
			}
		}
		else
		{
		zaZOrder = 0;
		zbZOrder = 0;
		zcZOrder = 0;
		zdZOrder = 2;
		zeZOrder = 0;
		zfZOrder = 0;
		zgZOrder = 0;
		zhZOrder = 0;
		}
		break;

		return;
	}


	// CanvasPanelSlot으로 캐스팅
	saSlot = Cast<UCanvasPanelSlot>(Image_0->Slot);
	sbSlot = Cast<UCanvasPanelSlot>(Image_1->Slot);
	scSlot = Cast<UCanvasPanelSlot>(Image_2->Slot);
	sdSlot = Cast<UCanvasPanelSlot>(Image_3->Slot);
	seSlot = Cast<UCanvasPanelSlot>(Image_4->Slot);
	sfSlot = Cast<UCanvasPanelSlot>(Image_5->Slot);
	sgSlot = Cast<UCanvasPanelSlot>(Image_6->Slot);
	shSlot = Cast<UCanvasPanelSlot>(Image_7->Slot);

	// 모든 Slot이 유효한지 확인
	if (saSlot && sbSlot && scSlot && sdSlot && seSlot && sfSlot && sgSlot && shSlot)
	{
		// ZOrder 설정
		saSlot->SetZOrder(zaZOrder);
		sbSlot->SetZOrder(zbZOrder);
		scSlot->SetZOrder(zcZOrder);
		sdSlot->SetZOrder(zdZOrder);
		seSlot->SetZOrder(zeZOrder);
		sfSlot->SetZOrder(zfZOrder);
		sgSlot->SetZOrder(zgZOrder);
		shSlot->SetZOrder(zhZOrder);
	}
}

void UHJMiniMapWidget::MiniMapUpdate(int32 StampID)
{
	switch (StampID)
	{
	case 1:
		zbZOrder = 3;
		sbSlot->SetZOrder(zbZOrder);
		a++;
		break;
	case 2:
		zcZOrder = 4;
		scSlot->SetZOrder(zcZOrder);
		b++;
		break;
	case 3:
		zeZOrder = 5;
		seSlot->SetZOrder(zeZOrder);
		c++;
		break;
	case 4:
		zfZOrder = 6;
		sfSlot->SetZOrder(zfZOrder);
		d++;
		break;
	case 5:
		zhZOrder = 7;
		shSlot->SetZOrder(zhZOrder);
		e++;
		break;
	default:
		break;
	}
}

