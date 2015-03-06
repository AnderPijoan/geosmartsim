#include "UiStyle.h"

UiStyle::UiStyle(QObject *parent) : QObject(parent){
    this->map_follow = false;
    this->border_weight = -1;
    this->fill_opacity = -1;
    this->border_opacity = -1;
    this->icon_url = "http://cdn.flaticon.com/png/32/33622.png";
}

UiStyle::~UiStyle(){
}

QJsonObject UiStyle::asJSON(){
    QJsonObject properties;
     if(!this->map_follow) { properties.insert("map_follow" , this->map_follow); }
    if(!this->color.isEmpty()) { properties.insert("color" , this->color); }
    if(!this->border_color.isEmpty()) { properties.insert("border_color" , this->border_color); }
    if(this->border_weight > -1) { properties.insert("weight" , this->border_weight); }
    if(this->fill_opacity > -1) { properties.insert("fill_opacity" , this->fill_opacity); }
    if(this->border_opacity > -1) { properties.insert("border_opacity" , this->border_opacity); }
    if(!this->icon_url.isEmpty()) { properties.insert("icon_url" , this->icon_url); }
    return properties;
}
