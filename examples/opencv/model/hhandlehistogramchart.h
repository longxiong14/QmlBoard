#ifndef HHANDLEHISTOGRAMCHART_H
#define HHANDLEHISTOGRAMCHART_H

#include <QJsonArray>
#include <QObject>

#include "../../Handles/hhandlepaint.h"
class HImageMapNodeDelegate;
class HHandleHistogramChart : public QObject, public HHandleDrawRect {
  Q_OBJECT
 public:
  explicit HHandleHistogramChart(QObject* parent = nullptr);

  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

 signals:

 public slots:

 protected:
  QJsonArray _chart_array;
  std::shared_ptr<HImageMapNodeDelegate> _image_node;
  QUuid _last_node;
};

#endif  // HHANDLEHISTOGRAMCHART_H
