#pragma once

#include <array>
#include <stdexcept>

template <int T>
class InputQueue
{
  public:
    InputQueue ();

    // Element access
    unsigned long at (int pos);
    unsigned long operator[] (int pos);
    int           size ();

    // Modifiers
    void push (unsigned long value);    // will pop last element on addition
    void clear ();

    // Underlying implementation
    int                          current_front_index;    // the index that is treated as the front
    std::array<unsigned long, T> data;                   // underlying data
};

template <int T>
inline InputQueue<T>::InputQueue ()
{
    current_front_index = 0;

    for (auto & val : data) val = 0;
}

template <int T>
inline unsigned long InputQueue<T>::at (int pos)
{
    if (pos > data.size ()) throw std::out_of_range ("InputQueue, out of range index");
    return data.at ((current_front_index + pos) % data.size ());
}

template <int T>
inline unsigned long InputQueue<T>::operator[] (int pos)
{
    if (pos > data.size ()) throw std::out_of_range ("InputQueue, out of range index");
    return data[(current_front_index + pos) % data.size ()];
}

template <int T>
inline int InputQueue<T>::size ()
{
    return data.size ();
}

template <int T>
inline void InputQueue<T>::push (unsigned long value)
{
    data.at ((current_front_index - 1) % data.size ()) = value;
    current_front_index -= 1;
    if (current_front_index < 0) current_front_index = data.size () - 1;
}

template <int T>
inline void InputQueue<T>::clear ()
{
    for (int i = 0; i < data.size (); i++) data.at ((current_front_index + i) % data.size ()) = 0;

    //data.at ((current_front_index) % data.size ()) &= 0b1111;
}
