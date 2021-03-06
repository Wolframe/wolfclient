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

#ifndef _FILE_DATA_LOADER_INCLUDED
#define _FILE_DATA_LOADER_INCLUDED

#include "DataLoader.hpp"

class FileDataLoader : public DataLoader
{	
	// intentionally omitting Q_OBJECT here, is done in DataLoader!
	
	public:
		FileDataLoader( QString dir );
		virtual ~FileDataLoader( ) {};

		virtual void datarequest( const QString& /*cmd*/, const QString& /*tag*/, const QByteArray& /*content*/);

	public slots:
		virtual void gotAnswer( bool success, const QString& tag, const QByteArray& content);

	private:
		void handleCreate( QString name, QByteArray data, QHash<QString, QString> *props );
		void handleRead( QString name, QHash<QString, QString> *props );
		void handleUpdate( QString name, QByteArray data, QHash<QString, QString> *props );
		void handleDelete( QString name, QHash<QString, QString> *props );
		void handleDomainDataLoad( QString formName, QString widgetName, QHash<QString, QString> *props );
		
	private:
		QString m_dir;
};

#endif // _FILE_DATA_LOADER_INCLUDED
//
// FileDataLoader.hpp
//
