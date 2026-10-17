#include "legoapi/world/world.h"
#include "legoapi/props/system/socksys.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nunativegscene_s;

static NARROWSOCKEXCEPTION *NarrowSockException;

static inline i32 NarrowSockPositionAllowed(GameObject_s *object, SOCKSYS *system, i32 level_index) {
    if (system == NULL || object->sock_position.location.sock == -1)
        return 0;
    if ((system->sock[object->sock_position.location.sock].flags & 0x800) == 0)
        return 0;
    if (NarrowSockException != NULL) {
        for (NARROWSOCKEXCEPTION *exception = NarrowSockException; exception->level_name != NULL; ++exception) {
            if (exception->level_index == -1 || exception->level_index != level_index ||
                exception->sock_index != object->sock_position.location.sock)
                continue;
            if (object->sock_position.distance >= exception->start_distance &&
                object->sock_position.distance <= exception->end_distance)
                return 0;
        }
    }
    return 1;
}

i32 ObjInNarrowSock(GameObject_s *object, SOCKSYS *system, i32 level_index) {
    if ((WORLD->current_level->flags & LEVEL_NARROW_SOCKS) == 0 && !VehicleArea)
        return 0;
    if (disable_narrow_socks || object->apiobj.field_0x27c == -1)
        return 0;
    if (object->apiobj.player_controlled) {
        GameObject_s *other = NULL;
        if (object == Player[0])
            other = Player[1];
        else if (object == Player[1])
            other = Player[0];
        if (other != NULL && other->apiobj.player_controlled && !NarrowSockPositionAllowed(other, system, level_index))
            return 0;
    }
    return NarrowSockPositionAllowed(object, system, level_index);
}

i32 objInNetWaitContext(GameObject_s *object, i32 context) {
    if (LEGOCONTEXT_NETWAIT != -1 && object != NULL && LEGOCONTEXT_NETWAIT == object->character_context) {
        return object->context_animation == context;
    }
    return false;
}

#include "legoapi/actions/character/simpletons_types.h"

void NarrowSockExceptions_Init(NARROWSOCKEXCEPTION *exceptions) {
    i32 level_index __attribute__((aligned(16)));
    if (exceptions == NULL)
        return;
    NarrowSockException = exceptions;
    for (; exceptions->level_name != NULL; ++exceptions) {
        Level_FindByName(exceptions->level_name, &level_index);
        exceptions->level_index = level_index;
    }
}

