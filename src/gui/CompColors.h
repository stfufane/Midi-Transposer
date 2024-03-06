#ifndef MIDITRANSPOSER_COMPCOLORS_H
#define MIDITRANSPOSER_COMPCOLORS_H

#include <iostream>

#include "nlohmann/json.hpp"

namespace Gui
{

/**
 * @brief String representations of the implemented colors
 * that can be read from a json file.
 */
struct CompColors {
    std::string mSliderBackground;
    std::string mSliderThumb;
    std::string mSliderText;
    std::string mSliderTrack;
    std::string mSliderOutline;
    std::string mLabelBackground;
    std::string mLabelText;
    std::string mKeyPlayedBackground;
    std::string mKeyPlayedText;

    static std::string getFileName() { return "colors.json"; }
};

inline void from_json(const nlohmann::json& j, CompColors& colors)
{
    try {
        j.at("slider_background").get_to(colors.mSliderBackground);
        j.at("slider_thumb").get_to(colors.mSliderThumb);
        j.at("slider_text").get_to(colors.mSliderText);
        j.at("slider_track").get_to(colors.mSliderTrack);
        j.at("slider_outline").get_to(colors.mSliderOutline);
        j.at("label_background").get_to(colors.mLabelBackground);
        j.at("label_text").get_to(colors.mLabelText);
        j.at("key_played_background").get_to(colors.mKeyPlayedBackground);
        j.at("key_played_text").get_to(colors.mKeyPlayedText);
    } catch (std::exception& e) {
        std::cout << "One or several values were not defined in the json configuration file\n" << e.what() << "\n";
    }
}

}

#endif