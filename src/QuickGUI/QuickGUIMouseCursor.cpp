#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"

template<> QuickGUI::MouseCursor* Ogre::Singleton<QuickGUI::MouseCursor>::ms_Singleton = 0;

namespace QuickGUI
{
	MouseCursor::MouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material, const unsigned int& RenderWidthInPixels, const unsigned int& RenderHeightInPixels) :
		mMaterial(material),
		mRelativeCursorDimensions(dimensions),
		mRenderWidthInPixels(RenderWidthInPixels),
		mRenderHeightInPixels(RenderHeightInPixels),
		mVisible(1),
		mHideWhenOffScreen(1),
		mOnTopBorder(0),
		mOnBotBorder(0),
		mOnLeftBorder(0),
		mOnRightBorder(0)
	{
		mPixelCursorDimensions.x = mRelativeCursorDimensions.x * RenderWidthInPixels;
		mPixelCursorDimensions.y = mRelativeCursorDimensions.y * RenderHeightInPixels;
		mConstraints.x = RenderWidthInPixels;
		mConstraints.y = RenderHeightInPixels;
		mPosition.x = mConstraints.x/2;
		mPosition.y = mConstraints.y/2;

		// Create Mouse Overlay
		mMouseOverlay = Ogre::OverlayManager::getSingleton().create("simpleGUI_Mouse_Overlay");
		mMouseOverlay->setZOrder(649);
		mMouseOverlay->show();

		// Create Mouse Overlay Container
		mMousePointerContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","simpleGUI_Mouse_Container"));
		mMousePointerContainer->setMetricsMode(Ogre::GMM_PIXELS);
		mMousePointerContainer->setPosition(mPosition.x,mPosition.y);
		mMousePointerContainer->setDimensions(Ogre::Math::Ceil(mPixelCursorDimensions.x), Ogre::Math::Ceil(mPixelCursorDimensions.y));
		mMousePointerContainer->setMaterialName(mMaterial);

		mMouseOverlay->add2D(mMousePointerContainer);
		mMousePointerContainer->show();
	}
	
	MouseCursor::~MouseCursor()
	{
		if(mMouseOverlay)
		{
			Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

			if(mMousePointerContainer)
			{
				mMouseOverlay->remove2D(mMousePointerContainer);
				om->destroyOverlayElement(mMousePointerContainer);
				mMousePointerContainer = NULL;
			}
			om->destroy(mMouseOverlay);
			mMouseOverlay = NULL;
		}
	}

	MouseCursor* MouseCursor::getSingletonPtr(void) { return ms_Singleton; }

	MouseCursor& MouseCursor::getSingleton(void) { assert( ms_Singleton );  return (*ms_Singleton); }

	void MouseCursor::_updateWindowDimensions( const unsigned int& RenderWidthInPixels, const unsigned int& RenderHeightInPixels)
	{
		mRenderWidthInPixels = RenderWidthInPixels;
		mRenderHeightInPixels = RenderHeightInPixels;

		mPixelCursorDimensions.x = mRelativeCursorDimensions.x * mRenderWidthInPixels;
		mPixelCursorDimensions.y = mRelativeCursorDimensions.y * mRenderHeightInPixels;
		mMousePointerContainer->setDimensions(Ogre::Math::Ceil(mPixelCursorDimensions.x), Ogre::Math::Ceil(mPixelCursorDimensions.y));

		mConstraints.x = RenderWidthInPixels;
		mConstraints.y = RenderHeightInPixels;
		mPosition.x = mConstraints.x/2;
		mPosition.y = mConstraints.y/2;

		constrainPosition();
	}

	void MouseCursor::constrainPosition()
	{
		bool offScreen = false;
		mOnRightBorder = false;
		mOnBotBorder = false;
		mOnTopBorder = false;
		mOnLeftBorder = false;

		if (mPosition.x >= (mConstraints.x - 1))
		{
			mPosition.x = mConstraints.x - 1;
			mOnRightBorder = true;
			offScreen = true;
		}

		if (mPosition.y >= (mConstraints.y - 1))
		{
			mPosition.y = mConstraints.y - 1;
			mOnBotBorder = true;
			offScreen = true;
		}

		if (mPosition.y <= 0)
		{
			mPosition.y = 0;
			mOnTopBorder = true;
			offScreen = true;
		}

		if (mPosition.x <= 0)
		{
			mPosition.x = 0;
			mOnLeftBorder = true;
			offScreen = true;
		}

		if (offScreen) GUIManager::getSingleton().injectMouseLeaves();
		// For example, if the user wants the mouse hidden, we shouldn't show it
		// even if its within bounds.
		else 
		{
			if(mVisible) show();
			else hide();
		}

		// Perform the actual moving of the mouse overlay
		mMousePointerContainer->setPosition(mPosition.x,mPosition.y);
	}

	bool MouseCursor::getHideWhenOffScreen()
	{
		return mHideWhenOffScreen;
	}

	Ogre::Vector2 MouseCursor::getPixelPosition()
	{
		return mPosition;
	}

	Ogre::Vector2 MouseCursor::getRelativePosition()
	{
		return mRelativeCursorDimensions;
	}

	Ogre::Vector2 MouseCursor::getRelativeSize()
	{
		return mRelativeCursorDimensions;
	}

	void MouseCursor::hide()
	{
		if(mMouseOverlay) 
		{
			mMouseOverlay->hide();
			mVisible = false;
		}
	}

	void MouseCursor::_hide()
	{
		if(mMouseOverlay) 
		{
			mMouseOverlay->hide();
		}
	}

	bool MouseCursor::isVisible()
	{
		return mVisible;
	}

	bool MouseCursor::mouseOnBotBorder()
	{
		return mOnBotBorder;
	}

	bool MouseCursor::mouseOnLeftBorder()
	{
		return mOnLeftBorder;
	}

	bool MouseCursor::mouseOnRightBorder()
	{
		return mOnRightBorder;
	}

	bool MouseCursor::mouseOnTopBorder()
	{
		return mOnTopBorder;
	}

	void MouseCursor::offsetPosition(const int& x_offset_in_pixels, const int& y_offset_in_pixels)
	{
		mPosition.x += x_offset_in_pixels;
		mPosition.y += y_offset_in_pixels;
		constrainPosition();
	}

	void MouseCursor::setMaterial(const Ogre::String& material)
	{
		mMaterial = material;
		mMousePointerContainer->setMaterialName(mMaterial);
	}

	void MouseCursor::setHideCursorWhenOSCursorOffscreen(bool hide)
	{
		mHideWhenOffScreen = hide;
	}

	void MouseCursor::setPosition(const int& xPixelPosition, const int& yPixelPosition)
	{
		mPosition.x = xPixelPosition;
		mPosition.y = yPixelPosition;
		constrainPosition();
	}

	void MouseCursor::setSize(const int& widthInPixels, const int& heightInPixels)
	{
		mMousePointerContainer->setDimensions(widthInPixels,heightInPixels);
	}

	void MouseCursor::show()
	{
		if(mMouseOverlay) 
		{
			mMouseOverlay->show();
			mVisible = true;
		}
	}
}
