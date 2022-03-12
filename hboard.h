#ifndef HBOARD_H
#define HBOARD_H
#include "HBoard_global.h"
#include <QtQuick/QQuickItem>
#include <QQueue>
#include <QMutex>
#include <QTransform>
#include <QHash>
#include <QUuid>
#include <QString>
#include <QSet>
class QSGNode;
class QSGTransformNode;
class HHandleBase;
class HNodeBase;
class HBOARD_EXPORT HBoard : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit HBoard(QQuickItem *parent = nullptr);
public:
    void pushTransform(const QTransform & trans);
    void pushNode(HNodeBase * node);
    void setHandle(HHandleBase* handle);
    QSGTransformNode* transformNode();
    QTransform transform();
    QHash<QUuid, HNodeBase*> nodes();
public:
    void setSelect(const QUuid& s);
    void clearSelect();
    void pushSelect(const QUuid& s);
    void removdSelect(const QUuid& s);
    void changeSelectStatus(const QUuid& s);
public:
    QString name();
    void setName(const QString& name);
public:
    QPoint WCS2LCS(const QPoint& point);
public:
    virtual QSGNode * updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

signals:
    void nameChanged();

private:
    QSGTransformNode*           _trans_node;
    HHandleBase*                _handle;
    QMutex                      _mutex;
    QQueue<QTransform>          _transform_list;
    QHash<QUuid, HNodeBase*>    _nodes;
    QQueue<HNodeBase*>          _node_list;
    QString                     _name;
    QSet<QUuid>                 _selects;
};

#endif // HBOARD_H
