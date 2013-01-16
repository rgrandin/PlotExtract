#include "myviewer.h"

MyViewer::MyViewer(QWidget *parent) :
    QWidget(parent)
{
}


bool MyViewer::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    update();
    return true;
}



void MyViewer::mousePressEvent(QMouseEvent *event)
{
    // Get Point
    QPoint p = mapFromGlobal(QCursor::pos());
    qint64 x = p.x();
    qint64 y = p.y();
    QMessageBox msgBox;
    QString message;
    message = "Point Coordinates: (";
    message.append(QString("%1").arg(x));
    message.append(",");
    message.append(QString("%1").arg(y));
    message.append(")");
    msgBox.setText(message);
    msgBox.setWindowTitle("Message");
    msgBox.exec();
}


void MyViewer::resizeImage(QImage *image, const QSize &newSize)
 {
     if (image->size() == newSize)
         return;

     QImage newImage(newSize, QImage::Format_RGB32);
     newImage.fill(qRgb(255, 255, 255));
     QPainter painter(&newImage);
     painter.drawImage(QPoint(0, 0), *image);
     *image = newImage;
 }
