#include "SScreenLoading.h"
#include "Widgets/Notifications/SProgressBar.h"

void SScreenLoading::Construct(const FArguments& InArgs, const FString &InMapname) {
	MapName = InMapname;
	ChildSlot[
		SNew(SProgressBar).Percent(this, &SScreenLoading::GetProgress)
	];
} 

TOptional<float> SScreenLoading::GetProgress() const{
	float LoadPercentage = GetAsyncLoadPercentage(*MapName);
	return LoadPercentage * 0.01;
}