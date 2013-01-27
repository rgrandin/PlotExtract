#include "plotextract.h"
#include "ui_plotextract.h"



PlotExtract::PlotExtract(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PlotExtract)
{
    ui->setupUi(this);

    // 'new' commands for each member pointer
    scene = new QGraphicsScene;
    myimg = new ClickablePixmap;
    pen = new QPen;
    brush = new QBrush;
    scenerefpt = new QPoint;


    ui->txt_horizontal_marker_size->setText("7");
    ui->txt_vertical_marker_size->setText("7");
    ui->txt_marker_size->setText("7");

    ui->rdo_sort_ascending->setChecked(true);
    ui->rdo_sort_horizontal_axis->setChecked(true);


    /*
      Connect slots associated with each GUI widget
     */
    // File -> Open menu item
    connect( ui->mnu_open, SIGNAL(triggered()), this, SLOT(GetInputFile()));

    // File -> Save menu item
    connect( ui->mnu_save, SIGNAL(triggered()), this, SLOT(OnSaveClicked()));

    // File -> Quit menu item
    connect( ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    // Help -> Instructions menu item
    connect( ui->mnu_instructions, SIGNAL(triggered()), this, SLOT(OnMenuInstructionsClicked()));

    // Help -> Credits menu item
    connect(ui->actionCredits, SIGNAL(triggered()), this, SLOT(OnMenuActionHelpCreditsActivated()));

    // Connect 'Select Image' button to callback to identify the input file
    connect( ui->btn_select_image, SIGNAL(clicked()), this, SLOT(GetInputFile()));

    // Connect to 'MouseClicked' signal of my ClickablePixmap class
    connect( myimg, SIGNAL(MouseClicked(qint64,qint64)), this, SLOT(OnClickInImage(qint64,qint64)));


    // Connect endpoint-select buttons to slots
    connect( ui->btn_horizontal_start, SIGNAL(clicked()), this, SLOT(OnHorizontalStartClicked()));
    connect( ui->btn_horizontal_end, SIGNAL(clicked()), this, SLOT(OnHorizontalEndClicked()));
    connect( ui->btn_vertical_start, SIGNAL(clicked()), this, SLOT(OnVerticalStartClicked()));
    connect( ui->btn_vertical_end, SIGNAL(clicked()), this, SLOT(OnVerticalEndClicked()));

    // Undo button
    connect( ui->btn_undo, SIGNAL(clicked()), this, SLOT(OnUndoLastClicked()));

    // Save button
    connect( ui->btn_save, SIGNAL(clicked()), this, SLOT(OnSaveClicked()));

    // Quit button
    connect(ui->btn_quit, SIGNAL(clicked()), this, SLOT(close()));

    // Marker color-selection
    connect( ui->btn_horizontal_color_dlg, SIGNAL(clicked()), this, SLOT(OnHorizontalAxisColorClicked()));
    connect( ui->btn_vertical_color_dlg, SIGNAL(clicked()), this, SLOT(OnVerticalAxisColorClicked()));
    connect( ui->btn_marker_color_dlg, SIGNAL(clicked()), this, SLOT(OnDataColorClicked()));

    // Clear-All button
    connect( ui->btn_clearall, SIGNAL(clicked()), this, SLOT(OnClearAllClicked()));

    // Marker size-selection
    connect( ui->txt_horizontal_marker_size, SIGNAL(editingFinished()), this, SLOT(OnHorizontalAxisDefinitionMarkersChanged()));
    connect( ui->txt_vertical_marker_size, SIGNAL(editingFinished()), this, SLOT(OnVerticalAxisDefinitionMarkersChanged()));
    connect( ui->txt_marker_size, SIGNAL(editingFinished()), this, SLOT(OnDataMarkersChanged()));


    // Image control
    connect(ui->btn_zoom_in, SIGNAL(clicked()), this, SLOT(OnBtnZoomInClicked()));
    connect(ui->btn_zoom_out, SIGNAL(clicked()), this, SLOT(OnBtnZoomOutClicked()));
    connect(ui->btn_zoom_fit, SIGNAL(clicked()), this, SLOT(OnBtnZoomFitClicked()));

    /*
      Set default values for member variables
     */
    npoints = 0;
    axes_markersize_horizontal = 5;
    axes_markersize_vertical = 5;
    markersize = 5;

    markercolor = Qt::blue;
    axis_color_horizontal = Qt::red;
    axis_color_vertical = Qt::green;

    QString stylesheet;
    stylesheet = "QPushButton#btn_marker_color_dlg { color: " + markercolor.name() + "}";
    ui->btn_marker_color_dlg->setStyleSheet(stylesheet);
    stylesheet = "QPushButton#btn_horizontal_color_dlg { color: " + axis_color_horizontal.name() + "}";
    ui->btn_horizontal_color_dlg->setStyleSheet(stylesheet);
    stylesheet = "QPushButton#btn_vertical_color_dlg { color: " + axis_color_vertical.name() + "}";
    ui->btn_vertical_color_dlg->setStyleSheet(stylesheet);

    select_horizontal_start = false;
    select_horizontal_end = false;
    select_vertical_start = false;
    select_vertical_end = false;

    datalist.DeleteAllNodes();
    markerlist.DeleteAllNodes();
    endpoints.ResetSize(4,3,0.0);

    QString countstring;
    countstring = "Number of Points: 0";
    ui->lbl_npoints->setText(countstring);


    // Disable buttons and text boxes that have no meaning without a loaded image
    ui->btn_undo->setDisabled(true);
    ui->btn_clearall->setDisabled(true);

    ui->btn_save->setDisabled(true);

    ui->btn_horizontal_start->setDisabled(true);
    ui->btn_horizontal_end->setDisabled(true);
    ui->btn_horizontal_color_dlg->setDisabled(true);
    ui->txt_horizontal_marker_size->setDisabled(true);
    ui->txt_horizontal_start->setDisabled(true);
    ui->txt_horizontal_end->setDisabled(true);

    ui->btn_vertical_start->setDisabled(true);
    ui->btn_vertical_end->setDisabled(true);
    ui->btn_vertical_color_dlg->setDisabled(true);
    ui->txt_vertical_marker_size->setDisabled(true);
    ui->txt_vertical_start->setDisabled(true);
    ui->txt_vertical_end->setDisabled(true);

    ui->btn_marker_color_dlg->setDisabled(true);
    ui->txt_marker_size->setDisabled(true);

    ui->btn_zoom_fit->setDisabled(true);
    ui->btn_zoom_in->setDisabled(true);
    ui->btn_zoom_out->setDisabled(true);
    ui->lbl_zoom_level->setDisabled(true);
    ui->lbl_zoom_level->setText("Zoom Level: --");

    zoomlevel = 1.0;
}


PlotExtract::~PlotExtract()
{
    delete ui;

    if(pen){
        delete pen;
    }
    if(brush){
        delete brush;
    }
    if(scenerefpt){
        delete scenerefpt;
    }
    if(myimg->AddedToScene()){
        delete scene;
        scene = NULL;
        myimg = NULL;
    } else {
        delete scene;
        delete myimg;
        scene = NULL;
        myimg = NULL;
    }
}


void PlotExtract::GetInputFile()
{
    // Get input filename and add image to scene
    QString dlgtitle("Open Image File");
    QString dir("./");
    QString filter("Images (*.png *.xpm *.jpg)");
    inputfile = QFileDialog::getOpenFileName(this,dlgtitle,dir,filter);

    if(inputfile != ""){

        if(myimg->AddedToScene()){
            delete scene;
            scene = NULL;
            myimg = NULL;
        } else {
            delete scene;
            delete myimg;
            scene = NULL;
            myimg = NULL;
        }

        scene = new QGraphicsScene;
        myimg = new ClickablePixmap;
        connect( myimg, SIGNAL(MouseClicked(qint64,qint64)), this, SLOT(OnClickInImage(qint64,qint64)));

        /* Clear all nodes. */
        npoints = 0;
        OnClearAllClicked();

        myimg->OpenImage(inputfile);
        myimg->AddToScene(scene);

        ui->img_view->setScene(scene);
        ui->img_view->show();
        ui->img_view->centerOn(0,0);

        OnBtnZoomFitClicked();

        // Create axis-definition ellipses of zero size.  These are 'dummy' ellipses so that
        // the 'removeItem' function calls later do not error-out.
        horizontal_start_ellipse = scene->addEllipse(0,0,0,0,*pen,Qt::SolidPattern);
        horizontal_end_ellipse = scene->addEllipse(0,0,0,0,*pen,Qt::SolidPattern);
        vertical_start_ellipse = scene->addEllipse(0,0,0,0,*pen,Qt::SolidPattern);
        vertical_end_ellipse = scene->addEllipse(0,0,0,0,*pen,Qt::SolidPattern);

        // Enable buttons and text-boxes that require a loaded image to be relevant
        ui->btn_horizontal_start->setDisabled(false);
        ui->btn_horizontal_end->setDisabled(false);
        ui->btn_horizontal_color_dlg->setDisabled(false);
        ui->txt_horizontal_marker_size->setDisabled(false);
        ui->txt_horizontal_start->setDisabled(false);
        ui->txt_horizontal_end->setDisabled(false);

        ui->btn_vertical_start->setDisabled(false);
        ui->btn_vertical_end->setDisabled(false);
        ui->btn_vertical_color_dlg->setDisabled(false);
        ui->txt_vertical_marker_size->setDisabled(false);
        ui->txt_vertical_start->setDisabled(false);
        ui->txt_vertical_end->setDisabled(false);

        ui->btn_marker_color_dlg->setDisabled(false);
        ui->txt_marker_size->setDisabled(false);

        ui->btn_zoom_fit->setDisabled(false);
        ui->btn_zoom_in->setDisabled(false);
        ui->btn_zoom_out->setDisabled(false);
        ui->lbl_zoom_level->setDisabled(false);

        zoomlevel = 1.0e0;
    }
}


void PlotExtract::OnClickInImage(qint64 x, qint64 y)
{
    // Map (x,y) global coordinates to scene and define ellipse rectangles
    QPointF scenept = ui->img_view->mapToScene(ui->img_view->mapFromGlobal(QPoint(x,y)));

    qreal offset = 3.5/zoomlevel;
    QRectF datarect(scenept.x()-offset-markersize/2.0,
                    scenept.y()-offset-markersize/2.0,
                    markersize,markersize);
    QRectF axishrect(scenept.x()-offset-axes_markersize_horizontal/2.0,
                     scenept.y()-offset-axes_markersize_horizontal/2.0,
                     axes_markersize_horizontal,axes_markersize_horizontal);
    QRectF axisvrect(scenept.x()-offset-axes_markersize_vertical/2.0,
                     scenept.y()-offset-axes_markersize_vertical/2.0,
                     axes_markersize_vertical,axes_markersize_vertical);

    /*
      Check if one of the axes-defining points has been selected and locate points
      accordingly.
     */
    if(select_horizontal_start == false && select_horizontal_end == false &&
       select_vertical_start == false && select_vertical_end == false){
        pen->setColor(markercolor);
        pen->setWidth(0);
        brush->setColor(markercolor);
        brush->setStyle(Qt::SolidPattern);

        npoints += 1;
        if(npoints == 1){
            // Execution of this section only occurs when this is the only node in
            // the list.  To ensure the list only consists of this node, all
            // nodes are deleted from this list, which results in a single blank
            // node being present in the list.
            markerlist.DeleteAllNodes();
            datalist.DeleteAllNodes();
        } else {
            // Add a node and move the list cursor to the newly-created node so that
            // a value can be assigned to its data array.
            markerlist.AddNode();
            markerlist.Advance();
            datalist.AddNode();
            datalist.Advance();
        }

        QGraphicsEllipseItem *ellipse = scene->addEllipse(datarect,*pen,*brush);
        markerlist.SetDataValue(ellipse,0);
        datalist.SetDataValue(scenept.x(),0);
        datalist.SetDataValue(scenept.y(),1);

        QString countstring;
        countstring = "Number of Points: ";
        countstring.append(QString("%1").arg(markerlist.GetNumNodes()));
        ui->lbl_npoints->setText(countstring);

        ui->btn_undo->setDisabled(false);
        ui->btn_clearall->setDisabled(false);
        ui->btn_save->setDisabled(false);
    }

    if(select_horizontal_start == true){
        pen->setColor(axis_color_horizontal);
        pen->setWidth(0);
        brush->setColor(axis_color_horizontal);
        brush->setStyle(Qt::SolidPattern);

        horizontal_start_ellipse = scene->addEllipse(axishrect,*pen,*brush);

        ui->btn_horizontal_start->setDisabled(false);
        select_horizontal_start = false;
    }

    if(select_horizontal_end == true){
        pen->setColor(axis_color_horizontal);
        pen->setWidth(0);
        brush->setColor(axis_color_horizontal);
        brush->setStyle(Qt::SolidPattern);

        horizontal_end_ellipse = scene->addEllipse(axishrect,*pen,*brush);

        ui->btn_horizontal_end->setDisabled(false);
        select_horizontal_end = false;
    }

    if(select_vertical_start == true){
        pen->setColor(axis_color_vertical);
        pen->setWidth(0);
        brush->setColor(axis_color_vertical);
        brush->setStyle(Qt::SolidPattern);

        vertical_start_ellipse = scene->addEllipse(axisvrect,*pen,*brush);

        ui->btn_vertical_start->setDisabled(false);
        select_vertical_start = false;
    }

    if(select_vertical_end == true){
        pen->setColor(axis_color_vertical);
        pen->setWidth(0);
        brush->setColor(axis_color_vertical);
        brush->setStyle(Qt::SolidPattern);

        vertical_end_ellipse = scene->addEllipse(axisvrect,*pen,*brush);

        ui->btn_vertical_end->setDisabled(false);
        select_vertical_end = false;
    }
}


void PlotExtract::OnHorizontalStartClicked()
{
    scene->removeItem(horizontal_start_ellipse);
    select_horizontal_start = true;
    ui->btn_horizontal_start->setDisabled(true);
}


void PlotExtract::OnHorizontalEndClicked()
{
    scene->removeItem(horizontal_end_ellipse);
    select_horizontal_end = true;
    ui->btn_horizontal_end->setDisabled(true);
}


void PlotExtract::OnVerticalStartClicked()
{
    scene->removeItem(vertical_start_ellipse);
    select_vertical_start = true;
    ui->btn_vertical_start->setDisabled(true);
}


void PlotExtract::OnVerticalEndClicked()
{
    scene->removeItem(vertical_end_ellipse);
    select_vertical_end = true;
    ui->btn_vertical_end->setDisabled(true);
}


void PlotExtract::OnUndoLastClicked()
{
    if(npoints > 0){
        markerlist.GoToFinalNode();
        QGraphicsEllipseItem *ellipse = markerlist.GetDataValue(0);
        scene->removeItem(ellipse);

        if(npoints > 1){
            // Delete last node in list
            markerlist.GoToFinalNode();
            markerlist.DeleteNode();

            datalist.GoToFinalNode();
            datalist.DeleteNode();
        } else {
            // Use 'DeleteAllNodes()' so remaining node in list is deleted and
            // a new, empty node is created.  This prevents errors when all
            // clicked points are undone and the user then goes to select another
            // point.
            markerlist.DeleteAllNodes();
            datalist.DeleteAllNodes();
        }

        npoints -= 1;

        QString countstring;
        if(npoints > 0){
            countstring = "Number of Points: ";
            countstring.append(QString("%1").arg(markerlist.GetNumNodes()));
        } else {
            // If no points are selected, the number of nodes in 'markerlist' won't
            // be 0 due to the use of 'DeleteAllNodes' above.  This is handled here.
            countstring = "Number of Points: 0";
        }
        ui->lbl_npoints->setText(countstring);
    }

    if(npoints == 0){
        ui->btn_undo->setDisabled(true);
        ui->btn_clearall->setDisabled(true);
        ui->btn_save->setDisabled(true);
    }
}


void PlotExtract::OnClearAllClicked()
{
    markerlist.GoToFinalNode();
    for(int i=0; i<npoints; i++){
        QGraphicsEllipseItem *ellipse = markerlist.GetDataValue(0);
        scene->removeItem(ellipse);
        markerlist.Rewind();
    }
    markerlist.DeleteAllNodes();

    npoints = 0;

    QString countstring;
    countstring = "Number of Points: 0";
    ui->lbl_npoints->setText(countstring);
    ui->btn_undo->setDisabled(true);
    ui->btn_clearall->setDisabled(true);
    ui->btn_save->setDisabled(true);
}


void PlotExtract::OnSaveClicked()
{
    /*
      Loop through markers and get pixel coordinates for each.  Also, get pixel
      coordinates from axes identification markers.
     */
    markerlist.GoToNode(1);
    datalist.DeleteAllNodes();

    QGraphicsEllipseItem *tmpellipse = NULL;    // Temporary variable for each ellipse
    QRectF tmprect;                             // Bounding rectangle for temp ellipse
    qreal xcenter = 0.0e0;                      // Center of bounding rectangle
    qreal ycenter = 0.0e0;                      // Center of bounding rectangle
    QString tmpstr;                             // Temporary variable for strings

    for(int i=0; i<npoints; i++){
        // Get pointer to ellipse
        tmpellipse = markerlist.GetDataValue(0);

        // Get bounding rectangle, and thus center location of ellipse
        tmprect = tmpellipse->rect();
        xcenter = tmprect.x() + tmprect.width()/2.0e0;
        ycenter = tmprect.y() + tmprect.height()/2.0e0;

        // Add center coordinates to 'datalist'
        if(i == 0){
            datalist.SetDataValue(xcenter,0);
            datalist.SetDataValue(ycenter,1);
        } else {
            datalist.AddNode();
            datalist.Advance();
            datalist.SetDataValue(xcenter,0);
            datalist.SetDataValue(ycenter,1);
        }

        // Advance to next node in 'markerlist'
        markerlist.Advance();
    }

    // Starting point of horizontal axis
    tmpstr = ui->txt_horizontal_start->text();
    tmprect = horizontal_start_ellipse->rect();
    xcenter = tmprect.x() + tmprect.width()/2.0e0;
    ycenter = tmprect.y() + tmprect.height()/2.0e0;
    endpoints(0,0) = xcenter;
    endpoints(0,1) = ycenter;
    endpoints(0,2) = (qreal)tmpstr.toDouble();

    // Ending point of horizontal axis
    tmpstr = ui->txt_horizontal_end->text();
    tmprect = horizontal_end_ellipse->rect();
    xcenter = tmprect.x() + tmprect.width()/2.0e0;
    ycenter = tmprect.y() + tmprect.height()/2.0e0;
    endpoints(1,0) = xcenter;
    endpoints(1,1) = ycenter;
    endpoints(1,2) = (qreal)tmpstr.toDouble();

    // Starting point of vertical axis
    tmpstr = ui->txt_vertical_start->text();
    tmprect = vertical_start_ellipse->rect();
    xcenter = tmprect.x() + tmprect.width()/2.0e0;
    ycenter = tmprect.y() + tmprect.height()/2.0e0;
    endpoints(2,0) = xcenter;
    endpoints(2,1) = ycenter;
    endpoints(2,2) = (qreal)tmpstr.toDouble();

    // Ending point of vertical axis
    tmpstr = ui->txt_vertical_end->text();
    tmprect = vertical_end_ellipse->rect();
    xcenter = tmprect.x() + tmprect.width()/2.0e0;
    ycenter = tmprect.y() + tmprect.height()/2.0e0;
    endpoints(3,0) = xcenter;
    endpoints(3,1) = ycenter;
    endpoints(3,2) = (qreal)tmpstr.toDouble();



    /*
      Loop through list and convert from pixel coordinates to real coordinates
      based on axes identification
     */

    // Define constant mapping parameters
    qreal dhx = endpoints(1,0) - endpoints(0,0);    // x-direction pixel length of horizontal axis
    qreal dhy = endpoints(1,1) - endpoints(0,1);    // y-direction pixel length of horizontal axis
    qreal dvx = endpoints(3,0) - endpoints(2,0);    // x-direction pixel length of vertical axis
    qreal dvy = endpoints(3,1) - endpoints(2,1);    // y-direction pixel length of vertical axis
    qreal theta = atan2(dhy,dhx);                   // Rotation of horizontal axis from horizontal
    qreal m = 1.0e0/(tan(atan2(dvy,dvx) - theta));  // Shear scaling factor of vertical axis
    qreal lh = sqrt(dhx*dhx + dhy*dhy);             // Length of horizontal reference axis [pixels]
    qreal lv = sqrt(dvx*dvx + dvy*dvy);             // Length of vertical reference axis [pixels]
    qreal dx = endpoints(1,2) - endpoints(0,2);     // Length of horizontal reference axis [real]
    qreal dy = endpoints(3,2) - endpoints(2,2);     // Length of vertical reference axis [real]
    qreal ppx = dx/lh;                              // Pixel-pitch, x-direction [real/pixels]
    qreal ppy = dy/lv;                              // Pixel-pitch, y-direction [real/pixels]
    qreal zx = endpoints(0,0) - endpoints(0,2)/ppx; // Zero-point, x-direction [pixels]
    qreal zy = endpoints(2,1) + endpoints(2,2)/ppy; // Zero-point, y-direction [pixels]
    qreal det = cos(theta)*cos(theta) - sin(theta)*sin(theta);  // Determinate of mapping matrix

    DLList<qreal,2> transformedlist;    // Transformed point coordinates, unsorted

    datalist.GoToNode(1);   // Start at first point selected
    for(int i=0; i<datalist.GetNumNodes(); i++){
        if(i != 0){
            datalist.Advance();             // Advance to next node
            transformedlist.AddNodeAfter(); // Add node
        }

        // Pixel-coordinates of clicked points
        qreal xp = datalist.GetDataValue(0);
        qreal yp = datalist.GetDataValue(1);

        // Determine true pixel coordinates, correcting for rotation and shear
        qreal x = (cos(theta)*xp - (sin(theta) - m*cos(theta))*yp)/det;
        qreal y = (-sin(theta)*xp + (cos(theta) + m*sin(theta))*yp)/det;

        // Subtract out the zero-point
        x -= zx;
        y -= zy;

        // Convert from pixels to real-world values
        x *= ppx;
        y *= -ppy;  // Negative since vertical pixels are numbered top-down and the
                    // vertical axis is assumed to be positive-up.

        // Save clicked-pixel values in list with real-world values.  'datalist' is
        // not overwritten to allow further use without restarting the program.
        transformedlist.SetDataValue(x,0);
        transformedlist.SetDataValue(y,1);
    }




    /*
      Sort data points in list based on axis identified by user
     */
    DLList<qreal,2> savelist;

    // Define first node in the saved-data list
    transformedlist.GoToNode(1);
    savelist.GoToNode(1);
    savelist.SetDataValue(transformedlist.GetDataValue(0),0);
    savelist.SetDataValue(transformedlist.GetDataValue(1),1);

    /*
      Loop through remaining 'transformedlist' nodes and re-order as necessary when placing
      into 'savelist'.  'savelist' is sorted in ascending order, based on the user-selected
      sort axis.  Output in ascending/descending order is handled by the file-writing
      routine.
     */
    for(int i=1; i<npoints; i++){
        transformedlist.Advance();
        qreal xval = transformedlist.GetDataValue(0);
        qreal yval = transformedlist.GetDataValue(1);
        qreal savecomparevalue;
        qreal datacomparevalue;
        int compareidx;


        savelist.GoToNode(1);
        if(ui->rdo_sort_horizontal_axis->isChecked()){
            compareidx = 0;
            savecomparevalue = savelist.GetDataValue(0);
            datacomparevalue = xval;
        } else {
            compareidx = 1;
            savecomparevalue = savelist.GetDataValue(1);
            datacomparevalue = yval;
        }

        int listlength = savelist.GetNumNodes();
        int count = 1;
        while(datacomparevalue >= savecomparevalue && count < listlength){
            savelist.Advance();
            savecomparevalue = savelist.GetDataValue(compareidx);
            count += 1;
        }

        if(datacomparevalue < savecomparevalue){
            if(savelist.IsHeadNode() == true){
                savelist.AddNodeBefore();
            } else {
                savelist.Rewind();
                savelist.AddNodeAfter();
            }
            savelist.SetDataValue(xval,0);
            savelist.SetDataValue(yval,1);
        } else {
            savelist.AddNodeAfter();
            savelist.SetDataValue(xval,0);
            savelist.SetDataValue(yval,1);
        }

    }



    /*
      Write sorted list to CSV file.  It is expected that 'savelist' has been sorted
      ascendingly by the correct axis.  The user-specified sort direction controls
      how the output loop cycles through the list.
     */
    QFileDialog dlg;
    QString dlgtitle("Save CSV File");
    QString dir("./");
    QString filter("Comma-Separated Values (*.csv)");
    QString savefilename = dlg.getSaveFileName(this, dlgtitle,dir,filter);

    /* Ensure that 'csv' file extension is used.  If not, append one. */
    std::string ext;
    ext = StringManip::DetermFileExt(savefilename.toStdString());
    if(ext != "csv" && ext != "CSV"){
        savefilename += ".csv";
    }

    QFile file(savefilename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "x, y \n";

    if(ui->rdo_sort_ascending->isChecked()){
        // Output in ascending order
        savelist.GoToNode(1);
        for(int i=0; i<savelist.GetNumNodes(); i++){
            out << savelist.GetDataValue(0) << ", " << savelist.GetDataValue(1) << "\n";
            savelist.Advance();
        }
    } else {
        // Output in descending order
        savelist.GoToFinalNode();
        for(int i=0; i<savelist.GetNumNodes(); i++){
            out << savelist.GetDataValue(0) << ", " << savelist.GetDataValue(1) << "\n";
            savelist.Rewind();
        }
    }

    // optional, as QFile destructor will already do it:
    file.close();

}


void PlotExtract::OnHorizontalAxisDefinitionMarkersChanged()
{
    pen->setColor(axis_color_horizontal);
    pen->setWidth(0);
    brush->setColor(axis_color_horizontal);
    brush->setStyle(Qt::SolidPattern);

    QString tmpstr;
    tmpstr = ui->txt_horizontal_marker_size->text();
    axes_markersize_horizontal = (qreal)tmpstr.toDouble();

    QRectF tmprect;
    qreal xcenter;
    qreal ycenter;
    tmprect = horizontal_start_ellipse->rect();
    if(tmprect.width() > 0.1){
        xcenter = tmprect.x() + tmprect.width()/2.0e0;
        ycenter = tmprect.y() + tmprect.height()/2.0e0;
        scene->removeItem(horizontal_start_ellipse);
        horizontal_start_ellipse = scene->addEllipse(xcenter-axes_markersize_horizontal/2.0,
                                                     ycenter-axes_markersize_horizontal/2.0,
                                                     axes_markersize_horizontal,
                                                     axes_markersize_horizontal,*pen,*brush);
    }

    tmprect = horizontal_end_ellipse->rect();
    if(tmprect.width() > 0.1){
        xcenter = tmprect.x() + tmprect.width()/2.0e0;
        ycenter = tmprect.y() + tmprect.height()/2.0e0;
        scene->removeItem(horizontal_end_ellipse);
        horizontal_end_ellipse = scene->addEllipse(xcenter-axes_markersize_horizontal/2.0,
                                                   ycenter-axes_markersize_horizontal/2.0,
                                                   axes_markersize_horizontal,
                                                   axes_markersize_horizontal,*pen,*brush);
    }
}


void PlotExtract::OnVerticalAxisDefinitionMarkersChanged()
{
    pen->setColor(axis_color_vertical);
    pen->setWidth(0);
    brush->setColor(axis_color_vertical);
    brush->setStyle(Qt::SolidPattern);

    QString tmpstr;
    tmpstr = ui->txt_vertical_marker_size->text();
    axes_markersize_vertical = (qreal)tmpstr.toDouble();

    QRectF tmprect;
    qreal xcenter;
    qreal ycenter;
    tmprect = vertical_start_ellipse->rect();
    if(tmprect.width() > 0.1){
        xcenter = tmprect.x() + tmprect.width()/2.0e0;
        ycenter = tmprect.y() + tmprect.height()/2.0e0;
        scene->removeItem(vertical_start_ellipse);
        vertical_start_ellipse = scene->addEllipse(xcenter-axes_markersize_vertical/2.0,
                                                   ycenter-axes_markersize_vertical/2.0,
                                                   axes_markersize_vertical,
                                                   axes_markersize_vertical,*pen,*brush);
    }

    tmprect = vertical_end_ellipse->rect();
    if(tmprect.width() > 0.1){
        xcenter = tmprect.x() + tmprect.width()/2.0e0;
        ycenter = tmprect.y() + tmprect.height()/2.0e0;
        scene->removeItem(vertical_end_ellipse);
        vertical_end_ellipse = scene->addEllipse(xcenter-axes_markersize_vertical/2.0,
                                                 ycenter-axes_markersize_vertical/2.0,
                                                 axes_markersize_vertical,
                                                 axes_markersize_vertical,*pen,*brush);
    }
}


void PlotExtract::OnDataMarkersChanged()
{
    pen->setColor(markercolor);
    pen->setWidth(0);
    brush->setColor(markercolor);
    brush->setStyle(Qt::SolidPattern);

    QString tmpstr;
    tmpstr = ui->txt_marker_size->text();
    markersize = (qreal)tmpstr.toDouble();

    QGraphicsEllipseItem *tmpellipse;
    QRectF tmprect;
    qreal xcenter;
    qreal ycenter;
    markerlist.GoToNode(1);
    for(int i=0; i<npoints; i++){
        tmpellipse = markerlist.GetDataValue(0);
        tmprect = tmpellipse->rect();
        xcenter = tmprect.x() + tmprect.width()/2.0e0;
        ycenter = tmprect.y() + tmprect.height()/2.0e0;

        scene->removeItem(tmpellipse);
        tmpellipse = scene->addEllipse(xcenter-markersize/2.0,
                                       ycenter-markersize/2.0,
                                       markersize,
                                       markersize,*pen,*brush);

        markerlist.SetDataValue(tmpellipse,0);

        markerlist.Advance();
    }
}


void PlotExtract::OnHorizontalAxisColorClicked()
{
    QString titlestr("Horizontal Axis-Definition Color Selection");
    axis_color_horizontal = QColorDialog::getColor(axis_color_horizontal, this,
                                                   titlestr, QColorDialog::ShowAlphaChannel);

    if(axis_color_horizontal.isValid() == false){
        axis_color_horizontal = Qt::red;
    }

    QString stylesheet;
    stylesheet = "QPushButton#btn_horizontal_color_dlg { color: " + axis_color_horizontal.name() + "}";
    ui->btn_horizontal_color_dlg->setStyleSheet(stylesheet);

    OnHorizontalAxisDefinitionMarkersChanged();
}


void PlotExtract::OnVerticalAxisColorClicked()
{
    QString titlestr("Vertical Axis-Definition Color Selection");
    axis_color_vertical = QColorDialog::getColor(axis_color_vertical, this,
                                                 titlestr, QColorDialog::ShowAlphaChannel);

    if(axis_color_vertical.isValid() == false){
        axis_color_vertical = Qt::green;
    }

    QString stylesheet;
    stylesheet = "QPushButton#btn_vertical_color_dlg { color: " + axis_color_vertical.name() + "}";
    ui->btn_vertical_color_dlg->setStyleSheet(stylesheet);

    OnVerticalAxisDefinitionMarkersChanged();
}


void PlotExtract::OnDataColorClicked()
{
    QString titlestr("Data Marker Color Selection");
    markercolor = QColorDialog::getColor(markercolor, this, titlestr,
                                         QColorDialog::ShowAlphaChannel);

    if(markercolor.isValid() == false){
        markercolor = Qt::blue;
    }

    QString stylesheet;
    stylesheet = "QPushButton#btn_marker_color_dlg { color: " + markercolor.name() + "}";
    ui->btn_marker_color_dlg->setStyleSheet(stylesheet);

    OnDataMarkersChanged();
}


void PlotExtract::OnMenuInstructionsClicked() const
{
    QMessageBox msgBox;
    QString message;

    message  = "INSTRUCTIONS: \n";
    message += "\n";
    message += "1.) Click 'Open Plot' to open image of plot. \n";
    message += "    \n";
    message += "2.) Identify each axis by first clicking the appropriate \n";
    message += "    start/end button and then clicking the corresponding \n";
    message += "    point in the plot.  The value associated with that point \n";
    message += "    can be entered in the text box at any time. \n";
    message += "    \n";
    message += "    The axis-definition points can be re-selected by clicking \n";
    message += "    the start/end button and then re-selecting the point in the \n";
    message += "    plot.  Clicking the start/end button will remove the previously \n";
    message += "    selected point. \n";
    message += "    \n";
    message += "3.) Select data points to be extracted by clicking within \n";
    message += "    the plot.";
    message += "    \n";
    message += "    The most recently selected data point can be deleted by \n";
    message += "    clicking 'Undo Last'.  Undo can be used for every point that \n";
    message += "    has been selected, but in each case will only remove the \n";
    message += "    youngest point in the list.  To remove the first selected \n";
    message += "    point, 'Undo Last' must be clicked for all points";
    message += "    \n";
    message += "    All selected data points can be deleted at once using 'Clear \n";
    message += "    All'.  Axis-definition points are left unchanged. \n";
    message += "    \n";
    message += "4.) Select the sort options for output.  The axis used for sorting \n";
    message += "    as well as the direction of the sort can be selected independently. \n";
    message += "    \n";
    message += "5.) Once axes are defined and data points selected, a CSV file can \n";
    message += "    be generated by clicking 'Save'.  You will be prompted to enter a \n";
    message += "    name for the output file.  The file you select will then contain \n";
    message += "    a comma-separated list of data point pairs.  These pairs are labeled \n";
    message += "    'x' for the horizontal axis and 'y' for the vertical axis.  The \n";
    message += "    units of these points correspond to the units used in the original \n";
    message += "    plot, provided values from the original plot were used to define \n";
    message += "    the axes. \n";

    msgBox.setText(message);
    msgBox.setWindowTitle("Instructions");
    msgBox.exec();
}


void PlotExtract::OnBtnZoomInClicked()
{
    ui->img_view->scale(1.25,1.25);
    zoomlevel *= 1.25;

    npoints -= 1;

    QString zoomstring;
    zoomstring = "Zoom Level: " + QString::number(zoomlevel) + "x";
    ui->lbl_zoom_level->setText(zoomstring);
}


void PlotExtract::OnBtnZoomOutClicked()
{
    ui->img_view->scale(0.75,0.75);
    zoomlevel *= 0.75;

    QString zoomstring;
    zoomstring = "Zoom Level: " + QString::number(zoomlevel) + "x";
    ui->lbl_zoom_level->setText(zoomstring);
}


void PlotExtract::OnBtnZoomFitClicked()
{
    ui->img_view->scale(1.0/zoomlevel,1.0/zoomlevel);
    zoomlevel = 1.0;

    QString zoomstring;
    zoomstring = "Zoom Level: " + QString::number(zoomlevel) + "x";
    ui->lbl_zoom_level->setText(zoomstring);
}


void PlotExtract::OnMenuActionHelpCreditsActivated()
{
    Credits dlg;
    dlg.exec();
}
