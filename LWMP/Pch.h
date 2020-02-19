#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

// winsock2
#include <WinSock2.h>
#include <WS2tcpip.h>

// bitreadercpp
#include <algorithm>

#pragma warning(push)

#pragma warning(disable : 4244)
#pragma warning(disable : 4804)
#pragma warning(disable : 4805)

#include <bitreader/bitreader.hpp>
#include <bitreader/bitwriter.hpp>
#include <bitreader/data_source/memory_byte_source.hpp>

#pragma warning(pop)

// boost
#include <boost/preprocessor.hpp>

// stl
#include <cstdint>
#include <cstdio>

// nameof
#include <nameof/nameof.hpp>

// detours
#include <detours.h>

// LWMP
#include "Debug.h"
#include "Helpers.h"
#include "Types.h"
#include "Math.h"
#include "MurmurHash.h"
#include "VarInt.h"