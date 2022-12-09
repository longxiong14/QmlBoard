#ifndef HKEYFACTORY_H
#define HKEYFACTORY_H
#include <QKeyEvent>

#include "../HBoard_global.h"
class HBoard;

class HBOARD_EXPORT HKeyFactoryBase {
 public:
  HKeyFactoryBase();
  HKeyFactoryBase(const HKeyFactoryBase&) = default;
  HKeyFactoryBase& operator=(const HKeyFactoryBase& l) = default;
  virtual ~HKeyFactoryBase();

  virtual void keyPressEvent(HBoard* board, QKeyEvent* event) = 0;
  virtual void keyReleaseEvent(HBoard* board, QKeyEvent* event) = 0;
  void clearKeys();

  QSet<int> keys();

 protected:
  QSet<int> _keys;
};

class HBOARD_EXPORT HKeyFactory : public HKeyFactoryBase {
 public:
  HKeyFactory();

  virtual void keyPressEvent(HBoard* board, QKeyEvent* event) override;
  virtual void keyReleaseEvent(HBoard* board, QKeyEvent* event) override;
};

#endif  // HKEYFACTORY_H
