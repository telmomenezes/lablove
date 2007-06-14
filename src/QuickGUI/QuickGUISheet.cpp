#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"

namespace QuickGUI
{
	Sheet::Sheet(const Ogre::String& name, const Ogre::String& material) :
		Panel(name,Ogre::Vector4(0,0,1,1),material,NULL,NULL),
		mDefaultCharacterHeight(1),
		mDefaultTextColor(Ogre::ColourValue::White),
		mDefaultSkin("qgui"),
		mAutoNameWindowCounter(0),
		mMenuOverlayZOrder(600)
	{
		mWidgetType = Widget::QGUI_TYPE_SHEET;
		mWindows.clear();
		// Sheets are always at zOrder 0.
		mOverlay = Ogre::OverlayManager::getSingleton().create(mInstanceName+".Overlay");
		mOverlay->setZOrder(0);
		mOverlay->show();
		mOverlay->add2D(mOverlayContainer);

		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourceManager::ResourceMapIterator rmi = fm->getResourceIterator();
		if(rmi.hasMoreElements()) mDefaultFont = rmi.getNext()->getName();
		else mDefaultFont = "";

		// Create menu overlay and container
		mMenuOverlay = Ogre::OverlayManager::getSingleton().create(mInstanceName+".MenuOverlay");
		mMenuOverlay->show();
		mMenuContainer = createOverlayContainer(mInstanceName+".MenuContainer","");
		mMenuOverlay->add2D(mMenuContainer);
		mMenuContainer->show();

		// Set zOrder very high, but leave room for containers and mouse cursor to show up correctly.
		mMenuOverlay->setZOrder(mMenuOverlayZOrder);
	}

	Sheet::~Sheet()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mWindows.clear();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		// destroy menu container
		mMenuOverlay->remove2D(mMenuContainer);
		om->destroyOverlayElement(mMenuContainer);
		mMenuContainer = NULL;

		// destroy menu overlay
		om->destroy(mMenuOverlay);
		mMenuOverlay = NULL;

		// destroy background overlay element
		mOverlayContainer->removeChild(mOverlayElement->getName());
		om->destroyOverlayElement(mOverlayElement);
		mOverlayElement = NULL;
	
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

	void Sheet::_applyDimensions()
	{
		// cannot set position/size of sheet widget..
	}

	Menu* Sheet::_createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Menu* newMenu = new Menu(name,dimensions,material,mMenuContainer,this);
		newMenu->setZOrderOffset(mMenuOverlayZOrder + 2);
		Widget::_addChildWidget(newMenu);
		// update count
		++mNumMenus;

		return newMenu;
	}

	Window* Sheet::_createWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Window* newWindow = new Window(name,dimensions,material,this);

		int zOrder = 10;
		if( !mWindows.empty() ) zOrder = mWindows.front()->getMaxZOrderValue();

		// note that zOrder starts at 11.  0-10 are reserved for Sheet Widgets.
		// text widgets and text cursor widgets are usually not included in
		// a windows ZOrder List, so we add 3 to make up for these.
		newWindow->setZOrder(zOrder+3);
		mWindows.push_front(newWindow);
		Widget::_addChildWidget(newWindow);

		return newWindow;
	}

	Window* Sheet::createWindow(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		// Many widgets can have no material, (label or empty window for example) but a regular window
		// must have a material!
		if( material == "" ) return NULL;

		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		return _createWindow(name,dimensions,material);
	}

	Window* Sheet::createWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		// Many widgets can have no material, (label or empty window for example) but a regular window
		// must have a material!
		if( material == "" ) return NULL;

		return _createWindow(name,dimensions,material);
	}

	Window* Sheet::createWindow(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = mDefaultSkin + ".window";

		return _createWindow(name,dimensions,material);
	}

	Window* Sheet::createWindow(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = "DefaultWindow" + Ogre::StringConverter::toString(mAutoNameWindowCounter);
		++mAutoNameWindowCounter;

		Ogre::String material = mDefaultSkin + ".window";

		return _createWindow(name,dimensions,material);
	}

	void Sheet::destroyWindow(const Ogre::String& name)
	{
		if( name == "" ) return;

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( (*it)->getInstanceName() == name )
			{
				Window* w = (*it);
				Widget::_removeChildWidget(w);
				it = mWindows.erase(it);
				delete w;
				sortListByWindowZOrder();
				return;
			}
		}
	}

	void Sheet::destroyWindow(Window* w)
	{
		destroyWindow(w->getInstanceName());
	}

	Ogre::Real Sheet::getDefaultCharacterHeight()
	{
		return mDefaultCharacterHeight;
	}

	Ogre::String Sheet::getDefaultFont()
	{
		return mDefaultFont;
	}

	Ogre::ColourValue Sheet::getDefaultTextColor()
	{
		return mDefaultTextColor;
	}

	Ogre::String Sheet::getDefaultSkin()
	{
		return mDefaultSkin;
	}

	Ogre::OverlayContainer* Sheet::getMenuContainer()
	{
		return mMenuContainer;
	}

	int Sheet::getMenuOverlayZOrder()
	{
		return mMenuOverlayZOrder;
	}

	Window* Sheet::getWindow(const Ogre::String& name)
	{
		if( name == "" ) return NULL;

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) 
				return (*it);
		}

		return NULL;
	}

	void Sheet::setActiveWindow(Window* w)
	{
		if( static_cast<unsigned int>(mWindows.size()) < 2 ||
			mWindows.front()->getInstanceName() == w->getInstanceName() )
			return;
		
		// If we make it here, the list has at least 2 windows, and the passed in window is not the active window

		std::list<Window*>::iterator it;
		for( it = mWindows.begin(); it != mWindows.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
			{
				it = mWindows.erase(it);
				break;
			}
		}

		mWindows.push_front(w);

		sortListByWindowZOrder();
	}

	void Sheet::setDefaultFont(const Ogre::String& font)
	{
		mDefaultFont = font;
	}

	void Sheet::setDefaultTextColor(const Ogre::ColourValue& color)
	{
		mDefaultTextColor = color;
	}

	void Sheet::setDefaultCharacterHeight(const Ogre::Real& height)
	{
		mDefaultCharacterHeight = height;
	}

	void Sheet::setDefaultSkin(const Ogre::String& skin)
	{
		mDefaultSkin = skin;
	}

	void Sheet::sortListByWindowZOrder()
	{
		// Re-organize all z-orders - min window zOrder is 10
		int zOrder = 10;
		std::list<Window*>::reverse_iterator rIt;
		for( rIt = mWindows.rbegin(); rIt != mWindows.rend(); ++rIt )
		{
			// text widgets and text cursor widgets are usually not included in
			// a windows ZOrder List, so we add 3 to make up for these.
			(*rIt)->setZOrder(zOrder+3);
			zOrder += (*rIt)->getMaxZOrderValue();
		}
	}
}
