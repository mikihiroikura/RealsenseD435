#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"          // Include short list of convenience functions for rendering

#include <algorithm>            // std::min, std::max
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include "realsense.h"

using namespace std;


using namespace std;

// Helper functions
void register_glfw_callbacks(window& app, glfw_state& app_state);

int main() {
	vector<realsense> rs_devices;
	rs2::context context;
	try
	{
		window app(1280, 720, "ShowMultiPointCloud");
		glfw_state app_state;
		register_glfw_callbacks(app, app_state);

		const rs2::device_list device_list = context.query_devices();
		for (rs2::device device : device_list)
		{
			realsense rs(device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER), RS2_FORMAT_RGB8, RS2_FORMAT_Z16);
			rs_devices.push_back(rs);
		}

		for (size_t i = 0; i < 30; i++)
		{
			for (realsense rs : rs_devices)
			{
				rs.update_frame();
			}
		}

		while (app)
		{
			for (realsense rs : rs_devices)
			{
				rs.update_frame();
				rs.update_color_img();
				rs.update_depth_img();
				rs.calc_pointcloud();
				app_state.tex.upload(rs.colorframe);
				draw_pointcloud(app.width(), app.height(), app_state, rs.points);

				//// Display in a GUI
				//string colorwinname = "Display Color Image " + rs.serial_num;
				//string depthwinname = "Display Depth Image " + rs.serial_num;

				//cv::namedWindow(colorwinname, cv::WINDOW_AUTOSIZE);
				//cv::namedWindow(depthwinname, cv::WINDOW_AUTOSIZE);
				//cv::imshow(colorwinname, rs.colorimg);
				//cv::imshow(depthwinname, rs.depthimg);
			}
		}

	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}