#include "gui.h"

MyGUI myGui;

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	myGui.Start();
	gtk_main();
	return 0;
}
