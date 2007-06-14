#include "QuickGUIImage.h"

namespace QuickGUI
{
	Image::Image(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_IMAGE;
		mMaterialPtr.setNull();

		mOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mOverlayElement);
		mOverlayElement->show();
	}

	Image::~Image()
	{
		// If a material was created (for rendertexture use), destroy and unload it
		if(!mMaterialPtr.isNull())
		{
			Ogre::String name = mMaterialPtr->getName();
			mMaterialPtr.setNull();
			Ogre::MaterialManager::getSingleton().remove(name);
		}
	}

	void Image::setMaterial(Ogre::RenderTexture* texture)
	{
		setMaterial(texture->getName(),true);
	}

	void Image::setMaterial(const Ogre::String& name, bool texture)
	{
		if(!texture)
		{
			mWidgetMaterial = name;
			mOverlayElement->setMaterialName(mWidgetMaterial);
		}
		else
		{
			// If a previous material exists, destroy and unload it
			if(!mMaterialPtr.isNull())
			{
				mOverlayElement->setMaterialName(mWidgetMaterial);
				Ogre::String name = mMaterialPtr->getName();
				mMaterialPtr.setNull();
				Ogre::MaterialManager::getSingleton().remove(name);
			}

			mMaterialPtr = Ogre::MaterialManager::getSingleton().create(mInstanceName+"_RenderTextureMaterial",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			Ogre::TextureUnitState* t = mMaterialPtr->getTechnique(0)->getPass(0)->createTextureUnitState(name);
			mOverlayElement->setMaterialName(mMaterialPtr->getName());
		}
	}

	void Image::setUV(const Ogre::Real& u1, const Ogre::Real& v1, const Ogre::Real& u2, const Ogre::Real& v2)
	{
		dynamic_cast<Ogre::PanelOverlayElement*>(mOverlayElement)->setUV(u1,v1,u2,v2);
	}
}