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
#include "TW_TaskMoveToPos.h"
#include "TW_TaskMoveToUnit.h"
#include "TW_TaskAttackUnit.h"
#include "TW_TaskAttackToPos.h"
#include "TW_TaskAttackItem.h"
#include "TW_Buff.h"
#include "TW_Skill.h"
#include "TW_Item.h"
#include "TW_Equip.h"
#include "TW_EffectData.h"
#include "TW_Effect.h"
#include "TW_MapItem.h"

//#include "TW_Unit.h"

TeMemoryPool<BeTaskActionStand>          mpTaskActionStand(256);
TeMemoryPool<BeTaskActionWalk>           mpTaskActionWalk(256);
TeMemoryPool<BeTaskActionAttack>         mpTaskActionAttack(256);
TeMemoryPool<BeTaskActionSpell>          mpTaskActionSpell(256);
TeMemoryPool<BeTaskActionDeath>          mpTaskActionDeath(256);

TeMemoryPool<BeTaskMoveToPos>            mpTaskMoveToPos(256);
TeMemoryPool<BeTaskMoveToUnit>           mpTaskMoveToUnit(256);
TeMemoryPool<BeTaskAttackUnit>           mpTaskAttackUnit(256);
TeMemoryPool<BeTaskAttackToPos>          mpTaskAttackToPos(256);
TeMemoryPool<BeTaskAttackItem>           mpTaskAttackItem(256);

TeMemoryPool<BeAttackingAttr>            mpAttackingAttr(256);
TeMemoryPool<BeCarry>                    mpCarry(256);
TeMemoryPool<BeBuffer>                   mpBuffer(256);
TeMemoryPool<BeSkill>                    mpSkill(256);
TeMemoryPool<BeItem>                     mpItem(256);
TeMemoryPool<BeMapItem>                  mpMapItem(256);
TeMemoryPool<BeEquip>                    mpEquip(256);
TeMemoryPool<BeEffectData>               mpEffectData(256);
TeMemoryPool<BeEffect>                   mpEffect(256);
//TeMemoryPool<BeUnit>         mpUnit;
