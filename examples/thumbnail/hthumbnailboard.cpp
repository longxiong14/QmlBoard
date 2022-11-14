#include "hthumbnailboard.h"

#include "hthumbailhandle.h"
HThumbnailBoard::HThumbnailBoard() {
  setHandle(std::make_shared<HThumbailHandle>());
}
