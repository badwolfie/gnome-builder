/* gb-clang-format-workbench-addin.c
 *
 * Copyright (C) 2016 Ian Hernandez <ihernandezs@openmailbox.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gb-clang-format-workbench-addin.h"

#include <ide.h>

struct _GbClangFormatWorkbenchAddin
{
  GObject       parent;
  IdeWorkbench *workbench;
};

static void workbench_addin_iface_init (IdeWorkbenchAddinInterface *iface);

G_DEFINE_TYPE_EXTENDED (GbClangFormatWorkbenchAddin,
                        gb_clang_format_workbench_addin,
                        G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (IDE_TYPE_WORKBENCH_ADDIN,
                                               workbench_addin_iface_init))

static void gb_clang_format_workbench_addin_load   (IdeWorkbenchAddin *addin,
                                                    IdeWorkbench      *workbench);

static void gb_clang_format_workbench_addin_unload (IdeWorkbenchAddin *addin,
                                                    IdeWorkbench      *workbench);

static void
workbench_addin_iface_init (IdeWorkbenchAddinInterface *iface)
{
  iface->load = gb_clang_format_workbench_addin_load;
  iface->unload = gb_clang_format_workbench_addin_unload;
}

static void
gb_clang_format_workbench_addin_init (GbClangFormatWorkbenchAddin *self)
{
}

static void
gb_clang_format_workbench_addin_class_init (GbClangFormatWorkbenchAddinClass *class)
{
}

static void
gb_clang_format_workbench_addin_load (IdeWorkbenchAddin *addin,
                                      IdeWorkbench      *workbench)
{
  GbClangFormatWorkbenchAddin *self = GB_CLANG_FORMAT_WORKBENCH_ADDIN (addin);

  g_assert (GB_IS_CLANG_FORMAT_WORKBENCH_ADDIN (addin));
  g_assert (IDE_IS_WORKBENCH (workbench));

  ide_set_weak_pointer (&self->workbench, workbench);
}

static void
gb_clang_format_workbench_addin_unload (IdeWorkbenchAddin *addin,
                                        IdeWorkbench      *workbench)
{
  GbClangFormatWorkbenchAddin *self = GB_CLANG_FORMAT_WORKBENCH_ADDIN (addin);

  g_assert (GB_IS_CLANG_FORMAT_WORKBENCH_ADDIN (addin));
  g_assert (IDE_IS_WORKBENCH (workbench));

  ide_clear_weak_pointer (&self->workbench);
}
