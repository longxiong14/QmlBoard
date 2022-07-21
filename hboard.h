#ifndef HBOARD_H
#define HBOARD_H
#include <QHash>
#include <QMutex>
#include <QQueue>
#include <QSet>
#include <QString>
#include <QTransform>
#include <QUuid>
#include <QtQuick/QQuickItem>
#include <functional>

#include "HBoard_global.h"
class QSGNode;
class QSGTransformNode;
class HHandleBase;
class HNodeBase;
class HBOARD_EXPORT HBoard : public QQuickItem {
  using task = std::function<void(void)>;
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
 public:
  explicit HBoard(QQuickItem* parent = nullptr);

 public:
  void home();

 public:
  void pushTransform(const QTransform& trans);
  void pushNode(HNodeBase* node, bool flag = true);
  void setHandle(HHandleBase* handle);
  QSGTransformNode* transformNode();
  QTransform transform();
  QHash<QUuid, HNodeBase*> nodes();
  void moveNode(const QUuid& n, QPoint dlt);      // move node delta
  void nodeMoveTo(const QUuid& n, QPoint point);  // move node to point
  void drawNodePoint(const QUuid& node,
                     const QList<QPoint> points);  // draw handle move event
  bool hasNode(const QUuid& node);

 public:
  void setSelect(const QUuid& s);
  void clearSelect();
  void pushSelect(const QUuid& s);
  void removdSelect(const QUuid& s);
  void changeSelectStatus(const QUuid& s);
  QSet<QUuid> selects();

 public:
  QString name();
  void setName(const QString& name);

 public:
  QPoint WCS2LCS(const QPoint& point);

 public:
  virtual QSGNode* updatePaintNode(QSGNode* node,
                                   UpdatePaintNodeData*) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void wheelEvent(QWheelEvent* event) override;
  virtual void hoverMoveEvent(QHoverEvent* event) override;

 protected:
  void pushTask(const task& t);
 signals:
  void nameChanged();
  void hoverPoint(int x, int y);

 private:
  QSGTransformNode* _trans_node;
  HHandleBase* _handle;
  QMutex _mutex;
  QHash<QUuid, HNodeBase*> _nodes;
  QString _name;
  //    QSet<QUuid>                 _selects;
  QQueue<task> _tasks;
};

#endif  // HBOARD_H
