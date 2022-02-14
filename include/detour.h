#pragma once

bool detour(BYTE* src, BYTE* dst, UInt32 len);
BYTE* detourWithTrampoline(BYTE* src, BYTE* dst, UInt32 len);