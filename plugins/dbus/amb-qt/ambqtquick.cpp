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
#include "ambqtquick.h"
#include <QtQml/qqml.h>

void Components::registerTypes(const char *uri)
{
 Q_ASSERT(uri == QLatin1String("amb"));
 int ret = qmlRegisterType<AmbProperty>(uri, 0, 1,"AutomotivePropertyItem");
}

void Components::initializeEngine(QQmlEngine *, const char *)
{

}

