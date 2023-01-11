#ifndef HBLURREDIMAGE_H
#define HBLURREDIMAGE_H
#include "../HBoard_global.h"
#include "himagemapnode.h"
class HBOARD_EXPORT HBlurredImageNode : public HImageMapNodeDelegate {
 public:
  HBlurredImageNode(const QImage& image, const QPointF& point, double scale);

  virtual QImage getImage(const QRectF& roi, double scale) override;

 protected:
  QImage _image;
  double _scale;
};

#endif  // HBLURREDIMAGE_H
