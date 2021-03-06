/* This is free and unencumbered software released into the public domain. */

#include <stdio.h>
#include <stdlib.h>
#include "bitcache.h"
#include "config.h"

#define PROGRAM_DESCRIPTION ""
#define PROGRAM_SUMMARY     ""

static bool debug   = FALSE;
static bool verbose = FALSE;
static bool version = FALSE;

static GOptionEntry entries[] = {
   { "debug",   'd', 0, G_OPTION_ARG_NONE, &debug,   "Enable debug output for troubleshooting.", NULL },
   { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Enable verbose output. May be given more than once.", NULL },
   { "version", 'V', 0, G_OPTION_ARG_NONE, &version, "Display the Bitcache version and exit.", NULL },
   { NULL },
};

int main(int argc, char* argv[]) {
  GError* error = NULL;
  GOptionContext* context = g_option_context_new(PROGRAM_SUMMARY);
  g_option_context_add_main_entries(context, entries, NULL);
  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    fprintf(stderr, "%s: %s\n", g_get_prgname(), error->message);
    return 1;
  }

  if (version) {
    // Display the Bitcache version and exit:
    printf("%s\n", PACKAGE_VERSION);
    return 0;
  }

  return 0;
}
