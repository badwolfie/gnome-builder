/* gb-formatter-options.c
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
#include "gb-formatter-options.h"

#include <stdlib.h>

struct _GbFormatterOptions
{
  GObject                    parent;
  GbFormatterOptionsPrivate *priv;
};

struct _GbFormatterOptionsPrivate
{
  gchar   *format_style;
  gchar   *format_default_style;
  gboolean format_on_save;
};

G_DEFINE_TYPE_WITH_PRIVATE (GbFormatterOptions,
                            gb_formatter_options,
                            G_TYPE_OBJECT);

enum
{
  GB_FORMATTER_OPTIONS_START = 0,
  GB_FORMATTER_OPTIONS_FORMAT_STYLE,
  GB_FORMATTER_OPTIONS_FORMAT_DEFAULT_STYLE,
  GB_FORMATTER_OPTIONS_FORMAT_ON_SAVE,
  N_PROPERTIES
};

static GParamSpec *gb_formatter_options_properties[N_PROPERTIES] = { NULL };

static void get_property (GObject      *object,
                          guint         property_id,
                          GValue       *value,
                          GParamSpec   *pspec);

static void set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec);

GbFormatterOptions *
gb_formatter_options_new (GSettings *settings)
{
  GbFormatterOptions *self;

  self = g_object_new (GB_TYPE_FORMATTER_OPTIONS, NULL);
  self->priv = gb_formatter_options_get_instance_private (self);

  gb_formatter_options_load_settings (self, settings);
  return self;
}

void
gb_formatter_options_load_settings (GbFormatterOptions *self,
                                    GSettings          *settings)
{
  gchar   *format_style;
  gchar   *default_format_style;
  gboolean format_on_save;

  format_style = g_settings_get_string (settings, "format-style");
  gb_formatter_options_set_format_style (self, format_style);

  default_format_style = g_settings_get_string (settings,
                                                "format-default-style");
  gb_formatter_options_set_format_default_style (self, default_format_style);

  format_on_save = g_settings_get_boolean (settings, "format-on-save");
  gb_formatter_options_set_format_on_save (self, format_on_save);

  g_object_unref (settings);
}

GSettings *
gb_formatter_options_get_settings (void)
{
  const gchar           *schema_dir = "/home/iann/Proyectos/gb-formatter/data";
  GError                *inner_error = NULL;
  GSettingsSchemaSource *schema_source;
  GSettingsSchema       *schema;
  GSettings             *settings;

  schema_source = g_settings_schema_source_new_from_directory (schema_dir,
                                                               NULL, FALSE,
                                                               &inner_error);
  if (inner_error != NULL)
    {
      g_error ("g_settings_schema_source_new_from_directory error: %s", inner_error->message);
      g_error_free (inner_error);
    }

  schema = g_settings_schema_source_lookup (schema_source,
                                            "org.gnome.builder.plugins.clang_format_plugin",
                                            FALSE);

  settings = g_settings_new_full (schema, NULL, NULL);

  return settings;
}

const gchar *
gb_formatter_options_get_format_style (GbFormatterOptions *self)
{
  return self->priv->format_style;
}

void
gb_formatter_options_set_format_style (GbFormatterOptions *self,
                                       const gchar        *value)
{
  if (self->priv->format_style != NULL)
    free (self->priv->format_style);

  self->priv->format_style = g_strdup (value);
  g_object_notify (G_OBJECT (self), "format-style");
}

const gchar *
gb_formatter_options_get_format_default_style (GbFormatterOptions *self)
{
  return self->priv->format_default_style;
}

void
gb_formatter_options_set_format_default_style (GbFormatterOptions *self,
                                               const gchar        *value)
{
  if (self->priv->format_default_style != NULL)
    free (self->priv->format_default_style);

  self->priv->format_default_style = g_strdup (value);
  g_object_notify (G_OBJECT (self), "format-default-style");
}

gboolean
gb_formatter_options_get_format_on_save (GbFormatterOptions *self)
{
  return self->priv->format_on_save;
}

void
gb_formatter_options_set_format_on_save (GbFormatterOptions *self,
                                         gboolean            value)
{
  if (self->priv->format_on_save != value)
    self->priv->format_on_save = value;

  g_object_notify (G_OBJECT (self), "format-on-save");
}

static void
gb_formatter_options_init (GbFormatterOptions *self)
{
}

static void
gb_formatter_options_class_init (GbFormatterOptionsClass *class)
{
  GObjectClass *g_class;

  g_class = (G_OBJECT_CLASS (class));
  g_class->get_property = get_property;
  g_class->set_property = set_property;

  gb_formatter_options_properties[GB_FORMATTER_OPTIONS_FORMAT_STYLE] =
      g_param_spec_string ("format-style",
                           "format-style",
                           "format-style",
                           "file",
                           G_PARAM_READWRITE);

  gb_formatter_options_properties[GB_FORMATTER_OPTIONS_FORMAT_DEFAULT_STYLE] =
      g_param_spec_string ("format-default-style",
                           "format-default-style",
                           "format-default-style",
                           "LLVM",
                           G_PARAM_READWRITE);

  gb_formatter_options_properties[GB_FORMATTER_OPTIONS_FORMAT_ON_SAVE] =
      g_param_spec_boolean ("format-on-save",
                            "format-on-save",
                            "format-on-save",
                            FALSE,
                            G_PARAM_READWRITE);

  g_object_class_install_properties (g_class,
                                     N_PROPERTIES,
                                     gb_formatter_options_properties);
}

static void
get_property (GObject    *object,
              guint       property_id,
              GValue     *value,
              GParamSpec *pspec)
{
  GbFormatterOptions *self;

  self = GB_FORMATTER_OPTIONS (object);

  switch (property_id)
    {
    case GB_FORMATTER_OPTIONS_FORMAT_STYLE:
      g_value_set_string (value, self->priv->format_style);
      break;

    case GB_FORMATTER_OPTIONS_FORMAT_DEFAULT_STYLE:
      g_value_set_string (value, self->priv->format_default_style);
      break;

    case GB_FORMATTER_OPTIONS_FORMAT_ON_SAVE:
      g_value_set_boolean (value, self->priv->format_on_save);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
set_property (GObject      *object,
              guint         property_id,
              const GValue *value,
              GParamSpec   *pspec)
{
  GbFormatterOptions *self;

  self = GB_FORMATTER_OPTIONS (object);

  switch (property_id)
    {
    case GB_FORMATTER_OPTIONS_FORMAT_STYLE:
      self->priv->format_style = (gchar *) g_value_get_string (value);
      break;

    case GB_FORMATTER_OPTIONS_FORMAT_DEFAULT_STYLE:
      self->priv->format_default_style = (gchar *) g_value_get_string (value);
      break;

    case GB_FORMATTER_OPTIONS_FORMAT_ON_SAVE:
      self->priv->format_on_save = g_value_get_boolean (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
