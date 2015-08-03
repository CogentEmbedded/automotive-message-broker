/*
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
import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    visible: true
    width: 500
    height: 400

    Column {
        anchors.fill: parent;

        Meter { name: "VehicleSpeed.Speed"}
        Meter { name: "EngineSpeed.Speed"}
        Meter { name: "SteeringWheel.Angle"}
        Meter { name: "ThrottlePosition.Value"}
        Meter { name: "BrakeOperation.BrakePressure"}
    }
}
