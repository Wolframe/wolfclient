#include "serialize/DataStructDescription.hpp"
#include <QDebug>

#undef WOLFRAME_LOWLEVEL_DEBUG

static void skipBrk( QString::const_iterator& itr, const QString::const_iterator& end)
{
	int brkcnt = 1;
	++itr;
	for (; itr != end; ++itr)
	{
		if (*itr == '{')
		{
			++brkcnt;
			continue;
		}
		if (*itr == '}')
		{
			--brkcnt;
			if (brkcnt == 0) break;
		}
	}
}

static void skipSpaces( QString::const_iterator& itr, const QString::const_iterator& end)
{
	for (; itr != end && itr->isSpace(); ++itr) {}
}

static bool isAlphaNum( QChar ch)
{
	return ch.isLetter() || ch == '_' || ch.isDigit();
}


class ParseContext
{
private:
	struct Element
	{
		QString name;
		bool isArray;
		QSharedPointer<DataStructDescription> description;

		Element( const Element& o)
			:name(o.name),isArray(o.isArray),description(o.description){}
		Element( const QString& name_, bool isArray_)
			:name(name_),isArray(isArray_),description(new DataStructDescription()){}
		Element()
			:name(""),isArray(false),description(new DataStructDescription()){}
	};

	QList<Element> stk;
	QList<QString> errmsg;

public:
	ParseContext( const ParseContext& o)
		:stk(o.stk),errmsg(o.errmsg){}
	ParseContext()
	{
		stk.push_back( Element());
	}

	const QList<QString>& errors() const
	{
		return errmsg;
	}

	void setError( const QString& msg)
	{
		QString pt;
		foreach (const Element& ei, stk)
		{
			if (pt.size()) pt.append("/");
			pt.append( ei.name);
		}
		errmsg.push_back( QString("error in datastructure description at ") + pt + ": " + msg);
	}

	const DataStructDescription* description() const
	{
		if (stk.isEmpty()) return 0;
		return &*stk.back().description;
	}

	bool open( const QString& name, bool array)
	{
		stk.push_back( Element( name, array));
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL open(" << name << ")";
#endif
		return true;
	}

	bool close()
	{
		if (stk.size() <= 1)
		{
			setError( "internal: illegal state in structure parser");
			return false;
		}
		Element elem = stk.back();
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "IN close() PARSED" << elem.description->toString();
#endif

		stk.pop_back();
		int idx = stk.back().description->addStructure( elem.name, *elem.description);
		if (0>idx)
		{
			setError( QString( "duplicate definition of structure '") + elem.name + "'");
			return false;
		}
		if (elem.isArray)
		{
			stk.back().description->at( idx).makeArray();
		}
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL close()";
#endif
		return true;
	}

	int defineAttributeConst( const QString& name, const QVariant& value)
	{
		int idx = stk.back().description->addAttribute( name, value);
		if (idx >= 0)
		{
			setError( QString( "duplicate definition of value '") + name + "'");
			return false;
		}
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL defineAttributeConst(" << name << value << ")";
#endif
		return idx;
	}

	int defineAttributeVariable( const QString& name, const QString& var, const QVariant& defaultvalue)
	{
		int idx = stk.back().description->addAttributeVariable( name, var, defaultvalue);
		if (idx >= 0)
		{
			setError( QString( "duplicate definition of value '") + name + "'");
			return false;
		}
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL defineAttributeVariable(" << name << var << defaultvalue << ")";
#endif
		return idx;
	}

	int defineAtomConst( const QString& name, const QVariant& value)
	{
		int idx = stk.back().description->addAtom( name, value);
		if (idx >= 0)
		{
			setError( QString( "duplicate definition of value '") + name + "'");
			return false;
		}
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL defineAtomConst(" << name << value << ")";
#endif
		return idx;
	}

	int defineAtomVariable( const QString& name, const QString& var, const QVariant& defaultvalue)
	{
		int idx = stk.back().description->addAtomVariable( name, var, defaultvalue);
		if (idx >= 0)
		{
			setError( QString( "duplicate definition of value '") + name + "'");
			return false;
		}
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL defineAtomVariable(" << name << var << defaultvalue << ")";
#endif
		return idx;
	}

	struct VariableReference
	{
		QString varname;
		QVariant defaultvalue;
		bool optional;

		VariableReference()
			:optional(false){}
	};

	bool parseVariableReference( VariableReference& def, QString::const_iterator& is, const QString::const_iterator& es)
	{
		QString::const_iterator start = is+1;
		skipBrk( is, es);
		if (is == es)
		{
			setError( QString("curly brackets not balanced {..}:") + QString( start,is-start));
			return false;
		}
		def.varname = QString( start, is-start).trimmed();
		++is;
		int dd;
		if (def.varname.indexOf('{') >= 0)
		{
			setError( QString( "expected variable reference instead of expression '") + def.varname + "'");
			return false;
		}
		if (def.varname == "?")
		{
			def.varname = "";
			def.optional = true;
		}
		else if ((dd=def.varname.indexOf(':')) >= 0)
		{
			def.optional = true;

			QString defaultvaluestr = def.varname.mid( dd+1, def.varname.size()-dd-1);
			if (defaultvaluestr == "?")
			{
				def.varname = def.varname.mid( 0, dd);
			}
			else
			{
				def.defaultvalue = QVariant( defaultvaluestr);
				def.varname = def.varname.mid( 0, dd);
			}
		}
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL parseVariableReference(" << QString(is, es-is) << ") RETURNS" << def.varname << def.defaultvalue;
#endif
		return true;
	}

	bool parseString( QString& str, QString::const_iterator& itr, const QString::const_iterator& end)
	{
		QChar eb = *itr;
		QString::const_iterator start;
		for (start=++itr; itr != end && *itr != eb; ++itr)
		{
			if (*itr == '\\')
			{
				++itr;
				if (itr == end)
				{
					setError( "string not terminated");
					return false;
				}
			}
		}
		if (itr == end)
		{
			setError( "string not terminated");
			return false;
		}
		str = QString( start, itr-start);
		if (itr != end) ++itr;
#ifdef WOLFRAME_LOWLEVEL_DEBUG
		qDebug() << "CALL parseString(" << QString( itr, end-itr) << ") RETURNS" << str;
#endif
		return true;
	}

	bool parseArrayMarker( QString::const_iterator& itr, const QString::const_iterator& end)
	{
		++itr;
		skipSpaces( itr, end);

		if (itr != end && *itr == ']')
		{
			++itr;
			return true;
		}
		else
		{
			setError( "array marker not closed, expected ']' after '['");
			return false;
		}
	}

	bool parseDataStructDescription( QString::const_iterator& si, const QString::const_iterator& se)
	{
		while (si != se)
		{
			skipSpaces( si, se);
			if (si == se) break;

			if (isAlphaNum(*si))
			{
				QString nodename;
				bool isArray = false;

				for (; si != se && isAlphaNum(*si); ++si)
				{
					nodename.push_back( *si);
				}
				skipSpaces( si, se);
				if (*si == '[')
				{
					if (!parseArrayMarker( si, se)) return false;
					isArray = true;
				}
				skipSpaces( si, se);
				if (si == se)
				{
					setError( "unexpected end of expression");
					return false;
				}
				else if (*si == '{')
				{
					//... Content
					QString::const_iterator start = si+1;
					skipBrk( si, se);
					if (si == se)
					{
						setError( "invalid tree (curly brackets not balanced)");
						return false;
					}
					skipSpaces( start, si);
					if (start == si)
					{
						setError( "invalid tree (empty expression or variable reference: \"{}\")");
						return false;
					}
					if (*start == '{')
					{
						//... Content variable reference
						VariableReference vardef;
						if (!parseVariableReference( vardef, si, se)) return false;
						int idx = defineAtomVariable( nodename, vardef.varname, vardef.defaultvalue);
						if (idx >= 0) return false;

						if (vardef.optional)
						{
							if (vardef.varname.isEmpty())
							{
								stk.back().description->at( idx).setAnyValue();
							}
							stk.back().description->at( idx).setOptional();
						}
						if (isArray)
						{
							stk.back().description->at( idx).makeArray();
						}
					}
					else if (*start == '?')
					{
						//... Any content
						++start;
						skipSpaces( start, si);
						if (start != si)
						{
							setError( "invalid tree (unexpected token after '?')");
							return false;
						}
						int idx = defineAtomVariable( nodename, "", QVariant());
						if (idx < 0) return false;
						stk.back().description->at( idx).setOptional();
					}
					else
					{
						//... Content substructure
						if (!open( nodename, isArray)) return false;
						if (!parseDataStructDescription( start, si)) return false;
						if (!close()) return false;
						++si;
					}
				}
				else if (*si == '=')
				{
					// ... Attribute
					++si; skipSpaces( si, se);
					if (si == se)
					{
						setError( "invalid tree (expected expression or string as attribute value)");
						return false;
					}
					if (*si == '\'' || *si == '\"')
					{
						//... Attribute constant
						QString content;
						if (!parseString( content, si, se)) return false;
						if (0>defineAttributeConst( nodename, content)) return false;
					}
					else if (*si == '{')
					{
						//... Attribute variable reference
						VariableReference vardef;
						if (!parseVariableReference( vardef, si, se)) return false;
						int idx = defineAttributeVariable( nodename, vardef.varname, vardef.defaultvalue);
						if (idx < 0) return false;
						if (vardef.optional)
						{
							if (vardef.varname.isEmpty())
							{
								stk.back().description->at( idx).setAnyValue();
							}
							stk.back().description->at( idx).setOptional();
						}
						if (isArray)
						{
							stk.back().description->at( idx).makeArray();
						}
					}
					else if (isAlphaNum(*si))
					{
						QString::const_iterator start = si;
						for (++si; si != se && isAlphaNum(*si); ++si) {}
						if (0>defineAttributeConst( nodename, QString( start, si-start))) return false;
					}
					else
					{
						setError( "invalid tree (expected curly bracket expression or string)");
						return false;
					}
				}
				else
				{
					setError( "invalid tree (expected curly bracket expression or string after identifier)");
				}
			}
			else if (*si == '\'' || *si == '\"')
			{
				//... Content constant
				QString content;
				if (!parseString( content, si, se)) return false;
				if (0>defineAtomConst( "", content)) return false;
			}
			else if (*si == '{')
			{
				//... Content variable
				VariableReference vardef;
				if (!parseVariableReference( vardef, si, se)) return false;
				int idx = defineAtomVariable( "", vardef.varname, vardef.defaultvalue);
				if (idx < 0) return false;
				if (vardef.optional)
				{
					if (vardef.varname.isEmpty())
					{
						stk.back().description->at( idx).setAnyValue();
					}
					stk.back().description->at( idx).setOptional();
				}
			}
			else if (*si == '?')
			{
				//... Any content
				int idx = defineAtomVariable( "", "", QVariant());
				if (idx < 0) return false;
				stk.back().description->at( idx).setAnyValue();
			}
			else
			{
				setError( "expected expression or string");
				return false;
			}
			skipSpaces( si, se);
			if (si != se)
			{
				if (*si == ';' || *si == ',')
				{
					++si;
				}
				else
				{
					setError( "invalid tree");
					return false;
				}
			}
		}
		return true;
	}
};

bool DataStructDescription::parse( const QString& source, QList<QString>& err)
{
	QString::const_iterator si = source.begin(), se = source.end();
	ParseContext ctx;

	if (!ctx.parseDataStructDescription( si, se))
	{
		foreach (const QString& msg, ctx.errors())
		{
			qCritical() << msg;
		}
		err = ctx.errors();
		return false;
	}
	err.clear();
	const DataStructDescription* res = ctx.description();
	if (!res)
	{
		qCritical() << "internal: data structure description parse result empty";
		return false;
	}
	inherit( *res);
	return true;
}
