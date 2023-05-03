#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QSqlQueryModel>

class employe
{
private:
    int CIN;
    QString Nom,Prenom,Phone,Email,Mdp,Function;
public:
    employe();
    employe(int,QString,QString,QString,QString,QString,QString);
        int getCIN();
        QString getNom();
        QString getPrenom();
        QString getEmail();
        QString getPhone();
        QString getFunctio();
        QString getMdp();
        void setCIN(int);
        void setNom(QString);
        void setPrenom(QString);
        void setEmail(QString);
        void setPhone(QString);
        void setFunction(QString);
        void setMdp(QString);
        bool ajouter();
        QSqlQueryModel* afficher();
        bool supprimer(int);
        bool modifier_Employee();

        QSqlQueryModel* rechercher(QString);
        bool testAJ(QString a);
        bool testNP(QString,QString);
        QSqlQueryModel* trier(QString);



        int statistique_E(QString Function);
        int statistique_ET();


        bool notification();
};

#endif // EMPLOYE_H
