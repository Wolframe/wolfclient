#include "WidgetVisitorStateConstructor.hpp"
#include "visitors/WidgetVisitor_QLabel.hpp"
#include "visitors/WidgetVisitor_QComboBox.hpp"
#include "visitors/WidgetVisitor_QCheckBox.hpp"
#include "visitors/WidgetVisitor_QGroupBox.hpp"
#include "visitors/WidgetVisitor_QButtonGroup.hpp"
#include "visitors/WidgetVisitor_QRadioButton.hpp"
#include "visitors/WidgetVisitor_QPushButton.hpp"
#include "visitors/WidgetVisitor_QSpinBox.hpp"
#include "visitors/WidgetVisitor_QDoubleSpinBox.hpp"
#include "visitors/WidgetVisitor_QListWidget.hpp"
#include "visitors/WidgetVisitor_QTreeWidget.hpp"
#include "visitors/WidgetVisitor_QLineEdit.hpp"
#include "visitors/WidgetVisitor_QTextEdit.hpp"
#include "visitors/WidgetVisitor_QPlainTextEdit.hpp"
#include "visitors/WidgetVisitor_QDateEdit.hpp"
#include "visitors/WidgetVisitor_QTimeEdit.hpp"
#include "visitors/WidgetVisitor_QDateTimeEdit.hpp"
#include "visitors/WidgetVisitor_QTableWidget.hpp"
#include "visitors/WidgetVisitor_QSlider.hpp"
#include "visitors/WidgetVisitor_FileChooser.hpp"
#include "visitors/WidgetVisitor_PictureChooser.hpp"

typedef WidgetVisitor::StateR (*StateConstructor)( QWidget* widget);

class WidgetVisitorState_default
	:public WidgetVisitor::State
{
public:
	WidgetVisitorState_default( QWidget* widget_)
		:WidgetVisitor::State(widget_){}

	virtual QVariant property( const QString& /*name*/)				{return QVariant();}
	virtual bool setProperty( const QString& /*name*/, const QVariant& /*data*/)	{return false;}
};

struct WidgetVisitorTypeMap :QHash<QString,StateConstructor>
{
	template <class VisitorStateClass>
	static WidgetVisitor::StateR stateConstructor( QWidget* widget)
	{
		return WidgetVisitor::StateR( new VisitorStateClass( widget));
	}
	template <class VisitorStateClass>
	void addClass( const char* name)
	{
		(*this)[ QString( name)] = &stateConstructor<VisitorStateClass>;
	}
	#define ADD_WidgetVisitorType( NAME ) { addClass<WidgetVisitorState_ ## NAME>( #NAME); }
	WidgetVisitorTypeMap()
	{
		ADD_WidgetVisitorType( QComboBox)
		ADD_WidgetVisitorType( QLabel)
		ADD_WidgetVisitorType( QCheckBox)
		ADD_WidgetVisitorType( QGroupBox)
		ADD_WidgetVisitorType( QButtonGroup)
		ADD_WidgetVisitorType( QRadioButton)
		ADD_WidgetVisitorType( QPushButton)
		ADD_WidgetVisitorType( QSpinBox)
		ADD_WidgetVisitorType( QDoubleSpinBox)
		ADD_WidgetVisitorType( QListWidget)
		ADD_WidgetVisitorType( QTreeWidget)
		ADD_WidgetVisitorType( QLineEdit)
		ADD_WidgetVisitorType( QTextEdit)
		ADD_WidgetVisitorType( QPlainTextEdit)
		ADD_WidgetVisitorType( QDateEdit)
		ADD_WidgetVisitorType( QTimeEdit)
		ADD_WidgetVisitorType( QDateTimeEdit)
		ADD_WidgetVisitorType( QTableWidget)
		ADD_WidgetVisitorType( QSlider)
		ADD_WidgetVisitorType( FileChooser)
		ADD_WidgetVisitorType( PictureChooser)
	}
};

WidgetVisitor::StateR createWidgetVisitorState( QWidget* widget)
{
	static WidgetVisitorTypeMap widgetVisitorTypeMap;
	QHash<QString,StateConstructor>::const_iterator wi = widgetVisitorTypeMap.find( widget->metaObject()->className());
	if (wi == widgetVisitorTypeMap.end())
	{
		return WidgetVisitor::StateR( new WidgetVisitorState_default( widget));
	}
	else
	{
		return wi.value()( widget);
	}
}


