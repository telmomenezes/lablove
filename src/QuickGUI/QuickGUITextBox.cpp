#include "QuickGUITextBox.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	TextBox::TextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget),
		mMaskUserInput(0),
		mBackSpaceDown(0),
		mBackSpaceTimer(0.0),
		mDeleteDown(0),
		mDeleteTimer(0.0),
		mLeftArrowDown(0),
		mRightArrowDown(0),
		mMoveCursorTimer(0.0),
		mCursorVisibilityTimer(0.0),
		mReadOnly(0),
		mInputMode(0)
	{
		mWidgetType = Widget::QGUI_TYPE_TEXTBOX;

		// Border Overlay gives us ability to assign material to TextBox border and Panel separately.
		mOverlayElement = createPanelOverlayElement(mInstanceName+".Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mOverlayElement);
		mOverlayElement->show();

		mCharacterHeight = 0.75;
		Ogre::Vector3 textDimensions = Ogre::Vector3(0,0,mCharacterHeight);
		// Label has no material, since it directly overlaps the textbox overlay element
		mTextWidget = new Text(mInstanceName+".Text",textDimensions,mChildrenContainer,this);
		mTextWidget->setTruncateMode(Text::LEFT);
		mTextWidget->setTruncationFeedback("");
		mTextWidget->setZOrderOffset(1);
		Widget::_addChildWidget(mTextWidget);

		mHorizontalAlignment = QGUI_HA_LEFT;
		mVerticalAlignment = QGUI_VA_MID;

		alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	TextBox::~TextBox()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mTextWidget = NULL;
	}

	void TextBox::_notifyTextChanged()
	{
		mTextWidget->setFont(mFont);
		mTextWidget->setCharacterHeight(mCharacterHeight);
		mTextWidget->setTextColor(mTextTopColor,mTextBotColor);
		mTextWidget->setText(mTextWidget->getText());

		alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void TextBox::addCharacter(Ogre::UTFString::unicode_char c)
	{
		if(mReadOnly) return;

		// Insert a character right before the text cursor.
		int index = mTextWidget->getCursorIndex();
		mText.insert(index,1,c);

		// Handle adding character to password box.
		Ogre::UTFString s;
		if(mMaskUserInput) s.append(mText.length(),mMaskSymbol);
		else s = mText;

		mTextWidget->setText(s);
		_notifyTextChanged();
		mTextWidget->incrementCursorIndex();
	}

	void TextBox::addOnEnterPressedEventHandler(MemberFunctionSlot* function)
	{
		mOnEnterPressedUserEventHandlers.push_back(function);
	}

	void TextBox::alignText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mHorizontalAlignment = ha;
		mVerticalAlignment = va;

		Ogre::Vector2 relativeLabelPos = mTextWidget->getRelativePosition();
		Ogre::Vector2 labelPos = mTextWidget->getAbsolutePosition();
		Ogre::Vector2 labelSize = mTextWidget->getAbsoluteSize();

		if( mHorizontalAlignment == QGUI_HA_LEFT ) 
		{
			Ogre::Real widthBuffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(widthBuffer,relativeLabelPos.y);
		}
		if( mHorizontalAlignment == QGUI_HA_MID )
		{
			mTextWidget->setPosition(((mAbsoluteDimensions.z / 2) - (labelSize.x / 2)) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}
		if( mHorizontalAlignment == QGUI_HA_RIGHT )
		{
			Ogre::Real widthBuffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(((mAbsoluteDimensions.z) - (labelSize.x) - widthBuffer) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}

		// Make sure to update the position, in case alignment has moved it
		relativeLabelPos = mTextWidget->getRelativePosition();

		if( mVerticalAlignment == QGUI_VA_TOP ) 
		{
			Ogre::Real heightBuffer = 3.0 / mPixelDimensions.w;
			mTextWidget->setPosition(relativeLabelPos.x,heightBuffer);
		}
		if( mVerticalAlignment == QGUI_VA_MID )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w / 2) - (labelSize.y / 2)) / mAbsoluteDimensions.w);
		}
		if( mVerticalAlignment == QGUI_VA_BOTTOM )
		{
			Ogre::Real heightBuffer = 3.0 / mPixelDimensions.w;
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w) - (labelSize.y) - heightBuffer) / mAbsoluteDimensions.w);
		}
	}

	void TextBox::backSpace()
	{
		if( mText == "" ) return;

		// remove character from cursor position
		int index = mTextWidget->getCursorIndex();
		if( index == 0 ) return;
		mText.erase(index-1,1);

		// Handle removing character to password box.
		Ogre::UTFString s;
		if(mMaskUserInput) s.append(mText.length(),mMaskSymbol);
		else s = mText;

		mTextWidget->setText(s);
		_notifyTextChanged();
		mTextWidget->decrementCursorIndex();
	}

	void TextBox::deleteCharacter()
	{
		if( mText == "" ) return;

		// remove character in front of cursor position
		int index = mTextWidget->getCursorIndex();
		mText.erase(index,1);

		// Handle removing character to password box.
		Ogre::UTFString s;
		if(mMaskUserInput) s.append(mText.length(),mMaskSymbol);
		else s = mText;

		mTextWidget->setText(s);
		_notifyTextChanged();
	}

	Text* TextBox::getTextWidget()
	{
		return mTextWidget;
	}

	bool TextBox::getReadOnly()
	{
		return mReadOnly;
	}

	void TextBox::maskUserInput(const Ogre::UTFString::unicode_char& symbol)
	{
		mMaskUserInput = true;
		mMaskSymbol = symbol;
	}

	void TextBox::deactivate(EventArgs& e)
	{
		mTextWidget->hideTextCursor();
		mInputMode = false;
		mBackSpaceDown = false;
		mLeftArrowDown = false;
		mRightArrowDown = false;

		Widget::deactivate(e);
	}

	bool TextBox::onEnterPressed(KeyEventArgs& e)
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnEnterPressedUserEventHandlers.begin(); it != mOnEnterPressedUserEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	bool TextBox::onCharacter(KeyEventArgs& e) 
	{ 
		if(!mReadOnly) 
		{
			mBackSpaceDown = false;
			mLeftArrowDown = false;
			mRightArrowDown = false;

			addCharacter(e.codepoint);
		}

		return Widget::onCharacter(e);
	}

	bool TextBox::onKeyDown(KeyEventArgs& e) 
	{ 
		if( (e.scancode == QuickGUI::KC_BACK) && !mReadOnly ) 
		{
			mBackSpaceDown = true;
			mLeftArrowDown = false;
			mRightArrowDown = false;
			mBackSpaceTimer = 0.0;
			backSpace();
		}
		else if( e.scancode == QuickGUI::KC_LEFT )
		{
			mMoveCursorTimer = 0.0;
			mLeftArrowDown = true;
			mRightArrowDown = false;
			mTextWidget->decrementCursorIndex();
		}
		else if( e.scancode == QuickGUI::KC_RIGHT )
		{
			mMoveCursorTimer = 0.0;
			mRightArrowDown = true;
			mLeftArrowDown = false;
			mTextWidget->incrementCursorIndex();
		}
		else if( e.scancode == QuickGUI::KC_DELETE )
		{
			mDeleteTimer = 0.0;
			mDeleteDown = true;
			mBackSpaceDown = false;
			mLeftArrowDown = false;
			mRightArrowDown = false;
			deleteCharacter();
		}

		return Widget::onKeyDown(e);
	}

	bool TextBox::onKeyUp(KeyEventArgs& e)
	{
		if( (e.scancode == QuickGUI::KC_BACK) && !mReadOnly )
			mBackSpaceDown = false;
		else if( e.scancode == QuickGUI::KC_LEFT ) mLeftArrowDown = false;
		else if( e.scancode == QuickGUI::KC_RIGHT ) mRightArrowDown = false;
		else if( e.scancode == QuickGUI::KC_DELETE ) mDeleteDown = false;

		return Widget::onKeyUp(e);
	}

	bool TextBox::onMouseButtonDown(MouseEventArgs& e)
	{
		if(e.button == QuickGUI::MB_Left) 
		{
			mTextWidget->setTextCursorPosition(e.position);
			mInputMode = true;
		}

		return Widget::onMouseButtonDown(e);
	}

	void TextBox::setReadOnly(bool readOnly)
	{
		mReadOnly = readOnly;
	}

	void TextBox::setText(const Ogre::UTFString& text)
	{
		Ogre::UTFString t = text;
		if(mMaskUserInput)
		{
			int index = 0;
			while( 0 < t.length() )
			{
				t[index] = mMaskSymbol;
			}
		}
		mTextWidget->setText(t);
		Widget::setText(text);
	}

	void TextBox::timeElapsed(Ogre::Real time)
	{
		mBackSpaceTimer += time;
		mCursorVisibilityTimer += time;
		mMoveCursorTimer += time;
		mDeleteTimer += time;

		// Hard coding the time to allow repetitive operations to be every .5 seconds
		if( mBackSpaceTimer > 0.125 )
		{
			if(mBackSpaceDown && !mReadOnly) backSpace();
			mBackSpaceTimer = 0.0;
		}

		if( mCursorVisibilityTimer > 0.5 )
		{
			if(mInputMode && !mReadOnly && !(mLeftArrowDown || mRightArrowDown)) mTextWidget->toggleTextCursorVisibility();
			mCursorVisibilityTimer = 0.0;
		}

		if( mMoveCursorTimer > 0.125 )
		{
			if(mLeftArrowDown) mTextWidget->decrementCursorIndex();
			else if(mRightArrowDown) mTextWidget->incrementCursorIndex();
			mMoveCursorTimer = 0.0;
		}

		if( mDeleteTimer > 0.125 )
		{
			if(mDeleteDown && !mReadOnly) deleteCharacter();
			mDeleteTimer = 0.0;
		}

		Widget::timeElapsed(time);
	}
}
