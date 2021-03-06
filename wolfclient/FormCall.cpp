#include "FormCall.hpp"
#include <QDebug>

QString FormCall::name( const QString& callstr)
{
	int idx=callstr.indexOf('?');
	return idx>=0?callstr.mid(0,idx):callstr;
}

static QVariant parseValue( const QString& callstr, int& idx)
{
	QVariant rt;
	QList<QVariant> rtlist;
	QString::const_iterator itr = callstr.begin() + idx, end = callstr.end();
	QString val;
	for (;itr != end && *itr != '&'; ++itr)
	{
		if (*itr == '\'')
		{
			for (++itr; itr != end; ++itr)
			{
				if (*itr == '\'')
				{
					++itr;
					if (itr != end && *itr == '\'')
					{
						//... escaped quote
						val.push_back( '\'');
						++itr;
					}
					else
					{
						//... end quote
						break;
					}
				}
				if (itr == end) break;
			}
			if (itr == end) break;
		}
		else if (*itr == ',')
		{
			rtlist.push_back( QVariant(val));
			val.clear();
		}
		else
		{
			val.push_back( *itr);
		}
	}
	idx = itr - callstr.begin();
	if (!rtlist.isEmpty())
	{
		rtlist.push_back( QVariant(val));
		rt = QVariant( rtlist);
	}
	else
	{
		rt = QVariant(val);
	}
	return rt;
}

FormCall::FormCall()
{}

FormCall::FormCall( const QString& callstr)
{
	init( callstr);
}

void FormCall::init( const QString& callstr)
{
	m_name.clear();
	m_parameter.clear();

	int paramidx = callstr.indexOf( '?');
	if (paramidx >= 0)
	{
		m_name = callstr.mid( 0, paramidx);
		while (paramidx < callstr.size())
		{
			int eqidx = callstr.indexOf( '=', paramidx);
			int amidx = callstr.indexOf( '&', paramidx+1);
			if (amidx < 0) amidx = callstr.size();
			if (eqidx < 0 || eqidx > amidx)
			{
				qCritical() << "Syntax error in form parameter list. missing assignment '=' in declaration";
				return;
			}
			else
			{
				QString paramname( callstr.mid( paramidx+1, eqidx-paramidx-1));
				paramidx = eqidx + 1;
				QVariant paramvalue( parseValue( callstr, paramidx));
				m_parameter.push_back( Parameter( paramname, paramvalue));
			}
		}
	}
	else
	{
		m_name = callstr;
	}
}

QList<QString> getFormCallProperties( const QString& str)
{
	QList<QString> rt;

	QString::const_iterator itr = str.begin(), end = str.end();
	QString::const_iterator start = end;
	for (; itr != end; ++itr)
	{
		if (*itr == '{')
		{
			++itr;
			start = itr;
		}
		else if (*itr == '}')
		{
			if (start != end)
			{
				QString var = QString( start, itr-start).trimmed();
				if (var.indexOf(':') < 0)
				{
					rt.push_back( var);
				}
				else
				{
					// ... definition { var : defaultvalue } is always fulfilled
				}
			}
		}
	}
	return rt;
}



