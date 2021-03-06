/***************************************************************************
    copyright            : (C) 2002 - 2008 by Scott Wheeler
    email                : wheeler@kde.org
 ***************************************************************************/

/***************************************************************************
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License version   *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 *                                                                         *
 *   Alternatively, this file is available under the Mozilla Public        *
 *   License Version 1.1.  You may obtain a copy of the License at         *
 *   http://www.mozilla.org/MPL/                                           *
 ***************************************************************************/

#ifndef TAGLIB_H
#define TAGLIB_H

#include "config.h"

#define TAGLIB_MAJOR_VERSION 1
#define TAGLIB_MINOR_VERSION 8
#define TAGLIB_PATCH_VERSION 0

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 1))
#define TAGLIB_IGNORE_MISSING_DESTRUCTOR _Pragma("GCC diagnostic ignored \"-Wnon-virtual-dtor\"")
#else
#define TAGLIB_IGNORE_MISSING_DESTRUCTOR
#endif

#if (defined(_MSC_VER) && _MSC_VER >= 1600)
#define TAGLIB_CONSTRUCT_BITSET(x) static_cast<unsigned long long>(x)
#else
#define TAGLIB_CONSTRUCT_BITSET(x) static_cast<unsigned long>(x)
#endif

#include <string>
#include <climits>

// Check the widths of integral types.

#if SIZEOF_SHORT != 2
# error TagLib requires that short is 16-bit wide.
#endif

#if SIZEOF_INT != 4
# error TagLib requires that int is 32-bit wide.
#endif

#if SIZEOF_LONGLONG != 8
# error TagLib requires that long long is 64-bit wide.
#endif

#if SIZEOF_WCHAR_T < 2
# error TagLib requires that wchar_t is sufficient to store a UTF-16 char.
#endif

// Atomic increment/decrement operations

#if defined(HAVE_STD_ATOMIC)
# include <atomic>
# define TAGLIB_ATOMIC_INT std::atomic<unsigned int>
# define TAGLIB_ATOMIC_INC(x) x.fetch_add(1)
# define TAGLIB_ATOMIC_DEC(x) (x.fetch_sub(1) - 1)
#elif defined(HAVE_BOOST_ATOMIC)
# include <boost/atomic.hpp>
# define TAGLIB_ATOMIC_INT boost::atomic<unsigned int>
# define TAGLIB_ATOMIC_INC(x) x.fetch_add(1)
# define TAGLIB_ATOMIC_DEC(x) (x.fetch_sub(1) - 1)
#elif defined(HAVE_GCC_ATOMIC)
# define TAGLIB_ATOMIC_INT int
# define TAGLIB_ATOMIC_INC(x) __sync_add_and_fetch(&x, 1)
# define TAGLIB_ATOMIC_DEC(x) __sync_sub_and_fetch(&x, 1)
#elif defined(HAVE_WIN_ATOMIC)
# if !defined(NOMINMAX)
#   define NOMINMAX
# endif
# include <windows.h>
# define TAGLIB_ATOMIC_INT long
# define TAGLIB_ATOMIC_INC(x) InterlockedIncrement(&x)
# define TAGLIB_ATOMIC_DEC(x) InterlockedDecrement(&x)
#elif defined(HAVE_MAC_ATOMIC)
# include <libkern/OSAtomic.h>
# define TAGLIB_ATOMIC_INT int32_t
# define TAGLIB_ATOMIC_INC(x) OSAtomicIncrement32Barrier(&x)
# define TAGLIB_ATOMIC_DEC(x) OSAtomicDecrement32Barrier(&x)
#elif defined(HAVE_IA64_ATOMIC)
# include <ia64intrin.h>
# define TAGLIB_ATOMIC_INT int
# define TAGLIB_ATOMIC_INC(x) __sync_add_and_fetch(&x, 1)
# define TAGLIB_ATOMIC_DEC(x) __sync_sub_and_fetch(&x, 1)
#else
# define TAGLIB_ATOMIC_INT int
# define TAGLIB_ATOMIC_INC(x) (++x)
# define TAGLIB_ATOMIC_DEC(x) (--x)
#endif

// Optimized byte swap functions.

#if defined(HAVE_MSC_BYTESWAP)
# include <stdlib.h>
#elif defined(HAVE_GLIBC_BYTESWAP)
# include <byteswap.h>
#elif defined(HAVE_MAC_BYTESWAP)
# include <libkern/OSByteOrder.h>
#elif defined(HAVE_OPENBSD_BYTESWAP)
# include <sys/endian.h>
#endif

#if defined(HAVE_GCC_BYTESWAP_16)
# define TAGLIB_BYTESWAP_16(x) __builtin_bswap16(x)
#elif defined(HAVE_MSC_BYTESWAP)
# define TAGLIB_BYTESWAP_16(x) _byteswap_ushort(x)
#elif defined(HAVE_GLIBC_BYTESWAP)
# define TAGLIB_BYTESWAP_16(x) __bswap_16(x)
#elif defined(HAVE_MAC_BYTESWAP)
# define TAGLIB_BYTESWAP_16(x) OSSwapInt16(x)
#elif defined(HAVE_OPENBSD_BYTESWAP)
# define TAGLIB_BYTESWAP_16(x) swap16(x)
#endif

#if defined(HAVE_GCC_BYTESWAP_32)
# define TAGLIB_BYTESWAP_32(x) __builtin_bswap32(x)
#elif defined(HAVE_MSC_BYTESWAP)
# define TAGLIB_BYTESWAP_32(x) _byteswap_ulong(x)
#elif defined(HAVE_GLIBC_BYTESWAP)
# define TAGLIB_BYTESWAP_32(x) __bswap_32(x)
#elif defined(HAVE_MAC_BYTESWAP)
# define TAGLIB_BYTESWAP_32(x) OSSwapInt32(x)
#elif defined(HAVE_OPENBSD_BYTESWAP)
# define TAGLIB_BYTESWAP_32(x) swap32(x)
#endif

#if defined(HAVE_GCC_BYTESWAP_64)
# define TAGLIB_BYTESWAP_64(x) __builtin_bswap64(x)
#elif defined(HAVE_MSC_BYTESWAP)
# define TAGLIB_BYTESWAP_64(x) _byteswap_uint64(x)
#elif defined(HAVE_GLIBC_BYTESWAP)
# define TAGLIB_BYTESWAP_64(x) __bswap_64(x)
#elif defined(HAVE_MAC_BYTESWAP)
# define TAGLIB_BYTESWAP_64(x) OSSwapInt64(x)
#elif defined(HAVE_OPENBSD_BYTESWAP)
# define TAGLIB_BYTESWAP_64(x) swap64(x)
#endif

//! A namespace for all TagLib related classes and functions

/*!
 * This namespace contains everything in TagLib.  For projects working with
 * TagLib extensively it may be convenient to add a
 * \code
 * using namespace TagLib;
 * \endcode
 */

namespace TagLib {

  class String;

  typedef wchar_t            wchar;   // Assumed to be sufficient to store a UTF-16 char.
  typedef unsigned char      uchar;
  typedef unsigned short     ushort;
  typedef unsigned int       uint;
  typedef unsigned long long ulonglong;

  // long/ulong can be either 32-bit or 64-bit wide.
  typedef unsigned long  ulong;

  /*!
   * Unfortunately std::wstring isn't defined on some systems, (i.e. GCC < 3)
   * so I'm providing something here that should be constant.
   */
#ifdef HAVE_STD_WSTRING
  typedef std::wstring wstring;
#else
  typedef std::basic_string<wchar> wstring;
#endif

#ifndef DO_NOT_DOCUMENT // Tell Doxygen to skip this class.
  /*!
   * \internal
   * This is just used as a base class for shared classes in TagLib.
   *
   * \warning This <b>is not</b> part of the TagLib public API!
   */

  class RefCounter
  {
  public:
    RefCounter() : refCount(1) {}

    void ref() { TAGLIB_ATOMIC_INC(refCount); }
    bool deref() { return (TAGLIB_ATOMIC_DEC(refCount) == 0); }
    int count() { return refCount; }

  private:
    volatile TAGLIB_ATOMIC_INT refCount;
  };

#endif // DO_NOT_DOCUMENT

}

/*!
 * \mainpage TagLib
 *
 * \section intro Introduction
 *
 * TagLib is a library for reading and editing audio meta data, commonly know as \e tags.
 *
 * Features:
 * - A clean, high level, C++ API to handling audio meta data.
 * - Format specific APIs for advanced API users.
 * - ID3v1, ID3v2, APE, FLAC, Xiph, iTunes-style MP4 and WMA tag formats.
 * - MP3, MPC, FLAC, MP4, ASF, AIFF, WAV, TrueAudio, WavPack, Ogg FLAC, Ogg Vorbis, Speex and Opus file formats.
 * - Basic audio file properties such as length, sample rate, etc.
 * - Long term binary and source compatibility.
 * - Extensible design, notably the ability to add other formats or extend current formats as a library user.
 * - Full support for unicode and internationalized tags.
 * - Dual <a href="http://www.mozilla.org/MPL/MPL-1.1.html">MPL</a> and
 *   <a href="http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html">LGPL</a> licenses.
 * - No external toolkit dependancies.
 *
 * \section why Why TagLib?
 *
 * TagLib originally was written to provide an updated and improved ID3v2 implementation in C++ for use
 * in a variety of Open Source projects.  Since development began in 2002 and the 1.0 release in 2004
 * it has expanded to cover a wide variety of tag and file formats and is used in a wide variety of
 * Open Source and proprietary applications.  It now supports a variety of UNIXes, including Apple's OS
 * X, as well as Microsoft Windows.
 *
 * \section commercial Usage in Commercial Applications
 *
 * TagLib's licenses \e do allow usage within propriety (\e closed) applications, however TagLib is \e not
 * public domain.  Please note the requirements of the LGPL or MPL, and adhere to at least one of them.
 * In simple terms, you must at a minimum note your usage of TagLib, note the licensing terms of TagLib and
 * if you make changes to TagLib publish them.  Please review the licenses above before using TagLib in your
 * software.  Note that you may choose either the MPL or the LGPL, you do not have to fulfill the
 * requirements of both.
 *
 * \section installing Installing TagLib
 *
 * Please see the <a href="http://developer.kde.org/~wheeler/taglib.html">TagLib website</a> for the latest
 * downloads.
 *
 * TagLib can be built using the CMake build system. TagLib installs a taglib-config and pkg-config file to
 * make it easier to integrate into various build systems.  Note that TagLib's include install directory \e must
 * be included in the header include path. Simply adding <taglib/tag.h> will \e not work.
 *
 * \section start Getting Started
 *
 * TagLib provides both simple, abstract APIs which make it possible to ignore the differences between tagging
 * formats and format specific APIs which allow programmers to work with the features of specific tagging
 * schemes.  There is a similar abstraction mechanism for AudioProperties.
 *
 * The best place to start is with the <b>Class Hierarchy</b> linked at the top of the page.  The File and
 * AudioProperties classes and their subclasses are the core of TagLib.  The FileRef class is also a convenient
 * way for using a value-based handle.
 *
 * \note When working with FileRef please consider that it has only the most basic (extension-based) file
 * type resolution.  Please see its documentation on how to plug in more advanced file type resolution.  (Such
 * resolution may be part of later TagLib releases by default.)
 *
 * Here's a very simple example with TagLib:
 *
 * \code
 *
 * TagLib::FileRef f("Latex Solar Beef.mp3");
 * TagLib::String artist = f.tag()->artist(); // artist == "Frank Zappa"
 *
 * f.tag()->setAlbum("Fillmore East");
 * f.save();
 *
 * TagLib::FileRef g("Free City Rhymes.ogg");
 * TagLib::String album = g.tag()->album(); // album == "NYC Ghosts & Flowers"
 *
 * g.tag()->setTrack(1);
 * g.save();
 *
 * \endcode
 *
 * More examples can be found in the \e examples directory of the source distribution.
 *
 * \section Contact
 *
 * Questions about TagLib should be directed to the TagLib mailing list, not directly to the author.
 *
 *  - <a href="http://developer.kde.org/~wheeler/taglib/">TagLib Homepage</a>
 *  - <a href="https://mail.kde.org/mailman/listinfo/taglib-devel">TagLib Mailing List (taglib-devel@kde.org)</a>
 *
 * \author Scott Wheeler <wheeler@kde.org> et al.
 *
 */

#endif
