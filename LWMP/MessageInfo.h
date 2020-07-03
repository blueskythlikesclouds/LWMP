#pragma once

enum class Variance
{
    INVARIANT,
    UNSIGNED_VARIANT,
    SIGNED_VARIANT
};

template<typename T>
class TVariance
{
public:
    static constexpr Variance VALUE = 
        std::is_same<T, VarUInt>::value ? Variance::UNSIGNED_VARIANT :
        std::is_same<T, VarInt>::value ? Variance::SIGNED_VARIANT :
        Variance::INVARIANT;
};

struct FieldInfo
{
    const size_t byteOffset;
    const size_t byteSize;
    const Variance variance;
};

struct MessageInfo
{
    const std::string_view name;
    const uint32_t id;
    const FieldInfo* const fields;
    const size_t fieldCount;
    const size_t byteSize;
};

#define DEFINE_MESSAGE_INFO() \
    static constexpr uint32_t ID = __COUNTER__; \
    static const MessageInfo INFO;

#define DECLARE_FIELD_INFO(r, data, elem) \
    { offsetof(data, elem), sizeof(data::elem), TVariance<decltype(data::elem)>::VALUE },

#define DECLARE_MESSAGE_INFO(type, ...) \
    static_assert(std::is_base_of<Message, type>::value, "Type does not inherit Message"); \
    \
    const FieldInfo fieldsOf##type[BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)] = \
    { \
        BOOST_PP_SEQ_FOR_EACH(DECLARE_FIELD_INFO, type, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
    }; \
    const MessageInfo type::INFO = \
    { \
        #type, \
        type::ID, \
        (const FieldInfo*)&fieldsOf##type, \
        BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
        sizeof(type) \
    }; \
    bool add##type = MessageInfoRegistry::add(&type::INFO); // 1000 IQ amirite?
