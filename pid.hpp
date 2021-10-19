class PIDcontroller {
public:
  const double m_kP;
  const double m_kI;
  const double m_kD;

  const double m_distancePerPulse;

  double m_error;
  double m_integral;

  virtual double sensorVal();

  virtual double measureTime();

  void run(double target) {
    double currentTime = (double)measureTime();
    double error = target - (double)sensorVal();
    double integral = integral + error;
  }
}
