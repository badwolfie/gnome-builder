/* gb-formatter.c
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
#include "gb-formatter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE   (512)
#define FILE_MAX_SIZE (10 * 1024 * 1024)

static gchar reading_buffer[BUFFER_SIZE];

struct _GbFormatter
{
  GObject             parent;
  GbFormatterPrivate *priv;
};

struct _GbFormatterClass
{
  GObjectClass parent_class;
};

struct _GbFormatterPrivate
{
  GbFormatterOptions *options;
  gchar              *input_file;
  gchar              *formatted_text;
};

G_DEFINE_TYPE_WITH_PRIVATE (GbFormatter, gb_formatter, G_TYPE_OBJECT);

GbFormatter *
gb_formatter_new (GbFormatterOptions *options)
{
  GbFormatter *self;

  self = g_object_new (GB_TYPE_FORMATTER, NULL);
  self->priv = gb_formatter_get_instance_private (self);

  self->priv->options = options;
  self->priv->input_file = NULL;
  self->priv->formatted_text = NULL;

  return self;
}

gchar *
gb_formatter_get_input_file (GbFormatter *self)
{
  return self->priv->input_file;
}

void
gb_formatter_set_input_file (GbFormatter *self,
                             const gchar *value)
{
  if (self->priv->input_file != NULL)
    free (self->priv->input_file);

  self->priv->input_file = g_strdup (value);
}

gchar *
gb_formatter_get_formatted_text (GbFormatter *self)
{
  return self->priv->formatted_text;
}

void
gb_formatter_set_formatted_text (GbFormatter *self,
                                 const gchar *value)
{
  if (self->priv->formatted_text != NULL)
    free (self->priv->formatted_text);

  self->priv->formatted_text = g_strdup (value);
}

GbFormatterOptions *
gb_formatter_get_options (GbFormatter *self)
{
  return self->priv->options;
}

void
gb_formatter_set_options (GbFormatter        *self,
                          GbFormatterOptions *value)
{
  if (self->priv->options != value)
    self->priv->options = value;
}

void
gb_formatter_process_file (GbFormatter *self,
                           const gchar *filename)
{
  FILE  *clang;
  gulong file_size = 0;
  size_t bytes_read;

  gb_formatter_set_input_file (self, filename);

  sprintf (reading_buffer,
           "clang-format -style=%s -fallback-style=%s %s",
           gb_formatter_options_get_format_style (self->priv->options),
           gb_formatter_options_get_format_default_style (self->priv->options),
           self->priv->input_file);

  clang = popen (reading_buffer, "r");
  memset (reading_buffer, 0x00, sizeof (reading_buffer));

  if (!clang)
    {
      puts ("Error while opening the pipe for clang-format.");
      return;
    }

  while (!feof (clang))
    {
      bytes_read = fread (reading_buffer, sizeof (char), BUFFER_SIZE, clang);
      file_size += bytes_read;

      if (file_size >= FILE_MAX_SIZE)
        {
          puts ("Error, file too big to process.");
          return;
        }

      self->priv->formatted_text =
          realloc (self->priv->formatted_text, file_size + 1);

      strcat (self->priv->formatted_text, reading_buffer);
      memset (reading_buffer, 0x00, sizeof (reading_buffer));
    }

  if (pclose (clang))
    {
      puts ("Error while closing the pipe for clang-format.");
      return;
    }
}

static void
gb_formatter_init (GbFormatter *self)
{
}

static void
gb_formatter_class_init (GbFormatterClass *class)
{
}

#undef BUFFER_SIZE
#undef FILE_MAX_SIZE
