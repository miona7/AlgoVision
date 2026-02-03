#ifndef APPCONSTANT_H
#define APPCONSTANT_H

namespace AppConstants {
    // window size
    constexpr int windowMinWidth  = 800;
    constexpr int windowMinHeight = 600;
    constexpr int windowWidth     = 1200;
    constexpr int windowHeight    = 900;

    // start page element's attributes
    constexpr int  btnSize        = 120;
    constexpr int  btnSpacing     = 24;
    constexpr auto startPageTitle = "AlgoVision";

    // menubar element's attributes
    constexpr int optionBtnSize  = 90;
    constexpr int optionIconSize = 64;

    // graph page element's attributes
    constexpr auto graphPageDefaultTitle = "Untilted";

    // node view
    constexpr double defaultRadius      = 30.0;
    constexpr double defaultBorderWidth = 3.0;

    inline double NodeScale = 1.0;

    constexpr double MinNodeScale = 0.4;
    constexpr double MaxNodeScale = 3.0;

    constexpr double EdgePadding   = 6.0;
    constexpr double BaseEdgeWidth = 2.0;

    static constexpr double BaseFontSize = 10.0;

    // dialogs
    constexpr int createGraphDialogMinWidth  = 400;
    constexpr int createGraphDialogMinHeight = 250;

    // legend
    constexpr int legendMinHeight = 250;
} // namespace AppConstants

#endif // APPCONSTANT
