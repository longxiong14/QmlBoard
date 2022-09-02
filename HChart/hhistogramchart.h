﻿#ifndef HHISTOGRAMCHART_H
#define HHISTOGRAMCHART_H
#include <QJsonArray>
#include <QQuickItem>
#include <QString>

#include "../HBoard_global.h"
/*
ex : array
    int array[255] = {1,2,3, ... ,255}
*/
class QSGGeometryNode;
class HBOARD_EXPORT HHistogramChart : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QJsonArray array READ array WRITE setArray NOTIFY arrayChanged)
  Q_PROPERTY(int left READ left WRITE setLeft NOTIFY leftChanged)
  Q_PROPERTY(int right READ right WRITE setRight NOTIFY rightChanged)
  Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

 public:
  HHistogramChart(QQuickItem *parent = nullptr);

  virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void hoverMoveEvent(QHoverEvent *event) override;

 public:
  QJsonArray array();
  void setArray(const QJsonArray &array);

  int left();
  void setLeft(int l);

  int right();
  void setRight(int r);

  QString color();
  void setColor(const QString &c);

 protected:
  void updateChart(QSGNode *node);
  void updateLeft(QSGNode *node);
  void updateRight(QSGNode *node);

  void updateNode(QSGGeometryNode *&node, int num);

 signals:
  void arrayChanged();
  void leftChanged();
  void rightChanged();
  void colorChanged();
 public slots:

 protected:
  typedef enum { NONE, LEFT, RIGHT } LOR;

 protected:
  QJsonArray _array;
  bool _array_changed;
  QSGGeometryNode *_chart_node;
  QSGGeometryNode *_left_node;
  QSGGeometryNode *_right_node;
  QSGNode *_bottom_rule;

  int _left;
  int _right;
  QString _color;

  LOR _lor;
};

#endif  // HHISTOGRAMCHART_H
