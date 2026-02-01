#pragma once

#include "pch.h"


struct BreakData {
	// time since epoch
    uint64_t start{};
    uint64_t end{};
    uint32_t duration_seconds{};

    BreakData() {}

    BreakData(uint64_t start, uint64_t end)
        : start(start), end(end) {
        duration_seconds = static_cast<uint32_t>(end - start);
    }
};
