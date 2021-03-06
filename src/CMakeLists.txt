cmake_minimum_required(VERSION 2.6)
project (server C)

IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Werror -Wno-unknown-pragmas -Wstrict-prototypes -Wpointer-arith -Wno-char-subscripts -Wno-long-long")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -DHAVE__BOOL")
ELSEIF(MSVC)
ELSE(CMAKE_COMPILER_IS_GNUCC)
    MESSAGE(STATUS "Unknown compiler ${CMAKE_C_COMPILER_ID}")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CRITBIT_INCLUDE_DIR})
include_directories (${CJSON_INCLUDE_DIR})
include_directories (${STORAGE_INCLUDE_DIR})
include_directories (${CRYPTO_INCLUDE_DIR})
include_directories (${QUICKLIST_INCLUDE_DIR})
include_directories (${CUTEST_INCLUDE_DIR})
include_directories (${LUA_INCLUDE_DIR})
include_directories (${BSON_INCLUDE_DIR})
include_directories (${INIPARSER_INCLUDE_DIR})

add_subdirectory(util)
add_subdirectory(kernel)
add_subdirectory(items)
add_subdirectory(attributes)
add_subdirectory(spells)
add_subdirectory(triggers)
add_subdirectory(modules)
add_subdirectory(races)

MACRO(ADD_LUA_MODULE MODULE_NAME FILES)
  ADD_LIBRARY (${MODULE_NAME} SHARED ${FILES})
  SET_TARGET_PROPERTIES(${MODULE_NAME}
    PROPERTIES
    PREFIX ""
  )
ENDMACRO(ADD_LUA_MODULE)

MACRO(TOLUA_BINDING PKGFILE FILES)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${PKGFILE}.c
    DEPENDS ${FILES} ${PKGFILE}
    COMMAND ${TOLUA_EXECUTABLE}
    ARGS -o ${CMAKE_CURRENT_SOURCE_DIR}/${PKGFILE}.c ${PKGFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
ENDMACRO(TOLUA_BINDING)

IF(NOT MSVC)
TOLUA_BINDING(log.pkg util/log.h)
TOLUA_BINDING(locale.pkg bind_locale.h)
TOLUA_BINDING(config.pkg bind_config.h)
TOLUA_BINDING(process.pkg bind_process.h)
TOLUA_BINDING(game.pkg bind_eressea.h config.h)
TOLUA_BINDING(eressea.pkg bind_eressea.h)
TOLUA_BINDING(settings.pkg bind_settings.h)
ENDIF()

set (ERESSEA_SRC
  eressea.c
  callback.c
  direction.c
  keyword.c
  skill.c
  json.c
  creation.c
  creport.c
  economy.c
  give.c
  items.c
  laws.c
  market.c
  monster.c
  randenc.c
  report.c
  spy.c
  study.c
  summary.c
  monsters.c
  ${SPELLS_SRC}
  ${RACES_SRC}
  ${ITEMS_SRC}
  ${MODULES_SRC}
  ${TRIGGERS_SRC}
  ${ATTRIBUTES_SRC}
  ${KERNEL_SRC}
  ${UTIL_SRC}
)

set(SERVER_SRC
  main.c
  console.c
  helpers.c
  config.pkg.c
  eressea.pkg.c
  game.pkg.c
  locale.pkg.c
  log.pkg.c
  process.pkg.c
  settings.pkg.c
  bind_building.c
  bind_config.c
  bind_locale.c
  bind_eressea.c
  bind_faction.c
  bind_hashtable.c
  bindings.c
  bind_message.c
  bind_monsters.c
  bind_process.c
  bind_region.c
  bind_settings.c
  bind_ship.c
  bind_storage.c
  bind_unit.c
  ${ERESSEA_SRC})

if (SQLITE3_FOUND)
set (SERVER_SRC
  sqlite.c
  bind_sqlite.c
  ${SERVER_SRC})
endif (SQLITE3_FOUND)

if (CURSES_FOUND)
set (SERVER_SRC
  gmtool.c
  listbox.c
  bind_gmtool.c
  ${SERVER_SRC})
endif(CURSES_FOUND)

add_executable(eressea ${SERVER_SRC})
target_link_libraries(eressea
  ${TOLUA_LIBRARIES}
  ${LUA_LIBRARIES}
  ${QUICKLIST_LIBRARIES}
  ${STORAGE_LIBRARIES}
  ${CRITBIT_LIBRARIES}
  ${CRYPTO_LIBRARIES}
  ${CJSON_LIBRARIES}
  ${INIPARSER_LIBRARIES}
)

set(TESTS_SRC
  test_eressea.c
  tests.c
  tests.test.c
  callback.test.c
  direction.test.c
  keyword.test.c
  skill.test.c
  json.test.c
  economy.test.c
  market.test.c
  laws.test.c
  ${UTIL_TESTS}
  ${KERNEL_TESTS}
  ${ERESSEA_SRC}
)

add_executable(test_eressea ${TESTS_SRC})
target_link_libraries(test_eressea ${CUTEST_LIBRARIES})
target_link_libraries(test_eressea
  ${LUA_LIBRARIES}
  ${QUICKLIST_LIBRARIES}
  ${STORAGE_LIBRARIES}
  ${CRITBIT_LIBRARIES}
  ${CRYPTO_LIBRARIES}
  ${CJSON_LIBRARIES}
  ${INIPARSER_LIBRARIES}
  )

add_test(server test_eressea)
add_test(
	NAME E3
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/game-e3
  COMMAND $<TARGET_FILE:eressea> runtests.lua
	)
add_test(
	NAME E2
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/game-e2
  COMMAND $<TARGET_FILE:eressea> runtests.lua
  )

install(TARGETS eressea DESTINATION bin)

if (SQLITE3_FOUND)
target_link_libraries(eressea ${SQLITE3_LIBRARIES})
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_SQLITE")
endif(SQLITE3_FOUND)

if (CURSES_FOUND)
include_directories (${CURSES_INCLUDE_DIR})
target_link_libraries(eressea ${CURSES_LIBRARIES})
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_CURSES")
endif(CURSES_FOUND)

if (LIBXML2_FOUND)
include_directories (${LIBXML2_INCLUDE_DIR})
target_link_libraries(eressea ${LIBXML2_LIBRARIES})
target_link_libraries(test_eressea ${LIBXML2_LIBRARIES})
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_LIBXML2")
endif (LIBXML2_FOUND)
