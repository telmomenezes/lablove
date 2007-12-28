# - Locate FreeType library
# This module defines
#  FREETYPE_LIBRARY, the library to link against
#  FREETYPE_FOUND, if false, do not try to link to FREETYPE
#  FREETYPE_INCLUDE_DIRS, where to find headers.
#  This is the concatenation of the paths:
#  FREETYPE_INCLUDE_DIR_ft2build
#  FREETYPE_INCLUDE_DIR_freetype2
#   
# $FREETYPE_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FREETYPE_DIR
# used in building FREETYPE.
# Created by Eric Wing. 

# Ugh, FreeType seems to use some #include trickery which 
# makes this harder than it should be. It looks like they
# put ft2build.h in a common/easier-to-find location which
# then contains a #include to a more specific header in a 
# more specific location (#include <freetype/config/ftheader.h>).
# Then from there, they need to set a bunch of #define's 
# so you can do something like:
# #include FT_FREETYPE_H
# Unfortunately, using CMake's mechanisms like INCLUDE_DIRECTORIES()
# wants explicit full paths and this trickery doesn't work too well.
# I'm going to attempt to cut out the middleman and hope 
# everything still works.
FIND_PATH(FREETYPE_INCLUDE_DIR_ft2build ft2build.h 
    $ENV{FREETYPE_DIR}
    /usr/local/include
    /usr/include
    /usr/local/X11R6
    /usr/X11R6/include
    /sw/include
    /opt/local/include
    /opt/csw/include
    /opt/include
    /usr/freeware/include/freetype2
)

FIND_PATH(FREETYPE_INCLUDE_DIR_freetype2 freetype/config/ftheader.h 
    $ENV{FREETYPE_DIR}/include/freetype2
    /usr/local/include/freetype2
    /usr/include/freetype2
    /usr/local/X11R6/include/freetype2
    /usr/X11R6/include/freetype2
    /sw/include/freetype2
    /opt/local/include/freetype2
    /opt/csw/include/freetype2
    /opt/include/freetype2
    /usr/freeware/include/freetype2
)

FIND_LIBRARY(FREETYPE_LIBRARY 
    NAMES freetype libfreetype freetype219
    PATHS
    $ENV{FREETYPE_DIR}/lib
    $ENV{FREETYPE_DIR}/lib
    /usr/local/lib
    /usr/lib
    /usr/local/X11R6/lib
    /usr/X11R6/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
    /usr/freeware/lib64
)

IF(FREETYPE_INCLUDE_DIR_ft2build AND FREETYPE_INCLUDE_DIR_freetype2)
    SET(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_ft2build};${FREETYPE_INCLUDE_DIR_freetype2}")
ENDIF(FREETYPE_INCLUDE_DIR_ft2build AND FREETYPE_INCLUDE_DIR_freetype2)


SET(FREETYPE_FOUND "NO")
IF(FREETYPE_LIBRARY AND FREETYPE_INCLUDE_DIRS)
    SET(FREETYPE_FOUND "YES")
ENDIF(FREETYPE_LIBRARY AND FREETYPE_INCLUDE_DIRS)
