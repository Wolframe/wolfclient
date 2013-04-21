/************************************************************************

 Copyright (C) 2011 - 2013 Project Wolframe.
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
#include "WidgetListener.hpp"
#include "WidgetEnabler.hpp"
#include "WidgetVisitorStateConstructor.hpp"
#include "WidgetMessageDispatcher.hpp"
#include "FormWidget.hpp"
#include "FormCall.hpp"
#include <QAbstractButton>
#include <QAbstractScrollArea>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>

WidgetListener::~WidgetListener()
{
}

bool WidgetListener::hasDataSignals( const QWidget* widget_)
{
	foreach (const QByteArray& prop, widget_->dynamicPropertyNames())
	{
		if (prop.startsWith( "datasignal:") && widget_->property( prop).isValid()) return true;
	}
	return false;
}

WidgetListener::WidgetListener( QWidget* widget_, DataLoader* dataLoader_)
	:QObject()
	,m_state(createWidgetVisitorState(widget_))
	,m_dataLoader(dataLoader_)
	,m_debug(false)
	,m_hasContextMenu(false)
{
	if (widget_->property( "contextmenu").isValid())
	{
		widget_->setContextMenuPolicy( Qt::CustomContextMenu);
		connect( widget_, SIGNAL( customContextMenuRequested( const QPoint&)),
			this, SLOT( showContextMenu( const QPoint&)));
		m_hasContextMenu = true;
	}
}

void WidgetListener::setDebug( bool v)
{
	if (!m_hasContextMenu)
	{
		if (v && !m_debug)
		{
			m_state->widget()->setContextMenuPolicy( Qt::CustomContextMenu);
			connect( m_state->widget(), SIGNAL( customContextMenuRequested( const QPoint&)),
				this, SLOT( showContextMenu( const QPoint&)));
		}
		else if (!v && m_debug)
		{
			m_state->widget()->setContextMenuPolicy( Qt::NoContextMenu);
			disconnect( m_state->widget(), SIGNAL( customContextMenuRequested( const QPoint&)),
				this, SLOT( showContextMenu( const QPoint&)));
		}
	}
	m_debug = v;
}

void WidgetListener::handleDataSignal( WidgetVisitor::DataSignalType dt)
{
	WidgetVisitor tv( m_state);
	foreach (QWidget* receiver, tv.get_datasignal_receivers( dt))
	{
		WidgetVisitor visitor( receiver);
		visitor.readAssignments();

		QAbstractButton* button = qobject_cast<QAbstractButton*>( receiver);
		if (button)
		{
			button->toggle();
			button->click();
		}
		if (receiver->property( "action").isValid())
		{
			WidgetMessageDispatcher dp( visitor);
			WidgetRequest domload = dp.getDomainLoadRequest( m_debug);
			if (!domload.content.isEmpty())
			{
				m_dataLoader->datarequest( domload.tag, domload.content);
			}
		}
	}
}

static bool widgetHasActions( QWidget* widget)
{
	if (widget->property( "action").isValid()) return true;
	if (widget->property( "form").isValid()) return true;

	foreach (const QByteArray& prop, widget->dynamicPropertyNames())
	{
		if (prop.startsWith( "action:")) return true;
		if (prop.startsWith( "form:")) return true;
	}
	return false;
}

static QString widgetRequestText( QWidget* widget, const QString& menuitem=QString())
{
	QString text;
	QVariant action;
	WidgetVisitor visitor( widget);
	QList<QString> condprops;
	WidgetRequest request;
	QString title;

	if (menuitem.isEmpty())
	{
		action = widget->property( "action");
		condprops = getActionRequestProperties( visitor);
		request = getActionRequest( visitor, true);
		if (qobject_cast<QPushButton*>( widget))
		{
			title = "Action click";
		}
		else
		{
			title = "Action domain load";
		}
	}
	else
	{
		action = widget->property( QByteArray("action:") + menuitem.toLatin1());
		if (!action.isValid()) return QString();
		condprops = getMenuActionRequestProperties( visitor, menuitem);
		request = getMenuActionRequest( visitor, menuitem, true);
		title = QString("Menu item '") + menuitem + "'";
	}
	if (action.isValid())
	{
		text.append( title);
		text.append( "\n");
		bool enabled = true;
		foreach (const QString& prop, condprops)
		{
			text.append( "Condition property '");
			text.append( prop);
			text.append( "'");
			if (visitor.property( prop).isValid())
			{
				text.append( " defined");
			}
			else
			{
				text.append( " undefined");
				enabled = false;
			}
			text.append( "\n");
		}
		if (enabled)
		{
			text.append( "Request:\n");
			text.append( request.content);
		}
		else
		{
			text.append( "No action because not all conditions are met\n");
		}
	}
	return text;
}

void WidgetListener::showContextMenu( const QPoint& pos)
{
	QPoint globalPos;
	QWidget* widget = m_state->widget();
	if (qobject_cast<QAbstractScrollArea*>(widget))
	{
		QAbstractScrollArea* as = qobject_cast<QAbstractScrollArea*>( widget);
		globalPos = as->viewport()->mapToGlobal( pos);
	}
	else
	{
		globalPos = widget->mapToGlobal( pos);
	}
	QMenu menu;
	WidgetVisitor visitor( m_state);
	QVariant contextmenudef_p( visitor.property( "contextmenu"));
	QList<QString> contextmenudef( contextmenudef_p.toString().split(','));
	int nofMenuEntries = 0;

	if (contextmenudef_p.isValid())
	{
		foreach (const QString& item, contextmenudef)
		{
			QString itemname = item.trimmed();
			if (itemname.isEmpty())
			{
				menu.addSeparator();
			}
			else
			{
				// defined context menu item:
				QByteArray prop( QByteArray( "contextmenu:") + itemname.toLatin1());
				QVariant actiontext( widget->property( prop));
				QAction* action;
				if (actiontext.isValid())
				{
					action = menu.addAction( actiontext.toString());
				}
				else
				{
					action = menu.addAction( item);
				}
				action->setData( QVariant( itemname));
				++nofMenuEntries;

				// check if menu item should be enabled or not:
				QList<QString> menuprops;
				foreach (const QString& menuprop, getMenuActionRequestProperties( visitor, itemname))
				{
					if (!menuprops.contains( menuprop)) menuprops.push_back( menuprop);
				}
				foreach (const QString& menuprop, getFormCallProperties( widget->property( QByteArray("form:") + itemname.toLatin1()).toString()))
				{
					if (!menuprops.contains( menuprop)) menuprops.push_back( menuprop);
				}
				bool enabled = true;
				foreach (const QString& menuprop, menuprops)
				{
					if (!visitor.property( menuprop).isValid())
					{
						qDebug() << "Menu item" << itemname << "disabled because of undefined property" << menuprop;
						enabled = false;
					}
				}
				action->setEnabled( enabled);
			}
		}
	}
	if (m_debug && widgetHasActions( widget))
	{
		if (nofMenuEntries)
		{
			menu.addSeparator();
		}
		QAction* action = menu.addAction( "Debug: Show action requests");
		action->setData( QVariant( 1));
		++nofMenuEntries;
	}
	if (nofMenuEntries)
	{
		QAction* selectedItem = menu.exec( globalPos);
		if (selectedItem)
		{
			FormWidget* form = visitor.formwidget();
			if (!form)
			{
				qCritical() << "no form associated with widget with context menu action";
				return;
			}
			QVariant action = selectedItem->data();
			if (!action.isValid())
			{
				qCritical() << "no data associated context menu action";
				return;
			}
			if (action.type() == QVariant::Int)
			{
				QString debugtext;
				if (widget->property( "action").isValid())
				{
					QString actionrequest = widgetRequestText( widget);
					if (actionrequest.isEmpty())
					{
						debugtext.append( "Action click has no action defined\n");
					}
					else
					{
						debugtext.append( actionrequest);
					}
				}
				foreach (const QString& item, contextmenudef)
				{
					QString itemname = item.trimmed();
					if (!itemname.isEmpty())
					{
						if (!debugtext.isEmpty())
						{
							debugtext.append( "-----------\n");
						}
						QString actionrequest = widgetRequestText( widget, itemname);
						if (actionrequest.isEmpty())
						{
							debugtext.append( "Menu item '");
							debugtext.append( itemname);
							debugtext.append( "' has no action defined\n");
						}
						else
						{
							debugtext.append( actionrequest);
						}
					}
				}
				QMessageBox msg( widget);
				msg.setText( debugtext);
				msg.exec();
			}
			else
			{
				form->executeMenuAction( widget, action.toString());
			}
		}
	}
}





