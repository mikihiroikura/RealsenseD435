#pragma once
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include <string>

using namespace std;

class realsense
{
private:
	cv::Mat colorimg;
	cv::Mat depthimg;
	rs2::frame colorframe;
	rs2::frame depthframe;
	rs2::pipeline pipe;
	rs2::frameset frames;
	string serial_num;

	//Color img
	unsigned int color_width = 1920;
	unsigned int color_height = 1080;
	unsigned int color_fps = 30;

	//Depth img
	unsigned int depth_width = 1280;
	unsigned int depth_height = 720;
	unsigned int depth_fps = 30;

public:
	realsense(string serial_num);
	void update_frame();
	void update_color_img();
	void update_depth_img();
};

