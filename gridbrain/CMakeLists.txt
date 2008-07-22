########### Project name ###########

PROJECT(gridbrain)
SET(GRIDBRAIN_MAJOR "0")
SET(GRIDBRAIN_MINOR "3")
SET(VERSION "${GRIDBRAIN_MAJOR}.${GRIDBRAIN_MINOR}")
SET(PACKAGE gridbrain)

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
    src/Gridbrain.cpp
    src/GridbrainMutations.cpp
    src/GridbrainRecombine.cpp
    src/GridbrainWrite.cpp
    src/Grid.cpp
    src/Coord.cpp
    src/Component.cpp
    src/Connection.cpp
    src/Gene.cpp
    src/ComponentSet.cpp
    src/RandDistManager.cpp
    mtwist/mtwist.c
    mtwist/randistrs.c
    src/CompNUL.cpp
    src/CompIN.cpp
    src/CompOUT.cpp
    src/CompSUM.cpp
    src/CompMAX.cpp
    src/CompMIN.cpp
    src/CompAVG.cpp
    src/CompMUL.cpp
    src/CompNOT.cpp
    src/CompAND.cpp
    src/CompINV.cpp
    src/CompNEG.cpp
    src/CompAMP.cpp
    src/CompMOD.cpp
    src/CompRAND.cpp
    src/CompEQ.cpp
    src/CompGTZ.cpp
    src/CompZERO.cpp
    src/CompCLK.cpp
    src/CompDMUL.cpp
    src/CompSEL.cpp
    src/CompMEM.cpp
    src/CompTMEM.cpp
    )

SET(UNIT_TEST_FILES
    UnitTest++/AssertException.cpp
    UnitTest++/Test.cpp
    UnitTest++/Checks.cpp
    UnitTest++/TestRunner.cpp
    UnitTest++/TestResults.cpp
    UnitTest++/TestReporter.cpp
    UnitTest++/TestReporterStdout.cpp
    UnitTest++/ReportAssert.cpp
    UnitTest++/TestList.cpp
    UnitTest++/TimeConstraint.cpp
    UnitTest++/TestDetails.cpp
    UnitTest++/MemoryOutStream.cpp
    UnitTest++/DeferredTestReporter.cpp
    UnitTest++/DeferredTestResult.cpp
    UnitTest++/XmlTestReporter.cpp)

IF(MINGW)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        UnitTest++/Win32/TimeHelpers.cpp)
ELSE(MINGW)
    SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        UnitTest++/Posix/SignalTranslator.cpp
        UnitTest++/Posix/TimeHelpers.cpp)
ENDIF(MINGW)

SET(UNIT_TEST_FILES
        ${UNIT_TEST_FILES}
        tests/Main.cpp
        tests/TestGrid.cpp
        tests/TestGridCoord.cpp
        tests/TestGridbrain.cpp
        tests/TestGridbrainMutations.cpp
        tests/TestGridbrainRecombine.cpp
        tests/TestGridbrainComponents.cpp
        tests/TestGridbrainComponentSet.cpp
        )

########### Include dirs ###########

INCLUDE_DIRECTORIES("UnitTest++")
INCLUDE_DIRECTORIES("mtwist")
INCLUDE_DIRECTORIES("include")

########### Targets ###########

ADD_LIBRARY(gridbrain SHARED ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(llove llove ${Artist_LIBRARIES} ${Lua_LIBRARIES})

ADD_EXECUTABLE(test ${UNIT_TEST_FILES})
SET(LINK_LIBS ${LINK_LIBS})

ADD_CUSTOM_COMMAND(TARGET test POST_BUILD COMMAND "./test")

ADD_DEFINITIONS(${DEFINITIONS})

TARGET_LINK_LIBRARIES(gridbrain ${LINK_LIBS})
TARGET_LINK_LIBRARIES(test gridbrain ${LINK_LIBS})

