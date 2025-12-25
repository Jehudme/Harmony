#pragma once

// ========================================================
// Exceptions Disabled - No Exception Support
// ========================================================
// This file is kept for compatibility but exceptions are disabled.
// Use assertions and error logging instead of exceptions.
// See Harmony/Assert.h and Harmony/Logger.h for error handling.
// ========================================================

// Deprecated - Do not use HARMONY_THROW
// Use HARMONY_ERROR(...) for error logging instead
#define HARMONY_THROW(...) \
    do { \
        HARMONY_ERROR("DEPRECATED: HARMONY_THROW called. Use HARMONY_ERROR instead. Message: " __VA_ARGS__); \
    } while(0)