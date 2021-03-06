/************************************************************************

 Copyright (C) 2011 - 2014 Project Wolframe.
 All rights reserved.

 This file is part of Project Wolframe.

 Commercial Usage
    Licensees holding valid Project Wolframe Commercial licenses may
    use this file in accordance with the Project Wolframe
    Commercial License Agreement provided with the Software or,
    alternatively, in accordance with the terms contained
    in a written agreement between the licensee and Project Wolframe.

 GNU General Public License Usage
    Alternatively, you can redistribute this file and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Wolframe is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Wolframe.  If not, see <http://www.gnu.org/licenses/>.

 If you have questions regarding the use of this file, please contact
 Project Wolframe.

************************************************************************/
#include "WidgetVisitor_QComboBox.hpp"
#include "WidgetListener.hpp"
#include "WidgetEnabler.hpp"
#include <QDebug>

WidgetVisitorState_QComboBox::WidgetVisitorState_QComboBox( QWidget* widget_)
	:WidgetVisitorObject(widget_)
	,m_mode(None)
	,m_comboBox(qobject_cast<QComboBox*>(widget_))
	,m_elementname(widget_->objectName().toLatin1())
	,m_currentindex(0)
{}

void WidgetVisitorState_QComboBox::clear()
{
	m_comboBox->clear();
	m_currentindex = 0;
	m_mode = None;
}

bool WidgetVisitorState_QComboBox::enter( const QString& name, bool writemode)
{
	switch (m_mode)
	{
		case None:
			if (name == m_elementname || name == "item")
			{
				if (writemode)
				{
					if (m_comboBox->count() < m_comboBox->maxCount())
					{
						m_comboBox->addItem( "");
						m_comboBox->setCurrentIndex( m_comboBox->count()-1);
						m_mode = Value;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if (m_currentindex < m_comboBox->count())
					{
						m_comboBox->setCurrentIndex( m_currentindex++);
						m_mode = Select;
					}
					else
					{
						return false;
					}
				}
				return true;
			}
		case Value:
		case Select:
			break;
	}
	return false;
}

bool WidgetVisitorState_QComboBox::leave( bool /*writemode*/)
{
	switch (m_mode)
	{
		case None:	return false;
		case Value:	m_mode = None; m_comboBox->setCurrentIndex( m_currentindex=0); return true;
		case Select:	m_mode = None; return true;
	}
	return false;
}

QVariant WidgetVisitorState_QComboBox::property( const QString& name)
{
	switch (m_mode)
	{
		case None:
			if (name == "selected")
			{
				if (m_comboBox->currentIndex() >= 0)
				{
					return m_comboBox->itemData( m_comboBox->currentIndex(), Qt::UserRole);
				}
				else
				{
					return m_comboBox->property( "_w_selected");
				}
			}
			break;
		case Value:
			break;
		case Select:
			if (name.isEmpty() || name == "text")
			{
				return QVariant( m_comboBox->currentText());
			}
			if (name == "id")
			{
				if (m_comboBox->currentIndex() < 0) return QVariant();
				return m_comboBox->itemData( m_comboBox->currentIndex(), Qt::UserRole);
			}
	}
	return QVariant();
}

bool WidgetVisitorState_QComboBox::setProperty( const QString& name, const QVariant& data)
{
	switch (m_mode)
	{
		case None:
			if (name == "selected")
			{
				m_comboBox->setProperty( "_w_selected", data);
				initSelected( data);
				return true;
			}
			return false;
		case Value:
			if (name.isEmpty() || name == "text")
			{
				if (m_comboBox->currentIndex() < 0) return false;
				m_comboBox->setItemText( m_comboBox->currentIndex(), data.toString());
				return true;
			}
			if (name == "id")
			{
				if (m_comboBox->currentIndex() < 0) return false;
				m_comboBox->setItemData( m_comboBox->currentIndex(), data, Qt::UserRole);
				return true;
			}
			if (name == "icon")
			{
				if (data.toString().startsWith( ":"))
				{
// internal resource
					m_comboBox->setItemIcon( m_comboBox->currentIndex(), QIcon( data.toString()));
				}
				else
				{
// base64 encoded data from server
					QByteArray decoded = QByteArray::fromBase64( data.toByteArray());
					QPixmap pixmap;
					pixmap.loadFromData( decoded);
					m_comboBox->setItemIcon( m_comboBox->currentIndex(), pixmap);
				}
				return true;
			}
			return false;
		case Select:
			if (name.isEmpty())
			{
				int idx = m_comboBox->findText( data.toString(), Qt::MatchExactly);
				if (idx < 0) return false;
				m_comboBox->setCurrentIndex( idx);
				return true;
			}
			if (name == "id")
			{
				int idx = m_comboBox->findData( data, Qt::UserRole, Qt::MatchExactly);
				if (idx < 0) return false;
				m_comboBox->setCurrentIndex( idx);
				return true;
			}
			return false;
	}
	return false;
}

bool WidgetVisitorState_QComboBox::isArrayElement( const QString& name) const
{
	return (name == m_elementname || name == "item");
}

void WidgetVisitorState_QComboBox::setState( const QVariant& state)
{
	int idx = m_comboBox->findData( state, Qt::UserRole, Qt::MatchExactly);
	if (idx >= 0) m_comboBox->setCurrentIndex( idx);
}

QVariant WidgetVisitorState_QComboBox::getState() const
{
	return m_comboBox->itemData( m_comboBox->currentIndex(), Qt::UserRole);
}

void WidgetVisitorState_QComboBox::initSelected( const QVariant& selected)
{
	int idx = m_comboBox->findData( selected, Qt::UserRole, Qt::MatchExactly);
	if (idx < 0) return;
	m_comboBox->setCurrentIndex( idx);
}

void WidgetVisitorState_QComboBox::endofDataFeed()
{
	QVariant selected = m_comboBox->property( "_w_selected");
	if (selected.isValid())
	{
		initSelected( selected);
		m_comboBox->setProperty( "_w_selected", QVariant());
	}
	m_comboBox->setSizeAdjustPolicy( QComboBox::AdjustToContents);
}

void WidgetVisitorState_QComboBox::connectDataSignals( WidgetListener::DataSignalType dt, WidgetListener& listener)
{
	switch (dt)
	{
		case WidgetListener::SigChanged:
			QObject::connect( m_comboBox, SIGNAL( currentIndexChanged( int)), &listener, SLOT( changed()), Qt::UniqueConnection); break;
		case WidgetListener::SigActivated:
			QObject::connect( m_comboBox, SIGNAL( activated( int)), &listener, SLOT( activated()), Qt::UniqueConnection); break;
		case WidgetListener::SigClicked:
		case WidgetListener::SigPressed:
		case WidgetListener::SigEntered:
		case WidgetListener::SigDoubleClicked:
			qCritical() << "try to connect to signal not provided" << m_comboBox->metaObject()->className() << WidgetListener::dataSignalTypeName(dt);
		case WidgetListener::SigDestroyed: break;
	}
}

void WidgetVisitorState_QComboBox::connectWidgetEnabler( WidgetEnabler& enabler)
{
	QObject::connect( m_comboBox, SIGNAL( currentIndexChanged( int)), &enabler, SLOT( changed()), Qt::UniqueConnection);
	QObject::connect( m_comboBox, SIGNAL( activated( int)), &enabler, SLOT( changed()), Qt::UniqueConnection);
}

