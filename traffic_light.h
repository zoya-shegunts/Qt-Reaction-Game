#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <QWidget>

enum Color {
    grey,
    red,
    yellow,
    green
};

class TrafficLight : public QWidget
{
    Q_OBJECT

public:
    TrafficLight(QWidget* parent = nullptr);
    void setColor(Color color);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Color m_color;
};

#endif // TRAFFIC_LIGHT_H
