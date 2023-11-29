/* GStreamer
 * Copyright (C) 2023 FIXME <emiel@FIXME.no>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstunzip
 *
 * The unzip element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! unzip ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstunzip.h"
#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>
#include <memory>
#include <zlib.h>

GST_DEBUG_CATEGORY_STATIC(gst_unzip_debug_category);
#define GST_CAT_DEFAULT gst_unzip_debug_category

/* prototypes */

static void gst_unzip_set_property(GObject* object, guint property_id, const GValue* value, GParamSpec* pspec);
static void gst_unzip_get_property(GObject* object, guint property_id, GValue* value, GParamSpec* pspec);
static void gst_unzip_dispose(GObject* object);
static void gst_unzip_finalize(GObject* object);
static GstFlowReturn gst_unzip_transform_ip(GstBaseTransform* trans, GstBuffer* buf);

enum
{
    PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_unzip_src_template =
    GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_CAPS_ANY);

static GstStaticPadTemplate gst_unzip_sink_template =
    GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK, GST_PAD_ALWAYS, GST_CAPS_ANY);

/* class initialization */

G_DEFINE_TYPE_WITH_CODE(
    GstUnzip,
    gst_unzip,
    GST_TYPE_BASE_TRANSFORM,
    GST_DEBUG_CATEGORY_INIT(gst_unzip_debug_category, "unzip", 0, "debug category for unzip element"));

static void gst_unzip_class_init(GstUnzipClass* klass)
{
    GObjectClass* gobject_class                 = G_OBJECT_CLASS(klass);
    GstBaseTransformClass* base_transform_class = GST_BASE_TRANSFORM_CLASS(klass);

    /* Setting up pads and setting metadata should be moved to
       base_class_init if you intend to subclass this class. */
    gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass), &gst_unzip_src_template);
    gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass), &gst_unzip_sink_template);

    gst_element_class_set_static_metadata(
        GST_ELEMENT_CLASS(klass), "FIXME Long name", "Generic", "FIXME Description", "FIXME <emiel@.no>");

    gobject_class->set_property = gst_unzip_set_property;
    gobject_class->get_property = gst_unzip_get_property;
    gobject_class->dispose      = gst_unzip_dispose;
    gobject_class->finalize     = gst_unzip_finalize;

    base_transform_class->transform_ip = GST_DEBUG_FUNCPTR(gst_unzip_transform_ip);
}

static void gst_unzip_init(GstUnzip* unzip)
{
}

void gst_unzip_set_property(GObject* object, guint property_id, const GValue* value, GParamSpec* pspec)
{
    GstUnzip* unzip = GST_UNZIP(object);

    GST_DEBUG_OBJECT(unzip, "set_property");

    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

void gst_unzip_get_property(GObject* object, guint property_id, GValue* value, GParamSpec* pspec)
{
    GstUnzip* unzip = GST_UNZIP(object);

    GST_DEBUG_OBJECT(unzip, "get_property");

    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

void gst_unzip_dispose(GObject* object)
{
    GstUnzip* unzip = GST_UNZIP(object);

    GST_DEBUG_OBJECT(unzip, "dispose");

    /* clean up as possible.  may be called multiple times */

    G_OBJECT_CLASS(gst_unzip_parent_class)->dispose(object);
}

void gst_unzip_finalize(GObject* object)
{
    GstUnzip* unzip = GST_UNZIP(object);

    GST_DEBUG_OBJECT(unzip, "finalize");

    /* clean up object here */

    G_OBJECT_CLASS(gst_unzip_parent_class)->finalize(object);
}

static GstFlowReturn gst_unzip_transform_ip(GstBaseTransform* trans, GstBuffer* inbuf)
{
    GstUnzip* unzip = GST_UNZIP(trans);

    GST_DEBUG_OBJECT(unzip, "transform_ip");
    GstMapInfo in, out;
    // compress the data
    gst_buffer_map(inbuf, &in, GST_MAP_READ);
    uint32_t size32 = *reinterpret_cast<uint32_t*>(in.data);
    size_t o_size   = size32;
    guint8* p       = in.data + sizeof(uint32_t);
    size_t len      = in.size - sizeof(uint32_t);
    auto buf        = std::make_unique<unsigned char[]>(o_size);
    int err         = uncompress(buf.get(), &o_size, p, len);
    gst_buffer_unmap(inbuf, &in);

    gst_buffer_resize(inbuf, 0, o_size);

    gst_buffer_fill(inbuf, 0, buf.get(), o_size);

    return GST_FLOW_OK;
}