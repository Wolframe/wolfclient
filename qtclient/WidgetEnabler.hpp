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

#ifndef _WIDGET_Enabler_HPP_INCLUDED
#define _WIDGET_Enabler_HPP_INCLUDED
#include "WidgetVisitorObject.hpp"
#include <QObject>
#include <QWidget>
#include <QList>
#include <QString>
#include <QSharedPointer>

///\class WidgetEnabler
///\brief Structure to enable widgets based on a set of properties
class WidgetEnabler :public QObject
{
	Q_OBJECT

public:
	///\brief Constructor
	WidgetEnabler( QWidget* widget_, const QList<QString>& properties_);
	virtual ~WidgetEnabler(){}
	QWidget* actionwidget() const			{return m_state->widget();}
	const QList<QString>& actionproperties() const	{return m_properties;}

public slots:
	void changed();

private:
	WidgetVisitorObjectR m_state;
	const QList<QString> m_properties;
};

typedef QSharedPointer<WidgetEnabler> WidgetEnablerR;

#endif

