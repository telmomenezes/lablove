########### Project name ###########

PROJECT(lablove)
SET(LABLOVE_MAJOR "0")
SET(LABLOVE_MINOR "3")
SET(VERSION "${LABLOVE_MAJOR}.${LABLOVE_MINOR}")
SET(PACKAGE lablove)

########### Global settings ###########

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
    src/Sim2D.cpp
    src/Laser.cpp
    src/SimHex.cpp
    src/HexCell.cpp
    src/ScriptableObject.cpp
    src/SimObj.cpp
    src/SimObj2D.cpp
    src/SimObjHex.cpp
    src/Brain.cpp
    src/InterfaceItem.cpp
    src/DummyBrain.cpp
    src/Gridbrain.cpp
    src/GridbrainMutations.cpp
    src/GridbrainRecombine.cpp
    src/GridbrainWrite.cpp
    src/Grid.cpp
    src/GridCoord.cpp
    src/GridbrainComponent
    src/GridbrainConnection.cpp
    src/GridbrainGeneTag.cpp
    src/GridbrainComponentSet.cpp
    src/GridbrainMemCell.cpp
    src/PopulationDynamics.cpp
    src/PopDynGenerations.cpp
    src/PopDynSpecies.cpp
    src/PopDynFixedSpecies.cpp
    src/PopDynSpeciesBuffers.cpp
    src/Symbol.cpp
    src/SymbolBitString.cpp
    src/SymbolRGB.cpp
    src/SymbolFixedString.cpp
    src/SymbolFloat.cpp
    src/SymbolFloatVector.cpp
    src/SymbolUL.cpp
    src/SymbolTable.cpp
    src/Log.cpp
    src/LogBestBrain.cpp
    src/Statistics.cpp
    src/StatCommon.cpp
    src/StatTime.cpp
    src/Graphic2D.cpp
    src/Graphic2DTriangle.cpp
    src/Graphic2DSquare.cpp
    src/Graphic2DGradient.cpp
    src/Graphic2DBiomorph.cpp
    src/RandDistManager.cpp
    src/Message.cpp
    src/mtwist/mtwist.c
    src/mtwist/randistrs.c)

SET(UNIT_TEST_FILES
    src/UnitTest++/AssertException.cpp
    src/UnitTest++/Test.cpp
    src/UnitTest++/Checks.cpp
    src/UnitTest++/TestRunner.cpp
    src/UnitTest++/TestResults.cpp
    src/UnitTest++/TestReporter.cpp
    src/UnitTest++/TestReporterStdout.cpp
    src/UnitTest++/ReportAssert.cpp
    src/UnitTest++/TestList.cpp
    src/UnitTest++/TimeConstraint.cpp
    src/UnitTest++/TestDetails.cpp
    src/UnitTest++/MemoryOutStream.cpp
    src/UnitTest++/DeferredTestReporter.cpp
    src/UnitTest++/DeferredTestResult.cpp
    src/UnitTest++/XmlTestReporter.cpp)

IF(MINGW)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        src/UnitTest++/Win32/TimeHelpers.cpp)
ELSE(MINGW)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        src/UnitTest++/Posix/SignalTranslator.cpp
        src/UnitTest++/Posix/TimeHelpers.cpp)
ENDIF(MINGW)

SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        src/tests/Main.cpp
        src/tests/TestSimulation.cpp
        src/tests/TestSim2D.cpp
        src/tests/TestGrid.cpp
        src/tests/TestGridCoord.cpp
        src/tests/TestGridbrain.cpp
        src/tests/TestGridbrainMutations.cpp
        src/tests/TestGridbrainRecombine.cpp
        src/tests/TestGridbrainComponents.cpp
        src/tests/TestGridbrainComponentSet.cpp
        src/tests/TestSymbolBitString.cpp
        src/tests/TestSymbolRGB.cpp
        src/tests/TestSymbolFixedString.cpp
        src/tests/TestSymbolFloat.cpp)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(EXTRA_SRC_FILES ${EXTRA_SRC_FILES} src/MacOSXSDL/SDLMain.m)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

########### Include dirs ###########

INCLUDE(CMakeModules/FindLua.cmake)
INCLUDE(CMakeModules/FindArtist.cmake)

INCLUDE_DIRECTORIES("src")
INCLUDE_DIRECTORIES("src/UnitTest++")
INCLUDE_DIRECTORIES("src/mtwist")
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

