#include "flash89s.h"
#include <pthread.h>
#include "gui.h"
#include "libAT89S.h"
#include "libPgmISPv2.h"

myAT89S myFlash89s;
PgmISPv2 ispv2;

extern MyGUI myGui;

pthread_t pth;

void *WriteThreadFunc(void *p);

void *WriteThreadFunc(void *p) {
	myFlash89s.WriteFlash(myGui.GetMcuName(), myGui.GetFlashFileName());
}

gboolean OnWrite(GtkWidget *widget, GdkEventButton *event, gpointer data) {
	//g_print("OnWrite\n");
	myFlash89s.prog=&ispv2;
	//kiem tra xem lay dung thong tin hay chua
	//g_print("%s\n", myGui.GetMcuName());
	//g_print("%s\n", myGui.GetFlashFileName());
	//return TRUE;
	pthread_create( &pth, NULL, WriteThreadFunc, NULL);
	return TRUE;
}
