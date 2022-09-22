#ifndef HHANDLEMOVE_H
#define HHANDLEMOVE_H
#include <QPointF>
#include <QTransform>

#include "hhandlebase.h"
class HBoard;
class HBOARD_EXPORT HHandleMove : public HHandleBase {
 public:
  HHandleMove();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void wheelEvent(HBoard* board, QWheelEvent* event) override;

  virtual void hoverEnterEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  void setCursorSize(int size);

  int cursorSize();

  void setColor(const QString& color);

  void setColor(const QColor& color);

  QColor getCursorColor();

  double getScale();

  void setScale(double s);

 protected:
  QPointF _last_point;
  QTransform _last_trans;
  double _scale;
  int _cursor_size;
  QString _cursor_color;
};

#endif  // HHANDLEMOVE_H
