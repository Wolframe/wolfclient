#include "WidgetVisitor_QDateEdit.hpp"
#include "WidgetVisitor.hpp"
#include "WidgetListener.hpp"
#include "WidgetEnabler.hpp"
#include <QSignalMapper>
#include <QWidget>
#include <QObject>
#include <QDebug>

WidgetVisitorState_QDateEdit::WidgetVisitorState_QDateEdit( QWidget* widget_)
	:WidgetVisitor::State(widget_)
	,m_dateEdit(qobject_cast<QDateEdit*>( widget_))
{
	m_dateEdit->setDate( QDate::currentDate());
}

bool WidgetVisitorState_QDateEdit::enter( const QString& /*name*/, bool /*writemode*/)
{
	return false;
}

bool WidgetVisitorState_QDateEdit::leave( bool /*writemode*/)
{
	return false;
}

void WidgetVisitorState_QDateEdit::clear()
{
	m_dateEdit->setDate( QDate::currentDate());
}

QVariant WidgetVisitorState_QDateEdit::property( const QString& name)
{
	if (name.isEmpty())
	{
		QVariant( m_dateEdit->date().toString( Qt::ISODate));
	}
	return QVariant();
}

bool WidgetVisitorState_QDateEdit::setProperty( const QString& name, const QVariant& data)
{
	if (name.isEmpty())
	{
		m_dateEdit->setDate( QDate::fromString( data.toString(), Qt::ISODate));
	}
	return false;
}

void WidgetVisitorState_QDateEdit::setState( const QVariant& state)
{
	qDebug() << "set state for date edit" << m_dateEdit->objectName();
	if (state.isValid()) m_dateEdit->setDate( state.toDate());
}

QVariant WidgetVisitorState_QDateEdit::getState() const
{
	return m_dateEdit->date().isValid()?QVariant( m_dateEdit->date()):QVariant();
}

void WidgetVisitorState_QDateEdit::connectDataSignals( WidgetVisitor::DataSignalType dt, WidgetListener& listener)
{
	switch (dt)
	{
		case WidgetVisitor::SigChanged:
			QObject::connect( m_dateEdit, SIGNAL( dateChanged( const QDate&)), &listener, SLOT( changed()), Qt::UniqueConnection); break;
		case WidgetVisitor::SigActivated:
		case WidgetVisitor::SigEntered:
		case WidgetVisitor::SigPressed:
		case WidgetVisitor::SigClicked:
		case WidgetVisitor::SigDoubleClicked:
			qCritical() << "try to connect to signal not provided" << m_dateEdit->metaObject()->className() << WidgetVisitor::dataSignalTypeName(dt);
	}
}

void WidgetVisitorState_QDateEdit::connectWidgetEnabler( WidgetEnabler& enabler)
{
	QObject::connect( m_dateEdit, SIGNAL( dateChanged( const QDate&)), &enabler, SLOT( changed()), Qt::UniqueConnection);
}

