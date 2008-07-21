########### Project name ###########

PROJECT(lablove)
SET(LABLOVE_MAJOR "0")
SET(LABLOVE_MINOR "3")
SET(VERSION "${LABLOVE_MAJOR}.${LABLOVE_MINOR}")
SET(PACKAGE lablove)

########### Global settings ###########

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

OPTION(DEBUG "Debug settings" OFF)
OPTION(PROFILE "Profile settings" OFF)
SET(CMAKE_VERBOSE_MAKEFILE "OFF")


########### Compiler settings ###########

IF(PROFILE)
    SET(CMAKE_CXX_FLAGS "-Wno-deprecated -g -pg")
ELSE(PROFILE)
    IF(DEBUG)
        SET(CMAKE_CXX_FLAGS "-Wno-deprecated -g")
    ELSE(DEBUG)
        SET(CMAKE_CXX_FLAGS "-O3")
    ENDIF(DEBUG)
ENDIF(PROFILE)

########### System defines ###########

IF(UNIX)
    ADD_DEFINITIONS(-D__UNIX)
ENDIF(UNIX)

IF(WIN32)
    ADD_DEFINITIONS(-D__WIN32)
ENDIF(WIN32)

########### Sources ###########

SET(SOURCE_FILES
    src/Simulation.cpp
    src/SimArtist.cpp
    src/Sim2D.cpp
    src/ScriptableObject.cpp
    src/SimObj.cpp
    src/Fitness.cpp
    src/SimObj2D.cpp
    src/Target2D.cpp
    src/Laser2D.cpp
    src/PopulationDynamics.cpp
    src/PopDynSEGA.cpp
    src/Species.cpp
    src/Goal.cpp
    src/Symbol.cpp
    src/SymbolBitString.cpp
    src/SymbolRGB.cpp
    src/SymbolFixedString.cpp
    src/SymbolFloat.cpp
    src/SymbolFloatVector.cpp
    src/SymbolUL.cpp
    src/SymbolTable.cpp
    src/TableSet.cpp
    src/Log.cpp
    src/LogBestBrain.cpp
    src/Statistics.cpp
    src/StatCommon.cpp
    src/StatTime.cpp
    src/Message.cpp
    src/CompPER.cpp
    src/CompACT.cpp
    gridbrain/src/Gridbrain.cpp
    gridbrain/src/GridbrainMutations.cpp
    gridbrain/src/GridbrainRecombine.cpp
    gridbrain/src/GridbrainWrite.cpp
    gridbrain/src/Grid.cpp
    gridbrain/src/Coord.cpp
    gridbrain/src/Component.cpp
    gridbrain/src/Connection.cpp
    gridbrain/src/GeneTag.cpp
    gridbrain/src/ComponentSet.cpp
    gridbrain/src/RandDistManager.cpp
    gridbrain/mtwist/mtwist.c
    gridbrain/mtwist/randistrs.c
    gridbrain/src/CompNUL.cpp
    gridbrain/src/CompIN.cpp
    gridbrain/src/CompOUT.cpp
    gridbrain/src/CompSUM.cpp
    gridbrain/src/CompMAX.cpp
    gridbrain/src/CompMIN.cpp
    gridbrain/src/CompAVG.cpp
    gridbrain/src/CompMUL.cpp
    gridbrain/src/CompNOT.cpp
    gridbrain/src/CompAND.cpp
    gridbrain/src/CompINV.cpp
    gridbrain/src/CompNEG.cpp
    gridbrain/src/CompAMP.cpp
    gridbrain/src/CompMOD.cpp
    gridbrain/src/CompRAND.cpp
    gridbrain/src/CompEQ.cpp
    gridbrain/src/CompGTZ.cpp
    gridbrain/src/CompZERO.cpp
    gridbrain/src/CompCLK.cpp
    gridbrain/src/CompDMUL.cpp
    gridbrain/src/CompSEL.cpp
    )

SET(UNIT_TEST_FILES
    gridbrain/UnitTest++/AssertException.cpp
    gridbrain/UnitTest++/Test.cpp
    gridbrain/UnitTest++/Checks.cpp
    gridbrain/UnitTest++/TestRunner.cpp
    gridbrain/UnitTest++/TestResults.cpp
    gridbrain/UnitTest++/TestReporter.cpp
    gridbrain/UnitTest++/TestReporterStdout.cpp
    gridbrain/UnitTest++/ReportAssert.cpp
    gridbrain/UnitTest++/TestList.cpp
    gridbrain/UnitTest++/TimeConstraint.cpp
    gridbrain/UnitTest++/TestDetails.cpp
    gridbrain/UnitTest++/MemoryOutStream.cpp
    gridbrain/UnitTest++/DeferredTestReporter.cpp
    gridbrain/UnitTest++/DeferredTestResult.cpp
    gridbrain/UnitTest++/XmlTestReporter.cpp)

IF(MINGW)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        gridbrain/UnitTest++/Win32/TimeHelpers.cpp)
ELSE(MINGW)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        gridbrain/UnitTest++/Posix/SignalTranslator.cpp
        gridbrain/UnitTest++/Posix/TimeHelpers.cpp)
ENDIF(MINGW)

SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        gridbrain/tests/Main.cpp
        gridbrain/tests/TestGrid.cpp
        gridbrain/tests/TestGridCoord.cpp
        gridbrain/tests/TestGridbrain.cpp
        gridbrain/tests/TestGridbrainMutations.cpp
        gridbrain/tests/TestGridbrainRecombine.cpp
        gridbrain/tests/TestGridbrainComponents.cpp
        gridbrain/tests/TestGridbrainComponentSet.cpp
        )

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(EXTRA_SRC_FILES ${EXTRA_SRC_FILES} src/MacOSXSDL/SDLMain.m)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

########### Include dirs ###########

INCLUDE(CMakeModules/FindLua.cmake)
INCLUDE(CMakeModules/FindArtist.cmake)

INCLUDE_DIRECTORIES("src")
INCLUDE_DIRECTORIES("gridbrain/UnitTest++")
INCLUDE_DIRECTORIES("gridbrain/mtwist")
INCLUDE_DIRECTORIES("gridbrain/include")
INCLUDE_DIRECTORIES(${Lua_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Artist_INCLUDE_DIR})

########### Targets ###########

ADD_LIBRARY(llove STATIC ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(llove llove ${Artist_LIBRARIES} ${Lua_LIBRARIES})

ADD_EXECUTABLE(lablove ${EXTRA_SRC_FILES} src/main.cpp)
TARGET_LINK_LIBRARIES(lablove llove ${Artist_LIBRARIES} ${Lua_LIBRARIES})

ADD_EXECUTABLE(test ${UNIT_TEST_FILES})
SET(LINK_LIBS ${LINK_LIBS} ${Artist_LIBRARIES} ${Lua_LIBRARIES})

ADD_CUSTOM_COMMAND(TARGET test POST_BUILD COMMAND "./test")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    INCLUDE(CMakeModules/FindFreeType.cmake)
    INCLUDE(${CMAKE_ROOT}/Modules/FindSDL.cmake)
    INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIR_freetype2})
    SET(LINK_LIBS ${LINK_LIBS} ${FREETYPE_LIBRARY} ${SDL_LIBRARY})
    SET(DEFINITIONS ${DEFINITIONS} -D_OSX_SDL_)
    SET_TARGET_PROPERTIES(llove lablove test PROPERTIES LINK_FLAGS "-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

ADD_DEFINITIONS(${DEFINITIONS})

TARGET_LINK_LIBRARIES(llove ${LINK_LIBS})
TARGET_LINK_LIBRARIES(lablove llove ${LINK_LIBS})
TARGET_LINK_LIBRARIES(test llove ${LINK_LIBS})

