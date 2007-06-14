#ifndef QUICKGUIWIDGET_H
#define QUICKGUIWIDGET_H

#include "OgrePanelOverlayElement.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIMemberFunctionPointer.h"
#include "QuickGUIPrerequisites.h"
#include "OgreTextAreaOverlayElement.h"

#include <vector>

namespace QuickGUI
{
	// Forward declaration solely for the getWindow function
	class Window;
	// Forward declaration solely for the getSheet function
	class Sheet;

	/**
	* Useful for widgets horizontally aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	typedef enum HorizontalAlignment
	{
		QGUI_HA_NO_ALIGNMENT	=  0,
		QGUI_HA_LEFT				,
		QGUI_HA_MID					,
		QGUI_HA_RIGHT
	};
	/**
	* Useful for widgets vertically aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	typedef enum VerticalAlignment
	{
		QGUI_VA_NO_ALIGNMENT	=  0,
		QGUI_VA_TOP					,
		QGUI_VA_MID					,
		QGUI_VA_BOTTOM
	};

	/** Main QuickGUI components for creating GUI.
		@remarks
		This class outlines the interface for
		widgets, providing default functionality.
	*/
	class Widget
	{
	public:
		/**
		* Outlining Types of widgets in the library.
		*/
		typedef enum Type
		{
			QGUI_TYPE_BUTTON		=  0,
			QGUI_TYPE_COMBOBOX			,
			QGUI_TYPE_IMAGE				,
			QGUI_TYPE_LABEL				,
			QGUI_TYPE_LIST				,
			QGUI_TYPE_LISTITEM			,
			QGUI_TYPE_MENU				,
			QGUI_TYPE_MENULIST			,
			QGUI_TYPE_NSTATEBUTTON		,
			QGUI_TYPE_PANEL				,
			QGUI_TYPE_PROGRESSBAR		,
			QGUI_TYPE_SHEET				,
			QGUI_TYPE_TEXT				,
			QGUI_TYPE_TEXTBOX			,
			QGUI_TYPE_TEXTCURSOR		,
			QGUI_TYPE_TITLEBAR			,
			QGUI_TYPE_WINDOW
		};
		/**
		* All widgets must support these events
		*/
		typedef enum Event
		{
			QGUI_EVENT_ACTIVATED      =  0,
			QGUI_EVENT_CHARACTER_KEY	  ,
			QGUI_EVENT_DEACTIVATED		  ,
			QGUI_EVENT_KEY_DOWN			  ,
			QGUI_EVENT_KEY_UP			  ,
			QGUI_EVENT_MOUSE_BUTTON_DOWN  ,
			QGUI_EVENT_MOUSE_BUTTON_UP	  ,
			QGUI_EVENT_MOUSE_CLICK		  ,
			QGUI_EVENT_MOUSE_ENTER		  ,
			QGUI_EVENT_MOUSE_LEAVE		  ,
			QGUI_EVENT_MOUSE_MOVE		  ,
			QGUI_EVENT_MOUSE_WHEEL			
		};
		/**
		* Borders that every widget can use.
		*/
		typedef enum Border
		{
			QGUI_BORDER_TOP		=  0,
			QGUI_BORDER_BOTTOM		,
			QGUI_BORDER_LEFT		,
			QGUI_BORDER_RIGHT
		};
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Widget(const Ogre::String& instanceName, const Ogre::Vector4& relativeDimensions, Ogre::String material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Widget();

		// Function that allows widgets to add a child widget after creation.
		void _addChildWidget(Widget* w);
		// sets the widget position/size based on mPixelDimensions
		virtual void _applyDimensions();
		// Internal method to create widget borders.
		void _createBorders();
		// Internal method to destroy widget borders.
		void _destroyBorders();
		// Recalculate the dimensions and reposition/resize the widget (calls _updateDimensions)
		virtual void _notifyDimensionsChanged();
		// Realign text, if the widget has a child label widget
		virtual void _notifyTextChanged() {}
		// Function that allows widgets to remove child widgets.
		void _removeChildWidget(Widget* w);
		// Calculate Absolute and Pixel dimensions
		void _updateDimensions(const Ogre::Vector4& relativeDimensions);
		// Internal method to update border sizes
		void _updateBorderSize();

		/** Adds an event handler to this widget
            @note
                Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@param
				EVENT Defined widget events, for example: QGUI_EVENT_ACTIVATED, QGUI_EVENT_CHARACTER_KEY, QGUI_EVENT_MOUSE_BUTTON_DOWN, etc
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addEventHandler(QuickGUI::QGUI_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
        */
		template<typename T> void addEventHandler(Event EVENT, bool (T::*function)(const EventArgs&), T* obj)
		{
			mUserEventHandlers[EVENT].push_back(new MemberFunctionPointer<T>(function,obj));
		}

		void addEventHandler(Event EVENT, MemberFunctionSlot* function);

		/**
		* Internal method.  User works in relative coordinates, but internally everything is done in pixels.
		* Used to calculate the pixel size acording to the dimensions provided.
		*/
		Ogre::Vector4 convertRelativeToPixelDimensions(const Ogre::Vector4& dimensions);

		/**
		* Moves draggingWidget.  By default, this widget is this widget, but this can be changed.
		* Allows dragging the titlebar or it's text to drag the window, for example.
		*/
		void drag(const Ogre::Real& relativeXOffset, const Ogre::Real& relativeYOffset);
		/**
		* Returns true if the widget is able to be dragged, false otherwise.
		*/
		bool draggingEnabled();
		/**
		* Enable or Disable dragging.
		*/
		void enableDragging(bool enable);

		std::vector<Widget*>* getChildWidgetList();
		Widget* getChildWidget(const Ogre::String& name);

		Ogre::Vector4 getPixelDimensions();
		Ogre::Vector2 getPixelPosition();
		Ogre::Vector2 getPixelSize();

		Ogre::Vector4 getAbsoluteDimensions();
		Ogre::Vector2 getAbsolutePosition();
		Ogre::Vector2 getAbsoluteSize();

		Ogre::Vector4 getRelativeDimensions();
		Ogre::Vector2 getRelativePosition();
		Ogre::Vector2 getRelativeSize();

		Ogre::Real getCharacterHeight();
		Ogre::String getFont();
		bool getGrabbed();
		
		Ogre::String getInstanceName();
		/**
		* Returns true if window is able to be repositions, false otherwise.
		*/
		bool getMovingEnabled();
		/**
		* Returns the Overlay Container is used to contain the Widget's OverlayElements
		*/
		Ogre::OverlayContainer* getOverlayContainer();
		/**
		* Returns the Widget's Parent Widget.  Does not throw an exception is the Parent
		* does not exist.  (Windows do not have parent widgets)
		*/
		Widget* getParentWidget();
		/**
		* Get Sheet this widget belongs to.  If the widget is a Sheet, it is returned.
		*/
		Sheet* getSheet();
		/**
		* Iterates through visible Children widgets to find and return the widget that is *hit* by the point.
		* Returns NULL is nothing is *hit*.
		*/
		virtual Widget* getTargetWidget(const Ogre::Vector2& p);
		/**
		* Returns the text element of this widget.  Text by default is "".
		*/
		Ogre::UTFString getText();
		Ogre::ColourValue getTextColorTop();
		Ogre::ColourValue getTextColorBot();
		/**
		* Returns the type of the widget, as enumerated above. ex. QGUI_TYPE_BUTTON.
		*/
		Type getWidgetType();
		/**
		* Get Window this widget is attached to.  If the widget is a Window, it is returned.
		*/
		Window* getWindow();
		/**
		* Get zOrder widget is drawn on.
		*/
		int getZOrder();
		/**
		* Returns the number of zOrders higher this widget is from it's Parent Widget.
		*/
		int getZOrderOffset();

		/**
		* Sets mVisible to false.  Widgets should override this to implement how they handle
		* hiding.
		*/
		virtual void hide();
		/**
		* Hide borders if they exist.
		*/
		void hideBorders();
		/**
		* Returns true if pixel point p is inside the pixel dimensions of this widget.
		*/
		virtual bool isPointWithinBounds(const Ogre::Vector2& p);
		bool isVisible();

		/**
		* Offset the widget dimensions.  Useful for dragging/moving widgets.
		*/
		void move(const Ogre::Real& relativeXOffset, const Ogre::Real& relativeYOffset);

		/**
		* Properly cleans up all child widgets.
		*/
		void removeAndDestroyAllChildWidgets();
		/**
		* Sets top/bottom/left/right border size in pixels.
		*/
		void setBorderWidth(int borderPixelHeight);
		/**
		* Sets top/bottom, left/right border size in pixels.
		*/
		void setBorderWidth(int topBotBorderPixelHeight, int leftRightBorderPixelHeight);
		/**
		* Sets top, bottom, left, right border size in pixels.
		*/
		void setBorderWidth(int topBorderPixelHeight, int botBorderPixelHeight, int leftBorderPixelHeight, int rightBorderPixelHeight);
		/**
		* Sets character height.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.
		*/
		virtual void setCharacterHeight(const Ogre::Real& relativeHeight);
		/**
		* Manually set the relative Dimensions of the widget.
		*/
		void setDimensions(const Ogre::Vector4& relativeDimensions);
		/**
		* This function specifies the widget that will be moved when the widget is *dragged*.
		* By default, the Dragging Widget is the widget itself, but this allows for the possibility
		* of moving a window by *dragging* the titlebar, or even the titlbar's text widget.
		*/
		void setDraggingWidget(Widget* w);
		/**
		* Sets text font.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.
		*/
		virtual void setFont(const Ogre::String& font);
		/**
		* Manually set mGrabbed to true.
		*/
		void setGrabbed(bool grabbed);
		/**
		* Sets only the height of the widget, relative to it's parent (or screen if parent does not exist)
		* NOTE: all children widgets will be notified that dimensions have changed.
		*/
		void setHeight(const Ogre::Real& relativeHeight);
		/**
		* If set to false, widget cannot be moved.
		*/
		void setMovingEnabled(bool enable);
		/**
		* Manually set position of widget relative to its parent, if exists. Window positioning is done
		* relative to the screen (Render Window).
		*/
		void setPosition(const Ogre::Real& xRelativePosition, const Ogre::Real& yRelativePosition);
		/**
		* Sets text.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		virtual void setText(const Ogre::UTFString& text);
		/**
		* Sets text color.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		virtual void setTextColor(const Ogre::ColourValue& color);
		virtual void setTextColor(const Ogre::ColourValue& topColor,const Ogre::ColourValue& botColor);
		/**
		* Sets the number of zOrders higher this widget is compared to its parent.
		*/
		void setZOrderOffset(int offset, bool updateWindowZList = true);
		/**
		* Sets mVisible to true.  Widgets should override this to implement how they handle
		* showing.
		*/
		virtual void show();
		/**
		* Shows borders, if they exist.
		*/
		void showBorders();
		/**
		* Sets text vertical alignment.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		void setVerticalAlignment(VerticalAlignment va);
		/**
		* Sets text horizontal alignment.  Widgets should override this, since text handling is widget specific,
		* in terms of truncating and alignment.  Or in some cases where a widget does not use text,
		* this will have no visual impact.
		*/
		void setHorizontalAlignment(HorizontalAlignment ha);

		// Event Handler functions - should be overriden to supply default functionality,
		// For example, the button won't look like a button unless you apply the *.over and
		// *.down material during onMouseEnters, Leaves, ButtonDown, etc. events
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void activate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		virtual void deactivate(EventArgs& e);
		virtual bool onMouseEnters(MouseEventArgs& e);
		virtual bool onMouseLeaves(MouseEventArgs& e);
		virtual bool onMouseMoved(MouseEventArgs& e);
		virtual bool onMouseWheel(MouseEventArgs& e);
		virtual bool onMouseButtonUp(MouseEventArgs& e);
		virtual bool onMouseButtonDown(MouseEventArgs& e);
		virtual bool onMouseClicked(MouseEventArgs& e);
		virtual bool onKeyDown(KeyEventArgs& e);
		virtual bool onKeyUp(KeyEventArgs& e);
		virtual bool onCharacter(KeyEventArgs& e);
		virtual void timeElapsed(Ogre::Real time);

	protected:
		Ogre::String				mInstanceName;
		Type						mWidgetType;
		bool						mVisible;

		bool						mGrabbed;
		bool						mMovingEnabled;
		bool						mDraggingEnabled;
		Widget*						mWidgetToDrag;
		int							mZOrderOffset;

		Ogre::String				mFont;
		Ogre::Real					mCharacterHeight;
		Ogre::UTFString				mText;
		Ogre::ColourValue			mTextTopColor;
		Ogre::ColourValue			mTextBotColor;
		VerticalAlignment			mVerticalAlignment;
		HorizontalAlignment			mHorizontalAlignment;

		Widget*						mParentWidget;
		Ogre::String				mWidgetMaterial;
		// Panel or TextArea Element, depending on the widget.
		Ogre::OverlayElement*		mOverlayElement;
		Ogre::OverlayContainer*		mOverlayContainer;

		Ogre::PanelOverlayElement*	mBorders[4];
		int							mBorderSizeInPixels[4];
		Ogre::String				mBorderMaterial[4];
		bool						mBordersHidden;

		// List of any child widgets this widget may have.
		std::vector<Widget*>		mChildWidgets;
		// Each widget has an overlay container that children are attached to.
		Ogre::OverlayContainer*		mChildrenContainer;

		// Everything is implemented in pixels under the hood
		Ogre::Vector4				mPixelDimensions;
		// First two values represent the absolute positions - 0,0 to 1,1 represent top left to bottom right of render window
		// Second two values represent the absolute sizes
		Ogre::Vector4				mAbsoluteDimensions;
		// First two values represent the relative positions - 0,0 to 1,1 represent top left to bottom right of parent widget
		// Second two values represent the relative sizes		
		Ogre::Vector4				mRelativeDimensions;

		// Event handlers! Only one per event per widget
		std::vector< std::vector<MemberFunctionSlot*> > mUserEventHandlers;

		void _initEventHandlers();

		/* Uses parent widget's absolute dimensions with given relative dimensions to produce absolute (screen) dimensions */
		Ogre::Vector4 getAbsoluteDimensions(const Ogre::Vector4& relativeDimensions);

		// COMMENT TAKEN DIRECTLY FROM OGRE
		/** A 2D element which contains other OverlayElement instances.
		@remarks
			This is a specialisation of OverlayElement for 2D elements that contain other
			elements. These are also the smallest elements that can be attached directly
			to an Overlay.
		@remarks
			OverlayContainers should be managed using OverlayManager. This class is responsible for
			instantiating / deleting elements, and also for accepting new types of element
			from plugins etc.
		*/
		Ogre::OverlayContainer* createOverlayContainer(const Ogre::String& name, const Ogre::String& material);

		// COMMENT TAKEN DIRECTLY FROM OGRE
		/** OverlayElement representing a flat, single-material (or transparent) panel which can contain other elements.
		@remarks
			This class subclasses OverlayContainer because it can contain other elements. Like other
			containers, if hidden it's contents are also hidden, if moved it's contents also move etc. 
			The panel itself is a 2D rectangle which is either completely transparent, or is rendered 
			with a single material. The texture(s) on the panel can be tiled depending on your requirements.
		@par
			This component is suitable for backgrounds and grouping other elements. Note that because
			it has a single repeating material it cannot have a discrete border (unless the texture has one and
			the texture is tiled only once). For a bordered panel, see it's subclass BorderPanelOverlayElement.
		@par
			Note that the material can have all the usual effects applied to it like multiple texture
			layers, scrolling / animated textures etc. For multiple texture layers, you have to set 
			the tiling level for each layer.
		*/
		Ogre::PanelOverlayElement* createPanelOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);

		// COMMENT TAKEN FROM OGRE
		/** This class implements an overlay element which contains simple unformatted text.
		*/
		Ogre::TextAreaOverlayElement* createTextAreaOverlayElement(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
	};
}

#endif
