#! /usr/bin/env python
import xgui

w = xgui.Master.CreateWindow()
w_desc = xgui.Object.Marshall(w)
w2 = xgui.Object.UnMarshall(w_desc)

print "COMPARING INSTANCES"
print w, "==", w2

print "CHECKING DOWNCASTING CORRECTNESS"
w.set("id", "id_della_finestra")
print "id_della_finestra ==",w2.get("id")

print "TESTING DYNAMIC METHODS CORRECTNESS"
wr = xgui.Object.UnMarshall( w.call("cicle", xgui.Object.Marshall(w)) )
print wr, "==", w

xgui.Master.DestroyWidget(w)

print "TESING DYNAMIC OBJECTS DESTRUCTION NOTIFICATION"
print wr
