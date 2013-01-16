#include "clickablepixmap.h"

ClickablePixmap::ClickablePixmap(QWidget *parent) :
    QWidget(parent)
{
    pixmap = NULL;

    setCursor(Qt::CrossCursor);
}



void ClickablePixmap::OpenImage(QString filename)
{
    if(pixmap == NULL){
        pixmap = new QPixmap;
        pixmap->load(filename);
    } else {
        delete pixmap;
        pixmap = new QPixmap;
        pixmap->load(filename);
    }
}



void ClickablePixmap::AddToScene(QGraphicsScene *scene)
{
    // Size this widget to match the pixmap size, and add both this
    // widget and the pixmap to the scene.

    qreal pixwidth = pixmap->width();
    qreal pixheight = pixmap->height();

    QRect pixrect = QRect(0,0,pixwidth,pixheight);
    this->setGeometry(pixrect);
    scene->addWidget(this);
    scene->addPixmap(*pixmap);
}



void ClickablePixmap::mousePressEvent(QMouseEvent *event)
{
    // Get location of click in global coordinates
    QPoint p = mapFromGlobal(QCursor::pos());
    qint64 x = p.x();
    qint64 y = p.y();

    // Emit signal containing (x,y) coordinates only for left-click.
    if(event->button() == Qt::LeftButton){
        emit MouseClicked(x,y);
    }
}


void ClickablePixmap::ScaleImage(QSize size)
{
    /*
      Only scale pixmap if it is larger than the given size.
      If the pixmap does need to be scaled, the limiting dimension
      is found so that the image isn't distorted.
     */

    if(pixmap->width() > size.width() || pixmap->height() > size.height()){
        qreal diffwidth = size.width() - pixmap->width();
        qreal diffheight = size.height() - pixmap->height();
        diffwidth = sqrt(diffwidth*diffwidth);
        diffheight = sqrt(diffheight*diffheight);

        if(diffwidth > diffheight){
            *pixmap = pixmap->scaledToWidth(size.width());
        } else {
            *pixmap = pixmap->scaledToHeight(size.height());
        }
    }
}


void ClickablePixmap::RemoveFromScene(QGraphicsScene *scene) const
{
    QList<QGraphicsItem*> itemlist;
    itemlist = scene->items();
    for(int i=0; i<itemlist.size(); i++){
        scene->removeItem(itemlist.value(i));
    }
}


QSize ClickablePixmap::GetSize() const
{
    return pixmap->size();
}
