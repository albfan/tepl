/*
 * This file is part of Gtef, a text editor library.
 *
 * Copyright 2017 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Gtef is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Gtef is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <gtef/gtef.h>

static void
test_add_action_entries_check_dups (void)
{
	GSimpleActionGroup *group;
	const GActionEntry entries[] =
	{
		{ "quit" },
		{ "about" }
	};

	group = g_simple_action_group_new ();
	gtef_action_map_add_action_entries_check_dups (G_ACTION_MAP (group),
						       entries,
						       G_N_ELEMENTS (entries),
						       NULL);
	g_object_unref (group);

	g_test_trap_subprocess ("/action-map/add-action-entries-check-dups/subprocess/add-two-times", 0, 0);
	g_test_trap_assert_failed ();
	g_test_trap_assert_stderr ("*the GActionMap already contains a GAction with the name*");
}

static void
test_add_action_entries_check_dups__add_two_times (void)
{
	GSimpleActionGroup *group;
	const GActionEntry entries[] =
	{
		{ "quit" }
	};

	group = g_simple_action_group_new ();

	gtef_action_map_add_action_entries_check_dups (G_ACTION_MAP (group),
						       entries,
						       G_N_ELEMENTS (entries),
						       NULL);
	gtef_action_map_add_action_entries_check_dups (G_ACTION_MAP (group),
						       entries,
						       G_N_ELEMENTS (entries),
						       NULL);

	g_object_unref (group);
}

gint
main (gint    argc,
      gchar **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/action-map/add-action-entries-check-dups",
			 test_add_action_entries_check_dups);
	g_test_add_func ("/action-map/add-action-entries-check-dups/subprocess/add-two-times",
			 test_add_action_entries_check_dups__add_two_times);

	return g_test_run ();
}
