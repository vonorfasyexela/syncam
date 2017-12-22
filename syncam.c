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
/*                                  MAIN                                    */
/****************************************************************************/
int main(int argc, char* argv[])
{
    s_appdata appdata;

    gst_init(NULL, NULL);

    if (!create_gst_elements(&appdata))
    {
        g_printerr("Cannot create elements\n");
        exit(1);
    }
    create_gst_caps(&appdata);
    link_gst_elements(&appdata);
    tune_gst_elements(&appdata);
    appdata.bus = gst_pipeline_get_bus(GST_PIPELINE(appdata.pipeline));

    g_print("Action! Press Ctrl+C to stop...");
    uint32_t index = 0;
    while (42)
    {
        // play pipeline and wait for messages
        gst_element_set_state(appdata.pipeline, GST_STATE_PLAYING);
        gst_bus_timed_pop_filtered(appdata.bus,
                                   GST_CLOCK_TIME_NONE,
                                   GST_MESSAGE_EOS | GST_MESSAGE_ERROR);

        // make pipeline NULL
        gst_element_set_state(appdata.pipeline, GST_STATE_NULL);
        index += FPS;
        g_object_set(appdata.sink_a, "index", index, NULL);
        g_object_set(appdata.sink_b, "index", index, NULL);
    }

    unref_gst_objects(&appdata);
    return 0;
}

