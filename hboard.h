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
class QSGGeometryNode;
class HBOARD_EXPORT HBoard : public QQuickItem {
  using task = std::function<void(void)>;
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QJsonObject items READ items WRITE setItems NOTIFY itemsChanged)
  Q_PROPERTY(bool rule READ rule WRITE setRule NOTIFY ruleChanged)
 public:
  explicit HBoard(QQuickItem *parent = nullptr);
  virtual ~HBoard() override;

 public:
  Q_INVOKABLE
  virtual void home();
  void checkItems();
  int save(const QString &path);
  int save(QJsonArray &save);
  int load(const QString &path);
  int load(const QJsonArray &nodes);

 public:
  virtual void pushTransform(const QTransform &trans);
  virtual void face(int x, int y);
  virtual void pushNode(std::shared_ptr<HNodeBase> node, bool flag = true);
  //  void pushDashNode(HNodeBase* node);
  virtual void removeNode(const QUuid &id);
  virtual void removeNodes(const QList<QUuid> &nodes);
  void clearNode();
  void removeSelectNode();
  void setHandle(std::shared_ptr<HHandleBase> handle);
  QSGTransformNode *transformNode();
  QTransform transform();
  QHash<QUuid, std::shared_ptr<HNodeBase>> nodes();
  QHash<QUuid, std::shared_ptr<HNodeBase>> visibleNodes();
  virtual void moveNode(const QUuid &n, QPointF dlt);      // move node delta
  virtual void nodeMoveTo(const QUuid &n, QPointF point);  // move node to point
  void drawNodePoint(const QUuid &node,
                     const QList<QPointF> points);  // draw handle move event
  void updateNodeIndexPoint(const QUuid &node, int index,
                            const QPointF &point);  // draw handle move event
  int updateNodeText(const QUuid &node, const QString &text,
                     const QRectF &rect = QRectF(), int pixel_size = 10);

  int updateNodeDrawMode(const QUuid &node, unsigned long mode);

  // start: relate position
  virtual bool updateNodeMat(const QUuid &node, const QImage &mat,
                             const QPointF &start);
  bool hasNode(const QUuid &node);
  void visibleNode(const QUuid &node, bool flag);
  std::shared_ptr<HNodeBase> getNodeById(const QUuid &id);

 public:
  void setSelect(const QUuid &s);
  void clearSelect();
  void pushSelect(const QUuid &s);
  void removeSelect(const QUuid &s);
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
  bool rule();
  void setRule(bool f);

 public:
  QPointF WCS2LCS(const QPointF &point);
  QPointF LCS2WCS(const QPointF &point);
  double getScale();
  QRectF getWCSBoundRect();

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
  void updateRule(QSGNode *parent);
  void buildTopRule(QList<QPointF> &list);
  void buildLeftRule(QList<QPointF> &list);
  bool containNodes(std::shared_ptr<HNodeBase> node);
  bool containNodes(const QUuid &id);
  int removeNodeToList(const QUuid &id);
  int removeNode(std::shared_ptr<HNodeBase> node);

  void updateSelectDragNodes();
  void removeDragNode(std::shared_ptr<HNodeBase> node);
 signals:
  void nameChanged();
  void hoverPoint(int x, int y);
  void itemsChanged();
  void updated();
  void ruleChanged();
  void faceChanged();

 protected:
  QSGTransformNode *_trans_node;
  std::shared_ptr<HHandleBase> _handle;
  QMutex _mutex;
  QHash<QUuid, std::shared_ptr<HNodeBase>> _nodes;
  //  QList<std::shared_ptr<HNodeBase>> _nodes;
  QString _name;
  QQueue<task> _tasks;
  QSet<int> _keys;
  QTimer _timer;
  QJsonObject _items;
  QSGGeometryNode *_rule;
  QSGNode *_drag_nodes;
  bool _rule_flag;
};

#endif  // HBOARD_H
