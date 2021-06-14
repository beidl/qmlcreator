/****************************************************************************
**
** Copyright (C) 2013-2015 Oleg Yadrov
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.LocalStorage 2.12
import ProjectManager 1.1
import "../components"

BlankScreen {
    id: projectsScreen

    StackView.onStatusChanged: {
        if (StackView.status === StackView.Activating)
            listView.model = ProjectManager.projects()
    }

    Component.onCompleted: {
        listView.model = ProjectManager.projects()
    }

    CListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: toolBar.height

        function getPath(data) {
            return data;
        }

        delegate: CFileButton {
            text: ProjectManager.importedProjectExists(modelData) ?
                      externalPicker.getDirNameForBookmark(modelData) : modelData
            isDir: true
            onClicked: {
                ProjectManager.subDir = ""
                if (ProjectManager.importedProjectExists(modelData)) {
                    console.log("Opening imported project");
                    ProjectManager.isImported = true;
                    ProjectManager.projectName = externalPicker.openFileOrPath(modelData)
                } else {
                    ProjectManager.isImported = false;
                    ProjectManager.projectName = modelData
                }
                leftView.push(Qt.resolvedUrl("FilesScreen.qml"))
            }
            onRemoveClicked: {
                var parameters = {
                    title: qsTr("Delete the project"),
                    text: qsTr("Are you sure you want to delete \"%1\"?").arg(modelData)
                }

                var callback = function(value)
                {
                    if (value)
                    {
                        if (ProjectManager.importedProjectExists(modelData))
                            ProjectManager.removeImportedProject(modelData)
                        else
                            ProjectManager.removeProject(modelData)
                        listView.model = ProjectManager.projects()
                    }
                }

                dialog.open(dialog.types.confirmation, parameters, callback)
            }
        }
    }

    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            spacing: 0

            CBackButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Projects")
            }

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf067"
                tooltipText: qsTr("New project")
                onClicked: {
                    newContextMenu.open()
                }
            }
        }
    }

    Menu {
        id: newContextMenu
        x: parent.width - width
        y: toolBar.height
        MenuItem {
            text: qsTr("New project...")
            onTriggered: {
                var parameters = {
                    title: qsTr("New project")
                }

                var callback = function(value)
                {
                    ProjectManager.createProject(value)
                    listView.model = ProjectManager.projects()
                }

                dialog.open(dialog.types.newProject, parameters, callback)
            }
        }

        MenuItem {
            text: qsTr("Import...")
            onTriggered: {
                externalPicker.startImport()
            }
        }
    }

    Connections {
        target: ProjectManager
        function onProjectListChanged() {
            listView.model = ProjectManager.projects();
        }
    }

    FastBlur {
        id: fastBlur
        height: 22 * settings.pixelDensity
        width: parent.width
        radius: 40
        opacity: 0.55

        source: ShaderEffectSource {
            sourceItem: listView
            sourceRect: Qt.rect(0, -toolBar.height, fastBlur.width, fastBlur.height)
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
