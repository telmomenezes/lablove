#ifndef QUICKGUIIMAGE_H
#define QUICKGUIIMAGE_H

#include "QuickGUIPrerequisites.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** Represents a simple Imagel.
		@remarks
		Pretty much a Label, but without text.
		@note
		Images also support Render To Texture.
	*/
	class Image :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Image(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Image();

		/**
		* Creates a material using the texture and applies it to the widget image.
		*/
		void setMaterial(Ogre::RenderTexture* texture);
		/**
		* Applies material to widget image.  If supplied name is a texture name,
		* material is created using the texture and applied to widget image.
		*/
		void setMaterial(const Ogre::String& name, bool texture = false);
		/**
		* Sets the texture coordinates for the panel.
		*/
		void setUV(const Ogre::Real& u1, const Ogre::Real& v1, const Ogre::Real& u2, const Ogre::Real& v2);

	protected:
		Ogre::MaterialPtr					mMaterialPtr;
	};
}

#endif
