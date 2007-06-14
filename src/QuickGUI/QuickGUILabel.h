#ifndef QUICKGUILABEL_H
#define QUICKGUILABEL_H

#include "QuickGUIPrerequisites.h"
#include "QuickGUIText.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** Represents a traditional Label.
		@remarks
		Labels are QuickGUI's method to showing text.
		@note
		Labels must be created by the Window class.
	*/
	class Label :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Label(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Label();

		/**
		* When user has changed the font, character height, or text,
		* the label must be updated and aligned according to its parent.
		*/
		void _notifyTextChanged();
		/**
		* Aligns the child Label widget horizontally and vertically
		*/
		void alignText(HorizontalAlignment ha, VerticalAlignment va);
		/**
		* Gets a handle to the Text widget used for this Widget.
		*/
		Text* getTextWidget();

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void activate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		virtual void deactivate(EventArgs& e);

	protected:
		// Default Label material, displayed in its original state.
		Ogre::String						mMaterial;

		Text*								mTextWidget;
	};
}

#endif
