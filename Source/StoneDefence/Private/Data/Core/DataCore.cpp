#include "Data/Core/DataCore.h"

FDataCore::FDataCore()
{
	Init();
}

void FDataCore::Init()
{
	Name = NAME_None;
	ID = INDEX_NONE;
	Gold = 0;
	Diamonds = 0;
	Copper = 0;
}

bool FDataCore::IsValid() const
{
	return ID != INDEX_NONE;
}
