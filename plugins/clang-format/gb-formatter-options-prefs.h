/* gb-formatter-options-prefs.h
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
#ifndef __GB_FORMATTER_OPTIONS_PREFS_H__
#define __GB_FORMATTER_OPTIONS_PREFS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GB_TYPE_FORMATTER_OPTIONS_PREFS (gb_formatter_options_prefs_get_type ())

G_DECLARE_FINAL_TYPE (GbFormatterOptionsPrefs,
                      gb_formatter_options_prefs,
                      GB, FORMATTER_OPTIONS_PREFS,
                      GtkDialog)

typedef struct _GbFormatterOptionsPrefsPrivate GbFormatterOptionsPrefsPrivate;

GbFormatterOptionsPrefs *gb_formatter_options_prefs_new (void);

G_END_DECLS

#endif /* __GB_FORMATTER_OPTIONS_PREFS_H__ */
