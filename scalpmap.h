#ifndef SCALPMAP_H
#define SCALPMAP_H

#include <QWidget>
#include <QPainter>
#include <QtGui>

class ScalpMap : public QWidget
{
    Q_OBJECT
public:
    explicit ScalpMap(int scalling_, QWidget *parent = 0);
    void paintEvent(QPaintEvent *event, QPainter* painter);

protected:
    QColor getElectrodeColor(double value);

protected:
    int scalling;
    QPixmap* scalpmapPixmap;
    QMap<QString, QMap<QString, QVector<double> > > electrodes;
    QString hardware;

signals:
    
public slots:
    void setCq(QString channel, double cq);
    void setHardware(QString hardware_);

};

#endif // SCALPMAP_H
