#pragma once

#include "common/Error.h"
#include "common/MemUtils.h"
#include "core/Color.h"
#include "renderer/Screen.h"

#include "SDL.h"

#include <array>
#include <cstdint>
#include <expected>
#include <memory>

namespace cg {
class SdlScreen {
public:
    class Painter {
    public:
        Painter(int width, int height, int rowSize, void* pixelData)
            : width_(width), height_(height), rowSize_(rowSize), pixelData_(pixelData) {}
        void paint(int row, int col, const Color& color) {
            uint32_t int32Color = colorToIntArgb(color).asUint32();

            uint32_t* destination = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(pixelData_) +
                                                                (height_ - static_cast<int>(row) - 1) * rowSize_ +
                                                                static_cast<int>(col) * bytesPerPixel);

            *destination = int32Color;
        }

        int width() { return width_; }
        int height() { return height_; }

    private:
        int width_;
        int height_;
        int rowSize_;
        void* pixelData_;
    };

    static_assert(PixelPainter<Painter>, "SdlScreen::Painter does not fulfill the PixelPainter concept.");

    SdlScreen(SdlScreen&&) = default;
    SdlScreen& operator=(SdlScreen&&) = default;
    ~SdlScreen();

    static std::expected<SdlScreen, Error> create(int width, int height, SDL_Window* window);

    int width() const { return width_; }
    int height() const { return height_; }

    void clear(const Color& color = Color::black());
    Painter paintPixels() { return Painter(width_, height_, rowSize_, pixelData_); }
    void flush();

private:
    static constexpr int bytesPerPixel = 4;

    class IntArgb {
    public:
        IntArgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : data(a << 24 | r << 16 | g << 8 | b) {}
        uint8_t r() const { return data && 0x00FF0000 >> 16; }
        uint8_t g() const { return data && 0x0000FF00 >> 8; }
        uint8_t b() const { return data && 0x000000FF; }
        uint8_t a() const { return data && 0xFF000000 >> 24; }
        uint32_t asUint32() const { return data; }

    private:
        uint32_t data;
    };

    SdlScreen(int width, int height, c_unique_ptr<SDL_Renderer>&& renderer, c_unique_ptr<SDL_Texture>&& sdlTexture);

    static IntArgb colorToIntArgb(const Color& color) {
        Color clampedColor = color.clamp() * 255;

        return IntArgb(roundToUint8(clampedColor.r()), roundToUint8(clampedColor.g()), roundToUint8(clampedColor.b()),
                       255);
    }

    static uint8_t roundToUint8(float val) { return static_cast<uint8_t>(val + 0.5f); }

    int width_;
    int height_;
    c_unique_ptr<SDL_Renderer> renderer_;
    c_unique_ptr<SDL_Texture> texture_;
    int rowSize_;
    void* pixelData_;
};

static_assert(Screen<SdlScreen>, "SdlScreen does not fulfill the Screen concept.");
} // namespace cg
