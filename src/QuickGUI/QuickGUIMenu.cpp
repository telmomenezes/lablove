#include "QuickGUIMenu.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Menu::Menu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,overlayContainer,ParentWidget),
		mMenuListCounter(0),
		mShowMenus(0)
	{
		mWidgetType = Widget::QGUI_TYPE_MENU;
		mMenuLists.clear();
	}

	Menu::~Menu()
	{
		clearAllMenuLists();
	}

	MenuList* Menu::addMenuList(const Ogre::String& name, const Ogre::UTFString& text, Ogre::Real relXPos, Ogre::Real relXSize, const Ogre::String& material)
	{
		MenuList* newMenuList = new MenuList(name,Ogre::Vector4(relXPos,0,relXSize,1),material,mChildrenContainer,this);
		newMenuList->setText(text);
		newMenuList->addEventHandler(Widget::QGUI_EVENT_MOUSE_BUTTON_DOWN,&Menu::evtHndlr_toggleMenuList,dynamic_cast<Menu*>(this));
		newMenuList->addEventHandler(Widget::QGUI_EVENT_DEACTIVATED,&Menu::evtHndlr_hideMenuList,dynamic_cast<Menu*>(this));
		newMenuList->setZOrderOffset(1);
		mMenuLists.push_back(newMenuList);
		Widget::_addChildWidget(newMenuList);

		return newMenuList;
	}

	MenuList* Menu::addMenuList(const Ogre::UTFString& text, Ogre::Real relXPos, Ogre::Real relXSize, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName+".MenuList" + Ogre::StringConverter::toString(mMenuListCounter);
		++mMenuListCounter;

		return addMenuList(name,text,relXPos,relXSize,material);
	}

	MenuList* Menu::addMenuList(const Ogre::UTFString& text, Ogre::Real relXPos, Ogre::Real relXSize)
	{
		Ogre::String name = mInstanceName+".MenuList" + Ogre::StringConverter::toString(mMenuListCounter);
		++mMenuListCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".menulist";

		return addMenuList(name,text,relXPos,relXSize,material);
	}

	void Menu::clearAllMenuLists()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mMenuLists.clear();
	}

	// Called when the user clicks outside the widget
	void Menu::deactivate(EventArgs& e)
	{
		// If the Mouse has clicked on any of the menu's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return;
		
		hideMenus();

		Label::deactivate(e);
	}

	bool Menu::evtHndlr_hideMenuList(const EventArgs& e)
	{
		// If the Mouse has clicked on any of the menu's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return true;
		
		hideMenus();

		return true;
	}

	bool Menu::evtHndlr_toggleMenuList(const EventArgs& e)
	{
		const EventArgs* temp = &e;
		const MouseEventArgs* mea = static_cast<const MouseEventArgs*>(temp);
		if( mea->button == QuickGUI::MB_Left )
		{
			// get index of button clicked
			bool buttonClicked = false;
			int index = 0;
			std::vector<MenuList*>::iterator it;
			for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			{
				if( (*it)->isMouseOverButton() ) 
				{
					buttonClicked = true;
					break;
				}
				
				++index;
			}
			if(!buttonClicked) return true;

			// get target list corresponding to the clicked anchor (button) and toggle visibility
			if(mMenuLists[index]->isListVisible()) 
			{
				mMenuLists[index]->hideList();
				mShowMenus = false;
			}
			else 
			{
				// Hide all menus and show just the desired one
				hideMenus();
				mMenuLists[index]->showList();
				mShowMenus = true;
			}
		}
		
		return true;
	}

	MenuList* Menu::getMenuList(unsigned int index)
	{
		if( (static_cast<int>(mMenuLists.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mMenuLists[index];
	}

	MenuList* Menu::getMenuList(const Ogre::String& name)
	{
		std::vector<MenuList*>::iterator it;
		for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	void Menu::hideMenus()
	{
		std::vector<MenuList*>::iterator it;
		for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			(*it)->hideList();

		mShowMenus = false;
	}

	bool Menu::onMouseButtonUp(MouseEventArgs& e)
	{
		if( e.button == QuickGUI::MB_Left )
		{
			std::vector<MenuList*>::iterator it;
			for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			{
				if( (*it)->isMouseOverButton() ) break;
				else if( (*it)->getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL )
				{
					(*it)->hideList();
					mShowMenus = false;
					break;
				}
			}
		}

		return Label::onMouseButtonUp(e);
	}

	bool Menu::onMouseMoved(MouseEventArgs& e)
	{
		if(mShowMenus)
		{
			// get index of button clicked
			int index = 0;
			bool found = false;
			std::vector<MenuList*>::iterator it;
			for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			{
				if( (*it)->isPointWithinBounds(MouseCursor::getSingleton().getPixelPosition()) )
				{
					found = true;
					break;
				}
				
				++index;
			}

			if(found)
			{
				if(!mMenuLists[index]->isListVisible())
				{
					// Hide all menus and show just the desired one
					hideMenus();
					mMenuLists[index]->showList();
					mShowMenus = true;
				}
			}
		}

		return Label::onMouseMoved(e);
	}
}
