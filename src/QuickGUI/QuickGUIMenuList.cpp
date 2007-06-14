#include "QuickGUIMenuList.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	MenuList::MenuList(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Button(name,dimensions,material+".button",overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_MENULIST;
		setCharacterHeight(0.8);

		// create list
		mList = new List(mInstanceName+".List",Ogre::Vector3(0,1.0,1.0),material+".list",getSheet()->getMenuContainer(),this);
		mList->setCharacterHeight(mCharacterHeight);
		mList->setFont(mFont);
		mList->setTextColor(mTextTopColor,mTextBotColor);
		mList->hide();
		// give the list the same zOrder as the button representing this menulist.
		mList->setZOrderOffset(mZOrderOffset,false);
		Widget::_addChildWidget(mList);
	}

	MenuList::~MenuList()
	{
		Widget::removeAndDestroyAllChildWidgets();
	}

	ListItem* MenuList::addListItem(const Ogre::UTFString& text)
	{
		return mList->addListItem(text);
	}

	ListItem* MenuList::addListItem(const Ogre::String& name, const Ogre::UTFString& text)
	{
		return mList->addListItem(name,text);
	}

	void MenuList::alignListItemText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mList->alignListItemText(ha,va);
	}

	void MenuList::deactivate(EventArgs& e)
	{
		// If the Mouse has clicked on any of the menu's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return;

		Button::deactivate(e);
	}

	ListItem* MenuList::getListItem(unsigned int index)
	{
		return mList->getListItem(index);
	}

	ListItem* MenuList::getListItem(const Ogre::String& name)
	{
		return mList->getListItem(name);
	}

	int MenuList::getNumberOfListItems()
	{
		return mList->getNumberOfListItems();
	}

	void MenuList::hideHighlight()
	{
		mList->hideHighlight();
	}

	void MenuList::highlightListItem(ListItem* i)
	{
		mList->highlightListItem(i);
	}

	void MenuList::hideList()
	{
		mList->hide();
	}

	bool MenuList::isListVisible()
	{
		return mList->isVisible();
	}

	bool MenuList::isMouseOverButton()
	{
		return isPointWithinBounds(MouseCursor::getSingleton().getPixelPosition());
	}

	void MenuList::removeListItem(unsigned int index)
	{
		mList->removeListItem(index);
	}

	void MenuList::setListCharacterHeight(const Ogre::Real& relativeHeight)
	{
		mList->setCharacterHeight(relativeHeight);
	}

	void MenuList::setListHighlightMaterial(const Ogre::String& material)
	{
		mList->setHighlightMaterial(material);
	}

	void MenuList::setListWidth(const Ogre::Real& relativeWidth)
	{
		// make it seem like the width is relative to the menu, and not the menu list
		Ogre::Real numPixels = relativeWidth * mParentWidget->getPixelDimensions().z;
		mList->setWidth(numPixels / mPixelDimensions.z);
	}

	void MenuList::showList()
	{
		mList->show();
	}
}
