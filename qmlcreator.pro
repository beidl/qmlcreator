QT += \
    core gui qml quick \
    multimedia sql \
    network websockets \
    xml svg

TARGET = qmlcreator
TEMPLATE = app

target.path = /usr/bin

CONFIG += mobility
MOBILITY =

RESOURCES += \
    qmlcreator_resources.qrc

HEADERS += \
    cpp/ProjectManager.h \
    cpp/QMLHighlighter.h \
    cpp/SyntaxHighlighter.h \
    cpp/MessageHandler.h \
    cpp/components/linenumbershelper.h \
    cpp/imeventfixer.h \
    cpp/imfixerinstaller.h

SOURCES += \
    cpp/main.cpp \
    cpp/components/linenumbershelper.cpp \
    cpp/imeventfixer.cpp \
    cpp/imfixerinstaller.cpp \
    cpp/ProjectManager.cpp \
    cpp/QMLHighlighter.cpp \
    cpp/SyntaxHighlighter.cpp \
    cpp/MessageHandler.cpp

lupdate_only {
SOURCES += \
    qml/components/*.qml \
    qml/components/dialogs/*.qml \
    qml/components/palettes/*.qml \
    qml/modules/*.qml \
    qml/screens/*.qml \
    qml/*.qml
}

TRANSLATIONS = resources/translations/qmlcreator_ru.ts

android {
    QT += androidextras \
        sensors bluetooth nfc \
        positioning location \
        3dcore 3drender 3dinput 3dlogic \
        3dextras 3dquick 3danimation
    OTHER_FILES += platform-specific/android/AndroidManifest.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/platform-specific/android
}

ios {
    QT += 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation

    QMAKE_IOS_DEPLOYMENT_TARGET=12.0

    OBJECTIVE_SOURCES += \
        cpp/ios/externalprojectpicker.mm

    OBJECTIVE_HEADERS += \
        cpp/ios/externalprojectpicker.h

    LIBS += -framework UIKit

    ICON_DATA.files = \
        $$PWD/platform-specific/ios/Icon.png \
        $$PWD/platform-specific/ios/Icon@2x.png \
        $$PWD/platform-specific/ios/Icon-60.png \
        $$PWD/platform-specific/ios/Icon-60@2x.png \
        $$PWD/platform-specific/ios/Icon-72.png \
        $$PWD/platform-specific/ios/Icon-72@2x.png \
        $$PWD/platform-specific/ios/Icon-76.png \
        $$PWD/platform-specific/ios/Icon-76@2x.png \
        $$PWD/platform-specific/ios/Def.png \
        $$PWD/platform-specific/ios/Def@2x.png \
        $$PWD/platform-specific/ios/Def-Portrait.png \
        $$PWD/platform-specific/ios/Def-568h@2x.png
    QMAKE_BUNDLE_DATA += ICON_DATA

    launch_xib.files = $$PWD/platform-specific/ios/Launch.xib
    QMAKE_BUNDLE_DATA += launch_xib

    QMAKE_INFO_PLIST = $$PWD/platform-specific/ios/Project-Info.plist
    OTHER_FILES += $$QMAKE_INFO_PLIST

    xcode_product_bundle_identifier_setting.value = "me.fredl.qmlcreator"

    MY_ENTITLEMENTS.name = CODE_SIGN_ENTITLEMENTS
    MY_ENTITLEMENTS.value = $$PWD/platform-specific/ios/qmlcreator.entitlements
    QMAKE_MAC_XCODE_SETTINGS += MY_ENTITLEMENTS

    CODE_SIGN_ENTITLEMENTS = $$PWD/platform-specific/ios/qmlcreator.entitlements

    Q_PRODUCT_BUNDLE_IDENTIFIER.name = PRODUCT_BUNDLE_IDENTIFIER
    Q_PRODUCT_BUNDLE_IDENTIFIER.value = me.fredl.qmlcreator
    QMAKE_MAC_XCODE_SETTINGS += Q_PRODUCT_BUNDLE_IDENTIFIER

    QMAKE_TARGET_BUNDLE_PREFIX = me.fredl
}

contains(CONFIG, click) {
    DEFINES += UBUNTU_CLICK
    QT += \
        sensors bluetooth nfc \
        positioning location

    # figure out the current build architecture
    CLICK_ARCH=$$system(dpkg-architecture -qDEB_HOST_ARCH)
    # do not remove this line, it is required by the IDE even if you do
    # not substitute variables in the manifest file
    UBUNTU_MANIFEST_FILE = $$PWD/manifest.json.in


    # substitute the architecture in the manifest file
    manifest_file.output   = manifest.json
    manifest_file.CONFIG  += no_link \
                             add_inputs_as_makefile_deps\
                             target_predeps
    manifest_file.commands = sed s/@CLICK_ARCH@/$$CLICK_ARCH/g ${QMAKE_FILE_NAME} > ${QMAKE_FILE_OUT}
    manifest_file.input = UBUNTU_MANIFEST_FILE
    QMAKE_EXTRA_COMPILERS += manifest_file

    # installation path of the manifest file
    mfile.CONFIG += no_check_exist
    mfile.files  += $$OUT_PWD/manifest.json
    mfile.path = /

    # AppArmor profile
    apparmor.files += $$PWD/qmlcreator.apparmor
    apparmor.path = /

    # Launcher icon
    iconfile.files += $$PWD/qmlcreator.png
    iconfile.path = /

    # Desktop launcher
    desktop.files += $$PWD/qmlcreator.desktop
    desktop.path = /

    # Run script
    runscript.files += $$PWD/run.sh
    runscript.path = /

    target.path = /

    INSTALLS += mfile apparmor iconfile desktop runscript
}

INSTALLS += target
