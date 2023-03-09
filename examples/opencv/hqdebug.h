#ifndef HQDEBUG_H
#define HQDEBUG_H
#include <QDebug>
#include <opencv2/core.hpp>

#define DEBUG qDebug() << __FUNCTION__ << __LINE__
QDebug operator<<(QDebug, const std::string& str);
QDebug operator<<(QDebug d, const cv::Rect& rect);
QDebug operator<<(QDebug d, const cv::Point& point);
QDebug operator<<(QDebug d, const cv::Size& point);
QDebug operator<<(QDebug d, const cv::Scalar& scalar);
QDebug operator<<(QDebug d, const cv::Mat& mat);

#define HPROPERTY(type, name, get, set, changed) \
  Q_PROPERTY(type name READ get WRITE set NOTIFY changed)

#define HDEFINETION(type, name, get, set, changed) \
 protected:                                        \
  type _##name;                                    \
                                                   \
 public:                                           \
  type get() { return _##name; }                   \
  void set(type t) {                               \
    _##name = t;                                   \
    changed();                                     \
  }

#define HDEFINETIONC(type, name, get, set, changed) \
 protected:                                         \
  type _##name;                                     \
                                                    \
 public:                                            \
  type get() { return _##name; }                    \
  void set(type t) {                                \
    if (_##name == t) return;                       \
    _##name = t;                                    \
    changed();                                      \
  }

#define SETGET(type, name, get, set) \
 protected:                          \
  type name;                         \
                                     \
 public:                             \
  type get() { return name; }        \
  void set(const type& t) { name = t; }

#endif  // HQDEBUG_H
