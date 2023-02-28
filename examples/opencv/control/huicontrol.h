#ifndef HUICONTROL_H
#define HUICONTROL_H

#include <QObject>

class HUIControl : public QObject
{
    Q_OBJECT
public:
    explicit HUIControl(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HUICONTROL_H
