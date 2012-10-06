//
// DataHandler.cpp
//

#include "DataHandler.hpp"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QWidget>
#include <QDebug>

#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QSlider>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QGroupBox>

namespace _Wolframe {
	namespace QtClient {

void DataHandler::writeFormData( QString form_name, QWidget *form, QByteArray *data )
{
	QXmlStreamWriter xml( data );
	xml.setAutoFormatting( true );
	xml.setAutoFormattingIndent( 2 );
	
	xml.writeStartDocument( );
	xml.writeStartElement( form_name );

	QList<QWidget *> widgets = form->findChildren<QWidget *>( );
	foreach( QWidget *widget, widgets ) {
		QString clazz = widget->metaObject( )->className( ); 
		QString name = widget->objectName( );
		
// ignore internal elements
		if( name == "" || name.startsWith( "qt_" ) ||
			clazz == "QLabel" ||
			!widget->isEnabled( ) ) {
			continue;
		}
		
		if( clazz == "QLineEdit" ) {
			QLineEdit *lineEdit = qobject_cast<QLineEdit *>( widget );
			QString text = lineEdit->text( );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QDateEdit" ) {
			QDateEdit *dateEdit = qobject_cast<QDateEdit *>( widget );
			QString text = dateEdit->date( ).toString( Qt::ISODate );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QTimeEdit" ) {
			QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>( widget );
			QString text = timeEdit->time( ).toString( Qt::ISODate );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QDateTimeEdit" ) {
			QDateTimeEdit *dateTimeEdit = qobject_cast<QDateTimeEdit *>( widget );
			QString text = dateTimeEdit->dateTime( ).toString( Qt::ISODate );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QComboBox" ) {
			QComboBox *comboBox = qobject_cast<QComboBox *>( widget );
			QString text = comboBox->itemText( comboBox->currentIndex( ) );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QSpinBox" ) {
			QSpinBox *spinBox = qobject_cast<QSpinBox *>( widget );
			QString text = QString::number( spinBox->value( ) );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QDoubleSpinBox" ) {
			QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>( widget );
			QString text = QString::number( spinBox->value( ) );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QSlider" ) {
			QSlider *slider = qobject_cast<QSlider *>( widget );
			QString text = QString::number( slider->value( ) );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QPlainTextEdit" ) {
			QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>( widget );
			QString text = plainTextEdit->toPlainText( );
			xml.writeTextElement( "", name, text );
		} else if( clazz == "QTextEdit" ) {
			QTextEdit *textEdit = qobject_cast<QTextEdit *>( widget );
			QString html = textEdit->toHtml( );
			xml.writeTextElement( "", name, html );
		} else if( clazz == "QCheckBox" ) {
			QCheckBox *checkBox = qobject_cast<QCheckBox *>( widget );
			QObject *parent = widget->parent( );
			QString clazzParent = parent->metaObject( )->className( ); 
			if( clazzParent == "QGroupBox" ) {
				QString groupName = parent->objectName( );
				if( checkBox->isChecked( ) ) {
					xml.writeTextElement( "", groupName, name );
				}
			} else {
				xml.writeTextElement( "", name, checkBox->isChecked( ) ? "true" : "false" );
			}
		} else if( clazz == "QRadioButton" ) {
			QRadioButton *radioButton = qobject_cast<QRadioButton *>( widget );
			QObject *parent = widget->parent( );
			QString clazzParent = parent->metaObject( )->className( ); 
			if( clazzParent == "QGroupBox" ) {
				QString groupName = parent->objectName( );
				if( radioButton->isChecked( ) ) {
					xml.writeTextElement( "", groupName, name );
				}
			} else {
				xml.writeTextElement( "", name, radioButton->isChecked( ) ? "true" : "false" );
			}
		}
		
		qDebug( ) << "Wrote " << clazz << name;
	}
	
	xml.writeEndElement( );
	xml.writeEndDocument( );
}

void DataHandler::resetForm( QWidget *form )
{
	QList<QWidget *> widgets = form->findChildren<QWidget *>( );
	foreach( QWidget *widget, widgets ) {
		QString clazz = widget->metaObject( )->className( ); 
		QString name = widget->objectName( );
// ignore internal elements
		if( name == "" || name.startsWith( "qt_" ) ||
			clazz == "QLabel" ||
			!widget->isEnabled( ) ) {
			continue;
		}
		
		if( clazz == "QLineEdit" ) {
			QLineEdit *lineEdit = qobject_cast<QLineEdit *>( widget );
			lineEdit->clear( );
		} else if( clazz == "QDateEdit" ) {
			QDateEdit *dateEdit = qobject_cast<QDateEdit *>( widget );
			// TODO
		} else if( clazz == "QTimeEdit" ) {
			QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>( widget );
			// TODO
		} else if( clazz == "QDateTimeEdit" ) {
			QDateTimeEdit *dateTimeEdit = qobject_cast<QDateTimeEdit *>( widget );
		} else if( clazz == "QComboBox" ) {
			QComboBox *comboBox = qobject_cast<QComboBox *>( widget );
			comboBox->clear( );
		} else if( clazz == "QSpinBox" ) {
			QSpinBox *spinBox = qobject_cast<QSpinBox *>( widget );
			// TODO
		} else if( clazz == "QDoubleSpinBox" ) {
			QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>( widget );
			// TODO
		} else if( clazz == "QSlider" ) {
			QSlider *slider = qobject_cast<QSlider *>( widget );
			// TODO
		} else if( clazz == "QPlainTextEdit" ) {
			QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>( widget );
			plainTextEdit->clear( );
		} else if( clazz == "QTextEdit" ) {
			QTextEdit *textEdit = qobject_cast<QTextEdit *>( widget );
			textEdit->clear( );
		} else if( clazz == "QCheckBox" ) {
			QCheckBox *checkBox = qobject_cast<QCheckBox *>( widget );
			checkBox->setChecked( false );
		} else if( clazz == "QRadioButton" ) {
			QRadioButton *radioButton = qobject_cast<QRadioButton *>( widget );
			radioButton->setChecked( false );
		}
		
		qDebug( ) << "Reset " << clazz << name;
	}
}

void DataHandler::readFormData( QString name, QWidget *form, QByteArray &data )
{
	QXmlStreamReader xml( data );
	QWidget *widget = 0;
	bool inForm = false;
	QString clazz;
	
	resetForm( form );
	
	while( !xml.atEnd( ) ) {
		xml.readNext( );
		if( xml.isStartElement( ) && xml.name( ) == name ) {
			inForm = true;
		} else if( xml.isEndElement( ) && xml.name( ) == name ) {
			inForm = false;
		} else {
			if( inForm ) {
				if( xml.isStartElement( ) ) {
					widget = qFindChild<QWidget *>( form, xml.name( ).toString( ) );
					if( widget ) {
						clazz = widget->metaObject( )->className( ); 
						QString text = xml.readElementText( QXmlStreamReader::ErrorOnUnexpectedElement );
						if( clazz == "QLineEdit" ) {
							QLineEdit *lineEdit = qobject_cast<QLineEdit *>( widget );
							lineEdit->setText( text );
						} else if( clazz == "QDateEdit" ) {
							QDateEdit *dateEdit = qobject_cast<QDateEdit *>( widget );
							dateEdit->setDate( QDate::fromString( text, Qt::ISODate ) );
						} else if( clazz == "QTimeEdit" ) {
							QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>( widget );
							timeEdit->setTime( QTime::fromString( text, Qt::ISODate ) );
						} else if( clazz == "QDateTimeEdit" ) {
							QDateTimeEdit *dateTimeEdit = qobject_cast<QDateTimeEdit *>( widget );
							dateTimeEdit->setDateTime( QDateTime::fromString( text, Qt::ISODate ) );
						} else if( clazz == "QSpinBox" ) {
							QSpinBox *spinBox = qobject_cast<QSpinBox *>( widget );
							spinBox->setValue( text.toInt( ) );
						} else if( clazz == "QDoubleSpinBox" ) {
							QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>( widget );
							doubleSpinBox->setValue( text.toDouble( ) );
						} else if( clazz == "QComboBox" ) {
							QComboBox *comboBox = qobject_cast<QComboBox *>( widget );
							for( int idx = 0; idx < comboBox->count( ); idx++ ) {
								if( comboBox->itemText( idx ) == text ) {
									comboBox->setCurrentIndex( idx );
									break;
								}
							}
						} else if( clazz == "QCheckBox" ) {
							QObject *parent = widget->parent( );
							QString clazzParent = parent->metaObject( )->className( ); 
							if( clazzParent != "QGroupBox" ) {
								QCheckBox *checkBox = qobject_cast<QCheckBox *>( widget );
								if( text == "true"  ) {
									checkBox->setChecked( true );
								} else {
									checkBox->setChecked( false );
								}
							}
						} else if( clazz == "QRadioButton" ) {
							QObject *parent = widget->parent( );
							QString clazzParent = parent->metaObject( )->className( ); 
							if( clazzParent != "QGroupBox" ) {
								QRadioButton *radioButton = qobject_cast<QRadioButton *>( widget );
								if( text == "true"  ) {
									radioButton->setChecked( true );
								} else {
									radioButton->setChecked( false );
								}
							}
						} else if( clazz == "QSlider" ) {
							QSlider *slider = qobject_cast<QSlider *>( widget );
							slider->setValue( text.toInt( ) );
						} else if( clazz == "QPlainTextEdit" ) {
							QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>( widget );
							plainTextEdit->setPlainText( text );
						} else if( clazz == "QTextEdit" ) {
							QTextEdit *textEdit = qobject_cast<QTextEdit *>( widget );
							textEdit->setHtml( text );
						} else if( clazz == "QGroupBox" ) {
							QList<QWidget *> children = widget->findChildren<QWidget *>( );
							foreach( QWidget *child, children ) {
								QString subClazz = child->metaObject( )->className( ); 
								QString subName = child->objectName( );
								if( subClazz == "QRadioButton" ) {
									QRadioButton *radioButton = qobject_cast<QRadioButton *>( child );
									QString subText = radioButton->text( );
									qDebug( ) << "radio" << name << subText << subText << name << subName;
									radioButton->setChecked( text.compare( subName ) == 0 );
								} else if( subClazz == "QCheckBox" ) {
									QCheckBox *checkBox = qobject_cast<QCheckBox *>( child );
									QString subText = checkBox->text( );
									qDebug( ) << "checkbox" << name << subText << subText << name << subName;
									if( text.compare( subName ) == 0 ) {
										checkBox->setChecked( true );
									}
								}
							}								
						}
					}
				}
			}
		}
	}
	if( xml.hasError( ) ) {
		qDebug( ) << xml.errorString( );
	}
}

} // namespace QtClient
} // namespace _Wolframe
