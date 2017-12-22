#ifndef SYNCAM_H
#define SYNCAM_H
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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <glib/gprintf.h>

/****************************************************************************/
/*                               CONSTANTS                                  */
/****************************************************************************/
#define FPS                                                                  5
#define WIDTH                                                              640
#define HEIGHT                                                             480
#define NUM_BUFFERS                                                          5
#define FILENAME_TEMPLATE_A                                     "shot%d_a.jpg"
#define FILENAME_TEMPLATE_B                                     "shot%d_b.jpg"

/****************************************************************************/
/*                               STRUCTURES                                 */
/****************************************************************************/
typedef struct t_appdata
{
    GstElement* pipeline;

    GstElement* src_a;
    GstElement* rate_a;
    GstElement* encoder_a;
    GstElement* sink_a;

    GstElement* src_b;
    GstElement* rate_b;
    GstElement* encoder_b;
    GstElement* sink_b;

    GstCaps* caps_a;
    GstCaps* ratecaps_a;
    GstCaps* caps_b;
    GstCaps* ratecaps_b;

    GstBus* bus;
} s_appdata;

/****************************************************************************/
/*                               PROTOTYPES                                 */
/****************************************************************************/
bool create_gst_elements(s_appdata* appdata);
void create_gst_caps(s_appdata* appdata);
bool link_gst_elements(s_appdata* appdata);
void tune_gst_elements(s_appdata* appdata);
void unref_gst_objects(s_appdata* appdata);

#endif // SYNCAM_H