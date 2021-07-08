#pragma once

#include <math.h>

namespace apex {

class Odometry {
public:
  Odometry(double (*leftEncoder)(void), double (*rightEncoder)(void),
           double (*sideEncoder)(void), const double wheelBase,
           const double trackingCenterToTrackingWheel,
           const double distancePerPulse, const double threshold)
      : m_leftEncoder(leftEncoder), m_rightEncoder(rightEncoder),
        m_sideEncoder(sideEncoder), m_wheelBase(wheelBase),
        m_trackingCenterToTrackingWheel(trackingCenterToTrackingWheel),
        m_distancePerPulse(distancePerPulse), m_threshold(threshold) {}

  double ticksToInches(double ticks) { return ticks * m_distancePerPulse; }

  void track() {
    // Measure the left and right encoders
    m_leftPos = m_leftEncoder();
    m_rightPos = m_rightEncoder();
    m_sidePos = m_sideEncoder();

    // Calculate the change in distance of the left and right side of the
    // drivetrain
    double deltaL = ticksToInches(m_leftPos - m_prevLeftPos);
    double deltaR = ticksToInches(m_rightPos - m_prevRightPos);
    double deltaS = ticksToInches(m_sidePos - m_prevSidePos);

    // Update the previous encoder values
    m_prevLeftPos = m_leftPos;
    m_prevRightPos = m_rightPos;
    m_prevSidePos = m_sidePos;

    // Calculate total change in encoders since the last reset
    m_absL += deltaL;
    m_absR += deltaR;
    m_absS += deltaS;

    // Calculate new absolute orientation
    double theta = m_startPos[2] + ((m_absL - m_absR) / m_wheelBase);

    // Calculate change in orientation
    double deltaTheta = m_pos[2] - theta;

    // Calculate orientation
    // TODO: Determine Correctness?
    // m_pos[2] += deltaTheta;
    // m_pos[2] = theta;

    double displacementLocalX;
    double displacementLocalY;
    // Calculate local offset
    // TODO: Finish Threshold
    if (std::fabs(deltaL - deltaR) < m_threshold) {
      displacementLocalX = deltaS;
      displacementLocalY = deltaL; // or deltaR
    } else {
      displacementLocalX =
          2 * sin(theta / 2) *
          (deltaS / deltaTheta + m_trackingCenterToTrackingWheel);
      displacementLocalY =
          2 * sin(theta / 2) * (deltaS / deltaTheta + m_wheelBase / 2);
    }

    m_pos[0] += displacementLocalX;
    m_pos[1] += displacementLocalY;
    m_pos[2] += deltaTheta;
  }

private:
  static constexpr double m_startPos[3] = {0, 0, 0};

  double (*m_leftEncoder)(void);
  double (*m_rightEncoder)(void);
  double (*m_sideEncoder)(void);

  const double m_wheelBase;
  const double m_trackingCenterToTrackingWheel;
  const double m_distancePerPulse;
  const double m_threshold;

  double m_leftPos = 0;
  double m_rightPos = 0;
  double m_sidePos = 0;

  double m_prevLeftPos = 0;
  double m_prevRightPos = 0;
  double m_prevSidePos = 0;

  double m_absL = 0;
  double m_absR = 0;
  double m_absS = 0;

  double m_pos[3] = {0, 0, 0};
};

} // namespace apex
