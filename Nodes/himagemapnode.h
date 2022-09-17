#ifndef HIMAGEMAPNODE_H
#define HIMAGEMAPNODE_H

#include "../HBoard_global.h"
#include "QImage"
#include "hfillnode.h"

class HBOARD_EXPORT HImageMapNodeDelegate : public HNodeBase {
 public:
  HImageMapNodeDelegate(const QRectF &rect = QRectF());

  virtual QSGNode *build(HBoard *b) override;

  virtual QSGNode *get() override;

  virtual QRectF getBoundRect() override;

  virtual QList<QPointF> getPointList() override;

  virtual void changedSelectStatus() override;

  virtual bool isSelect() override;

  virtual void move(const QPointF &) override;

  virtual void moveTo(const QPointF &) override;

  virtual void drawPoints(const QList<QPointF> &) override;

  virtual void updateDrawMode(unsigned long mode) override;

  virtual void setVisible(bool flag) override;

  virtual bool visible() override;

  virtual void timeOut() override;

  virtual int setText(const QString &text, const QRectF &position = QRectF(),
                      int pixel_size = 10) override;

  virtual QString getText() override;

  bool enableHome() override;

  virtual void setEnableHome(bool f) override;

  virtual void setDestory(bool flag) override;

  virtual void setFlag(NODEFLAG flag, bool open) override;

  virtual QImage getImage(const QRectF &roi, double scale) = 0;

  virtual NODETYPE nodeType() override;

  void setRect(const QRectF &rect);

 protected:
  std::shared_ptr<HFillNode> _fill_node;
};

class HBOARD_EXPORT HImageMapNode : public HImageMapNodeDelegate {
 public:
  HImageMapNode();
  HImageMapNode(const QString &path, const QPointF &start_point = QPointF());
  HImageMapNode(const QImage &mat, const QPointF &start_point = QPointF());
  HImageMapNode(const HImageMapNode &node);
  HImageMapNode &operator=(const HImageMapNode &node);
  virtual ~HImageMapNode() override;

  // start: relative position
  virtual void updateMat(HBoard *board, const QImage &mat,
                         const QPointF &start) override;

  virtual QImage getImage(const QRectF &roi, double scale) override;

 public:
  virtual int save(QJsonObject &o) override;
  virtual int load(const QJsonObject &o) override;
  virtual int save(const QString &path) override;
  virtual int load(const QString &path) override;

  QImage getImage();
  void setImage(const QImage &i);

 protected:
  QImage _image;
};

#endif  // HIMAGEMAPNODE_H
