#include "himagenode.h"
#include <QSGNode>
#include <QImage>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HImageNode::HImageNode(const QString &path, const QRect &rect)
    :HNodeBase(),_path(path),_rect(rect)
{
}

QSGNode *HImageNode::build(HBoard *board)
{
    if(board){
        QImage image;
        image.load(_path);
        if(image.isNull()){
            DEBUG << "image is null";
            return nullptr;
        }
        QSGTexture* texture = board->window()->createTextureFromImage(image);
//        QSGSimpleTextureNode * tn = new QSGSimpleTextureNode();
        setTexture(texture);
        if(_rect.size().isNull()){
            _rect.setSize(image.size());
        }
       setRect(_rect);
//        appendChildNode(tn);
//        return tn;
    }
    return  this;
}

QRect HImageNode::getBoundRect()
{
    return _rect;
}

//void HImageNode::update()
//{

//}

