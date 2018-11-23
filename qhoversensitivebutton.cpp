#include "qhoversensitivebutton.h"

QHoverSensitiveButton::QHoverSensitiveButton(QWidget *parent) : QPushButton(parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

void QHoverSensitiveButton::hoverEnter(QHoverEvent *)
{
    QFont font = this->font();
    font.setBold(true);
    this->setFont(font);
    repaint();
}

void QHoverSensitiveButton::hoverLeave(QHoverEvent *)
{
    QFont font = this->font();
    font.setBold(false);
    this->setFont(font);
    repaint();
}

void QHoverSensitiveButton::hoverMove(QHoverEvent *)
{
    QFont font = this->font();
    font.setBold(true);
    this->setFont(font);
    repaint();
}

bool QHoverSensitiveButton::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(event));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(event);
}
