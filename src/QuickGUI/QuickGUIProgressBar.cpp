#include "QuickGUIProgressBar.h"
#include "QuickGUIWindow.h"

namespace QuickGUI
{
	ProgressBar::ProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_PROGRESSBAR;

		// Border Overlay gives us ability to assign material to TextBox border and Panel separately.
		mOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mOverlayElement);
		mOverlayElement->show();

		mBarOverlayElement = createPanelOverlayElement(mInstanceName+"_Bar",mPixelDimensions,mWidgetMaterial+".bar");
		mChildrenContainer->addChild(mBarOverlayElement);
		mBarOverlayElement->show();
	}

	ProgressBar::~ProgressBar()
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnProgressChangedHandlers.begin(); it != mOnProgressChangedHandlers.end(); ++it )
			delete (*it);
		mOnProgressChangedHandlers.clear();

		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		// destroy bar overlay element
		mChildrenContainer->removeChild(mBarOverlayElement->getName());
		om->destroyOverlayElement(mBarOverlayElement);
	}

	void ProgressBar::_applyDimensions()
	{
		mOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
		mBarOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mBarOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void ProgressBar::addOnProgressChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnProgressChangedHandlers.push_back(function);
	}

	Ogre::Real ProgressBar::getProgress()
	{
		return mProgress;
	}

	void ProgressBar::hide()
	{
		mOverlayElement->hide();
		mBarOverlayElement->hide();
		
		Widget::hide();
	}

	bool ProgressBar::onProgressChanged(WidgetEventArgs& e)
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnProgressChangedHandlers.begin(); it != mOnProgressChangedHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	void ProgressBar::setMaterial(const Ogre::String& material)
	{
		mWidgetMaterial = material;
		mOverlayElement->setMaterialName(mWidgetMaterial);
		mBarOverlayElement->setMaterialName(mWidgetMaterial+".bar");
	}

	void ProgressBar::setProgress(Ogre::Real progress)
	{
		mProgress = progress;
		if(mProgress > 1.0) mProgress = 1.0;
		else if(mProgress < 0.0) mProgress = 0.0;

		// stretch bar image according to progress
		mBarOverlayElement->setWidth(mProgress * mPixelDimensions.z);
		// fire event and call any user defined handlers
		WidgetEventArgs e(this);
		e.handled = false;
		onProgressChanged(e);
	}

	void ProgressBar::show()
	{
		mOverlayElement->show();
		mBarOverlayElement->show();
		
		Widget::show();
	}
}