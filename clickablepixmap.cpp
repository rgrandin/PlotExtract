/**
 * @file clickablepixmap.cpp
 * @author Robert Grandin
 * @date 17 November 2011
 * @brief Implementation of ClickablePixmap class.
 */


#include "clickablepixmap.h"

ClickablePixmap::ClickablePixmap(QWidget *parent) :
    QWidget(parent)
{
    pixmap = NULL;

    setCursor(Qt::CrossCursor);

    added_to_scene = false;
}


ClickablePixmap::~ClickablePixmap()
{
    if(pixmap){
        delete pixmap;
    }
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

    added_to_scene = true;
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
    *pixmap = pixmap->scaled(size,Qt::KeepAspectRatio, Qt::SmoothTransformation);
}


void ClickablePixmap::RemoveFromScene(QGraphicsScene *scene)
{
    QList<QGraphicsItem*> itemlist;
    itemlist = scene->items();
    for(int i=0; i<itemlist.size(); i++){
        scene->removeItem(itemlist.value(i));
    }

    added_to_scene = false;
}


QSize ClickablePixmap::GetSize() const
{
    return pixmap->size();
}

bool ClickablePixmap::AddedToScene() const
{
    return added_to_scene;
}
