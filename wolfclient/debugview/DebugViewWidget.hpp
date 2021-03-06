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
#ifndef _WOLFRAME_DEBUG_VIEW_WIDGET_HPP_INCLUDED
#define _WOLFRAME_DEBUG_VIEW_WIDGET_HPP_INCLUDED
#include "debugview/DebugLogTree.hpp"
#include <QWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCloseEvent>
#include <QLineEdit>

class DebugViewWidget : public QWidget
{
Q_OBJECT

public:
	DebugViewWidget( QWidget *parent_ = 0);
	virtual ~DebugViewWidget();
	void bringToFront();

	virtual void closeEvent( QCloseEvent* event);

signals:
	void closed();

public slots:
	void refresh();
	void printMessages( QTreeWidgetItem* item, int column);
	void clear();
	void debugLevelChanged( int idx);
	void currentSubstringChanged( const QString& substr);

private:
	void addSubTree( QTreeWidgetItem* viewnode, const DebugLogTree::NodeStruct* datanode, int level);
	void printMessages( int nodeid);
	void clearMessages();

private:
	QIcon m_icon_error;
	QIcon m_icon_warning;
	QIcon m_icon_ok;
	QTableWidget* m_msglist;
	QTreeWidget* m_tree;
	QBoxLayout* m_layout;
	QComboBox* m_levelSelect;
	QLineEdit* m_patternEdit;
	LogLevel m_level;
	QString m_substr;
};

#endif


