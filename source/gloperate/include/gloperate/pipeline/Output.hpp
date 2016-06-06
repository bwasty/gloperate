
#pragma once


#include <gloperate/pipeline/Output.h>


namespace gloperate
{


template <typename T>
Output<T>::Output(const std::string & name, Stage * parent, const T & value)
: DataSlot<T>(value)
, m_valid(false)
{
    // Do not add property to object, yet. Just initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Register output, will also add output as a property
    this->initDataSlot(SlotType::Output, parent, cppexpose::PropertyOwnership::None);
}

template <typename T>
Output<T>::~Output()
{
}

template <typename T>
void Output<T>::invalidate()
{
    // Set state to invalid
    m_valid = false;

    // Promote changed-event
    this->DataSlot<T>::onValueChanged(this->m_value);
}

template <typename T>
bool Output<T>::isValid() const
{
    return m_valid;
}

template <typename T>
void Output<T>::onValueChanged(const T & value)
{
    // Set state to valid
    m_valid = true;

    // Promote changed-event
    DataSlot<T>::onValueChanged(value);
}


} // namespace gloperate
