#include <cmath>
#include <fstream>
#include <sstream>

#include "Datum.h"

std::istream& operator>>(std::istream& in, Datum& datum) {
    std::string hold = "";
    std::getline(in,hold);
    std::stringstream ss(hold);
    std::string date = "";
    std::getline(ss,date,',');
    std::string neg = "";
    std::getline(ss,neg,',');
    std::string pos = "";
    std::getline(ss,pos,',');
    std::string tot = "";
    std::getline(ss,tot,',');
    std::string percent = "";
    std::getline(ss,percent,',');
    datum.week = date;
    datum.negative = std::stoi(neg);
    datum.positive = std::stoi(pos);
    datum.total = std::stoi(tot);
    datum.positivity = std::stof(percent);
    return in;
}

[[nodiscard]] Vector<Datum> badDataEntries(const Vector<Datum>& data) noexcept {
    Vector<Datum> bad_data;
    for (int i = 0; i < data.size(); i++) {
        if ((data[i].positivity + 0.1 < data[i].compute_positivity()*100) || (data[i].positivity - 0.1 > data[i].compute_positivity()*100) || (data[i].compute_total() != data[i].total)) {
            bad_data.push_back(data[i]);
        }
    }
    return bad_data;
}

[[nodiscard]] bool goodData(const Vector<Datum>& data) noexcept {
    if (badDataEntries(data).empty()) {
        return true;
    }
    return false;
}

[[nodiscard]] Vector<Datum> readData(std::istream& file) {
    std::string hold = "";
    std::getline(file,hold);
    Datum placeholder;
    Vector<Datum> vec;
    std::stringstream ss = std::stringstream(hold);
    while (getline(file,hold)) {
        std::stringstream ss(hold);
        std::string date = "";
        std::getline(ss,date,',');
        std::string neg = "";
        std::getline(ss,neg,',');
        std::string pos = "";
        std::getline(ss,pos,',');
        std::string tot = "";
        std::getline(ss,tot,',');
        std::string percent = "";
        std::getline(ss,percent,',');
        placeholder.week = date;
        placeholder.negative = std::stoi(neg);
        placeholder.positive = std::stoi(pos);
        placeholder.total = std::stoi(tot);
        placeholder.positivity = std::stof(percent);
        vec.push_back(placeholder);
    }
    return vec;
}

[[nodiscard]] bool isBadDataEntry(const Datum& datum);
