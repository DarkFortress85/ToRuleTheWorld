#ifndef CMYVIEW_H
#define CMYVIEW_H

#include<QGraphicsView>
#include<QWheelEvent>
#include<QMouseEvent>

class CMyView : public QGraphicsView
{
public:
    CMyView(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override; //Zoomen beim Raddrehen
    void mouseReleaseEvent(QMouseEvent* event) override; //Wenn Maus ausgelassen wird, wieder normaler Zeiger, keine Hand

};

#endif // CMYVIEW_H
