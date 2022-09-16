#ifndef HIMAGEMAPNODE_H
#define HIMAGEMAPNODE_H

#include "../HBoard_global.h"
#include "QImage"
#include "hnodebase.h"
class HFillNode;
class HBOARD_EXPORT HImageMapNode : public HNodeBase {
 public:
  HImageMapNode();
  HImageMapNode(const QString &path, const QPointF &start_point = QPointF());
  HImageMapNode(const QImage &mat, const QPointF &start_point = QPointF());
  virtual ~HImageMapNode() override;

  virtual QSGNode *build(HBoard *) override;
  virtual QSGNode *get() override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void changedSelectStatus() override;
  virtual bool isSelect() override;
  virtual void move(const QPointF &) override;
  virtual void moveTo(const QPointF &) override;  // move to point
  virtual void drawPoints(const QList<QPointF> &) override;
  virtual NODETYPE nodeType() override;
  virtual void updateDrawMode(unsigned long mode) override;
  virtual void setVisible(bool flag) override;
  virtual bool visible() override;
  virtual void timeOut() override;
  virtual int setText(const QString &text, const QRectF &position = QRectF(),
                      int pixel_size = 10) override;
  virtual QString getText() override;

  virtual bool enableHome() override;
  virtual void setEnableHome(bool f) override;
  virtual void setDestory(bool flag) override;

  virtual void setFlag(NODEFLAG flag, bool open) override;

  // start: relative position
  virtual void updateMat(HBoard *board, const QImage &mat,
                         const QPointF &start) override;

  // roi: relative rectangle
  virtual void updateRoi(HBoard *board, const QRectF &roi) override;

 public:
  virtual int load(const QString &path) override;
  virtual int save(const QString &path) override;

  QImage getImage();
  void setImage(const QImage &i);
  QImage getScaleImage(double scale);

 protected:
  QImage _image;
  QImage _scale_image;
  std::shared_ptr<HFillNode> _fill_node;
  double _scale;
};

#endif  // HIMAGEMAPNODE_H
