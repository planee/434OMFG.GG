/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef _FORMATIONS_H
#define _FORMATIONS_H

#include "Define.h"
#include "UnorderedMap.h"
#include <map>

class Creature;
class CreatureGroup;

struct FormationInfo
{
    uint32 leaderGUID;
    float follow_dist;
    float follow_angle;
    uint32 groupAI;
};

enum GroupAIFlags
{
    FLAG_AGGRO_NONE = 0,                                                         // No creature group behavior
    FLAG_MEMBERS_ASSIST_LEADER = 0x00000001,                                                // The member aggroes if the leader aggroes
    FLAG_LEADER_ASSISTS_MEMBER = 0x00000002,                                                // The leader aggroes if the member aggroes
    FLAG_MEMBERS_ASSIST_MEMBER = (FLAG_MEMBERS_ASSIST_LEADER | FLAG_LEADER_ASSISTS_MEMBER), // every member will assist if any member is attacked
    FLAG_IDLE_IN_FORMATION = 0x00000200,                                                // The member will follow the leader when pathing idly
};

typedef UNORDERED_MAP<uint32/*memberDBGUID*/, FormationInfo*>   CreatureGroupInfoType;

class FormationMgr
{
    friend class ACE_Singleton<FormationMgr, ACE_Null_Mutex>;
    public:
        FormationMgr() { }
        ~FormationMgr();
        void AddCreatureToGroup(uint32 group_id, Creature* creature);
        void RemoveCreatureFromGroup(CreatureGroup* group, Creature* creature);
        void LoadCreatureFormations();
        CreatureGroupInfoType CreatureGroupMap;
};

class CreatureGroup
{
    private:
        Creature* m_leader; //Important do not forget sometimes to work with pointers instead synonims :D:D
        typedef std::map<Creature*, FormationInfo*>  CreatureGroupMemberType;
        CreatureGroupMemberType m_members;

        uint32 m_groupID;
        bool m_Formed;

    public:
        //Group cannot be created empty
        explicit CreatureGroup(uint32 id) : m_leader(NULL), m_groupID(id), m_Formed(false) {}
        ~CreatureGroup() {}

        Creature* getLeader() const { return m_leader; }
        uint32 GetId() const { return m_groupID; }
        bool isEmpty() const { return m_members.empty(); }
        bool isFormed() const { return m_Formed; }

        void AddMember(Creature* member);
        void RemoveMember(Creature* member);
        void FormationReset(bool dismiss);

        void LeaderMoveTo(float x, float y, float z, bool inverse_angle = false);
        void MemberAttackStart(Creature* member, Unit* target);
};

#define sFormationMgr ACE_Singleton<FormationMgr, ACE_Null_Mutex>::instance()

#endif