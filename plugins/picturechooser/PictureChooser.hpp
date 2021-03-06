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

#ifndef _PICTURECHOOSER_HPP_INCLUDED
#define _PICTURECHOOSER_HPP_INCLUDED

#include "FileChooser.hpp"

#include <QObject>
#include <QWidget>
#include <QString>
#include <QByteArray>
#include <QLabel>

#ifdef BUILD_AS_PLUGIN
#include <QDesignerExportWidget>
#define EXPORT_AS_PLUGIN QDESIGNER_WIDGET_EXPORT
#else
#define EXPORT_AS_PLUGIN X_EXPORT
#endif

class EXPORT_AS_PLUGIN PictureChooser : public QWidget
{
	Q_OBJECT
	Q_PROPERTY( QString fileName READ fileName WRITE setFileName )
		
	public:
		PictureChooser( QWidget *_parent = 0 );
		QString fileName( ) const;
		QByteArray picture( ) const;

	public slots:
		void setFileName( const QString &_filename );
		void setPicture( const QByteArray &_data );

	signals:
		void changed();

	private:
		void initialize( );

	private slots:
		void updatePicture( );
		void updatePicture( QString _fileName );
		void updatePicture( QByteArray _data );
		
	private:
		QLabel *m_label;
		QByteArray m_data;
		FileChooser *m_fileChooser;
};

#endif // _PICTURECHOOSER_HPP_INCLUDED
