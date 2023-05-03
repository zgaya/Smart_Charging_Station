#include "event.h"

#include <QDebug>
#include <QFileInfo> //QFileInfo provides information about a file's name and position (path) in the file system, its access rights and whether it is a directory or symbolic link,
#include <QSqlError>
#include <QSqlQuery>
Event::Event(QObject *parent) :
    QObject(parent)
{
}

QString Event::name() const
{
    return mName;
}
QString Event::information() const
{
    return minformation;
}
void Event::setName(const QString &name)
{
    if (name != mName) {
        mName = name;
        emit nameChanged(mName); //signal(emit)
    }
}
void Event::setinformation(const QString &information)
{
    if (information != minformation) {
        minformation = information;
        emit informationChanged(minformation); //signal(emit)
    }
}
QDateTime Event::startDate() const
{
    return mStartDate;
}

void Event::setStartDate(const QDateTime &startDate)
{
    if (startDate != mStartDate) {
        mStartDate = startDate;
        emit startDateChanged(mStartDate);
    }
}

QDateTime Event::endDate() const
{
    return mEndDate;
}

void Event::setEndDate(const QDateTime &endDate)
{
    if (endDate != mEndDate) {
        mEndDate = endDate;
        emit endDateChanged(mEndDate);
    }
}
