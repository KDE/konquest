#include <values.h>

#include "int_validator.h"
#include "int_validator.moc"

IntValidator::IntValidator( QWidget *parent, const char *name ) :
    QValidator( parent, name )
{
#ifdef MININT
    v_bottom = MININT;
#else
    v_bottom = ~MAXINT;
#endif
    v_top = MAXINT;
}

IntValidator::IntValidator( int bottom, int top, QWidget *parent, const char *name ) :
QValidator( parent, name )
{
    v_bottom = bottom;
    v_top = top;
}

IntValidator::~IntValidator() {}

QValidator::State
IntValidator::validate( QString &input, int & )
{
    if( input.isEmpty() ) {
        return QValidator::Valid;
    } else {
        bool ok;

        int value = input.toInt( &ok );

        if( !ok )
            return QValidator::Invalid;

        if( value < v_bottom || value > v_top )
            return QValidator::Valid;

        return QValidator::Acceptable;
    }
}

void
IntValidator::setRange( int b, int t )
{
    v_bottom = b;
    v_top = t;
}


