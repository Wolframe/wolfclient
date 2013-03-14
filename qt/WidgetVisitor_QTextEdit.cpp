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
#include "WidgetVisitor_QTextEdit.hpp"
#include <QDebug>

WidgetVisitorState_QTextEdit::WidgetVisitorState_QTextEdit( QWidget* widget_)
	:WidgetVisitor::State(widget_)
	,m_textEdit(qobject_cast<QTextEdit*>(widget_)){}

void WidgetVisitorState_QTextEdit::clear()
{
	m_textEdit->clear();
}

QVariant WidgetVisitorState_QTextEdit::property( const QByteArray& name)
{
	if (strcmp( name, "") == 0)
	{
		return QVariant( m_textEdit->toPlainText());
	}
	return QVariant();
}

bool WidgetVisitorState_QTextEdit::setProperty( const QByteArray& name, const QVariant& data)
{
	if (strcmp( name, "") == 0)
	{
		m_textEdit->setPlainText( data.toString());
		return true;
	}
	return false;
}

const QList<QByteArray>& WidgetVisitorState_QTextEdit::dataelements() const
{
	static const DataElements ar( "", 0);
	return ar;
}

void WidgetVisitorState_QTextEdit::setState( const QVariant& state)
{
	qDebug() << "Restoring tree state for text edit" << m_textEdit->objectName();
	if (state.isValid()) m_textEdit->setHtml( state.toString());
}

QVariant WidgetVisitorState_QTextEdit::getState() const
{
	return QVariant( m_textEdit->toHtml());
}

