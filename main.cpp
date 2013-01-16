#include <iostream>
using namespace std;
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
#include "plotextract.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlotExtract w;
    w.show();

    return a.exec();
}
