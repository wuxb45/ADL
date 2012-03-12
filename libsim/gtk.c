#include <gtk/gtk.h>
#include <glade/glade.h>

#include "debug.h"

static GtkWindow * top;
static GtkWindow * scroll;
static GtkWidget * state;
static GtkCList * clist;
static GtkWidget * step;

static int a=0;

void
step_callback(GtkWidget * widget, gpointer data)
{
	char buffer[2][20];

	gtk_clist_freeze(clist);
	gtk_clist_clear(clist);

	fgets(buffer[0], 19, stdin);
	fgets(buffer[1], 19, stdin);
	gtk_clist_append(clist, buffer);

	fgets(buffer[0], 19, stdin);
	fgets(buffer[1], 19, stdin);
	gtk_clist_append(clist, buffer);

	gtk_clist_thaw(clist);
}

int
main(int argc, char ** argv)
{
	GladeXML * xml;

	gtk_init(&argc, &argv);

	xml = glade_xml_new("debug.glade", NULL, NULL);

	glade_xml_signal_autoconnect(xml);

	step = glade_xml_get_widget(xml, "toolbutton_step");
	if(step){
		gtk_signal_connect(GTK_OBJECT(step), "clicked", step_callback, NULL);
	}

	clist = gtk_clist_new_with_titles(2, ctitle);
	gtk_clist_set_column_auto_resize(clist, 0, 1);
	gtk_clist_set_column_auto_resize(clist, 1, 1);
	

	scroll = glade_xml_get_widget(xml, "scrolledwindow1");
	if(scroll){
		gtk_container_add(GTK_CONTAINER(scroll), clist);
		gtk_widget_show((GtkWidget*)clist);
	}

	top = (GtkWindow*) glade_xml_get_widget(xml, "window1");
	if(top){
		gtk_widget_show((GtkWidget*)top);
	}else{
		return 0;
	}

	state = glade_xml_get_widget(xml, "label1");
	if(state == NULL){
		return 0;
	}

	gtk_main();
	return 0;
}
