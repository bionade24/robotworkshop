#ifndef _MOTORCONTROLLERCAN_H_
#define _MOTORCONTROLLERCAN_H_

#include "SocketCAN.h"
#include <vector>

enum CanResponse
{
  CAN_RESPONSE_RPM,
  CAN_RESPONSE_POS
};

/**
 * @class MotorControllerCAN
 * @brief CAN interface for Evocortex Centipede motor controller.
 * @author Stefan May
 * @date 13.05.2018
 */
class MotorControllerCAN : public SocketCANObserver
{
public:
  /**
   * Constructor
   * @param[in] can SocketCAN instance
   * @param[in] channel Identifier of CAN node, i.e. channel in range [0;15]
   */
  MotorControllerCAN(SocketCAN* can, unsigned short channel);

  /**
   * Destructor
   */
  ~MotorControllerCAN();

  /**
   * Enable device
   * @return successful transmission of enable command
   */
  bool enable();

  /**
   * Disable device
   * @return successful transmission of disabling command
   */
  bool disable();

  /**
   * Configure response of motor controller (revolutions per minute or position)
   * @param[in] mode response mode
   * @return successful transmission of configure command
   */
  bool configureResponse(enum CanResponse mode);

  /**
   * Get assigned channel via constructor
   * @return channel
   */
  unsigned short getChannel();

  /**
   * Set timeout interval. The motor controller needs frequently transmitted commands.
   * If the time span between two commands is longer than this timeout interval, the device is disabled.
   * The user needs to send an enabling command again.
   * @param[in] timeoutInMillis timeout interval in milliseconds
   * @return true==successful CAN transmission
   */
  bool setTimeout(unsigned short timeoutInMillis);

  /**
   * Set gear ratio (factor between motor and wheel revolutions)
   * @param[in] gearRatio (motor rev) / (wheel rev) for motor 1 and 2
   * @return true==successful CAN transmission
   */
  bool setGearRatio(float gearRatio[2]);

  /**
   * Set number of encoder ticks per motor revolution
   * @param[in] encoderTicksPerRev encoder ticks per motor revolution for motor 1 and 2
   * @return true==successful CAN transmission
   */
  bool setEncoderTicksPerRev(float encoderTicksPerRev[2]);

  /**
   * Set scaling parameter for PWM frequency. The base frequency is 500kHz, of which one can apply a fractional amount, e.g.
   * 10 => 50kHz
   * 20 => 25kHz
   * Default is 32 => 15,625kHz
   * Important: This value can only be changed before the motor controllers gets enabled.
   * @param[in] scale denominator d of term 1/d x 500kHz
   */
  bool setFrequencyScale(unsigned short scale);

  /**
   * The PWM signal can be adjusted in the range from [-127;127] which is equal to [-100%;100%].
   * To limit the possible output, one can set a different value between [0;127],
   * which is symmetrically applied to the positive and negative area, e.g. 32 => [-25%;25%]
   * The default value is: 63 => [-50%;50%]
   */
  bool setMaxPulseWidth(unsigned char pulse);

  /**
   * Set pulse width modulated signal
   * @param[in] rpm pulse width in range [-100;100], this device supports 2 channels
   * @return success
   */
  bool setPWM(int pwm[2]);

  /**
   * Set motor revolutions per minute
   * @param[in] rpm set point value, this device supports 2 channels
   * @return success
   */
  bool setRPM(float rpm[2]);

  /**
   * Get motor revolutions per minute
   * @param[out] rpm revolutions per minute for motor 1 and 2
   */
  void getRPM(float rpm[2]);

  /**
   * Get motor position (encoder ticks).
   * @param[out] pos position of motor 1 and 2. This is a modulo 2^15 value.
   */
  void getPos(short pos[2]);

  /**
   * Set proportional factor of PID controller
   * @param[in] kp proportional factor
   * @return success
   */
  bool setKp(float kp);

  /**
   * Set integration factor of PID controller
   * @param[in] ki integration factor
   * @return success
   */
  bool setKi(float ki);

  /**
   * Set differential factor of PID controller
   * @param[in] kd differential factor
   * @return success
   */
  bool setKd(float kd);

  /**
   * Set weight of input filter. Input values f are filtered with f'=weight*f'+(1-weight)*f.
   * @weight filtering weight. A value of 0 disables the filter. The value must be in the range of [0;1[
   */
  bool setInputWeight(float weight);

  /**
   * Wait for synchronization after a new PWM or RPM value is set.
   * @param[in] timeoutInMillis timeout period in milliseconds. A value of 0 disables the timeout check.
   * @return true==successful synchronization
   */
  bool waitForSync(unsigned int timeoutInMillis=100);

  /**
   * Stop motors
   */
  void stop();

protected:

private:

  bool sendFloat(int cmd, float f);

  /**
   * Implementation of inherited method from SocketCANObserver. This class is getting notified by the SocketCAN,
   * as soon as messages of interest arrive (having the desired CAN ID).
   * @param[in] frame CAN frame
   */
  void notify(struct can_frame* frame);

  SocketCAN* _can;

  can_frame _cf;

  float _rpm[2];

  short _pos[2];

  unsigned long _idSyncSend;

  unsigned long _idSyncReceive;

  unsigned short _channel;
};

#endif /* _MOTORCONTROLLERCAN_H_ */