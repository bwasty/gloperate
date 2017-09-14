
#pragma once


#include <functional>

#include <cppexpose/typed/TypedEnum.h>


namespace gloperate_text
{


enum class LineAnchor : unsigned char
{
    Ascent, Center, Baseline, Descent
};


} // namespace gloperate_text


namespace std
{


template<>
struct hash<gloperate_text::LineAnchor>
{
    std::hash<unsigned char>::result_type operator() (
        const gloperate_text::LineAnchor & arg) const
    {
        std::hash<unsigned char> hasher;
        return hasher(static_cast<unsigned char>(arg));
    }
};


} // namespace std


namespace cppexpose
{


template<>
class EnumValues<gloperate_text::LineAnchor>
{
public:
    static std::map<gloperate_text::LineAnchor, std::string> namedValues()
    {
        return {
            { gloperate_text::LineAnchor::Ascent,   "Ascent" },
            { gloperate_text::LineAnchor::Center,   "Centered" },
            { gloperate_text::LineAnchor::Baseline, "Baseline" },
            { gloperate_text::LineAnchor::Descent,  "Descent" }
        };
    }
};


} // namespace cppexpose
