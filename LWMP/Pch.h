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
#include <array>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

// detours
#include <detours.h>

// LWMP
#define BASE_ADDRESS 0x400000

#include "Address.h"
#include "VarInt.h"
#include "BitReader.h"
#include "BitWriter.h"
#include "Debug.h"
#include "Helpers.h"
#include "Types.h"
#include "Math.h"
#include "Globals.h"
#include "Functions.h"