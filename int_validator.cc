#include <limits.h>

#include "int_validator.h"
#include "int_validator.moc"

IntValidator::IntValidator( QWidget *parent, const char *name ) :
    QValidator( parent, name )
{
#ifdef INT_MIN
    v_bottom = INT_MIN;
#else
    v_bottom = ~INT_MAX;
#endif
    v_top = INT_MIN;
}

IntValidator::IntValidator( int bottom, int top, QWidget *parent, const char *name ) :
QValidator( parent, name )
{
    v_bottom = bottom;
    v_top = top;
}

IntValidator::~IntValidator() {}

QValidator::State
IntValidator::validate( QString &input, int & ) const
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


