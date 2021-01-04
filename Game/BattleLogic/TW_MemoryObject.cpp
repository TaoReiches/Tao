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
#include "TW_UnitLearnSkillData.h"
#include "TW_Unit.h"
#include "TW_CommandStop.h"
#include "TW_CommandMove.h"
#include "TW_CommandAttack.h"
#include "TW_CommandSpell.h"
#include "TW_CommandDropItem.h"
#include "TW_CommandPickItem.h"

TwMemoryPool<BeTaskActionStand>          mpTaskActionStand(256);
TwMemoryPool<BeTaskActionWalk>           mpTaskActionWalk(256);
TwMemoryPool<BeTaskActionAttack>         mpTaskActionAttack(256);
TwMemoryPool<BeTaskActionSpell>          mpTaskActionSpell(256);
TwMemoryPool<BeTaskActionDeath>          mpTaskActionDeath(256);

TwMemoryPool<BeTaskMoveToPos>            mpTaskMoveToPos(256);
TwMemoryPool<BeTaskMoveToUnit>           mpTaskMoveToUnit(256);
TwMemoryPool<BeTaskAttackUnit>           mpTaskAttackUnit(256);
TwMemoryPool<BeTaskAttackToPos>          mpTaskAttackToPos(256);
TwMemoryPool<BeTaskAttackItem>           mpTaskAttackItem(256);

TwMemoryPool<BeAttackingAttr>            mpAttackingAttr(256);
TwMemoryPool<BeCarry>                    mpCarry(256);
TwMemoryPool<BeBuffer>                   mpBuffer(256);
TwMemoryPool<BeSkill>                    mpSkill(256);
TwMemoryPool<BeItem>                     mpItem(256);
TwMemoryPool<BeMapItem>                  mpMapItem(256);
TwMemoryPool<BeEquip>                    mpEquip(256);
TwMemoryPool<BeEffectData>               mpEffectData(256);
TwMemoryPool<BeEffect>                   mpEffect(256);
TwMemoryPool<BeLearnSkillData>           mpLearnSkillData(256);
TwMemoryPool<TwUnit>                     mpUnit(256);
TwMemoryPool<TwUnitData>                 mpUnitData(256);

TwMemoryPool<BeStopCommand>              mpStopCommand(256);
TwMemoryPool<BeMoveCommand>              mpMoveCommand(256);
TwMemoryPool<BeAttackCommand>            mpAttackCommand(256);
TwMemoryPool<BeSpellCommand>             mpSpellCommand(256);
TwMemoryPool<BeDropItemCommand>          mpDropItemCommand(256);
TwMemoryPool<BePickItemCommand>          mpPickItemCommand(256);
