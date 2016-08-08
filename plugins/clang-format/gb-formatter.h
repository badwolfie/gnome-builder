/* gb-formatter.h
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
#ifndef __GB_FORMATTER_H__
#define __GB_FORMATTER_H__

#include "gb-formatter-options.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define GB_TYPE_FORMATTER (gb_formatter_get_type ())

G_DECLARE_FINAL_TYPE (GbFormatter, gb_formatter, GB, FORMATTER, GObject)

typedef struct _GbFormatterPrivate GbFormatterPrivate;

GbFormatter        *gb_formatter_new                (GbFormatterOptions *options);

gchar              *gb_formatter_get_input_file     (GbFormatter        *self);

void                gb_formatter_set_input_file     (GbFormatter        *self,
                                                     const gchar        *value);

gchar              *gb_formatter_get_formatted_text (GbFormatter        *self);

void                gb_formatter_set_formatted_text (GbFormatter        *self,
                                                     const gchar        *value);

GbFormatterOptions *gb_formatter_get_options        (GbFormatter        *self);

void                gb_formatter_set_options        (GbFormatter        *self,
                                                     GbFormatterOptions *value);

void                gb_formatter_process_file       (GbFormatter        *self,
                                                     const gchar        *filename);

G_END_DECLS

#endif /* __GB_FORMATTER_H__ */
