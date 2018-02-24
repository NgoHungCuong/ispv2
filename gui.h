#ifndef MY_GUI_H_
#define MY_GUI_H_
#include <stdint.h>
#include <gtk/gtk.h>
#include <pthread.h>
class MyGUI {
private:
	void Init(void);
	GtkWidget *mainWnd;
	GtkWidget *cbFamily;
	GtkWidget *cbMcu;
	GtkWidget *edFile;
	GtkWidget *btnBrowse;
	GtkWidget *btnWrite;
	GtkWidget *prgPrg;
public:
	void Start(void);
	uint32_t GetFamilyIndex(void);
	char *GetFamilyName(void);
	uint32_t GetMcuIndex(void);
	char *GetMcuName(void);
	char *GetFlashFileName(void);
	void SetFlashFileName(char *szName);
	void SetProgress(float flFrac);
	void SetProgress(char *szText);
};
#endif
