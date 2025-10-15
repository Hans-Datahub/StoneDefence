#include "Data/Save/GameSaveSlotList.h"

void UGameSaveSlotList::InitSaveGame(UWorld* InWorld)
{
	SlotList.InitSlot();
}

void UGameSaveSlotList::InitSaveGameFromArchives(UWorld* InWorld)
{
	for (auto& Tmp : SlotList.Slots)
	{
		Tmp.Value.GameThumbnail.InitResources();
	}
}
