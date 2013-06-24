/**
 * Copyright (C) 2013 Andi Drebes <andi.drebes@lip6.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "dialogs.h"
#include "glade_extras.h"
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <math.h>

int show_goto_dialog(double start, double end, double curr_value, double* time)
{
	int ret = 0;
	GladeXML* xml = glade_xml_new(DATA_PATH "/goto_dialog.glade", NULL, NULL);
	glade_xml_signal_autoconnect(xml);
	IMPORT_GLADE_WIDGET(xml, dialog);
	IMPORT_GLADE_WIDGET(xml, spin);

	printf("set range to %f - %f\n", start, end);
	gtk_spin_button_set_range(GTK_SPIN_BUTTON(spin), start, end);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(spin), pow(10, floor(log(fabs(curr_value+0.1)) / log(10))), end-start);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), curr_value);

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		*time = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin));
		ret = 1;
	}

	gtk_widget_destroy(dialog);
	g_object_unref(G_OBJECT(xml));

	return ret;
}

G_MODULE_EXPORT gint accept_dialog(GtkWidget *widget, gpointer data)
{
        gtk_dialog_response(GTK_DIALOG(widget), GTK_RESPONSE_ACCEPT);

        return 0;
}
