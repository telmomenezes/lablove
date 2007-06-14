#include "QuickGUIWindow.h"
// included to get access to default font/fontsize
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Window::Window(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Widget* parentWidget) :
		Panel(name,dimensions,material,NULL,parentWidget),
		mTitleBar(0),
		mTitleBarHidden(0)
	{
		mWidgetType = Widget::QGUI_TYPE_WINDOW;
		mOverlay = Ogre::OverlayManager::getSingleton().create(mInstanceName+".Overlay");
		mOverlay->setZOrder(0);
		mOverlay->show();
		mZOrderValues.push_back(0);

		// mChildrenContainer already created in Widget constructor
		mTitleBarContainer = createOverlayContainer(mInstanceName+".TitleBarContainer","");
		
		mOverlay->add2D(mOverlayContainer);
		mChildrenContainer->addChildImpl(mTitleBarContainer);
		
		mOverlayContainer->show();
		mChildrenContainer->show();
		mTitleBarContainer->show();

		// Create TitleBar - tradition titlebar dimensions: across the top of the window
		Ogre::Vector4 defaultTitleBarDimensions = Ogre::Vector4(0,0,1,0.05 / getAbsoluteDimensions().w);
		mTitleBar = new TitleBar(mInstanceName+".Titlebar",defaultTitleBarDimensions,mWidgetMaterial+".titlebar",mTitleBarContainer,this);
		mTitleBar->enableDragging(true);
		mTitleBar->setDraggingWidget(this);
		mTitleBar->getTextWidget()->enableDragging(true);
		mTitleBar->getTextWidget()->setDraggingWidget(this);
		mTitleBar->setZOrderOffset(2);
		Widget::_addChildWidget(mTitleBar);

		// Now that mOverlayContainer has been created (via _init() function) we can create the borders
		_createBorders();
	}

	Window::~Window()
	{
		Widget::_destroyBorders();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		// Delete all child widgets before deleting the window widget
		Widget::removeAndDestroyAllChildWidgets();
		
		// destroy background overlay element
		mOverlayContainer->removeChild(mOverlayElement->getName());
		om->destroyOverlayElement(mOverlayElement);
		mOverlayElement = NULL;

		// destroy TitleBar container
		mChildrenContainer->removeChild(mTitleBarContainer->getName());
		om->destroyOverlayElement(mTitleBarContainer);
		mTitleBarContainer = NULL;
	
		// destroy Children container
		mOverlayContainer->removeChild(mChildrenContainer->getName());
		om->destroyOverlayElement(mChildrenContainer);
		mChildrenContainer = NULL;
		
		// destroy default container
		mOverlay->remove2D(mOverlayContainer);
		om->destroyOverlayElement(mOverlayContainer);
		mOverlayContainer = NULL;

		// destroy overlay
		om->destroy(mOverlay);
	}

	void Window::activate(EventArgs& e)
	{
		getSheet()->setActiveWindow(this);

		Panel::activate(e);
	}

	TitleBar* Window::getTitleBar()
	{
		return mTitleBar;
	}

	void Window::hide()
	{
		Panel::hide();
	}

	bool Window::hide(const EventArgs& e)
	{
		Panel::hide();

		return true;
	}

	void Window::hideCloseButton()
	{
		if(mTitleBar) mTitleBar->hideCloseButton();
	}

	void Window::hideTitlebar()
	{
		if(mTitleBar) 
		{
			mTitleBar->hide();
			mTitleBarHidden = true;
		}
	}

	void Window::setText(const Ogre::UTFString& text)
	{
		mText = text;
		mTitleBar->setFont(mFont);
		mTitleBar->setCharacterHeight(mCharacterHeight);
		mTitleBar->setTextColor(mTextTopColor,mTextBotColor);
		mTitleBar->setText(mText);
	}

	void Window::setTextColor(const Ogre::ColourValue& color)
	{
		setTextColor(color,color);
	}

	void Window::setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor)
	{
		mTextTopColor = topColor;
		mTextBotColor = botColor;
		mTitleBar->setFont(mFont);
		mTitleBar->setCharacterHeight(mCharacterHeight);
		mTitleBar->setTextColor(mTextTopColor,mTextBotColor);
		mTitleBar->setText(mText);
	}

	void Window::setTitleBarHeight(Ogre::Real height)
	{
		if( height > mOverlayElement->getHeight() ) height = mOverlayElement->getHeight();

		if(mTitleBar) mTitleBar->setHeight(height);
	}

	void Window::setZOrder(int zOrder)
	{
		// offset relative to parent sheet, which is using an overlay of zOrder 0.
		int previousZOrderOffset = mZOrderOffset;
		mZOrderOffset = zOrder;
		
		int difference = mZOrderOffset - previousZOrderOffset;
		mOverlay->setZOrder(static_cast<Ogre::ushort>(mZOrderOffset));

		// Update zOrder Value List
		std::list<int>::iterator it;
		for( it = mZOrderValues.begin(); it != mZOrderValues.end(); ++it )
		{
			(*it) = (*it) + difference;
		}
	}

	void Window::showCloseButton()
	{
		if(mTitleBar) mTitleBar->showCloseButton();
	}

	void Window::showTitlebar()
	{
		if(mTitleBar) 
		{
			mTitleBar->show();
			mTitleBarHidden = false;
		}
	}
}
