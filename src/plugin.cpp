#include <gst/gst.h>
#include "gstunzip.h"
#include "gstzip.h"

static gboolean plugin_init(GstPlugin* plugin)
{

    /* FIXME Remember to set the rank if it's an element that is meant
       to be autoplugged by decodebin. */

    if (!gst_element_register(plugin, "zip", GST_RANK_NONE, GST_TYPE_ZIP))
        return FALSE;
    if (!gst_element_register(plugin, "unzip", GST_RANK_NONE, GST_TYPE_UNZIP))
        return FALSE;

    return TRUE;
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.2"
#endif
#ifndef PACKAGE
#define PACKAGE "zip"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME PACKAGE
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "https://oh.no/"
#endif

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR, GST_VERSION_MINOR, zip, "zip", plugin_init,
                  VERSION, "GPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
