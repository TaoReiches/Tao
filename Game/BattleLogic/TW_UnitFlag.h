#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum BeUnitFlag
{
    BUF_NULL = 0,
    BUF_DEAD = (1 << 0),
    BUF_REMOVE = (1 << 1),
    BUF_MORPHUNIT = (1 << 2),
    BUF_ISGHOST = (1 << 3),
    BUF_ISDIVIDMAN = (1 << 4),
    BUF_ISSUMMONTEMP = (1 << 5),
    BUF_ISSUMMONPER = (1 << 6),
    BUF_ISBLOCKED = (1 << 7),
    BUF_ISPERSIST = (1 << 8),
    BUF_ISINVINCIBLE = (1 << 9),
    BUF_HASBUMPATTACK = (1 << 10),
    BUF_NOOBSTACLE = (1 << 11),
    BUF_IGNOREUNITOBS = (1 << 12),
    BUF_IGNOREFIXEDOBS = (1 << 13),
    BUF_MOVING = (1 << 14),
    BUF_OBSTACLESET = (1 << 15),
    BUF_RELIVE = (1 << 16),
    BUF_BUYLIVE = (1 << 17),
    BUF_SPELL_SHARK = (1 << 18),
    BUF_HIDE = (1 << 19),
    BUF_HASINVISIBLE = (1 << 20),
    BUF_HASSHARED = (1 << 21),
    BUF_HASHALOSKILL = (1 << 22),
    BUF_IGNOREPHYSICRESIST = (1 << 23),
    BUF_NEEDUPDATESKILL = (1 << 24),
    BUF_NEEDUPDATEITEMSKILL = (1 << 25),
    BUF_ISPERSISTSKILL = (1 << 26),
    BUF_HAS_BIRTHACTION = (1 << 27),
    BUF_NEEDRECYCLEITEM = (1 << 28),
    BUF_IGNOREDOODADOBS = (1 << 29),
    BUF_ITEMINVALID = (1 << 30),
    BUF_IGNORETERRAINOBS = (1 << 31),

    BUF_HIDEALL = BUF_HIDE | BUF_NOOBSTACLE | BUF_ISINVINCIBLE,
    BUF_IGNOREALLOBS = BUF_IGNOREUNITOBS | BUF_IGNOREFIXEDOBS | BUF_IGNOREDOODADOBS,
    BUF_TEMPSUMMONNOTCONTROL = BUF_ISINVINCIBLE | BUF_ISSUMMONTEMP | BUF_IGNOREALLOBS,
    BUF_TEMPGHOSTNOTCONTROL = BUF_ISINVINCIBLE | BUF_ISGHOST | BUF_IGNOREALLOBS,
};
