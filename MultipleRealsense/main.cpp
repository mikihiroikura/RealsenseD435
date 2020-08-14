#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>


using namespace std;

int main() {
	rs2::context context;
	rs2::config config;
	vector<rs2::pipeline> pipes;

	try
	{
		
		const rs2::device_list device_list = context.query_devices();
		for (rs2::device device : device_list)
		{
			rs2::pipeline pipe;
			cout << device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) << endl;
			config.enable_device(device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
			config.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30);
			config.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);

			pipe.start(config);

			pipes.push_back(pipe);
		}
		rs2::colorizer color_map;

		rs2::frameset frames;
		for (int i = 0; i < 30; i++)
		{
			for (rs2::pipeline& pipe : pipes)
			{
				//Wait for all configured streams to produce a frame
				frames = pipe.wait_for_frames();
			}
		}

		while (true)
		{
			for (rs2::pipeline& pipe : pipes)
			{
				frames = pipe.wait_for_frames();
				rs2::frame colorframe = frames.get_color_frame();
				rs2::frame depthframe = (frames.get_depth_frame()).apply_filter(color_map);

				// Creating OpenCV Matrix from a color image
				cv::Mat color(cv::Size(1920, 1080), CV_8UC3, (void*)colorframe.get_data(), cv::Mat::AUTO_STEP);
				cv::Mat depth(cv::Size(1280, 720), CV_8UC3, (void*)depthframe.get_data(), cv::Mat::AUTO_STEP);

				// Display in a GUI
				cv::namedWindow("Display Color Image", cv::WINDOW_AUTOSIZE);
				cv::namedWindow("Display Depth Image", cv::WINDOW_AUTOSIZE);
				cv::imshow("Display Color Image", color);
				cv::imshow("Display Depth Image", depth);

				
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