IF(Artist_INCLUDE_DIR AND Artist_LIBRARIES)
    SET(Artist_FIND_QUIETLY TRUE)
ENDIF(Artist_INCLUDE_DIR AND Artist_LIBRARIES)

FIND_PATH(Artist_INCLUDE_DIR art.h /usr/include/artist /usr/local/include/artist ~/artist/include)

FIND_LIBRARY(Artist_LIBRARIES NAMES artist PATHS ~/artist)

IF(Artist_INCLUDE_DIR AND Artist_LIBRARIES)
    SET(Artist_FOUND TRUE)
ELSE(Artist_INCLUDE_DIR AND Artist_LIBRARIES)
    SET(Artist_FOUND FALSE)
ENDIF (Artist_INCLUDE_DIR AND Artist_LIBRARIES)

IF(Artist_FOUND)
    IF (NOT Artist_FIND_QUIETLY)
        MESSAGE(STATUS "Found Artist library: ${Artist_LIBRARIES}")
        MESSAGE(STATUS "Found Artist headers: ${Artist_INCLUDE_DIR}")
    ENDIF (NOT Artist_FIND_QUIETLY)
    ELSE(Artist_FOUND)
        IF(Artist_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "Could NOT find Artist")
        ENDIF(Artist_FIND_REQUIRED)
ENDIF(Artist_FOUND)

MARK_AS_ADVANCED(Artist_INCLUDE_DIR Artist_LIBRARIES)
