#ifndef QUICKGUIWINDOW_H
#define QUICKGUIWINDOW_H

#include "QuickGUIPanel.h"
#include "QuickGUITitleBar.h"

namespace QuickGUI
{
	/** Represents a traditional Window.
		@remarks
		The Window class is able to create all other widgets,
		and for the majority of times, will be the medium used
		to display and use other widgets.  The Window class
		requires 0-5 material definitions, depending on the
		contstructor used. (Empty Window vs Default Window)
		ex: Original Material - "sample.window"
		Required:
			"sample.window.titlebar"
			"sample.window.titlebar.button"
			"sample.window.titlebar.button.over"
			"sample.window.titlebar.button.down"
		@note
		Windows must be created by the GUIManager.
	*/
	class Window :
		public Panel
	{
	public:
		/** Constructor - Default Window
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				parentWidget sheet used to create this window.
			@note
				Requires all 5 materials listed above.
			@note
				This Window will have a TitleBar
        */
		Window(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Widget* parentWidget);
		virtual ~Window();
		
		TitleBar* getTitleBar();

		void hide();
		/**
		* Hide, as an event handler.  Used as the default method for close button to hide window
		*/
		bool hide(const EventArgs& e);
		void hideCloseButton();
		void hideTitlebar();

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		void activate(EventArgs& e);

		/**
		* Convenience method to set the TitleBar's Label's caption.
		*/
		void setText(const Ogre::UTFString& text);
		void setTextColor(const Ogre::ColourValue& color);
		void setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor);
		/**
		* Convenience method to set the TitleBar's height.
		*/
		void setTitleBarHeight(Ogre::Real height);
		void setZOrder(int zOrder);
		void showCloseButton();
		void showTitlebar();

	protected:
		Ogre::Overlay*						mOverlay;

		bool								mTitleBarHidden;
		Ogre::OverlayContainer*				mTitleBarContainer;
		TitleBar*							mTitleBar;
	};
}

#endif
