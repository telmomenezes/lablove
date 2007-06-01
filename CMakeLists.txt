PROJECT(lovelab)

OPTION(GRAPHICS "Build the project with graphical visualization" ON)

IF(GRAPHICS)
	INCLUDE(${CMAKE_ROOT}/Modules/FindSDL.cmake)
	INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
ENDIF(GRAPHICS)

#SET(CMAKE_VERBOSE_MAKEFILE "ON")

SET(CMAKE_CXX_FLAGS "-Wno-deprecated -O2")

ADD_EXECUTABLE(lovelab src/main.cpp src/LoveLab.cpp src/Simulation.cpp
src/SimSimple2D.cpp src/ScriptableObject.cpp src/SimulationObject.cpp src/ObjectSimple2D.cpp
src/PlantSimple2D.cpp src/AnimatSimple2D.cpp src/functions.cpp
src/Gridbrain.cpp src/Grid.cpp src/GridbrainComponent
src/GridbrainComponentSet.cpp src/PopulationDynamics.cpp
src/PopDynGenerations.cpp src/PopDynFixedSpecies.cpp src/random.cpp
src/Font.cpp src/Molecule.cpp src/MoleculeRGB.cpp src/Chemistry.cpp)

TARGET_LINK_LIBRARIES(lovelab lua dl)

IF(GRAPHICS)
	ADD_DEFINITIONS(-D__LOVE_GRAPHICS)
	TARGET_LINK_LIBRARIES(lovelab SDL GL GLU)
ENDIF(GRAPHICS)

