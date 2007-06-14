#ifndef QUICKGUITEXT_H
#define QUICKGUITEXT_H

#include "OgreTextAreaOverlayElement.h"
#include "QuickGUIPrerequisites.h"
#include "QuickGUITextCursor.h"
#include "QuickGUIWidget.h"

#include <vector>

namespace QuickGUI
{
	/** Represents a text component.
		@remarks
		All text in QuickGUI is done through the Text Widget.
		@note
		The dimensions of the widget (Width and Height) are the same
		as the actual text dimensions displayed.
		@note
		Text, unlike all other widgets, can be created by any widget.
	*/
	class Text :
		public Widget
	{
	public:
		/**
		* When the text is wider than its parent widget's width,
		* it needs to be truncated.  The mode decides whether to
		* truncate on the left or right side of the text.  The default
		* is on the right side, but the left can be useful, in
		* textboxes for example.
		*/
		typedef enum TruncateMode
		{
			LEFT	=  0,
			RIGHT,
			NONE
		};
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, and height of the widget.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Text(const Ogre::String& name, const Ogre::Vector3& dimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Text();

		/**
		* Internal method that truncates the text if its width is larger than
		* its parents width.
		*/
		Ogre::UTFString _adjustWidth();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();
		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();
		/**
		* When user has changed the font, character height, or text,
		* the label must be updated and aligned according to its parent.
		*/
		void _notifyTextChanged();

		// Overridden Event Handling functions
		/**
		* Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		void activate(EventArgs& e);
		/**
		* Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		void deactivate(EventArgs& e);
		/**
		* Sets the text cursor index to the previous index.
		*/
		void decrementCursorIndex();
		/**
		* Returns the horizontal pixel position within the text, given an index.
		*/
		Ogre::Real getCursorPosition(int index);
		/**
		* Returns the cursor index.  0 is before the first character, and the last is after the last character.
		*/
		int getCursorIndex();
		/**
		* Uses mText, mFont, and widget height in pixels to calculate the width of text in pixels
		*/
		Ogre::Real getTextWidth();
		Ogre::Real getTextWidth(const Ogre::UTFString& text);

		void hideTextCursor();
		/**
		* Advances the text cursor index by one.
		*/
		void incrementCursorIndex();

		/**
		* Overrides Widget::setCharacterHeight, as label dimensions depend on the text height 
		* and font and need to be recalculated.
		*/
		void setCharacterHeight(const Ogre::Real& relativeHeight);
		/**
		* Sets the string used to notify users that part of the text has been cut off (truncated).
		* By default, this value is "...".  If you don't want any feedback, set feedback to "".
		*/
		void setTruncationFeedback(const Ogre::UTFString& visualFeedback);
		/**
		* Overrides Widget::setFont, as label dimensions depend on the text height and font
		* and need to be recalculated.
		*/
		void setFont(const Ogre::String& font);
		/**
		* Sets the height of the widget, relative to it's parent (or screen if parent does not exist)
		* NOTE: Label widget height and character height are the same.
		* NOTE: all children widgets will be notified that dimensions have changed.
		*/
		void setHeight(const Ogre::Real& relativeHeight);
		/**
		* Sets the TextAreaOverlayElement's font, text color, character height, and text.
		*/
		void setText(const Ogre::UTFString& text);
		/**
		* Using the mouses position, determine and set the position of the text cursor.
		*/
		void setTextCursorPosition(const Ogre::Vector2& p);
		/**
		* Defines how to handle the situation where the text is wider than it's parent's Widget Width.
		*/
		void setTruncateMode(TruncateMode MODE);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();
		void showTextCursor();

		void toggleTextCursorVisibility();

	protected:
	private:
		TruncateMode mTruncateMode;
		Ogre::UTFString mFeedbackString;

		Ogre::TextAreaOverlayElement* mTextAreaOverlayElement;

		// Stores pixel offsets of where text characters begin and end.
		// Useful for placing a text cursor widget.
		std::vector<int> mCursorPositions;

		TextCursor* mTextCursor;
		unsigned int mCursorIndex;
		// Offset applied to cursor index position to appear in between character;
		int mCursorOffset;
		bool mTextCursorHidden;

		Ogre::Real convertPosToIndex(const Ogre::Vector2& p);
	};
}

#endif
