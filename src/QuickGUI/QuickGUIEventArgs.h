/** All supported Events used in QuickGUI.
	@remarks
	Every widget supports functions to fire the events listed below.  
	These Events will go to the widget's corresponding event handlers, 
	and call user defined event handlers if they have been created.
	Events iterate through their child or parent widgets, until it
	is handled, or there are no more children/parents.
	@note
	Events are fired via the GUIManager, which interacts with injected
	inputs.
*/

#ifndef QUICKGUIEVENTARGS_H
#define QUICKGUIEVENTARGS_H

#include "QuickGUIKeyCode.h"
#include "QuickGUIMouseButtonID.h"
#include "QuickGUIPrerequisites.h"

namespace QuickGUI
{
	class Widget;

	/** Basic EventArgs
	*/
	class EventArgs
	{
	public:
		EventArgs();
		~EventArgs();

		// handlers should set this to true if they handled the event.
		bool	handled;		
	};

	/** Widget EventArgs
	*/
	class WidgetEventArgs : public EventArgs
	{
	public:
		WidgetEventArgs(Widget* w) : widget(w) {}

		//pointer to a Widget object of relevance to the event.
		Widget*	widget;		
	};

	/** Mouse EventArgs
	*/
	class MouseEventArgs : public WidgetEventArgs
	{
	public:
		MouseEventArgs(Widget* w) : WidgetEventArgs(w) {}

		// holds current mouse position. (pixels)
		Ogre::Vector2	position;
		// holds variation of mouse position from last mouse input
		Ogre::Vector2	moveDelta;		
		// holds the mouse button that was down for the given event
		MouseButtonID	button;
		// holds the amount the scroll wheel has changed.
		Ogre::Real		wheelChange;	
	};

	/** Key EventArgs
	*/
	class KeyEventArgs : public WidgetEventArgs
	{
	public:
		KeyEventArgs(Widget* w) : WidgetEventArgs(w) {}

		// char codepoint for the key (only used for Character inputs).
		char		codepoint;		
		// Scan code of key that caused event (only used for key up & down inputs.
		KeyCode		scancode;		
	};

	/** ButtonState EventArgs
	*/
	class ButtonStateEventArgs : public WidgetEventArgs
	{
	public:
		ButtonStateEventArgs(Widget* w) : WidgetEventArgs(w) {}

		// index used to retreive the NStateButton's current state
		Ogre::ushort		currentState;
	};
}

#endif
