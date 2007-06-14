#include "QuickGUIComboBox.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ComboBox::ComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,overlayContainer,ParentWidget),
		mListItemHorizontalAlignment(QuickGUI::QGUI_HA_MID),
		mListItemVerticalAlignment(QuickGUI::QGUI_VA_MID)
	{
		mWidgetType = Widget::QGUI_TYPE_COMBOBOX;

		// Create CloseButton - remember to position it relative to it's parent (TitleBar)
		// Height of the Title Bar
		Ogre::Real height = (mAbsoluteDimensions.w / mAbsoluteDimensions.z);
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonWidth = (height * buttonHeight);
		// Make a 5 pixel buffer
		Ogre::Real buffer = 5.0 / mPixelDimensions.z;
		Ogre::Vector4 bDimensions = Ogre::Vector4((1 - (buttonWidth + buffer)),0.1,buttonWidth,buttonHeight);
		mButton = new Button(mInstanceName+".ComboButton",bDimensions,mWidgetMaterial+".button",mChildrenContainer,this);
		mButton->setZOrderOffset(1);
		Widget::_addChildWidget(mButton);

		// Make a 15 pixel buffer for the Label
		Ogre::Real bigBuffer = 15.0 / mPixelDimensions.z;
		mLabel = new Label(mInstanceName+".Label",Ogre::Vector4(bigBuffer,0,(1 - (buttonWidth + bigBuffer)),1),"",mChildrenContainer,this);
		mLabel->addEventHandler(Widget::QGUI_EVENT_DEACTIVATED,&ComboBox::deactivate,dynamic_cast<ComboBox*>(this));
		mLabel->setZOrderOffset(1);
		Widget::_addChildWidget(mLabel);

		mList = new List(mInstanceName+".List",Ogre::Vector3(0,1.0,1.0),mWidgetMaterial+".list",getSheet()->getMenuContainer(),this);
		mList->setCharacterHeight(mCharacterHeight);
		mList->setFont(mFont);
		mList->setTextColor(mTextTopColor,mTextBotColor);
		mList->hide();
		int derivedZOrder = getSheet()->getMenuOverlayZOrder() + 1;
		mList->setZOrderOffset(derivedZOrder - getWindow()->getZOrder(),false);
		Widget::_addChildWidget(mList);
	}

	ComboBox::~ComboBox()
	{
		Widget::removeAndDestroyAllChildWidgets();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			delete (*it);
		mOnSelectUserEventHandlers.clear();
	}

	void ComboBox::addOnSelectionEventHandler(MemberFunctionSlot* function)
	{
		mOnSelectUserEventHandlers.push_back(function);
	}

	void ComboBox::addListItem(const Ogre::UTFString& text)
	{
		ListItem* newListItem = mList->addListItem(text);
	}

	void ComboBox::addListItem(const Ogre::String& name, const Ogre::UTFString& text)
	{
		ListItem* newListItem = mList->addListItem(name,text);
	}

	void ComboBox::alignListItemText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mListItemHorizontalAlignment = ha;
		mListItemVerticalAlignment = va;

		mList->alignListItemText(mListItemHorizontalAlignment,mListItemVerticalAlignment);
	}

	int ComboBox::getNumberOfListItems()
	{
		return mList->getNumberOfListItems();
	}

	// Called when the user clicks outside the widget
	void ComboBox::deactivate(EventArgs& e)
	{
		// Restore default material
		mOverlayElement->setMaterialName(mWidgetMaterial);
		mButton->applyDefaultMaterial();
		// If the Mouse has clicked on the ComboBox's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		Ogre::Vector2 p = MouseCursor::getSingleton().getPixelPosition();
		if(getTargetWidget(p) != NULL) return;
		
		mList->hide();

		Label::deactivate(e);
	}

	bool ComboBox::deactivate(const EventArgs& e)
	{
		// Restore default material
		mOverlayElement->setMaterialName(mWidgetMaterial);
		mButton->applyDefaultMaterial();
		// If the Mouse has clicked on the ComboBox's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		Ogre::Vector2 p = MouseCursor::getSingleton().getPixelPosition();
		if(getTargetWidget(p) != NULL) return true;
		
		mList->hide();

		return true;
	}

	bool ComboBox::onMouseEnters(MouseEventArgs& e)
	{
		if(!mList->isVisible()) mOverlayElement->setMaterialName(mWidgetMaterial+".over");
		if(mList->isVisible()) mButton->applyButtonDownMaterial();

		return Label::onMouseEnters(e);
	}

	bool ComboBox::onMouseLeaves(MouseEventArgs& e)
	{
		if(!mList->isVisible()) mOverlayElement->setMaterialName(mWidgetMaterial);

		return Label::onMouseLeaves(e);
	}

	bool ComboBox::onMouseButtonDown(MouseEventArgs& e)
	{
		if( e.button == MB_Left )
		{
			Widget* w = mList->getTargetWidget(e.position);
			if( w != NULL)
			{
				e.widget = w;
				return onSelection(e);
			}
			else
			{
				if(!mList->isVisible())
				{
					// apply button ".down" material
					mOverlayElement->setMaterialName(mWidgetMaterial+".down");
					mButton->applyButtonDownMaterial();
					mList->show();
				}
				else
				{
					mOverlayElement->setMaterialName(mWidgetMaterial+".over");
					mButton->applyDefaultMaterial();
					mList->hide();
				}
			}
		}

		return Label::onMouseButtonDown(e);
	}

	bool ComboBox::onMouseButtonUp(MouseEventArgs& e)
	{
		if(mList->isVisible()) mButton->applyButtonDownMaterial();

		return Label::onMouseButtonUp(e);
	}

	bool ComboBox::onSelection(WidgetEventArgs& e)
	{
		setText(e.widget->getText());
		mOverlayElement->setMaterialName(mWidgetMaterial);
		mButton->applyDefaultMaterial();
		mList->hide();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	void ComboBox::removeListItem(unsigned int index)
	{
		mList->removeListItem(index);
	}

	void ComboBox::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		Label::setCharacterHeight(relativeHeight);
		mList->setCharacterHeight(relativeHeight);
	}

	void ComboBox::setText(const Ogre::UTFString& text)
	{
		// If text is bigger than combobox width, append "..." to a fitting portion of the text
		mText = text;	// store original text so we can retrieve it.

		mLabel->setFont(mFont);
		mLabel->setCharacterHeight(mCharacterHeight);
		mLabel->setTextColor(mTextTopColor,mTextBotColor);
		mLabel->setText(mText);
		mLabel->alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void ComboBox::setDropListHighlightMaterial(const Ogre::String& material)
	{
		mList->setHighlightMaterial(material);
	}

	void ComboBox::setDropListWidth(const Ogre::Real& relativeWidth)
	{
		mList->setWidth(relativeWidth);
	}

	void ComboBox::show()
	{
		Label::show();
		mList->hide();
	}
}
