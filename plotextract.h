#ifndef PLOTEXTRACT_H
#define PLOTEXTRACT_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QPen>
#include <QFile>
#include <QTextStream>
#include <QColorDialog>

#include "clickablepixmap.h"

#include <DLList.h>
#include <Array2D.h>

namespace Ui {
    class PlotExtract;
}


class PlotExtract : public QMainWindow
{
    Q_OBJECT

public:
    /**
      @brief Constructor.
      */
    explicit PlotExtract(QWidget *parent = 0);


    /**
      @brief Destructor
      */
    ~PlotExtract();




private slots:
    /**
      @brief Get name of input file to be loaded.
      @pre PlotExtract object exists.
      @post Image file loaded and displayed.
      @return None.
      */
    void GetInputFile();


    /**
      @brief Actions performed when the user clicks a point in the image.
      @pre PlotExtract object exists.
      @param x Global x-coordinate of mouse cursor during click.
      @param y Global y-coordinate of mouse cursor during click.
      @post Ellipse drawn on image indicating location of click.  Ellipse
            parameters dependent upon type of marker (axis or data) and
            marker options set elsewhere.
      @return None.
      */
    void OnClickInImage(qint64 x, qint64 y);


    /**
      @brief Signify that next point clicked is the starting reference point
             on the horizontal axis.
      @pre PlotExtract object exists.
      @post Member pointer for ellipse at start point on horizontal axis updated.
      @return None.
      */
    void OnHorizontalStartClicked();


    /**
      @brief Signify that next point clicked is the ending reference point
             on the horizontal axis.
      @pre PlotExtract object exists.
      @post Member pointer for ellipse at end point on horizontal axis updated.
      @return None.
      */
    void OnHorizontalEndClicked();


    /**
      @brief Signify that next point clicked is the starting reference point
             on the vertical axis.
      @pre PlotExtract object exists.
      @post Member pointer for ellipse at start point on vertical axis updated.
      @return None.
      */
    void OnVerticalStartClicked();


    /**
      @brief Signify that next point clicked is the ending reference point
             on the vertical axis.
      @pre PlotExtract object exists.
      @post Member pointer for ellipse at end point on vertical axis updated.
      @return None.
      */
    void OnVerticalEndClicked();


    /**
      @brief Remove last-selected data point.
      @pre PlotExtract object exists.
      @post Last node in list of selected data points is removed.
      @return None.
      */
    void OnUndoLastClicked();


    /**
      @brief Delete all data markers from plot.
      @pre PlotExtract object exists.
      @post Axis-definition markers untouched.  All data points removed.
      @return None.
      */
    void OnClearAllClicked();


    /**
      @brief Determine location of each point, sort, and write to file.
      @pre PlotExtract object exists.  List of selected data points must
           contain at least 1 node, and both axes must be defined.
      @post No changes to member lists.  Temporary variables created for
            coordinate transformation, sorting, and output.
      @return None.
      */
    void OnSaveClicked();


    /**
      @brief Update display of horizontal axis-definition markers.
      @pre PlotExtract object exists.
      @post Markers updated to reflect updated size/color.
      @return None.
      */
    void OnHorizontalAxisDefinitionMarkersChanged();


    /**
      @brief Update display of vertical axis-definition markers.
      @pre PlotExtract object exists.
      @post Markers updated to reflect updated size/color.
      @return None.
      */
    void OnVerticalAxisDefinitionMarkersChanged();


    /**
      @brief Update display of data markers.
      @pre PlotExtract object exists.
      @post Markers updated to reflect updated size/color.
      @return None.
      */
    void OnDataMarkersChanged();


    /**
      @brief Select color for horizontal axis-definition markers.
      @pre PlotExtract object exists.
      @post Markers updated to reflect updated color.
      @return None.
      */
    void OnHorizontalAxisColorClicked();


    /**
      @brief Select color for vertical axis-definition markers.
      @pre PlotExtract object exists.
      @post Markers updated to reflect updated color.
      @return None.
      */
    void OnVerticalAxisColorClicked();


    /**
      @brief Select color for data markers.
      @pre PlotExtract object exists.
      @post Markers updated to reflect updated color.
      @return None.
      */
    void OnDataColorClicked();


    /**
      @brief Show dialog box with instructions.
      @pre PlotExtract object exists.
      @post No change to object.
      @return None.
      */
    void OnMenuInstructionsClicked() const;



private:

    /** @brief Pointer to interface */
    Ui::PlotExtract *ui;

    /** @brief Graphics scene */
    QGraphicsScene *scene;

    /** @brief Plot image */
    ClickablePixmap *myimg;

    /** @brief Pen used for marking selected points */
    QPen *pen;

    /** @brief Brush used for marking selected points */
    QBrush *brush;

    /** @brief Name of input file */
    QString inputfile;

    /** @brief Number of points from the plot itself */
    qint64 npoints;

    /** @brief Size of markers used for horizontal axis-definition */
    qreal axes_markersize_horizontal;

    /** @brief Size of markers used for vertical axis-definition */
    qreal axes_markersize_vertical;

    /** @brief Size of markers used in plot */
    qreal markersize;

    /** @brief Color of markers for horizontal axis-definition */
    QColor axis_color_horizontal;

    /** @brief Color of markers for vertical axis-definition */
    QColor axis_color_vertical;

    /** @brief Color of markers for data points */
    QColor markercolor;

    /** @brief Track if next click in image specifies the starting point on horizontal axis */
    bool select_horizontal_start;

    /** @brief Track if next click in image specifies the ending point on horizontal axis */
    bool select_horizontal_end;

    /** @brief Track if next click in image specifies the starting point on vertical axis */
    bool select_vertical_start;

    /** @brief Track if next click in image specifies the ending point on vertical axis */
    bool select_vertical_end;

    /** @brief List of data points */
    DLList<qreal,2> datalist;

    /** @brief Array of starting and ending pixel coordinates for axes-definition points */
    Array2D<qreal> endpoints;

    /** @brief Upper-left point of the graphics scene */
    QPoint *scenerefpt;

    /** @brief Marker for horizontal axis start point */
    QGraphicsEllipseItem *horizontal_start_ellipse;

    /** @brief Marker for horizontal axis end point */
    QGraphicsEllipseItem *horizontal_end_ellipse;

    /** @brief Marker for vertical axis start point */
    QGraphicsEllipseItem *vertical_start_ellipse;

    /** @brief Marker for vertical axis end point */
    QGraphicsEllipseItem *vertical_end_ellipse;

    /** @brief List of markers for data points */
    DLList<QGraphicsEllipseItem*,1> markerlist;



};




#endif // PLOTEXTRACT_H
