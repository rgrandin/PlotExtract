/**
 * @file credits.cpp
 * @author Robert Grandin
 * @date 27 January 2013
 * @brief Implementation of Credits class.
 */


#include "credits.h"
#include "ui_credits.h"

Credits::Credits(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Credits)
{
    ui->setupUi(this);

    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));
}

Credits::~Credits()
{
    delete ui;
}
