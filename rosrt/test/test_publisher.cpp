/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2010, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

#include <gtest/gtest.h>

#include "rosrt/rosrt.h"

#include <ros/ros.h>

#include <std_msgs/UInt32.h>

#include <boost/thread.hpp>
#include <rosrt/detail/thread.h>

#ifdef __XENO__
#include <native/task.h>
#include <sys/mman.h>
#endif

using namespace rosrt;

struct Helper
{
  Helper()
  : count(0)
  {}

  void cb(const std_msgs::UInt32ConstPtr& msg)
  {
    latest = msg;
    ++count;
  }

  std_msgs::UInt32ConstPtr latest;
  uint32_t count;
};

TEST(Publisher, singlePublisher)
{
  ros::NodeHandle nh;

  Publisher<std_msgs::UInt32> pub(nh.advertise<std_msgs::UInt32>("test", 0), 1, std_msgs::UInt32());

  Helper h;
  ros::Subscriber sub = nh.subscribe("test", 0, &Helper::cb, &h);

  resetThreadAllocInfo();

  std_msgs::UInt32Ptr msg = pub.allocate();
  msg->data = 5;
  pub.publish(msg);

  ASSERT_EQ(getThreadAllocInfo().total_ops, 0ULL);

  while (h.count == 0)
  {
    ros::WallDuration(0.001).sleep();
    ros::spinOnce();
  }

  ASSERT_EQ(h.count, 1UL);
  ASSERT_EQ(h.latest->data, 5UL);
}

TEST(Publisher, simpleInitializeCompile)
{
  ros::NodeHandle nh;
  Publisher<std_msgs::UInt32> pub(nh, "test", 0, 1, std_msgs::UInt32());
}

TEST(Publisher, multiplePublishers)
{
  ros::NodeHandle nh;

  static const uint32_t count = 100;
  Publisher<std_msgs::UInt32> pubs[count];

  Helper helpers[count];
  ros::Subscriber subs[count];

  for (uint32_t i = 0; i < count; ++i)
  {
    std::stringstream topic;
    topic << "test" << i;
    pubs[i].initialize(nh.advertise<std_msgs::UInt32>(topic.str(), 0), 100, std_msgs::UInt32());
    subs[i] = nh.subscribe(topic.str(), 0, &Helper::cb, &helpers[i]);
  }

  for (uint32_t j = 0; j < 100; ++j)
  {
    for (uint32_t i = 0; i < count; ++i)
    {
      std_msgs::UInt32Ptr msg = pubs[i].allocate();
      ASSERT_TRUE(msg);
      msg->data = j;
      ASSERT_TRUE(pubs[i].publish(msg));
    }
  }

  uint32_t recv_count = 0;
  while (recv_count < count * 100)
  {
    ros::spinOnce();
    ros::WallDuration(0.01).sleep();

    recv_count = 0;

    for (uint32_t i = 0; i < count; ++i)
    {
      recv_count += helpers[i].count;
    }
  }

  ASSERT_EQ(recv_count, count * 100);

  for (uint32_t i = 0; i < count; ++i)
  {
    ASSERT_EQ(helpers[i].latest->data, 99UL);
  }
}

void publishThread(Publisher<std_msgs::UInt32>& pub, bool& done)
{
  while (!done)
  {
    std_msgs::UInt32Ptr msg = pub.allocate();
    if (msg)
    {
      pub.publish(msg);
#ifdef __XENO__
      rt_task_yield();
#endif
    }
    else
    {
#ifdef __XENO__
      rt_task_yield();
      rt_task_sleep(1000000);
#else
      ros::WallDuration(0.0001).sleep();
#endif
    }
  }
}

TEST(Publisher, multipleThreads)
{
  ros::NodeHandle nh;

  static const uint32_t count = 10;
  Publisher<std_msgs::UInt32> pubs[count];

  Helper helpers[count];
  ros::Subscriber subs[count];

  boost::shared_ptr<rosrt::thread> threads[count];

  bool done = false;
  for (uint32_t i = 0; i < count; ++i)
  {
    std::stringstream topic;
    topic << "test" << i;
    pubs[i].initialize(nh.advertise<std_msgs::UInt32>(topic.str(), 0), 100, std_msgs::UInt32());
    subs[i] = nh.subscribe(topic.str(), 0, &Helper::cb, &helpers[i]);
    threads[i].reset(new rosrt::thread(boost::bind(publishThread, boost::ref(pubs[i]), boost::ref(done))));
  }

  uint32_t recv_count = 0;
  while (recv_count < count * 10000)
  {
    ros::spinOnce();

    recv_count = 0;

    for (uint32_t i = 0; i < count; ++i)
    {
      recv_count += helpers[i].count;
    }

#ifdef __XENO__
    rt_task_yield();
    rt_task_sleep(1000000);
#else
    ros::WallDuration(0.01).sleep();
#endif
  }

  done = true;

  for (uint32_t i=0; i<count; ++i)
    threads[i]->join();

  ASSERT_GE(recv_count, count * 10000);

  for (uint32_t i = 0; i < count; ++i)
  {
    ASSERT_TRUE(helpers[i].latest);
  }
}

int main(int argc, char** argv)
{
#ifdef __XENO__
  mlockall(MCL_CURRENT | MCL_FUTURE);
  rt_task_shadow(NULL, "test_rt_publisher", 1, 0);
#endif

  ros::init(argc, argv, "test_rt_publisher");
  testing::InitGoogleTest(&argc, argv);

  ros::NodeHandle nh;
  rosrt::init();

  return RUN_ALL_TESTS();
}
