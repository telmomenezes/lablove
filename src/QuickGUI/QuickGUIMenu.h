#ifndef QUICKGUIMENU_H
#define QUICKGUIMENU_H

#include "QuickGUILabel.h"
#include "QuickGUIMenuList.h"
#include "QuickGUIPrerequisites.h"

#include <utility>
#include <vector>

namespace QuickGUI
{
	/** Represents a Menu.
		@remarks
		Menus are empty be default.  They do not become useful
		until adding MenuStrip widgets, which contain ListItem widgets.
		@note
		Menus must be created by the Window widget.
		@note
		Menus do not contain any text.
	*/
	class Menu :
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
		Menu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Menu();

		/**
		* Adds a menu list.
		*/
		MenuList* addMenuList(const Ogre::String& name, const Ogre::UTFString& text, Ogre::Real relXPos, Ogre::Real relXSize, const Ogre::String& material);
		/**
		* Adds a menu list.  Name is generated.
		*/
		MenuList* addMenuList(const Ogre::UTFString& text, Ogre::Real relXPos, Ogre::Real relXSize, const Ogre::String& material);
		/**
		* Adds a menu list.  Name is generated. Default Material applied.
		*/
		MenuList* addMenuList(const Ogre::UTFString& text, Ogre::Real relXPos, Ogre::Real relXSize);

		/**
		* Removes and Destroys all Child Lists, and their corresponding Anchor Button.
		*/
		void clearAllMenuLists();

		/**
		* Default Event Handler called when widget is deactivated.
		*/
		void deactivate(EventArgs& e);

		/**
		* Event Handler used to hide all child Lists, when a ListItem is clicked.
		*/
		bool evtHndlr_hideMenuList(const EventArgs& e);
		/**
		* Called whenever a child list creates a child list item.
		* Used to add MouseEnter and MouseLeave event handlers to List Item, 
		* for selection highlighting.
		*/
		//bool evtHndlr_listItemCreated(const EventArgs& e);
		//bool evtHndlr_listItemMouseEnters(const EventArgs& e);
		//bool evtHndlr_listItemMouseLeaves(const EventArgs& e);
		bool evtHndlr_toggleMenuList(const EventArgs& e);

		/**
		* Gets a List from the Menu.  No exception is thrown
		* if the index is out of bounds.
		*/
		MenuList* getMenuList(unsigned int index);
		MenuList* getMenuList(const Ogre::String& name);

		/**
		* Hides All Child Menus.
		*/
		void hideMenus();

		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonUp(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_MOVE event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseMoved(MouseEventArgs& e);

	protected:
		std::vector<MenuList*> mMenuLists;

		// If user clicks on a menu, it drops down and sets this variable to true;
		// Mousing over other menu anchor buttons will switch to their menus. (Windows XP menu functionality)
		bool mShowMenus;

		int mMenuListCounter;
	};
}

#endif
