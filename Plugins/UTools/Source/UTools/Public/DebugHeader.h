// Copyright IT&feel, CA. All Rights Reserved.
#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

inline void Print(const FString& Message, const FColor& Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message);
	}
}

inline void PrintLog(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT( "%s" ), *Message);
}

inline EAppReturnType::Type ShowMessage(const FText& Message,
                                 const FText& Title,
                                 const EAppMsgType::Type MessageType = EAppMsgType::Ok,
                                 const bool bShowWarning = true)
{
	const EAppMsgCategory MessageCategory = bShowWarning ? EAppMsgCategory::Warning : EAppMsgCategory::Info;
	return FMessageDialog::Open(MessageCategory, MessageType, Message, Title);
}

inline void ShowNotifyInfo(const FString& Message)
{
	FNotificationInfo Info(FText::FromString(Message));
	Info.bUseLargeFont = true;
	Info.FadeOutDuration = 8.0f;	

	FSlateNotificationManager::Get().AddNotification(Info);
}
