#include "traffic_light.h"

#include <QPainter>

TrafficLight::TrafficLight(QWidget* parent)
    : QWidget(parent)
{
    m_color = grey;
}

void TrafficLight::setColor(Color color) // const &
{
    m_color = color;
    update();
}

void TrafficLight::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.setBrush(QColor("#730507"));
    painter.drawRoundedRect(rect(), width() * 0.1, width() * 0.1);

    // circle size
    int diameter = qMin(width(), height()) * 0.4;

    // horizontal position
    int x = (width() - diameter) / 2;

    // vertical positions
    int topY = height() * 0.05;
    int middleY = height() * 0.375;
    int bottomY = height() * 0.699;

    // TOP
    if (m_color == red)
        painter.setBrush(Qt::red);
    else
        painter.setBrush(Qt::gray);

    painter.drawEllipse(x, topY, diameter, diameter);

    // MIDDLE
    if (m_color == yellow)
        painter.setBrush(Qt::yellow);
    else
        painter.setBrush(Qt::gray);

    painter.drawEllipse(x, middleY, diameter, diameter);

    // BOTTOM
    if (m_color == green)
        painter.setBrush(Qt::green);
    else
        painter.setBrush(Qt::gray);

    painter.drawEllipse(x, bottomY, diameter, diameter);
}