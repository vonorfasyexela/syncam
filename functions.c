// Copyright (c) 2016 Alexey Safronov

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

/****************************************************************************/
/*                                INCLUDES                                  */
/****************************************************************************/
#include "syncam.h"

/****************************************************************************/
/*                               FUNCTIONS                                  */
/****************************************************************************/
bool create_gst_elements(s_appdata* appdata)
{
    appdata->pipeline = gst_pipeline_new("pipe");

    appdata->src_a = gst_element_factory_make("v4l2src", "src_a");
    appdata->rate_a = gst_element_factory_make("videorate", "rate_a");
    appdata->encoder_a = gst_element_factory_make("jpegenc", "encoder_a");
    appdata->sink_a = gst_element_factory_make("multifilesink", "sink_a");

    appdata->src_b = gst_element_factory_make("v4l2src", "src_b");
    appdata->rate_b = gst_element_factory_make("videorate", "rate_b");
    appdata->encoder_b = gst_element_factory_make("jpegenc", "encoder_b");
    appdata->sink_b = gst_element_factory_make("multifilesink", "sink_b");

    if (!appdata->pipeline ||
        !appdata->src_a ||
        !appdata->rate_a ||
        !appdata->encoder_a ||
        !appdata->sink_a ||
        !appdata->src_b ||
        !appdata->rate_b ||
        !appdata->encoder_b ||
        !appdata->sink_b)
    {
        return false;
    }
    return true;
}

void create_gst_caps(s_appdata* appdata)
{
    appdata->caps_a = gst_caps_new_simple("video/x-raw",

                                          "width",
                                          G_TYPE_INT,
                                          WIDTH,

                                          "height",
                                          G_TYPE_INT,
                                          HEIGHT,

                                          NULL);

    appdata->ratecaps_a = gst_caps_new_simple("video/x-raw",

                                              "framerate",
                                              GST_TYPE_FRACTION,
                                              FPS,
                                              1,

                                              NULL);

    appdata->caps_b = gst_caps_new_simple("video/x-raw",

                                          "width",
                                          G_TYPE_INT,
                                          WIDTH,

                                          "height",
                                          G_TYPE_INT,
                                          HEIGHT,

                                          NULL);

    appdata->ratecaps_b = gst_caps_new_simple("video/x-raw",

                                              "framerate",
                                              GST_TYPE_FRACTION,
                                              FPS,
                                              1,

                                              NULL);
}

bool link_gst_elements(s_appdata* appdata)
{
    gst_bin_add_many(GST_BIN(appdata->pipeline),
                     appdata->src_a,
                     appdata->rate_a,
                     appdata->encoder_a,
                     appdata->sink_a,
                     appdata->src_b,
                     appdata->rate_b,
                     appdata->encoder_b,
                     appdata->sink_b,
                     NULL);

    if (gst_element_link_many(appdata->encoder_a, appdata->sink_a, NULL) != 1)
    {
        return false;
    }
    if (gst_element_link_filtered(appdata->src_a,
                                  appdata->rate_a,
                                  appdata->caps_a) != 1)
    {
        return false;
    }
    if (gst_element_link_filtered(appdata->rate_a,
                                  appdata->encoder_a,
                                  appdata->ratecaps_a) != 1)
    {
        return false;
    }
    if (gst_element_link_many(appdata->encoder_b, appdata->sink_b, NULL) != 1)
    {
        return false;
    }
    if (gst_element_link_filtered(appdata->src_b,
                                  appdata->rate_b,
                                  appdata->caps_b) != 1)
    {
        return false;
    }
    if (gst_element_link_filtered(appdata->rate_b,
                                  appdata->encoder_b,
                                  appdata->ratecaps_b) != 1)
    {
        return false;
    }
    return true;
}

void tune_gst_elements(s_appdata* appdata)
{
    g_object_set(appdata->sink_a, "location", FILENAME_TEMPLATE_A, NULL);
    g_object_set(appdata->src_a,
                 "device",
                 "/dev/video0",
                 "num-buffers",
                 NUM_BUFFERS,
                 NULL);
    g_object_set(appdata->sink_b, "location", FILENAME_TEMPLATE_B, NULL);
    g_object_set(appdata->src_b,
                 "device",
                 "/dev/video1",
                 "num-buffers",
                 NUM_BUFFERS,
                 NULL);
}

void unref_gst_objects(s_appdata* appdata)
{
    gst_object_unref(appdata->pipeline);
    gst_object_unref(appdata->bus);
    gst_caps_unref(appdata->caps_a);
    gst_caps_unref(appdata->ratecaps_a);
    gst_caps_unref(appdata->caps_b);
    gst_caps_unref(appdata->ratecaps_b);
}