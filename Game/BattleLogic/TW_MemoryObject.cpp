/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MemoryPool.h"
#include "TW_TaskActionStand.h"
#include "TW_TaskActionWalk.h"
#include "TW_TaskActionAttack.h"
#include "TW_TaskActionSpell.h"
#include "TW_TaskActionDeath.h"
#include "TW_AttackAttr.h"

//#include "TW_Unit.h"

TeMemoryPool<BeTaskActionStand>          mpTaskActionStand;
TeMemoryPool<BeTaskActionWalk>           mpTaskActionWalk;
TeMemoryPool<BeTaskActionAttack>         mpTaskActionAttack;
TeMemoryPool<BeTaskActionSpell>          mpTaskActionSpell;
TeMemoryPool<BeTaskActionDeath>          mpTaskActionDeath;

TeMemoryPool<BeAttackingAttr>            mpAttackingAttr;

//TeMemoryPool<BeUnit>         mpUnit;
