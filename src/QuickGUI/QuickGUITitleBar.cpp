#include "QuickGUITitleBar.h"
#include "QuickGUIManager.h"

#include "QuickGUIWindow.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	TitleBar::TitleBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_TITLEBAR;

		setCharacterHeight(0.8);
		alignText(QGUI_HA_LEFT,QGUI_VA_MID);

		// Create CloseButton
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonPixelHeight = 0.8 * mPixelDimensions.w;
		Ogre::Real verticalPixelSpace = 0.1 * mPixelDimensions.w;
		Ogre::Real buttonWidth = buttonPixelHeight / mPixelDimensions.z;
		Ogre::Real horizontalPixelSpace = verticalPixelSpace / mPixelDimensions.z;

		Ogre::Vector4 cbDimensions = Ogre::Vector4((1 - (buttonWidth + horizontalPixelSpace)),0.1,buttonWidth,buttonHeight);
		mCloseButton = new Button(mInstanceName+".CloseButton",cbDimensions,mWidgetMaterial+".button",mChildrenContainer,this);
		mCloseButton->addEventHandler(Widget::QGUI_EVENT_MOUSE_CLICK,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		mCloseButton->addEventHandler(Widget::QGUI_EVENT_MOUSE_BUTTON_UP,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		mCloseButton->setZOrderOffset(1);
		Widget::_addChildWidget(mCloseButton);
	}

	TitleBar::~TitleBar()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mCloseButton = NULL;
	}

	Button* TitleBar::getCloseButton()
	{
		return mCloseButton;
	}

	void TitleBar::hideCloseButton()
	{
		mCloseButton->hide();
	}

	void TitleBar::showCloseButton()
	{
		mCloseButton->show();
	}
}
