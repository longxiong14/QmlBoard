#ifndef HBOARDUICONTROL_H
#define HBOARDUICONTROL_H

#include <QObject>

#include "HBoard_global.h"

class HBOARD_EXPORT HBoardUIControl : public QObject {
  Q_OBJECT
 public:
  explicit HBoardUIControl(QObject *parent = nullptr);

 signals:

 public slots:
};

#endif  // HBOARDUICONTROL_H
