#include "QuickGUITextCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	TextCursor::TextCursor(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_TEXTCURSOR;

		Ogre::Real pixelWidth = QuickGUI::GUIManager::getSingleton().getRenderWindowWidth() * 0.01;
		mPixelDimensions.z = Ogre::Math::Ceil(pixelWidth);

		mOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mOverlayElement);
		mOverlayElement->show();
	}

	TextCursor::~TextCursor()
	{
	}

	void TextCursor::_applyDimensions()
	{
		Ogre::Real pixelWidth = QuickGUI::GUIManager::getSingleton().getRenderWindowWidth() * 0.01;
		mPixelDimensions.z = Ogre::Math::Ceil(pixelWidth);

		mOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void TextCursor::setPixelPosition(const Ogre::Real& xPixelPos, const Ogre::Real& yPixelPos)
	{
		mPixelDimensions.x = xPixelPos;
		mPixelDimensions.y = yPixelPos;
		mOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
	}

	void TextCursor::toggleVisibility()
	{
		if(mVisible) mOverlayElement->hide();
		else mOverlayElement->show();

		mVisible = !mVisible;
	}
}
