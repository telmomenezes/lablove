#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	Panel::Panel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* parentWidget) :
		Widget(name,dimensions,material,overlayContainer,parentWidget),
		mAutoNameWidgetCounter(0),
		mNumButtons(0),
		mNumComboBoxes(0),
		mNumImages(0),
		mNumLabels(0),
		mNumMenus(0),
		mNumNStateButtons(0),
		mNumPanels(0),
		mNumProgressBars(0),
		mNumTextBoxes(0)
	{
		mWidgetType = Widget::QGUI_TYPE_PANEL;
		mZOrderValues.clear();

		if( overlayContainer == NULL )
		{
			mOverlayContainer = createOverlayContainer(mInstanceName+".OverlayContainer","");
			mOverlayContainer->addChildImpl(mChildrenContainer);

			mOverlayContainer->show();
			mChildrenContainer->show();
		}

		mOverlayElement = createPanelOverlayElement(mInstanceName+".Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mOverlayElement);
		mOverlayElement->show();
	}

	Panel::~Panel()
	{
	}

	void Panel::_addZOrderValue(int zOrder)
	{
		mZOrderValues.push_back(zOrder);
		mZOrderValues.sort();
	}

	void Panel::_removeZOrderValue(int zOrder)
	{
		std::list<int>::iterator it;
		for( it = mZOrderValues.begin(); it != mZOrderValues.end(); ++it )
		{
			if( *it == zOrder ) mZOrderValues.erase(it);
			return;
		}
	}

	Button* Panel::_createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Button* newButton = new Button(name,dimensions,material,mChildrenContainer,this);
		newButton->setZOrderOffset(1);
		if(!mVisible) newButton->hide();
		Widget::_addChildWidget(newButton);
		// update count
		++mNumButtons;
		
		return newButton;
	}

	ComboBox* Panel::_createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		ComboBox* newComboBox = new ComboBox(name,dimensions,material,mChildrenContainer,this);
		newComboBox->setZOrderOffset(1);
		if(!mVisible) newComboBox->hide();
		Widget::_addChildWidget(newComboBox);
		// update count
		++mNumComboBoxes;
		
		return newComboBox;
	}

	Image* Panel::_createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture)
	{
		Ogre::String defaultMaterial = getSheet()->getDefaultSkin() + ".image";
		Image* newImage = new Image(name,dimensions,defaultMaterial,mChildrenContainer,this);
		if(texture) newImage->setMaterial(material,true);
		else newImage->setMaterial(material);
		newImage->setZOrderOffset(1);
		if(!mVisible) newImage->hide();
		Widget::_addChildWidget(newImage);
		// update count
		++mNumImages;

		return newImage;
	}

	Label* Panel::_createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Label* newLabel = new Label(name,dimensions,material,mChildrenContainer,this);
		newLabel->setZOrderOffset(1);
		if(!mVisible) newLabel->hide();
		Widget::_addChildWidget(newLabel);
		// update count
		++mNumLabels;

		return newLabel;
	}

	Menu* Panel::_createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Menu* newMenu = new Menu(name,dimensions,material,mChildrenContainer,this);
		newMenu->setZOrderOffset(1);
		if(!mVisible) newMenu->hide();
		Widget::_addChildWidget(newMenu);
		// update count
		++mNumMenus;

		return newMenu;
	}

	NStateButton* Panel::_createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		NStateButton* newNStateButton = new NStateButton(name,dimensions,mChildrenContainer,this);
		newNStateButton->setZOrderOffset(1);
		if(!mVisible) newNStateButton->hide();
		Widget::_addChildWidget(newNStateButton);
		// update count
		++mNumNStateButtons;

		return newNStateButton;
	}

	Panel* Panel::_createPanel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Panel* newPanel = new Panel(name,dimensions,material,mChildrenContainer,this);
		newPanel->setZOrderOffset(1);
		if(!mVisible) newPanel->hide();
		Widget::_addChildWidget(newPanel);
		// update count
		++mNumPanels;

		return newPanel;
	}

	ProgressBar* Panel::_createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		ProgressBar* newProgressBar = new ProgressBar(name,dimensions,material,mChildrenContainer,this);
		newProgressBar->setZOrderOffset(1);
		if(!mVisible) newProgressBar->hide();
		Widget::_addChildWidget(newProgressBar);
		// update count
		++mNumProgressBars;

		return newProgressBar;
	}

	TextBox* Panel::_createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		TextBox* newTextBox = new TextBox(name,dimensions,material,mChildrenContainer,this);
		newTextBox->setZOrderOffset(1);
		if(!mVisible) newTextBox->hide();
		Widget::_addChildWidget(newTextBox);
		// update count
		++mNumTextBoxes;

		return newTextBox;
	}

	bool Panel::_destroyWidget(Widget::Type TYPE, unsigned int index)
	{
		int counter = -1;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE )
			{
				++counter;
				if( counter == index )
				{
					Widget* w = (*it);
					mChildWidgets.erase(it);
					delete w;
					return true;
				}
			}
		}
		return false;
	}

	bool Panel::_destroyWidget(const Ogre::String& name)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name )
			{
				Widget* w = (*it);
				mChildWidgets.erase(it);
				delete w;
				return true;
			}
		}
		return false;
	}

	Widget* Panel::_getWidget(Widget::Type TYPE, unsigned int index)
	{
		int counter = -1;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE )
			{
				++counter;
				if( counter == index ) return (*it);
			}
		}
		return NULL;
	}

	Widget* Panel::_getWidget(const Ogre::String& name)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}
		return NULL;
	}

	void Panel::activate(EventArgs& e)
	{
		Widget::activate(e);

		std::vector<Widget*>::iterator wIt;
		for( wIt = mChildWidgets.begin(); wIt != mChildWidgets.end(); ++wIt )
		{
			(*wIt)->activate(e);
		}
	}

	Button* Panel::createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createButton(name,dimensions,material);
	}

	Button* Panel::createButton(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createButton(name,dimensions,material);
	}

	Button* Panel::createButton(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".button";

		return _createButton(name,dimensions,material);
	}

	Button* Panel::createButton(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".button";

		return _createButton(name,dimensions,material);
	}

	ComboBox* Panel::createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createComboBox(name,dimensions,material);
	}

	ComboBox* Panel::createComboBox(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createComboBox(name,dimensions,material);
	}

	ComboBox* Panel::createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;
		
		Ogre::String material = getSheet()->getDefaultSkin() + ".combobox";

		return _createComboBox(name,dimensions,material);
	}

	ComboBox* Panel::createComboBox(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;
		
		Ogre::String material = getSheet()->getDefaultSkin() + ".combobox";

		return _createComboBox(name,dimensions,material);
	}

	Image* Panel::createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createImage(name,dimensions,material,texture);
	}

	Image* Panel::createImage(const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createImage(name,dimensions,material,texture);
	}

	Image* Panel::createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, bool texture)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".image";

		return _createImage(name,dimensions,material,texture);
	}

	Image* Panel::createImage(const Ogre::Vector4& dimensions, bool texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".image";

		return _createImage(name,dimensions,material,texture);
	}

	Label* Panel::createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createLabel(name,dimensions,material);
	}

	Label* Panel::createLabel(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createLabel(name,dimensions,material);
	}

	Label* Panel::createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".label";

		return _createLabel(name,dimensions,material);
	}
	
	Label* Panel::createLabel(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".label";

		return _createLabel(name,dimensions,material);
	}

	Menu* Panel::createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createMenu(name,dimensions,material);
	}

	Menu* Panel::createMenu(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createMenu(name,dimensions,material);
	}

	Menu* Panel::createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".menu";

		return _createMenu(name,dimensions,material);
	}

	Menu* Panel::createMenu(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".menu";

		return _createMenu(name,dimensions,material);
	}

	NStateButton* Panel::createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createNStateButton(name,dimensions);
	}

	NStateButton* Panel::createNStateButton(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createNStateButton(name,dimensions);
	}

	Panel* Panel::createPanel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createPanel(name,dimensions,material);
	}

	Panel* Panel::createPanel(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createPanel(name,dimensions,material);
	}

	Panel* Panel::createPanel(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".panel";

		return _createPanel(name,dimensions,material);
	}

	Panel* Panel::createPanel(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".panel";

		return _createPanel(name,dimensions,material);
	}

	ProgressBar* Panel::createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createProgressBar(name,dimensions,material);
	}

	ProgressBar* Panel::createProgressBar(const Ogre::Vector4& dimensions,  const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createProgressBar(name,dimensions,material);
	}

	ProgressBar* Panel::createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".progressbar";

		return _createProgressBar(name,dimensions,material);
	}

	ProgressBar* Panel::createProgressBar(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".progressbar";

		return _createProgressBar(name,dimensions,material);
	}

	TextBox* Panel::createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		return _createTextBox(name,dimensions,material);
	}

	TextBox* Panel::createTextBox(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createTextBox(name,dimensions,material);
	}

	TextBox* Panel::createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions)
	{
		if( !(GUIManager::getSingleton().validWidgetName(name)) ) return NULL;

		Ogre::String material = getSheet()->getDefaultSkin() + ".textbox";

		return _createTextBox(name,dimensions,material);
	}

	TextBox* Panel::createTextBox(const Ogre::Vector4& dimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String material = getSheet()->getDefaultSkin() + ".textbox";

		return _createTextBox(name,dimensions,material);
	}

	void Panel::destroyButton(unsigned int index)
	{
		if( index >= mNumButtons ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_BUTTON,index))
			--mNumButtons;
	}

	void Panel::destroyButton(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumButtons;
	}

	void Panel::destroyButton(Button* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumButtons;
	}

	void Panel::destroyComboBox(unsigned int index)
	{
		if( index >= mNumComboBoxes ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_COMBOBOX,index))
			--mNumComboBoxes;
	}

	void Panel::destroyComboBox(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumComboBoxes;;
	}

	void Panel::destroyComboBox(ComboBox* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumComboBoxes;;
	}

	void Panel::destroyImage(unsigned int index)
	{
		if( index >= mNumImages ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_IMAGE,index))
			--mNumImages;
	}

	void Panel::destroyImage(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumImages;
	}

	void Panel::destroyImage(Image* i)
	{
		if(_destroyWidget(i->getInstanceName()))
			--mNumImages;
	}

	void Panel::destroyLabel(unsigned int index)
	{
		if( index >= mNumLabels ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_LABEL,index))
			--mNumLabels;
	}

	void Panel::destroyLabel(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumLabels;
	}

	void Panel::destroyLabel(Label* l)
	{
		if(_destroyWidget(l->getInstanceName()))
			--mNumLabels;
	}

	void Panel::destroyMenu(unsigned int index)
	{
		if( index >= mNumMenus ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_MENU,index))
			--mNumMenus;
	}

	void Panel::destroyMenu(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumMenus;
	}

	void Panel::destroyMenu(Menu* m)
	{
		if(_destroyWidget(m->getInstanceName()))
			--mNumMenus;
	}

	void Panel::destroyNStateButton(unsigned int index)
	{
		if( index >= mNumNStateButtons ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_NSTATEBUTTON,index))
			--mNumNStateButtons;
	}

	void Panel::destroyNStateButton(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumNStateButtons;
	}

	void Panel::destroyNStateButton(NStateButton* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumNStateButtons;
	}

	void Panel::destroyPanel(unsigned int index)
	{
		if( index >= mNumNStateButtons ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_PANEL,index))
			--mNumPanels;
	}

	void Panel::destroyPanel(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumPanels;
	}

	void Panel::destroyPanel(Panel* p)
	{
		if(_destroyWidget(p->getInstanceName()))
			--mNumPanels;
	}

	void Panel::destroyProgressBar(unsigned int index)
	{
		if( index >= mNumProgressBars ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_PROGRESSBAR,index))
			--mNumProgressBars;
	}

	void Panel::destroyProgressBar(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumProgressBars;
	}

	void Panel::destroyProgressBar(ProgressBar* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumProgressBars;
	}

	void Panel::destroyTextBox(unsigned int index)
	{
		if( index >= mNumTextBoxes ) return;

		if(_destroyWidget(Widget::QGUI_TYPE_TEXTBOX,index))
			--mNumTextBoxes;
	}

	void Panel::destroyTextBox(const Ogre::String& name)
	{
		if(_destroyWidget(name))
			--mNumTextBoxes;
	}

	void Panel::destroyTextBox(TextBox* b)
	{
		if(_destroyWidget(b->getInstanceName()))
			--mNumTextBoxes;
	}

	void Panel::deactivate(EventArgs& e)
	{
		Widget::deactivate(e);

		std::vector<Widget*>::iterator wIt;
		for( wIt = mChildWidgets.begin(); wIt != mChildWidgets.end(); ++wIt )
			(*wIt)->deactivate(e);
	}

	Button* Panel::getButton(unsigned int index)
	{
		if( index >= mNumButtons ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_BUTTON,index);
		if( w != NULL ) return dynamic_cast<Button*>(w);
		else return NULL;
	}

	Button* Panel::getButton(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Button*>(w);
		else return NULL;
	}

	ComboBox* Panel::getComboBox(unsigned int index)
	{
		if( index >= mNumComboBoxes ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_COMBOBOX,index);
		if( w != NULL ) return dynamic_cast<ComboBox*>(w);
		else return NULL;
	}

	ComboBox* Panel::getComboBox(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<ComboBox*>(w);
		else return NULL;
	}

	Image* Panel::getImage(unsigned int index)
	{
		if( index >= mNumImages ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_IMAGE,index);
		if( w != NULL ) return dynamic_cast<Image*>(w);
		else return NULL;
	}

	Image* Panel::getImage(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Image*>(w);
		else return NULL;
	}

	Label* Panel::getLabel(unsigned int index)
	{
		if( index >= mNumLabels ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_LABEL,index);
		if( w != NULL ) return dynamic_cast<Label*>(w);
		else return NULL;
	}

	Label* Panel::getLabel(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Label*>(w);
		else return NULL;
	}

	int Panel::getMaxZOrderValue()
	{
		return mZOrderValues.back();
	}

	Menu* Panel::getMenu( unsigned int index )
	{
		if( index >= mNumMenus ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_MENU,index);
		if( w != NULL ) return dynamic_cast<Menu*>(w);
		else return NULL;
	}

	Menu* Panel::getMenu(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Menu*>(w);
		else return NULL;
	}

	NStateButton* Panel::getNStateButton(unsigned int index)
	{
		if( index >= mNumNStateButtons ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_NSTATEBUTTON,index);
		if( w != NULL ) return dynamic_cast<NStateButton*>(w);
		else return NULL;
	}
		
	NStateButton* Panel::getNStateButton(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<NStateButton*>(w);
		else return NULL;
	}

	Panel* Panel::getPanel(unsigned int index)
	{
		if( index >= mNumPanels ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_PANEL,index);
		if( w != NULL ) return dynamic_cast<Panel*>(w);
		else return NULL;
	}

	Panel* Panel::getPanel(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Panel*>(w);
		else return NULL;
	}

	ProgressBar* Panel::getProgressBar(unsigned int index)
	{
		if( index >= mNumProgressBars ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_PROGRESSBAR,index);
		if( w != NULL ) return dynamic_cast<ProgressBar*>(w);
		else return NULL;
	}

	ProgressBar* Panel::getProgressBar(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<ProgressBar*>(w);
		else return NULL;
	}

	TextBox* Panel::getTextBox(unsigned int index)
	{
		if( index >= mNumTextBoxes ) return NULL;

		Widget* w = _getWidget(Widget::QGUI_TYPE_TEXTBOX,index);
		if( w != NULL ) return dynamic_cast<TextBox*>(w);
		else return NULL;
	}

	TextBox* Panel::getTextBox(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<TextBox*>(w);
		else return NULL;
	}
}
