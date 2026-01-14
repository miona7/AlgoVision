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
    constexpr double defaultRadius      = 30;
    constexpr double defaultBorderWidth = 3;

} // namespace AppConstants

#endif // APPCONSTANT
