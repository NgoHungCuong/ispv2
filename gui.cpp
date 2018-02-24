#include "gui.h"

#include "flash89s.h"

extern MyGUI myGui;

void MyGUI::SetFlashFileName(char *szName) {
	gtk_entry_set_text((GtkEntry*)edFile, szName);
}

gboolean OnBrowse( GtkWidget *widget, GdkEventButton *event, gpointer data ) {
	//g_print("OnBrowse\n");
	GtkWidget *fsel;
	fsel=gtk_file_chooser_dialog_new("Open Flash File", GTK_WINDOW(data),
	GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, 
	GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
	
	gtk_widget_show_all(fsel);
	
	gint res;
	
	res=gtk_dialog_run(GTK_DIALOG(fsel));
	
	if(res==GTK_RESPONSE_OK) {
		//g_print("Open Click\n");
		myGui.SetFlashFileName(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fsel)));
	} else {
		//g_print("Cancel Click\n");
	}
	gtk_widget_destroy(fsel);
	return TRUE;
}

void MyGUI::Init(void) {
	GtkWidget *label;
	GtkWidget *table;
	
	mainWnd=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainWnd), "ISP Prog v2");
	gtk_window_set_position(GTK_WINDOW(mainWnd), GTK_WIN_POS_CENTER_ALWAYS);	
	gtk_widget_set_size_request(mainWnd, 400, 150);
	gtk_window_set_resizable(GTK_WINDOW(mainWnd), FALSE);
	
	table = gtk_table_new (5, 5, TRUE);
	
	gtk_container_add (GTK_CONTAINER (mainWnd), table);
	
	//btn = gtk_button_new_with_label ("Read");
	
	//g_signal_connect(btn, "button_press_event", G_CALLBACK (OnRead), NULL);
	
	label=gtk_label_new("Family");
	
	//gtk_table_attach_defaults (GTK_TABLE (table), btn, 3, 4, 4, 5);
	
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
	
	gtk_widget_show (label);
	
	label=gtk_label_new("MCU");
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
	gtk_widget_show (label);
	
	label=gtk_label_new("File");
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
	gtk_widget_show (label);
	
	label=gtk_label_new("Progress");
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 3, 4);
	gtk_widget_show (label);
	
	label=gtk_label_new("http://ngohungcuong.com");
	gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 3, 4, 5);
	gtk_widget_show (label);
	gtk_widget_set_sensitive(label, FALSE);
    
    btnWrite = gtk_button_new_with_label ("Write");
    gtk_table_attach_defaults (GTK_TABLE (table), btnWrite, 4, 5, 4, 5);
    gtk_widget_show (btnWrite);
    g_signal_connect(btnWrite, "button_press_event", G_CALLBACK (OnWrite), NULL);
    
	btnBrowse = gtk_button_new_with_label ("Browse");
    gtk_table_attach_defaults (GTK_TABLE (table), btnBrowse, 4, 5, 2, 3);
    gtk_widget_show (btnBrowse);
    g_signal_connect(btnBrowse, "button_press_event", G_CALLBACK (OnBrowse), mainWnd);
    
    edFile = gtk_entry_new();
    gtk_table_attach_defaults (GTK_TABLE (table), edFile, 1, 4, 2, 3);
    gtk_widget_show (edFile);
    
    cbFamily = gtk_combo_new();
    GList *flist=NULL;
    flist=g_list_append(flist, (gpointer)"AT89Sxx");
    gtk_combo_set_popdown_strings (GTK_COMBO (cbFamily), flist);
    gtk_table_attach_defaults (GTK_TABLE (table), cbFamily, 1, 5, 0, 1);
    gtk_widget_show (cbFamily);
    
    cbMcu = gtk_combo_new();
    GList *mlist=NULL;
    mlist=g_list_append(mlist, (gpointer)"AT89S51");
    mlist=g_list_append(mlist, (gpointer)"AT89S52");
    gtk_combo_set_popdown_strings (GTK_COMBO (cbMcu), mlist);
    gtk_table_attach_defaults (GTK_TABLE (table), cbMcu, 1, 5, 1, 2);
    gtk_widget_show (cbMcu);
    
    
    prgPrg = gtk_progress_bar_new();
    gtk_table_attach_defaults (GTK_TABLE (table), prgPrg, 1, 5, 3, 4);
    gtk_widget_show (prgPrg);
	
    gtk_widget_show (table);
	
	g_signal_connect(mainWnd, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show(mainWnd);
	
	gtk_progress_bar_set_text((GtkProgressBar*)prgPrg, "Ready");
	gtk_progress_bar_set_fraction((GtkProgressBar*)prgPrg, 0.0);
}

void MyGUI::Start(void) {
	Init();
}

uint32_t MyGUI::GetFamilyIndex(void) {
	return 0;
}
char *MyGUI::GetFamilyName(void) {
	return (char*)gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (cbFamily)->entry));
}
uint32_t MyGUI::GetMcuIndex(void) {
	return 0;
}
char *MyGUI::GetMcuName(void) {
	return (char*)gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (cbMcu)->entry));
}
char *MyGUI::GetFlashFileName(void) {
	return (char*)gtk_entry_get_text(GTK_ENTRY(edFile));
}
void MyGUI::SetProgress(float flFrac) {
	gtk_progress_bar_set_fraction ( GTK_PROGRESS_BAR(prgPrg), flFrac );
}
void MyGUI::SetProgress(char *szText) {
	gtk_progress_bar_set_text((GtkProgressBar*)prgPrg, (gchar*)szText);
}
