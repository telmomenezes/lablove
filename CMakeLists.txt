PROJECT(lablove)

OPTION(DEBUG "Debug settings" OFF)

# TODO: Create a cmake script to find the correct directory for pycasso
INCLUDE_DIRECTORIES("/usr/include/pycasso")

#SET(CMAKE_VERBOSE_MAKEFILE "ON")

INCLUDE(CMakeModules/FindLua.cmake)

IF(DEBUG)
    SET(CMAKE_CXX_FLAGS "-Wno-deprecated -g")
ELSE(DEBUG)
    SET(CMAKE_CXX_FLAGS "-Wno-deprecated -O3")
ENDIF(DEBUG)

IF(UNIX)
    ADD_DEFINITIONS(-D__UNIX)
ENDIF(UNIX)

IF(WIN32)
    ADD_DEFINITIONS(-D__WIN32)
ENDIF(WIN32)

SET(SOURCE_FILES
    src/PopulationManager.cpp
    src/Simulation.cpp
    src/SimCont2D.cpp
    src/ScriptableObject.cpp
    src/SimulationObject.cpp
    src/GraphicalObject.cpp
    src/Agent.cpp
    src/Brain.cpp
    src/DummyBrain.cpp
    src/Gridbrain.cpp
    src/Grid.cpp
    src/GridbrainComponent
    src/GridbrainComponentSet.cpp
    src/PopulationDynamics.cpp
    src/PopDynGenerations.cpp
    src/PopDynFixedSpecies.cpp
    src/Symbol.cpp
    src/SymbolRGB.cpp
    src/SymbolFixedString.cpp
    src/SymbolFloat.cpp
    src/SymbolFloatVector.cpp
    src/SymbolUL.cpp
    src/SymbolTable.cpp
    src/Statistics.cpp
    src/StatCommon.cpp
    src/StatTime.cpp
    src/Graphic.cpp
    src/GraphicTriangle.cpp
    src/GraphicSquare.cpp
    src/GraphicBiomorph.cpp
    src/RandDistManager.cpp
    src/mtwist/mtwist.c
    src/mtwist/randistrs.c)


ADD_LIBRARY(llove SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(llove llove pycasso ${Lua_LIBRARIES} dl)

ADD_EXECUTABLE(lablove src/main.cpp)
TARGET_LINK_LIBRARIES(lablove llove pycasso ${Lua_LIBRARIES} dl)

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

IF(MINGW32)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        src/UnitTest++/Win32/TimeHelpers.cpp)
ELSE(MINGW32)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        src/UnitTest++/Posix/SignalTranslator.cpp
        src/UnitTest++/Posix/TimeHelpers.cpp)
ENDIF(MINGW32)

SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        src/tests/Main.cpp
        src/tests/TestSimulation.cpp
        src/tests/TestSimCont2D.cpp
        src/tests/TestGrid.cpp
        src/tests/TestGridbrain.cpp
        src/tests/TestGridbrainComponentSet.cpp
        src/tests/TestSymbolRGB.cpp
        src/tests/TestSymbolFixedString.cpp
        src/tests/TestSymbolFloat.cpp)

INCLUDE_DIRECTORIES("src")
INCLUDE_DIRECTORIES("src/UnitTest++")
INCLUDE_DIRECTORIES("src/mtwist")
INCLUDE_DIRECTORIES(${Lua_INCLUDE_DIR})

ADD_EXECUTABLE(test ${UNIT_TEST_FILES})
TARGET_LINK_LIBRARIES(test llove pycasso ${Lua_LIBRARIES} dl)

ADD_CUSTOM_COMMAND(TARGET test POST_BUILD COMMAND "./test")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    INCLUDE_DIRECTORIES("/usr/local/include/SDL")
    SET_TARGET_PROPERTIES(lablove test PROPERTIES LINK_FLAGS "-L/usr/local/lib -lfreetype /usr/local/lib/libSDLmain.a /usr/local/lib/libSDL.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,QuickTime -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
