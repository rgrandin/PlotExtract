/**
 * @file clickablepixmap.h
 * @author Robert Grandin
 * @date 17 November 2011
 * @brief Definition of ClickablePixmap class.
 *
 * @section Class Description & Notes
 *
 * This class defines a QPixmap which includes signal-emission when the user clicks
 * within the pixmap.
 *
 *
 * All functions contained within this class are intended for use with the GNU
 * C++ compiler (g++).  Use with other compilers may produce unexpected results
 * and such use is at the users' own risk.
 *
 *
 * @section Revisions
 *
 * @date 17 November 2011
 *	- Creation date.
 * @date 26 January 2013
 *  - Extend functionality.
 *
 *
 *
 *
 * @section License
 *
 * Copyright (c) 2011-2013, Robert Grandin
 * All rights reserved.
 *
 * Redistribution and use of this file is permitted provided that the following
 * conditions are met:
 * 	-# 	Redistributions must produce the above copyright notice, this list of
 * 		conditions, and the following disclaimer in the documentation and/or
 * 		other materials provided with the distribution.
 * 	-#	Neither the name of the organization nor the names of its contributors
 * 		may be used to endorse or promote products derived from this software
 * 		without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT
 * LIMITING TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *
 */


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
