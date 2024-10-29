/*
 * haya_topic_echo.cpp
 *
 * License: BSD-3-Clause
 *
 * Copyright (c) 2012-2023 Shoun Corporation <research.robosho@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of RT Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
#include "rmw/qos_profiles.h"
#include "haya_imu_msgs/msg/imu_data.hpp"
#include "sensor_msgs/msg/imu.hpp"

using std::placeholders::_1;

class HayaTopicEcho : public rclcpp::Node {
public:
    HayaTopicEcho(): Node("haya_topic_echo") {
        std::string message_type_;

        // Declare parameters, and takes default value
        declare_parameter("message_type", "haya");

        // Get parameters from config file, otherwise takes default value
        message_type_ = get_parameter("message_type").as_string();

        // Create the sensor QoS profile for subscriber
        rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
        qos_profile.depth = 1;
        auto qos = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, qos_profile.depth), qos_profile);

        if(message_type_ == "haya") {// haya_imu message type
            // Create the synchronous subscriber on topic '/imu_data' and tie it to the topic_callback
            haya_subscription_ = this->create_subscription<haya_imu_msgs::msg::ImuData>("/imu_data", qos, std::bind(&HayaTopicEcho::topic_callback, this, _1));                          
        }
        else {// ros2 message type
            // Create the synchronous subscriber on topic '/imu_data' and tie it to the topic_callback_ros2
            ros2_subscription_ = this->create_subscription<sensor_msgs::msg::Imu>("/imu_data", qos, std::bind(&HayaTopicEcho::topic_callback_ros2, this, _1));                                  
        }
    }

private:
    // A subscriber that listens to topic '/imu_data'
    rclcpp::Subscription<haya_imu_msgs::msg::ImuData>::SharedPtr haya_subscription_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr ros2_subscription_;

    /**
     * Actions to run every time a new message is received
     */
    void topic_callback(const haya_imu_msgs::msg::ImuData & imu_msg) {
        RCLCPP_INFO_STREAM(get_logger(), haya_imu_msgs::msg::to_yaml(imu_msg));  
    }

    void topic_callback_ros2(const sensor_msgs::msg::Imu & imu_msg) {
        RCLCPP_INFO_STREAM(get_logger(), sensor_msgs::msg::to_yaml(imu_msg));  
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HayaTopicEcho>());
    rclcpp::shutdown();
    return 0;
}
// haya_topic_echo.cpp