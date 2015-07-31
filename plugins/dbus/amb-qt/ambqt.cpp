/*
Copyright (C) 2012 Intel Corporation
Copyright (C) 2015 Cogent Embedded Inc.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "ambqt.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QtDebug>
#include <QDBusReply>

AmbProperty::AmbProperty(QString objectName, QString propName)
    :QObject(), mObjectName(objectName), mPropertyName(propName), mZone(0), managerIface(NULL)
{
    // Automatically connect when all properties have been set
    connect();
}


void AmbProperty::propertyChangedSlot(QString, QVariantMap values, QStringList )
{
    // try to get value
    mValue = values[mPropertyName];

    // try to parse time
    QVariant timeVariant = values["Time"];
    bool ok = false;
    double NewTime = 0;
    if (timeVariant.isValid())
        NewTime = timeVariant.toDouble(&ok);
    if (ok)
        mTime = NewTime;

    // emit the signal
    valueChanged(mValue);
}

void AmbProperty::connect()
{
    if (managerIface)
        disconnect();

    managerIface = new QDBusInterface("org.automotive.message.broker","/","org.automotive.Manager", QDBusConnection::systemBus(), this);

    if(!managerIface->isValid())
    {
        qWarning()<<"Failed to create manager interface";
        delete managerIface;
        return;
    }

    QDBusReply<QList<QDBusObjectPath>> reply = managerIface->call("FindObjectsForZone", mObjectName, mZone);

    if(reply.isValid())
    {
        for (int i=0; i < reply.value().size(); ++i)
        {
            QDBusInterface* mDBusInterface = new QDBusInterface("org.automotive.message.broker",
                                                                reply.value().at(i).path(),
                                                                "org.freedesktop.DBus.Properties",
                                                                QDBusConnection::systemBus(),
                                                                this);

            if(!mDBusInterface->isValid())
            {
                qDebug()<<"Failed to create dbus interface for property "<<propertyName();
                qDebug()<<"path: "<< reply.value().at(i).path();
                qDebug()<<"object: "<< objectName();
                qDebug()<<"property: "<< propertyName();
                qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
            }

            // store object interface for future use
            ServerProperties.append(mDBusInterface);

            if(!QDBusConnection::systemBus().connect("org.automotive.message.broker",
                                                     reply.value().at(i).path(),
                                                     "org.freedesktop.DBus.Properties",
                                                     "PropertiesChanged",
                                                     this,
                                                     SLOT(propertyChangedSlot(QString,QVariantMap,QStringList))))
            {
                qDebug()<<"Failed to connect to signal";
                qDebug()<<"path: "<< reply.value().at(i).path();
                qDebug()<<"object: "<< objectName();
                qDebug()<<"property: "<< propertyName();
                qDebug()<<"Error: "<< QDBusConnection::systemBus().lastError().message();
            }
        }
    }

    //    if(mInterfaceName.isEmpty())
    //        mInterfaceName = "org.automotive."+mPropertyName;

    //    mDBusInterface = new QDBusInterface("org.automotive.message.broker",objectPath(), interfaceName(), QDBusConnection::systemBus(),this);

    //    if(!mDBusInterface->isValid())
    //    {
    //        qDebug()<<"Failed to create dbus interface for property "<<propertyName();
    //        qDebug()<<"Path: "<<objectPath();
    //        qDebug()<<"Interface: "<<interfaceName();
    //        qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
    //    }

    //    QString signalName = propertyName() + "Changed";



    //    ///TODO: only use PropertiesChanged...  treat AmbProperty like an object rather than a representation of just a single property in the object

    //    if(!QDBusConnection::systemBus().connect("org.automotive.message.broker", objectPath(), mInterfaceName,
    //                                             signalName, this, SLOT(propertyChanged1(QDBusVariant,double))))
    //    {
    //        qDebug()<<"Failed to connect to signal";
    //        qDebug()<<"path: "<<objectPath();
    //        qDebug()<<"interface: "<<interfaceName();
    //        qDebug()<<"signal: "<<propertyName();
    //        qDebug()<<"Error: "<<QDBusConnection::systemBus().lastError().message();
    //    }
}

void AmbProperty::disconnect()
{
    delete managerIface;
    managerIface = NULL;

    QDBusReply<QList<QDBusObjectPath>> reply = managerIface->call("FindObjectsForZone", mObjectName, mZone);

    if(reply.isValid())
    {
        for (int i=0; i < reply.value().size(); ++i)
        {
            QDBusConnection::systemBus().disconnect("org.automotive.message.broker",
                                                    reply.value().at(i).path(),
                                                    "org.freedesktop.DBus.Properties",
                                                    "PropertiesChanged",
                                                    this,
                                                    SLOT(propertyChangedSlot(QString,QVariantMap,QStringList)));
        }
    }

    for (int i = 0; i < ServerProperties.size(); ++i)
        delete ServerProperties[i];

    ServerProperties.clear();
}

QVariant AmbProperty::updateValue()
{
    for (int i = 0; i < ServerProperties.size(); ++i)
    {
        if (!ServerProperties[i]->isValid())
        {
            qDebug()<<"error Interface is not valid "<<ServerProperties[i]->objectName();
        }

        QVariant v = ServerProperties[i]->property(propertyName().toUtf8());

        if (v.isValid())
            return v;
    }
}

void AmbProperty::setValue(QVariant v)
{
    for (int i = 0; i < ServerProperties.size(); ++i)
    {
        if (!ServerProperties[i]->isValid())
        {
            qDebug()<<"error Interface is not valid "<<ServerProperties[i]->objectName();
        }

        ServerProperties[i]->setProperty(propertyName().toUtf8(), v);
    }
}

