#include "WidgetVisitor_QDateTimeEdit.hpp"
#include "WidgetVisitor.hpp"
#include "WidgetListener.hpp"
#include "WidgetEnabler.hpp"
#include <QSignalMapper>
#include <QWidget>
#include <QDebug>

WidgetVisitorState_QDateTimeEdit::WidgetVisitorState_QDateTimeEdit( QWidget* widget_)
	:WidgetVisitor::State(widget_)
	,m_dateTimeEdit(qobject_cast<QDateTimeEdit*>( widget_))
{
	m_dateTimeEdit->setDateTime( QDateTime::currentDateTime());
}

bool WidgetVisitorState_QDateTimeEdit::enter( const QString& /*name*/, bool /*writemode*/)
{
	return false;
}

bool WidgetVisitorState_QDateTimeEdit::leave( bool /*writemode*/)
{
	return false;
}

void WidgetVisitorState_QDateTimeEdit::clear()
{
	m_dateTimeEdit->setDateTime( QDateTime::currentDateTime());
}

QVariant WidgetVisitorState_QDateTimeEdit::property( const QString& name)
{
	if (name.isEmpty())
	{
		QVariant( m_dateTimeEdit->dateTime().toString( Qt::ISODate));
	}
	return QVariant();
}

bool WidgetVisitorState_QDateTimeEdit::setProperty( const QString& name, const QVariant& data)
{
	if (name.isEmpty())
	{
		m_dateTimeEdit->setDateTime( QDateTime::fromString( data.toString(), Qt::ISODate));
	}
	return false;
}

void WidgetVisitorState_QDateTimeEdit::setState( const QVariant& state)
{
	qDebug() << "set state for date time edit" << m_dateTimeEdit->objectName();
	if (state.isValid()) m_dateTimeEdit->setDateTime( state.toDateTime());
}

QVariant WidgetVisitorState_QDateTimeEdit::getState() const
{
	return m_dateTimeEdit->dateTime().isValid()?QVariant( m_dateTimeEdit->dateTime()):QVariant();
}

void WidgetVisitorState_QDateTimeEdit::connectDataSignals( WidgetVisitor::DataSignalType dt, WidgetListener& listener)
{
	switch (dt)
	{
		case WidgetVisitor::SigChanged:
			QObject::connect( m_dateTimeEdit, SIGNAL( dateTimeChanged( const QDate&)), &listener, SLOT( changed()), Qt::UniqueConnection);
			QObject::connect( m_dateTimeEdit, SIGNAL( timeChanged( const QDate&)), &listener, SLOT( changed()), Qt::UniqueConnection);
			QObject::connect( m_dateTimeEdit, SIGNAL( dateChanged( const QDate&)), &listener, SLOT( changed()), Qt::UniqueConnection);
			break;
		case WidgetVisitor::SigActivated:
		case WidgetVisitor::SigEntered:
		case WidgetVisitor::SigPressed:
		case WidgetVisitor::SigClicked:
		case WidgetVisitor::SigDoubleClicked:
			qCritical() << "try to connect to signal not provided" << m_dateTimeEdit->metaObject()->className() << WidgetVisitor::dataSignalTypeName(dt);
	}
}

void WidgetVisitorState_QDateTimeEdit::connectWidgetEnabler( WidgetEnabler& enabler)
{
	QObject::connect( m_dateTimeEdit, SIGNAL( dateTimeChanged( const QDate&)), &enabler, SLOT( changed()), Qt::UniqueConnection);
	QObject::connect( m_dateTimeEdit, SIGNAL( timeChanged( const QDate&)), &enabler, SLOT( changed()), Qt::UniqueConnection);
	QObject::connect( m_dateTimeEdit, SIGNAL( dateChanged( const QDate&)), &enabler, SLOT( changed()), Qt::UniqueConnection);
}

