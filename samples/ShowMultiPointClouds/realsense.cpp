#include "realsense.h"

realsense::realsense(string num) {
	rs2::config config;
	realsense::serial_num = num;
	config.enable_device(realsense::serial_num);
	config.enable_stream(RS2_STREAM_COLOR, realsense::color_width, realsense::color_height, realsense::color_fps);
	config.enable_stream(RS2_STREAM_DEPTH, realsense::depth_width, realsense::depth_height, realsense::depth_fps);

	realsense::pipe.start(config);
}

void realsense::update_frame() {
	realsense::frames = realsense::pipe.wait_for_frames();
}

void realsense::update_color_img() {
	realsense::colorframe = realsense::frames.get_color_frame();
	realsense::colorimg = cv::Mat(cv::Size(realsense::color_width, realsense::color_height), CV_8UC3, (void*)realsense::colorframe.get_data(), cv::Mat::AUTO_STEP);
}

void realsense::update_depth_img() {
	rs2::colorizer color_map;
	realsense::depthframe = (realsense::frames.get_depth_frame()).apply_filter(color_map);
	realsense::depthimg = cv::Mat(cv::Size(realsense::depth_width, realsense::depth_height), CV_8UC3, (void*)realsense::depthframe.get_data(), cv::Mat::AUTO_STEP);
}