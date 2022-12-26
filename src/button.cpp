#include "button.hpp"

#include "text_label.hpp"

TextLabel Button::textLabel() const {
    auto label = TextLabel{};
    label.text = text;
    label.color = WHITE;
    label.x = rectangle.x + 4;
    label.y = rectangle.y + 4;
    return label;
}

TextLabel Button::textLabelSelected() const {
    auto label = TextLabel{};
    label.text = text;
    label.color = WHITE;
    label.x = rectangle.x + 4;
    label.y = rectangle.y + 5;
    return label;
}
