#ifndef QUICKGUIMOUSECURSOR_H
#define QUICKGUIMOUSECURSOR_H

#include "QuickGUIPrerequisites.h"

namespace QuickGUI
{
	/** Mouse representation inside the primary render window
		@remarks
		Through Ogre materials and GUIManager input handling, the
		mouse cursor gets updated and repositioned, simulating a mouse.
		@note
		MouseCursor is a Singleton.
	*/
	class MouseCursor :
		public Ogre::Singleton<MouseCursor>
	{
	public:
		/** Constructor
            @param
                dimensions The x Position, y Position, width, and height of the window in Relative Dimensions.
			@param
				material Ogre material defining the cursor image.
			@param
				RenderWidthInPixels ...
			@param
				RenderHeightInPixels ...
        */
		MouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material, const unsigned int& RenderWidthInPixels, const unsigned int& RenderHeightInPixels);
		virtual ~MouseCursor();
		/** Returns the Listener singleton object */
		static MouseCursor& getSingleton();
		/** Returns a pointer to the Listener singleton object */
		static MouseCursor* getSingletonPtr();

		/**
		* Updates the mouse constraints
		*/
		void _updateWindowDimensions( const unsigned int& RenderWidthInPixels, const unsigned int& RenderHeightInPixels);
		/**
		* Returns true if cursor is set to go invisible when mouse is off the screen, false otherwise.
		*/
		bool getHideWhenOffScreen();
		Ogre::Vector2 getPixelPosition();
		Ogre::Vector2 getRelativePosition();
		Ogre::Vector2 getRelativeSize();
		/**
		* Hides the cursor.
		*/
		void hide();
		/**
		* Hides the cursor, but does not record the mouse is hidden.  Used for automatic hiding,
		* when mouse goes outside of screen.  When it comes back into window, we need to use mVisible
		* variable to tell us if user intends the mouse to be shown in window or not.
		*/
		void _hide();
		
		bool isVisible();
		/**
		* Applies the pixel offset to the current cursor position.
		*/
		void offsetPosition(const int& x_offset_in_pixels, const int& y_offset_in_pixels);
		/**
		* Returns true if the mouse if on the bottom border of the screen, false otherwise.
		*/
		bool mouseOnBotBorder();
		/**
		* Returns true if the mouse if on the left border of the screen, false otherwise.
		*/
		bool mouseOnLeftBorder();
		/**
		* Returns true if the mouse if on the right border of the screen, false otherwise.
		*/
		bool mouseOnRightBorder();
		/**
		* Returns true if the mouse if on the top border of the screen, false otherwise.
		*/
		bool mouseOnTopBorder();
		/**
		* if toggled, the cursor will become invisible when the mouse moves away from the primary render window.
		* only useful if you have the mouse created with the DISCL_NONEXCLUSIVE option. (Refer to OIS creation of the mouse)
		*/
		void setHideCursorWhenOSCursorOffscreen(bool hide);
		/**
		* Sets the Ogre material defining the mouse
		*/
		void setMaterial(const Ogre::String& material);
		/**
		* Sets the position of the mouse cursor on the screen, in pixel coordinates.
		*/
		void setPosition(const int& xPixelPosition, const int& yPixelPosition);
		/**
		* Sets the Size of the mouse cursor on the screen, in pixel dimensions.
		*/
		void setSize(const int& widthInPixels, const int& heightInPixels);
		/**
		* Shows the cursor.
		*/
		void show();

	protected:
		// Default material.
		Ogre::String	mMaterial;
		// Width and Height in pixels
		Ogre::Vector2	mPixelCursorDimensions;
		Ogre::Vector2	mRelativeCursorDimensions;
		unsigned int	mRenderWidthInPixels;
		unsigned int	mRenderHeightInPixels;
		
		// Specifies the area (in screen pixels) that the mouse can move around in.
		Ogre::Vector2	mConstraints;				
		bool			mOnTopBorder;
		bool			mOnBotBorder;
		bool			mOnLeftBorder;
		bool			mOnRightBorder;
		
		bool			mHideWhenOffScreen;
		bool			mVisible;
		Ogre::Vector2	mPosition;

		// Mouse Pointer Overlay
		Ogre::Overlay*			mMouseOverlay;
		Ogre::OverlayContainer* mMousePointerContainer;			

		/**
		* Prevents the mouse cursor from going out of bounds.
		* The position can't exceed the primary render window dimensions (pixels)
		*/
		void constrainPosition();
	};
}

#endif
