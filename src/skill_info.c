#include "global.h"

#include "constants/classes.h"
#include "constants/characters.h"
#include "skillsystem.h"
#include "bmunit.h"

enum msgs {
    MSG_SKILL_DESC_,
    MSG_SKILL_DESC_WIP = 0x0D4C, /* "WIP" */
    MSG_SKILL_DESC_UNIT_BONUS_POW = 0x0D4D, /* unit_bonus_pow */
    MSG_SKILL_DESC_BATTLE_STATUS_BLOW_DEATH = 0x0D4E, /* battle_status_blow_death */
};

extern const u8 skill_icon_tiles[0x1000][0x80];

#define INFO_NAME(_name) .msg_name = _name,

#define DEF_INFO(_index, _name, _desc)  \
        [_index] = {                    \
            INFO_NAME(_name)            \
            .msg_desc = _desc,          \
            .icon = skill_icon_tiles[_index]    \
        }

struct skill_info CONST_DATA skill_info_table[0x100] = {

    DEF_INFO(skill_index_0,
             MSG_SKILL_DESC_WIP,
             MSG_SKILL_DESC_WIP),
    
    /* battle status skills */
    DEF_INFO(skill_index_battle_status_blow_death,
             MSG_SKILL_DESC_WIP,
             MSG_SKILL_DESC_BATTLE_STATUS_BLOW_DEATH),
    
    /* unit status bonus skills */
    DEF_INFO(skill_index_unit_bonus_pow,
             MSG_SKILL_DESC_WIP,
             MSG_SKILL_DESC_UNIT_BONUS_POW),
};


u16 get_skill_name_msg(const u8 skill_id)
{
	u16 msg = skill_info_table[skill_id].msg_name;

	return 0 == msg 
			  ? skill_info_table[0].msg_name
			  : msg;
}

u16 get_skill_desc_msg(const u8 skill_id)
{
	u16 msg = skill_info_table[skill_id].msg_desc;

	return 0 == msg 
			  ? skill_info_table[0].msg_desc
			  : msg;
}

const void *get_skill_icon_gfx(const u8 skill_id)
{
	const void *icon_gfx = skill_info_table[skill_id].icon;

	return 0 == icon_gfx 
			  ? skill_info_table[0].icon 
			  : icon_gfx;
}


/**
 * Skill table
 * 
 */

struct skill_rom_table CONST_DATA character_skill_rom_table[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skill = {
            skill_index_battle_status_blow_death,
            skill_index_unit_bonus_pow,
        },
    },
};

struct skill_rom_table CONST_DATA class_skill_rom_table[0x100] = {
    [CLASS_PALADIN] = {
        .skill = {
            skill_index_battle_status_blow_death,
            skill_index_unit_bonus_pow,
        },
    },
};
