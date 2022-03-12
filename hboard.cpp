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


    pushNode(new HImageNode("D:/Codes/build-Exe-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug/debug/test.jpg"));
//    pushNode(new HImageNode("D:/Codes/build-Exe-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug/debug/test.jpg", QRect(4000 * i,0,0,0)));
//    pushNode(new HImageNode("D:/Codes/build-Exe-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug/debug/DSC04018.JPG", QRect(4000 * i,0,0,0)));
//    for(int i = 0; i < 4; i++){
//        pushNode(new HImageNode("D:/Codes/build-Exe-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug/debug/test.jpg", QRect(4000 * i,0,0,0)));
//    }
    for(int i = 0; i < 4; i++){
//        pushNode(new HRectNode(QRect(i*400,0,200,200), Qt::GlobalColor::red));
        pushNode(new HRectNode(QRect(i*400,0,200,200), QColor(255,0,0,150)));
    }

    pushTransform(QTransform().scale(0.5,0.5));

}

void HBoard::pushTransform(const QTransform &trans)
{
    QMutexLocker lock(&_mutex);
    _transform_list.enqueue(trans);
}

void HBoard::pushNode(HNodeBase *node)
{
    QMutexLocker lock(&_mutex);
    _node_list.enqueue(node);
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
    for(const auto& s: _selects){
        removdSelect(s);
    }
    _selects.clear();
}

void HBoard::pushSelect(const QUuid &s)
{
    if(!_selects.contains(s)){
        _selects.insert(s);
    }
}

void HBoard::removdSelect(const QUuid &s)
{
    if(_selects.contains(s)){
        _selects.remove(s);
    }
}

void HBoard::changeSelectStatus(const QUuid &s)
{
    if(_selects.contains(s)){
        removdSelect(s);
    }else{
        pushSelect(s);
    }
}

QHash<QUuid, HNodeBase *> HBoard::nodes()
{
    return _nodes;
}

QString HBoard::name()
{
    return _name;
}

void HBoard::setName(const QString &name)
{
    if(name != _name){
        _name = name;
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
    while (!_node_list.empty()) {
        auto n = _node_list.dequeue();
        if(n){
            _trans_node->appendChildNode(n->build(this));
            _nodes.insert(n->id(), n);
        }
    }
    while (!_transform_list.empty()) {
        auto f = _transform_list.dequeue();
        if(_trans_node)_trans_node->setMatrix(f);
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
