#ifndef INTERFACE_H
#define INTERFACE_H
#include <QSortFilterProxyModel>
#include "employe.h"
#include <QDialog>
#include "secformdialog.h"
#include "arduino.h"
namespace Ui {
class interface;
}

class interface : public QDialog
{
    Q_OBJECT

public:
    explicit interface(QWidget *parent = nullptr);
    QSortFilterProxyModel *proxy;
    ~interface();

private slots:
    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_pb_chercher_clicked();

    void on_lineEdit_rechercher_textChanged(const QString &arg1);

    void on_pb_tri_clicked();

    void on_pb_generatepdf_2_clicked();

    void on_pb_generatepdf_clicked();

    void on_pb_statistique_clicked();

    void on_pb_statistique2_clicked();

    void on_pb_notifier_clicked();

    void on_pb_notifier_2_clicked();

    void on_calendrier_clicked();

    void on_pbarduinoT_ON_clicked();

    void on_pbarduinoT_off_clicked();
    void update_label();

private:
    Ui::interface *ui;
    employe E ;
    SECFORMDialog *secDialog; QByteArray data ;
    Arduino A;
};

#endif // INTERFACE_H
