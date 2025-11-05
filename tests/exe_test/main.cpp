#include "../../xgui/xgui.h"
#include <iostream>

using namespace xgui;

int OnWindowClose(xgui::Widget * w)
{
	Master::Quit();
	return EVT_PROPAGATE;
}
CppFCallback cb_on_window_close(OnWindowClose);

int on_img_mousedown(Widget *w, const std::string & button, int flags)
{
	if ( button == "right" ) {
		Container * ctx = (Container *)w->getContext();
		Menu * mnu = (Menu *)ctx->findChild ( "mnu" );

		mnu->popupMenu ( "Edit_mnu" );
	}

	return EVT_PROPAGATE;
} 
CppFTextStatusCallback cb_on_img_mousedown(on_img_mousedown);


int on_idle(Widget * w)
{
	std::cout << "on idle " << w->className() << std::endl;
	return 0;
}
CppFCallback cb_on_idle(on_idle);


int on_menu_select(Widget *w, const std::string & item_id, int status)
{
	std::cout << "MENU select: " << item_id << " - status: " << status << std::endl;
	std::vector<xgui::FileExtension> extensions;

	if (item_id == "Open_mnu") {
		extensions.push_back(FileExtension("Text", "*.txt"));
		xgui::StrVector files = xgui::Master::OpenFileDialog( (xgui::Window*)w->getContext(), extensions , "Open File Test", "", true);

		if (files.empty())
			std::cout << "No files selected" << std::endl;
		else
			for(xgui::StrVector::iterator i = files.begin(); i != files.end(); ++i)
				std::cout << *i << std::endl;
	}
	else if (item_id == "Save_mnu") {
		std::string file = xgui::Master::SaveFileDialog( (xgui::Window*)w->getContext(), extensions, "Save File Test", "", "pippo.txt");

		if (file.empty())
			std::cout << "No file selected" << std::endl;
		else
			std::cout << file << std::endl;
	}


	return EVT_PROPAGATE;
} 
CppFTextStatusCallback cb_on_menu_select(on_menu_select);


int on_slider_changepos(Widget *w, const std::string & pos)
{
	Container * ctx = (Container *)w->getContext();
	Progressbar * pbar = (Progressbar *)ctx->findChild ( "pbar" );

	pbar->setPos(xgui::semantic_cast<int> (pos));
	return EVT_PROPAGATE;
} 
CppFTextCallback cb_on_slider_changepos(on_slider_changepos);


int on_btn_click(Widget *w)
{
	std::string path;
	w->get("path", path);

	xgui::Button * b = dynamic_cast<xgui::Button*>(w);
	
	if (xgui::Object::Unpack(b->call("getIcon"))) {
		std::cout << "Distruggo widget: " << path << std::endl;
		Master::DestroyWidget(w);
	}
	else
		b->call("setIcon", xgui::Master::LoadImage("icon.png")->pack());
		
	return EVT_PROPAGATE;
} 
CppFCallback cb_on_btn_click(on_btn_click);

int on_dialog_quit(Widget * btn)
{
	xgui::Window * dialog = btn->getContext();

	dialog->modalReturn(0);
	return EVT_PROPAGATE;
}
CppFCallback cb_on_dialog_quit(on_dialog_quit);

int on_ok_click(Widget *w)
{
	Master::MessageDialog(w->getContext(), "Messaggio", "Ciao, Mondo!" );

	xgui::Window * dialog = xgui::Master::CreateWindow(w->getContext());
	xgui::VBox * box = xgui::Master::CreateVBox(dialog, 0, 0);
	xgui::Calendar * cal = xgui::Master::CreateCalendar(box);
	xgui::Button * btn = xgui::Master::CreateButton(box, "APPLICA");

	btn->linkEvent("onclick", &cb_on_dialog_quit);

	dialog->show();
	dialog->modalRun();

	xgui::Master::DestroyWidget(dialog);

	xgui::Master::RegisterIdleCallback(&cb_on_idle, w);

	return EVT_PROPAGATE;
} 
CppFCallback cb_on_ok_click(on_ok_click);


int on_btn_pbar_click(Widget *w)
{
	Container * ctx = (Container *)w->getContext();
	Progressbar * pbar = (Progressbar *)ctx->findChild ( "pbar" );

	pbar->setPos(pbar->getPos() + 1);

	return EVT_PROPAGATE;
} 
CppFCallback cb_on_btn_pbar_click(on_btn_pbar_click);

int on_text_submit(Widget *w, std::string const &text)
{
	List * ls = (List*)w->getContext()->findChild("list");

	if (ls) {
		Model * m = ls->getModel();
		if (m) {
			std::vector<std::string> vec;
			vec.push_back(text);
			vec.push_back("xxx");
			m->appendChild( vec, "icon" );
		}
	}
	return EVT_PROPAGATE;
}
CppFTextCallback cb_on_text_submit(on_text_submit);

int on_rm_click(Widget *w)
{
	Entry * e = (Entry*)w->getContext()->findChild("entry");
	List * ls = (List*)w->getContext()->findChild("list");

	if (ls && e) {
		Model * m = ls->getModel();
		if (m) {
			std::string entry_text;
			e->get("text", entry_text);
			int rm_pos = xgui::semantic_cast<int>(entry_text);
			m->removeChild( rm_pos );
		}
	}
	return EVT_PROPAGATE;
}
CppFCallback cb_on_rm_click(on_rm_click);


int on_toolbar_btn(Widget *w, std::string const &name, int state)
{
	std::cout << "Evento toolbar : " << name << " - " << state << std::endl;
	return EVT_PROPAGATE;
}
CppFTextStatusCallback cb_on_toolbar_btn(on_toolbar_btn);

int on_key_down(Widget *w, std::string const &name)
{
	std::cout << "KeyDown : " << name << std::endl;
	return EVT_PROPAGATE;
}
CppFTextCallback cb_on_key_down(on_key_down);

int on_key_up(Widget *w, std::string const &name)
{
	std::cout << "KeyUp : " << name << std::endl;
	return EVT_PROPAGATE;
}
CppFTextCallback cb_on_key_up(on_key_up);

int on_key_press(Widget *w, std::string const &name)
{
	std::cout << "KeyPress : " << name << std::endl;
	return EVT_PROPAGATE;
}
CppFTextCallback cb_on_key_press(on_key_press);


int on_move(Widget *w, std::string const &pos)
{
	std::cout << "Move : " << pos << std::endl;
	return EVT_PROPAGATE;
}
CppFTextCallback cb_on_move(on_move);

int on_resize(Widget *w, std::string const &size)
{
	std::cout << "Resize : " << size << std::endl;
	return EVT_PROPAGATE;
}
CppFTextCallback cb_on_resize(on_resize);

int on_mout(Widget *w)
{
	std::cout << "Mouse Out: " << w->className() << std::endl;
	return EVT_PROPAGATE;
}
CppFCallback cb_on_mout(on_mout);

int on_mover(Widget *w)
{
	std::cout << "Mouse Over: " << w->className() << std::endl;
	return EVT_PROPAGATE;
}
CppFCallback cb_on_mover(on_mover);

int on_compare(Widget *w, std::string const &a, std::string const &b)
{
	std::cout << "Str1: " << a << " Str2: " << b << std::endl;
	return a.compare(b);	
}
CppFTextPairCallback cb_on_compare(on_compare);

int on_mclick(Widget *w, const std::string &button, int status)
{
	Window * win = dynamic_cast<xgui::Window*>(w->getContext());
	Menu * m = win->getMenu();
	m->popupMenu("Text_mnu");
	return EVT_PROPAGATE;
}
CppFTextStatusCallback cb_on_mclick(on_mclick);

int on_tree_select(Widget * w, const std::string &item_path, int status)
{
	Tree * t = dynamic_cast<xgui::Tree*>(w);
	Model * model_root = t->getModel();

	Model * item = model_root->findByPath(item_path);
	std::cout << "Cambio Selezione elemento " << item->getString(0) << " : " << status << std::endl;

	return EVT_PROPAGATE;
}
CppFTextStatusCallback cb_on_tree_select(on_tree_select);

int on_tree_expand(Widget * w, const std::string &item_path, int status)
{
	Tree * t = dynamic_cast<xgui::Tree*>(w);
	Model * model_root = t->getModel();

	Model * item = model_root->findByPath(item_path);
	std::cout << "Espando elemento " << item->getString(0) << " : " << status << std::endl;

	return EVT_PROPAGATE;
}
CppFTextStatusCallback cb_on_tree_expand(on_tree_expand);

int main()
{
	StyleManager *sm = xgui::Master::CreateStyleManager();
	sm->addStyle ( "icon", Master::LoadImage("icon.png"), "", "", 0 );
	sm->addStyle ( "icon-selected", Master::LoadImage("icon_sel.png"), "", "", 0 );
	sm->addStyle ( "red-blue-bold", Master::NullImage(), "#FF0000", "#0000FF", StyleManager::TxtBold );

	Model * model = Master::CreateModel();
	std::vector<std::string> elem;
	elem.push_back("c21");
	elem.push_back("c22");
	model->appendChild(elem, "icon");
	elem.clear();
	elem.push_back("c11");
	elem.push_back("c12");
	model->appendChild(elem, "red-blue-bold");

	elem.clear();
	elem.push_back("c21-1");
	model->getChild(0)->appendChild(elem);
	elem.clear();
	elem.push_back("c21-2");
	model->getChild(0)->appendChild(elem);

	elem.clear();
	elem.push_back("c22-2");
	model->getChild(1)->appendChild(elem);
	elem.clear();
	elem.push_back("c22-2");
	model->getChild(1)->appendChild(elem);

	Image * icon = Master::LoadImage("icon.png");
	Window * w = Master::CreateWindow();
		VBox * vb = Master::CreateVBox(w, 0, 0);
			Toolbar * tb = Master::CreateToolbar(vb);
			HBox * hb = Master::CreateHBox(vb, 0, 0);
				Label * lbl = Master::CreateLabel(hb, "Password:");
				Entry * e = Master::CreateEntry(hb, "ciao", 0, true, true);
				Button * ok_btn = Master::CreateButton(hb, "OK", Master::NullImage(), true);
			HBox * hbi = Master::CreateHBox(vb, 10, true);
				Space * sp1 = Master::CreateSpace(hbi);
				ImageView * imv = Master::CreateImageView(hbi, Master::LoadImage("big.jpg"));
				Space * sp2 = Master::CreateSpace(hbi);
			Button * b = Master::CreateButton(vb, "ciao mondo", Master::NullImage());
			Checkbox * chk = Master::CreateCheckbox(vb, "test checkbox");
			Frame * frm = Master::CreateFrame ( vb, "Frame di prova" );
				HBox * hb3 = Master::CreateHBox(frm, 10);
					Progressbar * pbar = Master::CreateProgressbar(hb3);
					Button * btn_pbar = Master::CreateButton(hb3, "Step!");
					Slider * sld = Master::CreateSlider(hb3);
					VBox * vb3 = Master::CreateVBox(hb3);
						Spin * spin = Master::CreateSpin(vb3);
			Combobox * cbox = Master::CreateCombobox(vb);	
			HBox * hbt = Master::CreateHBox(vb);
				Tree * ts = Master::CreateTree(hbt, sm);
				List * ls = Master::CreateList(hbt, sm);
		Menu * mnu = Master::CreateMenu(w, true);
		Tab * tab = Master::CreateTab(vb);
			Frame * frm2 = Master::CreateFrame(tab, "frame tab");
			frm2->set("id", "TabPage1");
				VBox * frm2_vb = Master::CreateVBox(frm2, 0, 0);
				Button * tab_btn = Master::CreateButton ( frm2_vb, "prova" );
					tab_btn->linkEvent("onclick", &cb_on_rm_click);
				ImageView * imv2 = Master::CreateImageView(frm2_vb, Master::LoadImage("image.jpg"));
			Entry * tab_entry = Master::CreateEntry ( tab, "ciao" );
				tab_entry->set("id", "TabPage2");

	imv->set("view-size", "150 100");
	imv->set("scrolled", "1");

	lbl->set("text", "ciao");

	ls->set("id", "list");
	e->set("id", "entry");

	frm2->set("tab-label", "Sono la pagina 1");
	tab_entry->set("tab-label", "Sono la pagina 2");

	tab->linkEvent("onselect", &cb_on_toolbar_btn);

	e->set("expand", "1.0");
	ls->set("expand", "1.0");
	ts->set("expand", "1.0");
	b->set("expand", "1.0");
	hbt->set("expand", "1.0");

	imv->set("expand", "1.0");
	sp1->set("expand", "1.0");
	sp2->set("expand", "1.0");

	pbar->set("expand", "1.0");
	sld->set("expand", "1.0");

	cbox->setModel(model);
	
	w->set("text", "Prova XGUI");

	mnu->addItem("", "File_mnu", "&File", Master::NullImage(), "");
		mnu->addItem("File_mnu", "New_mnu", "New", icon);
		mnu->addItem("File_mnu", "Open_mnu", "Open");
		mnu->addItem("File_mnu", "Save_mnu", "Save");
	mnu->addItem("", "Edit_mnu", "&Edit");
		mnu->addItem("Edit_mnu", "Text_mnu", "Text");
			mnu->addItem("Text_mnu", "Cut_mnu", "Cut", Master::NullImage(), "check");
			mnu->addItem("Text_mnu", "", "", Master::NullImage(), "radio_begin");
				mnu->addItem("Text_mnu", "Copy_mnu", "Copy");
				mnu->addItem("Text_mnu", "Paste_mnu", "Paste");
			mnu->addItem("Text_mnu", "", "", Master::NullImage(), "radio_end");

	ls->set("id", "list");
	ls->setModel(model);
	ls->set("has-icons", "1");
	ls->set("has-checkboxes", "1");

	ts->setModel(model);
	ts->set("has-icons", "1");
	ts->set("has-checkboxes", "1");

 	ls->addColumn ( 0, "0", "c0", 100 );
	ls->addColumn ( 1, "1", "c1", 100, "right" );

	tb->set ( "style", "icons" );
	tb->addItem ( "btn1", "Salve! Sono un bottone!", icon, "ed io sono il suo tooltip" );
	tb->addItem ( "", "", Master::NullImage(), "", "separator" );
	tb->addItem ( "btn2", "Un ToggleButton!", icon, "Un ToolTip!", "check" );
	tb->addItem ( "", "", Master::NullImage(), "", "separator" );

	tb->addItem ( "", "", Master::NullImage(), "", "radio_begin" );
	tb->addItem ( "rbtn1", "RadioButton 1", icon, "Tooltip del radiobutton 1", "check" );
	tb->addItem ( "rbtn2", "RadioButton 2", icon, "Tooltip del radiobutton 2", "check" );
	tb->addItem ( "rbtn3", "RadioButton 3", icon, "Tooltip del radiobutton 3", "check" );
	tb->addItem ( "", "", Master::NullImage(), "", "radio_end" );

	b->linkEvent("onclick", &cb_on_btn_click);
	pbar->set("id", "pbar");
	mnu->set("id", "mnu");

	btn_pbar->linkEvent("onclick", &cb_on_btn_pbar_click);

	ok_btn->linkEvent("onclick", &cb_on_ok_click);

	e->linkEvent("onsubmit", &cb_on_text_submit);
	e->linkEvent("onchange", 0);
	e->linkEvent("onmouseover", &cb_on_mover);
	e->linkEvent("onmouseout", &cb_on_mout);
	e->linkEvent("onfocus", 0);
	e->linkEvent("onclick", 0);
	e->linkEvent("onblur", 0);
	//e->linkEvent("onmousemove", 0);

	w->linkEvent("onkeypress", &cb_on_key_press);
	w->linkEvent("onkeydown", &cb_on_key_down);
	w->linkEvent("onkeyup", &cb_on_key_up);

	tb->linkEvent("onclick", &cb_on_toolbar_btn);
	imv->linkEvent ( "onmousedown", &cb_on_img_mousedown );
	sld->linkEvent("onchange", &cb_on_slider_changepos);
	mnu->linkEvent("onselect", &cb_on_menu_select);
	tb->linkEvent("onmouseclick", &cb_on_mclick);

	ts->linkEvent("onselect", &cb_on_tree_select);
	ts->linkEvent("onexpand", &cb_on_tree_expand);

	w->setIcon(icon);
	w->linkEvent("onclose", &cb_on_window_close);
	w->show();
	w->set("size", "640 480");

	cbox->set("editable", "1");
	cbox->linkEvent("onchange", 0);

	w->set("position", "center");
	w->set("status-text", "Prova Status Bar");

	Master::Run();

	return 0;
}


int main3()
{
	Window * w = Master::CreateWindow();
	VBox * vb = Master::CreateVBox(w, 0, 0);
	Toolbar * tb = Master::CreateToolbar(vb);

	tb->addItem ( "tb1", "Test", Master::NullImage(), "", "" );

	w->linkEvent("onclose", &cb_on_window_close);
	w->show();

	Master::Run();

	return 0;
}
