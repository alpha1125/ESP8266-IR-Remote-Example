//
// Created by Lloyd Leung on 2020-05-18.
//
#ifndef _BUTTONS_H
#define _BUTTONS_H
#include <Arduino.h>

class Button {
public:
    String  hexCode,
            columnClass,
            buttonLabelText,
            buttonClass; // should have a default if null.

    bool isRowSpacer = false;

    // Yes buttonClass
    Button(
            String _hexCode,
            String _buttonLabelText,
            String _columnClass,
            String _buttonClass
    );

    // No buttonClass
    Button(
            String _hexCode,
            String _buttonLabelText,
            String _columnClass
    );

    Button();

};

#endif //_BUTTONS_H
