#include "secformdialog.h"
#include "ui_secformdialog.h"
#include <QtQml>
#include <QQuickView>
#include "qtquickcontrolsapplication.h"
#include "sqleventmodel.h"

SECFORMDialog::SECFORMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SECFORMDialog)
{
    ui->setupUi(this);


    //pour importer l'example de calendrier qt
    qmlRegisterType<SqlEventModel>("org.qtproject.examples.calendar", 1, 0, "SqlEventModel");
    ui->quickWidget->setSource(QUrl("qrc:/qml/main.qml"));
    ui->quickWidget->show();

}

SECFORMDialog::~SECFORMDialog()
{
    delete ui;
}
