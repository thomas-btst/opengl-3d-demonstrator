#ifndef RADIO_BUTTON_HPP
#define RADIO_BUTTON_HPP

#include <imgui-1.91.1/imgui.h>

#include <string>

struct Button {
    std::string name;
};

template <typename T>
class RadioButton {
    T value;
public:
    RadioButton(const T& initialValue)
     : value(initialValue)
    {}

    void addOption(const char* label, const T& optionValue) {
        if (ImGui::RadioButton(label, value == optionValue))
            value = optionValue;
    }
    inline const T& getSelectedValue() {return value;}
};

#endif