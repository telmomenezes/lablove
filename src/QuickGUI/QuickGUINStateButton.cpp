#include "QuickGUINStateButton.h"

namespace QuickGUI
{
	NStateButton::NStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Button(name,dimensions,"",overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_NSTATEBUTTON;
	}

	NStateButton::~NStateButton()
	{
		clearStates();
	}

	void NStateButton::addOnStateChangedEventHandler(MemberFunctionSlot* function)
	{
		mOnStateChangedUserEventHandlers.push_back(function);
	}

	void NStateButton::addState(const Ogre::String& name, const Ogre::String& material, Ogre::UTFString text)
	{
		NStateButton::State* s = new NStateButton::State(name,material,text);
		mStates.push_back(s);

		// The widget has an image when the first state gets added
		if(static_cast<int>(mStates.size()) == 1)
		{
			setCurrentState(mStates[0]);
		}
	}

	void NStateButton::clearStates()
	{
		std::vector<State*>::iterator it;
		for( it = mStates.begin(); it != mStates.end(); ++it )
		{
			delete (*it);
		}
		mStates.clear();
	}

	NStateButton::State* NStateButton::getCurrentState()
	{
		return mCurrentState;
	}

	int NStateButton::getIndexOfState(NStateButton::State* s)
	{
		int index = 0;
		std::vector<State*>::iterator it;
		for( it = mStates.begin(); it != mStates.end(); ++it )
		{
			if( (*it)->getName() == s->getName() ) return index;
			++index;
		}

		return -1;
	}

	NStateButton::State* NStateButton::getNextState()
	{
		int index = getIndexOfState(mCurrentState);
		
		int nextStateIndex = index + 1;
		
		if( nextStateIndex >= static_cast<int>(mStates.size()) ) nextStateIndex =  0;

		return mStates[nextStateIndex];
	}

	NStateButton::State* NStateButton::getPreviousState()
	{
		int index = getIndexOfState(mCurrentState);

		int prevStateIndex = index - 1;
		
		if( prevStateIndex < 0 ) prevStateIndex = static_cast<int>(mStates.size()) - 1;

		return mStates[prevStateIndex];
	}

	NStateButton::State* NStateButton::getState(Ogre::ushort index)
	{
		if( mStates.size() < index ) return NULL;
		return mStates[index];
	}

	NStateButton::State* NStateButton::getState(const Ogre::String& name)
	{
		std::vector<State*>::iterator it;
		for( it = mStates.begin(); it != mStates.end(); ++it )
		{
			if( (*it)->getName() == name ) return (*it);
		}

		return NULL;
	}

	bool NStateButton::onMouseButtonUp(MouseEventArgs& e)
	{
		e.widget = this;

		if( e.button == QuickGUI::MB_Left ) toggleNextState();
		else if( e.button == QuickGUI::MB_Right ) togglePreviousState();

		return Button::onMouseButtonUp(e);
	}

	bool NStateButton::onStateChanged(WidgetEventArgs& e)
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnStateChangedUserEventHandlers.begin(); it != mOnStateChangedUserEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	void NStateButton::setCurrentState(State* s)
	{
		// Make sure s is a state within this State Button
		if(getState(s->getName()) == NULL) return;

		mCurrentState = s;
		mWidgetMaterial = mCurrentState->getMaterial();
		mOverlayElement->setMaterialName(mWidgetMaterial);
		setText(mCurrentState->getText());

		Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
		mOverMaterialExists = mm->resourceExists(mWidgetMaterial+".over");
		mDownMaterialExists = mm->resourceExists(mWidgetMaterial+".down");

		WidgetEventArgs e(this);
		e.handled = false;
		onStateChanged(e);
	}

	void NStateButton::setCurrentState(Ogre::ushort index)
	{
		State* s = getState(index);
		if( s == NULL ) return;
		setCurrentState(s);
	}

	void NStateButton::setCurrentState(const Ogre::String& name)
	{
		State* s = getState(name);
		if( s == NULL ) return;
		setCurrentState(s);
	}

	void NStateButton::toggleNextState()
	{
		int index = getIndexOfState(mCurrentState);
		
		int nextStateIndex = index + 1;
		
		if( nextStateIndex >= static_cast<int>(mStates.size()) ) nextStateIndex =  0;

		setCurrentState(mStates[nextStateIndex]);
	}

	void NStateButton::togglePreviousState()
	{
		int index = getIndexOfState(mCurrentState);

		int prevStateIndex = index - 1;
		
		if( prevStateIndex < 0 ) prevStateIndex = static_cast<int>(mStates.size()) - 1;

		setCurrentState(mStates[prevStateIndex]);
	}
}