#ifndef HNODESCONTROL_H
#define HNODESCONTROL_H

#include <QObject>
class HReviewNodeManager;
class HNodesControl : public QObject {
  Q_OBJECT
 public:
  explicit HNodesControl(QObject *parent = nullptr);

  Q_INVOKABLE
  int push();

  Q_INVOKABLE
  int clear();

  Q_INVOKABLE
  int load();

  Q_INVOKABLE
  int save();
 signals:

 public slots:

 private:
  QList<std::shared_ptr<HReviewNodeManager>> _reviews;
};

#endif  // HNODESCONTROL_H
