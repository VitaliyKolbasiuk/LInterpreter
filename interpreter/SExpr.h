#pragma once

#include <iostream>

//---------------------------------------------------------------
//
// SExpr - symbolic expression
//
//---------------------------------------------------------------
//
//  Example of how implemented s-expression: ( a b )
//
//  !-------------!        !-------------!
//  ! SExpr: LIST !   +--->! SExpr: LIST !
//  !.............!   |    !.............!
//  ! CAR  ! CDR  !---+    ! CAR  ! CDR  !-->(nullptr)
//  !------!------!        !------!------!
//     |                      |
//     V                      V
//  !-----------------!    !-----------------!
//  ! SExpr: ATOM     !    ! SExpr: ATOM     !
//  !.................!    !.................!
//  ! value ! name: a !    ! value ! name: b !
//  !-------!---------!    !-------!---------!
//
//---------------------------------------------------------------
//

class ISExpr
{
public:
    enum Type {
        LIST=0,
        ATOM,
        INT,
        DOUBLE,
        STRING,
        WSTRING,
        ARRAY
    };

protected:
    ISExpr() = default;
    virtual ~ISExpr() = default;

    virtual Type type() const = 0;
    virtual ISExpr* print(  std::ostream& stream ) const = 0;
};

// List
class List : protected ISExpr
{
public:
    ISExpr* m_car = nullptr;
    List*   m_cdr = nullptr;

public:
    List() : m_car(nullptr), m_cdr(nullptr) {};
    List( ISExpr* car ) : m_car(car), m_cdr(nullptr) {};
    List( ISExpr* car, List* cdr ) : m_car(car), m_cdr(cdr) {};
    virtual ~List() {}

    Type type() const override { return LIST; }

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        stream << "( ";
        if ( m_car == nullptr )
        {
            stream << "nil";
        }
        else
        {
            m_cdr->print(stream);
        }
        
        for( List* it = m_cdr; it != nullptr; it=it->m_cdr )
        {
            stream << " ";
            it->print(stream);
        }

        stream << " )";
    }
};

// Atom
class Atom : protected ISExpr
{
    const char* m_name;
    ISExpr*     m_value = this;

public:
    Atom( const char* name ) : m_name(name), m_value(this) {};
    Atom( const char* name, ISExpr* value ) : m_name(name), m_value(value) {};
    virtual ~Atom() { delete [] m_name; }

    Type type() const override { return ATOM; }

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        stream << m_name;
    }

    const char* name() const { return m_name; }
    ISExpr*     value() const { return m_value; }
    void        setValue( ISExpr* newValue ) { m_value = newValue; }
};

struct SExpr
{
    enum Type {
        LIST=0,
        ATOM,
        INT,
        DOUBLE
    };

    Type    m_type = LIST;

    // CAR
    union {
        SExpr*      m_car;
        SExpr*      m_atomValue;
    };

    // CDR
    union {
        SExpr*      m_cdr;
        const char* m_atomName;
        int         m_intValue;
        double      m_doubleValue;
    };


    // NIL - "()"
    SExpr() : m_type(LIST), m_car(nullptr), m_cdr(nullptr)
    {}

    // "( carValue )"
    SExpr( SExpr* carValue ) : m_type(LIST), m_car(carValue), m_cdr(nullptr)
    {}

    SExpr( SExpr* carValue, SExpr* cdrValue ) : m_type(LIST), m_car(carValue), m_cdr(cdrValue)
    {}

    // atom
    SExpr( const char* atomName ) : m_type(ATOM), m_atomValue(this),  m_atomName(atomName) {
        auto len = std::strlen(atomName)+1;
        m_atomName = new char[ len ];
        std::memcpy( (char*)m_atomName, atomName, len );
    }

    ~SExpr() {
        if ( m_type == ATOM ) {
            if ( m_atomName == nullptr )
            {
                delete [] m_atomName;
            }
        }
    }

    bool isNil() const { return m_type==LIST && m_car == nullptr && m_cdr==nullptr; }

    void setValue( SExpr* value ) { m_car = value; }

    void print( const char* prefix = nullptr ) const
    {
        if ( prefix != nullptr )
        {
            std::cout << prefix;
        }
        
        switch (m_type)
        {
            case LIST:
            {
                if ( m_car == nullptr && m_cdr == nullptr )
                {
                    std::cout << "NIL " << std::flush;
                    break;
                }

                std::cout << "( " << std::flush;
                for( const auto* it = this; it != nullptr; it = it->m_cdr )
                {
                    if ( it->m_car == nullptr && m_cdr != nullptr )
                    {
                        std::cout << "NIL " << std::flush;
                    }
                    else
                    {
                        it->m_car->print();
                    }
                }
                std::cout << " )" << std::flush;
                break;
            }
            case ATOM:
            {
                std::cout << m_atomName << ' ' << std::flush;
                break;
            }
        }
    }
};
