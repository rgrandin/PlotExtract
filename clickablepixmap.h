#ifndef CLICKABLEPIXMAP_H
#define CLICKABLEPIXMAP_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPixmap>
#include <QPointer>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <math.h>

class ClickablePixmap : public QWidget
{
    Q_OBJECT
public:
    /**
      @brief Auto-generated constructor by Qt Creator wizard.
      */
    explicit ClickablePixmap(QWidget *parent = 0);


    /**
     * @brief Destructor.
     */
    virtual ~ClickablePixmap();

    /**
      @brief Open image as pixmap.
      @pre ClickablePixmap object exists.
      @param filename Name of file to be read into pixmap.
      @post File read and pixmap defined based on file.
      @return None.
      */
    void OpenImage(QString filename);


    /**
      @brief Add this widget to a scene.
      @pre ClickablePixmap object exists.
      @param scene Pointer to scene which is to display this
            widget and the pixmap.
      @post Widget scaled to match pixmap size.
      @return None.
      */
    void AddToScene(QGraphicsScene *scene);


    /**
      @brief Scale image.
      @pre ClickablePixmap object exists and image file has been
            read into pixmap.
      @param size Desired size of pixmap.
      @post Pixmap scaled to limiting size.  Aspect ratio is preserved.
      @return None.
      */
    void ScaleImage(QSize size);


    /**
      @brief Remove this widget and pixmap from scene.
      @pre ClickablePixmap object exists.
      @param scene Scene from which this widget and pixmap shall be removed.
      @post No change to this object.
      @return None.
      */
    void RemoveFromScene(QGraphicsScene *scene);


    /**
      @brief Get the size of the pixmap contained in this widget.
      @pre ClickablePixmap object exists and image file has been
            read into pixmap.
      @post No change to this object.
      @return Size of pixmap.
      */
    QSize GetSize() const;


    /**
     * @brief AddedToScene checks if this pixmap has been added to a scene.
     * @return True if pixmap has been added to a scene.  False if not.
     */
    bool AddedToScene() const;


protected:
    /**
      @brief Mouse button pressed.
      @warning This is triggered when any mouse button is pressed.
      */
    void mousePressEvent(QMouseEvent *event);


private:
    /** @brief Pixmap which will contain the image */
    QPixmap *pixmap;

    /** @brief Tracks if pixmap was added to a scene. */
    bool added_to_scene;


signals:
    /** @brief Global coordinates of mouse position when left-clicked */
    void MouseClicked(qint64 x, qint64 y);

public slots:

};

#endif // CLICKABLEPIXMAP_H
