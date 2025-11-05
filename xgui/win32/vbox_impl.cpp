//
// C++ Implementation: vbox_impl
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

namespace xguimpl
{
	static HWND MkVBox ( xgui::Container * parent, bool border )
	{
        return CreateWindowEx ( WS_EX_CONTROLPARENT |
			WS_EX_NOPARENTNOTIFY | (border ? WS_EX_CLIENTEDGE : 0),
			"XGUIBox", "",
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget, NULL, MAIN_HINSTANCE, NULL );

	}

	VBox::VBox ( HWND real_w ) :
		Widget ( real_w ), space(0)
	{
	}

	VBox::VBox ( xgui::Container * parent, int spc, bool border ) :
		Widget ( MkVBox ( parent, border ) ), space(spc)
	{
		SetWindowLongPtr ( widget, DWLP_DLGPROC, (LONG)(LONG_PTR)Master::dlgProc );
	}

	VBox::~VBox()
	{
	}


	bool VBox::addChild ( xgui::Widget * wid )
	{
		Widget * w = wid->getImpl();

		HWND hWid = w->widget;

		xgui::VBox * vb = dynamic_cast<xgui::VBox *>(this_widget);

		if (!vb->children_.empty()) {
			xgui::Widget * &pw = vb->children_.back();
			SetWindowPos(hWid, pw->getImpl()->widget, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		}

		w->h_resize = true;

		this_widget->getContext()->redraw();

		return true;
	}


	void VBox::delChild ( xgui::Widget * )
	{
	}


	void VBox::giveSize ( std::pair<int, int> size )
	{
		xgui::VBox * vb = dynamic_cast<xgui::VBox *>(this_widget);

		cur_x = size.first;
		cur_y = size.second;

		int w = cur_x - space * 2;
		int h = cur_y - space * 2;

		int slot_y = 0;  //somma delle dimensioni minime dei figli resizabili
		double py;

		if ( vb && !vb->children_.empty() ) {

			for ( xgui::WidgetsVector::iterator wid_iter = vb->children_.begin();
				wid_iter != vb->children_.end(); ++wid_iter )
			{
				xgui::Widget * wid = *wid_iter;
				Widget * wimpl = wid->getImpl();

				int min_y = wimpl->min_y;

				if (!wimpl->v_resize)
					h -= min_y;
				else 
					slot_y += min_y;
			}

			if ( slot_y == 0 )
				py = 1;
			else
				py = (double)h / (double)slot_y;

			int child_x = space;
			int child_y = space;

			for ( xgui::WidgetsVector::iterator wid_iter = vb->children_.begin();
				wid_iter != vb->children_.end(); ++wid_iter )
			{
				xgui::Widget * wid = *wid_iter;
				Widget * wimpl = wid->getImpl();

				int min_y = wimpl->min_y;

				if ( wimpl->v_resize )
					min_y = (int)(min_y * py);

				wimpl->giveSize ( std::make_pair(w, min_y) );
				wimpl->setPos ( std::make_pair(child_x, child_y) );

				child_y += wimpl->cur_y;
			}

		}

		SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y, SWP_NOMOVE | SWP_NOZORDER );
	}


	void VBox::recalcLayout()
	{
		xgui::VBox * vb = dynamic_cast<xgui::VBox *>(this_widget);
		if ( !vb ) {
			min_x = space * 2;
			min_y = space * 2;
			return;
		}

		min_x = min_y = 0;

		for ( xgui::WidgetsVector::iterator wid_iter = vb->children_.begin();
			wid_iter != vb->children_.end(); ++wid_iter )
		{
			xgui::Widget * child = *wid_iter;
			Widget * childimpl = child->getImpl();

			child->recalcLayout();

			min_x = max(min_x, childimpl->min_x);
			min_y += childimpl->min_y;
		}

		min_x += space * 2;
		min_y += space * 2;
	}


	void VBox::setExpand ( xgui::Widget * wid, float expand )
	{
		Widget * w = wid->getImpl();
		w->h_resize = true;
		w->v_resize = expand > 0.0;
	}
}
