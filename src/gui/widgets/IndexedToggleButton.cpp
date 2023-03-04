#include "IndexedToggleButton.h"

namespace Gui
{

void IndexedToggleButton::paintButton(juce::Graphics& g, bool, bool)
{
    // Calculations needed to draw the button centered in its container,
    // maximizing its bigger side.
    auto y_index = getToggleState() ? 1 : 0;
    auto bounds = getLocalBounds();

    int side, displayWidth, displayHeight, top_x, top_y;
    side = juce::jmin(bounds.getWidth(), bounds.getHeight());

    if (buttonWidth >= buttonHeight) {
        displayWidth = side;
        displayHeight = static_cast<int>(static_cast<float>(displayWidth) / ratio);
        top_x = centered ? (bounds.getWidth() - displayWidth) / 2 : 0;
        top_y = centered ? (bounds.getHeight() - displayHeight) / 2 : 0;
    } else {
        displayHeight = side;
        displayWidth = static_cast<int>(static_cast<float>(displayHeight) * ratio);
        top_x = centered ? (bounds.getWidth() - displayWidth) / 2 : 0;
        top_y = centered ? (bounds.getHeight() - displayHeight) / 2 : 0;
    }

    g.drawImage(*buttonsImage, top_x, top_y, displayWidth, displayHeight,
            buttonWidth * index, buttonHeight * y_index, buttonWidth, buttonHeight);
}

void IndexedToggleButton::setImage(juce::Image* image, int i, float nb_tiles, bool center)
{
    index = i;
    buttonsImage = image;
    buttonWidth = static_cast<int>(static_cast<float>(buttonsImage->getWidth()) / nb_tiles);
    buttonHeight = static_cast<int>(static_cast<float>(buttonsImage->getHeight()) / 2.0f);
    ratio = static_cast<float>(buttonWidth) / static_cast<float>(buttonHeight);
    centered = center;
}

} // Gui