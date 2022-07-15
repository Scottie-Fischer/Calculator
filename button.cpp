#include "button.h"

myButton::myButton(const QString &text, QWidget *parent):QToolButton(parent){
    setText(text);
}

QSize myButton::sizeHint() const{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(),size.height());
    return size;
}
