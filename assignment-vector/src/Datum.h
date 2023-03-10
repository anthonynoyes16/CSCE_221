#ifndef DATUM_H
#define DATUM_H

#include <iostream>
#include <string>

#include "Vector.h"

struct Datum {
    std::string week;
    unsigned int negative;
    unsigned int positive;
    unsigned int total;
    float positivity;

    [[nodiscard]] unsigned int compute_total() const noexcept {
        return positive + negative;
    }
    [[nodiscard]] float compute_positivity() const noexcept {
        float tot = float(compute_total());
        return positive / tot;
    }
};

std::ostream& operator<<(std::ostream& out, const Datum& datum);

std::istream& operator>>(std::istream& in, Datum& datum);

[[nodiscard]] Vector<Datum> readData(std::istream& file);

[[nodiscard]] Vector<Datum> badDataEntries(const Vector<Datum>& data) noexcept;

[[nodiscard]] bool goodData(const Vector<Datum>& data) noexcept;

#endif
