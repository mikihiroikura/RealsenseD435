#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
	rs2::pipeline pipe;
	rs2::config config;

	config.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30);
    config.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);

	pipe.start(config);

    rs2::colorizer color_map;

    rs2::frameset frames;
    for (int i = 0; i < 30; i++)
    {
        //Wait for all configured streams to produce a frame
        frames = pipe.wait_for_frames();
    }


    while (true)
    {
        frames = pipe.wait_for_frames();

        //Get each frame
        rs2::frame color_frame = frames.get_color_frame();
        rs2::frame depth_frame = (frames.get_depth_frame()).apply_filter(color_map);

        // Creating OpenCV Matrix from a color image
        cv::Mat color(cv::Size(1920, 1080), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
        cv::Mat depth(cv::Size(1280, 720), CV_8UC3, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);

        // Display in a GUI
        cv::namedWindow("Display Color Image", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Display Depth Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Display Color Image", color);
        cv::imshow("Display Depth Image", depth);

        int key = cv::waitKey(10);
        if (key == 'q')break;
    }


	return 0;
}