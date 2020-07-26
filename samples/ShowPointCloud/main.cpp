#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

int main() {

	rs2::pointcloud pc;
	rs2::points points;

	rs2::pipeline pipe;
	rs2::config config;
	rs2::frameset frames;

	config.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_BGR8, 30);
	config.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);

	pipe.start(config);

	//‰‚ß‚ÌˆÀ’è‚µ‚Ä‚¢‚È‚¢‚ÍÈ—ª‚·‚é
	for (int i = 0; i < 30; i++)
	{
		//Wait for all configured streams to produce a frame
		frames = pipe.wait_for_frames();
	}

	while (true)
	{
		frames = pipe.wait_for_frames();

		rs2::frame color_frame = frames.get_color_frame();
		cv::Mat color(cv::Size(1920, 1080), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);

		pc.map_to(color_frame);
		rs2::frame depth_frame = frames.get_depth_frame();
		points = pc.calculate(depth_frame);


		cv::imshow("Display Color Image", color);
		
		int key = cv::waitKey(10);
		if (key == 'q')break;
	}

	return 0;
}