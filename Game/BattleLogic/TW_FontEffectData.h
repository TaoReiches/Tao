#pragma once

/**********************************************
* Author: Tao Wang  Copyright reserved
* Contact: tao.reiches@gmail.com
**********************************************/

enum BeFontEffectColor
{
    BFEC_GREEN = 0,
    BFEC_RED,
    BFEC_SILVER,
    BFEC_PURPLE,
    BFEC_BLUE,
    BFEC_GOLD,
    BFEC_BLOOD_GREEN,
    BFEC_BLOOD_BLUE,
};

enum BeFontEffectSize
{
    BFES_SMALL = 0,
    BFES_BIG,
    BFES_CHARACTER,
    BFES_BIG_NEW,
};

enum BeProgramFontFlag
{
    BPFF_SONGTI = 0,
    BPFF_YAHEI = 1,
    BPFF_HUAKANG = 2,

    BPFF_FONT_MASK = 0x07,

    BPFF_SIZE_MASK = 0xF8,

    BPFF_BOLD = (1 << 8),
    BPFF_SHADOW = (1 << 9),
    BPFF_OUTLINE = (1 << 10),
    BPFF_OUTLINE_EX = (1 << 11),
    BPFF_UNDERLINE = (1 << 12),
    BPFF_ITALIC = (1 << 13),
    BPFF_VERTICAL = (1 << 14),
    BPFF_ROTATE_L90 = (1 << 15),
    BPFF_ROTATE_R90 = (1 << 16),
    BPFF_SMOOTH = (1 << 17),
    BPFF_EFFECT_MASK = 0xFF00,
    BPFF_SEFFECT_MASK = 0xF0000,
    BPFF_SPECIAL = (1 << 31),
    BPFF_SYSTEM_FLAG = (BPFF_FONT_MASK | BPFF_SIZE_MASK | BPFF_EFFECT_MASK | BPFF_SPECIAL),
    BPFF_CACHE_FLAG = (BPFF_SYSTEM_FLAG | BPFF_SEFFECT_MASK),
};

enum BeNewFontEffectType
{
    BNFET_FABAO = 0,
    BNFET_WULIBAOJI,
    BNFET_XIXUE,
    BNFET_SHANBIGEDANG,
    BNFET_NORMALDAMAGE,
    BNFET_SKILLDAMAGE,
    BNFET_GOLD,
    BNFET_SLOWDOWN,
    BNFET_DIZZY,
    BNFET_NARMAL,
};

enum BeFontEffectType
{
    BFET_PICFONT = 0,
    BFET_PROGRAMFONT_DOWNTOUP,
    BFET_PROGRAMFONT_UPTODOWN,
    BFET_PROGRAMFONT_MIDDLE,
    BFET_PROGRAMFONT_TIMERECIPROCAL,
};

#define BPFF_DEFAULT	 MAKE_FONTB(BPFF_YAHEI,14,BPFF_BOLD)
#define BPFF_UNIT_WND	 MAKE_FONTB(BPFF_YAHEI,12,BPFF_BOLD)
#define BPFF_CHAT_WORD	  MAKE_FONTB(BPFF_YAHEI,14,BPFF_BOLD)
#define BPFF_TITLE_WORD	 MAKE_FONTB(BPFF_YAHEI,16,BPFF_BOLD)
#define MAKE_FONTB(font,size,flag)   ( (font&BPFF_FONT_MASK)|((((size /2-1)<<3)&BPFF_SIZE_MASK ))|(flag&~(BPFF_FONT_MASK|BPFF_SIZE_MASK)) )
