
#ifndef STDAFX_H
#define STDAFX_H

#ifndef LINUX
#include "OatppMacros.h"
// Import headers that define types needed before _HAS_STD_BYTE is defined
#include "id/UuidFacade.h"

// Resolve std::byte type redefinition
#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0

// Add headers to be precompiled here
#include "mysql/jdbc.h"
#include "SqlSession.h"
#include "yaml-cpp/yaml.h"
#include "sw/redis++/redis++.h"

#endif // !LINUX

#endif //STDAFX_H
