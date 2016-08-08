/* gb-formatter-options.h
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
#ifndef __GB_FORMATTER_OPTIONS_H__
#define __GB_FORMATTER_OPTIONS_H__

#include <gio/gio.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GB_TYPE_FORMATTER_OPTIONS (gb_formatter_options_get_type ())

G_DECLARE_FINAL_TYPE (GbFormatterOptions,
                      gb_formatter_options,
                      GB, FORMATTER_OPTIONS,
                      GObject)

typedef struct _GbFormatterOptionsPrivate GbFormatterOptionsPrivate;

GbFormatterOptions *gb_formatter_options_new                      (GSettings          *settings);

void                gb_formatter_options_load_settings            (GbFormatterOptions *self,
                                                                   GSettings          *settings);

GSettings          *gb_formatter_options_get_settings             (void);

const gchar        *gb_formatter_options_get_format_style         (GbFormatterOptions *self);

void                gb_formatter_options_set_format_style         (GbFormatterOptions *self,
                                                                   const gchar        *value);

const gchar        *gb_formatter_options_get_format_default_style (GbFormatterOptions *self);

void                gb_formatter_options_set_format_default_style (GbFormatterOptions *self,
                                                                   const gchar        *value);

gboolean            gb_formatter_options_get_format_on_save       (GbFormatterOptions *self);

void                gb_formatter_options_set_format_on_save       (GbFormatterOptions *self,
                                                                   gboolean            value);

G_END_DECLS

#endif /* __GB_FORMATTER_OPTIONS_H__ */
