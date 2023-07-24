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

class List;
class Atom;
class BuiltinFunc;
class IntNumber;
class Double;


//----------
// ISExpr
//----------
class ISExpr
{
public:
    enum Type {
        LIST=0,
        BUILT_IN_FUNC,
        ATOM,
        DOUBLE,
        INT_NUMBER,
        STRING,
        WSTRING,
        ARRAY,
    };

protected:
    ISExpr() = default;
    virtual ~ISExpr() = default;

public:
    virtual Type type() const = 0;
    virtual ISExpr* print(  std::ostream& stream ) const = 0;
    virtual ISExpr* eval() = 0;

    ISExpr* print(  const char* prefix ) const { std::cout << prefix; print(std::cout); };

    List*   toList() {
        if ( type() != LIST )
        {
            __asm { int 3 }
            return nullptr;
        }
        return (List*) this;
    }
    
    Atom*   toAtom() {
        if ( type() != ATOM )
        {
            __asm { int 3 }
            return nullptr;
        }
        return (Atom*) this;
    }
    
    BuiltinFunc*  toBuiltinFunc() {
        if ( type() != BUILT_IN_FUNC )
        {
            return nullptr;
        }
        return (BuiltinFunc*) this;
    }
    
    IntNumber*  toIntNumber() {
        if ( type() != INT_NUMBER )
        {
            __asm { int 3 }
            return nullptr;
        }
        return (IntNumber*) this;
    }
    
    Double*  toDouble() {
        if ( type() != DOUBLE )
        {
            __asm { int 3 }
            return nullptr;
        }
        return (Double*) this;
    }
    
protected:
    const char* copyString( const char* name )
    {
        auto len = std::strlen(name)+1;
        char* string = new char[len];
        std::memcpy( string, name, len );
        return string;
    }
    

};

//------------------------
// List
//------------------------
class List : public ISExpr
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
    
    virtual ISExpr* eval() override { return this; } //TODO

    bool isEmptyList() { return m_car == nullptr && m_cdr == nullptr;}

    ISExpr* print(  const char* prefix ) const { std::cout << prefix; print(std::cout); };

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        stream << "( ";
        if ( m_car == nullptr )
        {
            stream << "nil";
        }
        else
        {
            m_car->print(stream);
        }
        
        for( List* it = m_cdr; it != nullptr; it=it->m_cdr )
        {
            stream << " ";
            it->m_car->print(stream);
        }

        stream << " )";
    }
};

//------------------------
// Atom
//------------------------
class Atom : public ISExpr
{
    const char* m_name;
    ISExpr*     m_value = this;

public:
    Atom( const char* name ) : m_name( copyString(name) ), m_value(this) {};
    Atom( const char* name, ISExpr* value ) : m_name( copyString(name) ), m_value(value) {};
    virtual ~Atom() { delete [] m_name; }

    Type type() const override { return ATOM; }

    virtual ISExpr* eval() override { return m_value; }

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        stream << m_name;
    }

    const char* name() const { return m_name; }
    ISExpr*     value() const { return m_value; }
    void        setValue( ISExpr* newValue ) { m_value = newValue; }
};

//------------------------
// BuiltinFunc
//------------------------
using BuiltInLambda = std::function< ISExpr* (List*) >;

class BuiltinFunc : public ISExpr
{
    const char* m_name;
    BuiltInLambda m_lambdaFunc;
    
public:
    BuiltinFunc( const char* name, BuiltInLambda lambdaFunc ) : m_name( copyString(name) ), m_lambdaFunc(lambdaFunc) {};
    virtual ~BuiltinFunc() { delete [] m_name; }

    Type type() const override { return BUILT_IN_FUNC; }

    virtual ISExpr* eval() override { return this; }

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        stream << m_name;
    }

    const char* name() const { return m_name; }
    BuiltInLambda func() const { return m_lambdaFunc; }
};


//------------------------
// IntNumber
//------------------------

class IntNumber: public ISExpr
{
    int64_t m_value;
public:
    IntNumber( int64_t value ) : m_value(value) {}
    
    Type type() const override { return INT_NUMBER; }

    virtual ISExpr* eval() override { return this; }

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        stream << m_value;
    }

    int64_t& value() { return m_value; }
};

//------------------------
// DoubleNumber
//------------------------

class Double: public ISExpr
{
    double m_value;
public:
    Double( double value ) : m_value(value) {}
    
    Type type() const override { return DOUBLE; }

    virtual ISExpr* eval() override { return this; }

    ISExpr* print( std::ostream& stream = std::cout ) const override
    {
        std::string str = std::to_string(m_value);
        str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
        str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
        stream << str;
    }

    double& value() { return m_value; }
};
