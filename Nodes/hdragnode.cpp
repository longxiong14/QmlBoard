#include "hdragnode.h"

HDragNode::HDragNode() : QSGNode(), _flag(0) {}

void HDragNode::setFlag(int f) { _flag = f; }
