#include "QuickGUILabel.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Label::Label(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_LABEL;

		mOverlayElement = createPanelOverlayElement(mInstanceName+".Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mOverlayElement);
		mOverlayElement->show();

		mCharacterHeight = 0.8;
		mTextWidget = new Text(mInstanceName+".Text",Ogre::Vector3(0,0,mCharacterHeight),mChildrenContainer,this);
		mTextWidget->setZOrderOffset(1,false);
		Widget::_addChildWidget(mTextWidget);

		mHorizontalAlignment = QGUI_HA_MID;
		mVerticalAlignment = QGUI_VA_MID;
	}

	Label::~Label()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mTextWidget = NULL;
	}

	void Label::_notifyTextChanged()
	{
		mTextWidget->setFont(mFont);
		mTextWidget->setCharacterHeight(mCharacterHeight);
		mTextWidget->setTextColor(mTextTopColor,mTextBotColor);
		mTextWidget->setText(mText);

		alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void Label::alignText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mHorizontalAlignment = ha;
		mVerticalAlignment = va;

		Ogre::Vector2 relativeLabelPos = mTextWidget->getRelativePosition();
		Ogre::Vector2 labelPos = mTextWidget->getAbsolutePosition();
		Ogre::Vector2 labelSize = mTextWidget->getAbsoluteSize();

		if( mHorizontalAlignment == QGUI_HA_LEFT ) 
		{
			// We should add a 5 pixel buffer
			Ogre::Real buffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(buffer,relativeLabelPos.y);
		}
		else if( mHorizontalAlignment == QGUI_HA_MID )
		{
			mTextWidget->setPosition(((mAbsoluteDimensions.z / 2) - (labelSize.x / 2)) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}
		else if( mHorizontalAlignment == QGUI_HA_RIGHT )
		{
			// We should add a 5 pixel buffer
			Ogre::Real buffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(((mAbsoluteDimensions.z) - (labelSize.x) - buffer) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}

		// Make sure to update the position, in case alignment has moved it
		relativeLabelPos = mTextWidget->getRelativePosition();

		if( mVerticalAlignment == QGUI_VA_TOP ) mTextWidget->setPosition(relativeLabelPos.x,0.0);
		else if( mVerticalAlignment == QGUI_VA_MID )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w / 2) - (labelSize.y / 2)) / mAbsoluteDimensions.w);
		}
		else if( mVerticalAlignment == QGUI_VA_BOTTOM )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w) - (labelSize.y)) / mAbsoluteDimensions.w);
		}
	}

	void Label::activate(EventArgs& e) 
	{
		mTextWidget->activate(e);
		Widget::activate(e);
	}

	void Label::deactivate(EventArgs& e) 
	{
		Widget::deactivate(e);
	}

	Text* Label::getTextWidget()
	{
		return mTextWidget;
	}
}
