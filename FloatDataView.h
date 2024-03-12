//
// Created by brendan on 3/10/24.
//

#ifndef FLOATDATAVIEW_H
#define FLOATDATAVIEW_H

#include <cstddef>
class FloatDataView {
public:
    FloatDataView()
    : m_start(nullptr), m_size(0)
    {}

    FloatDataView(const float* start, size_t size) : m_start(start), m_size(size)
    {}

    [[nodiscard]] size_t size() const { return m_size; }

    const float& operator[](size_t index) const { return m_start[index]; }

    [[nodiscard]] const float* begin() const { return m_start; }
    [[nodiscard]] const float* end() const { return m_start + m_size; }

private:
    const float* m_start {};
    size_t m_size {};
};


#endif //FLOATDATAVIEW_H
