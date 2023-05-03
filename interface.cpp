#include "interface.h"
#include "ui_interface.h"
#include "employe.h"
#include "mainwindow.h"
#include "arduino.h"
#include "QMessageBox"
#include <QTextDocument>
#include <QPrinter>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QTextTableFormat>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDialog>
#include <QTextStream>
#include <QStackedWidget>
#include <qdatastream.h>
#include <QDate>
#include "secformdialog.h"
#include <QDebug>
interface::interface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::interface)
{
    ui->setupUi(this);
    ui->gestion_employe->setCurrentIndex(0);
    ui->lineEdit_CIN->setValidator(new QIntValidator(0, 99999999, this));
    int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
         //le slot update_label suite à la reception du signal readyRead (reception des données).
       // end arduino
}

interface::~interface()
{
    delete ui;
}

void interface::on_pb_ajouter_clicked()
{
    int CIN=ui->lineEdit_CIN->text().toInt();
    QString Nom=ui->lineEdit_Nom->text();
    QString Prenom=ui->lineEdit_Prenom->text();
    QString Phone=ui->lineEdit_Phone->text();
    QString Email=ui->lineEdit_Email->text();
    QString Function=ui->comboBox_Function->currentText();
    QString Mdp=ui->lineEdit_Mdp->text();
    employe E(CIN,Nom,Prenom,Phone,Email,Function,Mdp);
    bool test=E.ajouter();
        if(test){
            QMessageBox::information(nullptr, QObject::tr("Ajouter un employé"),
                        QObject::tr("ajout effectué.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
               ui->tab_employee->setModel(E.afficher());

        }
        else
            QMessageBox::critical(nullptr, QObject::tr("Ajouter un employé"),
                        QObject::tr("ajout non effectué.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);

}

void interface::on_pb_supprimer_clicked()
{
   employe E1;
   E1.setCIN(ui->lineEdit_Supp->text().toInt());
   bool test=E1.supprimer(E1.getCIN());
   if(test){
       QMessageBox::information(nullptr, QObject::tr("Supprimer un employé"),
                   QObject::tr("Suppresion effectué.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);
       ui->lineEdit_Supp->setText("");
       ui->tab_employee->setModel(E1.afficher());

   }
   else
       QMessageBox::critical(nullptr, QObject::tr("not ok"),
                   QObject::tr("Suppresion non effectué.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);

}

void interface::on_pb_modifier_clicked()
{
           int CIN=ui->lineEdit_CIN->text().toInt();
           QString Nom=ui->lineEdit_Nom->text();
           QString Prenom=ui->lineEdit_Prenom->text();
           QString Phone=ui->lineEdit_Phone->text();
           QString Email=ui->lineEdit_Email->text();
           QString Function=ui->comboBox_Function->currentText();
           QString Mdp=ui->lineEdit_Mdp->text();

      employe E(CIN,Nom,Prenom,Phone,Email,Function,Mdp);
      bool test=E.modifier_Employee();
      if(test)
        {
            QMessageBox::information(nullptr, QObject::tr("Modifier un employé"),
                               QObject::tr("employé modifié.\n"
                                           "Click Cancel to exit."), QMessageBox::Cancel);
            ui->tab_employee->setModel(E.afficher());

        }
        else{
            QMessageBox::critical(nullptr, QObject::tr("Modifier un employé"),
                               QObject::tr("Employe non modifié.\n"
                                           "Click Cancel to exit."), QMessageBox::Cancel);
        }
}

void interface::on_pb_chercher_clicked()
{
    QString x = ui->lineEdit_rechercher->text();
    bool test=E.rechercher(x);//makensh test
    if(test)
      {
          QMessageBox::information(nullptr, QObject::tr("Rechercher un employé "),
                             QObject::tr("employé existe.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);

          ui->tab_employee_2->setModel(E.rechercher(x));
}
    else{
        QMessageBox::critical(nullptr, QObject::tr("rechercher un employé"),
                           QObject::tr("Employe non existant.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);

    }
}

void interface::on_lineEdit_rechercher_textChanged(const QString &arg1)
{
    //creation model (masque du tableau) : permet recherche et tri
        proxy = new QSortFilterProxyModel();

     //definir la source (tableau original)
        proxy->setSourceModel(E.afficher());

     //pour la recherche
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive); // S=s (pas de difference entre majiscule et miniscule)
        proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone
       //remplissage tableau avec le masque
        ui->tab_employee_2->setModel(proxy);
    proxy->setFilterFixedString(arg1);
}

void interface::on_pb_tri_clicked()
{
    QString l=ui->comboBox_tri->currentText();
    ui->tab_employee_2->setModel(E.trier(l));
}

void interface::on_pb_generatepdf_2_clicked()
{
    QString strStream;
                   QTextStream out(&strStream);
              const int rowCount = ui->tab_employee_2->model()->rowCount();
     const int columnCount =ui->tab_employee_2->model()->columnCount();


                          out <<  "<html>\n"
                                  "<head>\n"
                                  "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                                  <<  QString("<title>%1</title>\n").arg("employe")
                                  <<  "</head>\n"
                                  "<body bgcolor=#FFFFFF link=#5000A0>\n"
                                      "<h1>Liste des Employés</h1>"

                                      "<table border=1 cellspacing=0 cellpadding=2>\n";

                          // headers
                              out << "<thead><tr bgcolor=#f0f0f0>";
                              for (int column = 0; column < columnCount; column++)
                                  if (!ui->tab_employee_2->isColumnHidden(column))
                                      out << QString("<th>%1</th>").arg(ui->tab_employee_2->model()->headerData(column, Qt::Horizontal).toString());
                              out << "</tr></thead>\n";
                              // data table
                                 for (int row = 0; row < rowCount; row++) {
                                     out << "<tr>";
                                     for (int column = 0; column < columnCount; column++) {
                                         if (!ui->tab_employee_2->isColumnHidden(column)) {
                                             QString data = ui->tab_employee_2->model()->data(ui->tab_employee_2->model()->index(row, column)).toString().simplified();
                                             out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                                         }
                                     }
                                     out << "</tr>\n";
                                 }
                                 out <<  "</table>\n"
                                     "</body>\n"
                                     "</html>\n";



                  QTextDocument *document = new QTextDocument();
                  document->setHtml(strStream);


                  //QTextDocument document;
                  //document.setHtml(html);
                  QPrinter printer(QPrinter::PrinterResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setOutputFileName("Liste Employes.pdf");
                  document->print(&printer);
}

void interface::on_pb_generatepdf_clicked()
{
    QString strStream;
                   QTextStream out(&strStream);
              const int rowCount = ui->tab_employee->model()->rowCount();
     const int columnCount =ui->tab_employee->model()->columnCount();


                          out <<  "<html>\n"
                                  "<head>\n"
                                  "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                                  <<  QString("<title>%1</title>\n").arg("employe")
                                  <<  "</head>\n"
                                  "<body bgcolor=#FFFFFF link=#5000A0>\n"
                                      "<h1>Liste des Employés</h1>"

                                      "<table border=1 cellspacing=0 cellpadding=2>\n";

                          // headers
                              out << "<thead><tr bgcolor=#f0f0f0>";
                              for (int column = 0; column < columnCount; column++)
                                  if (!ui->tab_employee->isColumnHidden(column))
                                      out << QString("<th>%1</th>").arg(ui->tab_employee->model()->headerData(column, Qt::Horizontal).toString());
                              out << "</tr></thead>\n";
                              // data table
                                 for (int row = 0; row < rowCount; row++) {
                                     out << "<tr>";
                                     for (int column = 0; column < columnCount; column++) {
                                         if (!ui->tab_employee->isColumnHidden(column)) {
                                             QString data = ui->tab_employee->model()->data(ui->tab_employee->model()->index(row, column)).toString().simplified();
                                             out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                                         }
                                     }
                                     out << "</tr>\n";
                                 }
                                 out <<  "</table>\n"
                                     "</body>\n"
                                     "</html>\n";



                  QTextDocument *document = new QTextDocument();
                  document->setHtml(strStream);


                  //QTextDocument document;
                  //document.setHtml(html);
                  QPrinter printer(QPrinter::PrinterResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setOutputFileName("Liste Employes1.pdf");
                  document->print(&printer);
}

void interface::on_pb_statistique_clicked()
{
    ui->gestion_employe->setCurrentIndex(2);
    ui->progressBar->setValue(0);
}

void interface::on_pb_statistique2_clicked()
{
    int SA=0;
    int a =E.statistique_E(ui->comboBoxStat->currentText());
     int t=E.statistique_ET();
     SA=(a*100)/t;
     ui->progressBar->setValue(SA);
}

void interface::on_pb_notifier_clicked()
{
     ui->gestion_employe->setCurrentIndex(2);
}

void interface::on_pb_notifier_2_clicked()
{
    //int CIN=ui->lineEdit_CIN_Notif->text().toInt();
    QString Nom=ui->lineEdit_Nom_Notif->text();
    QString Prenom=ui->lineEdit_Prenom_Notif->text();
    QString Function=ui->comboBox_Function_Notif->currentText();


bool testNP=E.testNP(Nom,Prenom);

   if (testNP)
 {

       E.notification();
      QMessageBox::information(nullptr, QObject::tr("Notifier Employeé "),
                      QObject::tr("employé notifié.\n"
"Click Cancel to exit."), QMessageBox::Cancel);
}
else
{ QMessageBox::critical(nullptr, QObject::tr("not ok"),
             QObject::tr("Notification non effectué.\n"
                   "Click Cancel to exit."), QMessageBox::Cancel);
}
}


void interface::on_calendrier_clicked()
{
    secDialog= new SECFORMDialog(this);
        secDialog->show();
}

void interface::on_pbarduinoT_ON_clicked()
{
    QString data;
    A.write_to_arduino("1");

    int CIN=ui->lineEdit_ID->text().toInt();
    QString id_string=QString::number(CIN);
    bool test=E.rechercher(id_string);
    if(test)
    {
    A.write_to_arduino(id_string.toStdString().c_str());
    QString Function=ui->comboBox_Function_2->currentText();
    data =A.read_from_arduino();
    QString DataAsString = QString(data);
     E.notification();
     QMessageBox::warning(this,"Warning","Attention il ya un fuite de GAZ detecte !! ");
    }
}

void interface::on_pbarduinoT_off_clicked()
{
    A.write_to_arduino("0");
    int CIN=ui->lineEdit_ID->text().toInt();
    QString id_string=QString::number(CIN);
    QString Function=ui->comboBox_Function_2->currentText();
    bool test=E.rechercher(id_string);
    if(test)
    {
      A.write_to_arduino(id_string.toStdString().c_str());
    }
}
void interface::update_label()
{
     data =A.read_from_arduino();
     QString DataAsString = QString(data);
         qDebug()<< "this is data:"<< data;
    if (data =="ON")
    {  //ui->label_45->setText("alarme activée");
        ui->label_46->setText("alarm activée");
         QMessageBox::warning(this,"Warning","Attention il ya un fuite de GAZ detecte !! ");
         //E.notificationA();
    }
    else if (data =="OFF")
    {  // ui->label_45->setText("alarme désactivée");
        ui->label_46->setText("alarme désactivée");
    }
}
