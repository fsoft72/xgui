#include "../../xgui/xgui.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace xgui;

// Template for member function text callbacks (not in callback.h)
template <typename ObjectType>
class CppMTextCallback : public TextCallback
{
	private:
		ObjectType * obj;
		int (ObjectType::*func)(Widget*, std::string const &text);

	public:
		CppMTextCallback(ObjectType * const o, int(ObjectType::*fptr)(Widget*, std::string const &))
		: TextCallback(), obj(o), func(fptr) {}
		virtual ~CppMTextCallback() {}
		virtual int call(Widget * w, std::string const &text) { return (obj->*func)(w, text); }
};

/**
 * Complex Widget Example - Showcasing All XGUI Widgets
 *
 * This example demonstrates all available widgets in the XGUI library
 * with interactive events that:
 * - Log values to the console
 * - Reflect changes across widgets (e.g., slider updates progress bar)
 * - Show real-time interactions between different UI components
 */
class ComplexWidgetDemo : public Window
{
private:
	// Layout containers
	Tab * main_tab;
	VBox * basic_tab;
	VBox * input_tab;
	VBox * data_tab;
	VBox * advanced_tab;

	// Basic Controls (Tab 1)
	Label * info_label;
	Button * action_btn;
	Button * toggle_btn;
	Checkbox * enable_checkbox;
	Label * click_counter_label;
	int click_count;

	// Input Controls (Tab 2)
	Entry * name_entry;
	Entry * password_entry;
	Slider * slider_widget;
	Progressbar * progress_bar;
	Spin * spin_widget;
	Label * slider_value_label;
	Label * spin_value_label;
	Combobox * combo_widget;
	Label * combo_label;

	// Data View Widgets (Tab 3)
	List * data_list;
	Model * list_model;
	Tree * tree_widget;
	Model * tree_model;
	Calendar * calendar_widget;
	Label * calendar_label;
	ImageView * image_view;

	// Advanced Tab (Tab 4)
	Frame * settings_frame;
	Toolbar * toolbar_widget;
	Label * status_label;
	Slider * opacity_slider;
	Label * opacity_label;

	// Menu
	Menu * menu_bar;

	// Callbacks
	Callback * cb_window_close;
	Callback * cb_action_btn;
	Callback * cb_toggle_btn;
	Callback * cb_checkbox;
	TextCallback * cb_name_entry;
	TextCallback * cb_password_entry;
	Callback * cb_slider_change;
	Callback * cb_spin_change;
	TextStatusCallback * cb_combo_select;
	TextStatusCallback * cb_list_select;
	TextStatusCallback * cb_tree_select;
	TextStatusCallback * cb_calendar_select;
	TextStatusCallback * cb_menu_select;
	TextStatusCallback * cb_tab_select;
	TextStatusCallback * cb_toolbar_click;
	Callback * cb_opacity_change;

public:
	ComplexWidgetDemo() : Window(0), click_count(0)
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "XGUI Complex Widget Demo Started" << std::endl;
		std::cout << "========================================\n" << std::endl;

		// Initialize callbacks
		cb_window_close = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onWindowClose);
		cb_action_btn = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onActionButton);
		cb_toggle_btn = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onToggleButton);
		cb_checkbox = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onCheckbox);
		cb_name_entry = new CppMTextCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onNameEntry);
		cb_password_entry = new CppMTextCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onPasswordEntry);
		cb_slider_change = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onSliderChange);
		cb_spin_change = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onSpinChange);
		cb_combo_select = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onComboSelect);
		cb_list_select = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onListSelect);
		cb_tree_select = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onTreeSelect);
		cb_calendar_select = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onCalendarSelect);
		cb_menu_select = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onMenuSelect);
		cb_tab_select = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onTabSelect);
		cb_toolbar_click = new CppMTextStatusCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onToolbarClick);
		cb_opacity_change = new CppMCallback<ComplexWidgetDemo>(this, &ComplexWidgetDemo::onOpacityChange);

		// Create UI
		createMenu();

		// Create main tab container first
		main_tab = Master::CreateTab(this, "top");

		// Create tab pages with main_tab as parent
		createBasicTab();
		createInputTab();
		createDataTab();
		createAdvancedTab();

		// Set page labels
		main_tab->setPageLabel(basic_tab, "Basic Controls");
		main_tab->setPageLabel(input_tab, "Input Widgets");
		main_tab->setPageLabel(data_tab, "Data Views");
		main_tab->setPageLabel(advanced_tab, "Advanced");
		main_tab->linkEvent("onselect", cb_tab_select);

		// Window setup
		linkEvent("onclose", cb_window_close);
		set("text", "XGUI Complex Widget Demo");
		set("size", "900 700");
		set("position", "center");
	}

	virtual ~ComplexWidgetDemo()
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "XGUI Complex Widget Demo Ended" << std::endl;
		std::cout << "========================================\n" << std::endl;

		delete cb_window_close;
		delete cb_action_btn;
		delete cb_toggle_btn;
		delete cb_checkbox;
		delete cb_name_entry;
		delete cb_password_entry;
		delete cb_slider_change;
		delete cb_spin_change;
		delete cb_combo_select;
		delete cb_list_select;
		delete cb_tree_select;
		delete cb_calendar_select;
		delete cb_menu_select;
		delete cb_tab_select;
		delete cb_toolbar_click;
		delete cb_opacity_change;
	}

	void createMenu()
	{
		menu_bar = Master::CreateMenu(this, true);
		menu_bar->addItem("", "file_menu", "&File");
		menu_bar->addItem("file_menu", "new_item", "&New");
		menu_bar->addItem("file_menu", "open_item", "&Open");
		menu_bar->addItem("file_menu", "save_item", "&Save");
		menu_bar->addItem("file_menu", "", ""); // separator
		menu_bar->addItem("file_menu", "exit_item", "E&xit");

		menu_bar->addItem("", "edit_menu", "&Edit");
		menu_bar->addItem("edit_menu", "copy_item", "&Copy");
		menu_bar->addItem("edit_menu", "paste_item", "&Paste");

		menu_bar->addItem("", "help_menu", "&Help");
		menu_bar->addItem("help_menu", "about_item", "&About");

		menu_bar->linkEvent("onselect", cb_menu_select);
	}

	void createBasicTab()
	{
		basic_tab = Master::CreateVBox(main_tab, 10, true);

		// Title
		Label * title = Master::CreateLabel(basic_tab, "Basic Controls Demo");
		title->set("font-weight", "bold");
		title->set("font-size", "16");

		// Info label
		info_label = Master::CreateLabel(basic_tab, "Welcome! This demo showcases all XGUI widgets.");

		// Click counter
		click_counter_label = Master::CreateLabel(basic_tab, "Button clicks: 0");
		click_counter_label->set("font-size", "14");

		// Action button
		action_btn = Master::CreateButton(basic_tab, "Click Me!");
		action_btn->linkEvent("onclick", cb_action_btn);

		// Toggle button
		toggle_btn = Master::CreateButton(basic_tab, "Toggle: OFF", 0, true);
		toggle_btn->linkEvent("onclick", cb_toggle_btn);

		// Checkbox
		HBox * checkbox_box = Master::CreateHBox(basic_tab, 5, false);
		enable_checkbox = Master::CreateCheckbox(checkbox_box, "Enable Advanced Features");
		enable_checkbox->set("value", "0");
		enable_checkbox->linkEvent("onclick", cb_checkbox);

		// Space filler
		Master::CreateSpace(basic_tab, 0, 0)->set("expand", "1.0");
	}

	void createInputTab()
	{
		input_tab = Master::CreateVBox(main_tab, 10, true);

		// Title
		Label * title = Master::CreateLabel(input_tab, "Input Widgets Demo");
		title->set("font-weight", "bold");
		title->set("font-size", "16");

		// Entry widgets
		Frame * entry_frame = Master::CreateFrame(input_tab, "Text Entry");
		VBox * entry_box = Master::CreateVBox(entry_frame, 5, false);

		HBox * name_box = Master::CreateHBox(entry_box, 5, false);
		Label * name_label = Master::CreateLabel(name_box, "Name:");
		name_label->set("size", "100 -1");
		name_entry = Master::CreateEntry(name_box, "Enter your name");
		name_entry->set("expand", "1.0");
		name_entry->linkEvent("onchange", cb_name_entry);

		HBox * pass_box = Master::CreateHBox(entry_box, 5, false);
		Label * pass_label = Master::CreateLabel(pass_box, "Password:");
		pass_label->set("size", "100 -1");
		password_entry = Master::CreateEntry(pass_box, "");
		password_entry->set("expand", "1.0");
		password_entry->set("password", "1");
		password_entry->linkEvent("onchange", cb_password_entry);

		// Slider and Progress Bar (linked)
		Frame * slider_frame = Master::CreateFrame(input_tab, "Slider & Progress Bar (Linked)");
		VBox * slider_box = Master::CreateVBox(slider_frame, 5, false);

		slider_value_label = Master::CreateLabel(slider_box, "Slider Value: 50");
		slider_widget = Master::CreateSlider(slider_box, 0, 100);
		slider_widget->set("value", "50");
		slider_widget->linkEvent("onchange", cb_slider_change);

		progress_bar = Master::CreateProgressbar(slider_box, 0, 100);
		progress_bar->set("value", "50");

		// Spinner
		Frame * spin_frame = Master::CreateFrame(input_tab, "Spinner Control");
		VBox * spin_box = Master::CreateVBox(spin_frame, 5, false);

		spin_value_label = Master::CreateLabel(spin_box, "Spinner Value: 10");
		HBox * spin_hbox = Master::CreateHBox(spin_box, 5, false);
		Label * spin_label = Master::CreateLabel(spin_hbox, "Count:");
		spin_label->set("size", "100 -1");
		spin_widget = Master::CreateSpin(spin_hbox, 0, 100);
		spin_widget->set("value", "10");
		spin_widget->set("expand", "1.0");
		spin_widget->linkEvent("onchange", cb_spin_change);

		// Combobox
		Frame * combo_frame = Master::CreateFrame(input_tab, "Combobox");
		VBox * combo_box = Master::CreateVBox(combo_frame, 5, false);

		combo_label = Master::CreateLabel(combo_box, "Selected: (none)");
		combo_widget = Master::CreateCombobox(combo_box, false);
		combo_widget->appendText("Option 1");
		combo_widget->appendText("Option 2");
		combo_widget->appendText("Option 3");
		combo_widget->appendText("Option 4");
		combo_widget->appendText("Option 5");
		combo_widget->linkEvent("onselect", cb_combo_select);

		// Space filler
		Master::CreateSpace(input_tab, 0, 0)->set("expand", "1.0");
	}

	void createDataTab()
	{
		data_tab = Master::CreateVBox(main_tab, 10, true);

		// Title
		Label * title = Master::CreateLabel(data_tab, "Data View Widgets");
		title->set("font-weight", "bold");
		title->set("font-size", "16");

		HBox * data_hbox = Master::CreateHBox(data_tab, 10, false);
		data_hbox->set("expand", "1.0");

		// List widget
		VBox * list_vbox = Master::CreateVBox(data_hbox, 5, false);
		list_vbox->set("expand", "1.0");
		Master::CreateLabel(list_vbox, "List Widget:");

		data_list = Master::CreateList(list_vbox, 0);
		data_list->set("expand", "1.0");
		data_list->addColumn(0, "col_name", "Name", 120);
		data_list->addColumn(1, "col_value", "Value", 80);

		list_model = Master::CreateModel();
		std::vector<std::string> row1;
		row1.push_back("Item 1");
		row1.push_back("100");
		list_model->appendChild(row1);

		std::vector<std::string> row2;
		row2.push_back("Item 2");
		row2.push_back("200");
		list_model->appendChild(row2);

		std::vector<std::string> row3;
		row3.push_back("Item 3");
		row3.push_back("300");
		list_model->appendChild(row3);

		data_list->setModel(list_model);
		data_list->linkEvent("onselect", cb_list_select);

		// Tree widget
		VBox * tree_vbox = Master::CreateVBox(data_hbox, 5, false);
		tree_vbox->set("expand", "1.0");
		Master::CreateLabel(tree_vbox, "Tree Widget:");

		tree_widget = Master::CreateTree(tree_vbox, 0);
		tree_widget->set("expand", "1.0");

		tree_model = Master::CreateModel();
		Model * root1 = Master::CreateModel();
		root1->addString("Root 1");
		Model * child1_1 = Master::CreateModel();
		child1_1->addString("Child 1.1");
		Model * child1_2 = Master::CreateModel();
		child1_2->addString("Child 1.2");
		root1->appendChild(child1_1);
		root1->appendChild(child1_2);
		tree_model->appendChild(root1);

		Model * root2 = Master::CreateModel();
		root2->addString("Root 2");
		Model * child2_1 = Master::CreateModel();
		child2_1->addString("Child 2.1");
		root2->appendChild(child2_1);
		tree_model->appendChild(root2);

		tree_widget->setModel(tree_model);
		tree_widget->linkEvent("onselect", cb_tree_select);

		// Calendar widget
		VBox * calendar_vbox = Master::CreateVBox(data_tab, 5, false);
		calendar_label = Master::CreateLabel(calendar_vbox, "Selected Date: (none)");
		calendar_widget = Master::CreateCalendar(calendar_vbox);
		calendar_widget->linkEvent("onselect", cb_calendar_select);
	}

	void createAdvancedTab()
	{
		advanced_tab = Master::CreateVBox(main_tab, 10, true);

		// Title
		Label * title = Master::CreateLabel(advanced_tab, "Advanced Widgets");
		title->set("font-weight", "bold");
		title->set("font-size", "16");

		// Toolbar
		toolbar_widget = Master::CreateToolbar(advanced_tab);
		toolbar_widget->addItem("btn_new", "New", 0, "New document");
		toolbar_widget->addItem("btn_open", "Open", 0, "Open file");
		toolbar_widget->addItem("btn_save", "Save", 0, "Save file");
		toolbar_widget->addItem("sep1", "", 0, "", "separator");
		toolbar_widget->addItem("btn_cut", "Cut", 0, "Cut selection");
		toolbar_widget->addItem("btn_copy", "Copy", 0, "Copy selection");
		toolbar_widget->addItem("btn_paste", "Paste", 0, "Paste from clipboard");
		toolbar_widget->linkEvent("onclick", cb_toolbar_click);

		// Settings Frame
		settings_frame = Master::CreateFrame(advanced_tab, "Settings");
		VBox * settings_vbox = Master::CreateVBox(settings_frame, 10, false);

		// Opacity control
		opacity_label = Master::CreateLabel(settings_vbox, "Opacity: 100%");
		opacity_slider = Master::CreateSlider(settings_vbox, 0, 100);
		opacity_slider->set("value", "100");
		opacity_slider->linkEvent("onchange", cb_opacity_change);

		// Status label
		status_label = Master::CreateLabel(advanced_tab, "Status: Ready");
		status_label->set("font-style", "italic");

		// Space filler
		Master::CreateSpace(advanced_tab, 0, 0)->set("expand", "1.0");
	}

	// Event Handlers
	int onWindowClose(Widget * w)
	{
		std::cout << "[EVENT] Window closing..." << std::endl;
		Master::Quit();
		return EVT_PROPAGATE;
	}

	int onActionButton(Widget * w)
	{
		click_count++;
		std::cout << "[EVENT] Action button clicked! Total clicks: " << click_count << std::endl;

		std::ostringstream oss;
		oss << "Button clicks: " << click_count;
		click_counter_label->set("text", oss.str());

		info_label->set("text", "Action button was clicked!");

		return EVT_PROPAGATE;
	}

	int onToggleButton(Widget * w)
	{
		std::string pressed;
		toggle_btn->get("pressed", pressed);

		std::cout << "[EVENT] Toggle button changed! New state: " << pressed << std::endl;

		if (pressed == "1") {
			toggle_btn->set("text", "Toggle: ON");
			info_label->set("text", "Toggle is now ON");
		} else {
			toggle_btn->set("text", "Toggle: OFF");
			info_label->set("text", "Toggle is now OFF");
		}

		return EVT_PROPAGATE;
	}

	int onCheckbox(Widget * w)
	{
		std::string value;
		enable_checkbox->get("value", value);

		std::cout << "[EVENT] Checkbox changed! Enabled: " << (value == "1" ? "Yes" : "No") << std::endl;

		if (value == "1") {
			info_label->set("text", "Advanced features ENABLED");
			settings_frame->set("enabled", "1");
		} else {
			info_label->set("text", "Advanced features DISABLED");
			settings_frame->set("enabled", "0");
		}

		return EVT_PROPAGATE;
	}

	int onNameEntry(Widget * w, std::string const &text)
	{
		std::cout << "[EVENT] Name entry changed: '" << text << "'" << std::endl;

		if (!text.empty()) {
			std::string msg = "Hello, " + text + "!";
			info_label->set("text", msg);
		} else {
			info_label->set("text", "Enter your name in the text field");
		}

		return EVT_PROPAGATE;
	}

	int onPasswordEntry(Widget * w, std::string const &text)
	{
		std::cout << "[EVENT] Password entry changed (length: " << text.length() << " characters)" << std::endl;
		return EVT_PROPAGATE;
	}

	int onSliderChange(Widget * w)
	{
		std::string value;
		slider_widget->get("value", value);

		std::cout << "[EVENT] Slider changed: " << value << std::endl;

		// Update label
		std::string label_text = "Slider Value: " + value;
		slider_value_label->set("text", label_text);

		// Update linked progress bar
		progress_bar->set("value", value);

		return EVT_PROPAGATE;
	}

	int onSpinChange(Widget * w)
	{
		std::string value;
		spin_widget->get("value", value);

		std::cout << "[EVENT] Spinner changed: " << value << std::endl;

		// Update label showing current spinner value
		std::string label_text = "Spinner Value: " + value;
		spin_value_label->set("text", label_text);

		return EVT_PROPAGATE;
	}

	int onComboSelect(Widget * w, std::string const &id, int state)
	{
		std::string text;
		combo_widget->get("text", text);

		std::cout << "[EVENT] Combobox selection: '" << text << "'" << std::endl;

		std::string label_text = "Selected: " + text;
		combo_label->set("text", label_text);

		return EVT_PROPAGATE;
	}

	int onListSelect(Widget * w, std::string const &id, int state)
	{
		if (state) {
			Model * selected = dynamic_cast<Model*>(list_model->findByPath(id));
			if (selected) {
				std::string name = selected->getString(0);
				std::string value = selected->getString(1);

				std::cout << "[EVENT] List item selected: " << name << " = " << value << std::endl;

				std::string msg = "Selected list item: " + name;
				status_label->set("text", msg);
			}
		} else {
			std::cout << "[EVENT] List item deselected" << std::endl;
			status_label->set("text", "Status: Ready");
		}

		return EVT_PROPAGATE;
	}

	int onTreeSelect(Widget * w, std::string const &id, int state)
	{
		if (state) {
			Model * selected = dynamic_cast<Model*>(tree_model->findByPath(id));
			if (selected) {
				std::string name = selected->getString(0);

				std::cout << "[EVENT] Tree item selected: " << name << std::endl;

				std::string msg = "Selected tree item: " + name;
				status_label->set("text", msg);
			}
		} else {
			std::cout << "[EVENT] Tree item deselected" << std::endl;
			status_label->set("text", "Status: Ready");
		}

		return EVT_PROPAGATE;
	}

	int onCalendarSelect(Widget * w, std::string const &date, int state)
	{
		std::cout << "[EVENT] Calendar date selected: " << date << std::endl;

		std::string label_text = "Selected Date: " + date;
		calendar_label->set("text", label_text);

		return EVT_PROPAGATE;
	}

	int onMenuSelect(Widget * w, std::string const &id, int state)
	{
		std::cout << "[EVENT] Menu item selected: " << id << std::endl;

		if (id == "exit_item") {
			std::cout << "[EVENT] Exit menu item selected, closing..." << std::endl;
			Master::Quit();
		} else if (id == "about_item") {
			info_label->set("text", "XGUI Complex Widget Demo v1.0");
		} else {
			std::string msg = "Menu action: " + id;
			status_label->set("text", msg);
		}

		return EVT_PROPAGATE;
	}

	int onTabSelect(Widget * w, std::string const &id, int state)
	{
		std::cout << "[EVENT] Tab changed to page: " << id << std::endl;

		std::string msg = "Switched to tab: " + id;
		if (info_label) {
			info_label->set("text", msg);
		}

		return EVT_PROPAGATE;
	}

	int onToolbarClick(Widget * w, std::string const &id, int state)
	{
		std::cout << "[EVENT] Toolbar button clicked: " << id << std::endl;

		std::string action = "Toolbar action: ";
		if (id == "btn_new") action += "New";
		else if (id == "btn_open") action += "Open";
		else if (id == "btn_save") action += "Save";
		else if (id == "btn_cut") action += "Cut";
		else if (id == "btn_copy") action += "Copy";
		else if (id == "btn_paste") action += "Paste";
		else action += id;

		status_label->set("text", action);

		return EVT_PROPAGATE;
	}

	int onOpacityChange(Widget * w)
	{
		std::string value;
		opacity_slider->get("value", value);

		std::cout << "[EVENT] Opacity changed: " << value << "%" << std::endl;

		std::string label_text = "Opacity: " + value + "%";
		opacity_label->set("text", label_text);

		return EVT_PROPAGATE;
	}

	void run()
	{
		show();
		Master::Run();
	}
};

int main(int argc, char* argv[])
{
	std::cout << "Initializing XGUI..." << std::endl;

	// Initialize dynamic classes (required for widget creation)
	Master::InitDynamicClasses();

	std::cout << "Creating complex widget demo window..." << std::endl;

	// Create and run the demo
	ComplexWidgetDemo * demo = new ComplexWidgetDemo();
	demo->run();

	std::cout << "Application terminated." << std::endl;

	return 0;
}
