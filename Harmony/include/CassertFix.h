// CassertFix.h - DEPRECATED
//
// This header is no longer used and is kept only for historical reference.
// 
// ORIGINAL PURPOSE:
// This header was intended to fix MSVC C++20 compatibility issues with nlohmann/json
// by force-including <cassert> and defining JSON_ASSERT before template instantiation.
//
// WHY IT WAS REPLACED:
// With MSVC's /Zc:preprocessor (conforming preprocessor) flag, the force-include
// mechanism (/FI) does not reliably propagate macros into template instantiations
// due to strict two-phase name lookup in C++20.
//
// CURRENT SOLUTION:
// JSON_ASSERT is now defined as a compile definition in CMakeLists.txt:
//   target_compile_definitions(Harmony PRIVATE "JSON_ASSERT(x)=((void)0)")
//
// This disables assertions within nlohmann/json, which is acceptable because:
// 1. These are internal library consistency checks, not application logic
// 2. The Harmony project has its own assertion system (HARMONY_ASSERT)
// 3. This is a standard workaround for this MSVC preprocessor issue
//
// This file can be safely removed in a future cleanup.

#pragma once

// Deprecated - do not use
// See comment above for explanation
