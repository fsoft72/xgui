#include "../../xgui/xgui.h"
#include <iostream>
#include <fstream>

using namespace xgui;

class ClassesFrame : public Frame
{
	private:
		TextStatusCallback * cb_on_class_selection;
		Callback * cb_on_class_create;

		VBox * box;
		Model * classes;
		List * class_list;
		
		VBox * creation_box;
		VBox * init_props_box;
		Button * creation_btn;

		ClassInfo * selected_class;

	public:
		ClassesFrame(Container * parent) : Frame(parent, "Objects Creator"), selected_class(0)
		{
			cb_on_class_selection = new CppMTextStatusCallback<ClassesFrame>(this, &ClassesFrame::onClassSelection);
			cb_on_class_create = new CppMCallback<ClassesFrame>(this, &ClassesFrame::onClassCreate);

			classes = Master::CreateModel();

			box = Master::CreateVBox(this, 0, 0);
			class_list = Master::CreateList(box, 0);
			
			creation_box = Master::CreateVBox(box, 0, 0);
			init_props_box = Master::CreateVBox(creation_box, 0, 0);
			creation_btn = Master::CreateButton(creation_box, "Create");
		
			set("expand", "1.0");
			class_list->set("expand", "1.0");
			
 			class_list->addColumn(0, "c0", "Class", 150);

			update();

			class_list->setModel(classes);

			class_list->linkEvent("onselect", cb_on_class_selection);
			creation_btn->linkEvent("onclick", cb_on_class_create);
		}

		virtual ~ClassesFrame() 
		{
			delete cb_on_class_selection;
			delete cb_on_class_create;
		}

		void update();

		void updateInitProperties(std::string const &class_name)
		{
			selected_class = Master::FindClass(class_name);

			const std::map<std::string, bool> & init_props = selected_class->initProperties();
			for(std::map<std::string, bool>::const_iterator prop = init_props.begin(); prop != init_props.end(); ++prop) {

				HBox * p_box = Master::CreateHBox(init_props_box, 0, 0);
				Label * p_label = Master::CreateLabel(p_box, prop->first);
				Entry * p_entry = Master::CreateEntry(p_box, "");

				p_entry->set("expand", "1.0");

				p_entry->set("visible", "1");
				p_label->set("visible", "1");
				p_box->set("visible", "1");
			}
		}

		int onClassSelection(Widget * w, std::string const &id, int state)
		{
			if (state) {
				Model * selection = dynamic_cast<Model*>(classes->findByPath(id));
				updateInitProperties(selection->getString(0));
			}
			else {
				selected_class = 0;
				while(init_props_box->size()) {
					Widget * w = init_props_box->getChild(0);
					Master::DestroyWidget(w);
				}
			}
			return EVT_PROPAGATE;
		}

		int onClassCreate(Widget * w);
};

class ObjectsSelector : public Window
{
	private:
		Model * objects;
		Model * selected_object;

		VBox * box;
		Tree * tree;
		Button * sel_btn;

		Callback * on_ok_btn;
		TextStatusCallback * on_sel_obj;

	public:
		ObjectsSelector(Window * parent, Model * objs) : Window(parent), objects(objs), selected_object(0)
		{
			on_ok_btn = new CppMCallback<ObjectsSelector>(this, &ObjectsSelector::onOkBtn);
			on_sel_obj = new CppMTextStatusCallback<ObjectsSelector>(this, &ObjectsSelector::onSelectObject);

			box = Master::CreateVBox(this, 0, 0);
			tree = Master::CreateTree(box);
			sel_btn = Master::CreateButton(box, "Confirm");

			tree->set("expand", "1.0");
			tree->setModel(objects);

			tree->linkEvent("onselect", on_sel_obj);
			sel_btn->linkEvent("onclick", on_ok_btn);
		}

		~ObjectsSelector() 
		{
			delete on_ok_btn;
			delete on_sel_obj;
		}

		Object * select()
		{
			Object * o = 0;

			set("text", "Objects Marshaller");
			set("size", "640 480");
			set("position", "center");

			show();
			if ( (modalRun() == 100) && (selected_object) )
				o = Master::FindInPool(selected_object->getString(1));
			
			Master::DestroyWidget(this);
			return o;
		}

		int onOkBtn(Widget * w)
		{
			modalReturn(100);
			return EVT_PROPAGATE; 
		}

		int onSelectObject(Widget * w, std::string const &id, int state)
		{
			if(state)
				selected_object = dynamic_cast<Model*>(objects->findByPath(id));
			return EVT_PROPAGATE; 
		}
};

class EventsLinker : public Window
{
	private:
		Model * events;
		Object * object;

		VBox * box;
		List * list;

		TextStatusCallback * on_sel_evt;

		static std::map<std::string, Callback*> events_map;

		struct DebugCallback : xgui::TextStatusCallback, xgui::TextCallback
		{
			using TextStatusCallback::call;

			std::string event_name;

			DebugCallback(std::string const &evt_name) : event_name(evt_name) {}

			int call(Widget *w) {
				std::cout << "*** EVENT " << event_name << " ON " << w->className() << ":" << w->id() << std::endl;
				return EVT_PROPAGATE;
			}

			int call(Widget *w, std::string const &text) {
				std::cout << "*** EVENT " << event_name << " ON " << w->className() << ":" << w->id() << "(text: " << text << " ) " << std::endl;
				return EVT_PROPAGATE;
			}

			int call(Widget *w, std::string const &text, int status) {
				std::cout << "*** EVENT " << event_name << " ON " << w->className() << ":" << w->id() << "(text: " << text << " status: " << status << ")" << std::endl;
				return EVT_PROPAGATE;
			}
		};

	public:
		EventsLinker(Window * parent, Object * obj) : Window(parent), object(obj)
		{
			on_sel_evt = new CppMTextStatusCallback<EventsLinker>(this, &EventsLinker::onCheckEvent);

			events = Master::CreateModel();

			box = Master::CreateVBox(this, 0, 0);
			list = Master::CreateList(box);

			list->addColumn(0, "id0", "Eventi");
			list->set("expand", "1.0");
			list->set("has-checkboxes", "1");

			populateEventsMap();
			list->setModel(events);
			populateEventsList();

			list->linkEvent("oncheck", on_sel_evt);
		}

		~EventsLinker() 
		{
			delete on_sel_evt;
		}

		void run()
		{
			set("text", "Events Linker");
			set("size", "640 480");
			set("position", "center");

			show();
			modalRun();
			Master::DestroyWidget(this);
		}

		void populateEventsMap()
		{
			if (events_map.empty()) {
				events_map["onmouseup"] = new DebugCallback("onmouseup");
				events_map["onmousedown"] = new DebugCallback("onmousedown");
				events_map["onmousedblclick"] = new DebugCallback("onmousedblclick");
				events_map["onmouseclick"] = new DebugCallback("onmouseclick");

				events_map["onkeypress"] = new DebugCallback("onkeypress");
				events_map["onkeydown"] = new DebugCallback("onkeydown");
				events_map["onkeyup"] = new DebugCallback("onkeyup");
				events_map["onmove"] = new DebugCallback("onmove");
				events_map["onresize"] = new DebugCallback("onresize");
				events_map["onmousemove"] = new DebugCallback("onmousemove");
		
				events_map["onfocus"] = new DebugCallback("onfocus");
				events_map["onblur"] = new DebugCallback("onblur");
				events_map["onmouseover"] = new DebugCallback("onmouseover");
				events_map["onmouseout"] = new DebugCallback("onmouseout");
				events_map["onshow"] = new DebugCallback("onshow");
				events_map["onhide"] = new DebugCallback("onhide");
				events_map["onenable"] = new DebugCallback("onenable");
				events_map["ondisable"] = new DebugCallback("ondisable");
				events_map["ondestroy"] = new DebugCallback("ondestroy");

				events_map["onclick"] = new DebugCallback("onclick");

				events_map["onselect"] = new DebugCallback("onselect");
				events_map["onsubmit"] = new DebugCallback("onsubmit");
				events_map["onchange"] = new DebugCallback("onchange");
				events_map["onrefresh"] = new DebugCallback("onrefresh");

				events_map["onheaderclick"] = new DebugCallback("onheaderclick");
				events_map["oncheck"] = new DebugCallback("oncheck");
				events_map["onexpand"] = new DebugCallback("onexpand");
				
				events_map["onclose"] = new DebugCallback("onclose");
			}
		}

		void populateEventsList()
		{
			Widget * w = dynamic_cast<Widget*>(object);
			events->removeAllChildren();

			if (w) {
				events->appendChild("onfocus" );
				if (w->hasEvent("onfocus")) list->check(events, events->size()-1, true);
				events->appendChild("onblur" );
				if (w->hasEvent("onblur")) list->check(events, events->size()-1, true);
				events->appendChild("onmouseover" );
				if (w->hasEvent("onmouseover")) list->check(events, events->size()-1, true);
				events->appendChild("onmouseout" );
				if (w->hasEvent("onmouseout")) list->check(events, events->size()-1, true);
				events->appendChild("onmousemove" );
				if (w->hasEvent("onmousemove")) list->check(events, events->size()-1, true);
				events->appendChild("onmousedown" );
				if (w->hasEvent("onmousedown")) list->check(events, events->size()-1, true);
				events->appendChild("onmouseup" );
				if (w->hasEvent("onmouseup")) list->check(events, events->size()-1, true);
				events->appendChild("onshow" );
				if (w->hasEvent("onshow")) list->check(events, events->size()-1, true);
				events->appendChild("onhide" );
				if (w->hasEvent("onhide")) list->check(events, events->size()-1, true);
				events->appendChild("onenable");
				if (w->hasEvent("onenable")) list->check(events, events->size()-1, true);
				events->appendChild("ondisable");
				if (w->hasEvent("ondisable")) list->check(events, events->size()-1, true);
				events->appendChild("onresize" );
				if (w->hasEvent("onresize")) list->check(events, events->size()-1, true);
				events->appendChild("onmove" );
				if (w->hasEvent("onmove")) list->check(events, events->size()-1, true);
				events->appendChild("onkeydown" );	
				if (w->hasEvent("onkeydown")) list->check(events, events->size()-1, true);
				events->appendChild("onkeyup" );
				if (w->hasEvent("onkeyup")) list->check(events, events->size()-1, true);
				events->appendChild("onkeypress" );	
				if (w->hasEvent("onkeypress")) list->check(events, events->size()-1, true);
				events->appendChild("onmouseclick" );	
				if (w->hasEvent("onmouseclick")) list->check(events, events->size()-1, true);
				events->appendChild("onmousedblclick" );
				if (w->hasEvent("onmousedblclick")) list->check(events, events->size()-1, true);
				events->appendChild("ondestroy" );
				if (w->hasEvent("ondestroy")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Button*>(object)) {
				events->appendChild("onclick");
				if (w->hasEvent("onclick")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Checkbox*>(object)) {
				events->appendChild("onclick");
				if (w->hasEvent("onclick")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Combobox*>(object)) {
				events->appendChild("onsubmit");
				if (w->hasEvent("onsubmit")) list->check(events, events->size()-1, true);
				events->appendChild("onchange");
				if (w->hasEvent("onchange")) list->check(events, events->size()-1, true);
				events->appendChild("onselect");
				if (w->hasEvent("onselect")) list->check(events, events->size()-1, true);
				events->appendChild("onrefresh");
				if (w->hasEvent("onrefresh")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Calendar*>(object)) {
				events->appendChild("onselect");
				if (w->hasEvent("onselect")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Entry*>(object)) {
				events->appendChild("onsubmit");
				if (w->hasEvent("onsubmit")) list->check(events, events->size()-1, true);
				events->appendChild("onchange");
				if (w->hasEvent("onchange")) list->check(events, events->size()-1, true);
			}

			if(dynamic_cast<List*>(object)) {
				events->appendChild("onselect");
				if (w->hasEvent("onselect")) list->check(events, events->size()-1, true);
				events->appendChild("onrefresh");
				if (w->hasEvent("onrefresh")) list->check(events, events->size()-1, true);
				events->appendChild("onheaderclick");
				if (w->hasEvent("onheaderclick")) list->check(events, events->size()-1, true);
				events->appendChild("oncheck");
				if (w->hasEvent("oncheck")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Menu*>(object)) {
				events->appendChild("onselect");
				if (w->hasEvent("onselect")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Progressbar*>(object)) {
				events->appendChild("onchange");
				if (w->hasEvent("onchange")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Slider*>(object)) {
				events->appendChild("onchange");
				if (w->hasEvent("onchange")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Spin*>(object)) {
				events->appendChild("onsubmit");
				if (w->hasEvent("onsubmit")) list->check(events, events->size()-1, true);
				events->appendChild("onchange");
				if (w->hasEvent("onchange")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Tab*>(object)) {
				events->appendChild("onselect");
				if (w->hasEvent("onselect")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Toolbar*>(object)) {
				events->appendChild("onclick");
				if (w->hasEvent("onclick")) list->check(events, events->size()-1, true);
			}

			if(dynamic_cast<Tree*>(object)) {
				events->appendChild("onselect");
				if (w->hasEvent("onselect")) list->check(events, events->size()-1, true);
				events->appendChild("onrefresh");
				if (w->hasEvent("onrefresh")) list->check(events, events->size()-1, true);
				events->appendChild("oncheck");
				if (w->hasEvent("oncheck")) list->check(events, events->size()-1, true);
				events->appendChild("onexpand");
				if (w->hasEvent("onexpand")) list->check(events, events->size()-1, true);
			}

			if (dynamic_cast<Window*>(object)) {
				events->appendChild("onclose");
				if (w->hasEvent("onclose")) list->check(events, events->size()-1, true);
			}
		}

		int onCheckEvent(Widget * w, std::string const &id, int state)
		{
			Model * selected_event = dynamic_cast<Model*>(events->findByPath(id));

			if (selected_event) {
				std::string event_name = selected_event->getString(0);
				Widget * w = dynamic_cast<Widget*>(object);
				if(w) {
					if (state) {
						Callback * cb = events_map[event_name];
						w->linkEvent(event_name, cb);
					}
					else
						w->linkEvent(event_name, 0);
				}
			}

			return EVT_PROPAGATE; 
		}
};
std::map<std::string, Callback*> EventsLinker::events_map;

class ObjectsFrame : public Frame
{
	private:
		Model * selected_object;
		std::string selected_prop;
		std::string selected_method;
		std::map<Object*, Model*> registered_windows;

		HBox * box;

		VBox * obj_box;
		HBox * obj_btn_box;
		Button * destroy_btn;
		Button * unselect_btn;
		Tree * objects_tree;
		Model * objects;
		Button * events_btn;

		VBox * set_box;
		Button * refresh_btn;
		Button * set_btn;
		Entry * prop_entry;
		Model * properties;
		List * prop_list;

		VBox * call_box;
		Model * methods;
		List * methods_list;
		HBox * rv_box;
		Label * rv_label;
		Entry * rv_entry;

		HBox * p1_box;
		Entry * p1_entry;
		Button * p1_btn;

		HBox * p2_box;
		Entry * p2_entry;
		Button * p2_btn;

		HBox * p3_box;
		Entry * p3_entry;
		Button * p3_btn;

		HBox * p4_box;
		Entry * p4_entry;
		Button * p4_btn;

		HBox * p5_box;
		Entry * p5_entry;
		Button * p5_btn;

		Button * call_btn;

		TextStatusCallback * cb_on_obj_selection;
		TextStatusCallback * cb_on_prop_selection;
		TextStatusCallback * cb_on_method_selection;
		Callback * cb_on_set_prop;
		Callback * cb_on_call_method;
		Callback * cb_on_unselect_obj;
		Callback * cb_on_destroy_obj;
		Callback * cb_on_refresh_props;
		Callback * cb_on_events_manage;

		Callback * cb_on_marshall_obj1;
		Callback * cb_on_marshall_obj2;
		Callback * cb_on_marshall_obj3;
		Callback * cb_on_marshall_obj4;
		Callback * cb_on_marshall_obj5;

		Callback * cb_on_window_closed;

		Model * findObject(Object * o, Model * node)
		{
			Model * ret = 0;

			if(o->id() == node->getString(1))
				return node;
	
			for(unsigned int i = 0; i<node->size(); ++i) {
				if ( (ret = findObject(o, node->getChild(i))) != 0 )
					break;
			}

			return ret;
		}

		void removeFromPool(Model * node)
		{
			if(!node) return;
			
			Object * o = Master::FindInPool(node->getString(1));
			Master::RemoveFromPool(o);

			for(unsigned int i = 0; i<node->size(); ++i)
				removeFromPool(node->getChild(i));
		}

	public:
		ObjectsFrame(Container * parent) : Frame(parent, "Objects Manager"), selected_object(0)
		{
			cb_on_obj_selection = new CppMTextStatusCallback<ObjectsFrame>(this, &ObjectsFrame::onObjSelection);
			cb_on_prop_selection = new CppMTextStatusCallback<ObjectsFrame>(this, &ObjectsFrame::onPropSelection);
			cb_on_method_selection = new CppMTextStatusCallback<ObjectsFrame>(this, &ObjectsFrame::onMethodSelection);

			cb_on_set_prop = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onSetProp);
			cb_on_call_method = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onCallMethod);
			cb_on_unselect_obj = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onUnselectObj);
			cb_on_destroy_obj = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onDestroyObj);
			cb_on_refresh_props = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::updateProperties);
			cb_on_events_manage = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onManageEvents);

			cb_on_marshall_obj1 = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onMarshallP1);
			cb_on_marshall_obj2 = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onMarshallP2);
			cb_on_marshall_obj3 = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onMarshallP3);
			cb_on_marshall_obj4 = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onMarshallP4);
			cb_on_marshall_obj5 = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onMarshallP5);

			cb_on_window_closed = new CppMCallback<ObjectsFrame>(this, &ObjectsFrame::onWindowClosed);

			box = Master::CreateHBox(this, 0, 0);

			obj_box = Master::CreateVBox(box, 0, 0);
			obj_btn_box = Master::CreateHBox(obj_box, 0, 0);
			destroy_btn = Master::CreateButton(obj_btn_box, "Destroy");
			unselect_btn = Master::CreateButton(obj_btn_box, "Unselect");
			objects_tree = Master::CreateTree(obj_box);
			events_btn = Master::CreateButton(obj_box, "Test Events");

			set_box = Master::CreateVBox(box, 0, 0);
			refresh_btn = Master::CreateButton(set_box, "Refresh");
			prop_list = Master::CreateList(set_box);
			prop_entry = Master::CreateEntry(set_box, "");
			prop_entry->set("enabled", "0");
			set_btn = Master::CreateButton(set_box, "Set");
			set_btn->set("enabled", "0");

			call_box = Master::CreateVBox(box, 0, 0);
			methods_list = Master::CreateList(call_box);
			
			rv_box = Master::CreateHBox(call_box, 0, 0);
			rv_label = Master::CreateLabel(rv_box, "Result:");
			rv_entry = Master::CreateEntry(rv_box, "");
			rv_entry->set("expand", "1.0");
			rv_entry->set("editable", "0");

			p1_box = Master::CreateHBox(call_box, 0, 0);
			p1_entry = Master::CreateEntry(p1_box, "");
			p1_btn = Master::CreateButton(p1_box, "Object");
			p1_btn->linkEvent("onclick", cb_on_marshall_obj1);
			p1_entry->set("expand", "1.0");
			p1_entry->set("enabled", "0");
			
			p2_box = Master::CreateHBox(call_box, 0, 0);
			p2_entry = Master::CreateEntry(p2_box, "");
			p2_btn = Master::CreateButton(p2_box, "Object");
			p2_btn->linkEvent("onclick", cb_on_marshall_obj2);
			p2_entry->set("expand", "1.0");
			p2_entry->set("enabled", "0");
			
			p3_box = Master::CreateHBox(call_box, 0, 0);
			p3_entry = Master::CreateEntry(p3_box, "");
			p3_btn = Master::CreateButton(p3_box, "Object");
			p3_btn->linkEvent("onclick", cb_on_marshall_obj3);
			p3_entry->set("expand", "1.0");
			p3_entry->set("enabled", "0");

			p4_box = Master::CreateHBox(call_box, 0, 0);
			p4_entry = Master::CreateEntry(p4_box, "");
			p4_btn = Master::CreateButton(p4_box, "Object");
			p4_btn->linkEvent("onclick", cb_on_marshall_obj4);
			p4_entry->set("expand", "1.0");
			p4_entry->set("enabled", "0");

			p5_box = Master::CreateHBox(call_box, 0, 0);
			p5_entry = Master::CreateEntry(p5_box, "");
			p5_btn = Master::CreateButton(p5_box, "Object");
			p5_btn->linkEvent("onclick", cb_on_marshall_obj5);
			p5_entry->set("expand", "1.0");
			p5_entry->set("enabled", "0");

			call_btn = Master::CreateButton(call_box, "Call");
			call_btn->set("enabled", "0");
			
			objects = Master::CreateModel();
			properties = Master::CreateModel();
			methods = Master::CreateModel();

			prop_list->addColumn(0, "c0", "Property");
			prop_list->addColumn(1, "c1", "Value");

			methods_list->addColumn(0, "c0", "Method");
			methods_list->addColumn(1, "c1", "Number Of Parameters");

			set("expand", "1.0");
			obj_box->set("expand", "1.0");
			set_box->set("expand", "1.0");
			call_box->set("expand", "1.0");
			destroy_btn->set("expand", "1.0");
			unselect_btn->set("expand", "1.0");
			objects_tree->set("expand", "1.0");
			prop_list->set("expand", "1.0");
			methods_list->set("expand", "1.0");
			
			objects_tree->setModel(objects);
			prop_list->setModel(properties);
			methods_list->setModel(methods);

			objects_tree->linkEvent("onselect", cb_on_obj_selection);
			prop_list->linkEvent("onselect", cb_on_prop_selection);
			methods_list->linkEvent("onselect", cb_on_method_selection);
			
			set_btn->linkEvent("onclick", cb_on_set_prop);
			call_btn->linkEvent("onclick", cb_on_call_method);
			unselect_btn->linkEvent("onclick", cb_on_unselect_obj);
			destroy_btn->linkEvent("onclick", cb_on_destroy_obj);
			refresh_btn->linkEvent("onclick", cb_on_refresh_props);
			events_btn->linkEvent("onclick", cb_on_events_manage);
		}

		~ObjectsFrame() 
		{
			delete cb_on_obj_selection;
			delete cb_on_prop_selection;
			delete cb_on_method_selection;
			delete cb_on_set_prop;
			delete cb_on_call_method;
			delete cb_on_unselect_obj;
			delete cb_on_destroy_obj;
			delete cb_on_refresh_props;
			delete cb_on_events_manage;
	
			delete cb_on_marshall_obj1;
			delete cb_on_marshall_obj2;
			delete cb_on_marshall_obj3;
			delete cb_on_marshall_obj4;
			delete cb_on_marshall_obj5;
		}

		void addObject(Object * obj)
		{
			Model * old_selected = selected_object;
			Model * root = selected_object ? selected_object : objects;

			std::string entry_text = obj->className();
			entry_text += " : ";
			entry_text += obj->id();

			Model * child = Master::CreateModel();
			child->addString(entry_text);
			child->addString(obj->id());

			root->appendChild(child);
			Master::AddToPool(obj);

			Window * w = dynamic_cast<Window*>(obj);
			if (w) {
				registered_windows[obj] = child;
				w->linkEvent("onclose", cb_on_window_closed);
			}

			Container * cont = dynamic_cast<Container*>(obj);
			if(cont) {
				selected_object = child;
				for(unsigned int i = 0; i < cont->size(); ++i)
					addObject(cont->getChild(i));
			}

			Model * model = dynamic_cast<Model*>(obj);
			if(model) {
				model->ref();
				selected_object = child;
				for(unsigned int i = 0; i < model->size(); ++i)
					addObject(model->getChild(i));
			}

			StyleManager * sm = dynamic_cast<StyleManager*>(obj);
			if (sm)
				sm->ref();

			Image * img = dynamic_cast<Image*>(obj);
			if(img)
				img->ref();

			selected_object = old_selected;
		}

		Object * getSelected()
		{
			if(selected_object)
				return Master::FindInPool(selected_object->getString(1));
			else
				return 0;
		}

		int updateProperties(Widget * w = 0)
		{
			properties->removeAllChildren();
			selected_prop = "";

			Object * obj = getSelected();
			if (!obj) return EVT_PROPAGATE;

			std::vector<std::string> props = obj->properties();
			for(std::vector<std::string>::iterator p = props.begin(); p != props.end(); ++p) {
				std::string value;
				std::vector<std::string> entry;

				obj->get(*p, value);

				entry.push_back(*p);
				entry.push_back(value);

				properties->appendChild(entry);
			}

			Model * m = dynamic_cast<Model*>(obj);
			if (m) {
				for(unsigned int i = 0; i<m->numberOfStrings(); ++i) {
					std::vector<std::string> entry;

					entry.push_back(semantic_cast<std::string>(i));
					entry.push_back(m->getString(i));

					properties->appendChild(entry);
				}
			}

			return EVT_PROPAGATE;	
		}

		void updateMethods()
		{
			methods->removeAllChildren();
			selected_method = "";

			Object * obj = getSelected();
			if (!obj) return;

			std::vector<std::string> mv = obj->methods();
			for(std::vector<std::string>::iterator m = mv.begin(); m != mv.end(); ++m) {
				std::string nparams;
				std::vector<std::string> entry;

				DMethod * method = obj->findMethod(*m);
				nparams = semantic_cast<std::string>(method->numberOfParameters());

				entry.push_back(*m);
				entry.push_back(nparams);

				methods->appendChild(entry);
			}
		}

		int writeObjectMarshall(Entry * e)
		{
			ObjectsSelector * sel = new ObjectsSelector(getContext(), objects);
			std::string marshall = "";
			Object * o = sel->select();
			
			if (o)
				marshall = o->pack();
			
			e->set("text", marshall);

			return EVT_PROPAGATE;
		}

		int onWindowClosed(Widget * w)
		{
			return EVT_BLOCK;
		}

		int onFinalizeObj(Widget * w)
		{
			Object * o = getSelected();
			if (!o) return EVT_PROPAGATE;

			o->getClass()->finalize(o);

			return EVT_PROPAGATE;
		}

		int onManageEvents(Widget * w)
		{
			EventsLinker * linker = new EventsLinker(getContext(), getSelected());
			linker->run();
			return EVT_PROPAGATE;
		}

		int onObjSelection(Widget * w, std::string const &id, int state);

		int onPropSelection(Widget * w, std::string const &id, int state)
		{
			if(state) {
				Object * o = getSelected();
				std::string prop_value;

				selected_prop = dynamic_cast<Model*>(properties->findByPath(id))->getString(0);
				o->get(selected_prop, prop_value);
				prop_entry->set("text", prop_value);
				prop_entry->set("enabled", "1");
				set_btn->set("enabled", "1");
			}

			return EVT_PROPAGATE;
		}

		int onMethodSelection(Widget * w, std::string const &id, int state)
		{
			if(state) {
				Object * o = getSelected();
				selected_method = dynamic_cast<Model*>(methods->findByPath(id))->getString(0);
				DMethod * m = o->findMethod(selected_method);

				call_btn->set("enabled", "1");
				unsigned int nparam = m->numberOfParameters();
				p1_entry->set("enabled", "0");
				p2_entry->set("enabled", "0");
				p3_entry->set("enabled", "0");
				p4_entry->set("enabled", "0");
				p5_entry->set("enabled", "0");
				
				if (nparam >= 1)
					p1_entry->set("enabled", "1");
				if (nparam >= 2)
					p2_entry->set("enabled", "1");
				if (nparam >= 3)
					p3_entry->set("enabled", "1");
				if (nparam >= 4)
					p4_entry->set("enabled", "1");
				if (nparam >= 5)
					p5_entry->set("enabled", "1");
			}
			return EVT_PROPAGATE;
		}

		int onSetProp(Widget * w)
		{
			Object * o = getSelected();

			std::string prop_value;
			prop_entry->get("text", prop_value);

			o->set(selected_prop, prop_value);

			if (selected_prop == "id") {
				std::string name = o->className();
				name += " : ";
				name += o->id();

				selected_object->setString(name, 0);
				selected_object->setString(prop_value, 1);

				objects_tree->refresh();
			}

			prop_entry->set("text", "");
			prop_entry->set("enabled", "0");
			set_btn->set("enabled", "0");

			updateProperties();

			return EVT_PROPAGATE;
		}

		int onUnselectObj(Widget * w);

		int onDestroyObj(Widget * w)
		{
			if (!selected_object) return EVT_PROPAGATE;
			Object * o = getSelected();

			removeFromPool(selected_object);

			Model * parent = dynamic_cast<Model*>(selected_object->getParent());
			int pos = parent->findChildPos(selected_object);
			parent->removeChild(pos);

			Model * m = dynamic_cast<Model*>(o);
			StyleManager * sm = dynamic_cast<StyleManager*>(o);
			Image * img = dynamic_cast<Image*>(o);

			if(img)
				img->ref();
			else if (m)
				m->unref();
			else if (sm)
				sm->unref();
			else
				Master::DestroyObject(o);

			onUnselectObj(0);

			return EVT_PROPAGATE;
		}

		int onCallMethod(Widget * w)
		{
			Object * o = getSelected();
			DMethod * method = o->findMethod(selected_method);
			unsigned int nparams = method->numberOfParameters();

			std::string rv, p1, p2, p3, p4, p5;
			p1_entry->get("text", p1);
			p2_entry->get("text", p2);
			p3_entry->get("text", p3);
			p4_entry->get("text", p4);
			p5_entry->get("text", p5);

			if (nparams == 0)
				rv = o->call(selected_method);
			else if (nparams == 1)
				rv = o->call(selected_method, p1);
			else if (nparams == 2)
				rv = o->call(selected_method, p1, p2);
			else if (nparams == 3)
				rv = o->call(selected_method, p1, p2, p3);
			else if (nparams == 4)
				rv = o->call(selected_method, p1, p2, p3, p4);
			else if (nparams == 5)
				rv = o->call(selected_method, p1, p2, p3, p4, p5);

			p1_entry->set("text", "");
			p2_entry->set("text", "");
			p3_entry->set("text", "");
			p4_entry->set("text", "");
			p5_entry->set("text", "");
			rv_entry->set("text", rv);
			
			return EVT_PROPAGATE;
		}

		int onMarshallP1(Widget * w) { return writeObjectMarshall(p1_entry); }
		int onMarshallP2(Widget * w) { return writeObjectMarshall(p2_entry); }
		int onMarshallP3(Widget * w) { return writeObjectMarshall(p3_entry); }
		int onMarshallP4(Widget * w) { return writeObjectMarshall(p4_entry); }
		int onMarshallP5(Widget * w) { return writeObjectMarshall(p5_entry); }
};

class PlanUi : public Window
{
	private:
		Callback * cb_on_close;	
		TextStatusCallback * cb_on_menu;

		Menu * menu;
		HBox * main_box;

		ClassesFrame * classes;
		ObjectsFrame * objects;

	public:
		PlanUi() : Window(0), menu(0), main_box(0), classes(0), objects(0)
		{
			cb_on_close = new CppMCallback<PlanUi>(this, &PlanUi::onClose);
			cb_on_menu = new CppMTextStatusCallback<PlanUi>(this, &PlanUi::onMenu);

			linkEvent("onclose", cb_on_close);

			menu = Master::CreateMenu(this);
			menu->addItem("", "File_mnu", "&File");
			menu->addItem("File_mnu", "LoadXml_mnu", "&Load Xml");
			menu->addItem("File_mnu", "SaveXml_mnu", "&Save Xml");
			menu->addItem("", "Master_mnu", "&Master");
			menu->addItem("Master_mnu", "DTD_mnu", "&Generate Dtd");
			menu->addItem("Master_mnu", "Plugin_mnu", "Load &Plugin");

			main_box = Master::CreateHBox(this, 0, 0);
			classes = new ClassesFrame(main_box);
			objects = new ObjectsFrame(main_box);

			menu->linkEvent("onselect", cb_on_menu);
		}

		virtual ~PlanUi() 
		{
			delete cb_on_close;
			delete cb_on_menu;
		}
	
		void run()
		{
			set("size", "1024 600");
			set("position", "center");

			Window::show();

			set("text", "XGUI Plan");

			Master::Run();
		}
	
		int onClose(Widget * w)
		{
			Master::Quit();
			return EVT_PROPAGATE;
		}

		int onMenu(Widget * menu, std::string const &id, int state)
		{
			if(id == "LoadXml_mnu") {
				FileExtensionsVector filters;
				filters.push_back(FileExtension("(.xml) Xml Gui Description", "*.xml"));
				std::vector<std::string> files = Master::OpenFileDialog(this, filters, "Open");
				if(!files.empty()) {
					Object * current_parent = objects->getSelected();
					Object * loaded_obj = Master::LoadXml(files[0], current_parent);
					if(loaded_obj)
						objects->addObject(loaded_obj);
				}
			}
			else if(id == "SaveXml_mnu") {
				FileExtensionsVector filters;
				filters.push_back(FileExtension("(.xml) Xml Gui Description", "*.xml"));
				std::string file = Master::SaveFileDialog(this, filters, "Save");

				Object * to_dump = objects->getSelected();
				if(!to_dump) return EVT_PROPAGATE;

				Master::SaveXml(to_dump, file);
			}
			else if(id == "DTD_mnu") {
				FileExtensionsVector filters;
				filters.push_back(FileExtension("(.dtd) Xml Document Type Definitions", "*.dtd"));
				std::string file = Master::SaveFileDialog(this, filters, "Save");

				if(file.empty()) return EVT_PROPAGATE;

				std::string dtd = Master::GenerateDTD();
				std::ofstream f(file.c_str());
				if(!f) return EVT_PROPAGATE;

				f << dtd;
				f.close();
			}
			else if(id == "Plugin_mnu") {
				FileExtensionsVector filters;
				filters.push_back(FileExtension("(.so) XGui Linux Plugin", "*.so"));
				filters.push_back(FileExtension("(.dll) XGui Windows Plugin", "*.dll"));

				std::vector<std::string> files = Master::OpenFileDialog(this, filters, "Open");
				if(!files.empty()) {
					Master::LoadLibrary(files[0]);
					classes->update();
				}
			}

			return EVT_PROPAGATE;
		}

		ObjectsFrame * getObjects() { return objects; }

		ClassesFrame * getClasses() { return classes; }
};


int ClassesFrame::onClassCreate(Widget * w)
{
	if(!selected_class) return EVT_PROPAGATE;

	StringMap props;

	for(unsigned int i = 0; i<init_props_box->size(); ++i) {
		std::string prop_name;
		std::string prop_value;

		HBox * prop_box = dynamic_cast<HBox*>(init_props_box->getChild(i));
		prop_box->getChild(0)->get("text", prop_name);
		prop_box->getChild(1)->get("text", prop_value);

		if (!prop_value.empty())
			props[prop_name] = prop_value;
	}

	PlanUi * ui = dynamic_cast<PlanUi*>(this->getContext());

	Object * parent = ui->getObjects()->getSelected();
	Object * o = selected_class->create(parent, props);

	if (o) {
		Window * win = dynamic_cast<Window*>(o);
		if (win)
			win->show();	
		else
			o->set("visible", "1");
		ui->getObjects()->addObject(o);
	}

	return EVT_PROPAGATE;
}

void ClassesFrame::update()
{
	PlanUi * ui = dynamic_cast<PlanUi*>(this->getContext());
	ObjectsFrame * objs = ui->getObjects();

	Object * o = objs ? objs->getSelected() : 0;
	ClassInfo * parent_cinfo = o ? o->getClass() : 0;

	selected_class = 0;
	while(init_props_box->size()) {
		Widget * w = init_props_box->getChild(0);
		Master::DestroyWidget(w);
	}

	classes->removeAllChildren();

	std::vector<std::string> available_classes = Master::availableClasses();
	for(std::vector<std::string>::iterator c = available_classes.begin(); c != available_classes.end(); ++c) {
		ClassInfo * cinfo = Master::FindClass(*c);
		if (cinfo->isInstanceable())
			if ( (!parent_cinfo) || (cinfo->canBeContainedIn(parent_cinfo)) )
				classes->appendChild(*c);
	}
}

int ObjectsFrame::onUnselectObj(Widget * w)
{
	PlanUi * ui = dynamic_cast<PlanUi*>(this->getContext());

	if (!selected_object) return EVT_PROPAGATE;

	objects_tree->selectAll(objects, false);
	selected_object = 0;

	updateProperties();
	updateMethods();
	ui->getClasses()->update();

	return EVT_PROPAGATE;
}

int ObjectsFrame::onObjSelection(Widget * w, std::string const &id, int state)
{
	PlanUi * ui = dynamic_cast<PlanUi*>(this->getContext());
	
	if(state) {
		selected_object = dynamic_cast<Model*>(objects->findByPath(id));
		updateProperties();
		updateMethods();
		ui->getClasses()->update();
	}

	return EVT_PROPAGATE;
}

int main()
{
	Master::InitDynamicClasses();
	PlanUi * ui = new PlanUi();
	ui->run();
}
