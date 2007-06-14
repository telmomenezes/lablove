#include "QuickGUIButton.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"
#include "OgreFont.h"

namespace QuickGUI
{
	Button::Button(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_BUTTON;
		setCharacterHeight(0.5);

		Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();

		mOverMaterialExists = mm->resourceExists(mWidgetMaterial+".over");
		mDownMaterialExists = mm->resourceExists(mWidgetMaterial+".down");
	}

	Button::~Button()
	{
	}

	void Button::applyButtonDownMaterial()
	{
		if(mDownMaterialExists) 
		{
			// apply button ".down" material
			mOverlayElement->setMaterialName(mWidgetMaterial+".down");
		}
	}

	void Button::applyDefaultMaterial()
	{
		mOverlayElement->setMaterialName(mWidgetMaterial);
	}

	bool Button::onMouseButtonDown(MouseEventArgs& e) 
	{
		applyButtonDownMaterial();

		return Label::onMouseButtonDown(e);
	}

	bool Button::onMouseButtonUp(MouseEventArgs& e) 
	{ 
		if(mOverMaterialExists) mOverlayElement->setMaterialName(mWidgetMaterial+".over");

		return Label::onMouseButtonUp(e);
	}

	bool Button::onMouseEnters(MouseEventArgs& e) 
	{ 
		if(mOverMaterialExists) mOverlayElement->setMaterialName(mWidgetMaterial+".over");

		return Label::onMouseEnters(e);
	}

	bool Button::onMouseLeaves(MouseEventArgs& e) 
	{ 
		mOverlayElement->setMaterialName(mWidgetMaterial);

		return Label::onMouseLeaves(e);
	}
}
