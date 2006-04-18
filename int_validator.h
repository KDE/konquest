#ifndef _INT_VALIDATOR_H_
#define _INT_VALIDATOR_H_

#include <qvalidator.h>


class IntValidator : public QValidator
{
    Q_OBJECT

public:
    IntValidator( QWidget *parent );
    IntValidator( int bottom, int top, QWidget *parent );

    virtual ~IntValidator();

    virtual QValidator::State validate( QString &, int & ) const;
    
    virtual void setRange( int bottom, int top );

    int bottom() const { return v_bottom; };
    int top() const { return v_top; };

private:
    int v_bottom, v_top;
};



#endif /* _INT_VALIDATOR_H_ */

