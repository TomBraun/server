project(kernel C)

SET(_TEST_FILES
build.test.c
config.test.c
ship.test.c
spell.test.c
ally.test.c
battle.test.c
building.test.c
magic.test.c
equipment.test.c
curse.test.c
item.test.c
move.test.c
pool.test.c
reports.test.c
spellbook.test.c
curse.test.c
jsonconf.test.c
)

SET(_FILES
alchemy.c
alliance.c
ally.c
battle.c
build.c
building.c
calendar.c
command.c
config.c
connection.c
curse.c
equipment.c
faction.c
group.c
item.c
magic.c
messages.c
move.c
names.c
order.c
pathfinder.c
plane.c
player.c
pool.c
race.c
region.c
reports.c
resources.c
save.c
ship.c
skills.c
spellbook.c
spell.c
teleport.c
terrain.c
unit.c
xmlreader.c
jsonconf.c
)

FOREACH(_FILE ${_FILES})
    LIST(APPEND _SOURCES ${PROJECT_NAME}/${_FILE})
ENDFOREACH(_FILE)
SET(KERNEL_SRC ${_SOURCES} PARENT_SCOPE)
FOREACH(_FILE ${_TEST_FILES})
    LIST(APPEND _TESTS ${PROJECT_NAME}/${_FILE})
ENDFOREACH(_FILE)
SET(KERNEL_TESTS ${_TESTS} PARENT_SCOPE)
