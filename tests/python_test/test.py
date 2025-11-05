#! /usr/bin/env python

import xgui, time

from os3.utils import thread_control

tc = None
quit = False

def modal_win_quit(win):
	return xgui.EVT_BLOCK
modal_quit_cb = xgui.PyCallback(modal_win_quit)

def count_func(win, spin):
	print "Entering update thread"
	
	for i in range(0, 10):
		xgui.Master.GuiLock()
		spin.set("pos", str(i))
		xgui.Master.GuiUnlock()
		time.sleep(1)

	win.modalReturn(0)
	
def btn_cb_impl(btn):
	global tc

	w = xgui.Master.CreateWindow(btn.getContext())
	spin = xgui.Master.CreateProgressbar(w, 0, 10)

	w.linkEvent("close", modal_quit_cb)
	w.show()

	tc.spawn("counter", count_func, (w, spin))
	w.modalRun()

	xgui.Master.DestroyWidget(w)
	print "bye"

	return None
btn_cb = xgui.PyCallback(btn_cb_impl)

def win_cb_impl(win):
	xgui.Master.Quit()
win_cb = xgui.PyCallback(win_cb_impl)

def menu_selected_impl ( menu, id, status ):
	print id, status
	 
	w = menu.getContext ()

	print w

	if id == 'open':
		xgui.Master.OpenFileDialog ( w,  
						(
							xgui.FileExtension( "Text", "txt" ),
							xgui.FileExtension( "JPEG", "jpg" )
						), 
						"Apriamo un bel file",
						"/tmp",
						False
					 )
	elif id == 'save':
		pass


	return 1
menu_selected = xgui.PyTextStatusCallback( menu_selected_impl )

def text_cb_impl(entry, text):
	print "Testo della entry",text
text_cb = xgui.PyTextCallback(text_cb_impl)


def thread_func():
	global quit
	
	while not quit:
		time.sleep(1)
		print "ciao"
		
	print "bye, bye"

def gui_thread():
	xgui.Master.InitThreads()
	xgui.Master.GuiLock()

	w = xgui.Master.CreateWindow ()
	vbox = xgui.Master.CreateVBox ( w, 0, 0 )
	tbar = xgui.Master.CreateToolbar ( vbox )
	clist = xgui.Master.CreateList ( vbox )
	hbox  = xgui.Master.CreateHBox ( vbox, 0, 0 )
	lbl   = xgui.Master.CreateLabel (hbox, "Dir:" )
	txt   = xgui.Master.CreateEntry ( hbox, "/home" )
	btn   = xgui.Master.CreateButton ( hbox, "R" )

	vbox.setExpand ( hbox, False )
	vbox.setExpand ( clist, True )
	hbox.setExpand ( txt, True )

	menu = xgui.Master.CreateMenu ( w, True )
	menu.addItem ( "", "file", "File" )
	menu.addItem ( "file", "open", "Open" )
	menu.addItem ( "file", "save", "Save" )


	menu.linkEvent ( "onselect", menu_selected )
	btn.linkEvent("onclick", btn_cb)
	w.linkEvent("onclose", win_cb)

	w.show()

	xgui.Master.Run()
	xgui.Master.GuiUnlock()


tc = thread_control.ThreadControl()
tc.spawn("one", thread_func, ())

gui_thread()

print "Quitting..."
quit = True
