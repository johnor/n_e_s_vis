#pragma once

#include <string>
#include <vector>

namespace nesvis {

class LogWidget {
public:
    LogWidget() = default;

    void draw();

    void add(const std::string &text);

private:
    std::vector<std::string> text_list_;
};

} // namespace nesvis
