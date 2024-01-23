#include "SdlScreen.h"

#include "glm/ext/vector_common.hpp"
#include "glm/vec3.hpp"

#include <cassert>
#include <cmath>

namespace cg {
SdlScreen::SdlScreen(int width, int height, c_unique_ptr<SDL_Renderer>&& renderer, c_unique_ptr<SDL_Texture>&& texture)
    : width_(width), height_(height), renderer_(std::move(renderer)), texture_(std::move(texture)) {
    [[maybe_unused]] int error = SDL_LockTexture(texture_.get(), nullptr, &pixelData_, &rowSize_);
    assert(!error && "SDL_LockTexture failed: invalid texture or texture not created for streaming");
}

SdlScreen::~SdlScreen() { SDL_UnlockTexture(texture_.get()); }

std::expected<SdlScreen, Error> SdlScreen::create(int width, int height, SDL_Window* window) {
    assert(width > 0 && height > 0);
    assert(window != nullptr);

    c_unique_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(window, -1, 0), &SDL_DestroyRenderer);
    if (renderer == nullptr) {
        return std::unexpected(Error(ErrorCode::OperationFailed, SDL_GetError()));
    }

    c_unique_ptr<SDL_Texture> texture(
        SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height),
        &SDL_DestroyTexture);
    if (texture == nullptr) {
        return std::unexpected(Error(ErrorCode::OperationFailed, SDL_GetError()));
    }

    return SdlScreen(width, height, std::move(renderer), std::move(texture));
}

void SdlScreen::clearScreen(const Color& color) {
    uint32_t int32Color = colorToIntArgb(color).asUint32();

    for (int row = 0; row < height_; ++row) {
        uint32_t* destination = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(pixelData_) + row * rowSize_);
        for (int column = 0; column < width_; ++column) {
            *destination = int32Color;
            ++destination;
        }
    }
}

void SdlScreen::flush() {
    SDL_UnlockTexture(texture_.get());
    [[maybe_unused]] int error = SDL_RenderCopy(renderer_.get(), texture_.get(), nullptr, nullptr);
    assert(!error && "SDL_RenderCopy failed");
    SDL_RenderPresent(renderer_.get());
    error = SDL_LockTexture(texture_.get(), nullptr, &pixelData_, &rowSize_);
    assert(!error && "SDL_LockTexture failed: invalid texture or texture not created for streaming");
}
} // namespace cg