#include <createpdf.h>
#include <QPainter>
#include <QPointF>
#include <configdaten.h>
#include <pdfdaten.h>
#include <QLineF>
#include <QVector>
#include <QPolygonF>


CreatePDF::CreatePDF(QPainter *painter, const PdfDaten *pd, QObject *parent) : QObject(parent)
{
    this->m_painter = painter;
    painter->setRenderHint(QPainter::Antialiasing);

    bool parseOK = false;
    double ringDistance = pd->ringDist.toDouble(&parseOK);
    if(!parseOK){
        return;
    }

    // Auf die Box bezogen
    //*********************************************************
    QPointF sizeBoxX = transformReal2PDF(QPointF(pd->cfg.m_boxSize,0));
    QPointF sizeBoxY = transformReal2PDF(QPointF(0,pd->cfg.m_boxSize));
    QPointF leftUpperBoxPosition  = transformReal2PDF(QPointF(pd->cfg.m_PositionX,pd->cfg.m_PositionY));
    QPointF rightUpperBoxPosition = transformReal2PDF(QPointF(pd->cfg.m_PositionX,pd->cfg.m_PositionY)) + sizeBoxX;
    QPointF center = leftUpperBoxPosition + sizeBoxX/2 + sizeBoxY/2;
    // Header Info
    QPointF pointShooter = rightUpperBoxPosition+QPointF(0,-650);
    QPointF pointLocation= rightUpperBoxPosition+QPointF(0,-400);
    QPointF pointDate    = rightUpperBoxPosition+QPointF(0,-150);
    QPointF pointCaliber = leftUpperBoxPosition+QPointF(0,-650);
    QPointF pointDistance= leftUpperBoxPosition+QPointF(0,-400);
    QPointF pointRingDist= leftUpperBoxPosition+QPointF(0,-150);

    // Auf DinA4 bezogen
    //**************************************************************
//    QPointF dina4LO = transformReal2PDF(QPointF(0,0));
    QPointF dina4MO = transformReal2PDF(QPointF(210/2,0));
//    QPointF dina4RO = transformReal2PDF(QPointF(210,0));
//    QPointF dina4LU = transformReal2PDF(QPointF(0,297));
    QPointF dina4RU = transformReal2PDF(QPoint(210,297));
    QPointF pointSession = dina4MO+QPointF(0,350);

    // Block1 (allgemein)
    QPointF originBlock1 = transformReal2PDF( QPointF(pd->cfg.m_b1x,pd->cfg.m_b1y));
    // Block2 (POA)
    QPointF originBlock2 = transformReal2PDF( QPointF(pd->cfg.m_b2x,pd->cfg.m_b2y));
    // Block3 (COI)
    QPointF originBlock3 = transformReal2PDF( QPointF(pd->cfg.m_b3x,pd->cfg.m_b3y));


    QFont sansFont("Helvetica", 16);
    QFont sansFontFixed("Courier New", 9);
    QFont sansSmallFont("Courier New", 7);
    QPen myPen(Qt::black, 5, Qt::SolidLine);
    QPen dashPen(Qt::black,2, Qt::DashLine);
    QBrush lightgray(Qt::gray);
    lightgray.setStyle(Qt::Dense6Pattern);
    painter->setFont(sansFont);
    painter->setPen(myPen);

    drawText(pointSession,Qt::AlignCenter,pd->sessionName);

    sansFont.setPointSize(11);
    myPen.setWidth(3);
    painter->setFont(sansFont);
    painter->setPen(myPen);

    drawText(pointShooter,Qt::AlignRight,pd->shooterName);
    drawText(pointLocation,Qt::AlignRight,pd->locationName);
    drawText(pointDate,Qt::AlignRight,pd->dateString);

    drawText(pointCaliber,Qt::AlignLeft, "Kaliber");
    drawText(pointDistance,Qt::AlignLeft,"Entfernung: ");
    drawText(pointRingDist,Qt::AlignLeft,"Ringdistanz: ");
    drawText(pointCaliber+QPointF(1000,0) ,Qt::AlignLeft, " : " + pd->caliber);
    drawText(pointDistance+QPointF(1000,0),Qt::AlignLeft," : " + pd->distance + " m");
    drawText(pointRingDist+QPointF(1000,0),Qt::AlignLeft," : " + pd->ringDist + " mm");

    sansFont.setPointSize(6);
    myPen.setWidth(3);
    painter->setFont(sansFont);
    painter->setPen(myPen);

    QString owner = "";
    if(!(pd->cfg.m_regName.isEmpty())){
          owner   = "Registered for " + pd->cfg.m_regName;
    }
    else{
        owner   = "Unregistered version" + pd->cfg.m_regName;
    }
    QString footer =  owner + ". Made with version "+ pd->cfg.m_version +" of Shooty ©2019 Frank Zimmermann (fz@zenmeister.de)";
    drawText(dina4RU+QPointF(-500,-500),Qt::AlignRight,footer);
    qDebug() << "Version: " <<  pd->cfg.m_version;

    // Kalibrierung für PDF Box berechnen und ggfs. verändern
    double maxXY = 0.0;
    foreach(QPointF p, pd->realShots){
        if(qFabs(p.x()) > maxXY){
            maxXY = qFabs(p.x());
        }
        if(qFabs(p.y()) > maxXY){
            maxXY = qFabs(p.y());
        }
    }

    int boxSize=pd->cfg.m_minPDFBox;

    if( ((maxXY/ringDistance)-pd->cfg.m_minPDFBox) > 0){
        boxSize = qCeil(maxXY/ringDistance);
        // In der Grafik unten rechts kenntlich machen
    }
    drawText(rightUpperBoxPosition + sizeBoxY + QPointF(0,130),Qt::AlignRight,"Anzahl Ringe: " + QString::number(boxSize));

// Schussbildbox
    QPointF points[4] = {
        QPointF(leftUpperBoxPosition),
        QPointF(leftUpperBoxPosition+sizeBoxX),
        QPointF(leftUpperBoxPosition+sizeBoxX+sizeBoxY),
        QPointF(leftUpperBoxPosition+sizeBoxY)
    };
    myPen.setWidth(22);
    painter->setFont(sansFont);
    painter->setPen(myPen);
    painter->drawPolygon(points, 4);

// KS für POA zeichnen
    if(pd->cfg.m_poaks){
        QLineF verticalPOA = QLineF(leftUpperBoxPosition+sizeBoxX/2,leftUpperBoxPosition+sizeBoxX/2+sizeBoxY);
        QLineF horizontalPOA = QLineF(leftUpperBoxPosition+sizeBoxY/2,leftUpperBoxPosition+sizeBoxX+sizeBoxY/2);
        myPen.setWidth(10);
        painter->setFont(sansFont);
        painter->setPen(myPen);
        painter->drawLine(verticalPOA);
        painter->drawLine(horizontalPOA);
    }

    // KS für COI zeichnen
    if(pd->cfg.m_coiks){
        QPointF comBox = pd->com;
        QPointF comG;
        comBox.setY(-(pd->com).y());
        comG = transformRealTarget2PDFBox(comBox,ringDistance,boxSize,sizeBoxX.x())+center;
        QPointF top;
        QPointF bottom;
        QPoint left;
        QPoint right;
        top.setX(comG.x());
        top.setY(leftUpperBoxPosition.y());
        bottom.setX(comG.x());
        bottom.setY((leftUpperBoxPosition+sizeBoxY).y());
        left.setX(leftUpperBoxPosition.x());
        left.setY(comG.y());
        right.setX((leftUpperBoxPosition+sizeBoxX).x());
        right.setY(comG.y());
        QLineF verticalCOILine = QLineF(top,bottom);
        QLineF horizontalCOILine = QLineF(left,right);
        painter->setFont(sansFont);
        dashPen.setWidth(2);
        painter->setPen(dashPen);
        painter->drawLine(QLineF(horizontalCOILine));
        painter->drawLine(QLineF(verticalCOILine));
    }
    // Linie zu COI zeichnen
    if(pd->cfg.m_poacoiline){
        QPointF from= QPointF();
        QPointF to  = QPointF((pd->com).x(),-(pd->com).y());
        QPointF a = transformRealTarget2PDFBox(from,ringDistance,boxSize,sizeBoxX.x())+center;
        QPointF b = transformRealTarget2PDFBox(to,ringDistance,boxSize,sizeBoxX.x())+center;
        QLineF offsetLine = QLineF(a,b);
        myPen.setWidth(8);
        painter->setPen(myPen);
        painter->drawLine(offsetLine);

    }


    // Shots zeichnen
    myPen.setWidth(4);
    painter->setPen(myPen);
    foreach(QPointF p, pd->realShots){
        QPointF ps;
        ps.setX(p.x());
        ps.setY(-p.y());
        p = ps;
        QPointF shot = transformRealTarget2PDFBox(p,ringDistance,boxSize,sizeBoxX.x())+center;
        QPointF r = transformRealTarget2PDFBox(QPointF(pd->caliber2,pd->caliber2),ringDistance,boxSize,sizeBoxX.x());
        painter->drawEllipse(shot,r.x(),r.y());
    }
    //  Center Ring
    myPen.setWidth(8);
    painter->setFont(sansFont);
    painter->setPen(myPen);
    painter->drawEllipse(center,70,70);
    painter->drawEllipse(center,90,90);


    // Auswertungswerte
    painter->setFont(sansFontFixed);
    painter->setPen(myPen);
    // 1. Block (allgemein)
    drawText(originBlock1,Qt::AlignLeft,"Allgemein");
    drawText(originBlock1+QPointF(1400,150),Qt::AlignRight,"Treffer: ");
    drawText(originBlock1+QPointF(1400,150),Qt::AlignLeft,pd->anzahl);
    drawText(originBlock1+QPointF(1400,300),Qt::AlignRight,"Ringe: ");
    drawText(originBlock1+QPointF(1400,300),Qt::AlignLeft,pd->rings);
    drawText(originBlock1+QPointF(1400,450),Qt::AlignLeft,"                ( MOA   /  SMOA )");
    drawText(originBlock1+QPointF(1400,600),Qt::AlignRight,"Diagonale (D): ");
    drawText(originBlock1+QPointF(1400,600),Qt::AlignLeft,pd->d);
    drawText(originBlock1+QPointF(1400,750),Qt::AlignRight,"Max Höhe: ");
    drawText(originBlock1+QPointF(1400,750),Qt::AlignLeft,pd->maxhoehe);
    drawText(originBlock1+QPointF(1400,900),Qt::AlignRight,"Max Breite: ");
    drawText(originBlock1+QPointF(1400,900),Qt::AlignLeft,pd->maxbreite);
    drawText(originBlock1+QPointF(1400,1050),Qt::AlignRight,"FOM: ");
    drawText(originBlock1+QPointF(1400,1050),Qt::AlignLeft,pd->fom);
    drawText(originBlock1+QPointF(1400,1200),Qt::AlignRight,"Extr.Spr.(ES): ");
    drawText(originBlock1+QPointF(1400,1200),Qt::AlignLeft,pd->es);

    // Zeichnungen für Rechteck
    if(pd->cfg.m_drechteck){
        // Rechteck zeichnen
        QList<QPointF> p1p2 = pd->d2Points;
        QPointF p;
        QPointF p1;
        QPointF p2;

        p = p1p2.at(0);
        p1 = p;
        p1.setY(-p.y());
        p = p1p2.at(1);
        p2 = p;
        p2.setY(-p.y());

        QPointF p11 = transformRealTarget2PDFBox(p1,ringDistance,boxSize,sizeBoxX.x())+center;
        QPointF p22 = transformRealTarget2PDFBox(p2,ringDistance,boxSize,sizeBoxX.x())+center;
        QPointF points[4] = {
            p11,
            QPointF(p22.x(),p11.y()),
            p22,
            QPointF(p11.x(),p22.y())
        };
        myPen.setWidth(4);
        painter->setPen(myPen);
        painter->setFont(sansFont);
        painter->drawPolygon(points, 4);
    }

    // Zeichnungen für ES
    if(pd->cfg.m_esline){
        // Extreme spread  Zeichnen
        QList<QPointF> p1p2 = pd->es2Points;
        QPointF p;
        QPointF p1;
        QPointF p2;

        p = p1p2.at(0);
        p1 = p;
        p1.setY(-p.y());
        p = p1p2.at(1);
        p2 = p;
        p2.setY(-p.y());

        QPointF p11 = transformRealTarget2PDFBox(p1,ringDistance,boxSize,sizeBoxX.x())+center;
        QPointF p22 = transformRealTarget2PDFBox(p2,ringDistance,boxSize,sizeBoxX.x())+center;
        painter->drawLine(p11,p22);
    }
    myPen.setWidth(4);
    painter->setPen(myPen);
    painter->setFont(sansFontFixed);
    painter->setPen(myPen);

    // 2. Block (auf POA bezogen)
    drawText(originBlock2,Qt::AlignLeft,"Daten mit Bezug auf POA");
    drawText(originBlock2+QPointF(1400,150),Qt::AlignLeft,"                ( MOA   /  SMOA )");
    drawText(originBlock2+QPointF(1400,300),Qt::AlignRight,"Mittl Radius: ");
    drawText(originBlock2+QPointF(1400,300),Qt::AlignLeft,pd->mittlRadiusValue);
    drawText(originBlock2+QPointF(1400,450),Qt::AlignRight,"Offset x: ");
    drawText(originBlock2+QPointF(1400,450),Qt::AlignLeft,pd->shotGroupXValue);
    drawText(originBlock2+QPointF(1400,600),Qt::AlignRight,"Offset y: ");
    drawText(originBlock2+QPointF(1400,600),Qt::AlignLeft,pd->shotGroupYValue);
    drawText(originBlock2+QPointF(1400,750),Qt::AlignRight,"Offset r,phi: ");
    drawText(originBlock2+QPointF(1400,750),Qt::AlignLeft,pd->shotGroup2Value);
    //    drawText(originBlock2+QPointF(1400,900),Qt::AlignRight,"σ (RDM): ");
    //    drawText(originBlock2+QPointF(1400,900),Qt::AlignLeft,pd->sigmaValue);
    //    drawText(originBlock2+QPointF(1400,1050),Qt::AlignRight,"Std.Abw.(X): ");
    //    drawText(originBlock2+QPointF(1400,1050),Qt::AlignLeft,pd->stdabwx);
    //    drawText(originBlock2+QPointF(1400,1200),Qt::AlignRight,"Std.Abw.(Y): ");
    //    drawText(originBlock2+QPointF(1400,1200),Qt::AlignLeft,pd->stdabwy);
    //    drawText(originBlock2+QPointF(1400,1350),Qt::AlignRight,"Std.Abw.(R): ");
    //    drawText(originBlock2+QPointF(1400,1350),Qt::AlignLeft,pd->rds);
    //    drawText(originBlock2+QPointF(1400,1500),Qt::AlignRight,"CEP 50: ");
    //    drawText(originBlock2+QPointF(1400,1500),Qt::AlignLeft,pd->cep50);
    //    drawText(originBlock2+QPointF(1400,1650),Qt::AlignRight,"CEP 90: ");
    //    drawText(originBlock2+QPointF(1400,1650),Qt::AlignLeft,pd->cep90);
    //    drawText(originBlock2+QPointF(1400,1800),Qt::AlignRight,"CEP 95: ");
    //    drawText(originBlock2+QPointF(1400,1800),Qt::AlignLeft,pd->cep95);

    // 3. Block (auf COI bezogen)
    drawText(originBlock3,Qt::AlignLeft,"Daten mit Bezug auf COI");
    drawText(originBlock3+QPointF(1400,150),Qt::AlignLeft,"                ( MOA   /  SMOA )");
    drawText(originBlock3+QPointF(1400,300),Qt::AlignRight,"Mittl Radius: ");
    drawText(originBlock3+QPointF(1400,300),Qt::AlignLeft,pd->mittlRadiusValueC);
    drawText(originBlock3+QPointF(1400,450),Qt::AlignRight,"σ (RDM): ");
    drawText(originBlock3+QPointF(1400,450),Qt::AlignLeft,pd->sigmaValueC);
    drawText(originBlock3+QPointF(1400,600),Qt::AlignRight,"Std.Abw.(X): ");
    drawText(originBlock3+QPointF(1400,600),Qt::AlignLeft,pd->stdabwxC);
    drawText(originBlock3+QPointF(1400,750),Qt::AlignRight,"Std.Abw.(Y): ");
    drawText(originBlock3+QPointF(1400,750),Qt::AlignLeft,pd->stdabwyC);
    drawText(originBlock3+QPointF(1400,900),Qt::AlignRight,"Std.Abw.(R): ");
    drawText(originBlock3+QPointF(1400,900),Qt::AlignLeft,pd->rdsC);
    drawText(originBlock3+QPointF(1400,1050),Qt::AlignRight,"CEP 50: ");
    drawText(originBlock3+QPointF(1400,1050),Qt::AlignLeft,pd->cep50C);
    drawText(originBlock3+QPointF(1400,1200),Qt::AlignRight,"CEP 90: ");
    drawText(originBlock3+QPointF(1400,1200),Qt::AlignLeft,pd->cep90C);
    drawText(originBlock3+QPointF(1400,1350),Qt::AlignRight,"CEP 95: ");
    drawText(originBlock3+QPointF(1400,1350),Qt::AlignLeft,pd->cep95C);

    QList<QPointF> ch;
    QPolygonF chPolygon;

    // Wenn Zeichnungen mit konvexer Huelle gefordert sind
    if(pd->cfg.m_konvex || pd->cfg.m_rundKonvex || pd->cfg.m_rundKonvexCMR){
        ch = findConvexHull(pd->realShots);
        foreach(QPointF p, ch){
            QPointF tmp = p;
            p.setY(-tmp.y());
            chPolygon << transformRealTarget2PDFBox(p,ringDistance,boxSize,sizeBoxX.x())+center;
        }
        qDebug("Ist geschlossen? %s", (chPolygon.isClosed())?"ja":"nein");
        qDebug("Anzahl der Punkte: %d", chPolygon.length());
    }

    // Zeichnungen für konvexe Huelle
    if(pd->cfg.m_konvex){
        if(pd->realShots.length()>2){
            myPen.setWidth(4);
            painter->setFont(sansFont);
            painter->setPen(myPen);
            painter->setBrush(lightgray);
            painter->drawPolygon(chPolygon);
        }
    }


    // Zeicnungen für runde konvexe Huelle
    if(pd->cfg.m_rundKonvex){
        double alpha = 1.0/(pd->cfg.m_rundPrecision + 2.0);
        QPolygonF pN;
        if(pd->realShots.length()>2){
            QPolygonF polygon = QPolygonF(chPolygon);
            polygon << polygon.first();   // schliessen
            for(int k=0; k< 9; k++){  // Anzahl der Iterationen k
                pN.clear();
                for(int i=0; i<polygon.length()-1 ;i++){
                    pN << polygon[i] + (polygon[i+1]-polygon[i])*alpha;
                    pN << polygon[i] + (polygon[i+1]-polygon[i])*(1-alpha);
                }
                polygon.clear();
                polygon = QPolygonF(pN);
                polygon << polygon.first();
                qDebug("Anzahl der Elemente: %d",polygon.length());
            }
            myPen.setWidth(4);
            painter->setFont(sansFont);
            painter->setPen(myPen);
            painter->setBrush(lightgray);
            painter->drawPolygon(pN);
        }
    }


    // Zeichnungen für catmull-rom
    if(pd->cfg.m_rundKonvexCMR){
        QPolygonF cmrPolygon;
        if(pd->realShots.length()>3){
            double tension = pd->cfg.m_rundSpannung;
            cmrPolygon = cmr(chPolygon,tension);
            myPen.setWidth(8);
            painter->setFont(sansFont);
            painter->setPen(myPen);
            painter->setBrush(lightgray);
            painter->drawPolygon(cmrPolygon);
        }
    }


    // Kreise drucken
    QPointF theCenter  = QPointF((pd->com).x(),-(pd->com).y());
    QPointF pdfCenter = transformRealTarget2PDFBox(theCenter,ringDistance,boxSize,sizeBoxX.x())+center;
    if(pd->cfg.m_circleR){
        myPen.setWidth(6);
        painter->setPen(myPen);
        painter->setBrush(Qt::NoBrush);
        QPointF radius = QPointF(pd->circleR,0);
        QPointF virtPoint = transformRealTarget2PDFBox(radius,ringDistance,boxSize,sizeBoxX.x());
        painter->drawEllipse(pdfCenter,virtPoint.x(),virtPoint.x());
    }
    if(pd->cfg.m_circleSigma){
        myPen.setWidth(6);
        painter->setPen(myPen);
        painter->setBrush(Qt::NoBrush);
        QPointF radius = QPointF(pd->circleSigma,0);
        QPointF virtPoint = transformRealTarget2PDFBox(radius,ringDistance,boxSize,sizeBoxX.x());
        painter->drawEllipse(pdfCenter,virtPoint.x(),virtPoint.x());
    }
    if(pd->cfg.m_circleCEP50){
        myPen.setWidth(6);
        painter->setPen(myPen);
        painter->setBrush(Qt::NoBrush);
        QPointF radius = QPointF(pd->circleCEP50,0);
        QPointF virtPoint = transformRealTarget2PDFBox(radius,ringDistance,boxSize,sizeBoxX.x());
        painter->drawEllipse(pdfCenter,virtPoint.x(),virtPoint.x());
    }
    if(pd->cfg.m_circleCEP90){
        myPen.setWidth(6);
        painter->setPen(myPen);
        painter->setBrush(Qt::NoBrush);
        QPointF radius = QPointF(pd->circleCEP90,0);
        QPointF virtPoint = transformRealTarget2PDFBox(radius,ringDistance,boxSize,sizeBoxX.x());
        painter->drawEllipse(pdfCenter,virtPoint.x(),virtPoint.x());
    }
    if(pd->cfg.m_circleCEP95){
        myPen.setWidth(6);
        painter->setPen(myPen);
        painter->setBrush(Qt::NoBrush);
        QPointF radius = QPointF(pd->circleCEP95,0);
        QPointF virtPoint = transformRealTarget2PDFBox(radius,ringDistance,boxSize,sizeBoxX.x());
        painter->drawEllipse(pdfCenter,virtPoint.x(),virtPoint.x());
    }
    if(pd->cfg.m_circleRinge){
        myPen.setWidth(3);
        painter->setPen(myPen);
        int ring = ringDistance * boxSize;
        while(ring > 0){
            myPen.setWidth(2);
            painter->setPen(myPen);
            painter->setBrush(Qt::NoBrush);
            QPointF radius = QPointF(ring,0);
            QPointF virtPoint = transformRealTarget2PDFBox(radius,ringDistance,boxSize,sizeBoxX.x());
            painter->drawEllipse(center,virtPoint.x(),virtPoint.x());
            ring -= ringDistance;
        }
    }

    // Koordinaten drucken
    if(pd->cfg.m_trefferkoordinaten){
        if(pd->koordinaten->count() <= 120){
            painter->setFont(sansSmallFont);
            int dy = 100;
            QPointF pos = leftUpperBoxPosition+sizeBoxX+QPointF(50,0);
            drawText(pos + QPointF(400,-8 *dy),Qt::AlignLeft,"Treffer Koordinaten (x,y,r,phi)");
            for(int i=0; i< pd->koordinaten->count();i++){

                drawText(pos + QPoint(0,-5*dy) + QPointF(0,dy*i),Qt::AlignLeft,pd->koordinaten->item(i)->text());
            }
        }
        else{
            QMessageBox msgBox;
            msgBox.setText("Mehr als 120 Treffer werden nicht als Koordinaten ausgegeben (ggfs. als CSV exportieren und drucken).");
            msgBox.exec();
        }
    }
}

QPointF CreatePDF::transformReal2PDF(QPointF pt)
{
    double fact= 47.2441;  // 1mm = 47.2441 points
    QPointF pPDF;
    pPDF = pt*fact;
    return pPDF;
}

QPointF CreatePDF::transformRealTarget2PDFBox(QPointF pt,double rd,int size,double pdfboxPixel)
{
    double z = pdfboxPixel;
    double n = 2* (size*rd);
    double fact= z/n;  // 1mm = ??? points
    QPointF pPDF;
    pPDF = pt*fact;
    return pPDF;
}



void CreatePDF::drawText(qreal x, qreal y, Qt::Alignment flags,
              const QString & text, QRectF * boundingRect)
{
   const qreal size = 32767.0;
   QPointF corner(x, y - size);
   if (flags & Qt::AlignHCenter) corner.rx() -= size/2.0;
   else if (flags & Qt::AlignRight) corner.rx() -= size;
   if (flags & Qt::AlignVCenter) corner.ry() += size/2.0;
   else if (flags & Qt::AlignTop) corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect{corner.x(), corner.y(), size, size};
   m_painter->drawText(rect, flags, text, boundingRect);
}

void CreatePDF::drawText(const QPointF & point, Qt::Alignment flags,
              const QString & text, QRectF * boundingRect )
{
   drawText(point.x(), point.y(), flags, text, boundingRect);
}

QList<QPointF> CreatePDF::findConvexHull(QList<QPointF> s)
{
    QList<QPointF> convexHull;
    QVector<QPointF> p(s.length()+2);
    QPointF bottom;
    int N = s.length();
    int j=0;
    foreach(QPointF point, s){
        p[j++]=point;
    }

    int i,min,M;
    double th,v;
    int lastIndex = 0;
    for(min=0,i=1;i<N;i++){
        if( p[i].y() < p[min].y() ){
            min = i;
        }
    }
    p[N] = p[min];
    th = 0.0;
    for(M=0;M<N;M++){
        std::swap(p[M],p[min]);
        min = N;
        v= th;
        th = 360.0;
        for(i=M+1;i<=N;i++)
            if(theta(p[M],p[i]) > v)
                if(theta(p[M],p[i]) < th){
                    min = i;
                    th = theta(p[M],p[min]);
                }
        if(min ==N){
            lastIndex = M;
            break;
        }
    }
    for(int i=0;i<=M; i++){
        convexHull.append(p[i]);
    }

    return convexHull;
}

double CreatePDF::theta(QPointF p1, QPointF p2){
    double dx, dy, ax,ay;
    double t;
    dx = p2.x() - p1.x();
    ax = qFabs(dx);
    dy = p2.y() - p1.y();
    ay = qFabs(dy);
    if(ax+ay == 0){
        t=0;
    }
    else{
        t = dy/(ax+ay);
    }
    if(dx<0){
        t = 2-t;
    }
    else{
        if(dy<0){
            t = 4+t;
        }
    }
    return t*90.0;
}

QPolygonF CreatePDF::cmr(QPolygonF ch, double tension){
    QPolygonF cmrPolygon;
    tension /= 10.0;
    double alpha = 0.5;
    int pointsPerSegment=20;
    QPointF firstPoint = ch.first();
    QPointF lastPoint  = ch.last();
    ch.append(firstPoint);   // Kurve jetzt geschlossen
    ch.insert(0,lastPoint);
    ch.append(ch.at(2));

    double t;
    double t0;
    double t1;
    double t2;
    double t3;
    QPointF m1;
    QPointF m2;
    for(int j=0; j< ch.length()-3;j++){
        QPointF a;
        QPointF b;
        QPointF c;
        QPointF d;
        QPointF p0 = ch[j];
        QPointF p1 = ch[j+1];
        QPointF p2 = ch[j+2];
        QPointF p3 = ch[j+3];

        t0 = 0.0;
        t1 = t0 + pow(QLineF(p0,p1).length(),alpha);
        t2 = t1 + pow(QLineF(p1,p2).length(),alpha);
        t3 = t2 + pow(QLineF(p2,p3).length(),alpha);

        m1 = (1.0 -tension) * (t2-t1) * ((p1-p0) / (t1-t0) - (p2-p0) / (t2-t0) + (p2-p1) / (t2-t1));
        m2 = (1.0 -tension) * (t2-t1) * ((p2-p1) / (t2-t1) - (p3-p1) / (t3-t1) + (p3-p2) / (t3-t2));
        a = 2.0 * (p1-p2) + m1 + m2;
        b = -3.0 * (p1-p2) - m1 - m1 - m2;
        c = m1;
        d = p1;

        double step = 1.0/pointsPerSegment;
        for(int i = 0; i<pointsPerSegment; i++){
            t = i * step;
            QPointF pointT = a *  t * t * t + b * t * t + c * t + d;
            cmrPolygon << pointT;
        }
    }
    return cmrPolygon;
}
