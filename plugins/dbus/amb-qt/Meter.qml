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
import amb 0.1

Row {

    property string name
    onNameChanged: {
        // we use objectName.propertyName format
        var parts = name.split(".");
        prop.objectName = parts[0];
        prop.propertyName = parts.length >= 2 ? parts[1] : "Value";
        prop.connect();
    }

    spacing: 10
    Text {
        text: parent.name
        font.pointSize: 14
        width: 300 // this is a guess
    }
    Text {
        text: qsTr("%1").arg(prop.value)
        font.pointSize: 14
    }

    AutomotivePropertyItem {
        id: prop
    }
}
