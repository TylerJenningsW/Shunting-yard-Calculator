#pragma once
size_t truncate(size_t value)
{
    if (value < 0) return 0;
    if (value > 255) return 255;

    return value;
}