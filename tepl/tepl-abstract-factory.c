/*
 * This file is part of Tepl, a text editor library.
 *
 * Copyright 2017 - Sébastien Wilmet <swilmet@gnome.org>
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

#include "tepl-abstract-factory.h"
#include "tepl-file.h"
#include "tepl-tab.h"
#include "tepl-tab-label.h"

/**
 * SECTION:abstract-factory
 * @Short_description: Abstract factory singleton class
 * @Title: TeplAbstractFactory
 *
 * The Tepl framework uses the #TeplAbstractFactory singleton to create some
 * objects and widgets. By creating a subclass of #TeplAbstractFactory (to
 * override the desired virtual functions) and setting the instance with
 * tepl_abstract_factory_set_singleton(), an application can tell Tepl to create
 * custom objects and widgets.
 *
 * Note that #GtkTextViewClass has the ::create_buffer factory method, that
 * #TeplView overrides to create a #TeplBuffer. How the #TeplView and
 * #TeplBuffer are created can be customized with the ::create_tab vfunc of
 * #TeplAbstractFactory.
 *
 * Recommendation for the subclass name: in Tepl, #TeplAbstractFactory is an
 * abstract class, but in an application it is a concrete class. So
 * “MyappAbstractFactory” is not a good name for a #TeplAbstractFactory
 * subclass. “MyappFactory” is a better name (of course change “Myapp” with the
 * application namespace).
 *
 * For an application written in Vala, see #TeplAbstractFactoryVala.
 */

/* API design:
 *
 * An example of another sub-classable singleton is PeasEngine, in libpeas. I
 * didn't really like the PeasEngine implementation with get_default(), so I've
 * implemented TeplAbstractFactory differently.
 * tepl_abstract_factory_set_singleton() is more explicit. And doing things
 * explicitly is clearer. -- swilmet
 */

static TeplAbstractFactory *singleton = NULL;

G_DEFINE_TYPE (TeplAbstractFactory, tepl_abstract_factory, G_TYPE_OBJECT)

static void
tepl_abstract_factory_finalize (GObject *object)
{
	if (singleton == TEPL_ABSTRACT_FACTORY (object))
	{
		singleton = NULL;
	}

	G_OBJECT_CLASS (tepl_abstract_factory_parent_class)->finalize (object);
}

static TeplTab *
tepl_abstract_factory_create_tab_default (TeplAbstractFactory *factory)
{
	return tepl_tab_new ();
}

static GtkWidget *
tepl_abstract_factory_create_tab_label_default (TeplAbstractFactory *factory,
						TeplTab             *tab)
{
	return tepl_tab_label_new (tab);
}

static TeplFile *
tepl_abstract_factory_create_file_default (TeplAbstractFactory *factory)
{
	return tepl_file_new ();
}

static void
tepl_abstract_factory_class_init (TeplAbstractFactoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = tepl_abstract_factory_finalize;

	klass->create_tab = tepl_abstract_factory_create_tab_default;
	klass->create_tab_label = tepl_abstract_factory_create_tab_label_default;
	klass->create_file = tepl_abstract_factory_create_file_default;
}

static void
tepl_abstract_factory_init (TeplAbstractFactory *factory)
{
}

/**
 * tepl_abstract_factory_set_singleton:
 * @factory: (transfer full): a #TeplAbstractFactory.
 *
 * Sets the #TeplAbstractFactory singleton. This should be called early in
 * main(), for example just after calling tepl_init().
 *
 * This function must be called only once, before the first call to
 * tepl_abstract_factory_get_singleton().
 *
 * Tepl takes ownership of the @factory reference.
 *
 * Since: 3.0
 */
void
tepl_abstract_factory_set_singleton (TeplAbstractFactory *factory)
{
	g_return_if_fail (TEPL_IS_ABSTRACT_FACTORY (factory));

	if (singleton != NULL)
	{
		g_warning ("%s(): the TeplAbstractFactory singleton is already created.",
			   G_STRFUNC);
		return;
	}

	singleton = factory;
}

/**
 * tepl_abstract_factory_get_singleton:
 *
 * Gets the #TeplAbstractFactory singleton instance.
 *
 * If tepl_abstract_factory_set_singleton() has not been called, the singleton
 * is created with a #TeplAbstractFactory instance.
 *
 * Returns: (transfer none): the #TeplAbstractFactory singleton instance.
 * Since: 3.0
 */
TeplAbstractFactory *
tepl_abstract_factory_get_singleton (void)
{
	if (G_UNLIKELY (singleton == NULL))
	{
		singleton = g_object_new (TEPL_TYPE_ABSTRACT_FACTORY, NULL);
	}

	return singleton;
}

void
_tepl_abstract_factory_unref_singleton (void)
{
	if (singleton != NULL)
	{
		g_object_unref (singleton);
	}

	/* singleton is not set to NULL here, it is set to NULL in
	 * tepl_abstract_factory_finalize() (i.e. when we are sure that the ref
	 * count reaches 0).
	 */
}

/**
 * tepl_abstract_factory_create_main_window:
 * @factory: the #TeplAbstractFactory.
 * @app: a #GtkApplication.
 *
 * Creates a main #GtkApplicationWindow in the sense of
 * tepl_application_window_is_main_window().
 *
 * Returns: (transfer floating) (nullable): a new main application window, or
 * %NULL if the vfunc is not implemented.
 */
GtkApplicationWindow *
tepl_abstract_factory_create_main_window (TeplAbstractFactory *factory,
					  GtkApplication      *app)
{
	g_return_val_if_fail (TEPL_IS_ABSTRACT_FACTORY (factory), NULL);
	g_return_val_if_fail (GTK_IS_APPLICATION (app), NULL);

	if (TEPL_ABSTRACT_FACTORY_GET_CLASS (factory)->create_main_window != NULL)
	{
		return TEPL_ABSTRACT_FACTORY_GET_CLASS (factory)->create_main_window (factory, app);
	}

	g_warning ("The TeplAbstractFactory::create_main_window vfunc is not implemented.");
	return NULL;
}

/**
 * tepl_abstract_factory_create_tab:
 * @factory: the #TeplAbstractFactory.
 *
 * Returns: (transfer floating): a new #TeplTab.
 * Since: 3.0
 */
TeplTab *
tepl_abstract_factory_create_tab (TeplAbstractFactory *factory)
{
	g_return_val_if_fail (TEPL_IS_ABSTRACT_FACTORY (factory), NULL);

	return TEPL_ABSTRACT_FACTORY_GET_CLASS (factory)->create_tab (factory);
}

/**
 * tepl_abstract_factory_create_tab_label:
 * @factory: the #TeplAbstractFactory.
 * @tab: a #TeplTab.
 *
 * Creates a new tab label for @tab, suitable for gtk_notebook_set_tab_label().
 *
 * Returns: (transfer floating) (nullable): a new #GtkWidget, or %NULL for the
 * default tab label (“page N” with #GtkNotebook).
 * Since: 3.0
 */
GtkWidget *
tepl_abstract_factory_create_tab_label (TeplAbstractFactory *factory,
					TeplTab             *tab)
{
	g_return_val_if_fail (TEPL_IS_ABSTRACT_FACTORY (factory), NULL);
	g_return_val_if_fail (TEPL_IS_TAB (tab), NULL);

	return TEPL_ABSTRACT_FACTORY_GET_CLASS (factory)->create_tab_label (factory, tab);
}

/**
 * tepl_abstract_factory_create_file:
 * @factory: the #TeplAbstractFactory.
 *
 * Returns: (transfer full): a new #TeplFile.
 * Since: 4.0
 */
TeplFile *
tepl_abstract_factory_create_file (TeplAbstractFactory *factory)
{
	g_return_val_if_fail (TEPL_IS_ABSTRACT_FACTORY (factory), NULL);

	return TEPL_ABSTRACT_FACTORY_GET_CLASS (factory)->create_file (factory);
}
