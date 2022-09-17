#include "global.h"

#include "bmunit.h"
#include "bmbattle.h"
#include "skillsystem.h"

int (*judge_skill_func)(struct Unit*, const u8);
static EWRAM_DATA struct skill_fast_list skill_fast_list0 = {};
static EWRAM_DATA struct skill_fast_list skill_fast_list1 = {};

/**
 * Universial judgements
 */
static int judge_skill_common_rom_list(struct Unit* unit, const u8 skill_id)
{
    int i;
    int char_id = unit->pCharacterData->number;
    int class_id = unit->pClassData->number;

    // judge character
    for (i = 0; i < SKILL_ROM_LIST_MAX; ++i)
        if(skill_id == character_skill_rom_table[char_id].skill[i])
            return 1;

    // judge class
    for (i = 0; i < SKILL_ROM_LIST_MAX; ++i)
        if(skill_id == class_skill_rom_table[class_id].skill[i])
            return 1;
    
    return 0;
}

static int judge_skill_common(struct Unit *unit, const u8 skill_id)
{
    int ret = 0;

    // judge rom
    ret = judge_skill_common_rom_list(unit, skill_id);
    if (ret)
        return ret;

    // judge ram (WIP)

    return ret;
}

/*
 * fast judge
 */

struct skill_fast_list *get_or_create_skill_fast_list(struct Unit* unit)
{
    struct skill_fast_list *list;
    int i;

    /* judge whether the fast list exists */
    if (skill_fast_list0.unit_index == unit->index)
        return &skill_fast_list0;
    
    if (skill_fast_list1.unit_index == unit->index)
        return &skill_fast_list1;
    
    /* merge new list */
    list = &gBattleTarget.unit != unit
         ? &skill_fast_list0
         : &skill_fast_list1;
    
    /* build list */
    list->unit_index = unit->index;
    list->len = 0;

    for (i = 1; i < skill_index_ff; ++i) {
        if (1 == judge_skill_common(unit, i))
            list->skill[list->len++] = i;

        if (list->len >= SKILL_FAST_LIST_LEN)
            break;
    }

    return list;
}

static int judge_skill_fast_list(struct Unit* unit, const u8 skill_id)
{
    int i;
    struct skill_fast_list *list;

    if (!SKILL_VALID(skill_id))
        return 0;

    list = get_or_create_skill_fast_list(unit);

    for (i = 0; i < list->len; ++i)
        if (skill_id == list->skill[i])
            return 1;

    return 0;
}


/**
 * Misc
 */
void init_skill_system()
{
    judge_skill_func = (void *)((uintptr_t)judge_skill_fast_list + 1);

    skill_fast_list0.unit_index = 0;
    skill_fast_list0.len = 0;
    skill_fast_list1.unit_index = 0;
    skill_fast_list1.len = 0;
}
