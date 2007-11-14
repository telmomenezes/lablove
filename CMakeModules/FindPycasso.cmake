IF(Pycasso_INCLUDE_DIR AND Pycasso_LIBRARIES)
    SET(Pycasso_FIND_QUIETLY TRUE)
ENDIF(Pycasso_INCLUDE_DIR AND Pycasso_LIBRARIES)

FIND_PATH(Pycasso_INCLUDE_DIR pyc.h /usr/include/pycasso /usr/local/include/pycasso ~/pycasso/include)

FIND_LIBRARY(Pycasso_LIBRARIES NAMES pycasso)

IF(Pycasso_INCLUDE_DIR AND Pycasso_LIBRARIES)
    SET(Pycasso_FOUND TRUE)
ELSE(Pycasso_INCLUDE_DIR AND Pycasso_LIBRARIES)
    SET(Pycasso_FOUND FALSE)
ENDIF (Pycasso_INCLUDE_DIR AND Pycasso_LIBRARIES)

IF(Pycasso_FOUND)
    IF (NOT Pycasso_FIND_QUIETLY)
        MESSAGE(STATUS "Found Pycasso library: ${Pycasso_LIBRARIES}")
        MESSAGE(STATUS "Found Pycasso headers: ${Pycasso_INCLUDE_DIR}")
    ENDIF (NOT Pycasso_FIND_QUIETLY)
    ELSE(Pycasso_FOUND)
        IF(Pycasso_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "Could NOT find Pycasso")
        ENDIF(Pycasso_FIND_REQUIRED)
ENDIF(Pycasso_FOUND)

MARK_AS_ADVANCED(Pycasso_INCLUDE_DIR Pycasso_LIBRARIES)
