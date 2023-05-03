#include "sqleventmodel.h"
#include "connection.h"
#include "event.h"
#include <QFileInfo>
#include "QSqlQuery"
#include <QObject>
#include "qsqlquery.h"

//The C++ class, SqlEventModel, inherits SqlQueryModel to create a database with dummy events for certain dates.
SqlEventModel::SqlEventModel()
{

}


QList<QObject*> SqlEventModel::eventsForDate(const QDate &date)
{

    QSqlQuery query;


    query.prepare("SELECT DATEE,MATRICULE,ID FROM RESERVATION where DATEE like :date");
    query.bindValue(":date", date);
    query.exec();

    QList<QObject*> events;
    while (query.next()) {
        Event *event = new Event(this);
        //Event *event1 = new Event(this);
        event->setName(query.value("ID,MATRICULE").toString());
        event->setName(query.value("MATRICULE").toString());
        event->setinformation(query.value("MATRICULE").toString());

        QDateTime startDate;
        startDate.setDate(query.value("DATEE").toDate());
        //startDate.setTime(QTime(0, 0).addSecs(query.value("startTime").toInt()));
        QString start="00000";
        startDate.setTime(QTime(0, 0).addSecs(start.toInt()));
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value("DATEE").toDate());
        endDate.setTime(QTime(0, 0).addSecs(start.toInt()));
        event->setEndDate(endDate);

        events.append(event);
          //events.append(event1);
    }

    return events;

}
/*
    Defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.
*/
void SqlEventModel::createConnection()
{
    Connection c;
     bool test=c.createconnect();
    if ((!test)) {
        qFatal("Cannot open database");
        return;
    }

    QSqlQuery query;
    // We store the time as seconds because it's easier to query.
    query.exec("SELECT* from RESERVATION");
    return;
}
