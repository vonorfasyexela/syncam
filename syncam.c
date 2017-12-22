#include <gst/gst.h>
#include <glib/gprintf.h>

#define FPS         5
#define WIDTH       640
#define HEIGHT      480

int main (int argc, char *argv[]) {

	GstElement *pipeline, *src_a, *rate_a, *encoder_a, *sink_a;
	GstElement *src_b, *rate_b, *encoder_b, *sink_b;
	GstCaps *caps_a, *ratecaps_a;
	GstCaps *caps_b, *ratecaps_b;
	GstBus *bus;

	// init GStreamer
	gst_init(NULL, NULL);

	// create elements
	pipeline = gst_pipeline_new("pipe");
	src_a = gst_element_factory_make("v4l2src", "src_a");
	rate_a = gst_element_factory_make("videorate", "rate_a");
	encoder_a = gst_element_factory_make("jpegenc", "encoder_a");
	sink_a = gst_element_factory_make("multifilesink", "sink_a");

	src_b = gst_element_factory_make("v4l2src", "src_b");
	rate_b = gst_element_factory_make("videorate", "rate_b");
	encoder_b = gst_element_factory_make("jpegenc", "encoder_b");
	sink_b = gst_element_factory_make("multifilesink", "sink_b");

	if (!pipeline || !src_a || !rate_a || !encoder_a || !sink_a) {
			g_printerr("Cannot create elements\n");
			return -1;
	}
	if (!pipeline || !src_b || !rate_b || !encoder_b || !sink_b) {
			g_printerr("Cannot create elements\n");
			return -1;
	}

	// put elements into pipeline
	gst_bin_add_many(GST_BIN(pipeline), src_a, rate_a, encoder_a, sink_a, NULL);
	gst_bin_add_many(GST_BIN(pipeline), src_b, rate_b, encoder_b, sink_b, NULL);

	// make caps
	caps_a = gst_caps_new_simple("video/x-raw", 
							   "width", G_TYPE_INT, WIDTH,
							   "height", G_TYPE_INT, HEIGHT,
							   NULL );
	ratecaps_a = gst_caps_new_simple("video/x-raw",
									"framerate", GST_TYPE_FRACTION, FPS, 1,
									NULL);

	caps_b = gst_caps_new_simple("video/x-raw", 
							   "width", G_TYPE_INT, WIDTH,
							   "height", G_TYPE_INT, HEIGHT,
							   NULL );
	ratecaps_b = gst_caps_new_simple("video/x-raw",
									"framerate", GST_TYPE_FRACTION, FPS, 1,
									NULL);

	// and link all that we can
	if (gst_element_link_many( encoder_a, sink_a, NULL) != 1) {
			g_printerr("Cannot link elements\n");
			gst_object_unref(pipeline);
			return -1;
	}
	if (gst_element_link_filtered(src_a, rate_a, caps_a) != 1) {
			g_printerr("Cannot link elements\n");
			gst_object_unref(pipeline);
			return -1;
	}
	if (gst_element_link_filtered(rate_a, encoder_a, ratecaps_a) != 1) {
			g_printerr("Cannot link elements\n");
			gst_object_unref(pipeline);
			return -1;
	}


	if (gst_element_link_many( encoder_b, sink_b, NULL) != 1) {
			g_printerr("Cannot link elements\n");
			gst_object_unref(pipeline);
			return -1;
	}
	if (gst_element_link_filtered(src_b, rate_b, caps_b) != 1) {
			g_printerr("Cannot link elements\n");
			gst_object_unref(pipeline);
			return -1;
	}
	if (gst_element_link_filtered(rate_b, encoder_b, ratecaps_b) != 1) {
			g_printerr("Cannot link elements\n");
			gst_object_unref(pipeline);
			return -1;
	}

	// setting elements
	g_object_set(sink_a, "location", "shot%d_a.jpg", NULL);
	g_object_set(src_a, "device", "/dev/video0", "num-buffers", 5, NULL);	
	g_object_set(sink_b, "location", "shot%d_b.jpg", NULL);
	g_object_set(src_b, "device", "/dev/video1", "num-buffers", 5, NULL);	
	int index = 0;

	// take a bus
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

	g_print("Action! Press Ctrl+C to stop...");

	while (1) {

		// play pipeline and wait for messages
		gst_element_set_state(pipeline, GST_STATE_PLAYING);
		gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);
		
		// make pipeline NULL
		gst_element_set_state(pipeline, GST_STATE_NULL);
		index += FPS;
		g_object_set(sink_a, "index", index, NULL);
		g_object_set(sink_b, "index", index, NULL);
	}

	// unref
	gst_object_unref(pipeline);
	gst_object_unref(bus);
	gst_caps_unref(caps_a);
	gst_caps_unref(ratecaps_a);
	gst_caps_unref(caps_b);
	gst_caps_unref(ratecaps_b);
	return 0;
}
