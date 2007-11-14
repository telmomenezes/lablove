IF(Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FIND_QUIETLY TRUE)
ENDIF(Lua_INCLUDE_DIR AND Lua_LIBRARIES)

FIND_PATH(Lua_INCLUDE_DIR lua.h /usr/include /usr/local/include /usr/include/lua50 /usr/include/lua51)

FIND_LIBRARY(Lua_LIBRARIES NAMES lua lua50 lua51)
FIND_LIBRARY(LuaLib_LIBRARIES NAMES lualib lualib50)

IF(Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FOUND TRUE)
    INCLUDE(CheckLibraryExists)
    CHECK_LIBRARY_EXISTS(${Lua_LIBRARIES} lua_close "" Lua_NEED_PREFIX)
ELSE(Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FOUND FALSE)
ENDIF (Lua_INCLUDE_DIR AND Lua_LIBRARIES)

SET(Lua_LIBRARIES ${Lua_LIBARIES} ${LuaLib_LIBRARIES})

IF(Lua_FOUND)
    IF (NOT Lua_FIND_QUIETLY)
        MESSAGE(STATUS "Found Lua library: ${Lua_LIBRARIES}")
        MESSAGE(STATUS "Found Lua  headers: ${Lua_INCLUDE_DIR}")
    ENDIF (NOT Lua_FIND_QUIETLY)
    ELSE(Lua_FOUND)
        IF(Lua_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "Could NOT find Lua")
        ENDIF(Lua_FIND_REQUIRED)
ENDIF(Lua_FOUND)

MARK_AS_ADVANCED(Lua_INCLUDE_DIR Lua_LIBRARIES)
