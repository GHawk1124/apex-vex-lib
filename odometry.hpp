#pragma once

#include <math.h>

namespace apex {

class Odometry {
public:
  Odometry(double (*leftEncoder)(void), double (*rightEncoder)(void),
           const double wheelRad, const double wheelBase,
           const double distancePerPulse)
      : m_leftEncoder(leftEncoder), m_rightEncoder(rightEncoder),
        m_wheelBase(wheelBase), m_wheelRad(wheelRad),
        m_distancePerPulse(distancePerPulse) {}

  virtual void wait();

  double ticksToInches(double ticks) { return ticks * m_distancePerPulse; }

  void track() {
    // Measure the left and right encoders
    m_leftPos = m_leftEncoder();
    m_rightPos = m_rightEncoder();

    // Calculate the change in distance of the left and right side of the
    // drivetrain
    double deltaL = ticksToInches(m_leftPos - m_prevLeftPos);
    double deltaR = ticksToInches(m_rightPos - m_prevRightPos);

    // Update the previous encoder values
    m_prevLeftPos = m_leftPos;
    m_prevRightPos = m_rightPos;

    // Calculate total change in encoders since the last reset
    m_absL += deltaL;
    m_absR += deltaR;

    // Calculate new absolute orientation
    double theta = m_startPos[2] + ((m_absL - m_absR) / m_wheelBase);

    // Calculate change in orientation
    double deltaTheta = m_pos[2] - theta;

    // Calculate orientation
    m_pos[2] += deltaTheta;

    double displacementLocal;
    // Calculate local offset
    if (deltaL == deltaR) {
      displacementLocal = deltaL; // or deltaR
    } else {
      displacementLocal = 2 * sin(m_pos[2] / 2) *
    }

    wait();
  }

private:
  const double m_distancePerPulse;
  const double m_wheelRad;
  const double m_wheelBase;
  const double m_startPos[3] = {0, 0, 0};

  double (*m_leftEncoder)(void);
  double (*m_rightEncoder)(void);
  double m_leftPos = 0;
  double m_rightPos = 0;
  double m_prevLeftPos = 0;
  double m_prevRightPos = 0;
  double m_absL = 0;
  double m_absR = 0;

  double m_pos[3] = {0, 0, 0};
};

} // namespace apex
