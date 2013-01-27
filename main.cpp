/**
 * @file main.cpp
 * @author Robert Grandin
 * @date 17 November 2011
 * @brief Main function for PlotExtract application.
 */



#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>
#include <time.h>

#include <QtGui/QApplication>
#include <QSafeApplication.h>
#include "plotextract.h"

int main(int argc, char *argv[])
{
    SafeApplication a(argc, argv);
    PlotExtract w;
    w.show();

    return a.exec();
}
