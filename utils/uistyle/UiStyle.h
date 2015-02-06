#ifndef UISTYLE_H
#define UISTYLE_H

#include <QObject>
#include <QJsonObject>

class UiStyle : public QObject
{
public:
    explicit UiStyle(QObject *parent = 0);
    ~UiStyle();
    QJsonObject asJSON();

    // Variables
    bool map_follow; // Map follows agent
    QString color; // Agent geometry inner color
    QString border_color; // Agent geometry border color
    int border_weight; // Agent geometry border weight
    double fill_opacity; // Agent geometry inner opacity
    double border_opacity; // Agent geometry border opacity
    QString icon_url; // Agent geometry (only for points) icon
};

#endif // UISTYLE_H
