#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	List::List(const Ogre::String& name, const Ogre::Vector3& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,Ogre::Vector4(dimensions.x,dimensions.y,dimensions.z,0),material,overlayContainer,ParentWidget),
		mAutoNameListItemCount(0),
		mAutoNameListCount(0),
		mListItemHorizontalAlignment(QuickGUI::QGUI_HA_MID),
		mListItemVerticalAlignment(QuickGUI::QGUI_VA_MID)
	{
		mWidgetType = Widget::QGUI_TYPE_LIST;

		mDefaultListItemHeight = mParentWidget->getAbsoluteDimensions().w;
		mHighlightMaterial = getSheet()->getDefaultSkin() + ".list.highlight";

		// create highlight container for the list
		mHighlightContainer = createOverlayContainer(mInstanceName+".HighlightContainer","");
		mChildrenContainer->addChildImpl(mHighlightContainer);

		mHighlightPanel = createPanelOverlayElement(mInstanceName+".HighlightPanel",mPixelDimensions,mHighlightMaterial);
		mHighlightContainer->addChild(mHighlightPanel);
		mHighlightPanel->show();

		mItems.clear();
		mChildLists.clear();
	}

	List::~List()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mItems.clear();
		mChildLists.clear();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		mHighlightContainer->removeChild(mHighlightPanel->getName());
		om->destroyOverlayElement(mHighlightPanel);
		mHighlightPanel = NULL;

		mChildrenContainer->removeChild(mHighlightContainer->getName());
		om->destroyOverlayElement(mHighlightContainer);
		mHighlightContainer = NULL;
	}

	ListItem* List::addListItem(const Ogre::UTFString& text)
	{
		Ogre::String name = mInstanceName + ".ListItem" + Ogre::StringConverter::toString(mAutoNameListItemCount);
		++mAutoNameListItemCount;

		return addListItem(name,text);
	}

	ListItem* List::addListItem(const Ogre::String& name, const Ogre::UTFString& text)
	{
		Ogre::Vector4 OriginalAbsoluteDimensions = mAbsoluteDimensions;

		// Calculate the current absolute height of the list
		Ogre::Real currentListHeight = 0.0;
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			currentListHeight = currentListHeight + (*it)->getAbsoluteDimensions().w;
		}

		// Grow the list to accept the new incoming list item
		Ogre::Real calculatedListHeight = currentListHeight + mDefaultListItemHeight;
		// funny, this should almost always be more than 1.0, which defies relative coordinates
		// IMPORTANT! Do not call _notifyDimensionsChanged, otherwise all child ListItems will have their dimensions auto adjusted.
		// We need to keep them as is, and later use the unchanged values to calculate new values, as the list height has changed.
		// Essentially, we want to maintain the ListItems' height while adjusting the List's height.
		mRelativeDimensions.w = calculatedListHeight / (mParentWidget->getAbsoluteDimensions().w);
		_updateDimensions(mRelativeDimensions);
		
		// Shrink the previous list Items back to the correct size
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			Ogre::Vector2 pos = (*it)->getAbsolutePosition();
			Ogre::Vector2 size = (*it)->getAbsoluteSize();

			(*it)->setDimensions(
				Ogre::Vector4(
					(pos.x - mAbsoluteDimensions.x) / mAbsoluteDimensions.z,
					(pos.y - mAbsoluteDimensions.y) / mAbsoluteDimensions.w,
					size.x / mAbsoluteDimensions.z,
					size.y / mAbsoluteDimensions.w
					)
				);
		}
		
		// Add the List Item
		ListItem* newListItem = new ListItem(name,Ogre::Vector4(0,currentListHeight / calculatedListHeight,1.0,mDefaultListItemHeight / mAbsoluteDimensions.w),mChildrenContainer,this);
		newListItem->setCharacterHeight(mCharacterHeight);
		newListItem->setFont(mFont);
		newListItem->setTextColor(mTextTopColor,mTextBotColor);
		newListItem->setText(text);
		newListItem->alignText(mListItemHorizontalAlignment,mListItemVerticalAlignment);
		newListItem->setZOrderOffset(1,false);
		mItems.push_back(newListItem);
		Widget::_addChildWidget(newListItem);
		if(!mVisible) newListItem->hide();

		// Important!  Even though we have set the dimensions, we need to update the overlay elements (of the list, specifically)
		_notifyDimensionsChanged();
		
		return newListItem;
	}

	void List::alignListItemText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mListItemHorizontalAlignment = ha;
		mListItemVerticalAlignment = va;

		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->alignText(mListItemHorizontalAlignment,mListItemVerticalAlignment);
		}
	}

	void List::deactivate(EventArgs& e)
	{
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return;

		if(mParentWidget) mParentWidget->deactivate(e);

		Label::deactivate(e);
	}

	ListItem* List::getListItem(unsigned int index)
	{
		if( (static_cast<int>(mItems.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mItems[index];
	}

	ListItem* List::getListItem(const Ogre::String& name)
	{
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	int List::getNumberOfListItems()
	{
		return static_cast<int>(mChildLists.size());
	}

	void List::hide()
	{
		mHighlightPanel->hide();

		Label::hide();
	}

	void List::hideHighlight()
	{
		mHighlightPanel->hide();
	}

	void List::highlightListItem(ListItem* i)
	{
		Ogre::Vector4 liPixelDimensions = i->getPixelDimensions();
		mHighlightPanel->setPosition(liPixelDimensions.x,liPixelDimensions.y);
		mHighlightPanel->setDimensions(liPixelDimensions.z,liPixelDimensions.w);
		mHighlightPanel->show();
	}

	void List::removeList(unsigned int index)
	{
		if( (static_cast<int>(mChildLists.size()) - 1) < static_cast<int>(index) ) return;
		
		int counter = 0;
		std::vector<List*>::iterator it;
		for( it = mChildLists.begin(); it != mChildLists.end(); ++it )
		{
			if( counter == index )
			{
				Widget::_removeChildWidget(*it);
				List* l = *(mChildLists.erase(it));
				delete l;
				return;
			}
			++counter;
		}
	}

	void List::removeListItem(unsigned int index)
	{
		if( (static_cast<int>(mChildLists.size()) - 1) < static_cast<int>(index) ) return;

		// Delete the List Item
		ListItem* li;
		int counter = 0;
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( counter == index )
			{
				Widget::_removeChildWidget(*it);
				li = (*it);
				it = mItems.erase(it);
			}

			++counter;
		}

		Ogre::Real FreeHeight = li->getAbsoluteDimensions().w;
		delete li;

		// IMPORTANT! Do not call _notifyDimensionsChanged, otherwise all child ListItems will have their dimensions auto adjusted.
		// We need to keep them as is, and later use the unchanged values to calculate new values, as the list height has changed.
		// Essentially, we want to maintain the ListItems' height while adjusting the List's height.
		mRelativeDimensions.w = (mAbsoluteDimensions.w - FreeHeight) / (mParentWidget->getAbsoluteDimensions().w);
		_updateDimensions(mRelativeDimensions);

		Ogre::Real newYPos = 0.0;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			Ogre::Vector2 pos = (*it)->getAbsolutePosition();
			pos.y = newYPos;
			Ogre::Vector2 size = (*it)->getAbsoluteSize();

			(*it)->setDimensions(
				Ogre::Vector4(
					(pos.x - mAbsoluteDimensions.x) / mAbsoluteDimensions.z,
					(pos.y) / mAbsoluteDimensions.w,
					size.x / mAbsoluteDimensions.z,
					size.y / mAbsoluteDimensions.w
					)
				);

			newYPos = newYPos + (*it)->getAbsoluteSize().y;
		}
	}

	void List::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		Label::setCharacterHeight(relativeHeight);
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->setCharacterHeight(relativeHeight);
		}
	}

	void List::setHighlightMaterial(const Ogre::String& material)
	{
		mHighlightMaterial = material;
		mHighlightPanel->setMaterialName(mHighlightMaterial);
	}

	void List::setWidth(const Ogre::Real& relativeWidth)
	{
		mRelativeDimensions.z = relativeWidth;
		Widget::_notifyDimensionsChanged();
	}

	void List::show()
	{
		mHighlightPanel->hide();

		Label::show();
	}
}