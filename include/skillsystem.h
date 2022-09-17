#ifndef GUARD_SKILLSYS_H
#define GUARD_SKILLSYS_H

#define SKILL_ROM_LIST_MAX 5
#define SKILL_FAST_LIST_LEN 10
#define SKILL_ICON_SHEET_INDEX 0x1
#define SKILL_ICON(index) ((SKILL_ICON_SHEET_INDEX << 8) + index)

/**
 * skill index definitions
 * 
 */
enum skill_index {
	skill_index_0 = 0,

	/* battle status skills */
	skill_index_battle_status_blow_death,

	/* unit status bonus skills */
	skill_index_unit_bonus_pow,
	skill_index_unit_bonus_skl,
	skill_index_unit_bonus_spd,
	skill_index_unit_bonus_luk,
	skill_index_unit_bonus_def,
	skill_index_unit_bonus_res,
	skill_index_unit_bonus_life_and_death,

	/* end */
	skill_index_ff = 0xFF,
};

#define SKILL_VALID(aSkill) ((aSkill > skill_index_0) && (aSkill < skill_index_ff))

/**
 * struct skill_info - information about a skill
 * 
 * @msg_desc: text index of skill description
 * @msg_name: text index of skill name, may not be set to reserve space
 * @icon: pointer to icon graphics
 */

struct skill_info {
	/* 00 */ u16 msg_desc;
	/* 02 */ u16 msg_name;
	/* 04 */ const void* icon;
};

/**
 * skill_fast_list - ERAM data assist for judgement
 * directly get this via function: get_or_create_skill_fast_list(unit)
 * 
 * @unit_index: index of unit struct to identify which unit
 * @len: number of skills, for nihil, you can directly put len to zero
 * @skill: skills list
 */
struct skill_fast_list {
    u8 unit_index;
    s8 len;
    u8 skill[SKILL_FAST_LIST_LEN];
};

/**
 * skill_rom_table - ROM data stored character & class's basic skills
 * 
 * @skill: skills list
 */
struct skill_rom_table {
	u8 skill[SKILL_ROM_LIST_MAX];
};

extern struct skill_info CONST_DATA skill_info_table[0x100];
extern struct skill_rom_table CONST_DATA character_skill_rom_table[0x100];
extern struct skill_rom_table CONST_DATA class_skill_rom_table[0x100];
extern int (*judge_skill_func)(struct Unit*, const u8);

/**
 * init_skill_system - initialize judge functions, etc.
 * this function is called from main
 */
void init_skill_system();

/**
 * skill_tester() - check whether unit has a skill
 *  
 * @unit: unit struct to check skill
 * @skill_id: skill index
 * @return: true if the unit has skill, false otherwise
 */
static inline int skill_tester(struct Unit* unit, const u8 skill_id)
{
	return judge_skill_func(unit, skill_id);
}

/**
 * struct get_or_create_skill_fast_list - get fast list
 * 
 * if list is merged, return it directly to fasten the judgement
 * otherwise merge it.
 * 
 * @unit: pointer to unit struct
 * @return: fast list
 */
struct skill_fast_list *get_or_create_skill_fast_list(struct Unit* unit);



/**
 * get_skill_name_msg() - get skill name text index
 *  
 * @skill_id: skill index
 * @return: text index of skill name
 */
u16 get_skill_name_msg(const u8 skill_id);
u16 get_skill_desc_msg(const u8 skill_id);

/**
 * get_skill_icon_gfx() - get skill icon graphics pointer
 *  
 * @skill_id: skill index
 * @return: pointer to skill icon graphics in rodata
 */
const void *get_skill_icon_gfx(const u8 skill_id);

#endif /* GUARD_SKILLSYS_H */
