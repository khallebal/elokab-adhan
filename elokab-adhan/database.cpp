/***************************************************************************
 *   Copyright (C) 2010-2011 by Amine Roukh     <amineroukh@gmail.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <QtSql>



#include "database.h"
#include <QApplication>
Database::Database()
{
    
}

bool Database::checkDatabase()
{
	QDir appDir(QApplication::applicationDirPath());

#ifdef Q_OS_HAIKU
    appDir.cd(".");
    dbFilePath.setFileName(appDir.absolutePath()+"/data/countries.db");
#else
    appDir.cdUp();

    dbFilePath.setFileName(appDir.absolutePath()+"/share/elokab/elokab-adhan/data/countries.db");
#endif

    if ( dbFilePath.exists() )
        return true;

    return false;
}

bool Database::makeConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(dbFilePath.fileName());
    db.setUserName("");
    db.setPassword("");

    if ( db.isOpen() || !db.open())
        return false;
    
    return true;
}

void Database::closeConnection()
{
    db.removeDatabase(db.connectionName());
}

QStringList Database::getCountriesList(QStringList *countryCodeList)
{
    QStringList countriesList;
    
    QSqlQuery query;
    query.exec("SELECT countryName, countryCode FROM countriesTable");
    
    while(query.next())
    {
        countriesList.append(query.value(0).toString());
        (*countryCodeList).append(query.value(1).toString());
    }
    
    return countriesList;
}

QStringList Database::getCitiesList(int countryNo)
{
    QStringList citiesList;
    
    QSqlQuery query;
    query.prepare("SELECT cityName FROM citiesTable WHERE countryNO=?");
    query.addBindValue(countryNo);
    query.exec();
    
    while(query.next())
    {
        citiesList.append(query.value(0).toString());
    }
    
    return citiesList;
}

QStringList Database::getInfo(QString cityName)
{
    QStringList infoList;
    
    QSqlQuery query;
    query.prepare("SELECT latitude, longitude, timeZone, dayLight FROM citiesTable WHERE cityName=?");
    query.addBindValue(cityName);
    query.exec();
    
    while(query.next())
    {
        infoList.append(query.value(0).toString());
        infoList.append(query.value(1).toString());
        infoList.append(query.value(2).toString());
        infoList.append(query.value(3).toString());
    }
    
    return infoList;
}
