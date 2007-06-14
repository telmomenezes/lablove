#ifndef QUICKGUILISTITEM_H
#define QUICKGUILISTITEM_H

#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUINStateButton.h"
#include "QuickGUIPrerequisites.h"

namespace QuickGUI
{
	/** Represents an element of a List.
		@remarks
		List Items can contain text, a button, and an image.
		@note
		Lists Items are meant to be created by Lists.
	*/
	class ListItem :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x pos, y pos, width and height of the widget.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		ListItem(const Ogre::String& name, const Ogre::Vector4& dimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~ListItem();

		/**
		* Add an NStateButton to the ListItem.  Useful examples: properties - (...), checkbox - [x]
		* Note that clicking this button doesn't cause the list to be hidden.
		*/
		NStateButton* addNStateButton(const Ogre::Vector4& dimensions);
		/**
		* Add an Image to the ListItem.
		*/
		Image* addImage(const Ogre::Vector4& dimensions, const Ogre::String& material);

		/**
		* Default Event Handler called when widget is deactivated.
		*/
		void deactivate(EventArgs& e);
		/**
		* Event Handler used to hide all parent Lists, when a ListItem is clicked.
		*/
		bool evtHndlr_hideMenus(const EventArgs& e);

		/**
		* Returns a pointer to the NStateButton, if it exists.
		*/
		NStateButton* getNStateButton();
		/**
		* Returns a pointer to the Image, if it exists.
		*/
		Image* getImage();

		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonUp(MouseEventArgs& e);
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
		* Remove (and destroy) the NStateButton.
		* No exception is thrown if NStateButton does not exist.
		*/
		void removeNStateButton();
		/**
		* Remove (and destroy) the Image.
		* No exception is thrown if Image does not exist.
		*/
		void removeImage();

	protected:
		Image* mImage;
		NStateButton* mButton;
	};
}

#endif
