// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Save/GameSaveSlotList.h"

#define LOCTEXT_NAMESPACE "SaveSlot"

FSaveSlot::FSaveSlot()
	:DataText(LOCTEXT("SaveSlot","Save Slot Yep~"))
	,isSaveSlotEmpty(true)
{}

FSaveSlotList::FSaveSlotList(){
	for (int32 i = 0; i < 20; i++)
		Slots.Add(i, FSaveSlot());
	for (int32 i = 1; i < 6; i++)
		CompletionStatus.Add(i, 0.0f);
}

int32 FSaveSlotList::GetMinialIndexOfUnsavedSlot() {
	for (auto& Temp : Slots) {
		if (Temp.Value.isSaveSlotEmpty)
			return Temp.Key;
	}

	return INDEX_NONE;
}

bool FSaveSlotList::AddGameData(int32 SlotIndex) {
	if (Slots.Contains(SlotIndex)) {

		Slots[SlotIndex].isSaveSlotEmpty = false;
		return true;
	}
	return false;
}

bool FSaveSlotList::RemoveGameData(int32 SlotIndex) {
	if (Slots.Contains(SlotIndex)) {

		Slots[SlotIndex].isSaveSlotEmpty = true;
		return true;
	}
	return false;
}


#undef LOCATEXT_NAMESPACE