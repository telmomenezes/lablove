#ifndef QUICKGUIBUTTON_H
#define QUICKGUIBUTTON_H

#include "QuickGUILabel.h"
#include "QuickGUIPrerequisites.h"

namespace QuickGUI
{
	/** Represents a traditional button.
	@remarks
	The Button class requires at least 3 materials to define it's image:
	Normal State, Mouse Over, and Mouse Down.  For example, if you pass
	the constructor "sample.button" as its arguement for the material,
	the class will expect "sample.button.over" and "sample.button.down"
	to exist.  By default, buttons will handle mouse over and mouse down
	events, and change the image appropriately.
	@note
	In order to get the most use out of buttons, you will need to add
	an event handler.
	@note
	Buttons are meant to be created via the Window widget.
	*/
	class Button :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Button(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Button();

		/**
		* Useful when you want to simulate the button being pressed down by the mouse.
		* If you actually want to click the mouse, use the mouse, or call onMouseButtonDown.
		*/
		void applyButtonDownMaterial();
		/**
		* If supplying a method to simulate the button being pressed down, we need a method
		* to restore the button to the normal looking state.
		*/
		void applyDefaultMaterial();

		/**
		* Default Handler for the QGUI_EVENT_MOUSE_ENTER event.  material.over is applied to the button
		* image. If not handled, it will be passed to the parent widget 
		* (if exists)
		*/
		bool onMouseEnters(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_LEAVE event.  material is applied to the button
		* image. If not handled, it will be passed to the parent widget 
		* (if exists)
		*/
		bool onMouseLeaves(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		virtual bool onMouseButtonUp(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonDown(MouseEventArgs& e);

	protected:
		bool mOverMaterialExists;
		bool mDownMaterialExists;
	};
}

#endif
