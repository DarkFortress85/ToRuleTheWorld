#include "myview.h"


CMyView::CMyView(QWidget* parent) : QGraphicsView(parent)
{   
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); //Basis soll unter der Maus bleiben
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag); //verschieben der Karte mit Linker Maustaste
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);//Nur geänderte Bereiche werden neu gezeichnet
    viewport()->setCursor(Qt::ArrowCursor);
}

void CMyView::wheelEvent(QWheelEvent* event)
{
    int numDegrees = event->angleDelta().y() / 8;
    double scaleFactor = qPow(1.2,numDegrees/15.0);
    scale(scaleFactor, scaleFactor);
}

void CMyView::mouseReleaseEvent(QMouseEvent* event)
{

   QGraphicsView::mouseReleaseEvent(event); //zusäztlicher Aufruf der Funktionalitäten der Basisklasse zur Sicherheit
   viewport()->setCursor(Qt::ArrowCursor);
}
