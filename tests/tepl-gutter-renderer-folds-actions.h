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

#ifndef TEPL_GUTTER_RENDERER_FOLDS_ACTION_H
#define TEPL_GUTTER_RENDERER_FOLDS_ACTION_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include <tepl/tepl.h>

G_BEGIN_DECLS

#define TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION 		(tepl_gutter_renderer_folds_action_get_type ())
#define TEPL_GUTTER_RENDERER_FOLDS_ACTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION, TeplGutterRendererFoldsAction))
#define TEPL_GUTTER_RENDERER_FOLDS_ACTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION, TeplGutterRendererFoldsActionClass))
#define TEPL_IS_GUTTER_RENDERER_FOLDS_ACTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION))
#define TEPL_IS_GUTTER_RENDERER_FOLDS_ACTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION))
#define TEPL_GUTTER_RENDERER_FOLDS_ACTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_GUTTER_RENDERER_FOLDS_ACTION, TeplGutterRendererFoldsActionClass))

typedef struct _TeplGutterRendererFoldsActionClass		TeplGutterRendererFoldsActionClass;
typedef struct _TeplGutterRendererFoldsAction		TeplGutterRendererFoldsAction;

struct _TeplGutterRendererFoldsAction
{
	TeplGutterRendererFolds parent_instance;
};

struct _TeplGutterRendererFoldsActionClass
{
	TeplGutterRendererFoldsClass parent_class;
};

GType				tepl_gutter_renderer_folds_action_get_type		(void) G_GNUC_CONST;

GtkSourceGutterRenderer *	tepl_gutter_renderer_folds_action_new		(void);

G_END_DECLS

#endif /* TEPL_GUTTER_RENDERER_FOLDS_ACTION_H */
