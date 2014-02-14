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

#ifndef _SERVER_DEFINITION_DIALOG_HPP_INCLUDED
#define _SERVER_DEFINITION_DIALOG_HPP_INCLUDED

#include <QDialog>
#include <QAbstractButton>

#include "serverDefinition.hpp"
#include "WolframeClient.hpp"

namespace Ui {
class ServerDefinitionDialog;
}

class SKELETON_VISIBILITY ServerDefinitionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ServerDefinitionDialog( ServerDefinition& params, QWidget *_parent = 0 );
	~ServerDefinitionDialog();

private slots:
	void done( int retCode );	// overwrite the done method in order
					// to validate the data
	void selectCertFile();
	void selectKeyFile();
	void selectCAbundleFile();

	void SSLtoggle();
	void updateSSLfields();
	void updateButtons();

	void testConnection();
	void error( QString error );
	void connected( );
	void disconnected( );

private:
	void buildParams( ServerDefinition& params );

private:
	Ui::ServerDefinitionDialog	*ui;
	ServerDefinition&		m_params;
	QString				m_currentDir;
	WolframeClient *m_client;

};

#endif // _SERVER_DEFINITION_DIALOG_HPP_INCLUDED
