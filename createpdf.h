#ifndef CREATEPDF_H
#define CREATEPDF_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <configdaten.h>
#include <pdfdaten.h>

class CreatePDF : public QObject
{
    Q_OBJECT
public:
    explicit CreatePDF(QPainter *painter, const PdfDaten *pd, QObject *parent = 0);

signals:

public slots:   
private:
    QPainter *m_painter;
    QPointF transformReal2PDF(QPointF pt);
    QPointF transformRealTarget2PDFBox(QPointF pt, double rd, int size, double pdfboxPixel);
    void drawText(qreal x, qreal y, Qt::Alignment flags,
                  const QString & text, QRectF * boundingRect = 0);
    void drawText(const QPointF & point, Qt::Alignment flags,
                  const QString & text, QRectF * boundingRect = {});
    QList<QPointF> findConvexHull(QList<QPointF> s);
    double theta(QPointF p1, QPointF p2);
    QPolygonF cmr(QPolygonF ch, double tension);
};

#endif // CREATEPDF_H
