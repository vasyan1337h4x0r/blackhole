#pragma once

#include <string>

#include <boost/mpl/transform.hpp>
#include <boost/variant/variant.hpp>

#include "blackhole/cpp17/string_view.hpp"

namespace blackhole {
namespace attribute {

struct owned_t;

struct value_t {
    typedef boost::variant<
        std::int64_t,
        double,
        string_view
    > type;

    /// Underlying type.
    type inner;

    value_t(int val): inner(static_cast<std::int64_t>(val)) {}
    value_t(double val): inner(val) {}
    value_t(string_view val): inner(val) {}
    value_t(const owned_t& val);

    auto operator==(const value_t& other) const -> bool {
        return inner == other.inner;
    }
};

template<typename T>
struct into_owned {
    typedef T type;
};

template<>
struct into_owned<string_view> {
    typedef std::string type;
};

struct owned_t {
    typedef boost::make_variant_over<
        boost::mpl::transform<
            value_t::type::types,
            into_owned<boost::mpl::_1>
        >::type
    >::type type;

    /// Underlying type.
    type inner;

    owned_t(int val): inner(static_cast<std::int64_t>(val)) {}
    owned_t(double val): inner(val) {}
    owned_t(std::string val): inner(std::move(val)) {}
};

}  // namespace attribute
}  // namespace blackhole
