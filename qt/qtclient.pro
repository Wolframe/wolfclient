TEMPLATE = app

TARGET = qtclient

CONFIG += thread qt uitools designer debug

DEFINES += WITH_SSL

INCLUDEPATH += .

unix:LIBS += plugins/libwolframewidgets.so
win32:LIBS += plugins/release/wolframewidgets.lib
macxLIBS += plugins/build/Release/libwolframewidgets.dylib

QT += core gui network sql

SOURCES += \	
	MainWindow.cpp \
	FormLoader.cpp \
	FileFormLoader.cpp \
	NetworkFormLoader.cpp \
	DataLoader.cpp \
	FileDataLoader.cpp \
	NetworkDataLoader.cpp \
	WolframeClient.cpp \
	WolframeClientProtocol.cpp \
	WolframeClientProtocolBase.cpp \
	WidgetVisitor.cpp \
	WidgetEnabler.cpp \
	WidgetListener.cpp \
	WidgetRequest.cpp \
	WidgetMessageDispatcher.cpp \
	WidgetVisitorStateConstructor.cpp \
	visitors/WidgetVisitor_QLabel.cpp \
	visitors/WidgetVisitor_QComboBox.cpp \
	visitors/WidgetVisitor_QCheckBox.cpp \
	visitors/WidgetVisitor_QGroupBox.cpp \
	visitors/WidgetVisitor_QButtonGroup.cpp \
	visitors/WidgetVisitor_QRadioButton.cpp \
	visitors/WidgetVisitor_QSpinBox.cpp \
	visitors/WidgetVisitor_QDoubleSpinBox.cpp \
	visitors/WidgetVisitor_QListWidget.cpp \
	visitors/WidgetVisitor_QLineEdit.cpp \
	visitors/WidgetVisitor_QTextEdit.cpp \
	visitors/WidgetVisitor_QPlainTextEdit.cpp \
	visitors/WidgetVisitor_QDateEdit.cpp \
	visitors/WidgetVisitor_QTimeEdit.cpp \
	visitors/WidgetVisitor_QDateTimeEdit.cpp \
	visitors/WidgetVisitor_QTreeWidget.cpp \
	visitors/WidgetVisitor_QTableWidget.cpp \
	visitors/WidgetVisitor_QSlider.cpp \
	visitors/WidgetVisitor_FileChooser.cpp \
	visitors/WidgetVisitor_PictureChooser.cpp \
	DataTree.cpp \
	DataSerializeItem.cpp \
	DataTreeSerialize.cpp \
	FormCall.cpp \
	FormWidget.cpp \
	PreferencesDialog.cpp \
	FormChooseDialog.cpp \
	connection.cpp \
	manageServersDialog.cpp \
	serverDefinitionDialog.cpp \
	settings.cpp \
	loginDialog.cpp \
	LoadMode.cpp \
	DebugTerminal.cpp \
	HistoryLineEdit.cpp \
	qtclient.cpp

HEADERS += \
	MainWindow.hpp \
	FormLoader.hpp \
	FileFormLoader.hpp \
	NetworkFormLoader.hpp \
	DataLoader.hpp \
	FileDataLoader.hpp \
	NetworkDataLoader.hpp \	
	WolframeClient.hpp \
	WolframeClientProtocol.hpp \
	WolframeClientProtocolBase.hpp \
	WidgetVisitor.hpp \
	WidgetEnabler.hpp \
	WidgetListener.hpp \
	WidgetRequest.hpp \
	WidgetMessageDispatcher.hpp \
	WidgetVisitorStateConstructor.hpp \
	visitors/WidgetVisitor_QLabel.hpp \
	visitors/WidgetVisitor_QComboBox.hpp \
	visitors/WidgetVisitor_QCheckBox.hpp \
	visitors/WidgetVisitor_QGroupBox.hpp \
	visitors/WidgetVisitor_QButtonGroup.hpp \
	visitors/WidgetVisitor_QRadioButton.hpp \
	visitors/WidgetVisitor_QSpinBox.hpp \
	visitors/WidgetVisitor_QDoubleSpinBox.hpp \
	visitors/WidgetVisitor_QListWidget.hpp \
	visitors/WidgetVisitor_QLineEdit.hpp \
	visitors/WidgetVisitor_QTextEdit.hpp \
	visitors/WidgetVisitor_QPlainTextEdit.hpp \
	visitors/WidgetVisitor_QDateEdit.hpp \
	visitors/WidgetVisitor_QTimeEdit.hpp \
	visitors/WidgetVisitor_QDateTimeEdit.hpp \
	visitors/WidgetVisitor_QTreeWidget.hpp \
	visitors/WidgetVisitor_QTableWidget.hpp \
	visitors/WidgetVisitor_QSlider.hpp \
	visitors/WidgetVisitor_FileChooser.hpp \
	visitors/WidgetVisitor_PictureChooser.hpp \
	DataTree.hpp \
	DataSerializeItem.hpp \
	DataTreeSerialize.hpp \
	FormCall.hpp \
	FormWidget.hpp \
	PreferencesDialog.hpp \
	FormChooseDialog.hpp \
	connection.hpp \
	manageServersDialog.hpp \
	serverDefinitionDialog.hpp \
	settings.hpp \
	loginDialog.hpp \
	LoadMode.hpp \
	DebugTerminal.hpp \
	HistoryLineEdit.hpp

RESOURCES = \
	qtclient.qrc

FORMS += \
	MainWindow.ui \
	PreferencesDialog.ui \
	PreferencesDialogDeveloper.ui \
	PreferencesDialogInterface.ui \
	loginDialog.ui \
	manageServersDialog.ui \
	serverDefinitionDialog.ui

TRANSLATIONS += \
	i18n/qtclient.de_CH.ts \
	i18n/qtclient.ro_RO.ts

QT_LRELEASE = $$QMAKE_MOC 
QT_LRELEASE ~= s,moc,lrelease,

updateqm.input = TRANSLATIONS
updateqm.hpputput = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
isEmpty(vcproj):updateqm.variable_out = PRE_TARGETDEPS
updateqm.commands = $$QT_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.name = LRELEASE ${QMAKE_FILE_IN}
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
