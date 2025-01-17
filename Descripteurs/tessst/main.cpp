#include "mainwindow.h"
#include "bibliowindow.h"
#include "connexionwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnexionWindow user;
    user.show();


    return a.exec();
}
