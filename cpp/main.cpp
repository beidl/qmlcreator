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

#include <QGuiApplication>
#include <QDateTime>
#include <QDir>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QtGlobal>
#include "MessageHandler.h"
#include "ProjectManager.h"
#include "SyntaxHighlighter.h"
#include "components/linenumbershelper.h"
#include "imfixerinstaller.h"

#ifdef Q_OS_IOS
#include "ios/externalprojectpicker.h"
#endif

inline static void createNecessaryDir(const QString& path) {
    const QDir configDir = QDir(path);
    if (!configDir.exists()) {
        const auto success = configDir.mkpath(configDir.absolutePath());
        if (success)
            return;

        qWarning() << "Failed to create" << configDir.absolutePath();
        return;
    }
}

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>

bool checkAndroidStoragePermissions() {
    const auto permissionsRequest = QStringList(
    { QString("android.permission.READ_EXTERNAL_STORAGE"),
      QString("android.permission.WRITE_EXTERNAL_STORAGE")});

    if (QtAndroid::checkPermission(permissionsRequest[0])
            == QtAndroid::PermissionResult::Denied
            || (QtAndroid::checkPermission(permissionsRequest[1]))
            == QtAndroid::PermissionResult::Denied) {
        auto permissionResults
                = QtAndroid::requestPermissionsSync(permissionsRequest);
        if ((permissionResults[permissionsRequest[0]]
             == QtAndroid::PermissionResult::Denied)
                || (permissionResults[permissionsRequest[1]]
                    == QtAndroid::PermissionResult::Denied))
            return false;
    }
    return true;
}

#endif

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qInstallMessageHandler(&MessageHandler::handler);
    QGuiApplication app(argc, argv);
    app.setApplicationVersion("1.4.0");
#ifdef UBUNTU_CLICK
    app.setOrganizationName(QStringLiteral("me.fredl.qmlcreator"));
#else
    app.setApplicationName("QML Creator");
    app.setOrganizationName("wearyinside");
    app.setOrganizationDomain("com.wearyinside.qmlcreator");
#endif

    const QString configPath =
            QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) +
            QDir::separator();
    const QString cachePath =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
            QDir::separator();

    createNecessaryDir(configPath);
    createNecessaryDir(cachePath);

    QTranslator translator;
    translator.load("qmlcreator_" + QLocale::system().name(), ":/resources/translations");
    app.installTranslator(&translator);

#ifdef Q_OS_IOS
    qmlRegisterType<ExternalProjectPicker>("ExternalProjectPicker", 1, 1, "ExternalProjectPicker");
#endif
    qmlRegisterSingletonType<ProjectManager>("ProjectManager", 1, 1, "ProjectManager", &ProjectManager::projectManagerProvider);
    qmlRegisterType<SyntaxHighlighter>("SyntaxHighlighter", 1, 1, "SyntaxHighlighter");
    qmlRegisterType<LineNumbersHelper>("LineNumbersHelper", 1, 1, "LineNumbersHelper");

#ifdef Q_OS_ANDROID
    while(!checkAndroidStoragePermissions());
#endif

    uint GRID_UNIT_PX = qgetenv("GRID_UNIT_PX").toUInt();
    if (GRID_UNIT_PX == 0) {
        GRID_UNIT_PX = 8;
    }

    QQmlApplicationEngine engine;

#ifdef Q_OS_IOS
    ExternalProjectPicker externalPicker;
#endif

    const QString qtVersion = QT_VERSION_STR;
    const QString buildDateTime = QStringLiteral("%1 %2").arg(__DATE__, __TIME__);
    engine.rootContext()->setContextProperty("qtVersion", qtVersion);
    engine.rootContext()->setContextProperty("buildDateTime", buildDateTime);

    engine.rootContext()->setContextProperty("configPath", configPath);
    engine.rootContext()->setContextProperty("cachePath", cachePath);

    engine.rootContext()->setContextProperty("GRID_UNIT_PX", GRID_UNIT_PX);

#if !defined(Q_OS_ANDROID)
    engine.rootContext()->setContextProperty("platformResizesView", true);
#else
    engine.rootContext()->setContextProperty("platformResizesView", false);
#endif

#if defined(Q_OS_IOS)
    engine.rootContext()->setContextProperty("platformHasNativeCopyPaste", true);
    engine.rootContext()->setContextProperty("platformHasNativeDragHandles", true);
    engine.rootContext()->setContextProperty("platformIsIpadOs", true);
    engine.rootContext()->setContextProperty("externalPicker", &externalPicker);
#else
    engine.rootContext()->setContextProperty("platformHasNativeCopyPaste", false);
    engine.rootContext()->setContextProperty("platformHasNativeDragHandles", false);
    engine.rootContext()->setContextProperty("platformIsIpadOs", false);
    engine.rootContext()->setContextProperty("externalPicker", nullptr);
#endif

    engine.rootContext()->setContextProperty("oskEventFixer", new ImFixerInstaller());

    engine.load(QUrl("qrc:/qml/main.qml"));

    ProjectManager::setQmlEngine(&engine);
    MessageHandler::setQmlEngine(&engine);
    return app.exec();
}
