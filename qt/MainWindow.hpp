//
// MainWindow.hpp
//

#ifndef _MAIN_WINDOW_HPP_INCLUDED
#define _MAIN_WINDOW_HPP_INCLUDED

#include <QCommandLine>	
#include <QMainWindow>
#include <QtUiTools>
#include <QTranslator>
#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMap>
#include <QAction>
#include <QCloseEvent>

#include "global.hpp"
#include "FormLoader.hpp"
#include "DebugTerminal.hpp"
#include "WolframeClient.hpp"
#include "FormWidget.hpp"
#include "loginDialog.hpp"
#include "LoadMode.hpp"
#include "ui_MainWindow.h"
#include "settings.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow( QWidget *_parent = 0 );
		virtual ~MainWindow( );		
			
	private:
		Ui::MainWindow m_ui;		// ui definition from designer
		QTranslator m_translatorApp;	// contains the translations for this application
		QTranslator m_translatorQt; 	// contains the translations for qt
		QCommandLine *m_cmdline;	// command line parser	
		FormWidget *m_formWidget;	// current active form
		QUiLoader *m_uiLoader;		// the UI loader for all forms
		FormLoader *m_formLoader;	// form loader (visible form)
		DataLoader *m_dataLoader;	// load and saves data (data form)
		DebugTerminal *m_debugTerminal;	// protocol debug terminal (interactive)
		WolframeClient *m_wolframeClient; // the client protocol class
		QString m_currentLanguage;	// code of the current interface language
		QString m_host;			// wolframe server port to use
		unsigned short m_port;		// wolframe port to use
		bool m_secure;			// use SSL for wolframe protocol
		bool m_checkSSL;		// verify SSL connection
		QString m_clientCertFile;	// filename of the client certfificate
		QString m_clientKeyFile;	// filename of the client key file
		QString m_CACertFile;		// filename of the CA certificate
		LoadMode m_uiLoadMode;		// how to load UI forms and data
		LoadMode m_dataLoadMode;	// how to load data domains
		bool m_debug;			// show debug windows from the beginning
		bool m_developer;		// show developer backdors
		LoginDialog *m_loginDialog;	// the login dialog
		QString m_settings;		// file to read settings from
		QString m_uiFormsDir;		// for FileFormLoader (forms dir)
		QString m_uiFormTranslationsDir; // for FileFormLoader (i18n dir)
		QString m_uiFormResourcesDir;	// for FileFormLoader (resources dir)
		QString m_dataLoaderDir;	// for FileDataLoader (data dir)
		QStringList m_languages;	// available interface translations
		QString m_language;		// the current language of the interface
		QMdiArea *m_mdiArea;		// pointer to MDI workspace in the main window
		QStringList m_forms;		// list of available forms
		bool m_mdi;
		QActionGroup *m_subWinGroup;	// group holding list of currently opened MDI subwindows
		QMap<QAction *, QMdiSubWindow *> m_subWinMap; // maps actions to MDI subwindows
		QMap<QMdiSubWindow *, QAction *> m_revSubWinMap; // reverse map (the above is actually a bimap)
		ApplicationSettings settings;	// Application settings
		
	public slots:
		void readSettings( );
		void parseArgs( );
		void initialize( );
		void loadLanguages( );
		void loadForm( QString formName );
		void loadLanguage( QString language );
		
	protected:
		void changeEvent( QEvent *_event );
		void closeEvent( QCloseEvent *e );
	
	private:
		void switchTranslator( QTranslator &translator, const QString &filename, const QString &i18n );
		void CreateMdiSubWindow( const QString form );
		int nofSubWindows( ) const;
		void activateAction( const QString name, bool enabled );
		QString composeWindowListTitle( const int idx, const QString title );
		QKeySequence::StandardKey defaultKeySequenceFromString( const QString s );
		void updateActionShortcuts( );
		void addDeveloperMenu( );

	private slots:
// slots for command line parsing
		void switchFound( const QString &name );
		void optionFound( const QString &name, const QVariant &value );
		void paramFound( const QString &name, const QVariant &value );
		void parseError( const QString &error );

// slots for the wolframe client		
		void wolframeError( QString error );
		void connected( );
		void disconnected( );
		void mechsReceived( QStringList mechs );
		void authOk( );
		void authFailed( );

// menu slots
		void languageSelected( QAction *action );

// form loader slots
		void languageCodesLoaded( QStringList languages );
		void formListLoaded( QStringList forms );
		void formLoaded( QString name );
		void formError( QString error );
		
// MDI slots
		void subWindowSelected( QAction *action );
		void subWindowChanged( QMdiSubWindow *w );
		void updateMdiMenusAndToolbars( );
		void updateWindowMenu( );

// generic updating of status in menus and toolbars
		void updateMenusAndToolbars( );

// auto-wired slots for the menu
		void on_actionRestart_triggered( );
		void on_actionExit_triggered( );
		void on_actionPreferences_triggered( );
		void on_actionAbout_triggered( );
		void on_actionAboutQt_triggered( );
		void on_actionDebugTerminal_triggered( bool checked );  
		void on_actionOpenForm_triggered( );
		void on_actionOpenFormNewWindow_triggered( );	
		void on_actionReloadWindow_triggered( );
		void on_actionNextWindow_triggered( );
		void on_actionPreviousWindow_triggered( );
		void on_actionClose_triggered( );
		void on_actionCloseAll_triggered( );
		void on_actionLogin_triggered( );
		void on_actionLogout_triggered( );
		void on_actionManageServers_triggered( );
};

#endif // _MAIN_WINDOW_HPP_INCLUDED
