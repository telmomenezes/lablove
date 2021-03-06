IF(Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FIND_QUIETLY TRUE)
ENDIF(Lua_INCLUDE_DIR AND Lua_LIBRARIES)

FIND_PATH(Lua_INCLUDE_DIR lua.h
            /usr/include
            /usr/local/include
            /usr/include/lua51
            /usr/local/include/lua51
            /usr/include/lua5.1
            /usr/local/include/lua5.1
            c:/libs/include
            ~/lua/include)

FIND_LIBRARY(Lua_LIBRARIES NAMES lua lua51 lua5.1 PATHS
                c:/libs/lib ~/lua/lib)

IF(Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FOUND TRUE)
    INCLUDE(CheckLibraryExists)
    CHECK_LIBRARY_EXISTS(${Lua_LIBRARIES} lua_close "" Lua_NEED_PREFIX)
ELSE(Lua_INCLUDE_DIR AND Lua_LIBRARIES)
    SET(Lua_FOUND FALSE)
ENDIF (Lua_INCLUDE_DIR AND Lua_LIBRARIES)

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
