/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This file contains platform-specific typedefs and defines.

#ifndef WEBRTC_TYPEDEFS_H_
#define WEBRTC_TYPEDEFS_H_

// Reserved words definitions
// TODO(andrew): Look at removing these.
#define WEBRTC_EXTERN extern
#define G_CONST const
#define WEBRTC_INLINE extern __inline

// Define WebRTC preprocessor identifiers based on the current build platform.
// TODO(andrew): Clean these up. We can probably remove everything in this
// block.
//   - TARGET_MAC_INTEL and TARGET_MAC aren't used anywhere.
//   - In the few places where TARGET_PC is used, it should be replaced by
//     something more specific.
//   - Do we really support PowerPC? Probably not. Remove WEBRTC_MAC_INTEL
//     from build/common.gypi as well.
#if defined(WIN32)
    // Windows & Windows Mobile.
    #if !defined(WEBRTC_TARGET_PC)
        #define WEBRTC_TARGET_PC
    #endif
#elif defined(__APPLE__)
    // Mac OS X.
    #if defined(__LITTLE_ENDIAN__ )
        #if !defined(WEBRTC_TARGET_MAC_INTEL)
            #define WEBRTC_TARGET_MAC_INTEL
        #endif
    #else
        #if !defined(WEBRTC_TARGET_MAC)
            #define WEBRTC_TARGET_MAC
        #endif
    #endif
#else
    // Linux etc.
    #if !defined(WEBRTC_TARGET_PC)
        #define WEBRTC_TARGET_PC
    #endif
#endif

// Derived from Chromium's build/build_config.h
// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
// TODO(andrew): replace WEBRTC_LITTLE_ENDIAN with WEBRTC_ARCH_LITTLE_ENDIAN?
#if defined(_M_X64) || defined(__x86_64__)
#define WEBRTC_ARCH_X86_FAMILY
#define WEBRTC_ARCH_X86_64
#define WEBRTC_ARCH_64_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(_M_IX86) || defined(__i386__)
#define WEBRTC_ARCH_X86_FAMILY
#define WEBRTC_ARCH_X86
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__ARMEL__)
// TODO(andrew): We'd prefer to control platform defines here, but this is
// currently provided by the Android makefiles. Commented to avoid duplicate
// definition warnings.
//#define WEBRTC_ARCH_ARM
// TODO(andrew): Chromium uses the following two defines. Should we switch?
//#define WEBRTC_ARCH_ARM_FAMILY
//#define WEBRTC_ARCH_ARMEL
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__aarch64__)
#define WEBRTC_ARCH_64_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#elif defined(__mips__)
#define WEBRTC_ARCH_32_BITS
#define WEBRTC_ARCH_LITTLE_ENDIAN
#else
#error Please add support for your architecture in typedefs.h
#endif

#if defined(__SSE2__) || defined(_MSC_VER)
#define WEBRTC_USE_SSE2
#endif

#if defined(WEBRTC_TARGET_PC)

#if !defined(_MSC_VER)
  #include <stdint.h>
  #ifdef __cplusplus
    #define __STDC_CONSTANT_MACROS
    #ifdef _STDINT_H
      #undef _STDINT_H
    #endif
    # include <stdint.h>
  #endif
#else
    // Define C99 equivalent types.
    // Since MSVC doesn't include these headers, we have to write our own
    // version to provide a compatibility layer between MSVC and the WebRTC
    // headers.
    typedef signed char         int8_t;
    typedef signed short        int16_t;
    typedef signed int          int32_t;
    typedef signed long long    int64_t;
    typedef unsigned char       uint8_t;
    typedef unsigned short      uint16_t;
    typedef unsigned int        uint32_t;
    typedef unsigned long long  uint64_t;
#endif

#if defined(WIN32)
    typedef __int64             WebRtc_Word64;
    typedef unsigned __int64    WebRtc_UWord64;
#else
    typedef int64_t             WebRtc_Word64;
    typedef uint64_t            WebRtc_UWord64;
#endif
    typedef int32_t             WebRtc_Word32;
    typedef uint32_t            WebRtc_UWord32;
    typedef int16_t             WebRtc_Word16;
    typedef uint16_t            WebRtc_UWord16;
    typedef char                WebRtc_Word8;
    typedef uint8_t             WebRtc_UWord8;

    // Define endian for the platform
    #define WEBRTC_LITTLE_ENDIAN

#elif defined(WEBRTC_TARGET_MAC_INTEL)
    #include <stdint.h>

    typedef int64_t             WebRtc_Word64;
    typedef uint64_t            WebRtc_UWord64;
    typedef int32_t             WebRtc_Word32;
    typedef uint32_t            WebRtc_UWord32;
    typedef int16_t             WebRtc_Word16;
    typedef char                WebRtc_Word8;
    typedef uint16_t            WebRtc_UWord16;
    typedef uint8_t             WebRtc_UWord8;

    // Define endian for the platform
    #define WEBRTC_LITTLE_ENDIAN

#else
    #error "No platform defined for WebRTC type definitions (typedefs.h)"
#endif

// Borrowed from Chromium's base/compiler_specific.h.
// Annotate a virtual method indicating it must be overriding a virtual
// method in the parent class.
// Use like:
//   virtual void foo() OVERRIDE;
#if defined(_MSC_VER)
#define OVERRIDE override
#elif defined(__clang__)
// Clang defaults to C++03 and warns about using override. Squelch that.
// Intentionally no push/pop here so all users of OVERRIDE ignore the warning
// too. This is like passing -Wno-c++11-extensions, except that GCC won't die
// (because it won't see this pragma).
#pragma clang diagnostic ignored "-Wc++11-extensions"
#define OVERRIDE override
#elif defined(__GNUC__) && __cplusplus >= 201103 && \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) >= 40700
// GCC 4.7 supports explicit virtual overrides when C++11 support is enabled.
#define OVERRIDE override
#else
#define OVERRIDE
#endif

// Annotate a function indicating the caller must examine the return value.
// Use like:
//   int foo() WARN_UNUSED_RESULT;
// TODO(ajm): Hack to avoid multiple definitions until the base/ of webrtc and
// libjingle are merged.
#if !defined(WARN_UNUSED_RESULT)
#if defined(__GNUC__)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif
#endif  // WARN_UNUSED_RESULT

// Put after a variable that might not be used, to prevent compiler warnings:
//   int result UNUSED = DoSomething();
//   assert(result == 17);
#ifndef UNUSED
#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif
#endif

// Annotate a function that will not return control flow to the caller.
#if defined(_MSC_VER)
#define NO_RETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define NO_RETURN __attribute__((noreturn))
#else
#define NO_RETURN
#endif

#endif  // WEBRTC_TYPEDEFS_H_
