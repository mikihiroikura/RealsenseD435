#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>


using namespace std;

struct Realsense
{
	cv::Mat color;
	cv::Mat depth;
	rs2::frame colorframe;
	rs2::frame depthframe;
	rs2::pipeline pipe;
	rs2::config config;
	rs2::frameset frames;
};

int main() {
	rs2::context context;
	vector<Realsense> rs_devices;

	try
	{
		
		const rs2::device_list device_list = context.query_devices();
		for (rs2::device device : device_list)
		{
			Realsense rs;
			cout << device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) << endl;
			rs.config.enable_device(device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
			rs.config.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30);
			rs.config.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);

			rs.pipe.start(rs.config);

			rs_devices.push_back(rs);
		}
		rs2::colorizer color_map;

		for (int i = 0; i < 30; i++)
		{
			for (Realsense rs: rs_devices)
			{
				//Wait for all configured streams to produce a frame
				rs.frames = rs.pipe.wait_for_frames();
			}
		}

		while (true)
		{
			for (int i=0; i<rs_devices.size(); i++)
			{
				rs_devices[i].frames = rs_devices[i].pipe.wait_for_frames();
				rs_devices[i].colorframe = rs_devices[i].frames.get_color_frame();
				rs_devices[i].depthframe = (rs_devices[i].frames.get_depth_frame()).apply_filter(color_map);

				// Creating OpenCV Matrix from a color image
				rs_devices[i].color = cv::Mat(cv::Size(1920, 1080), CV_8UC3, (void*)rs_devices[i].colorframe.get_data(), cv::Mat::AUTO_STEP);
				rs_devices[i].depth = cv::Mat(cv::Size(1280, 720), CV_8UC3, (void*)rs_devices[i].depthframe.get_data(), cv::Mat::AUTO_STEP);
				
				// Display in a GUI
				string colorwinname = "Display Color Image " + to_string(i);
				string depthwinname = "Display Depth Image " + to_string(i);

				cv::namedWindow(colorwinname, cv::WINDOW_AUTOSIZE);
				cv::namedWindow(depthwinname, cv::WINDOW_AUTOSIZE);
				cv::imshow(colorwinname, rs_devices[i].color);
				cv::imshow(depthwinname, rs_devices[i].depth);

				
			}
			int key = cv::waitKey(10);
			if (key == 'q') break;
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}