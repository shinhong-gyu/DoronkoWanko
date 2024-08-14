// Fill out your copyright notice in the Description page of Project Settings.


#include "HJMiniMapWidget.h"

void UHJMiniMapWidget::ShowFloor(int32 Floor)
{
	if (!Image_0 || !Image_1 || !Image_2 || !Image_3 || !Image_4 || !Image_5 || !Image_6 || !Image_7)
	{
		return;
	}

	// �⺻ ZOrder �� �ʱ�ȭ
	zaZOrder = 1;
	zbZOrder = 0;
	zcZOrder = 0;
	zdZOrder = 0;
	zeZOrder = 0;
	zfZOrder = 0;
	zgZOrder = 0;
	zhZOrder = 0;

	// CanvasPanelSlot���� ĳ����
	saSlot = Cast<UCanvasPanelSlot>(Image_0->Slot);
	sbSlot = Cast<UCanvasPanelSlot>(Image_1->Slot);
	scSlot = Cast<UCanvasPanelSlot>(Image_2->Slot);
	sdSlot = Cast<UCanvasPanelSlot>(Image_3->Slot);
	seSlot = Cast<UCanvasPanelSlot>(Image_4->Slot);
	sfSlot = Cast<UCanvasPanelSlot>(Image_5->Slot);
	sgSlot = Cast<UCanvasPanelSlot>(Image_6->Slot);
	shSlot = Cast<UCanvasPanelSlot>(Image_7->Slot);

	// ��� Slot�� ��ȿ���� Ȯ��
	if (saSlot && sbSlot && scSlot && sdSlot && seSlot && sfSlot && sgSlot && shSlot)
	{
		// �ʱ� ZOrder ����
		saSlot->SetZOrder(zaZOrder);
		sbSlot->SetZOrder(zbZOrder);
		scSlot->SetZOrder(zcZOrder);
		sdSlot->SetZOrder(zdZOrder);
		seSlot->SetZOrder(zeZOrder);
		sfSlot->SetZOrder(zfZOrder);
		sgSlot->SetZOrder(zgZOrder);
		shSlot->SetZOrder(zhZOrder);
	}

	// Floor�� ���� ZOrder ����
	switch (Floor)
	{
	case 0: // ���� 
		zaZOrder = 0;
		zbZOrder = 0;
		zcZOrder = 0;
		zdZOrder = 2;
		zeZOrder = 0;
		zfZOrder = 0;
		zgZOrder = 0;
		zhZOrder = 0;
		break;
	case 1:
		zaZOrder = 2;
		zbZOrder = 0;
		zcZOrder = 0;
		zdZOrder = 0;
		zeZOrder = 0;
		zfZOrder = 0;
		zgZOrder = 0;
		zhZOrder = 0;
		break;
	case 2:
		zaZOrder = 0;
		zbZOrder = 0;
		zcZOrder = 0;
		zdZOrder = 0;
		zeZOrder = 0;
		zfZOrder = 0;
		zgZOrder = 2;
		zhZOrder = 0;
		break;

		return;
	}
}
