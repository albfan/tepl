/*
 * This file is part of Tepl, a text editor library.
 *
 * Copyright 2016 - David Rabel <david.rabel@noresoft.com>
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

#include "tepl-gutter-renderer-folds-actions.h"

G_DEFINE_TYPE (TeplGutterRendererFoldsAction,
	       tepl_gutter_renderer_folds_action,
	       TEPL_TYPE_GUTTER_RENDERER_FOLDS)

static void
tepl_gutter_renderer_folds_action_draw (GtkSourceGutterRenderer      *renderer,
				     cairo_t                      *cr,
				     GdkRectangle                 *background_area,
				     GdkRectangle                 *cell_area,
				     GtkTextIter                  *start,
				     GtkTextIter                  *end,
				     GtkSourceGutterRendererState  state)
{
	gchar* line;
	TeplGutterRendererFoldsState folding_state;

   g_autoptr(GRegex) line_regex_start = NULL;
   g_autoptr(GRegex) line_regex_end = NULL;
   GMatchInfo *info = NULL;
   line_regex_start = g_regex_new("^\\s*{", 0, 0, NULL);
   line_regex_end = g_regex_new("^\\s*}", 0, 0, NULL);

	line = gtk_text_iter_get_text (start, end);

   //TODO: How to know if fold is opened or closed
  
	folding_state = TEPL_GUTTER_RENDERER_FOLDS_STATE_NONE;
   //TODO: How to get self to know if we are into a continue or not
	//folding_state = TEPL_GUTTER_RENDERER_FOLDS_STATE_CONTINUE;

	//folding_state = TEPL_GUTTER_RENDERER_FOLDS_STATE_START_FOLDED;
   if (g_regex_match(line_regex_start, line, 0, &info))
	{
		folding_state = TEPL_GUTTER_RENDERER_FOLDS_STATE_START_OPENED;
	}
   if (g_regex_match(line_regex_end, line, 0, &info))
	{
		folding_state = TEPL_GUTTER_RENDERER_FOLDS_STATE_END;
	}

	tepl_gutter_renderer_folds_set_state (TEPL_GUTTER_RENDERER_FOLDS (renderer), folding_state);

	if (GTK_SOURCE_GUTTER_RENDERER_CLASS (tepl_gutter_renderer_folds_action_parent_class)->draw != NULL)
	{
		GTK_SOURCE_GUTTER_RENDERER_CLASS (tepl_gutter_renderer_folds_action_parent_class)->draw (renderer,
												      cr,
												      background_area,
												      cell_area,
												      start,
												      end,
												      state);
	}
}

static void
tepl_gutter_renderer_folds_action_class_init (TeplGutterRendererFoldsActionClass *klass)
{
	GtkSourceGutterRendererClass *renderer_class = GTK_SOURCE_GUTTER_RENDERER_CLASS (klass);

	renderer_class->draw = tepl_gutter_renderer_folds_action_draw;
}

static void
tepl_gutter_renderer_folds_action_init (TeplGutterRendererFoldsAction *self)
{
}

GtkSourceGutterRenderer *
tepl_gutter_renderer_folds_action_new (void)
{
	return g_object_new (TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION, NULL);
}
