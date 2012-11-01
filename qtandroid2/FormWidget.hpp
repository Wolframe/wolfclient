//
// FormWidget.hpp
//

#ifndef _FORM_WIDGET_HPP_INCLUDED
#define _FORM_WIDGET_HPP_INCLUDED

#include "DataLoader.hpp"
#include "DataHandler.hpp"
#include "FormLoader.hpp"

#include <QWidget>
#include <QBuffer>
#include <QtUiTools>
#include <QHBoxLayout>

namespace _Wolframe {
	namespace QtClient {

	class FormWidget : public QWidget
	{
	Q_OBJECT

	public:
		FormWidget( FormLoader *_formLoader, DataLoader *_dataLoader, QUiLoader *_uiLoader, QWidget *_parent );
		virtual ~FormWidget( );
		void loadForm( QString name );
		void loadLanguage( QString language );
			
	private:
		QString m_name;			// name of the form
		QUiLoader *m_uiLoader;		// UI loader to user for loading designer XML files
		FormLoader *m_formLoader;	// form loader (visible form)
		DataLoader *m_dataLoader;	// load and saves data (data form)
		QWidget *m_ui;			// the ui loaded from the designer XML file
		DataHandler *m_dataHandler;	// form/data handler
		QLocale m_locale;		// current locale to use for form translations
		QHBoxLayout *m_layout;		// main layout swallowing the form

	private:
		void initialize( );
	
	signals:
		void formLoaded( QString name );
		
	private slots:
		void formLoaded( QString name, QByteArray form );
		void formLocalizationLoaded( QString name, QByteArray localization );
		void dataLoaded( QString name, QByteArray xml );
		void dataSaved( QString name );
		void formDomainLoaded( QString form_name, QString widget_name, QByteArray data );

		void on_buttons_accepted( );
		void on_buttons_rejected( );
	};
	
} // namespace QtClient
} // namespace _Wolframe

#endif // _FORM_WIDGET_HPP_INCLUDED
