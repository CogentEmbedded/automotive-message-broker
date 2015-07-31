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
#include "timestamp.h"

#include <QCoreApplication>

#include <debugout.h>
#include <iostream>

int main(int argc, char** argv)
{
	QCoreApplication app(argc,argv);

	AmbProperty speed;

	double totalLatency=0;
	int numSamples=0;

	QObject::connect(&speed, &AmbProperty::valueChanged,[&](QVariant val)
	{
		double t1 = speed.time();
		double t2 = amb::currentTime();

		double latency = (t2-t1)*1000.0;

		DebugOut(0)<<"latency: "<<latency<<std::endl;
		totalLatency+=latency;
		numSamples++;
		DebugOut(0)<<"Average: "<<totalLatency / numSamples<<std::endl;
		qDebug()<<"Value: "<<val;
	});

	speed.setObjectName("VehicleSpeed");
	speed.setPropertyName("Speed");
	speed.connect();

	qDebug() << "Updated="<< speed.updateValue();

	return app.exec();
}
