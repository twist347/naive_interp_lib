#pragma once

#define GENERATE_MOVE_AND_DELETE_COPY_SEMANTICS(class_name) \
    constexpr class_name(const class_name &) = delete; \
    constexpr class_name(class_name &&) noexcept = default; \
    constexpr auto operator=(const class_name &) -> class_name & = delete; \
    constexpr auto operator=(class_name &&) noexcept -> class_name & = default;