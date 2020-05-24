//
// Created by Lloyd Leung on 2020-05-18.
//

#include "Button.h"


// Date constructor
Button::Button(
        String _hexCode,
        String _buttonLabelText,
        String _columnClass

) {
    hexCode         = _hexCode;
    columnClass     = _columnClass;
    buttonLabelText = _buttonLabelText;
    buttonClass     = "";
}

Button::Button(
        String _hexCode,
        String _buttonLabelText,
        String _columnClass,
        String _buttonClass
) {
    hexCode         = _hexCode;
    columnClass     = _columnClass;
    buttonLabelText = _buttonLabelText;
    buttonClass     = _buttonClass;
}


// Date constructor
Button::Button() {
    isRowSpacer = true;
}

