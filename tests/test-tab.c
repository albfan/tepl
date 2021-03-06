/*
 * This file is part of Tepl, a text editor library.
 *
 * Copyright 2016 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Tepl is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Tepl is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <tepl/tepl.h>
#include "tepl/tepl-progress-info-bar.h"
#include "tepl/tepl-io-error-info-bar.h"
#include <stdlib.h>

static void
info_bar_response_cb (GtkInfoBar *info_bar,
		      gint        response_id,
		      gpointer    user_data)
{
	gtk_widget_destroy (GTK_WIDGET (info_bar));
}

static void
basic_cb (GtkButton *button,
	  TeplTab   *tab)
{
	TeplInfoBar *info_bar;
	GtkWidget *entry;

	info_bar = tepl_info_bar_new_simple (GTK_MESSAGE_WARNING,
					     "Primary message.",
					     "Secondary message.");

	entry = gtk_entry_new ();
	gtk_widget_show (entry);
	tepl_info_bar_add_content_widget (info_bar, entry);

	tepl_info_bar_add_close_button (info_bar);

	tepl_tab_add_info_bar (tab, GTK_INFO_BAR (info_bar));
	gtk_widget_show (GTK_WIDGET (info_bar));
}

static void
progress_cb (GtkButton *button,
	     TeplTab   *tab)
{
	TeplProgressInfoBar *info_bar;

	info_bar = _tepl_progress_info_bar_new ("File loading... The full and very long path is: "
						"/home/seb/a/very/long/path/like/this/is/beautiful"
						"/but/is/it/correctly/wrapped/in/the/info/bar/that"
						"/is/the/question",
						TRUE);

	_tepl_progress_info_bar_set_fraction (info_bar, 0.3);

	g_signal_connect (info_bar,
			  "response",
			  G_CALLBACK (info_bar_response_cb),
			  NULL);

	tepl_tab_add_info_bar (tab, GTK_INFO_BAR (info_bar));
	gtk_widget_show (GTK_WIDGET (info_bar));
}

static void
add_io_loading_error_info_bar (TeplTab *tab,
			       GError  *error)
{
	GFile *location;
	GtkSourceFile *file;
	GtkSourceBuffer *buffer;
	GtkSourceFileLoader *loader;
	TeplIoErrorInfoBar *info_bar;

	location = g_file_new_for_path ("/home/seb/test.c");
	file = gtk_source_file_new ();
	gtk_source_file_set_location (file, location);
	buffer = gtk_source_buffer_new (NULL);
	loader = gtk_source_file_loader_new (buffer, file);

	info_bar = _tepl_io_error_info_bar_new ();
	_tepl_io_error_info_bar_set_loading_error (info_bar, loader, error);

	g_signal_connect (info_bar,
			  "response",
			  G_CALLBACK (info_bar_response_cb),
			  NULL);

	tepl_tab_add_info_bar (tab, GTK_INFO_BAR (info_bar));
	gtk_widget_show (GTK_WIDGET (info_bar));

	g_object_unref (location);
	g_object_unref (file);
	g_object_unref (buffer);
	g_object_unref (loader);
}

static void
permission_denied_cb (GtkButton *button,
		      TeplTab   *tab)
{
	GError *error = g_error_new (G_IO_ERROR, G_IO_ERROR_PERMISSION_DENIED, "blah");
	add_io_loading_error_info_bar (tab, error);
	g_error_free (error);
}

static void
not_found_cb (GtkButton *button,
	      TeplTab   *tab)
{
	GError *error = g_error_new (G_IO_ERROR, G_IO_ERROR_NOT_FOUND, "blah");
	add_io_loading_error_info_bar (tab, error);
	g_error_free (error);
}

static void
conversion_fallback_cb (GtkButton *button,
			TeplTab   *tab)
{
	GError *error = g_error_new (GTK_SOURCE_FILE_LOADER_ERROR,
				     GTK_SOURCE_FILE_LOADER_ERROR_CONVERSION_FALLBACK,
				     "blah");
	add_io_loading_error_info_bar (tab, error);
	g_error_free (error);
}

static GtkWidget *
create_side_panel (TeplTab *tab)
{
	GtkGrid *vgrid;
	GtkWidget *basic;
	GtkWidget *progress;
	GtkWidget *permission_denied;
	GtkWidget *not_found;
	GtkWidget *conversion_fallback;

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	basic = gtk_button_new_with_label ("Basic");
	gtk_container_add (GTK_CONTAINER (vgrid), basic);
	g_signal_connect_object (basic,
				 "clicked",
				 G_CALLBACK (basic_cb),
				 tab,
				 0);

	progress = gtk_button_new_with_label ("Progress");
	gtk_container_add (GTK_CONTAINER (vgrid), progress);
	g_signal_connect_object (progress,
				 "clicked",
				 G_CALLBACK (progress_cb),
				 tab,
				 0);

	permission_denied = gtk_button_new_with_label ("Permission denied");
	gtk_container_add (GTK_CONTAINER (vgrid), permission_denied);
	g_signal_connect_object (permission_denied,
				 "clicked",
				 G_CALLBACK (permission_denied_cb),
				 tab,
				 0);

	not_found = gtk_button_new_with_label ("Not found");
	gtk_container_add (GTK_CONTAINER (vgrid), not_found);
	g_signal_connect_object (not_found,
				 "clicked",
				 G_CALLBACK (not_found_cb),
				 tab,
				 0);

	conversion_fallback = gtk_button_new_with_label ("Conversion fallback");
	gtk_container_add (GTK_CONTAINER (vgrid), conversion_fallback);
	g_signal_connect_object (conversion_fallback,
				 "clicked",
				 G_CALLBACK (conversion_fallback_cb),
				 tab,
				 0);

	return GTK_WIDGET (vgrid);
}

static GtkWidget *
create_window_content (void)
{
	GtkGrid *hgrid;
	TeplTab *tab;
	GtkWidget *side_panel;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (hgrid), GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_set_column_spacing (hgrid, 6);
	g_object_set (hgrid,
		      "margin", 6,
		      NULL);

	tab = tepl_tab_new ();
	side_panel = create_side_panel (tab);

	gtk_container_add (GTK_CONTAINER (hgrid), side_panel);
	gtk_container_add (GTK_CONTAINER (hgrid), GTK_WIDGET (tab));

	gtk_widget_show_all (GTK_WIDGET (hgrid));

	return GTK_WIDGET (hgrid);
}

gint
main (gint    argc,
      gchar **argv)
{
	GtkWidget *window;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	gtk_container_add (GTK_CONTAINER (window), create_window_content ());
	gtk_widget_show (window);

	g_signal_connect (window,
			  "destroy",
			  G_CALLBACK (gtk_main_quit),
			  NULL);

	gtk_main ();

	return EXIT_SUCCESS;
}
