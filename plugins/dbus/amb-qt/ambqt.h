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
#ifndef AMBQT_H
#define AMBQT_H

#include <QObject>
#include <QDBusInterface>
#include <QtDebug>

#define AUTOPROPERTY(type, name, Name) \
	public: \
	void set ## Name(type s) { m ## Name = s; } \
	type name() { return m ## Name; } \
	private: \
	type m ## Name;

class QDBusInterface;

class AmbProperty: public QObject
{
	Q_OBJECT
    Q_PROPERTY(QString objectName READ objectName WRITE setObjectName)
    AUTOPROPERTY(QString, objectName, ObjectName)
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName)
	AUTOPROPERTY(QString, propertyName, PropertyName)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(int zone READ zone WRITE setZone)
	AUTOPROPERTY(int, zone, Zone)
	Q_PROPERTY(double time READ time)


public:
    AmbProperty(): mPropertyName("Value"), mZone(0), mTime(0), managerIface(NULL) { }

    AmbProperty(QString objectName, QString propName);

	QVariant value()
	{
		return mValue;
	}

    void setValue(QVariant v);

	double time(){ return mTime; }

signals:
	void valueChanged(QVariant val);

public Q_SLOTS:
	void connect();
    QVariant updateValue();
    void disconnect();

private Q_SLOTS:
    void propertyChangedSlot(QString, QVariantMap values, QStringList);

private:
    QList<QDBusInterface*> ServerProperties;
    QDBusInterface* managerIface;
	double mTime;
	QVariant mValue;
};

#endif // AMBQT_H
