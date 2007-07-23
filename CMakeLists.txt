PROJECT(lablove)

OPTION(GRAPHICS "Build the project with graphical visualization" ON)
OPTION(DEBUG "Debug settings" OFF)

# TODO: look for or inplement a FindOgre.cmake module
IF(GRAPHICS)
	INCLUDE(${CMAKE_ROOT}/Modules/FindSDL.cmake)
	INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
	ADD_DEFINITIONS(-D__LAYER_SDLOPENGL)
ENDIF(GRAPHICS)

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
	src/Molecule.cpp
	src/MoleculeRGB.cpp
	src/Chemistry.cpp
	src/Statistics.cpp
	src/StatMedAvgMinMax.cpp
	src/StatTime.cpp)

IF(GRAPHICS)
	ADD_DEFINITIONS(-D__LABLOVE_WITH_GRAPHICS)
	SET(SOURCE_FILES ${SOURCE_FILES}
		src/layer/ScreenManager.cpp
		src/layer/Screen.cpp
		src/layer/ScreenSDLOpenGL.cpp)
ENDIF(GRAPHICS)

ADD_EXECUTABLE(lablove ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(lablove lua dl)

IF(GRAPHICS)
	TARGET_LINK_LIBRARIES(lablove SDL GL GLU)
ENDIF(GRAPHICS)

IF(UNIX)
	ADD_DEFINITIONS(-D__UNIX)
ENDIF(UNIX)

IF(WIN32)
	ADD_DEFINITIONS(-D__WIN32)
ENDIF(WIN32)
