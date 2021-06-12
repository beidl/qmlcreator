# Generated from qmlcreator.pro.

#####################################################################
## qmlcreator Binary:
#####################################################################

qt_internal_add_executable(qmlcreator
    GUI
    OUTPUT_DIRECTORY "/usr/bin"
    INSTALL_DIRECTORY "/usr/bin"
    SOURCES
        cpp/MessageHandler.cpp cpp/MessageHandler.h
        cpp/ProjectManager.cpp cpp/ProjectManager.h
        cpp/QMLHighlighter.cpp cpp/QMLHighlighter.h
        cpp/SyntaxHighlighter.cpp cpp/SyntaxHighlighter.h
        cpp/components/linenumbershelper.cpp cpp/components/linenumbershelper.h
        cpp/imeventfixer.cpp cpp/imeventfixer.h
        cpp/imfixerinstaller.cpp cpp/imfixerinstaller.h
        cpp/main.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Multimedia
        Qt::Network
        Qt::Qml
        Qt::Quick
        Qt::Sql
        Qt::Svg
        Qt::WebSockets
        Qt::Xml
)

# Resources:
set(qmlcreator_resources_resource_files
    "qml/components/CApplicationWindow.qml"
    "qml/components/CBackButton.qml"
    "qml/components/CCheckBox.qml"
    "qml/components/CCodeArea.qml"
    "qml/components/CContextMenuButton.qml"
    "qml/components/CDialogButton.qml"
    "qml/components/CEditorScrollBar.qml"
    "qml/components/CFileButton.qml"
    "qml/components/CFlickable.qml"
    "qml/components/CHorizontalSeparator.qml"
    "qml/components/CIcon.qml"
    "qml/components/CInformationItem.qml"
    "qml/components/CLabel.qml"
    "qml/components/CListView.qml"
    "qml/components/CNavigationButton.qml"
    "qml/components/CNavigationScrollBar.qml"
    "qml/components/CScrollBar.qml"
    "qml/components/CSettingButton.qml"
    "qml/components/CSlider.qml"
    "qml/components/CSplitView.qml"
    "qml/components/CSwitcher.qml"
    "qml/components/CTextArea.qml"
    "qml/components/CTextField.qml"
    "qml/components/CToolBar.qml"
    "qml/components/CToolButton.qml"
    "qml/components/CTooltip.qml"
    "qml/components/CVerticalSeparator.qml"
    "qml/components/dialogs/BaseDialog.qml"
    "qml/components/dialogs/ConfirmationDialog.qml"
    "qml/components/dialogs/DialogLoader.qml"
    "qml/components/dialogs/FontFamilyDialog.qml"
    "qml/components/dialogs/FontSizeDialog.qml"
    "qml/components/dialogs/IndentSizeDialog.qml"
    "qml/components/dialogs/ListDialog.qml"
    "qml/components/dialogs/MessageDialog.qml"
    "qml/components/dialogs/NewDirDialog.qml"
    "qml/components/dialogs/NewFileDialog.qml"
    "qml/components/dialogs/NewProjectDialog.qml"
    "qml/components/palettes/BasePalette.qml"
    "qml/components/palettes/CutePalette.qml"
    "qml/components/palettes/DarkPalette.qml"
    "qml/components/palettes/LightPalette.qml"
    "qml/components/palettes/PaletteLoader.qml"
    "qml/examples/3D/main.qml"
    "qml/examples/Accelerometer/main.qml"
    "qml/examples/Ambient Light Sensor/main.qml"
    "qml/examples/Analog Clock/Clock.qml"
    "qml/examples/Analog Clock/ClockBackground.qml"
    "qml/examples/Analog Clock/ClockDigits.qml"
    "qml/examples/Analog Clock/ClockHand.qml"
    "qml/examples/Analog Clock/main.qml"
    "qml/examples/Camera/main.qml"
    "qml/examples/Canvas/main.qml"
    "qml/examples/Controls/main.qml"
    "qml/examples/Debugging/Rect.qml"
    "qml/examples/Debugging/main.qml"
    "qml/examples/Device Info/main.qml"
    "qml/examples/Dialogs/ColorDialogTab.qml"
    "qml/examples/Dialogs/MessageDialogTab.qml"
    "qml/examples/Dialogs/main.qml"
    "qml/examples/Gradient Text/main.qml"
    "qml/examples/Graphical Effects/BrightnessContrastTab.qml"
    "qml/examples/Graphical Effects/DesaturateTab.qml"
    "qml/examples/Graphical Effects/DirectionalBlurTab.qml"
    "qml/examples/Graphical Effects/main.qml"
    "qml/examples/Joystick/Joystick.qml"
    "qml/examples/Joystick/Player.qml"
    "qml/examples/Joystick/main.qml"
    "qml/examples/Live Webcam/WebcamsModel.qml"
    "qml/examples/Live Webcam/main.qml"
    "qml/examples/Local Storage/Database.js"
    "qml/examples/Local Storage/main.qml"
    "qml/examples/Looped List View/DateSelector.qml"
    "qml/examples/Looped List View/LoopedListView.qml"
    "qml/examples/Looped List View/LoopedListViewDelegate.qml"
    "qml/examples/Looped List View/NumberSelector.qml"
    "qml/examples/Looped List View/main.qml"
    "qml/examples/Map/main.qml"
    "qml/examples/Multi-touch/TouchItem.qml"
    "qml/examples/Multi-touch/main.qml"
    "qml/examples/Particles/main.qml"
    "qml/examples/Proximity Sensor/main.qml"
    "qml/examples/Repeater/ColorRect.qml"
    "qml/examples/Repeater/main.qml"
    "qml/examples/Settings/main.qml"
    "qml/examples/Shader Effect/main.qml"
    "qml/examples/Styles/SBusyIndicatorStyle.qml"
    "qml/examples/Styles/SButtonStyle.qml"
    "qml/examples/Styles/main.qml"
    "qml/examples/Swipe View/LogInScreen.qml"
    "qml/examples/Swipe View/RegisterScreen.qml"
    "qml/examples/Swipe View/RestorePasswordScreen.qml"
    "qml/examples/Swipe View/SwipeScreen.qml"
    "qml/examples/Swipe View/SwipeView.qml"
    "qml/examples/Swipe View/main.qml"
    "qml/examples/Timer/main.qml"
    "qml/examples/Torch/main.qml"
    "qml/examples/Transform/main.qml"
    "qml/examples/WebSocket/main.qml"
    "qml/examples/XMLHttpRequest/main.qml"
    "qml/main.qml"
    "qml/modules/3D.qml"
    "qml/modules/3DInput.qml"
    "qml/modules/3DRenderer.qml"
    "qml/modules/AudioEngine.qml"
    "qml/modules/Bluetooth.qml"
    "qml/modules/Canvas3D.qml"
    "qml/modules/Controls.qml"
    "qml/modules/Dialogs.qml"
    "qml/modules/Extras.qml"
    "qml/modules/FolderListModel.qml"
    "qml/modules/GraphicalEffects.qml"
    "qml/modules/Layouts.qml"
    "qml/modules/LocalStorage.qml"
    "qml/modules/Location.qml"
    "qml/modules/Models.qml"
    "qml/modules/Multimedia.qml"
    "qml/modules/Nfc.qml"
    "qml/modules/Particles.qml"
    "qml/modules/Positioning.qml"
    "qml/modules/Qml.qml"
    "qml/modules/Quick.qml"
    "qml/modules/Scene3D.qml"
    "qml/modules/Sensors.qml"
    "qml/modules/Settings.qml"
    "qml/modules/Styles.qml"
    "qml/modules/Test.qml"
    "qml/modules/WebSockets.qml"
    "qml/modules/WebView.qml"
    "qml/modules/Window.qml"
    "qml/modules/XmlListModel.qml"
    "qml/screens/AboutScreen.qml"
    "qml/screens/BlankScreen.qml"
    "qml/screens/EditorScreen.qml"
    "qml/screens/ExamplesScreen.qml"
    "qml/screens/FilesScreen.qml"
    "qml/screens/MainMenuScreen.qml"
    "qml/screens/ModulesScreen.qml"
    "qml/screens/PlaygroundScreen.qml"
    "qml/screens/ProjectsScreen.qml"
    "qml/screens/SettingsScreen.qml"
    "qtquickcontrols2.conf"
    "resources/dictionaries/javascript.txt"
    "resources/dictionaries/keywords.txt"
    "resources/dictionaries/properties.txt"
    "resources/dictionaries/qml.txt"
    "resources/fonts/editor/dejavusansmono.ttf"
    "resources/fonts/editor/droidsansmono.ttf"
    "resources/fonts/editor/firamono.ttf"
    "resources/fonts/editor/liberationmono.ttf"
    "resources/fonts/editor/sourcecodepro.ttf"
    "resources/fonts/editor/ubuntumono.ttf"
    "resources/fonts/ui/fontawesome.ttf"
    "resources/fonts/ui/robotobold.ttf"
    "resources/fonts/ui/robotobolditalic.ttf"
    "resources/fonts/ui/robotoitalic.ttf"
    "resources/fonts/ui/robotoregular.ttf"
    "resources/images/file.svg"
    "resources/images/folder.svg"
    "resources/images/icon128.png"
    "resources/images/icon256.png"
    "resources/images/icon512.png"
    "resources/images/particle1.png"
    "resources/images/particle2.png"
    "resources/meshes/logo.obj"
    "resources/templates/JsFile.js"
    "resources/templates/MainFile.qml"
    "resources/templates/QmlFile.qml"
    "resources/translations/qmlcreator_ru.qm"
)

qt_internal_add_resource(qmlcreator "qmlcreator_resources"
    PREFIX
        "/"
    FILES
        ${qmlcreator_resources_resource_files}
)


#### Keys ignored in scope 1:.:.:qmlcreator.pro:<TRUE>:
# MOBILITY = <EMPTY>
# TEMPLATE = "app"
# TRANSLATIONS = "resources/translations/qmlcreator_ru.ts"

## Scopes:
#####################################################################

qt_internal_extend_target(qmlcreator CONDITION lupdate_only
    SOURCES
        qml/*.qml
        qml/components/*.qml
        qml/components/dialogs/*.qml
        qml/components/palettes/*.qml
        qml/modules/*.qml
        qml/screens/*.qml
)

if(ANDROID AND (ANDROID))
    set_property(TARGET qmlcreator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/platform-specific/android
    )
endif()

qt_internal_extend_target(qmlcreator CONDITION ANDROID
    PUBLIC_LIBRARIES
        Qt::3DAnimation
        Qt::3DCore
        Qt::3DExtras
        Qt::3DInput
        Qt::3DLogic
        Qt::3DQuick
        Qt::3DRender
        Qt::AndroidExtras
        Qt::Bluetooth
        Qt::Location
        Qt::Nfc
        Qt::Positioning
        Qt::Sensors
)

#### Keys ignored in scope 3:.:.:qmlcreator.pro:ANDROID:
# OTHER_FILES = "platform-specific/android/AndroidManifest.xml"

qt_internal_extend_target(qmlcreator CONDITION IOS
    PUBLIC_LIBRARIES
        Qt::3DAnimation
        Qt::3DCore
        Qt::3DExtras
        Qt::3DInput
        Qt::3DLogic
        Qt::3DRender
)

#### Keys ignored in scope 4:.:.:qmlcreator.pro:IOS:
# ICON_DATA.files = "$$PWD/platform-specific/ios/Icon.png" "$$PWD/platform-specific/ios/Icon@2x.png" "$$PWD/platform-specific/ios/Icon-60.png" "$$PWD/platform-specific/ios/Icon-60@2x.png" "$$PWD/platform-specific/ios/Icon-72.png" "$$PWD/platform-specific/ios/Icon-72@2x.png" "$$PWD/platform-specific/ios/Icon-76.png" "$$PWD/platform-specific/ios/Icon-76@2x.png" "$$PWD/platform-specific/ios/Def.png" "$$PWD/platform-specific/ios/Def@2x.png" "$$PWD/platform-specific/ios/Def-Portrait.png" "$$PWD/platform-specific/ios/Def-568h@2x.png"
# OTHER_FILES = "$$QMAKE_INFO_PLIST"
# QMAKE_BUNDLE_DATA = "ICON_DATA" "launch_xib"
# QMAKE_INFO_PLIST = "$$PWD/platform-specific/ios/Project-Info.plist"
# QMAKE_TARGET_BUNDLE_PREFIX = "me.fredl"
# launch_xib.files = "$$PWD/platform-specific/ios/Launch.xib"
# xcode_product_bundle_identifier_setting.value = "me.fredl.qmlcreator"

qt_internal_extend_target(qmlcreator CONDITION CONFIG___contains___click
    DEFINES
        UBUNTU_CLICK
    PUBLIC_LIBRARIES
        Qt::Bluetooth
        Qt::Location
        Qt::Nfc
        Qt::Positioning
        Qt::Sensors
)

#### Keys ignored in scope 5:.:.:qmlcreator.pro:CONFIG___contains___click:
# CLICK_ARCH = "$$system(dpkg-architecture -qDEB_HOST_ARCH)"
# INSTALLS = "mfile" "apparmor" "iconfile" "desktop" "runscript"
# QMAKE_EXTRA_COMPILERS = "manifest_file"
# UBUNTU_MANIFEST_FILE = "$$PWD/manifest.json.in"
# apparmor.files = "$$PWD/qmlcreator.apparmor"
# apparmor.path = "/"
# desktop.files = "$$PWD/qmlcreator.desktop"
# desktop.path = "/"
# iconfile.files = "$$PWD/qmlcreator.png"
# iconfile.path = "/"
# manifest_file.CONFIG = "no_link" "add_inputs_as_makefile_deps" "target_predeps"
# manifest_file.commands = "sed" "s/@CLICK_ARCH@/$$CLICK_ARCH/g" "${QMAKE_FILE_NAME}" ">" "${QMAKE_FILE_OUT}"
# manifest_file.input = "UBUNTU_MANIFEST_FILE"
# manifest_file.output = "manifest.json"
# mfile.CONFIG = "no_check_exist"
# mfile.files = "$$OUT_PWD/manifest.json"
# mfile.path = "/"
# runscript.files = "$$PWD/run.sh"
# runscript.path = "/"
# target.path = "/"
