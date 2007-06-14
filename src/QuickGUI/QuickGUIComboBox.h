#ifndef QUICKGUICOMBOBOX_H
#define QUICKGUICOMBOBOX_H

#include "QuickGUIButton.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIPrerequisites.h"

#include <vector>

namespace QuickGUI
{
	/** Represents a traditional ComboBox.
	@remarks
	The ComboBox class requires at least 3 materials to define it's image:
	Normal State, Mouse Over, and Mouse Down.  For example, if you pass
	the constructor "sample.combobox" as its arguement for the material,
	the class will expect "sample.combobox.over" and "sample.combobox.down"
	to exist.  The ComboBox supplies a list of items from which the user
	can choose.
	@note
	In order to get the most use out of ComboBox, you will need to add
	ListItems.
	@note
	ComboBoxes are meant to be created via the Window widget.
	*/
	class ComboBox :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		ComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~ComboBox();

		/**
		* Adds (and creates) a list item to the drop down list.
		* Note that you cannot add a button or Image to the ListItem.
		*/
		void addListItem(const Ogre::UTFString& text);
		void addListItem(const Ogre::String& name, const Ogre::UTFString& text);
		/**
		* Add user defined events, that will be called when a selection is made.
		*/
		template<typename T> void addOnSelectionEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnSelectUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnSelectionEventHandler(MemberFunctionSlot* function);
		/**
		* Aligns each ListItem text vertically and horizontally.
		*/
		void alignListItemText(HorizontalAlignment ha, VerticalAlignment va);

		/**
		* Returns the number of ListItems the ComboBox Drop Down List contains.
		*/
		int getNumberOfListItems();

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void deactivate(EventArgs& e);
		/**
		* Event Handler assigned to child widgets of the ComboBox.
		*/
		bool deactivate(const EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_ENTER event.  material.over is applied to the button
		* image. If not handled, it will be passed to the parent widget 
		* (if exists)
		*/
		bool onMouseEnters(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_LEAVE event.  material is applied to the button
		* image. If not handled, it will be passed to the parent widget 
		* (if exists)
		*/
		bool onMouseLeaves(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonDown(MouseEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonUp(MouseEventArgs& e);
		/**
		* Specific Handler for the ComboBox Widget.  Called when the user selects a ListItem
		*/
		bool onSelection(WidgetEventArgs& e);

		/**
		* Removes (and destroys) a ListItem from the Drop Down List.  No exception is thrown
		* if the index is out of bounds.
		*/
		void removeListItem(unsigned int index);

		/**
		* Manually sets the height of the characters for each list item and Combo Box Label component.
		* Note that the text can be set larger than the widget dimensions.
		*/
		void setCharacterHeight(const Ogre::Real& relativeHeight);
		/**
		* Sets the text of the ComboBox's Label component.
		*/
		void setText(const Ogre::UTFString& text);
		/**
		* Sets the material displayed when the mouse moves over a list item.
		*/
		void setDropListHighlightMaterial(const Ogre::String& material);
		/**
		* Sets the width of the Drop Down List, relative to the combobox width.
		*/
		void setDropListWidth(const Ogre::Real& relativeWidth);
		/**
		* Shows the combobox.  Drop Down list is hidden.
		*/
		void show();

	protected:
		// ComboBox Label component, usually displays the text of the ListItem
		// that has been selected.
		Label* mLabel;
		// Button that shows the drop down list.
		Button* mButton;
		// Drop down list.
		List* mList;

		HorizontalAlignment mListItemHorizontalAlignment;
		VerticalAlignment mListItemVerticalAlignment;

		// User defined event handlers that are called when a Selection is made.
		std::vector<MemberFunctionSlot*> mOnSelectUserEventHandlers;
	};
}

#endif
