#ifndef HBOARD_H
#define HBOARD_H
#include <QHash>
#include <QJsonObject>
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
class HFillNode;
class HBOARD_EXPORT HBoard : public QQuickItem {
  using task = std::function<void(void)>;
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QJsonObject items READ items WRITE setItems NOTIFY itemsChanged)
 public:
  explicit HBoard(QQuickItem *parent = nullptr);
  virtual ~HBoard() override;

 public:
  void home();
  void checkItems();
  int save(const QString &path);
  int save(QJsonArray &save);
  int load(const QString &path);
  int load(const QJsonArray &nodes);

 public:
  void pushTransform(const QTransform &trans);
  void pushNode(std::shared_ptr<HNodeBase> node, bool flag = true);
  //  void pushDashNode(HNodeBase* node);
  void removeNode(const QUuid &id);
  void clearNode();
  void removeSelectNode();
  void setHandle(HHandleBase *handle);
  QSGTransformNode *transformNode();
  QTransform transform();
  QHash<QUuid, std::shared_ptr<HNodeBase>> nodes();
  QHash<QUuid, std::shared_ptr<HNodeBase>> visibleNodes();
  void moveNode(const QUuid &n, QPointF dlt);      // move node delta
  void nodeMoveTo(const QUuid &n, QPointF point);  // move node to point
  void drawNodePoint(const QUuid &node,
                     const QList<QPointF> points);  // draw handle move event
  bool hasNode(const QUuid &node);
  void visibleNode(const QUuid &node, bool flag);

 public:
  void setSelect(const QUuid &s);
  void clearSelect();
  void pushSelect(const QUuid &s);
  void removdSelect(const QUuid &s);
  void changeSelectStatus(const QUuid &s);
  void changeSelectParam(const QString &key, const QJsonValue &value);
  QSet<QUuid> selects();

 public:  // keys
  QSet<int> keys();

 public:
  QString name();
  void setName(const QString &name);
  QJsonObject items();
  void setItems(const QJsonObject &item);

 public:
  QPointF WCS2LCS(const QPointF &point);
  QPointF LCS2WCS(const QPointF &point);
  double getScale();

 public:
  virtual QSGNode *updatePaintNode(QSGNode *node,
                                   UpdatePaintNodeData *) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void hoverEnterEvent(QHoverEvent *event) override;
  virtual void hoverMoveEvent(QHoverEvent *event) override;
  virtual void hoverLeaveEvent(QHoverEvent *event) override;

  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;
  //  virtual void sceneGraphInvalidated() override;

 protected:
  void pushTask(const task &t);
  QJsonObject getHandleParam();
 signals:
  void nameChanged();
  void hoverPoint(int x, int y);
  void itemsChanged();

 private:
  QSGTransformNode *_trans_node;
  HHandleBase *_handle;
  QMutex _mutex;
  QHash<QUuid, std::shared_ptr<HNodeBase>> _nodes;
  QString _name;
  QQueue<task> _tasks;
  QSet<int> _keys;
  QTimer _timer;
  QJsonObject _items;
};

#endif  // HBOARD_H
