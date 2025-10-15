// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/SimpleArchivesInterface.h"
#include "Core/SimpleArchivesGlobalVariable.h"

// Add default functionality here for any ISimpleArchivesInterface functions that are not pure virtual.
ISimpleArchivesInterface::ISimpleArchivesInterface()
{
	SimpleArchivesGlobalVariable::AddArchivesInterface(this);//�˴�thisʵ�ʻ�ָ������Instance����Ϊ������Ĺ�����ø���Ĺ���
}

ISimpleArchivesInterface::~ISimpleArchivesInterface()
{
	SimpleArchivesGlobalVariable::RemoveArchivesInterface(this);
}
