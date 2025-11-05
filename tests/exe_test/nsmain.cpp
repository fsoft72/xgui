#include <iostream>
#include "../../xgui/xgui.h"

using namespace xgui;

int OnWindowClose(xgui::Widget * w)
{
	Master::Quit();
	return EVT_PROPAGATE;
}
CppFCallback cb_on_window_close(OnWindowClose);

int on_btn_click(Widget *w)
{
	std::string path;
	w->get("path", path);

	std::cout << "Distruggo widget: " << path << std::endl;
	Master::DestroyWidget(w);
	
	return EVT_PROPAGATE;
} 
CppFCallback cb_on_btn_click(on_btn_click);

int main(int argc, const char * argv[])
{
	Master::Instance();

	std::cout << "Master ready" << std::endl;

	Window * w = Master::CreateWindow();
	VBox * v_box = Master::CreateVBox(w, 0, true);
		HBox * box = Master::CreateHBox(v_box, 0, true);
			Button * b = Master::CreateButton(box, "Prova");
			Button * b2 = Master::CreateButton(box, "Prova2");
		HBox * box2 = Master::CreateHBox(v_box, 0, true);
			Button * b3 = Master::CreateButton(box2, "Prova3");
			Button * b4 = Master::CreateButton(box2, "Prova4");

	box->set("expand", "1");
	b->set("expand", "1");
	b2->set("expand", "1");
	box2->set("expand", "1");
	b3->set("expand", "1");
	b4->set("expand", "1");

	w->linkEvent("onclose", &cb_on_window_close);
	b->linkEvent("onclick", &cb_on_btn_click);
	b2->linkEvent("onclick", &cb_on_btn_click);
	b3->linkEvent("onclick", &cb_on_btn_click);
	b4->linkEvent("onclick", &cb_on_btn_click);
	w->show();

	Master::Run();

	std::cerr << "!!! Applicazione terminata !!!" << std::endl;

    return 0;
}
