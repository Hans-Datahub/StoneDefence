// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/SimpleArchivesInterface.h"
#include "Core/SimpleArchivesGlobalVariable.h"

// Add default functionality here for any ISimpleArchivesInterface functions that are not pure virtual.
ISimpleArchivesInterface::ISimpleArchivesInterface()
{
	SimpleArchivesGlobalVariable::AddArchivesInterface(this);//此处this实际会指向子类Instance，因为是子类的构造调用父类的构造
}

ISimpleArchivesInterface::~ISimpleArchivesInterface()
{
	SimpleArchivesGlobalVariable::RemoveArchivesInterface(this);
}
