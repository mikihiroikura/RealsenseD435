// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"          // Include short list of convenience functions for rendering

#include <algorithm>            // std::min, std::max
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>

using namespace std;


using namespace std;

// Helper functions
void register_glfw_callbacks(window& app, glfw_state& app_state);

int main(int argc, char* argv[]) try
{
    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "RealSense Pointcloud Example");
    // Construct an object to manage view state
    glfw_state app_state;
    // register callbacks to allow manipulation of the pointcloud
    register_glfw_callbacks(app, app_state);

    // Declare pointcloud object, for calculating pointclouds and texture mappings
    rs2::pointcloud pc;
    // We want the points object to be persistent so we can display the last cloud when a frame drops
    rs2::points points;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();
    rs2::frameset frames;
    for (int i = 0; i < 30; i++)
    {
        //Wait for all configured streams to produce a frame
        frames = pipe.wait_for_frames();
    }

    while (app) // Application still alive?
    {
        // Wait for the next set of frames from the camera
        auto frames = pipe.wait_for_frames();

        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Tell pointcloud object to map to this color frame
        pc.map_to(color);

        auto depth = frames.get_depth_frame();

        // Generate the pointcloud and texture mappings
        points = pc.calculate(depth);

        // Upload the color frame to OpenGL
        app_state.tex.upload(color);

        //‰ñ“]“®ì‚ð‰Á‚¦‚Ä‚Ý‚é
        app_state.pitch += 1;

        // Draw the pointcloud
        draw_pointcloud(app.width(), app.height(), app_state, points);

        //‰æ‘œ‚ào—Í
    //    rs2::frame color_frame = frames.get_color_frame();
    //    cv::Mat color_img(cv::Size(1920, 1080), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
    //    //cv::cvtColor(color_img, color_img, cv::COLOR_RGB2BGR);
    //    cv::namedWindow("Color img", cv::WINDOW_AUTOSIZE);
    //    cv::imshow("Color img", color_img);

    //    int key = cv::waitKey(10);
    //    if (key == 'q')break;
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error& e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
