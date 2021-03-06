/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "halls_of_reflection.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
    
enum Yells
{
    SAY_JAINA_INTRO_1                   = 0,
    SAY_JAINA_INTRO_2                   = 1,
    SAY_JAINA_INTRO_3                   = 2,
    SAY_JAINA_INTRO_4                   = 3,
    SAY_JAINA_INTRO_5                   = 4,
    SAY_JAINA_INTRO_6                   = 5,
    SAY_JAINA_INTRO_7                   = 6,
    SAY_JAINA_INTRO_8                   = 7,
    SAY_JAINA_INTRO_9                   = 8,
    SAY_JAINA_INTRO_10                  = 9,
    SAY_JAINA_INTRO_11                  = 10,
    SAY_JAINA_INTRO_END                 = 11,

    SAY_SYLVANAS_INTRO_1                = 0,
    SAY_SYLVANAS_INTRO_2                = 1,
    SAY_SYLVANAS_INTRO_3                = 2,
    SAY_SYLVANAS_INTRO_4                = 3,
    SAY_SYLVANAS_INTRO_5                = 4,
    SAY_SYLVANAS_INTRO_6                = 5,
    SAY_SYLVANAS_INTRO_7                = 6,
    SAY_SYLVANAS_INTRO_8                = 7,
    SAY_SYLVANAS_INTRO_END              = 8,

    SAY_UTHER_INTRO_A2_1                = 0,
    SAY_UTHER_INTRO_A2_2                = 1,
    SAY_UTHER_INTRO_A2_3                = 2,
    SAY_UTHER_INTRO_A2_4                = 3,
    SAY_UTHER_INTRO_A2_5                = 4,
    SAY_UTHER_INTRO_A2_6                = 5,
    SAY_UTHER_INTRO_A2_7                = 6,
    SAY_UTHER_INTRO_A2_8                = 7,
    SAY_UTHER_INTRO_A2_9                = 8,
    SAY_UTHER_INTRO_H2_1                = 9,
    SAY_UTHER_INTRO_H2_2                = 10,
    SAY_UTHER_INTRO_H2_3                = 11,
    SAY_UTHER_INTRO_H2_4                = 12,
    SAY_UTHER_INTRO_H2_5                = 13,
    SAY_UTHER_INTRO_H2_6                = 14,
    SAY_UTHER_INTRO_H2_7                = 15,

    SAY_LK_INTRO_1                      = 0,
    SAY_LK_INTRO_2                      = 1,
    SAY_LK_INTRO_3                      = 2,

    SAY_FALRIC_INTRO_1                  = 5,
    SAY_FALRIC_INTRO_2                  = 6,

    SAY_MARWYN_INTRO_1                  = 4,

    SAY_FROSTSWORN_AGGRO                = 0,
    SAY_FROSTSWORN_DEATH                = 1,
};

enum Events
{
    EVENT_NONE,

    EVENT_START_INTRO,
    EVENT_SKIP_INTRO,

    EVENT_INTRO_A2_1,
    EVENT_INTRO_A2_2,
    EVENT_INTRO_A2_3,
    EVENT_INTRO_A2_3_1,
    EVENT_INTRO_A2_3_2,
    EVENT_INTRO_A2_4,
    EVENT_INTRO_A2_5,
    EVENT_INTRO_A2_6,
    EVENT_INTRO_A2_7,
    EVENT_INTRO_A2_8,
    EVENT_INTRO_A2_9,
    EVENT_INTRO_A2_10,
    EVENT_INTRO_A2_11,
    EVENT_INTRO_A2_12,
    EVENT_INTRO_A2_13,
    EVENT_INTRO_A2_14,
    EVENT_INTRO_A2_15,
    EVENT_INTRO_A2_16,
    EVENT_INTRO_A2_17,
    EVENT_INTRO_A2_18,
    EVENT_INTRO_A2_19,

    EVENT_INTRO_H2_1,
    EVENT_INTRO_H2_2,
    EVENT_INTRO_H2_3,
    EVENT_INTRO_H2_4,
    EVENT_INTRO_H2_5,
    EVENT_INTRO_H2_6,
    EVENT_INTRO_H2_7,
    EVENT_INTRO_H2_8,
    EVENT_INTRO_H2_9,
    EVENT_INTRO_H2_10,
    EVENT_INTRO_H2_11,
    EVENT_INTRO_H2_12,
    EVENT_INTRO_H2_13,
    EVENT_INTRO_H2_14,
    EVENT_INTRO_H2_15,

    EVENT_INTRO_LK_1,
    EVENT_INTRO_LK_2,
    EVENT_INTRO_LK_2_1,
    EVENT_INTRO_LK_3,
    EVENT_INTRO_LK_4,
    EVENT_INTRO_LK_5,
    EVENT_INTRO_LK_6,
    EVENT_INTRO_LK_7,
    EVENT_INTRO_LK_8,
    EVENT_INTRO_LK_9,

    EVENT_INTRO_END,
};

enum eEnum
{
    ACTION_START_INTRO,
    ACTION_SKIP_INTRO,

    QUEST_DELIVRANCE_FROM_THE_PIT_A2              = 24710,
    QUEST_DELIVRANCE_FROM_THE_PIT_H2              = 24712,
    QUEST_WRATH_OF_THE_LICH_KING_A2               = 24500,
    QUEST_WRATH_OF_THE_LICH_KING_H2               = 24802,
};

enum Spells
{
    SPELL_CAST_VISUAL                  = 65633, //Jaina/Sylavana lo lanzan para invocar a uther
    SPELL_BOSS_SPAWN_AURA              = 72712, //Falric and Marwyn
    SPELL_UTHER_DESPAWN                = 70693,
    SPELL_TAKE_FROSTMOURNE             = 72729,
    SPELL_FROSTMOURNE_DESPAWN          = 72726,
    SPELL_FROSTMOURNE_VISUAL           = 73220,
    SPELL_FROSTMOURNE_SOUNDS           = 70667,

    //Frostsworn's Speels
    SPELL_SHIELD_THROWN                = 69222,
    H_SPELL_SHIELD_THROWN              = 73076,
    SPELL_SPIKE                        = 69184,
    H_SPELL_SPIKE                      = 70399,
    SPELL_CLONE_NAME                   = 57507,
    SPELL_CLONE_MODEL                  = 45204,

    // Reflection's Spells
    SPELL_BALEFUL_STRIKE               = 69933,
    SPELL_SPIRIT_BURST                 = 69900,
    H_SPELL_BALEFUL_STRIKE             = 70400,
    H_SPELL_SPIRIT_BURST               = 73046,
};

const Position HallsofReflectionLocs[]=
{
    {5283.234863f, 1990.946777f, 707.695679f, 0.929097f},   // 2 Loralen Follows
    {5408.031250f, 2102.918213f, 707.695251f, 0.792756f},   // 9 Sylvanas Follows
    {5401.866699f, 2110.837402f, 707.695251f, 0.800610f},   // 10 Loralen follows
};

const Position SpawnPos              = {5262.540527f, 1949.693726f, 707.695007f, 0.808736f}; // Jaina/Sylvanas Beginning Position
const Position MoveThronePos         = {5306.952148f, 1998.499023f, 709.341431f, 1.277278f}; // Jaina/Sylvanas walks to throne
const Position UtherSpawnPos         = {5308.310059f, 2003.857178f, 709.341431f, 4.650315f};
const Position LichKingSpawnPos      = {5362.917480f, 2062.307129f, 707.695374f, 3.945812f};
const Position LichKingMoveThronePos = {5312.080566f, 2009.172119f, 709.341431f, 3.973301f}; // Lich King walks to throne
const Position LichKingMoveAwayPos   = {5400.069824f, 2102.7131689f, 707.69525f, 0.843803f}; // Lich King walks away
const Position PartnerRunPos         = {5292.298340f, 2015.797119f, 707.695008f, 0.858684f}; // partner walks away

class npc_jaina_and_sylvanas_hor : public CreatureScript
{
public:
    npc_jaina_and_sylvanas_hor() : CreatureScript("npc_jaina_and_sylvanas_hor") {}

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();

        if (creature->AI())
        {
            creature->AI()->DoAction(ACTION_START_INTRO);
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM( 0, "Can you remove the sword?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_jaina_and_sylvanas_horAI(creature);
    }

    // AI of Part1: handle the intro till start of gauntlet event.
    struct npc_jaina_and_sylvanas_horAI : public ScriptedAI
    {
        npc_jaina_and_sylvanas_horAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;
        uint64 _utherGuid;
        uint64 _lichkingGuid;
        EventMap events;

        void Reset()
        {
            events.Reset();

            _utherGuid = 0;
            _lichkingGuid = 0;

            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetVisible(true);
        }

        void DoAction(int32 actionId)
        {
            switch (actionId)
            {
                case ACTION_START_INTRO:
                    events.ScheduleEvent(EVENT_START_INTRO, 0);
                    break;
                case ACTION_SKIP_INTRO:
                    events.ScheduleEvent(EVENT_SKIP_INTRO, 0);
                    break;
            }
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            switch (events.ExecuteEvent())
            {
                case EVENT_START_INTRO:
                    me->GetMotionMaster()->MovePoint(0, MoveThronePos);
                    // Begining of intro is differents between fActions as the speech sequence and timers are differents.
                    if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                        events.ScheduleEvent(EVENT_INTRO_A2_1, 0);
                    else
                        events.ScheduleEvent(EVENT_INTRO_H2_1, 0);
                    break;
            // A2 Intro Events
                case EVENT_INTRO_A2_1:
                    Talk(SAY_JAINA_INTRO_1);
                    events.ScheduleEvent(EVENT_INTRO_A2_2, 6000);
                    break;
                case EVENT_INTRO_A2_2:
                    Talk(SAY_JAINA_INTRO_2);
                    events.ScheduleEvent(EVENT_INTRO_A2_3, 8000);
                    break;
                case EVENT_INTRO_A2_3:
                    Talk(SAY_JAINA_INTRO_3);
                    events.ScheduleEvent(EVENT_INTRO_A2_3_1, 8000);
                    break;
                 case EVENT_INTRO_A2_3_1:
                    Talk(SAY_JAINA_INTRO_4);
                    events.ScheduleEvent(EVENT_INTRO_A2_3_2, 9000);
                    break;
                case EVENT_INTRO_A2_3_2:
                    DoCast(me, SPELL_CAST_VISUAL);
                    me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
                    instance->HandleGameObject(instance->GetData64(DATA_FROSTMOURNE), true);
                    events.ScheduleEvent(EVENT_INTRO_A2_4, 3000);
                    break;
                case EVENT_INTRO_A2_4:
                    // spawn UTHER during speach 2
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->CastSpell(uther, SPELL_BOSS_SPAWN_AURA, true);
                        uther->GetMotionMaster()->MoveIdle();
                        uther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
                        _utherGuid = uther->GetGUID();
                    }
                    events.ScheduleEvent(EVENT_INTRO_A2_5, 2000);
                    break;
                case EVENT_INTRO_A2_5:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_1);
                    events.ScheduleEvent(EVENT_INTRO_A2_6, 3000);
                    break;
                case EVENT_INTRO_A2_6:
                    Talk(SAY_JAINA_INTRO_5);
                    events.ScheduleEvent(EVENT_INTRO_A2_7, 6000);
                    break;
                case EVENT_INTRO_A2_7:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_2);
                    events.ScheduleEvent(EVENT_INTRO_A2_8, 6500);
                    break;
                case EVENT_INTRO_A2_8:
                    Talk(SAY_JAINA_INTRO_6);
                    events.ScheduleEvent(EVENT_INTRO_A2_9, 2000);
                    break;
                case EVENT_INTRO_A2_9:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_3);
                    events.ScheduleEvent(EVENT_INTRO_A2_10, 9000);
                    break;
                case EVENT_INTRO_A2_10:
                    Talk(SAY_JAINA_INTRO_7);
                    events.ScheduleEvent(EVENT_INTRO_A2_11, 5000);
                    break;
                case EVENT_INTRO_A2_11:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_4);
                    events.ScheduleEvent(EVENT_INTRO_A2_12, 11000);
                    break;
                case EVENT_INTRO_A2_12:
                    Talk(SAY_JAINA_INTRO_8);
                    events.ScheduleEvent(EVENT_INTRO_A2_13, 4000);
                    break;
                case EVENT_INTRO_A2_13:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_5);
                    events.ScheduleEvent(EVENT_INTRO_A2_14, 12500);
                    break;
                case EVENT_INTRO_A2_14:
                    Talk(SAY_JAINA_INTRO_9);
                    events.ScheduleEvent(EVENT_INTRO_A2_15, 10000);
                    break;
                case EVENT_INTRO_A2_15:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_6);
                    events.ScheduleEvent(EVENT_INTRO_A2_16, 22000);
                    break;
                case EVENT_INTRO_A2_16:                    
                    events.ScheduleEvent(EVENT_INTRO_A2_17, 4000);
                    break;
                case EVENT_INTRO_A2_17:
                    Talk(SAY_JAINA_INTRO_10);
                    events.ScheduleEvent(EVENT_INTRO_A2_18, 2000);
                    break;
                case EVENT_INTRO_A2_18:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                    {
                        uther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_8);
                    }
                    events.ScheduleEvent(EVENT_INTRO_A2_19, 13000);
                    break;
                case EVENT_INTRO_A2_19:
                    Talk(SAY_JAINA_INTRO_11);
                    events.ScheduleEvent(EVENT_INTRO_LK_1, 2000);
                    break;
             // H2 Intro Event
                case EVENT_INTRO_H2_1:
                    Talk(SAY_SYLVANAS_INTRO_1);
                    events.ScheduleEvent(EVENT_INTRO_H2_2,14000);
                    break;
                case EVENT_INTRO_H2_2:
                    Talk(SAY_SYLVANAS_INTRO_2);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                    events.ScheduleEvent(EVENT_INTRO_H2_3, 7000);
                    break;
                case EVENT_INTRO_H2_3:
                    Talk(SAY_SYLVANAS_INTRO_3);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                    events.ScheduleEvent(EVENT_INTRO_H2_4, 5000);
                    break;
                case EVENT_INTRO_H2_4:
                    DoCast(me, SPELL_CAST_VISUAL);
                    instance->HandleGameObject(instance->GetData64(DATA_FROSTMOURNE), true);
                    me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
                    events.ScheduleEvent(EVENT_INTRO_H2_5, 4000);
                    break;
                case EVENT_INTRO_H2_5:
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->CastSpell(uther, SPELL_BOSS_SPAWN_AURA, true);
                        uther->GetMotionMaster()->MoveIdle();
                        uther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
                        _utherGuid = uther->GetGUID();
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_1);
                    }
                    events.ScheduleEvent(EVENT_INTRO_H2_6, 9500);
                    break;
                case EVENT_INTRO_H2_6:
                    Talk(SAY_SYLVANAS_INTRO_4);
                    events.ScheduleEvent(EVENT_INTRO_H2_7, 3500);
                    break;
                case EVENT_INTRO_H2_7:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_2);
                    events.ScheduleEvent(EVENT_INTRO_H2_8, 7000);
                    break;
                case EVENT_INTRO_H2_8:
                    Talk(SAY_SYLVANAS_INTRO_5);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                    events.ScheduleEvent(EVENT_INTRO_H2_9, 5000);
                    break;
                case EVENT_INTRO_H2_9:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_3);
                    events.ScheduleEvent(EVENT_INTRO_H2_10, 19000);
                    break;
                case EVENT_INTRO_H2_10:
                    Talk(SAY_SYLVANAS_INTRO_6);
                    events.ScheduleEvent(EVENT_INTRO_H2_11, 1500);
                    break;
                case EVENT_INTRO_H2_11:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_4);
                    events.ScheduleEvent(EVENT_INTRO_H2_12, 19500);
                    break;
                case EVENT_INTRO_H2_12:
                    Talk(SAY_SYLVANAS_INTRO_7);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                    events.ScheduleEvent(EVENT_INTRO_H2_13, 2000);
                    break;
                case EVENT_INTRO_H2_13:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                    {
                        uther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_5);
                    }
                    events.ScheduleEvent(EVENT_INTRO_H2_14, 12000);
                    break;
                case EVENT_INTRO_H2_14:
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_6);
                    events.ScheduleEvent(EVENT_INTRO_H2_15, 8000);
                    break;
                case EVENT_INTRO_H2_15:
                    Talk(SAY_SYLVANAS_INTRO_8);
                    events.ScheduleEvent(EVENT_INTRO_LK_1,3000);
                    break;
            // Remaining Intro Events common for both faction
                case EVENT_INTRO_LK_1:                   
                    // Spawn LK in front of door, and make him move to the sword.
                    if (Creature* lichking = me->SummonCreature(NPC_LICH_KING_EVENT, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        lichking->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                        lichking->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
                        lichking->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
                        lichking->SetReactState(REACT_PASSIVE);
                        _lichkingGuid = lichking->GetGUID();
                    }
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(DATA_FROSTSWORN_DOOR)))
                           gate->SetGoState(GO_STATE_ACTIVE);
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                    {
                        if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            uther->AI()->Talk(SAY_UTHER_INTRO_A2_9);
                        else
                            uther->AI()->Talk(SAY_UTHER_INTRO_H2_7);
                    }
                    events.ScheduleEvent(EVENT_INTRO_LK_2_1, 2000);
                    break;
                case EVENT_INTRO_LK_2_1:
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(DATA_FROSTSWORN_DOOR)))
                        gate->SetGoState(GO_STATE_READY);
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                    {
                        uther->SetUInt64Value(UNIT_FIELD_TARGET, (*me, _lichkingGuid));
                        uther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                    }
                    events.ScheduleEvent(EVENT_INTRO_LK_2, 7000);
                    break;
                case EVENT_INTRO_LK_2:
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGuid))
                    {
                        lichking->AI()->Talk(SAY_LK_INTRO_1);
                        lichking->HandleEmoteCommand(EMOTE_STATE_ROAR);
                    }
                    events.ScheduleEvent(EVENT_INTRO_LK_3, 3000);
                    break;
                case EVENT_INTRO_LK_3:
                     // The Lich King banishes Uther to the abyss.
                    if (Creature* uther = me->GetCreature(*me, _utherGuid))
                    {
                        uther->CastSpell( uther, SPELL_UTHER_DESPAWN, true);
                        uther->DisappearAndDie();
                        _utherGuid = 0;
                        me->RemoveAllAuras();
                    }
                    // He steps forward and removes the runeblade from the heap of skulls.
                    events.ScheduleEvent(EVENT_INTRO_LK_4, 4000);
                    break;
                case EVENT_INTRO_LK_4:
                    if (GameObject *frostmourne = me->FindNearestGameObject(GO_FROSTMOURNE, 11.0f))
                        frostmourne->SetPhaseMask(0,true);
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGuid))
                    {
                        lichking->CastSpell(lichking, SPELL_TAKE_FROSTMOURNE, true);
                        lichking->CastSpell(lichking, SPELL_FROSTMOURNE_VISUAL, true);
                    }
                    me->RemoveAllAuras();
                    events.ScheduleEvent(EVENT_INTRO_LK_5, 3500);
                    break;
                case EVENT_INTRO_LK_5:
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGuid))
                    {
                        lichking->AI()->Talk(SAY_LK_INTRO_2);
                        lichking->HandleEmoteCommand(EMOTE_STATE_POINT);
                    }
                    events.ScheduleEvent(EVENT_INTRO_LK_6, 11000);
                    break;
                case EVENT_INTRO_LK_6:
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGuid))
                        lichking->AI()->Talk(SAY_LK_INTRO_3);
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGuid))
                    {
                        lichking->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                        lichking->GetMotionMaster()->MovePoint(1, LichKingSpawnPos);
                    }
                    if (Creature* loralen = me->GetCreature(*me, instance->GetData64(DATA_LORALEN)))
                    {
                        loralen->SetUnitMovementFlags(MOVEMENTFLAG_NONE);
                        loralen->GetMotionMaster()->MovePoint(0, LichKingSpawnPos);
                    }
                    events.ScheduleEvent(EVENT_INTRO_LK_7, 9000);
                    break;
                case EVENT_INTRO_LK_7:                   
                    if (Creature* falric = me->GetCreature(*me, instance->GetData64(DATA_FALRIC)))
                    {
                        falric->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
                        falric->SetVisible(true);
                        falric->CastSpell(falric, SPELL_BOSS_SPAWN_AURA, true);
                        falric->AI()->Talk(SAY_FALRIC_INTRO_1);
                        falric->HandleEmoteCommand(EMOTE_ONESHOT_YES);
                    }
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(DATA_FROSTSWORN_DOOR)))
                        gate->SetGoState(GO_STATE_ACTIVE);
                    if (Creature* loralen = me->GetCreature(*me, instance->GetData64(DATA_LORALEN)))
                    {
                        loralen->SetUnitMovementFlags(MOVEMENTFLAG_NONE);
                        loralen->GetMotionMaster()->MovePoint(0, PartnerRunPos);
                    }
                    events.ScheduleEvent(EVENT_INTRO_LK_8, 2500);
                    break;
                case EVENT_INTRO_LK_8:
                    if (Creature* marwyn = me->GetCreature(*me, instance->GetData64(DATA_MARWYN)))
                    {
                        marwyn->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
                        marwyn->SetVisible(true);
                        marwyn->CastSpell(marwyn, SPELL_BOSS_SPAWN_AURA, true);
                        marwyn->AI()->Talk(SAY_MARWYN_INTRO_1);
                        marwyn->HandleEmoteCommand(EMOTE_ONESHOT_YES);
                    }
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(DATA_FROSTSWORN_DOOR)))
                        gate->SetGoState(GO_STATE_READY);
                    events.ScheduleEvent(EVENT_INTRO_LK_9, 2500);
                    break;
                case EVENT_INTRO_LK_9:                   
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGuid))
                    {
                        lichking->CastSpell( lichking, SPELL_UTHER_DESPAWN, true);
                        lichking->DisappearAndDie();
                    }
                    me->GetMotionMaster()->MovePoint(0, LichKingSpawnPos);
                    if (Creature* loralen = me->GetCreature(*me, instance->GetData64(DATA_LORALEN)))
                        loralen->GetMotionMaster()->MovePoint(0, LichKingSpawnPos);

                    if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                        Talk(SAY_JAINA_INTRO_END);
                    else
                        Talk(SAY_SYLVANAS_INTRO_END);
                    events.ScheduleEvent(EVENT_SKIP_INTRO, 8000);
                    break;
                case EVENT_SKIP_INTRO:
                    if (Creature* falric = me->GetCreature(*me, instance->GetData64(DATA_FALRIC)))
                        falric->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                    if (Creature* marwyn = me->GetCreature(*me, instance->GetData64(DATA_MARWYN)))
                        marwyn->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                    if (instance)
                    {
                        instance->SetData(DATA_WAVE_COUNT, SPECIAL);  // start first wave
                        instance->SetData(DATA_WAVES_BEGIN, DONE); //if all players die waves restart when they get Rooms center
                    }
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(DATA_FROSTSWORN_DOOR)))
                        gate->SetGoState(GO_STATE_ACTIVE);
                    events.ScheduleEvent(EVENT_INTRO_END, 4000);
                    break;
                case EVENT_INTRO_END:
                    if (Creature* falric = me->GetCreature(*me, instance->GetData64(DATA_FALRIC)))
                    {
                        falric->AI()->Talk(SAY_FALRIC_INTRO_2);
                        falric->HandleEmoteCommand(EMOTE_STATE_ROAR);
                    }
                    me->CastSpell( me, SPELL_UTHER_DESPAWN, true);
                    me->DisappearAndDie();
                    if (Creature* loralen = me->GetCreature(*me, instance->GetData64(DATA_LORALEN)))
                        loralen->DisappearAndDie();
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(DATA_FROSTSWORN_DOOR)))
                        gate->SetGoState(GO_STATE_READY);
                    break;
            }
        }
    };
};

enum TrashSpells
{
    // Ghostly Priest
    SPELL_SHADOW_WORD_PAIN                        = 72318,
    SPELL_CIRCLE_OF_DESTRUCTION                   = 72320,
    SPELL_COWER_IN_FEAR                           = 72321,
    SPELL_DARK_MENDING                            = 72322,

    // Phantom Mage
    SPELL_FIREBALL                                = 72163,
    SPELL_FLAMESTRIKE                             = 72169,
    SPELL_FROSTBOLT                               = 72166,
    SPELL_CHAINS_OF_ICE                           = 72121,
    SPELL_HALLUCINATION                           = 72342,

    // Phantom Hallucination (same as phantom mage + HALLUCINATION_2 when dies)
    SPELL_HALLUCINATION_2                         = 72344,

    // Shadowy Mercenary
    SPELL_SHADOW_STEP                             = 72326,
    SPELL_DEADLY_POISON                           = 72329,
    SPELL_ENVENOMED_DAGGER_THROW                  = 72333,
    SPELL_KIDNEY_SHOT                             = 72335,

    // Spectral Footman
    SPELL_SPECTRAL_STRIKE                         = 72198,
    SPELL_SHIELD_BASH                             = 72194,
    SPELL_TORTURED_ENRAGE                         = 72203,

    // Tortured Rifleman
    SPELL_SHOOT                                   = 72208,
    SPELL_CURSED_ARROW                            = 72222,
    SPELL_FROST_TRAP                              = 72215,
    SPELL_ICE_SHOT                                = 72268,
};

enum TrashEvents
{
    EVENT_TRASH_NONE,

    // Ghostly Priest
    EVENT_SHADOW_WORD_PAIN,
    EVENT_CIRCLE_OF_DESTRUCTION,
    EVENT_COWER_IN_FEAR,
    EVENT_DARK_MENDING,

    // Phantom Mage
    EVENT_FIREBALL,
    EVENT_FROSTBOLT,
    EVENT_CHAINS_OF_ICE,
    EVENT_HALLUCINATION,

    // Shadowy Mercenary
    EVENT_SHADOW_STEP,
    EVENT_DEADLY_POISON,
    EVENT_ENVENOMED_DAGGER_THROW,
    EVENT_KIDNEY_SHOT,

    // Spectral Footman
    EVENT_SPECTRAL_STRIKE,
    EVENT_SHIELD_BASH,
    EVENT_TORTURED_ENRAGE,

    // Tortured Rifleman
    EVENT_SHOOT,
    EVENT_CURSED_ARROW,
    EVENT_FROST_TRAP,
    EVENT_ICE_SHOT,
};

class npc_ghostly_priest : public CreatureScript
{
public:
    npc_ghostly_priest() : CreatureScript("npc_ghostly_priest") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ghostly_priestAI(creature);
    }

    struct npc_ghostly_priestAI: public ScriptedAI
    {
        npc_ghostly_priestAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
            events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
            events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHADOW_WORD_PAIN:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_SHADOW_WORD_PAIN);
                        events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000);
                        return;
                    case EVENT_CIRCLE_OF_DESTRUCTION:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_CIRCLE_OF_DESTRUCTION);
                        events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
                        return;
                    case EVENT_COWER_IN_FEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_COWER_IN_FEAR);
                        events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
                        return;
                    case EVENT_DARK_MENDING:
                        // find an ally with missing HP
                        if (Unit* target = DoSelectLowestHpFriendly(40, DUNGEON_MODE(30000, 50000)))
                        {
                            DoCast(target, SPELL_DARK_MENDING);
                            events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
                        }
                        else
                            // no friendly unit with missing hp. re-check in just 5 sec.
                            events.ScheduleEvent(EVENT_DARK_MENDING, 5000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class npc_phantom_mage : public CreatureScript
{
public:
    npc_phantom_mage() : CreatureScript("npc_phantom_mage") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_phantom_mageAI(creature);
    }

    struct npc_phantom_mageAI: public ScriptedAI
    {
        npc_phantom_mageAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_FIREBALL, 3000); // TODO: adjust timers            
            events.ScheduleEvent(EVENT_FROSTBOLT, 9000);
            events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 12000);
            events.ScheduleEvent(EVENT_HALLUCINATION, 40000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIREBALL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_FIREBALL);
                        events.ScheduleEvent(EVENT_FIREBALL, 15000);
                        return;
                    case EVENT_FROSTBOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_FROSTBOLT);
                        events.ScheduleEvent(EVENT_FROSTBOLT, 15000);
                        return;
                    case EVENT_CHAINS_OF_ICE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_CHAINS_OF_ICE);
                        events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 15000);
                        return;
                    case EVENT_HALLUCINATION:
                        DoCast(SPELL_HALLUCINATION);
                        return;
                }
            }
            DoSpellAttackIfReady(SPELL_FLAMESTRIKE);
        }
    };
};

class npc_phantom_hallucination : public CreatureScript
{
public:
    npc_phantom_hallucination() : CreatureScript("npc_phantom_hallucination") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_phantom_hallucinationAI(creature);
    }

    struct npc_phantom_hallucinationAI : public npc_phantom_mage::npc_phantom_mageAI
    {
        npc_phantom_hallucinationAI(Creature* creature) : npc_phantom_mage::npc_phantom_mageAI(creature)
        {
        }

        void JustDied(Unit* /*killer*/)
        {
            DoCast(SPELL_HALLUCINATION_2);
        }
    };

};

class npc_shadowy_mercenary : public CreatureScript
{
public:
    npc_shadowy_mercenary() : CreatureScript("npc_shadowy_mercenary") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shadowy_mercenaryAI(creature);
    }

    struct npc_shadowy_mercenaryAI: public ScriptedAI
    {
        npc_shadowy_mercenaryAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SHADOW_STEP, 8000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
            events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
            events.ScheduleEvent(EVENT_KIDNEY_SHOT, 12000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHADOW_STEP:
                        DoCast(SPELL_SHADOW_STEP);
                        events.ScheduleEvent(EVENT_SHADOW_STEP, 8000);
                        return;
                    case EVENT_DEADLY_POISON:
                        DoCast(me->GetVictim(), SPELL_DEADLY_POISON);
                        events.ScheduleEvent(EVENT_DEADLY_POISON, 10000);
                        return;
                    case EVENT_ENVENOMED_DAGGER_THROW:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_ENVENOMED_DAGGER_THROW);
                        events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
                        return;
                    case EVENT_KIDNEY_SHOT:
                        DoCast(me->GetVictim(), SPELL_KIDNEY_SHOT);
                        events.ScheduleEvent(EVENT_KIDNEY_SHOT, 10000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class npc_spectral_footman : public CreatureScript
{
public:
    npc_spectral_footman() : CreatureScript("npc_spectral_footman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_spectral_footmanAI(creature);
    }

    struct npc_spectral_footmanAI: public ScriptedAI
    {
        npc_spectral_footmanAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_SHIELD_BASH, 10000);
            events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPECTRAL_STRIKE:
                        DoCast(me->GetVictim(), SPELL_SPECTRAL_STRIKE);
                        events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000);
                        return;
                    case EVENT_SHIELD_BASH:
                        DoCast(me->GetVictim(), SPELL_SHIELD_BASH);
                        events.ScheduleEvent(EVENT_SHIELD_BASH, 5000);
                        return;
                    case EVENT_TORTURED_ENRAGE:
                        DoCast(SPELL_TORTURED_ENRAGE);
                        events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class npc_tortured_rifleman : public CreatureScript
{
public:
    npc_tortured_rifleman() : CreatureScript("npc_tortured_rifleman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_tortured_riflemanAI(creature);
    }

    struct npc_tortured_riflemanAI  : public ScriptedAI
    {
        npc_tortured_riflemanAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SHOOT, 2000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_CURSED_ARROW, 10000);
            events.ScheduleEvent(EVENT_FROST_TRAP, 1000);
            events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHOOT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_SHOOT);
                        events.ScheduleEvent(EVENT_SHOOT, 2000);
                        return;
                    case EVENT_CURSED_ARROW:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_CURSED_ARROW);
                        events.ScheduleEvent(EVENT_CURSED_ARROW, 10000);
                        return;
                    case EVENT_FROST_TRAP:
                        DoCast(SPELL_FROST_TRAP);
                        events.ScheduleEvent(EVENT_FROST_TRAP, 30000);
                        return;
                    case EVENT_ICE_SHOT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_ICE_SHOT);
                        events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};


class npc_frostsworn_general : public CreatureScript
{
public:
    npc_frostsworn_general() : CreatureScript("npc_frostsworn_general") { }

    struct npc_frostsworn_generalAI : public ScriptedAI
    {
        npc_frostsworn_generalAI(Creature *creature) : ScriptedAI(creature)
        {
            instance = (InstanceScript*)creature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        uint32 _shieldTimer;
        uint32 _spikeTimer;
        uint32 _cloneTimer;

        void Reset()
        {
            if (!instance)
                return;

            _shieldTimer = 5000;
            _spikeTimer = 14000;
            _cloneTimer = 22000;

            instance->SetData(DATA_FROSTSWORN_EVENT, NOT_STARTED);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void JustDied(Unit* /*Killer*/)
        {
            if (!instance)
                return;

            Talk(SAY_FROSTSWORN_DEATH);
            instance->SetData(DATA_FROSTSWORN_EVENT, DONE);
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!instance)
                return;

            if (me->GetVictim())
                return;

            if (who->GetTypeId() != TYPEID_PLAYER || instance->GetData(DATA_MARWYN_EVENT) != DONE|| !me->IsWithinDistInMap(who, 20.0f)) 
                return;

            if (Player* player = who->ToPlayer())
                if (player->IsGameMaster())
                    return;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            AttackStart(who);
        }

        void EnterCombat(Unit* /*Victim*/)
        {
            if (!instance)
                return;

            Talk(SAY_FROSTSWORN_AGGRO);
            instance->SetData(DATA_FROSTSWORN_EVENT, IN_PROGRESS);
        }

        void UpdateAI(uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (_shieldTimer < uiDiff)
            {
                if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                     DoCast(target, SPELL_SHIELD_THROWN);
                _shieldTimer = urand(5000, 15000);
            }
            else
                _shieldTimer -= uiDiff;

            if (_spikeTimer < uiDiff)
            {
                if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SPIKE);
                _spikeTimer = urand(14000, 25000);
            }
            else
                _spikeTimer -= uiDiff;

            if (_cloneTimer < uiDiff)
            {
                SummonClones();
                _cloneTimer = 60000;
            }
            else
                _cloneTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

        void SummonClones()
        {
            std::list<Unit *> playerList;
            SelectTargetList(playerList, 5, SELECT_TARGET_TOPAGGRO, 0, true);
            for (std::list<Unit*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
            {
                Unit* temp = (*itr);
                Creature* reflection = me->SummonCreature(NPC_REFLECTION, temp->GetPositionX(), temp->GetPositionY(), temp->GetPositionZ(), temp->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                reflection->SetName(temp->GetName());
                temp->CastSpell(reflection, SPELL_CLONE_NAME, true);
                temp->CastSpell(reflection, SPELL_CLONE_MODEL, true);
                reflection->setFaction(me->getFaction());
                reflection->AI()->AttackStart(temp);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_frostsworn_generalAI(creature);
    }
};

class npc_spiritual_reflection : public CreatureScript
{
public:
    npc_spiritual_reflection() : CreatureScript("npc_spiritual_reflection") { }

    struct npc_spiritual_reflectionAI : public ScriptedAI
    {
        npc_spiritual_reflectionAI(Creature *creature) : ScriptedAI(creature)
        {
            Reset();
        }

        InstanceScript* instance;
        uint32 _StrikeTimer;

        void Reset()
        {
            _StrikeTimer = urand(1000,3000);
        }

        void JustDied(Unit* Killer)
        {
            DoCast(Killer, SPELL_SPIRIT_BURST);
        }

        void UpdateAI(uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (_StrikeTimer < uiDiff)
            {
                if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_BALEFUL_STRIKE);
                _StrikeTimer = urand(3000, 8000);
            }
            else
                _StrikeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_spiritual_reflectionAI(creature);
    }
};

enum Defs
{
    SAY_LICH_KING_AGGRO_A    = 0,
    SAY_LICH_KING_AGGRO_H    = 1,
    SAY_LICH_KING_WALL_02    = 3,
    SAY_LICH_KING_WALL_03    = 4,
    SAY_LICH_KING_WALL_04    = 5,

    SAY_JAINA_AGGRO          = 0,
    SAY_JAINA_WALL_01        = 1,
    SAY_JAINA_WALL_02        = 2,
    SAY_JAINA_WALL_03        = 3,
    SAY_JAINA_WALL_04        = 4,
    SAY_JAINA_ESCAPE_01      = 5,
    SAY_JAINA_ESCAPE_02      = 6,
    SAY_JAINA_TRAP           = 7,
    SAY_JAINA_FINAL_1        = 8,
    SAY_JAINA_FINAL_2        = 9,
    SAY_JAINA_FINAL_3        = 8,

    SAY_SYLVANAS_AGGRO       = 0,
    SAY_SYLVANAS_WALL_01     = 1,
    SAY_SYLVANAS_WALL_02     = 2,
    SAY_SYLVANAS_WALL_03     = 3,
    SAY_SYLVANAS_WALL_04     = 4,
    SAY_SYLVANAS_ESCAPE_01   = 5,
    SAY_SYLVANAS_ESCAPE_02   = 6,
    SAY_SYLVANAS_TRAP        = 7,
    SAY_SYLVANAS_FINAL_1     = 8,

    /*SPELLS AND VISUAL EFFECTS*/
    SPELL_WINTER                       = 69780,
    SPELL_FURY_OF_FROSTMOURNE          = 70063,
    SPELL_ICE_PRISON_VISUAL            = 69708,
    SPELL_DARK_ARROW                   = 70194,
    SPELL_ICE_BARRIER                  = 69787,
    SPELL_DESTROY_ICE_WALL_01          = 69784, //Jaina
    SPELL_DESTROY_ICE_WALL_03          = 70225, //Sylvana
    SPELL_SYLVANA_JUMP                 = 68339,
    SPELL_SYLVANA_STEP                 = 69087,
    SPELL_FIRE_CANNON                  = 67461,

    FACTION                            = 2076,
};

const Position CannonSpawns[4] =
{
    {5230.00f, 1658.75f, 802.22f, 0.00f},
    {5245.74f, 1644.44f, 802.35f, 0.00f},
    {5260.64f, 1636.07f, 802.16f, 0.00f},
    {5275.90f, 1633.22f, 802.25f, 0.00f},
};

class npc_jaina_and_sylvana_hor_part2 : public CreatureScript
{
public:
    npc_jaina_and_sylvana_hor_part2() : CreatureScript("npc_jaina_and_sylvana_hor_part2") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        InstanceScript* instance = (InstanceScript*)creature->GetInstanceScript();

        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->CLOSE_GOSSIP_MENU();
                ((npc_jaina_and_sylvana_hor_part2AI*)creature->AI())->Start(false,true);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                creature->setActive(true);
                if (instance)
                {
                    instance->SetData64(DATA_ESCAPE_LIDER, creature->GetGUID());
                    instance->SetData(DATA_LICHKING_EVENT, IN_PROGRESS);
                }
                return true;
            default:
                return false;
        }
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        InstanceScript*   instance = (InstanceScript*)creature->GetInstanceScript();

        if (!instance)
            return false;

        if (instance->GetData(DATA_LICHKING_EVENT) == DONE)
            return false;

        if (creature->IsQuestGiver())
            player->PrepareQuestMenu( creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We are ready!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_jaina_and_sylvana_hor_part2AI(creature);
    }

    struct npc_jaina_and_sylvana_hor_part2AI : public npc_escortAI
    {
        npc_jaina_and_sylvana_hor_part2AI(Creature *creature) : npc_escortAI(creature)
        {
            instance = (InstanceScript*)creature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        uint32 _castTimer;
        uint32 _stepTimer;
        uint32 _step_doCount;
        int32 _holdTimer;
        uint32 _doCount;
        bool _doFight;
        bool _didHappen;        
        bool _wallCast;
        uint64 _lichKingGuid;
        uint64 _liderGuid;
        uint64 _iceWallGuid;
        uint64 _wallTargetGuid;        
        Creature* _lichKing;
        uint32 _chestEntry;

        void Reset()
        {
            if (!instance)
                return;

            if (instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
                return;

            _step_doCount = 0;
            _stepTimer = 500;
            _doFight = true;
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            _wallTargetGuid = 0;

            if (me->GetEntry() == NPC_JAINA_PART2)
            {
                me->CastSpell(me, SPELL_ICE_BARRIER, false);
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
            }
            if (instance->GetData(DATA_LICHKING_EVENT) == DONE)
                me->SetVisible(false);
        }

        void AttackStart(Unit* who)
        {
            if (!who)
                return;

            if (instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS || _doFight != true)
                return;

            npc_escortAI::AttackStart(who);

        }

        void JustDied(Unit* /*pKiller*/)
        {
            if (!instance)
                return;
            instance->SetData(DATA_LICHKING_EVENT, FAIL);
        }

        void WaypointReached(uint32 i)
        {
            switch(i)
            {
                case 3:
                    instance->SetData(DATA_ICE_WALL_1, IN_PROGRESS);
                    if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(GO_ICE_WALL_1)))
                    {
                        gate->SetGoState(GO_STATE_READY);
                        _iceWallGuid = gate->GetGUID();
                    }
                    break;
                case 4:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_WALL_01);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_WALL_01);
                    _castTimer = 1000;
                    _holdTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature* walltarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        _wallTargetGuid = walltarget->GetGUID();
                        walltarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if (me->GetEntry() == NPC_JAINA_PART2)
                            me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    _wallCast = true;
                    break;
                case 6:
                    instance->SetData(DATA_ICE_WALL_2, IN_PROGRESS);
                    if (Creature* walltarget = instance->instance->GetCreature(_wallTargetGuid))
                    {
                        if (walltarget->IsAlive())
                        {
                            walltarget->DespawnOrUnsummon();
                            _wallTargetGuid = 0;
                        }
                    }
                    break;
                case 8:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_WALL_02);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_WALL_02);
                    _castTimer = 1000;
                    _holdTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *walltarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        _wallTargetGuid = walltarget->GetGUID();
                        walltarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if (me->GetEntry() == NPC_JAINA_PART2)
                            me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    _wallCast = true;
                    break;
                case 9:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_ESCAPE_02);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_ESCAPE_02);
                    break;
                case 11:
                    instance->SetData(DATA_ICE_WALL_3, IN_PROGRESS);
                    if (Creature* walltarget = instance->instance->GetCreature(_wallTargetGuid))
                    {
                        if (walltarget->IsAlive())
                        {
                            walltarget->DespawnOrUnsummon();
                            _wallTargetGuid = 0;
                        }
                    }
                    break;
                case 12:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_WALL_03);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_WALL_03);
                    _castTimer = 1000;
                    _holdTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *walltarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        _wallTargetGuid = walltarget->GetGUID();
                        walltarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if (me->GetEntry() == NPC_JAINA_PART2)
                            me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    _wallCast = true;
                    break;
                case 15:
                    instance->SetData(DATA_ICE_WALL_4, IN_PROGRESS);
                    if (Creature* walltarget = instance->instance->GetCreature(_wallTargetGuid))
                    {
                        if (walltarget->IsAlive())
                        {
                            walltarget->DespawnOrUnsummon();
                            _wallTargetGuid = 0;
                        }
                    }
                    break;
                case 16:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_WALL_04);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_WALL_04);
                    _castTimer = 1000;
                    _holdTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *walltarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        _wallTargetGuid = walltarget->GetGUID();
                        walltarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if (me->GetEntry() == NPC_JAINA_PART2)
                            me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    _wallCast = true;
                    break;
                case 17:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_ESCAPE_01);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_ESCAPE_01);
                    break;
                case 19:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_TRAP);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_TRAP);
                    break;
                case 20:
                    if (Creature* walltarget = instance->instance->GetCreature(_wallTargetGuid))
                    {
                        if (walltarget->IsAlive())
                        {
                            walltarget->DespawnOrUnsummon();
                            _wallTargetGuid = 0;
                        }
                    }
                    SetEscortPaused(true);
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                    break;
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!who || !instance)
                return;

            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = who->ToPlayer();

            if (player->GetTeam() == ALLIANCE && me->GetEntry() == NPC_SYLVANAS_PART2)
                return;

            if (player->GetTeam() == HORDE && me->GetEntry() == NPC_JAINA_PART2)
                return;

            if (me->IsWithinDistInMap(who, 270.0f) && instance->GetData(DATA_FROSTSWORN_EVENT) == DONE && instance->GetData(DATA_PHASE) == 3)
            {
                player = who->ToPlayer();
                _didHappen = true;
                instance->SetData(DATA_PHASE, 4);
            }
        }

        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
        {
            if (!instance)
                return;

            if (instance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
            {
                uiDamage = 0;
                return;
            }

            if (instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS && _wallCast == true)
            {
                _holdTimer = _holdTimer + 100;
                return;
            }
        }

        void JumpNext_step_doCount(uint32 Time)
        {
            _stepTimer = Time;
            _step_doCount++;
        }

        void Intro()
        {
            switch(_step_doCount)
            {
                case 0:
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                    _lichKingGuid = instance->GetData64(DATA_LICHKING);
                    _lichKing = instance->instance->GetCreature(_lichKingGuid);
                    me->SetUInt64Value(UNIT_FIELD_TARGET, _lichKing->GetGUID());
                    JumpNext_step_doCount(100);
                    break;
                case 1:
                    HoRQuestComplete(me->GetEntry());
                    if (_lichKing)
                    {
                        if (me->GetEntry() == NPC_JAINA_PART2)
                            _lichKing->AI()->Talk(SAY_LICH_KING_AGGRO_A);
                        _lichKing->AI()->AttackStart(me);
                        me->AI()->AttackStart(_lichKing);
                        if (me->GetEntry() == NPC_SYLVANAS_PART2)
                            _lichKing->AI()->Talk(SAY_LICH_KING_AGGRO_H);
                        _lichKing->AI()->AttackStart(me);
                        me->AI()->AttackStart(_lichKing);
                    }
                    JumpNext_step_doCount(3000);
                    break;
                case 2:
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                    {
                        _doFight = false;
                        if (_lichKing)
                            me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
                    }
                    JumpNext_step_doCount(3000);
                    break;
                case 3:
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        _doFight = true;
                    JumpNext_step_doCount(100);
                    break;
                case 4:
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                    {
                        if (_lichKing)
                            me->CastSpell(_lichKing, SPELL_SYLVANA_STEP, false);
                    }
                    JumpNext_step_doCount(3000);
                    break;
                case 5:
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                    {
                        _doFight = false;
                        if (_lichKing)
                            me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
                    }
                    JumpNext_step_doCount(3000);
                    break;
                case 6:
                    _doFight = true;
                    if (_lichKing)
                    {
                        if (me->GetEntry() == NPC_SYLVANAS_PART2)
                            DoCast(SPELL_SYLVANA_JUMP);
                        DoCast(_lichKing, me->GetEntry() == NPC_JAINA_PART2 ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, false);
                        _lichKing->AttackStop();
                    }
                    JumpNext_step_doCount(1480);
                    break;
                case 7:
                    me->RemoveAllAuras();
                    if (_lichKing && !_lichKing->HasAura(SPELL_ICE_PRISON_VISUAL))
                        _lichKing->AddAura(me->GetEntry() == NPC_JAINA_PART2 ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, _lichKing);
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                    me->AttackStop();
                    if (me->GetEntry() == NPC_JAINA_PART2)
                    {
                        me->RemoveAurasDueToSpell(SPELL_ICE_BARRIER);
                        Talk(SAY_JAINA_AGGRO);
                    }
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        Talk(SAY_SYLVANAS_AGGRO);
                    JumpNext_step_doCount(1000);
                    break;
                case 8:
                    me->GetMotionMaster()->MovePoint(0, 5577.187f, 2236.003f, 733.012f);
                    if (_lichKing && !_lichKing->HasAura(SPELL_ICE_PRISON_VISUAL))
                    {
                        _lichKing->AddAura(me->GetEntry() == NPC_JAINA_PART2 ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, _lichKing);
                        me->SetUInt64Value(UNIT_FIELD_TARGET, _lichKing->GetGUID());
                    }
                    JumpNext_step_doCount(7000);
                    break;
                case 9:
                    if (_lichKing && (!_lichKing->HasAura(SPELL_ICE_PRISON_VISUAL) || !_lichKing->HasAura(SPELL_DARK_ARROW)))
                    {
                        _lichKing->AddAura(me->GetEntry() == NPC_JAINA_PART2 ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, _lichKing);
                        me->SetUInt64Value(UNIT_FIELD_TARGET, _lichKing->GetGUID());
                    }
                    me->RemoveAllAuras();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _doCount = 1;
                    JumpNext_step_doCount(100);
                    break;
            }
        }

        void Outro()
        {
            switch(_step_doCount)
            {
                case 10:
                    JumpNext_step_doCount(10000);
                    break;
                case 11:
                    if (GameObject* gunship = instance->instance->GetGameObject(instance->GetData64(me->GetEntry() == NPC_JAINA_PART2 ? GO_SKYBREAKER : GO_ORGRIM_HAMMER)))
                        gunship->SetPhaseMask(1, true);

                    if (Creature* captain = me->SummonCreature(me->GetEntry() == NPC_JAINA_PART2? NPC_BARTLETT : NPC_KORM, 5251.17f, 1610.2f, 795.812f, 2.07997f))
                    if (GameObject* cave = instance->instance->GetGameObject(instance->GetData64(GO_CAVE)))
                    {
                        Creature *caveTarget = me->SummonCreature(NPC_ICE_WALL,cave->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000);
                        caveTarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 4);

                        for(int8 i = 0; i<4; i++)
                        {
                            if (Creature* cannoner = me->SummonCreature(NPC_ICE_WALL, CannonSpawns[i],TEMPSUMMON_MANUAL_DESPAWN,720000))
                            {
                                cannoner->setFaction(me->getFaction());
                                cannoner->CastSpell(caveTarget, SPELL_FIRE_CANNON, true);
                            }
                        }
                    }
                    JumpNext_step_doCount(6000);
                    break;
                case 12:
                    if (GameObject* cave = instance->instance->GetGameObject(instance->GetData64(GO_CAVE)))
                        cave->SetGoState(GO_STATE_READY);
                    me->RemoveAllAuras();
                    if (_lichKing)
                        _lichKing->DespawnOrUnsummon();
                    HoRQuestComplete(38211);
                    JumpNext_step_doCount(10000);
                    break;
                case 13:
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                    instance->SetData(DATA_LICHKING_EVENT, DONE);
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_FINAL_1);
                    else
                        Talk(SAY_SYLVANAS_FINAL_1);
                    me->SummonGameObject(me->GetEntry() == NPC_JAINA_PART2 ? GO_STAIRS_SKYBREAKER : GO_STAIRS_ORGRIM_HAMMER, 5247.45f, 1627.72f, 784.302f, 5.88208f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
                    JumpNext_step_doCount(10000);
                    break;
                case 14:
                    if (me->GetEntry() == NPC_JAINA_PART2)
                        Talk(SAY_JAINA_FINAL_2);
                    JumpNext_step_doCount(20000);
                    break;
                case 15:
                    JumpNext_step_doCount(5000);
                    break;
                case 16:
                    me->SetOrientation(0.68f);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    JumpNext_step_doCount(5000);
                    break;
            }
        }

        void HoRQuestComplete(uint32 killCredit)
        {
            if (instance)
            {
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    i->GetSource()->KilledMonsterCredit(killCredit, 0);
            }
        }

        void UpdateEscortAI(const uint32 diff)
        {
            if (!instance || !_didHappen)
                return;

            DoMeleeAttackIfReady();

            if (instance->GetData(DATA_PHASE) == 4 && instance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
            {
                if (_stepTimer < diff)
                    Intro();
                else
                    _stepTimer -= diff;
            }

            if (instance->GetData(DATA_LICHKING_EVENT) == SPECIAL && instance->GetData(DATA_PHASE) != 6)       //End Cinematic
            {
                instance->SetData(DATA_PHASE, 6);
                _step_doCount = 10;
            }

            if (instance->GetData(DATA_PHASE) == 6)
            {
                if (_stepTimer < diff)
                    Outro();
                else
                    _stepTimer -= diff;
                return;
            }

            if (_wallCast == true && _castTimer < diff)
            {
                if (me->GetEntry() == NPC_SYLVANAS_PART2)
                {
                    if (Creature* walltarget = instance->instance->GetCreature(_wallTargetGuid))
                        me->CastSpell(walltarget, SPELL_DESTROY_ICE_WALL_03, false);
                    _castTimer = 1000;
                }
                
            }
            else
                _castTimer -= diff;

            if (_wallCast == true && _holdTimer < 10000 && ( instance->GetData(DATA_SUMMONS) == 0 || !me->IsInCombat()))
            {
                _wallCast = false;
                me->InterruptNonMeleeSpells(false);
                SetEscortPaused(false);
                if (GameObject* gate = instance->instance->GetGameObject(_iceWallGuid))
                    gate->SetGoState(GO_STATE_ACTIVE);
                ++_doCount;
                switch(_doCount)
                {
                    case 2:
                        if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(GO_ICE_WALL_2)))
                        {
                            gate->SetGoState(GO_STATE_READY);
                            if (_lichKing && _lichKing->IsAlive())
                                _lichKing->AI()->Talk(SAY_LICH_KING_WALL_02);
                                _iceWallGuid = gate->GetGUID();
                        }
                        break;
                    case 3:
                        if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(GO_ICE_WALL_3)))
                        {
                            gate->SetGoState(GO_STATE_READY);
                            if (_lichKing && _lichKing->IsAlive())
                                _lichKing->AI()->Talk(SAY_LICH_KING_WALL_03);
                                _iceWallGuid = gate->GetGUID();
                        }
                        break;
                    case 4:
                        if (GameObject* gate = instance->instance->GetGameObject(instance->GetData64(GO_ICE_WALL_4)))
                        {
                            gate->SetGoState(GO_STATE_READY);
                            if (_lichKing && _lichKing->IsAlive())
                                _lichKing->AI()->Talk(SAY_LICH_KING_WALL_04);
                                _iceWallGuid = gate->GetGUID();
                        }
                        break;
                    case 5:
                        if (_lichKing && _lichKing->IsAlive())
                        {
                            _lichKing->RemoveAurasDueToSpell(SPELL_WINTER);
                            _lichKing->SetSpeed(MOVE_WALK, 2.5f, true);
                            _step_doCount = 0;
                        }
                        break;
                }
            }
            else
            {
                _holdTimer -= diff;
                if (_holdTimer <= 0)
                    _holdTimer = 0;
            }
            return;
        }
    };
};

class at_hor_waves_restarter : public AreaTriggerScript
{
    public:
        at_hor_waves_restarter() : AreaTriggerScript("at_hor_waves_restarter") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            InstanceScript* instance = player->GetInstanceScript();

            if (player->IsGameMaster())
                return true;

            if (instance->GetData(DATA_WAVE_COUNT) == SPECIAL)
                return true;

            if (instance->GetData(DATA_WAVES_BEGIN) == DONE && instance->GetData(DATA_MARWYN_EVENT) != DONE)
            {
                instance->SetData(DATA_WAVE_COUNT, SPECIAL);

                if (Creature* falric = player->GetCreature(*player, instance->GetData64(DATA_FALRIC)))
                {
                    falric->CastSpell(falric, SPELL_BOSS_SPAWN_AURA, true);
                    falric->SetVisible(true);
                }

                if (Creature* marwyn = player->GetCreature(*player, instance->GetData64(DATA_MARWYN)))
                {
                    marwyn->CastSpell(marwyn, SPELL_BOSS_SPAWN_AURA, true);
                    marwyn->SetVisible(true);
                }
            }
            return true;
        }
};

void AddSC_halls_of_reflection()
{
    new npc_jaina_and_sylvanas_hor();
    new npc_ghostly_priest();
    new npc_phantom_mage();
    new npc_phantom_hallucination();
    new npc_shadowy_mercenary();
    new npc_spectral_footman();
    new npc_tortured_rifleman();
    new npc_frostsworn_general();
    new at_hor_waves_restarter();
    new npc_jaina_and_sylvana_hor_part2();
}
