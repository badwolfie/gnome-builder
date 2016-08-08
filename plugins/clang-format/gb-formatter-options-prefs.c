/* gb-formatter-options-prefs.c
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
#include "gb-formatter-options-prefs.h"
#include "gb-formatter-options.h"

enum
{
  FORMATTER_STYLE_FILE,
  FORMATTER_STYLE_LLVM,
  FORMATTER_STYLE_GOOGLE,
  FORMATTER_STYLE_CHROMIUM,
  FORMATTER_STYLE_MOZILLA,
  FORMATTER_STYLE_WEBKIT,
  // FORMATTER_STYLE_CUSTOM,
  N_FORMATTER_STYLES
};

static const gchar *formatter_styles[] = {
  "file", "LLVM", "Google", "Chromium", "Mozilla", "WebKit" //, "Custom"
};

struct _GbFormatterOptionsPrefs
{
  GtkDialog                       parent;
  GbFormatterOptionsPrefsPrivate *priv;
};

struct _GbFormatterOptionsPrefsPrivate
{
  GtkComboBox    *style_combo;
  GtkComboBox    *default_style_combo;
  GtkCheckButton *format_on_save_check;
};

G_DEFINE_TYPE_WITH_PRIVATE (GbFormatterOptionsPrefs, gb_formatter_options_prefs, GTK_TYPE_DIALOG);

static void       create_widgets       (GbFormatterOptionsPrefs *self);

static void       connect_signals      (GbFormatterOptionsPrefs *self);

static void       load_settings        (GbFormatterOptionsPrefs *self);

static void       on_check_box_toggled (GtkToggleButton         *sender,
                                        gpointer                 user_data);

static void       on_combo_box_changed (GtkComboBox             *sender,
                                        gpointer                 user_data);

static void       set_combo_box_value  (GtkComboBox             *combo_box,
                                        const gchar             *value);

GbFormatterOptionsPrefs *
gb_formatter_options_prefs_new (void)
{
  GbFormatterOptionsPrefs *self;

  self = g_object_new (GB_TYPE_FORMATTER_OPTIONS_PREFS,
                       "use-header-bar", TRUE,
                       NULL);

  self->priv = gb_formatter_options_prefs_get_instance_private (self);

  create_widgets (self);
  connect_signals (self);
  load_settings (self);

  return self;
}

static void
gb_formatter_options_prefs_init (GbFormatterOptionsPrefs *self)
{
}

static void
gb_formatter_options_prefs_class_init (GbFormatterOptionsPrefsClass *self)
{
}

static void
create_widgets (GbFormatterOptionsPrefs *self)
{
  gint    i;
  GtkBox *content_area;

  GtkBox              *h_box1;
  GtkHeaderBar        *header_bar;
  GtkLabel            *format_style_label;
  GtkTreeIter          iter_style;
  GtkListStore        *model_style;
  GtkCellRendererText *renderer_style;

  GtkBox              *h_box2;
  GtkLabel            *format_default_style_label;
  GtkTreeIter          iter_default_style;
  GtkListStore        *model_default_style;
  GtkCellRendererText *renderer_default_style;


  header_bar = GTK_HEADER_BAR (gtk_dialog_get_header_bar (GTK_DIALOG (self)));
  gtk_header_bar_set_title (header_bar, g_strconcat ("ClangFormat ", "Preferences", NULL));
  gtk_header_bar_set_show_close_button (header_bar, TRUE);


  format_style_label = GTK_LABEL (gtk_label_new ("Format style"));
  gtk_widget_set_halign (GTK_WIDGET (format_style_label), GTK_ALIGN_START);

  self->priv->style_combo = GTK_COMBO_BOX (gtk_combo_box_new ());
  model_style = gtk_list_store_new (1, G_TYPE_STRING);
  gtk_combo_box_set_model (self->priv->style_combo, GTK_TREE_MODEL (model_style));

  for (i = 0; i < N_FORMATTER_STYLES; i++)
    {
      gtk_list_store_append (model_style, &iter_style);
      gtk_list_store_set (model_style, &iter_style, 0, formatter_styles[i], -1);
    }

  renderer_style = GTK_CELL_RENDERER_TEXT (gtk_cell_renderer_text_new ());

  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self->priv->style_combo),
                              GTK_CELL_RENDERER (renderer_style),
                              TRUE);

  gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (self->priv->style_combo),
                                 GTK_CELL_RENDERER (renderer_style),
                                 "text", 0);


  format_default_style_label = GTK_LABEL (gtk_label_new ("Fallback style"));
  gtk_widget_set_halign (GTK_WIDGET (format_default_style_label), GTK_ALIGN_START);

  self->priv->default_style_combo = GTK_COMBO_BOX (gtk_combo_box_new ());
  model_default_style = gtk_list_store_new (1, G_TYPE_STRING);
  gtk_combo_box_set_model (self->priv->default_style_combo, GTK_TREE_MODEL (model_default_style));

  for (i = 1; i < N_FORMATTER_STYLES; i++)
    {
      gtk_list_store_append (model_default_style, &iter_default_style);
      gtk_list_store_set (model_default_style, &iter_default_style, 0, formatter_styles[i], -1);
    }

  renderer_default_style = GTK_CELL_RENDERER_TEXT (gtk_cell_renderer_text_new ());

  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self->priv->default_style_combo),
                              GTK_CELL_RENDERER (renderer_default_style),
                              TRUE);

  gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (self->priv->default_style_combo),
                                 GTK_CELL_RENDERER (renderer_default_style),
                                 "text", 0);


  self->priv->format_on_save_check = GTK_CHECK_BUTTON (
      gtk_check_button_new_with_label (" Format file on save")
  );


  h_box1 = GTK_BOX (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10));

  gtk_box_pack_start (h_box1,
                      GTK_WIDGET (format_style_label),
                      FALSE, TRUE,
                      0);

  gtk_box_pack_start (h_box1,
                      GTK_WIDGET (self->priv->style_combo),
                      TRUE, TRUE,
                      0);


  h_box2 = GTK_BOX (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10));

  gtk_box_pack_start (h_box2,
                      GTK_WIDGET (format_default_style_label),
                      FALSE, TRUE,
                      0);

  gtk_box_pack_start (h_box2,
                      GTK_WIDGET (self->priv->default_style_combo),
                      TRUE, TRUE,
                      0);


  content_area = GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (self)));
  gtk_box_set_spacing (content_area, 10);

  gtk_box_pack_start (content_area,
                      GTK_WIDGET (h_box1),
                      FALSE, TRUE,
                      0);

  gtk_box_pack_start (content_area,
                      GTK_WIDGET (h_box2),
                      FALSE, TRUE,
                      0);

  gtk_box_pack_start (content_area,
                      GTK_WIDGET (self->priv->format_on_save_check),
                      FALSE, TRUE,
                      0);


  gtk_window_set_default_size (GTK_WINDOW (self), 300, 100);
  gtk_container_set_border_width (GTK_CONTAINER (content_area), 10);

  gtk_widget_show_all (GTK_WIDGET (self));
}

static void
connect_signals (GbFormatterOptionsPrefs *self)
{
  g_signal_connect (self->priv->format_on_save_check, "toggled",
                    G_CALLBACK (on_check_box_toggled),
                    NULL);

  g_signal_connect (self->priv->style_combo, "changed",
                    G_CALLBACK (on_combo_box_changed),
                    "format-style");

  g_signal_connect (self->priv->default_style_combo, "changed",
                    G_CALLBACK (on_combo_box_changed),
                    "format-default-style");
}

static void
load_settings (GbFormatterOptionsPrefs *self)
{
  GSettings *settings;
  gchar     *format_style;
  gchar     *default_format_style;
  gboolean   format_on_save;

  settings = gb_formatter_options_get_settings ();

  format_style = g_settings_get_string (settings, "format-style");
  set_combo_box_value (self->priv->style_combo, format_style);

  default_format_style = g_settings_get_string (settings, "format-default-style");
  set_combo_box_value (self->priv->default_style_combo,
                                              default_format_style);

  format_on_save = g_settings_get_boolean (settings, "format-on-save");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (self->priv->format_on_save_check),
                                format_on_save);

  g_object_unref (settings);
}

static void
set_combo_box_value (GtkComboBox *combo_box,
                     const gchar *value)
{
  GtkTreeIter   iter;
  GtkTreeModel *model;
  gboolean      valid;

  model = gtk_combo_box_get_model (combo_box);
  valid = gtk_tree_model_get_iter_first (model, &iter);

  while (valid)
    {
      gchar *str_value;

      gtk_tree_model_get (model, &iter, 0, &str_value, -1);

      if (g_strcmp0 (str_value, value) == 0)
        break;

      valid = gtk_tree_model_iter_next (model, &iter);
    }

  if (!valid)
    valid = gtk_tree_model_get_iter_first (model, &iter);

  gtk_combo_box_set_active_iter (combo_box, &iter);
}

static void
on_check_box_toggled (GtkToggleButton *sender,
                      gpointer         user_data)
{
  GSettings *settings;
  gboolean   value;

  settings = gb_formatter_options_get_settings ();
  value = gtk_toggle_button_get_active (sender);

  g_settings_set_boolean (settings, "format-on-save", value);
  g_object_unref (settings);
}

static void
on_combo_box_changed (GtkComboBox *sender,
                      gpointer     user_data)
{
  GSettings    *settings;
  GtkTreeIter   iter;
  GtkTreeModel *model;
  gchar        *value;


  settings = gb_formatter_options_get_settings ();

  model = gtk_combo_box_get_model (sender);
  gtk_combo_box_get_active_iter (sender, &iter);

  gtk_tree_model_get (model, &iter, 0, &value, -1);

  g_settings_set_string (settings, (const gchar *) user_data, value);
  g_object_unref (settings);
}
