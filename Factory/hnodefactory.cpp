#include "hnodefactory.h"

#include <QDebug>

#include "../Nodes/himagemapnode.h"
#include "../Nodes/himagenode.h"
#include "../Nodes/hmultshapenode.h"
#include "../Nodes/hshapenodes.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HNodeFactory::HNodeFactory() {}

std::shared_ptr<HNodeBase> HNodeFactory::create(const QJsonObject &param) {
  std::shared_ptr<HNodeBase> node = nullptr;
  auto type = static_cast<HNodeBase::NODETYPE>(param.value("nodeType").toInt());
  DEBUG << type;
  switch (type) {
    case HNodeBase::NODETYPE::SHAPE:
      node = std::make_shared<HFillNode>();
      break;
    case HNodeBase::NODETYPE::IMAGE:
      node = std::make_shared<HImageNode>();
      break;
    case HNodeBase::NODETYPE::SHAPELINE:
      node = std::make_shared<HShapeLineNode>();
      break;
    case HNodeBase::NODETYPE::SHAPERECT:
      node = std::make_shared<HShapeRectNode>();
      break;
    case HNodeBase::NODETYPE::SHAPECURVE:
      node = std::make_shared<HShapeCurveNode>();
      break;
    case HNodeBase::NODETYPE::SHAPEPOLY:
      node = std::make_shared<HShapePolyNode>();
      break;
    case HNodeBase::NODETYPE::SHAPEFILLRECT:
      node = std::make_shared<HShapeFillRectNode>();
      break;
    case HNodeBase::NODETYPE::SHAPECIRCLE:
      node = std::make_shared<HShapeCircleNode>();
      break;
    case HNodeBase::NODETYPE::SHAPEFILLCIRCLE:
      node = std::make_shared<HShapeFillCircleNode>();
      break;
    case HNodeBase::NODETYPE::SHAPECROSS:
      node = std::make_shared<HShapeCrossNode>();
      break;
    case HNodeBase::NODETYPE::SHAPEXNODE:
      node = std::make_shared<HShapeXNode>();
      break;
    case HNodeBase::NODETYPE::MAPINAGE:
      node = std::make_shared<HImageMapNode>();
      break;
    case HNodeBase::NODETYPE::SHAPEELLIPSE:
      node = std::make_shared<HShapeEllipseNode>();
      break;
    case HNodeBase::NODETYPE::SHAPEFILLELLIPSE:
      node = std::make_shared<HShapeFillEllipseNode>();
      break;
    case HNodeBase::NODETYPE::MULTSHAPE:
      node = std::make_shared<HMultShapeNode>();
      break;
  }
  return node;
}
