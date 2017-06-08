#ifndef TANKCHASSIS_H_
#define TANKCHASSIS_H_

#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

#include <iostream>
#include "../../tank_chassis/src/Motorcontroller.h"

using namespace std;

/**
 * @class Main class for low-cost robot platform (DAGU metal tank)
 * @author Stefan May
 * @date 15.05.2014
 */
class TankChassis
{
public:

  /**
   * Standard Constructor
   */
  TankChassis();

  /**
   * ROS main loop (blocking method)
   */
  void run();

private:

  /**
   * Normalize velocity, i.e., scale values, if one of both exceeds vMax
   */
  void normalizeVelocity(double &vl, double &vr);

  /**
   * 2D Motion model: computes tracks velocity based on linear and angular velocity
   */
  void twistToTrackspeed(double *vl, double *vr, double v, double omega) const;

  /**
   * 2D Motion model: computes linear and angular velocity based on tracks velocity
   */
  void trackspeedToTwist(double vl, double vr, double *v, double *omega) const;

  /**
   * Translates the metric speed v into the needed motor RPM
   */
  double trackspeedToRPM(double v) const;

  /**
   * ROS joystick callback
   * @param joy message with joystick command
   */
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  /**
   * ROS command velocity callback
   * @param cmd message with velocity command
   */
  void velocityCallback(const geometry_msgs::Twist::ConstPtr& cmd);

  ros::NodeHandle _nh;
  ros::Subscriber _joySub;
  ros::Subscriber _velSub;

  Motorcontroller _motor;

  double _vl, _vr;

  // maximum velocity [m/s]
  double _vMax;

  // distance of tracks
  double _track;

  // circumference of pinion
  double _pinionCircumference;

  // time elapsed since last call
  ros::Time _lastCmd;
};

#endif