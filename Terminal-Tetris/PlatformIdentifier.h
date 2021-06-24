/** @file */
#pragma once

#define UNKNOWN_PLATFORM 0
#define WINDOWS_PLATFORM 1
#define LINUX_PLATFORM 2

#if defined(_WIN32)
#define PLATFORM WINDOWS_PLATFORM

#elif defined(__linux__)
#define PLATFORM LINUX_PLATFORM

#else
#define PLATFORM UNKNOWN_PLATFORM
#endif

// Is used to distinguish between platforms