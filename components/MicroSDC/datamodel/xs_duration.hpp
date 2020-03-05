#include <chrono>
#include <string>

class Duration
{
public:
  explicit Duration(const std::string& string);
  Duration(bool isNegative, unsigned int years, unsigned int months, unsigned int days,
           unsigned int hours, unsigned int minutes, double seconds);

  bool isNegative() const;
  unsigned int years() const;
  unsigned int months() const;
  unsigned int days() const;
  unsigned int hours() const;
  unsigned int minutes() const;
  double seconds() const;

  std::string str() const;

  std::chrono::seconds toDurationSeconds() const;
  std::chrono::system_clock::time_point toExpirationTimePoint() const;

protected:
  void parse(const std::string& string);

private:
  bool isNegative_{false};
  unsigned int years_{0};
  unsigned int months_{0};
  unsigned int days_{0};
  unsigned int hours_{0};
  unsigned int minutes_{0};
  double seconds_{0.0};
};
