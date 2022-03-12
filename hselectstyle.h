#ifndef HSELECTSTYLE_H
#define HSELECTSTYLE_H

#include <QSGRenderNode>
class HSelectStyle : public QSGRenderNode
{
public:
    HSelectStyle();

    virtual void render(const RenderState *state) override;
};

#endif // HSELECTSTYLE_H
