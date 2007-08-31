PROJECT(lablove)

OPTION(DEBUG "Debug settings" OFF)

# TODO: Create a cmake script to find the correct directory for pycasso
INCLUDE_DIRECTORIES("/usr/include/pycasso")

#SET(CMAKE_VERBOSE_MAKEFILE "ON")

IF(DEBUG)
	SET(CMAKE_CXX_FLAGS "-Wno-deprecated -g")
ELSE(DEBUG)
	SET(CMAKE_CXX_FLAGS "-Wno-deprecated -O3")
ENDIF(DEBUG)

ADD_SUBDIRECTORY(src)

SET(SOURCE_FILES
	src/main.cpp
	src/Lab.cpp
	src/Simulation.cpp
	src/SimSimple2D.cpp
	src/ScriptableObject.cpp
	src/SimulationObject.cpp
	src/ObjectSimple2D.cpp
	src/PlantSimple2D.cpp
	src/AnimatSimple2D.cpp
	src/functions.cpp
	src/Gridbrain.cpp
	src/Grid.cpp
	src/GridbrainComponent
	src/GridbrainComponentSet.cpp
	src/PopulationDynamics.cpp
	src/PopDynGenerations.cpp
	src/PopDynFixedSpecies.cpp
	src/random.cpp
	src/Symbol.cpp
	src/SymbolRGB.cpp
	src/SymbolTable.cpp
	src/Statistics.cpp
	src/StatMedAvgMinMax.cpp
	src/StatTime.cpp)

ADD_EXECUTABLE(lablove ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(lablove pycasso lua dl)

IF(UNIX)
	ADD_DEFINITIONS(-D__UNIX)
ENDIF(UNIX)

IF(WIN32)
	ADD_DEFINITIONS(-D__WIN32)
ENDIF(WIN32)
