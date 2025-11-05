//
// C++ Implementation: frame
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "precomp.h"

#ifndef WIN32
#	include "frame.h"
#	include "gtk/frame_impl.h"
#	include "master.h"
#	include "dynamic.h"
#endif

static void Frame_set(xgui::Object * o, std::string const &name, std::string const &val)
{
	xgui::Frame * self = dynamic_cast<xgui::Frame*>(o);
	xguimpl::Frame * impl = dynamic_cast<xguimpl::Frame*>(self->getImpl());

	impl->set_frame_prop(name, val);
}

static void Frame_get(xgui::Object * o, std::string const &name, std::string &dest)
{
	xgui::Frame * self = dynamic_cast<xgui::Frame*>(o);
	xguimpl::Frame * impl = dynamic_cast<xguimpl::Frame*>(self->getImpl());

	impl->get_frame_prop(name, dest);
}

namespace xgui
{
	xgui::FrameClass frame_class_info;

	FrameClass::FrameClass() : ContainerClass() 
	{
		registerProperty("alignment", Frame_set, Frame_get, true);
		registerProperty("text", Frame_set, Frame_get, true);

		registerInitProperty("text", false);

		registerMethod("getRootChild", new DMethod0< Ptr< Type<xgui::Widget> >, xgui::Frame>(&xgui::Frame::getRootChild));
	}

	std::string const &FrameClass::className() { static std::string const c_name = "xgui::Frame"; return c_name; }

	bool FrameClass::isInstanceable() { return true; }

	xgui::Object * FrameClass::create(xgui::Object * parent, xgui::StringMap &properties)
	{
		xgui::StringMap::iterator prop_i;
		xgui::Container * wparent = dynamic_cast<xgui::Container*>( parent );
		xgui::Frame * w = 0;

		if (!parent) {
			XGUI_ERROR("Cannot create a Frame without parent");
			return 0;
		}

		if (!wparent) {
			XGUI_ERROR("Cannot create a Frame as a child of a " << parent->className());
			return 0;
		}

		std::string text = "";

		if ( (prop_i = properties.find("text")) != properties.end() ) {
			text = prop_i->second;
			properties.erase(prop_i);
		}

		w = xgui::Master::CreateFrame(wparent, text);

		return w;
	}
}

namespace xgui
{
	Frame::Frame(xgui::Container * parent, xgui::ClassInfo * cinfo, xguimpl::Frame * widget)
	: Container ( parent, cinfo, widget )
	{
		frame_impl_ = dynamic_cast<xguimpl::Frame *>(impl_);
	}

	Frame::Frame(xgui::Container * parent, const std::string & label)
	: Container ( parent, &frame_class_info, new xguimpl::Frame(parent, label) )
	{
		frame_impl_ = dynamic_cast<xguimpl::Frame *>(impl_);
		impl_->setAccessor(this);
		post_construction();
	}

	Frame::~Frame() {}

	void Frame::addChild(xgui::Widget * w)
	{
		frame_impl_->addChild ( w );
		Container::addChild ( w );
	}

	void Frame::delChild(xgui::Widget * w)
	{
		frame_impl_->delChild ( w );
		Container::delChild ( w );
	}

	xgui::Widget * Frame::getRootChild()
	{
		return frame_impl_->getChild();
	}
}
