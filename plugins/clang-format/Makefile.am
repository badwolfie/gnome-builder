if ENABLE_CLANG_FORMAT_PLUGIN

EXTRA_DIST = $(plugin_DATA)

plugindir = $(libdir)/gnome-builder/plugins
plugin_LTLIBRARIES = libclang-format-plugin.la
dist_plugin_DATA = clang-format.plugin

libclang_format_plugin_la_SOURCES =		\
	clang-format-plugin.c				\
	gb-clang-format-workbench-addin.c	\
	gb-clang-format-workbench-addin.h	\
	gb-formatter.c						\
	gb-formatter.h						\
	gb-formatter-options.c				\
	gb-formatter-options.h				\
	gb-formatter-options-prefs.c		\
	gb-formatter-options-prefs.h		\
	$(NULL)

libclang_format_plugin_la_CFLAGS = $(PLUGIN_CFLAGS)
libclang_format_plugin_la_LDFLAGS = $(PLUGIN_LDFLAGS)

include $(top_srcdir)/plugins/Makefile.plugin

gsettings_SCHEMAS =														\
	gsettings/org.gnome.builder.plugins.clang_format_plugin.gschema.xml	\
	$(NULL)

.PRECIOUS: $(gsettings_SCHEMAS)

@GSETTINGS_RULES@

endif

-include $(top_srcdir)/git.mk

