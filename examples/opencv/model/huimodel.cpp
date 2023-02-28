#include "huimodel.h"

HUIModel::HUIModel(QObject *parent) : QObject(parent) {}

int HUIModel::checkedSource(const QString &board, bool checked) {
  _checked_source.insert(board, checked);
  sourceCheckedChanged();
  return 0;
}

void HUIModel::setSourceChecked(const QJsonObject &o) {
  _checked_source = o;
  sourceCheckedChanged();
}

QJsonObject HUIModel::sourceChecked() { return _checked_source; }

void HUIModel::setDest(const QString &d) {
  if (_dest_board != d) {
    _dest_board = d;
    destChanged();
  }
}

QString HUIModel::dest() { return _dest_board; }
