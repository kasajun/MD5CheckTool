#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __C20_H_825F828C_DB90_413D_A19A_BD48F31FE10B__
#define __C20_H_825F828C_DB90_413D_A19A_BD48F31FE10B__

#if defined(_MSVC_LANG) && _MSVC_LANG >= 202002L

#define IF_LIKELY(A)			if (A) [[likely]]
#define IF_UNLIKELY(A)			if (A) [[unlikely]]

#define WHILE_LIKELY(A)			while (A) [[likely]]
#define WHILE_UNLIKELY(A)		while (A) [[unlikely]]

#define SWITCH_FALLTHROUGH		[[fallthrough]];

#else

#define IF_LIKELY(A)			if (A)
#define IF_UNLIKELY(A)			if (A)

#define WHILE_LIKELY(A)			while (A)
#define WHILE_UNLIKELY(A)		while (A)

#define SWITCH_FALLTHROUGH		;

#endif
#endif
