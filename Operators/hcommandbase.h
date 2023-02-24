#ifndef HCOMMANDBASE_H
#define HCOMMANDBASE_H
#include <memory>
#include <stack>

#include "../HBoard_global.h"
class HBoardActionBase;
class HBOARD_EXPORT HCommandBase {
 public:
  HCommandBase();
  HCommandBase(const HCommandBase &other) = default;
  HCommandBase &operator=(const HCommandBase &other) = default;
  virtual ~HCommandBase();

  virtual int excute(std::shared_ptr<HBoardActionBase>) = 0;
  virtual int undo() = 0;
  virtual int redo() = 0;
};

class HBOARD_EXPORT HCommand : public HCommandBase {
 public:
  HCommand();

  virtual int excute(std::shared_ptr<HBoardActionBase>) override;
  virtual int undo() override;
  virtual int redo() override;

 public:
  void clearExcute();
  void clearUndos();

 protected:
  std::stack<std::shared_ptr<HBoardActionBase>> _excutes;
  std::stack<std::shared_ptr<HBoardActionBase>> _undos;
};

#endif  // HCOMMANDBASE_H
