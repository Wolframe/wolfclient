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

#ifndef _WIDGET_LISTENER_HPP_INCLUDED
#define _WIDGET_LISTENER_HPP_INCLUDED
#include "WidgetVisitorObject.hpp"
#include "DataLoader.hpp"
#include <QObject>
#include <QWidget>
#include <QSharedPointer>

///\class WidgetListenerImpl
///\brief Implementation of WidgetListener
class WidgetListenerImpl :public WidgetListener
{
public:
	///\brief Function to check, if data signal lister has to be created for a widget
	static bool hasDataSignals( const QWidget* widget_);

public:
	///\brief Constructor
	WidgetListenerImpl( QWidget* widget_, DataLoader* dataLoader_);
	virtual ~WidgetListenerImpl(){}

	virtual void handleDataSignal( DataSignalType dt);
	virtual void handleShowContextMenu( const QPoint& pos);

	void setDebug( bool v);
	void blockSignals( bool v);

private:
	WidgetVisitorObjectR m_state;
	DataLoader* m_dataLoader;
	bool m_debug;
	bool m_hasContextMenu;
};

typedef QSharedPointer<WidgetListenerImpl> WidgetListenerR;

#endif

