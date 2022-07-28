#ifndef HBOARD_H
#define HBOARD_H
#include <QHash>
#include <QMutex>
#include <QQueue>
#include <QSet>
#include <QString>
#include <QTimer>
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
  Q_INVOKABLE
  void visibleNode(bool flag);

  Q_INVOKABLE
  void showAll();

 public:
  void home();

 public:
  void pushTransform(const QTransform& trans);
  void pushNode(HNodeBase* node, bool flag = true);
  void removeNode(const QUuid& id);
  void setHandle(HHandleBase* handle);
  QSGTransformNode* transformNode();
  QTransform transform();
  QHash<QUuid, HNodeBase*> nodes();
  QHash<QUuid, HNodeBase*> visibleNodes();
  void moveNode(const QUuid& n, QPoint dlt);      // move node delta
  void nodeMoveTo(const QUuid& n, QPoint point);  // move node to point
  void drawNodePoint(const QUuid& node,
                     const QList<QPoint> points);  // draw handle move event
  bool hasNode(const QUuid& node);
  void visibleNode(const QUuid& node, bool flag);

 public:
  void setSelect(const QUuid& s);
  void clearSelect();
  void pushSelect(const QUuid& s);
  void removdSelect(const QUuid& s);
  void changeSelectStatus(const QUuid& s);
  QSet<QUuid> selects();

 public:  // keys
  QSet<int> keys();

 public:
  QString name();
  void setName(const QString& name);

 public:
  QPoint WCS2LCS(const QPoint& point);
  QPoint LCS2WCS(const QPoint& point);
  double getScale();

 public:
  virtual QSGNode* updatePaintNode(QSGNode* node,
                                   UpdatePaintNodeData*) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void wheelEvent(QWheelEvent* event) override;
  virtual void hoverEnterEvent(QHoverEvent* event) override;
  virtual void hoverMoveEvent(QHoverEvent* event) override;
  virtual void hoverLeaveEvent(QHoverEvent* event) override;

  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void keyReleaseEvent(QKeyEvent* event) override;

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
  QQueue<task> _tasks;
  QSet<int> _keys;
  QHash<QUuid, HNodeBase*> _dash_nodes;
  QTimer _timer;
};

#endif  // HBOARD_H
