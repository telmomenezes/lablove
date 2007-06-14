#ifndef QUICKGUIPROGRESSBAR_H
#define QUICKGUIPROGRESSBAR_H

#include "QuickGUIPrerequisites.h"
#include "QuickGUIText.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** Represents a vertical or horizontal bar displaying
		visual status showing a current position relative to
		the beginning and end of the bar.
		@remarks
		Useful for Life Bar's, or anything similar.
		@note
		ProgressBars must be created by the Window class.
	*/
	class ProgressBar :
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
		ProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~ProgressBar();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();
		/**
		* Add user defined event that will be called when amount of progress has changed.
		*/
		template<typename T> void addOnProgressChangedEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnProgressChangedHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnProgressChangedEventHandler(MemberFunctionSlot* function);
		Ogre::Real getProgress();
		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		/**
		* Default Handler for handling progress changes.
		* Note that this event is not passed to its parents, the event is specific to this widget.
		*/
		bool onProgressChanged(WidgetEventArgs& e);
		void setMaterial(const Ogre::String& material);
		/**
		* Sets progress.  Value should be between 0.0 and 1.0
		*/
		void setProgress(Ogre::Real progress);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

	protected:
	private:
		Type mProgressBarType;

		Ogre::PanelOverlayElement* mBarOverlayElement;

		Ogre::Real mProgress;

		// Default material, displayed in its original state.
		Ogre::String mMaterial;

		std::vector<MemberFunctionSlot*> mOnProgressChangedHandlers;
	};
}

#endif
