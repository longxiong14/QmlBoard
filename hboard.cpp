#include "hboard.h"
#include <QSGSimpleRectNode>
#include <QQmlEngine>
#include <QGraphicsScene>
#include <QSGSimpleTextureNode>
#include <QSGTexture>
#include <QSGImageNode>
#include <QQuickWindow>
#include <QSGOpaqueTextureMaterial>
#include "Handles/hhandlebase.h"
#include "Handles/hhandlemove.h"
#include "Handles/hhandlearrow.h"
#include "Nodes/hnodebase.h"
#include "Nodes/himagenode.h"
#include "Nodes/hrectnode.h"
#include "hboardmanager.h"
#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HBoard::HBoard(QQuickItem *parent):
    QQuickItem(parent),
    _trans_node(nullptr),
    _handle(new HHandleArrow()),
    _name("")
{
    setFlags(QQuickItem::ItemHasContents);
    setClip(true);
    setAcceptHoverEvents(true);
    setSmooth(true);
    setAntialiasing(true);
    setAcceptedMouseButtons(Qt::MouseButton::LeftButton
                            | Qt::MouseButton::RightButton
                            | Qt::MouseButton::MiddleButton);
}

void HBoard::pushTransform(const QTransform &trans)
{
    pushTask([=](){
        if(_trans_node)_trans_node->setMatrix(trans);
    });
}

void HBoard::pushNode(HNodeBase *node)
{
    pushTask([=](){
        if(node){
            _trans_node->appendChildNode(node->build(this));
            _nodes.insert(node->id(), node);
        }

    });
}

void HBoard::setHandle(HHandleBase *handle)
{
    _handle = handle;
}

void HBoard::setSelect(const QUuid &s)
{
    clearSelect();
    pushSelect(s);
}

void HBoard::clearSelect()
{
    for(auto n : _nodes){
        if(n->isSelect()){
            pushTask([=](){
                n->changedSelectStatus();
            });
        }
    }
    update();
}

void HBoard::pushSelect(const QUuid &s)
{
    pushTask([=](){
        if(_nodes.contains(s)){
            if(!_nodes[s]->isSelect()){
                _nodes[s]->changedSelectStatus();
            }
        }
    });
    update();
}

void HBoard::removdSelect(const QUuid &s)
{
    pushTask([=](){
        if(_nodes.contains(s)){
            if(_nodes[s]->isSelect()){
                _nodes[s]->changedSelectStatus();
            }
        }
    });
    update();
}

void HBoard::changeSelectStatus(const QUuid &s)
{
    if(_nodes.contains(s)){
        if(_nodes[s]->isSelect()){
            removdSelect(s);
        }else{
            pushSelect(s);
        }
    }
}

QSet<QUuid> HBoard::selects()
{
    QSet<QUuid> set;
    for(auto n :_nodes){
        if(n->isSelect()){
            set.insert(n->id());
        }
    }
    return set;
}

QHash<QUuid, HNodeBase *> HBoard::nodes()
{
    return _nodes;
}

void HBoard::moveNode(const QUuid &n, QPoint dlt)
{
    pushTask([=](){
        if(_nodes.contains(n)){
            _nodes[n]->move(dlt);
        }
    });
    update();
}

void HBoard::nodeMoveTo(const QUuid &n, QPoint point)
{
    pushTask([=](){
        if(_nodes.contains(n)){
            _nodes[n]->moveTo(point);
        }
    });
    update();
}

void HBoard::drawNodePoint(const QUuid &node, const QList<QPoint> points)
{
    if(_nodes.contains(node))
        pushTask([=](){
            _nodes[node]->drawPoints(points);
        });
}

bool HBoard::hasNode(const QUuid &node)
{
    return _nodes.contains(node);
}

QString HBoard::name()
{
    return _name;
}

void HBoard::setName(const QString &name)
{
    if(name != _name){
        auto old = _name;
        _name = name;
        HBoardManager::getInstance()->changeBoardNmae(old, this);
        nameChanged();
    }
}

QPoint HBoard::WCS2LCS(const QPoint &point)
{
    QPoint pt;
    if(_trans_node){
        auto trans_form = _trans_node->matrix().toTransform();
        pt = trans_form.inverted().map(point);
    }
    return pt;
}

QSGTransformNode *HBoard::transformNode()
{
    return _trans_node;
}

QTransform HBoard::transform()
{
    QTransform trans;
    if(_trans_node)trans = _trans_node->matrix().toTransform();
    return trans;
}

QSGNode *HBoard::updatePaintNode(QSGNode * node, QQuickItem::UpdatePaintNodeData *)
{
    if(!node){
        node =  new QSGNode();
        _trans_node= new QSGTransformNode();
        _trans_node->setMatrix(QMatrix4x4(QTransform()));
        node->appendChildNode(_trans_node);
        node->setFlag(QSGNode::OwnedByParent);
    }

    QMutexLocker lock(&_mutex);
    while (!_tasks.empty()) {
        auto f = _tasks.dequeue();
        f();
    }
    return node;
}

void HBoard::mousePressEvent(QMouseEvent *event)
{
    if(_handle)_handle->mousePressEvent(this, event);
    update();
}

void HBoard::mouseMoveEvent(QMouseEvent *event)
{
    if(_handle)_handle->mouseMoveEvent(this, event);
    update();
}

void HBoard::mouseReleaseEvent(QMouseEvent *event)
{
    if(_handle)_handle->mouseReleaseEvent(this, event);
    update();
}

void HBoard::wheelEvent(QWheelEvent *event)
{
    if(_handle)_handle->wheelEvent(this, event);
    update();
}

void HBoard::hoverMoveEvent(QHoverEvent *event)
{
    //    DEBUG << WCS2LCS(event->pos());
}

void HBoard::pushTask(const HBoard::task &t)
{
    QMutexLocker lock(&_mutex);
    _tasks.push_back(t);
}
