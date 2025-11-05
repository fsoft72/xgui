//
// C++ Implementation: hbox_impl
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
	static HWND MkHBox ( xgui::Container * parent, bool border )
	{
        return CreateWindowEx ( WS_EX_CONTROLPARENT |
			WS_EX_NOPARENTNOTIFY | (border ? WS_EX_CLIENTEDGE : 0),
			"XGUIBox", "",
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, (HWND)parent->getImpl()->widget, NULL, MAIN_HINSTANCE, NULL );

	}

	HBox::HBox ( HWND real_w ) :
		Widget ( real_w ), space(0)
	{
	}

	HBox::HBox ( xgui::Container * parent, int spc, bool border ) :
		Widget ( MkHBox ( parent, border ) ), space(spc)
	{
		SetWindowLongPtr ( widget, DWLP_DLGPROC, (LONG)(LONG_PTR)Master::dlgProc );
	}

	HBox::~HBox()
	{
	}


	bool HBox::addChild ( xgui::Widget * wid )
	{
		Widget * w = wid->getImpl();

		HWND hWid = w->widget;

		xgui::HBox * hb = dynamic_cast<xgui::HBox *>(this_widget);

		if (!hb->children_.empty()) {
			xgui::Widget * &pw = hb->children_.back();
			SetWindowPos(hWid, pw->getImpl()->widget, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		}

		w->v_resize = true;

		this_widget->getContext()->redraw();

		return true;
	}


	void HBox::delChild ( xgui::Widget * )
	{
	}


	void HBox::giveSize ( std::pair<int, int> size )
	{
		xgui::HBox * hb = dynamic_cast<xgui::HBox *>(this_widget);

		cur_x = size.first;
		cur_y = size.second;

		int w = cur_x - space * 2;
		int h = cur_y - space * 2;

		int slot_x = 0;  //somma delle dimensioni minime dei figli resizabili
		double px;

		if ( hb && !hb->children_.empty() ) {

			for ( xgui::WidgetsVector::iterator wid_iter = hb->children_.begin();
				wid_iter != hb->children_.end(); ++wid_iter )
			{
				xgui::Widget * wid = *wid_iter;
				Widget * wimpl = wid->getImpl();

				int min_x = wimpl->min_x;

				if (!wimpl->h_resize)
					w -= min_x;
				else 
					slot_x += min_x;
			}

			if ( slot_x == 0 )
				px = 1;
			else
				px = (double)w / (double)slot_x;

			int child_x = space;
			int child_y = space;

			for ( xgui::WidgetsVector::iterator wid_iter = hb->children_.begin();
				wid_iter != hb->children_.end(); ++wid_iter )
			{
				xgui::Widget * wid = *wid_iter;
				Widget * wimpl = wid->getImpl();

				int min_x = wimpl->min_x;

				if ( wimpl->h_resize )
					min_x = (int)(min_x * px);

				wimpl->giveSize ( std::make_pair(min_x, h) );
				wimpl->setPos ( std::make_pair(child_x, child_y) );

				child_x += wimpl->cur_x;
			}

		}

		SetWindowPos ( widget, NULL, 0, 0, cur_x, cur_y, SWP_NOMOVE | SWP_NOZORDER );
	}


	void HBox::recalcLayout()
	{
		xgui::HBox * hb = dynamic_cast<xgui::HBox *>(this_widget);
		if ( !hb ) {
			min_x = space * 2;
			min_y = space * 2;
			return;
		}

		min_x = min_y = 0;

		for ( xgui::WidgetsVector::iterator wid_iter = hb->children_.begin();
			wid_iter != hb->children_.end(); ++wid_iter )
		{
			xgui::Widget * child = *wid_iter;
			Widget * childimpl = child->getImpl();

			child->recalcLayout();

			min_x += childimpl->min_x;
			min_y = max(min_y, childimpl->min_y);
		}

		min_x += space * 2;
		min_y += space * 2;
	}


	void HBox::setExpand ( xgui::Widget * wid, float expand )
	{
		Widget * w = wid->getImpl();
		w->h_resize = expand > 0.0;
		w->v_resize = true;
	}
}
