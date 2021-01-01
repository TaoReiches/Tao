#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

#include "TW_MemoryPool.h"

class BeTaskActionStand;
extern TwMemoryPool<BeTaskActionStand>          mpTaskActionStand;

class BeTaskActionWalk;
extern TwMemoryPool<BeTaskActionWalk>           mpTaskActionWalk;

class BeTaskActionAttack;
extern TwMemoryPool<BeTaskActionAttack>         mpTaskActionAttack;

class BeTaskActionSpell;
extern TwMemoryPool<BeTaskActionSpell>          mpTaskActionSpell;

class BeTaskActionDeath;
extern TwMemoryPool<BeTaskActionDeath>          mpTaskActionDeath;

class BeTaskMoveToPos;
extern TwMemoryPool<BeTaskMoveToPos>            mpTaskMoveToPos;

class BeTaskMoveToUnit;
extern TwMemoryPool<BeTaskMoveToUnit>           mpTaskMoveToUnit;

class BeTaskAttackUnit;
extern TwMemoryPool<BeTaskAttackUnit>           mpTaskAttackUnit;

class BeTaskAttackToPos;
extern TwMemoryPool<BeTaskAttackToPos>          mpTaskAttackToPos;

class BeTaskAttackItem;
extern TwMemoryPool<BeTaskAttackItem>           mpTaskAttackItem;

class BeAttackingAttr;
extern TwMemoryPool<BeAttackingAttr>            mpAttackingAttr;

class BeCarry;
extern TwMemoryPool<BeCarry>                    mpCarry;

class BeUnitData;
extern TwMemoryPool<BeUnitData>                 mpUnitData;

class TwUnit;
extern TwMemoryPool<TwUnit>                     mpUnit;

class BeSkill;
extern TwMemoryPool<BeSkill>                    mpSkill;

class BeBuffer;
extern TwMemoryPool<BeBuffer>                   mpBuffer;

class BeItem;
extern TwMemoryPool<BeItem>                     mpItem;

class BeMapItem;
extern TwMemoryPool<BeMapItem>                  mpMapItem;

class BeEquip;
extern TwMemoryPool<BeEquip>                    mpEquip;

class BeEffectData;
extern TwMemoryPool<BeEffectData>               mpEffectData;

class BeEffect;
extern TwMemoryPool<BeEffect>                   mpEffect;

class BeLearnSkillData;
extern TwMemoryPool<BeLearnSkillData>           mpLearnSkillData;

class BeStopCommand;
extern TwMemoryPool<BeStopCommand>              mpStopCommand;

class BeMoveCommand;
extern TwMemoryPool<BeMoveCommand>              mpMoveCommand;

class BeAttackCommand;
extern TwMemoryPool<BeAttackCommand>            mpAttackCommand;

class BeSpellCommand;
extern TwMemoryPool<BeSpellCommand>             mpSpellCommand;

class BeDropItemCommand;
extern TwMemoryPool<BeDropItemCommand>          mpDropItemCommand;

class BePickItemCommand;
extern TwMemoryPool<BePickItemCommand>          mpPickItemCommand;
