#include "hnodefactory.h"

#include "../Nodes/himagenode.h"
#include "../Nodes/hshapenodes.h"
HNodeFactory::HNodeFactory() {}

std::shared_ptr<HNodeBase> HNodeFactory::create(const QJsonObject &param) {
  std::shared_ptr<HNodeBase> node = nullptr;
  auto type = param.value("shape_type").toString();
  if (0 == type.compare("image")) {
    node = std::make_shared<HImageNode>();
  } else if (0 == type.compare("fill node")) {
    node = std::make_shared<HFillNode>();
  } else if (0 == type.compare("shape line")) {
    node = std::make_shared<HShapeLineNode>();
  } else if (0 == type.compare("shape rect")) {
    node = std::make_shared<HShapeRectNode>();
  } else if (0 == type.compare("shape curve")) {
    node = std::make_shared<HShapeCurveNode>();
  } else if (0 == type.compare("shape poly")) {
    node = std::make_shared<HShapePolyNode>();
  } else if (0 == type.compare("shape fill rect")) {
    node = std::make_shared<HShapeFillRectNode>();
  } else if (0 == type.compare("shape circle")) {
    node = std::make_shared<HShapeCircleNode>();
  } else if (0 == type.compare("shape fill circle")) {
    node = std::make_shared<HShapeFillCircleNode>();
  } else if (0 == type.compare("shape cross")) {
    node = std::make_shared<HShapeCrossNode>();
  } else if (0 == type.compare("shape x node")) {
    node = std::make_shared<HShapeXNode>();
  }
  return node;
}
