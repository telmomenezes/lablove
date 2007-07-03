PROJECT(lablove)

OPTION(GRAPHICS "Build the project with graphical visualization" ON)
OPTION(DEBUG "Debug settings" OFF)

# TODO: look for or inplement a FindOgre.cmake module
IF(GRAPHICS)
	INCLUDE_DIRECTORIES("/usr/include/OGRE/")
	#INCLUDE(${CMAKE_ROOT}/Modules/FindOgre.cmake)
	#INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIR})
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
	src/StatMedAvgMinMax.cpp)

IF(GRAPHICS)
	ADD_DEFINITIONS(-D__LABLOVE_WITH_GRAPHICS)
	SET(SOURCE_FILES ${SOURCE_FILES} src/OgreApplication.cpp)
	SET(SOURCE_FILES ${SOURCE_FILES}
		src/QuickGUI/QuickGUIButton.cpp
		src/QuickGUI/QuickGUIMenu.cpp
		src/QuickGUI/QuickGUITextBox.cpp
		src/QuickGUI/QuickGUIComboBox.cpp
		src/QuickGUI/QuickGUIMenuList.cpp
		src/QuickGUI/QuickGUITextCursor.cpp
		src/QuickGUI/QuickGUIEventArgs.cpp
		src/QuickGUI/QuickGUIMouseCursor.cpp
		src/QuickGUI/QuickGUITitleBar.cpp
		src/QuickGUI/QuickGUIImage.cpp
		src/QuickGUI/QuickGUINStateButton.cpp
		src/QuickGUI/QuickGUIWidget.cpp
		src/QuickGUI/QuickGUILabel.cpp
		src/QuickGUI/QuickGUIPanel.cpp
		src/QuickGUI/QuickGUIWindow.cpp
		src/QuickGUI/QuickGUIList.cpp
		src/QuickGUI/QuickGUIProgressBar.cpp
		src/QuickGUI/QuickGUIListItem.cpp
		src/QuickGUI/QuickGUISheet.cpp
		src/QuickGUI/QuickGUIManager.cpp
		src/QuickGUI/QuickGUIText.cpp)
ENDIF(GRAPHICS)

ADD_EXECUTABLE(lablove ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(lablove lua dl)

IF(GRAPHICS)
	TARGET_LINK_LIBRARIES(lablove OgreMain OIS Cg freeimage z m dl)
ENDIF(GRAPHICS)

