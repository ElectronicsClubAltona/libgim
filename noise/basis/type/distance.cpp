#include "./distance.hpp"
#include "../../../extent.hpp"
#include "../../../types/casts.hpp"


///////////////////////////////////////////////////////////////////////////////
template <size_t S, size_t R>
static const std::array<
    util::vectori<S>,
    util::pow(R*2+1,S)
>
generate (void)
{
    using value_type = typename util::vectori<S>::value_type;

    static const util::extent_range<
        S,typename util::vectori<S>::value_type
    > area (util::extent<S,value_type> {R*2+1});

    std::array<
        util::vectori<S>,
        util::pow(R*2+1,S)
    > out;

    std::transform (area.begin (), area.end (),
                    out.begin (),
                    [] (auto i) {
        return i.template as<util::vector> () - sign_cast<value_type> (R);
    });

    return out;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, size_t R>
const std::array<
    util::vectori<S>,
    util::pow(R*2+1,S)
>
util::noise::basis::type::distance<S,R>::OFFSETS = generate<S,R> ();


//-----------------------------------------------------------------------------
template struct util::noise::basis::type::distance<1,1>;
template struct util::noise::basis::type::distance<2,1>;
template struct util::noise::basis::type::distance<3,1>;

template struct util::noise::basis::type::distance<1,2>;
template struct util::noise::basis::type::distance<2,2>;
template struct util::noise::basis::type::distance<3,2>;
