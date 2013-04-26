#include "WidgetVisitor_QPlainTextEdit.hpp"
#include "WidgetVisitor.hpp"
#include "WidgetListener.hpp"
#include "WidgetEnabler.hpp"
#include <QDebug>

WidgetVisitorState_QPlainTextEdit::WidgetVisitorState_QPlainTextEdit( QWidget* widget_)
	:WidgetVisitor::State(widget_)
	,m_plainTextEdit(qobject_cast<QPlainTextEdit*>(widget_)){}

void WidgetVisitorState_QPlainTextEdit::clear()
{
	m_plainTextEdit->clear();
}

QVariant WidgetVisitorState_QPlainTextEdit::property( const QString& name)
{
	if (name.isEmpty())
	{
		return QVariant( m_plainTextEdit->toPlainText());
	}
	return QVariant();
}

bool WidgetVisitorState_QPlainTextEdit::setProperty( const QString& name, const QVariant& data)
{
	if (name.isEmpty())
	{
		m_plainTextEdit->setPlainText( m_textLoaded = data.toString());
		return true;
	}
	return false;
}

void WidgetVisitorState_QPlainTextEdit::setState( const QVariant& state)
{
	qDebug() << "set state for plain text edit" << m_plainTextEdit->objectName();
	if (state.isValid())
	{
		QList<QVariant> stateval = state.toList();
		if (stateval.size() >= 1)
		{
			m_plainTextEdit->setPlainText( stateval.at(0).toString());
		}
	}
}

QVariant WidgetVisitorState_QPlainTextEdit::getState() const
{
	QList<QVariant> stateval;
	stateval.push_back( QString( m_plainTextEdit->toPlainText()));
	return (m_textLoaded != stateval.at(0).toString())?QVariant( stateval):QVariant();
}

void WidgetVisitorState_QPlainTextEdit::connectDataSignals( WidgetVisitor::DataSignalType dt, WidgetListener& listener)
{
	switch (dt)
	{
		case WidgetVisitor::SigChanged:
			QObject::connect( m_plainTextEdit, SIGNAL( blockCountChanged( int)), &listener, SLOT( changed()), Qt::UniqueConnection);
			QObject::connect( m_plainTextEdit, SIGNAL( cursorPositionChanged()), &listener, SLOT( changed()), Qt::UniqueConnection);
			QObject::connect( m_plainTextEdit, SIGNAL( modificationChanged( bool)), &listener, SLOT( changed()), Qt::UniqueConnection);
			QObject::connect( m_plainTextEdit, SIGNAL( selectionChanged()), &listener, SLOT( changed()), Qt::UniqueConnection);
			QObject::connect( m_plainTextEdit, SIGNAL( textChanged()), &listener, SLOT( changed()), Qt::UniqueConnection);
			break;
		case WidgetVisitor::SigActivated:
		case WidgetVisitor::SigEntered:
		case WidgetVisitor::SigPressed:
		case WidgetVisitor::SigClicked:
		case WidgetVisitor::SigDoubleClicked:
			qCritical() << "try to connect to signal not provided" << m_plainTextEdit->metaObject()->className() << WidgetVisitor::dataSignalTypeName(dt);
	}
}

void WidgetVisitorState_QPlainTextEdit::connectWidgetEnabler( WidgetEnabler& enabler)
{
	QObject::connect( m_plainTextEdit, SIGNAL( modificationChanged( bool)), &enabler, SLOT( changed()), Qt::UniqueConnection);
	QObject::connect( m_plainTextEdit, SIGNAL( selectionChanged()), &enabler, SLOT( changed()), Qt::UniqueConnection);
	QObject::connect( m_plainTextEdit, SIGNAL( textChanged()), &enabler, SLOT( changed()), Qt::UniqueConnection);
}


