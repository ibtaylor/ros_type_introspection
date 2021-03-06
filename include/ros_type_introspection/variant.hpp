#ifndef VARIANT_H
#define VARIANT_H

#include <type_traits>
#include <limits>
#include <ros_type_introspection/builtin_types.hpp>
#include <ros_type_introspection/details/exceptions.hpp>
#include <ros_type_introspection/details/conversion_impl.hpp>


namespace RosIntrospection
{

class VarNumber
{

public:

    VarNumber() {
        _raw_data[8] = OTHER;
    }

    template<typename T> VarNumber(T value);

    BuiltinType getTypeID() const;

    template<typename T> T convert( ) const;

    template<typename T> T extract( ) const;

    template <typename T> void assign(const T& value);

    explicit operator double() const { return convert<double>(); }

private:
    uint8_t _raw_data[9];

};

template <typename T> inline
bool operator ==(const VarNumber& var, const T& num)
{
    return var.convert<T>() == num;
}

template <typename T> inline
bool operator ==(const T& num, const VarNumber& var)
{
    return var.convert<T>() == num;
}

//----------------------- Implementation ----------------------------------------------


template<typename T>
inline VarNumber::VarNumber(T value)
{
    static_assert (std::numeric_limits<T>::is_specialized, "not a number");
    assign(value);
}

inline BuiltinType VarNumber::getTypeID() const {
    return static_cast<BuiltinType>(_raw_data[8]);
}

template<typename T>
inline T VarNumber::extract( ) const
{
    static_assert (std::numeric_limits<T>::is_specialized, "not a number");

    if( getTypeID() != RosIntrospection::getType<T>() )
    {
        throw TypeException("VarNumber::extract -> wrong type");
    }
    return * reinterpret_cast<const T*>( _raw_data );
}

template <typename T>
inline void VarNumber::assign(const T& value)
{
    static_assert (std::numeric_limits<T>::is_specialized, "not a number");
    *reinterpret_cast<T *>( _raw_data ) =  value;
    _raw_data[8] = RosIntrospection::getType<T>() ;
}

template<typename DST>
inline DST VarNumber::convert() const
{
    using namespace RosIntrospection::details;
    DST target;

    //----------
    switch( _raw_data[8] )
    {
    case CHAR:
    case INT8:   convert_impl<int8_t,  DST>(*reinterpret_cast<const int8_t*>( _raw_data), target  ); break;

    case INT16:  convert_impl<int16_t, DST>(*reinterpret_cast<const int16_t*>( _raw_data), target  ); break;
    case INT32:  convert_impl<int32_t, DST>(*reinterpret_cast<const int32_t*>( _raw_data), target  ); break;
    case INT64:  convert_impl<int64_t, DST>(*reinterpret_cast<const int64_t*>( _raw_data), target  ); break;

    case BOOL:
    case BYTE:
    case UINT8:   convert_impl<uint8_t,  DST>(*reinterpret_cast<const uint8_t*>( _raw_data), target  ); break;

    case UINT16:  convert_impl<uint16_t, DST>(*reinterpret_cast<const uint16_t*>( _raw_data), target  ); break;
    case UINT32:  convert_impl<uint32_t, DST>(*reinterpret_cast<const uint32_t*>( _raw_data), target  ); break;
    case UINT64:  convert_impl<uint64_t, DST>(*reinterpret_cast<const uint64_t*>( _raw_data), target  ); break;

    case FLOAT32:  convert_impl<float, DST>(*reinterpret_cast<const float*>( _raw_data), target  ); break;
    case FLOAT64:  convert_impl<double, DST>(*reinterpret_cast<const double*>( _raw_data), target  ); break;

    default:  throw TypeException("VarNumber::extract -> cannot convert type" + std::to_string(_raw_data[8])); break;

    }
    return  target;
}

} //end namespace





#endif // VARIANT_H
