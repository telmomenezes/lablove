#include "QuickGUIManager.h"

#include "OgreFontManager.h"

template<> QuickGUI::GUIManager* Ogre::Singleton<QuickGUI::GUIManager>::ms_Singleton = 0;

namespace QuickGUI
{
	GUIManager::GUIManager(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight) :
		mRenderWindowWidth(RenderWindowWidth),
		mRenderWindowHeight(RenderWindowHeight),
		mMouseCursor(0),
		mActiveSheet(0),
		mWidgetContainingMouse(0),
		mActiveWidget(0),
		mClickTimeout(1),
		mAutoNameSheetCounter(0)
	{
		mWidgetNames.clear();

		mMouseButtonDown[0] = NULL;
		mMouseButtonDown[1] = NULL;
		mMouseButtonDown[2] = NULL;
		mMouseButtonDown[3] = NULL;
		mMouseButtonDown[4] = NULL;
		mMouseButtonDown[5] = NULL;
		mMouseButtonDown[6] = NULL;
		mMouseButtonDown[7] = NULL;

		mDefaultSheet = createSheet("DefaultSheet","");
		mActiveSheet = mDefaultSheet;
	}

	GUIManager::~GUIManager()
	{
		clearAll();
		destroyMouseCursor();
	}

	GUIManager* GUIManager::getSingletonPtr(void) { return ms_Singleton; }

	GUIManager& GUIManager::getSingleton(void) { assert( ms_Singleton );  return (*ms_Singleton); }

	void GUIManager::_notifyWindowDimensions(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight)
	{
		mRenderWindowWidth = RenderWindowWidth;
		mRenderWindowHeight = RenderWindowHeight;
		
		if( mMouseCursor ) mMouseCursor->_updateWindowDimensions(mRenderWindowWidth,mRenderWindowHeight);

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
			(*it)->_notifyDimensionsChanged();
	}

	void GUIManager::clearAll()
	{
		mWidgetNames.clear();

		mMouseButtonDown[0] = NULL;
		mMouseButtonDown[1] = NULL;
		mMouseButtonDown[2] = NULL;
		mMouseButtonDown[3] = NULL;
		mMouseButtonDown[4] = NULL;
		mMouseButtonDown[5] = NULL;
		mMouseButtonDown[6] = NULL;
		mMouseButtonDown[7] = NULL;
		mWidgetContainingMouse = NULL;
		mActiveWidget = NULL;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
			delete (*it);
		mSheets.clear();

		// reset counter
		mAutoNameSheetCounter = 0;
		// create default sheet
		mDefaultSheet = createSheet("DefaultSheet","");
		mActiveSheet = mDefaultSheet;

		destroyMouseCursor();
	}

	MouseCursor* GUIManager::createMouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material)
	{
		mMouseCursor = new MouseCursor(dimensions,material,mRenderWindowWidth,mRenderWindowHeight);

		return mMouseCursor;
	}

	MouseCursor* GUIManager::createMouseCursor(const Ogre::Vector2& dimensions)
	{
		Ogre::String material = "qgui.pointer";

		return createMouseCursor(dimensions,material);
	}

	Sheet* GUIManager::_createSheet(const Ogre::String& name, const Ogre::String& material)
	{
		Sheet* newSheet = new Sheet(name,material);

		mSheets.push_back(newSheet);

		return newSheet;
	}

	Sheet* GUIManager::createSheet(const Ogre::String& name, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createSheet(name,material);
	}

	Sheet* GUIManager::createSheet(const Ogre::String& name)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		// the default skin for a sheet is transparency.
		return _createSheet(name,"");
	}
	
	Sheet* GUIManager::createSheet()
	{
		Ogre::String name = "Sheet" + Ogre::StringConverter::toString(mAutoNameSheetCounter);
		++mAutoNameSheetCounter;

		// the default skin for a sheet is transparency.
		return _createSheet(name,"");
	}

	void GUIManager::destroyMouseCursor()
	{
		delete mMouseCursor;
		mMouseCursor = NULL;
	}

	void GUIManager::destroySheet(const Ogre::String& name)
	{
		if( name == "" ) return;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name )
			{
				Sheet* s = (*it);
				it = mSheets.erase(it);
				delete s;
				break;
			}
		}
	}

	void GUIManager::destroySheet(Sheet* sheet)
	{
		destroySheet(sheet->getInstanceName());
	}

	Sheet* GUIManager::getActiveSheet()
	{
		return mActiveSheet;
	}

	Sheet* GUIManager::getDefaultSheet()
	{
		return mDefaultSheet;
	}

	MouseCursor* GUIManager::getMouseCursor()
	{
		return mMouseCursor;
	}

	Widget* GUIManager::getMouseOverWidget()
	{
		return mWidgetContainingMouse;
	}

	Ogre::Real GUIManager::getRenderWindowAspectRatio()
	{
		return (static_cast<Ogre::Real>(mRenderWindowWidth) / static_cast<Ogre::Real>(mRenderWindowHeight));
	}

	unsigned int GUIManager::getRenderWindowWidth()
	{
		return mRenderWindowWidth;
	}

	unsigned int GUIManager::getRenderWindowHeight()
	{
		return mRenderWindowHeight;
	}

	Sheet* GUIManager::getSheet(const Ogre::String& name)
	{
		if( name == "" ) return NULL;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) 
				return (*it);
		}

		return NULL;
	}

	bool GUIManager::injectChar(char c)
	{
		KeyEventArgs args(0);
		args.codepoint = c;
		args.handled = false;

		if( isalpha(c) || isdigit(c) || ispunct(c) || c == ' ' )
		{
			if(mActiveWidget != NULL) 
			{
				args.widget = mActiveWidget;
				mActiveWidget->onCharacter(args);
			}
		}

		return args.handled;
	}

	bool GUIManager::injectKeyDown(const KeyCode& kc)
	{
		KeyEventArgs args(0);
		args.scancode = kc;
		args.handled = false;

		if(mActiveWidget != NULL) 
		{
			args.widget = mActiveWidget;
			mActiveWidget->onKeyDown(args);
		}

		return args.handled;
	}

	bool GUIManager::injectKeyUp(const KeyCode& kc)
	{
		KeyEventArgs args(0);
		args.scancode = kc;
		args.handled = false;

		if(mActiveWidget != NULL) 
		{
			args.widget = mActiveWidget;
			mActiveWidget->onKeyUp(args);
		}

		return args.handled;
	}
	
	bool GUIManager::injectMouseButtonDown(const MouseButtonID& button)
	{
		if( (mMouseCursor == NULL) || (!mMouseCursor->isVisible()) ) return false;

		MouseEventArgs args(0);

		args.position.x = mMouseCursor->getPixelPosition().x;
		args.position.y = mMouseCursor->getPixelPosition().y;
		args.button = button;
		args.handled = false;

		if(mWidgetContainingMouse != NULL) 
		{
			// mActiveWidget is the last widget the user clicked on, ie TextBox, ComboBox, etc.
			if( (mActiveWidget != NULL) && (mActiveWidget->getInstanceName() != mWidgetContainingMouse->getInstanceName()) )
			{
				args.widget = mActiveWidget;
				mActiveWidget->deactivate(args);
				// reset to false, becase within this context, the event (mouseButtonDown) has not been handled
				args.handled = false;
				mActiveWidget = NULL;
			}

			mActiveWidget = mWidgetContainingMouse;
			
			args.widget = mActiveWidget;
			mWidgetContainingMouse->onMouseButtonDown(args);
			// Gaurds against the scenario where a handler changes game state (clears GUI)
			if( mWidgetContainingMouse == NULL ) return args.handled;
				
			// When a window becomes active, all its child widgets except for textboxes
			Window* w = mWidgetContainingMouse->getWindow();
			if( w != NULL ) w->activate(args);
			else mWidgetContainingMouse->getSheet()->activate(args);

			// Only one textbox can be active at a time.  In the event that the clicked widget
			// is a textbox, activate it!
			if(mActiveWidget->getWidgetType() == Widget::QGUI_TYPE_TEXTBOX) mActiveWidget->activate(args);

			// Record that the mouse button went down on this widget (non-window)
			mMouseButtonDown[args.button] = mWidgetContainingMouse;
		}
		else
		{
			if( mActiveWidget != NULL )
			{
				args.widget = mActiveWidget;
				mActiveWidget->deactivate(args);
				mActiveWidget = NULL;
			}

			mMouseButtonDown[args.button] = NULL;
		}

		mMouseButtonTimings[button] = time(NULL);

		return args.handled;
	}

	bool GUIManager::injectMouseButtonUp(const MouseButtonID& button)
	{
		if( (mMouseCursor == NULL) || (!mMouseCursor->isVisible()) ) return false;

		MouseEventArgs args(0);

		args.position.x = mMouseCursor->getPixelPosition().x;
		args.position.y = mMouseCursor->getPixelPosition().y;
		args.button = button;
		args.handled = false;

		// If the MouseButton was not pressed on this widget, do not register the button being released on the widget
		// Obviously if the recorded widget is NULL, we know that nothing will be registered
		if( mMouseButtonDown[button] == NULL ) 
		{
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);
			return false;
		}
		if( mWidgetContainingMouse == NULL )
		{
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);
			return false;
		}
		if( mMouseButtonDown[button]->getInstanceName() != mWidgetContainingMouse->getInstanceName() )
		{
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);
			return false;
		}

		// after this point, we know that the user had mouse button down on this widget, and is now doing mouse button up

		args.widget = mWidgetContainingMouse;

		mWidgetContainingMouse->onMouseButtonUp(args);
		// Need to gaurd against the scenario where  mousebutton up destroys the UI
		if( (mWidgetContainingMouse != NULL) && (time(NULL) - mMouseButtonTimings[button] < mClickTimeout) ) mWidgetContainingMouse->onMouseClicked(args);

		return args.handled;
	}

	bool GUIManager::injectMouseLeaves(void)
	{
		if( (mMouseCursor == NULL) || (!mMouseCursor->isVisible()) ) return false;

		if(mMouseCursor->getHideWhenOffScreen()) mMouseCursor->_hide();

		return true;
	}

	bool GUIManager::injectMouseMove(const int& xPixelOffset, const int& yPixelOffset)
	{
		if( mMouseCursor == NULL ) return false;

		MouseEventArgs args(0);

		args.moveDelta.x = xPixelOffset;
		args.moveDelta.y = yPixelOffset;
		args.handled = false;

		if( mMouseCursor->mouseOnTopBorder() && yPixelOffset < 0 ) args.moveDelta.y = 0;
		if( mMouseCursor->mouseOnBotBorder() && yPixelOffset > 0 ) args.moveDelta.y = 0;
		if( mMouseCursor->mouseOnLeftBorder() && xPixelOffset < 0 ) args.moveDelta.x = 0;
		if( mMouseCursor->mouseOnRightBorder() && xPixelOffset > 0 ) args.moveDelta.x = 0;

		// Update Mouse Cursor Position
		mMouseCursor->offsetPosition(xPixelOffset,yPixelOffset);

		args.position.x = mMouseCursor->getPixelPosition().x;
		args.position.y = mMouseCursor->getPixelPosition().y;

		if(!mMouseCursor->isVisible()) return args.handled;

		// Now that moving the cursor is handled, move onto widget event handling.
		if( mActiveSheet == NULL ) return args.handled;

		// See if we should be dragging a widget.
		if( (mWidgetContainingMouse != NULL) && (mWidgetContainingMouse->getGrabbed()) && (mWidgetContainingMouse->draggingEnabled()) )
		{
			// If a widget is active, make in inactive
			if( mActiveWidget != NULL ) mActiveWidget->deactivate(args);

			mWidgetContainingMouse->drag(
				args.moveDelta.x / static_cast<Ogre::Real>(mRenderWindowWidth),
				args.moveDelta.y / static_cast<Ogre::Real>(mRenderWindowHeight));

			return args.handled;
		}

		// Now get the widget the cursor is over.
		Widget* hitWidget = mActiveSheet->getTargetWidget(args.position);
		args.widget = hitWidget;

		// Take care of properly firing MouseMoved, MouseEnters, and MouseLeaves events
		if(hitWidget)
		{
			if( mWidgetContainingMouse != NULL )
			{
				if( mWidgetContainingMouse->getInstanceName() != hitWidget->getInstanceName() )
				{
					mWidgetContainingMouse->onMouseLeaves(args);
					mWidgetContainingMouse = hitWidget;
					mWidgetContainingMouse->onMouseEnters(args);
				}
				mWidgetContainingMouse->onMouseMoved(args);
			}
			else
			{
				mWidgetContainingMouse = hitWidget;
				mWidgetContainingMouse->onMouseEnters(args);
				mWidgetContainingMouse->onMouseMoved(args);
			}
		}
		else
		{
			if( mWidgetContainingMouse != NULL )
			{
				mWidgetContainingMouse->onMouseLeaves(args);
			}
			mWidgetContainingMouse = NULL;
		}

		return args.handled;
	}

	bool GUIManager::injectMousePosition(const int& xPixelPosition, const int& yPixelPosition)
	{
		if( mMouseCursor == NULL ) return false;

		MouseEventArgs args(0);
		args.handled = false;

		Ogre::Vector2 pos = mMouseCursor->getPixelPosition();

		injectMouseMove(xPixelPosition - pos.x,yPixelPosition - pos.y);

		return args.handled;
	}

	bool GUIManager::injectMouseWheelChange(float delta)
	{
		MouseEventArgs args(0);
		args.handled = false;

		args.wheelChange = delta;

		if(mWidgetContainingMouse != NULL) 
		{
			args.widget = mWidgetContainingMouse;
			mWidgetContainingMouse->onMouseWheel(args);
		}

		return args.handled;
	}

	void GUIManager::injectTime(Ogre::Real time)
	{
		if( mActiveSheet != NULL ) mActiveSheet->timeElapsed(time);
	}

	void GUIManager::setActiveSheet(Sheet* s)
	{
		if( s == NULL ) return;

		std::list<Sheet*>::iterator it;
		for( it = mSheets.begin(); it != mSheets.end(); ++it )
			(*it)->hide();

		mActiveSheet = s;
		mActiveSheet->show();
	}

	bool GUIManager::validWidgetName(const Ogre::String& name, bool addToList)
	{
		if( name == "" ) return false;

		Ogre::StringVector::iterator it;
		for( it = mWidgetNames.begin(); it != mWidgetNames.end(); ++it )
		{
			if( (*it) == name ) return false;
		}

		if(addToList) mWidgetNames.push_back(name);

		return true;
	}
}
