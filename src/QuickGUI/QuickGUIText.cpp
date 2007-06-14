#include "QuickGUILabel.h"
#include "QuickGUIManager.h"
#include "QuickGUIText.h"
#include "OgreFontManager.h"
#include "OgreFont.h"

namespace QuickGUI
{
	Text::Text(const Ogre::String& name, const Ogre::Vector3& dimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,Ogre::Vector4(dimensions.x,dimensions.y,0,dimensions.z),"",overlayContainer,ParentWidget),
		mTextCursorHidden(1),
		mCursorIndex(0),
		mCursorOffset(-3)
	{
		mWidgetType = Widget::QGUI_TYPE_TEXT;
		mTruncateMode = RIGHT;
		mFeedbackString = "...";
		mCursorPositions.push_back(-5);
		
		mTextAreaOverlayElement = createTextAreaOverlayElement(mInstanceName+".Caption",mPixelDimensions,"");
		mOverlayElement = mTextAreaOverlayElement;
		mOverlayContainer->addChild(mOverlayElement);
		mTextAreaOverlayElement->show();
		mTextAreaOverlayElement->setFontName(mFont);
		mTextAreaOverlayElement->setCharHeight(mPixelDimensions.w);

		Ogre::String tcMaterial = getSheet()->getDefaultSkin() + ".textcursor";
		mTextCursor = new TextCursor(mInstanceName+".TextCursor",Ogre::Vector4(0,0,0,1),tcMaterial,mChildrenContainer,this);
		mTextCursor->setZOrderOffset(1,false);
		mTextCursor->hide();
	}

	Text::~Text()
	{
		delete mTextCursor;
		mTextCursor = NULL;
		mTextAreaOverlayElement = NULL;
	}

	Ogre::UTFString Text::_adjustWidth()
	{
		Ogre::UTFString displayText = mText;
		// Adding a 5 pixel buffer, which helps textBoxes which have borders
		int parentWidth = mParentWidget->getPixelDimensions().z - 5;

		if( getTextWidth(displayText) > parentWidth )
		{
			// calling getTextWidth indexes the text, so cursor can be placed within text.  Only reason
			// we don't break out early with truncate mode set to NONE.
			if(mTruncateMode != NONE)
			{
				// get width of feedback string
				Ogre::Real feedbackWidth = getTextWidth(mFeedbackString);
				Ogre::Real allotedWidth = parentWidth - feedbackWidth;
				while( (getTextWidth(displayText) > allotedWidth) && (displayText != "") )
				{
					if(mTruncateMode == RIGHT) displayText = displayText.erase(displayText.length()-1,1);
					else if(mTruncateMode == LEFT) displayText = displayText.erase(0,1);
				}
				// concatenate
				if( (mTruncateMode == RIGHT) && (mFeedbackString != "") ) displayText.append(mFeedbackString);
				else if( (mTruncateMode == LEFT) && (mFeedbackString != "") ) displayText.insert(0,mFeedbackString);
			}
		}

		return displayText;
	}

	void Text::_applyDimensions()
	{
		mTextAreaOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mTextAreaOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
		mTextAreaOverlayElement->setCharHeight(mPixelDimensions.w);
	}

	void Text::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();
	}

	void Text::_notifyTextChanged()
	{
		// adjust width to match the width of text
		mRelativeDimensions.z = (getTextWidth() / mParentWidget->getPixelDimensions().z);
		_notifyDimensionsChanged();
	}

	void Text::activate(EventArgs& e)
	{
		Widget::activate(e);
	}

	Ogre::Real Text::convertPosToIndex(const Ogre::Vector2& p)
	{
		Ogre::Real xPos = p.x;

		int index;
		for( index = 0; index < static_cast<int>(mCursorPositions.size()); ++index )
		{
			if( xPos < mCursorPositions[index] + mPixelDimensions.x )
			{
				if( index <= 0 ) return index;

				// determine if point is closer to the left or right index
				int range = mCursorPositions[index] - mCursorPositions[index-1];
				if( (mCursorPositions[index] - xPos) >= (range / 2) ) return index;
				else return index - 1;
			}
		}

		return (index - 1);
	}

	void Text::deactivate(EventArgs& e)
	{
		mParentWidget->deactivate(e);
	}

	void Text::decrementCursorIndex()
	{
		if( mCursorIndex > 0 ) --mCursorIndex;
		mTextCursor->show();
		mTextCursor->setPixelPosition(getCursorPosition(mCursorIndex),mPixelDimensions.y);
	}

	Ogre::Real Text::getCursorPosition(int index)
	{
		if( index >= static_cast<int>(mCursorPositions.size()) ) return (mPixelDimensions.x - 2);

		return (mPixelDimensions.x + mCursorPositions[index] + mCursorOffset);
	}

	int Text::getCursorIndex()
	{
		return mCursorIndex;
	}

	Ogre::Real Text::getTextWidth()
	{
		return getTextWidth(mText);
	}

	Ogre::Real Text::getTextWidth(const Ogre::UTFString& text)
	{
		mCursorPositions.clear();
		// Store first cursor position
		mCursorPositions.push_back(0);

		if(text == "") return 0.0;

		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::Font* f = dynamic_cast<Ogre::Font*>(fm->getByName(mFont).get());

		Ogre::Real width = 0.0;
		for( int index = 0; index < static_cast<int>(text.length_Characters()); ++index )
		{
			if( text[index] == ' ' ) width += (f->getGlyphAspectRatio('0') * mPixelDimensions.w);
			else width += (f->getGlyphAspectRatio(text[index]) * mPixelDimensions.w);
			mCursorPositions.push_back(width);
		}

		// now we know the text width in pixels, and have index positions at the start/end of each character.

		return width;
	}

	void Text::hideTextCursor()
	{
		mTextCursor->hide();
		mTextCursorHidden = true;
	}

	void Text::incrementCursorIndex()
	{
		++mCursorIndex;
		if( mCursorIndex >= static_cast<int>(mCursorPositions.size()) ) --mCursorIndex;
		mTextCursor->show();
		mTextCursor->setPixelPosition(getCursorPosition(mCursorIndex),mPixelDimensions.y);
	}

	void Text::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		// Enforce the Text Widget's dimensions to match the Actual Text Dimensions,
		// as Text is not bounded to it's overlay element size
		setHeight(relativeHeight);
	}

	void Text::setTruncationFeedback(const Ogre::UTFString& visualFeedback)
	{
		mFeedbackString = visualFeedback;
		setText(mText);
	}

	void Text::setFont(const Ogre::String& font)
	{
		mFont = font;
		_notifyTextChanged();
	}

	void Text::setHeight(const Ogre::Real& relativeHeight)
	{
		mRelativeDimensions.w = relativeHeight;
		_notifyTextChanged();
	}

	void Text::setText(const Ogre::UTFString& text)
	{
		mText = text;

		mTextAreaOverlayElement->setFontName(mFont);
		mTextAreaOverlayElement->setCharHeight(mPixelDimensions.w);
		mTextAreaOverlayElement->setColourTop(mTextTopColor);
		mTextAreaOverlayElement->setColourBottom(mTextBotColor);
		mTextAreaOverlayElement->setCaption(_adjustWidth());
		
		_notifyTextChanged();
	}

	void Text::setTextCursorPosition(const Ogre::Vector2& p)
	{
		mCursorIndex = convertPosToIndex(p);
		mTextCursor->setPixelPosition(getCursorPosition(mCursorIndex),mPixelDimensions.y);
	}

	void Text::setTruncateMode(TruncateMode MODE)
	{
		mTruncateMode = MODE;
		setText(mText);
	}

	void Text::show()
	{
		if(!mTextCursorHidden) mTextCursor->show();
		Widget::show();
	}

	void Text::showTextCursor()
	{
		mTextCursor->show();
		mTextCursorHidden = false;
	}

	void Text::toggleTextCursorVisibility()
	{
		mTextCursor->toggleVisibility();
		mTextCursorHidden = !mTextCursorHidden;
	}
}
