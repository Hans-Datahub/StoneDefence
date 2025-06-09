
#include "Global/UI_Data.h"


//用于限制用于拖拽防御塔蓝图中键触发范围
FGuid IconGUID4DollSelection;//TowerIconGUID->IconGUID4DollSelection
AActor* TowerDoll;
bool bLockedGUID;

ARuleOfCharacter* ClickedTower = nullptr;
ARuleOfCharacter* ClickedMonster = nullptr;
