#include <settings.h>
#include <platform.h>
#include "stdafx.h"

#include <kernel/alchemy.c>
#include <kernel/alliance.c>
#include <kernel/battle.c>
#include <kernel/binarystore.c>
#include <kernel/connection.c>
#include <kernel/build.c>
#include <kernel/building.c>
#include <kernel/calendar.c>
#include <kernel/command.c>
#include <kernel/config.c>
#include <kernel/curse.c>
#include <kernel/equipment.c>
#include <kernel/faction.c>
#include <kernel/group.c>
#include <kernel/item.c>
#include <kernel/magic.c>
#include <kernel/message.c>
#include <kernel/move.c>
#include <kernel/names.c>
#include <kernel/order.c>
#include <kernel/pathfinder.c>
#include <kernel/plane.c>
#include <kernel/player.c>
#include <kernel/pool.c>
#include <kernel/race.c>
#include <kernel/region.c>
#include <kernel/reports.c>
#include <kernel/resources.c>
#include <kernel/save.c>
#include <kernel/ship.c>
#include <kernel/skill.c>
#include <kernel/spell.c>
#include <kernel/teleport.c>
#include <kernel/terrain.c>
#include <kernel/textstore.c>
#include <kernel/unit.c>
#include <kernel/sqlite.c>
#ifdef HAVE_LIBXML
#include <kernel/xmlreader.c>
#else
void register_xmlreader(void) {}
void enable_xml_gamecode(void) {}
#endif

#include <modules/arena.c>
#include <modules/autoseed.c>
#include <modules/dungeon.c>
#include <modules/gmcmd.c>
#include <modules/museum.c>
#include <modules/score.c>
#include <modules/weather.c>
#include <modules/wormhole.c>
#include <modules/xecmd.c>
#include <modules/xmas.c>
