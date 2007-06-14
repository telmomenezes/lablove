#include "QuickGUIListItem.h"
#include "QuickGUIList.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	ListItem::ListItem(const Ogre::String& name, const Ogre::Vector4& dimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,"",overlayContainer,ParentWidget),
		mImage(0),
		mButton(0)
	{
		mWidgetType = Widget::QGUI_TYPE_LISTITEM;
		setCharacterHeight(1.0);
	}
	
	ListItem::~ListItem()
	{
		Widget::removeAndDestroyAllChildWidgets();

		mImage = NULL;
		mButton = NULL;
	}

	NStateButton* ListItem::addNStateButton(const Ogre::Vector4& dimensions)
	{
		if(mButton != NULL) return NULL;

		mButton = new NStateButton(mInstanceName+".NStateButton",dimensions,mChildrenContainer,this);
		mButton->addEventHandler(Widget::QGUI_EVENT_DEACTIVATED,&ListItem::evtHndlr_hideMenus,dynamic_cast<ListItem*>(this));
		mButton->addEventHandler(Widget::QGUI_EVENT_MOUSE_BUTTON_UP,&ListItem::evtHndlr_hideMenus,dynamic_cast<ListItem*>(this));
		mButton->setZOrderOffset(1,false);
		if(!mVisible) mButton->hide();
		Widget::_addChildWidget(mButton);

		return mButton;
	}

	Image* ListItem::addImage(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if(mImage != NULL) return NULL;

		mImage = new Image(mInstanceName+".Image",dimensions,material,mChildrenContainer,this);
		mImage->addEventHandler(Widget::QGUI_EVENT_DEACTIVATED,&ListItem::evtHndlr_hideMenus,dynamic_cast<ListItem*>(this));
		mImage->setZOrderOffset(1,false);
		if(!mVisible) mImage->hide();
		Widget::_addChildWidget(mImage);

		return mImage;
	}

	void ListItem::deactivate(EventArgs& e)
	{
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return;

		if(mParentWidget) mParentWidget->deactivate(e);

		Label::deactivate(e);
	}

	bool ListItem::evtHndlr_hideMenus(const EventArgs& e)
	{
		deactivate(const_cast<EventArgs&>(e));

		return true;
	}

	NStateButton* ListItem::getNStateButton()
	{
		return mButton;
	}

	Image* ListItem::getImage()
	{
		return mImage;
	}

	bool ListItem::onMouseButtonUp(MouseEventArgs& e)
	{
		bool retVal = Label::onMouseButtonUp(e);
		// When the widget or any of its child widgets receive the mouse up event, 
		// hide the menu containing this list item.  This is performed in Menu::onMouseButtonUp
		e.handled = false;
		if(mParentWidget) mParentWidget->onMouseButtonUp(e);
		return retVal;
	}

	bool ListItem::onMouseEnters(MouseEventArgs& e)
	{
		e.widget = this;
		List* parentList = dynamic_cast<List*>(mParentWidget);

		parentList->highlightListItem(this);

		return Label::onMouseEnters(e);
	}

	bool ListItem::onMouseLeaves(MouseEventArgs& e)
	{
		e.widget = this;
		List* parentList = dynamic_cast<List*>(mParentWidget);

		parentList->hideHighlight();

		return Label::onMouseLeaves(e);
	}

	void ListItem::removeNStateButton()
	{
		if(mButton == NULL) return;

		Widget::_removeChildWidget(mButton);
		delete mButton;
		mButton = NULL;
	}

	void ListItem::removeImage()
	{
		if(mImage == NULL) return;

		Widget::_removeChildWidget(mImage);
		delete mImage;
		mImage = NULL;
	}
}