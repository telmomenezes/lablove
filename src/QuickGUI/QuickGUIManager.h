#ifndef QUICKGUIMANAGER_H
#define QUICKGUIMANAGER_H

#include "QuickGUIKeyCode.h"
#include "QuickGUIMouseButtonID.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUIPrerequisites.h"
#include "QuickGUISheet.h"

#include <list>
#include <time.h>
#include <utility>
#include <ctype.h>

namespace QuickGUI
{
	/** Manages Windows, Mouse Cursor, and Input
		@remarks
		The most important class of QuickGUI, responsible for creating and
		destroying Windows, updating the Mouse Cursor, and handling input.
		GUIManager has a simple clearAll method, which cleans up all created
		widgets.  This supports multiple game states that have differing GUI
		Layouts.
		@note
		GUIManager is a Singleton, and frequently accessed by widgets, for
		information on the rendering window, default font, text color, character
		height, or setting focus to itself. (Window Widget does this)
		@note
		GUIManager allows 5 zOrder per window, and manages Window zOrdering so
		that windows and widgets are rendered properly on top of each other.
	*/
	class GUIManager :
		public Ogre::Singleton<GUIManager>
	{
	public:
		/** Constructor
            @param
                RenderWindowWidth Width of the primary render window in Pixels.
            @param
                RenderWindowHeight Height of the primary render window in Pixels.
        */
        GUIManager(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight);
		/** Standard Destructor. */
		virtual ~GUIManager();
		/** Returns the Listener singleton object */
		static GUIManager& getSingleton();
		/** Returns a pointer to the Listener singleton object */
		static GUIManager* getSingletonPtr();

		/**
		* Updates MouseCursor class and all Windows that the window dimensions have changed.
		*/
		void _notifyWindowDimensions(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight);
		/**
		* Iterates through Window List and destroys it, which properly destroys all child widgets.
		* NOTE: mouse cursor object is also destroyed.
		*/
		void clearAll();
		/** Create a Mouse Cursor representing the Mouse
            @param
                dimensions The relative x Position, y Position, width, and height of the cursor.
            @param
                material Ogre material to define the cursor image.
        */
		MouseCursor* createMouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material);
		/** Create a Mouse Cursor representing the Mouse
            @param
                dimensions The relative x Position, y Position, width, and height of the cursor.
            @note
				Default Skin material for mouse cursor will be applied.
        */
		MouseCursor* createMouseCursor(const Ogre::Vector2& dimensions);

		Sheet* createSheet(const Ogre::String& name, const Ogre::String& material);
		Sheet* createSheet(const Ogre::String& name);
		Sheet* createSheet();

		/**
		* Destroys the Mouse Cursor - done in desctructor
		*/
		void destroyMouseCursor();
		/** Destroys a Window and all child widgets that exist
		    @param
				name Name of the window to destroy.
			@note
				name can also be reference name given to the window.
			@note 
				no exception is thrown if window does not exist
		*/
		void destroySheet(const Ogre::String& name);
		/** Destroys a Window and all child widgets that exist
		    @param
				window Window to destroy.
		*/
		void destroySheet(Sheet* sheet);

		/**
		* Returns the sheet currently being used, whether shown or hidden.
		*/
		Sheet* getActiveSheet();
		/**
		* Returns the default sheet, automatically created with the GUI manager.
		*/
		Sheet* getDefaultSheet();

		MouseCursor* getMouseCursor();
		Widget* getMouseOverWidget();
		/**
		* Width / Height.  The Width and Height are not gotten directly
		* from the Render Window, so updates to the window dimensions should notify
		* the manager, ie _notifyWindowDimensions(...).
		*/
		Ogre::Real getRenderWindowAspectRatio();
		/**
		* Get primary render window width in pixels
		*/
		unsigned int getRenderWindowWidth();
		/**
		* Get primary render window height in pixels
		*/
		unsigned int getRenderWindowHeight();

		/**
		* Iterates through sheet list and returns the Sheet with the
		* matching name.  Null if no match found.
		*/
		Sheet* getSheet(const Ogre::String& name);

		/**
		* Useful for Text Input Widgets, like the TextBox
		*/
		bool injectChar(char c);
		bool injectKeyDown(const KeyCode& kc);
		bool injectKeyUp(const KeyCode& kc);
		
		bool injectMouseButtonDown(const MouseButtonID& button);
		bool injectMouseButtonUp(const MouseButtonID& button);
		/**
		* Injection when the mouse leaves the primary render window
		*/
		bool injectMouseLeaves(void);
		bool injectMouseMove(const int& xPixelOffset, const int& yPixelOffset);
		bool injectMousePosition(const int& xPixelPosition, const int& yPixelPosition);
		bool injectMouseWheelChange(float delta);

		void injectTime(Ogre::Real time);
		/**
		* Hides all other sheets and shows this one.
		*/
		void setActiveSheet(Sheet* s);

		/**
		* Checks if the desired widget name already exists.  If it already exists,
		* false is returned.  Otherwise, if addToList is true, the name is added to
		* list of used Widget names, and true is returned.
		*/
		bool validWidgetName(const Ogre::String& name, bool addToList = true);

	protected:
		int						mRenderWindowWidth;
		int						mRenderWindowHeight;

		MouseCursor*			mMouseCursor;

		Ogre::StringVector		mWidgetNames;

		QuickGUI::Sheet*		mDefaultSheet;
		// Sheet currently being shown.
		QuickGUI::Sheet*		mActiveSheet;
		// Includes the Default Sheet.
		std::list<Sheet*>		mSheets;
		int						mAutoNameSheetCounter;

		time_t					mClickTimeout;	// max number of seconds a click can be performed in
		time_t					mMouseButtonTimings[8];
		// Keep track of mouse button down/up and on what widget.  This prevents left mouse button down on button A,
		// moving the mouse to button B, and releasing the left mouse button, causing button B to be pressed. (example)
		Widget*					mMouseButtonDown[8];

		Widget*					mWidgetContainingMouse;
		// Stores reference to last clicked Widget.
		Widget*					mActiveWidget;

		Sheet* _createSheet(const Ogre::String& name, const Ogre::String& material);
    };
}

#endif
