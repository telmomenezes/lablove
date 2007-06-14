#ifndef QUICKGUIPANEL_H
#define QUICKGUIPANEL_H

#include "QuickGUIButton.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIImage.h"
#include "QuickGUIMenu.h"
#include "QuickGUINStateButton.h"
#include "QuickGUIPrerequisites.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIText.h"
#include "QuickGUITextBox.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	class Panel :
		public Widget
	{
	public:
		Panel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* parentWidget);
		virtual ~Panel();

		void _addZOrderValue(int zOrder);
		void _removeZOrderValue(int zOrder);

		Button* createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		Button* createButton(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Button* createButton(const Ogre::String& name, const Ogre::Vector4& dimensions);
		Button* createButton(const Ogre::Vector4& dimensions);

		ComboBox* createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		ComboBox* createComboBox(const Ogre::Vector4& dimensions, const Ogre::String& material);
		ComboBox* createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions);
		ComboBox* createComboBox(const Ogre::Vector4& dimensions);

		Image* createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture);
		Image* createImage(const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture);
		Image* createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, bool texture);
		Image* createImage(const Ogre::Vector4& dimensions, bool texture);

		Label* createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		Label* createLabel(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Label* createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions);
		Label* createLabel(const Ogre::Vector4& dimensions);

		Menu* createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		Menu* createMenu(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Menu* createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions);
		Menu* createMenu(const Ogre::Vector4& dimensions);

		NStateButton* createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions);
		NStateButton* createNStateButton(const Ogre::Vector4& dimensions);

		Panel* createPanel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		Panel* createPanel(const Ogre::Vector4& dimensions, const Ogre::String& material);
		Panel* createPanel(const Ogre::String& name, const Ogre::Vector4& dimensions);
		Panel* createPanel(const Ogre::Vector4& dimensions);

		ProgressBar* createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		ProgressBar* createProgressBar(const Ogre::Vector4& dimensions, const Ogre::String& material);
		ProgressBar* createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions);
		ProgressBar* createProgressBar(const Ogre::Vector4& dimensions);

		TextBox* createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		TextBox* createTextBox(const Ogre::Vector4& dimensions, const Ogre::String& material);
		TextBox* createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions);
		TextBox* createTextBox(const Ogre::Vector4& dimensions);

		void destroyButton(unsigned int index);
		void destroyButton(const Ogre::String& name);
		void destroyButton(Button* b);

		void destroyComboBox(unsigned int index);
		void destroyComboBox(const Ogre::String& name);
		void destroyComboBox(ComboBox* b);

		void destroyImage(unsigned int index);
		void destroyImage(const Ogre::String& name);
		void destroyImage(Image* i);

		void destroyLabel(unsigned int index);
		void destroyLabel(const Ogre::String& name);
		void destroyLabel(Label* b);

		void destroyMenu(unsigned int index);
		void destroyMenu(const Ogre::String& name);
		void destroyMenu(Menu* b);

		void destroyNStateButton(unsigned int index);
		void destroyNStateButton(const Ogre::String& name);
		void destroyNStateButton(NStateButton* b);

		void destroyPanel(unsigned int index);
		void destroyPanel(const Ogre::String& name);
		void destroyPanel(Panel* p);

		void destroyProgressBar(unsigned int index);
		void destroyProgressBar(const Ogre::String& name);
		void destroyProgressBar(ProgressBar* b);

		void destroyTextBox(unsigned int index);
		void destroyTextBox(const Ogre::String& name);
		void destroyTextBox(TextBox* b);

		Button* getButton(unsigned int index);
		Button* getButton(const Ogre::String& name);

		ComboBox* getComboBox(unsigned int index);
		ComboBox* getComboBox(const Ogre::String& name);

		Image* getImage(unsigned int index);
		Image* getImage(const Ogre::String& name);

		Label* getLabel(unsigned int index);
		Label* getLabel(const Ogre::String& name);

		// returns the highest zOrder value used by any of its child widgets.
		int getMaxZOrderValue();
		
		Menu* getMenu(unsigned int index);
		Menu* getMenu(const Ogre::String& name);

		NStateButton* getNStateButton(unsigned int index);
		NStateButton* getNStateButton(const Ogre::String& name);

		Panel* getPanel(unsigned int index);
		Panel* getPanel(const Ogre::String& name);

		ProgressBar* getProgressBar(unsigned int index);
		ProgressBar* getProgressBar(const Ogre::String& name);
		
		TextBox* getTextBox(unsigned int index);
		TextBox* getTextBox(const Ogre::String& name);

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		void activate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		void deactivate(EventArgs& e);

	protected:
		std::list<int>						mZOrderValues;

		int									mAutoNameWidgetCounter;

		// Recording the number of widgets created
		unsigned int						mNumButtons;
		unsigned int						mNumComboBoxes;
		unsigned int						mNumImages;
		unsigned int						mNumLabels;
		unsigned int						mNumMenus;
		unsigned int						mNumNStateButtons;
		unsigned int						mNumPanels;
		unsigned int						mNumProgressBars;
		unsigned int						mNumTextBoxes;

		/** 
		* Private functions preventing users from setting the Widget Instance Name.  Names
		* can be given to Windows using the "setReferenceName()" function.
        */
		Button* _createButton(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		ComboBox* _createComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		Image* _createImage(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool texture);
		Label* _createLabel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		// virtual, because Sheets, which contain multiple windows, need to have an overlay made for menus that are to keep highest zOrder.
		virtual Menu* _createMenu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		NStateButton* _createNStateButton(const Ogre::String& name, const Ogre::Vector4& dimensions);
		Panel* _createPanel(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		ProgressBar* _createProgressBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);
		TextBox* _createTextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material);

		bool _destroyWidget(Widget::Type TYPE, unsigned int index);
		bool _destroyWidget(const Ogre::String& name);

		Widget* _getWidget(Widget::Type TYPE, unsigned int index);
		Widget* _getWidget(const Ogre::String& name);
	};
}

#endif
