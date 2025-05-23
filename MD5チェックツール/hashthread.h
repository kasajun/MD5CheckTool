#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __HASHTHREAD_H_E03E02D0_8829_49b2_AE83_F8FAC8C083F9__
#define __HASHTHREAD_H_E03E02D0_8829_49b2_AE83_F8FAC8C083F9__

#include "..\shared\targetver.h"
#include "..\shared\charlib.h"
#include "..\shared\md32_ctx.h"
#include "..\shared\sha_ctx.h"
#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include <errno.h>


#ifndef MAX_PATH_SIZE
#ifdef _UNICODE
#define MAX_PATH_SIZE					(_MAX_PATH * 4)
#define MAX_PATH_SIZE_ALIGNMENT			(272 * 4)
#else
#define MAX_PATH_SIZE					(_MAX_PATH * 2)
#define MAX_PATH_SIZE_ALIGNMENT			(272 * 2)
#endif
#endif

#ifndef MAX_STRINGTABLE
#define MAX_STRINGTABLE					128
#define MAX_STRINGTABLE_ALIGNMENT		128
#endif

#ifndef MAX_LOADSTRING
#define MAX_LOADSTRING					64
#define MAX_LOADSTRING_ALIGNMENT		64
#endif

#ifndef NUMBER_LOADSTRING
#define NUMBER_LOADSTRING				33
#define NUMBER_LOADSTRING_ALIGNMENT		48
#endif


#define HASHFILE_CHARCODE_ANSI			0
#define HASHFILE_CHARCODE_SHIFT_JIS		0
#define HASHFILE_CHARCODE_UNICODE		1
#define HASHFILE_CHARCODE_UTF16			1
#define HASHFILE_CHARCODE_UTF8			2
#define HASHFILE_CHARCODE_EUCJP			3
#define HASHFILE_CHARCODE_CHARCODENAME	(DWORD)-2
#define HASHFILE_CHARCODE_BUFFERSIZE	0x00001000


#define MAX_HASH_TYPE					22

#define HASH_LEN						64
#define HASH_LOADSTRING					(HASH_LEN * 2 + 1)

#define HASH_MD5_TYPE					0
#define HASH_MD5_NAME					"MD5"
#define HASH_MD5_LISTNAME				"MD5 (初期値)"
#define HASH_MD5_NAMEW					L"MD5"
#define HASH_MD5_LISTNAMEW				L"MD5 (初期値)"
#define HASH_MD5_LEN					MD5_DIGEST_LENGTH
#define HASH_MD5_LOADSTRING				(HASH_MD5_LEN * 2 + 1)
#define HASH_MD5_COLOR					HASH_MD5_TYPE
#define HASH_MD5_BENCH_SIZE				0x400000000
#define HASH_MD5_BYTE_SWAP				0

#define HASH_SHA1_TYPE					1
#define HASH_SHA1_NAME					"SHA-1"
#define HASH_SHA1_LISTNAME				HASH_SHA1_NAME
#define HASH_SHA1_NAMEW					L"SHA-1"
#define HASH_SHA1_LISTNAMEW				HASH_SHA1_NAMEW
#define HASH_SHA1_LEN					SHA1_DIGEST_LENGTH
#define HASH_SHA1_LOADSTRING			(HASH_SHA1_LEN * 2 + 1)
#define HASH_SHA1_COLOR					HASH_SHA1_TYPE
#define HASH_SHA1_BENCH_SIZE			0x800000000
#define HASH_SHA1_BYTE_SWAP				0

#define HASH_SHA_256_192_TYPE			2
#define HASH_SHA_256_192_NAME			"SHA-256/192"
#define HASH_SHA_256_192_LISTNAME		HASH_SHA_256_192_NAME
#define HASH_SHA_256_192_NAMEW			L"SHA-256/192"
#define HASH_SHA_256_192_LISTNAMEW		HASH_SHA_256_192_NAMEW
#define HASH_SHA_256_192_LEN			SHA256_192_DIGEST_LENGTH
#define HASH_SHA_256_192_LOADSTRING		(HASH_SHA_256_192_LEN * 2 + 1)
#define HASH_SHA_256_192_COLOR			HASH_SHA_256_192_TYPE
#define HASH_SHA_256_192_BENCH_SIZE		0x800000000
#define HASH_SHA_256_192_BYTE_SWAP		0

#define HASH_SHA_224_TYPE				3
#define HASH_SHA_224_NAME				"SHA-224"
#define HASH_SHA_224_LISTNAME			HASH_SHA_224_NAME
#define HASH_SHA_224_NAMEW				L"SHA-224"
#define HASH_SHA_224_LISTNAMEW			HASH_SHA_224_NAMEW
#define HASH_SHA_224_LEN				SHA224_DIGEST_LENGTH
#define HASH_SHA_224_LOADSTRING			(HASH_SHA_224_LEN * 2 + 1)
#define HASH_SHA_224_COLOR				HASH_SHA_224_TYPE
#define HASH_SHA_224_BENCH_SIZE			0x800000000
#define HASH_SHA_224_BYTE_SWAP			0

#define HASH_SHA_256_TYPE				4
#define HASH_SHA_256_NAME				"SHA-256"
#define HASH_SHA_256_LISTNAME			"SHA-256 (推奨)"
#define HASH_SHA_256_NAMEW				L"SHA-256"
#define HASH_SHA_256_LISTNAMEW			L"SHA-256 (推奨)"
#define HASH_SHA_256_LEN				SHA256_DIGEST_LENGTH
#define HASH_SHA_256_LOADSTRING			(HASH_SHA_256_LEN * 2 + 1)
#define HASH_SHA_256_COLOR				HASH_SHA_256_TYPE
#define HASH_SHA_256_BENCH_SIZE			0x800000000
#define HASH_SHA_256_BYTE_SWAP			0

#define HASH_SHA_384_TYPE				5
#define HASH_SHA_384_NAME				"SHA-384"
#define HASH_SHA_384_LISTNAME			HASH_SHA_384_NAME
#define HASH_SHA_384_NAMEW				L"SHA-384"
#define HASH_SHA_384_LISTNAMEW			HASH_SHA_384_NAMEW
#define HASH_SHA_384_LEN				SHA384_DIGEST_LENGTH
#define HASH_SHA_384_LOADSTRING			(HASH_SHA_384_LEN * 2 + 1)
#define HASH_SHA_384_COLOR				HASH_SHA_384_TYPE
#define HASH_SHA_384_BENCH_SIZE			0x400000000
#define HASH_SHA_384_BYTE_SWAP			0

#define HASH_SHA_512_TYPE				6
#define HASH_SHA_512_NAME				"SHA-512"
#define HASH_SHA_512_LISTNAME			HASH_SHA_512_NAME
#define HASH_SHA_512_NAMEW				L"SHA-512"
#define HASH_SHA_512_LISTNAMEW			HASH_SHA_512_NAMEW
#define HASH_SHA_512_LEN				SHA512_DIGEST_LENGTH
#define HASH_SHA_512_LOADSTRING			(HASH_SHA_512_LEN * 2 + 1)
#define HASH_SHA_512_COLOR				HASH_SHA_512_TYPE
#define HASH_SHA_512_BENCH_SIZE			0x400000000
#define HASH_SHA_512_BYTE_SWAP			0

#define HASH_SHA_512_224_TYPE			7
#define HASH_SHA_512_224_NAME			"SHA-512/224"
#define HASH_SHA_512_224_LISTNAME		HASH_SHA_512_224_NAME
#define HASH_SHA_512_224_NAMEW			L"SHA-512/224"
#define HASH_SHA_512_224_LISTNAMEW		HASH_SHA_512_224_NAMEW
#define HASH_SHA_512_224_LEN			HASH_SHA_224_LEN
#define HASH_SHA_512_224_LOADSTRING		HASH_SHA_224_LOADSTRING
#define HASH_SHA_512_224_COLOR			HASH_SHA_512_224_TYPE
#define HASH_SHA_512_224_BENCH_SIZE		0x400000000
#define HASH_SHA_512_224_BYTE_SWAP		0

#define HASH_SHA_512_256_TYPE			8
#define HASH_SHA_512_256_NAME			"SHA-512/256"
#define HASH_SHA_512_256_LISTNAME		HASH_SHA_512_256_NAME
#define HASH_SHA_512_256_NAMEW			L"SHA-512/256"
#define HASH_SHA_512_256_LISTNAMEW		HASH_SHA_512_256_NAMEW
#define HASH_SHA_512_256_LEN			HASH_SHA_256_LEN
#define HASH_SHA_512_256_LOADSTRING		HASH_SHA_256_LOADSTRING
#define HASH_SHA_512_256_COLOR			HASH_SHA_512_256_TYPE
#define HASH_SHA_512_256_BENCH_SIZE		0x400000000
#define HASH_SHA_512_256_BYTE_SWAP		0

#define HASH_SHA3_224_TYPE				9
#define HASH_SHA3_224_NAME				"SHA3-224"
#define HASH_SHA3_224_LISTNAME			HASH_SHA3_224_NAME
#define HASH_SHA3_224_NAMEW				L"SHA3-224"
#define HASH_SHA3_224_LISTNAMEW			HASH_SHA3_224_NAMEW
#define HASH_SHA3_224_LEN				HASH_SHA_224_LEN
#define HASH_SHA3_224_LOADSTRING		HASH_SHA_224_LOADSTRING
#define HASH_SHA3_224_COLOR				HASH_SHA3_224_TYPE
#define HASH_SHA3_224_BENCH_SIZE		0x200000000
#define HASH_SHA3_224_BYTE_SWAP			0

#define HASH_SHA3_256_TYPE				10
#define HASH_SHA3_256_NAME				"SHA3-256"
#define HASH_SHA3_256_LISTNAME			HASH_SHA3_256_NAME
#define HASH_SHA3_256_NAMEW				L"SHA3-256"
#define HASH_SHA3_256_LISTNAMEW			HASH_SHA3_256_NAMEW
#define HASH_SHA3_256_LEN				HASH_SHA_256_LEN
#define HASH_SHA3_256_LOADSTRING		HASH_SHA_256_LOADSTRING
#define HASH_SHA3_256_COLOR				HASH_SHA3_256_TYPE
#define HASH_SHA3_256_BENCH_SIZE		0x200000000
#define HASH_SHA3_256_BYTE_SWAP			0

#define HASH_SHA3_384_TYPE				11
#define HASH_SHA3_384_NAME				"SHA3-384"
#define HASH_SHA3_384_LISTNAME			HASH_SHA3_384_NAME
#define HASH_SHA3_384_NAMEW				L"SHA3-384"
#define HASH_SHA3_384_LISTNAMEW			HASH_SHA3_384_NAMEW
#define HASH_SHA3_384_LEN				HASH_SHA_384_LEN
#define HASH_SHA3_384_LOADSTRING		HASH_SHA_384_LOADSTRING
#define HASH_SHA3_384_COLOR				HASH_SHA3_384_TYPE
#define HASH_SHA3_384_BENCH_SIZE		0x200000000
#define HASH_SHA3_384_BYTE_SWAP			0

#define HASH_SHA3_512_TYPE				12
#define HASH_SHA3_512_NAME				"SHA3-512"
#define HASH_SHA3_512_LISTNAME			HASH_SHA3_512_NAME
#define HASH_SHA3_512_NAMEW				L"SHA3-512"
#define HASH_SHA3_512_LISTNAMEW			HASH_SHA3_512_NAMEW
#define HASH_SHA3_512_LEN				HASH_SHA_512_LEN
#define HASH_SHA3_512_LOADSTRING		HASH_SHA_512_LOADSTRING
#define HASH_SHA3_512_COLOR				HASH_SHA3_512_TYPE
#define HASH_SHA3_512_BENCH_SIZE		0x200000000
#define HASH_SHA3_512_BYTE_SWAP			0

#define HASH_CRC32_TYPE					13
#define HASH_CRC32_NAME					"crc32"
#define HASH_CRC32_LISTNAME				HASH_CRC32_NAME
#define HASH_CRC32_NAMEW				L"crc32"
#define HASH_CRC32_LISTNAMEW			HASH_CRC32_NAMEW
#define HASH_CRC32_LEN					4
#define HASH_CRC32_LOADSTRING			(HASH_CRC32_LEN * 2 + 1)
#define HASH_CRC32_HASH_COLOR			HASH_CRC32_TYPE
#define HASH_CRC32_BENCH_SIZE			0x1000000000
#define HASH_CRC32_BYTE_SWAP			1

#define HASH_CRC32C_TYPE				14
#define HASH_CRC32C_NAME				"crc32c"
#define HASH_CRC32C_LISTNAME			HASH_CRC32C_NAME
#define HASH_CRC32C_NAMEW				L"crc32c"
#define HASH_CRC32C_LISTNAMEW			HASH_CRC32C_NAMEW
#define HASH_CRC32C_LEN					4
#define HASH_CRC32C_LOADSTRING			(HASH_CRC32C_LEN * 2 + 1)
#define HASH_CRC32C_HASH_COLOR			14
#ifdef _M_X64
#define HASH_CRC32C_BENCH_SIZE			0x8000000000
#else
#define HASH_CRC32C_BENCH_SIZE			0x4000000000
#endif
#define HASH_CRC32C_BYTE_SWAP			1

#define HASH_XXH32_TYPE					15
#define HASH_XXH32_NAME					"XXH32"
#define HASH_XXH32_LISTNAME				HASH_XXH32_NAME
#define HASH_XXH32_NAMEW				L"XXH32"
#define HASH_XXH32_LISTNAMEW			HASH_XXH32_NAMEW
#define HASH_XXH32_LEN					4
#define HASH_XXH32_LOADSTRING			(HASH_XXH32_TYPE * 2 + 1)
#define HASH_XXH32_HASH_COLOR			HASH_XXH32_TYPE
#define HASH_XXH32_BENCH_SIZE			0x2000000000
#define HASH_XXH32_BYTE_SWAP			1

#define HASH_XXH64_TYPE					16
#define HASH_XXH64_NAME					"XXH64"
#define HASH_XXH64_NAMEW				L"XXH64"
#ifndef _M_X64
#define HASH_XXH64_LISTNAME				"XXH64 (旧:xxHash[低速注意])"
#define HASH_XXH64_LISTNAMEW			L"XXH64 (旧:xxHash[低速注意])"
#else
#define HASH_XXH64_LISTNAME				"XXH64 (旧:xxHash)"
#define HASH_XXH64_LISTNAMEW			L"XXH64 (旧:xxHash)"
#endif
#define HASH_XXH64_LEN					8
#define HASH_XXH64_LOADSTRING			(HASH_XXH64_TYPE * 2 + 1)
#define HASH_XXH64_COLOR				HASH_XXH64_TYPE
#if  _M_X64
#define HASH_XXH64_BENCH_SIZE			0x4000000000
#else
#define HASH_XXH64_BENCH_SIZE			0x2000000000
#endif
#define HASH_XXH64_BYTE_SWAP			1

#define HASH_XXH3_TYPE					17
#define HASH_XXH3_NAME					"XXH3"
#define HASH_XXH3_LISTNAME				HASH_XXH3_NAME
#define HASH_XXH3_NAMEW					L"XXH3"
#define HASH_XXH3_LISTNAMEW				HASH_XXH3_NAMEW
#define HASH_XXH3_LEN					8
#define HASH_XXH3_LOADSTRING			(HASH_XXH3_LEN * 2 + 1)
#define HASH_XXH3_COLOR					HASH_XXH3_TYPE
#if _MSC_VER > 1300
#define HASH_XXH3_BENCH_SIZE			0x8000000000
#else
#define HASH_XXH3_BENCH_SIZE			0x2000000000
#endif
#define HASH_XXH3_BYTE_SWAP				1

#define HASH_XXH128_TYPE				18
#define HASH_XXH128_NAME				"XXH128"
#define HASH_XXH128_LISTNAME			"XXH128 (旧:xxHash3)"
#define HASH_XXH128_NAMEW				L"XXH128"
#define HASH_XXH128_LISTNAMEW			L"XXH128 (旧:xxHash3)"
#define HASH_XXH128_LEN					16
#define HASH_XXH128_LOADSTRING			(HASH_XXH128_LEN * 2 + 1)
#define XXH128_HASH_COLOR				HASH_XXH128_TYPE
#if _MSC_VER > 1300
#define HASH_XXH128_BENCH_SIZE			0x8000000000
#else
#define HASH_XXH128_BENCH_SIZE			0x2000000000
#endif
#define HASH_XXH128_BYTE_SWAP			1

#define HASH_MD2_TYPE					19
#define HASH_MD2_NAME					"MD2"
#define HASH_MD2_LISTNAME				"MD2 (鈍速注意)"
#define HASH_MD2_NAMEW					L"MD2"
#define HASH_MD2_LISTNAMEW				L"MD2 (鈍速注意)"
#define HASH_MD2_LEN					MD2_DIGEST_LENGTH
#define HASH_MD2_LOADSTRING				(HASH_MD2_LEN * 2 + 1)
#define HASH_MD2_COLOR					HASH_MD2_TYPE
#define HASH_MD2_BENCH_SIZE				0x10000000
#define HASH_MD2_BYTE_SWAP				0

#define HASH_MD4_TYPE					20
#define HASH_MD4_NAME					"MD4"
#define HASH_MD4_LISTNAME				HASH_MD4_NAME
#define HASH_MD4_NAMEW					L"MD4"
#define HASH_MD4_LISTNAMEW				HASH_MD4_NAMEW
#define HASH_MD4_LEN					MD4_DIGEST_LENGTH
#define HASH_MD4_LOADSTRING				(HASH_MD4_LEN * 2 + 1)
#define HASH_MD4_COLOR					HASH_MD4_TYPE
#define HASH_MD4_BENCH_SIZE				0x400000000
#define HASH_MD4_BYTE_SWAP				0

#define HASH_NONE_TYPE					21
#define HASH_NONE_LEN					4
#define HASH_NONE_NAME					"None"
#define HASH_NONE_LISTNAME				"None (メモリー速度)"
#define HASH_NONE_NAMEW					L"None"
#define HASH_NONE_LISTNAMEW				L"None (メモリー速度)"
#define HASH_NONE_LOADSTRING			(HASH_NONE_TYPE * 2 + 1)
#define HASH_NONE_COLOR					HASH_NONE_TYPE
#define HASH_NONE_BENCH_SIZE			0x10000000000
#define HASH_NONE_BYTE_SWAP				0

#define HASH_MAX_COLOR					22

#define HASH_NAME_SPACE					": "
#define HASH_TEXT_SPACE					"  "
#define HASH_TEXT_RETURNCODE			"\r\n"

#define HASH_RTF_HEADER1				"{\\rtf1\\ansi\\ansicpg932\\deff0\\nouicompat\\deflang1033\\deflangfe1041{\\fonttbl{\\f0\\fnil\\fcharset128 \\\'82\\\'6c\\\'82\\\'72 \\\'82\\\'6f\\\'83\\\'53\\\'83\\\'56\\\'83\\\'62\\\'83\\\'4e;}}\r\n{\\colortbl ;"
#define HASH_RTF_HEADER2				"}\r\n\\uc1 \r\n\\pard\\sa200\\sl276\\slmult1\\b0\\f0\\fs22\\lang17 "
#define HASH_RTF_COLOR_TAG0				"\\cf0 "
#define HASH_RTF_COLOR_TAG1				"\\cf1 "
#define HASH_RTF_RETURNCODE				"\\line\r\n"
#define HASH_RTF_RETURNCODE1			"\\pre\r\n"
#define HASH_RTF_FOOTER					"\\b0\\par\r\n}"

#define HASH_HTML_HEADER				"<!DOCTYPE>\r\n<html>\r\n<body>\r\n\r\n<!--StartFragment-->\r\n<p>"
#define HASH_HTML_COLOR_TAG0			"<span style=\""
#define HASH_HTML_COLOR_TAG1			"\">"
#define HASH_HTML_COLOR_TAG2			"</span>"
#define HASH_HTML_RETURNCODE			"<br>\r\n"
#define HASH_HTML_FOOTER				"</p><!--EndFragment-->\r\n\r\n</body>\r\n</html>"


#define HASH_RTF_COLOR1					"\\red243\\green21\\blue89;"
#define HASH_RTF_COLOR2					"\\red255\\green100\\blue100;"
#define HASH_RTF_COLOR3					"\\red255\\green171\\blue103;"
#define HASH_RTF_COLOR4					"\\red254\\green230\\blue133;"
#define HASH_RTF_COLOR5					"\\red91\\green231\\blue169;"
#define HASH_RTF_COLOR6					"\\red91\\green169\\blue231;"
#define HASH_RTF_COLOR7					"\\red102\\green117\\blue255;"
#define HASH_RTF_COLOR8					"\\red186\\green102\\blue255;"
#define HASH_RTF_COLOR9					"\\red255\\green100\\blue255;"
#define HASH_RTF_COLOR10				"\\red191\\green243\\blue162;"
#define HASH_RTF_COLOR11				"\\red151\\green228\\blue187;"
#define HASH_RTF_COLOR12				"\\red110\\green212\\blue212;"
#define HASH_RTF_COLOR13				"\\red115\\green186\\blue223;"
#define HASH_RTF_COLOR14				"\\red247\\green150\\blue70;"
#define HASH_RTF_COLOR15				"\\red255\\green192\\blue0;"
#define HASH_RTF_COLOR16				"\\red255\\green201\\blue150;"
#define HASH_RTF_COLOR17				"\\red255\\green132\\blue116;"
#define HASH_RTF_COLOR18				"\\red191\\green64\\blue135;"
#define HASH_RTF_COLOR19				"\\red121\\green64\\blue191;"
#define HASH_RTF_COLOR20				"\\red143\\green128\\blue255;"
#define HASH_RTF_COLOR21				"\\red255\\green82\\blue162;"
#define HASH_RTF_COLOR22				"\\red0\\green0\\blue0;"

#define HASH_HTML_COLOR1				"color: #f31559;"
#define HASH_HTML_COLOR2				"color: #ff6464;"
#define HASH_HTML_COLOR3				"color: #ffab67;"
#define HASH_HTML_COLOR4				"color: #fee685;"
#define HASH_HTML_COLOR5				"color: #91e7a9;"
#define HASH_HTML_COLOR6				"color: #91a9e7;"
#define HASH_HTML_COLOR7				"color: #6675ff;"
#define HASH_HTML_COLOR8				"color: #ba66ff;"
#define HASH_HTML_COLOR9				"color: #ff64ff;"
#define HASH_HTML_COLOR10				"color: #bff3a2;"
#define HASH_HTML_COLOR11				"color: #97e4bb;"
#define HASH_HTML_COLOR12				"color: #6ed4d4;"
#define HASH_HTML_COLOR13				"color: #73badf;"
#define HASH_HTML_COLOR14				"color: #f79646;"
#define HASH_HTML_COLOR15				"color: #ffc000;"
#define HASH_HTML_COLOR16				"color: #ffc996;"
#define HASH_HTML_COLOR17				"color: #ff8474;"
#define HASH_HTML_COLOR18				"color: #bf4087;"
#define HASH_HTML_COLOR19				"color: #7940bf;"
#define HASH_HTML_COLOR20				"color: #8f80ff;"
#define HASH_HTML_COLOR21				"color: #ff52a2;"
#define HASH_HTML_COLOR22				"color: black;"

#define HASH_TYPE1						HASH_MD5_TYPE
#define HASH_TYPE2						HASH_SHA1_TYPE
#define HASH_TYPE3						HASH_SHA_224_TYPE
#define HASH_TYPE4						HASH_SHA_256_TYPE
#define HASH_TYPE5						HASH_SHA_256_192_TYPE
#define HASH_TYPE6						HASH_SHA_384_TYPE
#define HASH_TYPE7						HASH_SHA_512_TYPE
#define HASH_TYPE8						HASH_SHA_512_224_TYPE
#define HASH_TYPE9						HASH_SHA_512_256_TYPE
#define HASH_TYPE10						HASH_SHA3_224_TYPE
#define HASH_TYPE11						HASH_SHA3_256_TYPE
#define HASH_TYPE12						HASH_SHA3_384_TYPE
#define HASH_TYPE13						HASH_SHA3_512_TYPE
#define HASH_TYPE14						HASH_CRC32_TYPE
#define HASH_TYPE15						HASH_CRC32C_TYPE
#define HASH_TYPE16						HASH_XXH32_TYPE
#define HASH_TYPE17						HASH_XXH64_TYPE
#define HASH_TYPE18						HASH_XXH3_TYPE
#define HASH_TYPE19						HASH_XXH128_TYPE
#define HASH_TYPE20						HASH_MD2_TYPE
#define HASH_TYPE21						HASH_MD4_TYPE
#define HASH_TYPE22						HASH_NONE_TYPE


#define FILE_BUFFER_SIZE				0x00010000	// 65536
#define FILE_SMALL_BUFFER_SIZE			0x00001000	// 4096
#define FILE_DEF_COUNTSIZE				0x00000100	// 256
#define FILE_MAX_COUNTSIZE				0x00100000	// 1048576
#define HASHFILE_NOLINE					0xFFFFFFFF
#define DEF_FILE_ATTRIBUTE				~(FILE_ATTRIBUTE_HIDDEN)	// ~(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)

#define APP_MESSAGE_INIT				0x00000001
#define APP_MESSAGE_START				0x00000002
#define APP_MESSAGE_PROGRESS			0x00000003
#define APP_MESSAGE_END					0x00000004
#define APP_MESSAGE_ERROR				0x00000005
#define APP_MESSAGE_PAUSE				0x00000006
#define APP_MESSAGE_FILE_CHANGE			0x00000007
#define APP_MESSAGE_FILE_EXIT			0x00000008
#define APP_MESSAGE_FOLDER_INIT			0x00000009
#define APP_MESSAGE_FOLDER_INIT_EXIT	0x0000000A
#define APP_MESSAGE_SUBFOLDER_OPEN		0x0000000B
#define APP_MESSAGE_HASHFILE_INIT		0x0000000C
#define APP_MESSAGE_HASHFILE_INIT_EXIT	0x0000000D
#define APP_MESSAGE_HASHFILE_HASHCHANGE	0x0000000E
#define APP_MESSAGE_FILE_HASHCOMP		0x0000000F
#define APP_MESSAGE_LASTERROR			0x00000100

#define APP_PRGRESS_START				0x00000001
#define APP_PRGRESS_EXIT				0x00000002
#define APP_PRGRESS_PAUSE				0x00000004
#define APP_PRGRESS_CANCEL				0x00000008


struct tagHashThread_FileRecode {
	TCHAR szFileName[MAX_PATH_SIZE];
	size_t nFileNameLength;
	__int64 llFileSize;
	DWORD dwFileAttributes;
	FILETIME ftFileLastWriteTime;
	BYTE pFileHashByte[HASH_LEN];
	DWORD dwFileLastError; 
	DWORD dwFileHashType;
	DWORD dwHashFileID;
	DWORD dwHashFileHashLine;
	BOOL nIsHashOldFile;
	BYTE nHashFileHashByte[HASH_LEN];
	BOOL nHashFileCmp;			// Hashファイルの比較が記録されます。ただし memcmp の戻り値が代入されることに注意してください。
	DWORD dwHashFileHashType;
	UINT nNextFile;//次のファイル番号が記録されます。
};

struct tagHashThread_MultiFile {
	DWORD dwStructSize;
	DWORD dwMaxBufSize;
	DWORD dwFileCount;			// ファイルの数が記録されます。
	DWORD dwFileCurrentCount;	// 計算中のファイルNo.が記録されます。
	TCHAR szCurrentFolder[MAX_PATH_SIZE];
	__int64 llFileAllSize;
	__int64 llFileAllCurrentSize;
	__int64 llFileAllSizeBackup;
	DWORD dwHashFileCharCode;	// Hashファイルの文字コードが記録されます。
	DWORD dwHashFileBom;		// Unicode UTF-8 の BOM が存在していたら、BOM のサイズが記録されます。
	struct tagHashThread_FileRecode *ptagFileRecode;
};

typedef struct tagHashThread {
	DWORD dwStructSize; // バージョンアップ用に予約
	HWND hWnd;
	UINT nMessageID;
	BOOL nSuspendMessage;
	HANDLE hHashThreadHandle;
	unsigned int nThreadID;
	DWORD dwHashThreadLastError;
	BYTE* mdCTX;
	const TCHAR* cpHashName;// 通常は "Hash" が記録されます。
	DWORD dwHashType;// 通常は 0 が記録されます。
	DWORD dwHashLen;// 通常は 16 が記録されます。
	BYTE bHashByte[HASH_LEN];
	const TCHAR* cpHashExt;
	size_t nHashExtLen;
	DWORD dwStartTime;
	DWORD dwEndTime;
	BOOL nIsFileNoCheck;
	TCHAR* pFilePath;
	__int64 llFileSize;
	__int64 llFileReadSize;
	DWORD dwFileAttributeMask;	// 追加できるファイルの属性を指定します。
	BOOL nFileNoCache;// Hash計算時にファイルキャッシュを無効にします。
	FILE* pInFilePointer;
	BYTE* pFileBuffer;
	DWORD dwFileBufferAllocateSize;
	DWORD dwFileBufferSize;
	DWORD dwCreateFileAttributes;
	//
	// ここからマルチファイル用の定義
	//
	HANDLE hFile;
	struct tagHashThread_MultiFile *tagMultiFile;
} HashThread, *lptagHashThread;


BOOL HashThread_GetString(TCHAR* pOutTChar, const BYTE* cpInBinary, const DWORD dwHashType, BOOL bIsUpper = FALSE, TCHAR* pOutBase64TChar = NULL);

unsigned __stdcall HashThread_MultiFile_Thread(void* lpThread);
//unsigned __stdcall HashThread_MultiFile_Mmap_Thread(void* lpThread);
unsigned __stdcall HashThread_MultiFile_Bench_Thread(void* lpThread);

BOOL HashThread_Startup(HashThread* ptagHashThread);
BOOL HashThread_Shutdown(HashThread* ptagHashThread);
BOOL HashThread_SetHashType(HashThread* ptagHashThread, DWORD dwHashType);
BOOL HashThread_SetHashFunc(BOOL nIsNoAsm);
DWORD HashThread_GetCurrentItem_Core(const HashThread* cptagHashThread);
DWORD HashThread_GetCurrentItem(const HashThread* cptagHashThread);
DWORD HashThread_GetCountItem_Core(const HashThread* cptagHashThread);
DWORD HashThread_GetCountItem(const HashThread* cptagHashThread);
tagHashThread_FileRecode* HashThread_GetFileRecode_Core(const HashThread* cptagHashThread, DWORD dwItem = (DWORD)-1);
tagHashThread_FileRecode* HashThread_GetFileRecode(const HashThread* cptagHashThread, DWORD dwItem = (DWORD)-1);
tagHashThread_FileRecode* HashThread_GetFileCurrentFileRecode_Core(const HashThread* cptagHashThread);
tagHashThread_FileRecode* HashThread_GetFileCurrentFileRecode(const HashThread* cptagHashThread);
tagHashThread_FileRecode* HashThread_GetFileCountFileRecode_Core(const HashThread* cptagHashThread);
tagHashThread_FileRecode* HashThread_GetFileCountFileRecode(const HashThread* cptagHashThread);

#define HASHTHREAD_FILERECODE(ptagHashThread, dwItem) HashThread_GetFileRecode((ptagHashThread), (dwItem))
#define HASHTHREAD_FILECURRENT(ptagHashThread) HashThread_GetFileCurrentFileRecode((ptagHashThread))
#define HASHTHREAD_FILECOUNT(ptagHashThread) HashThread_GetFileCountFileRecode((ptagHashThread))

BOOL HashThread_AddFileRecode(HashThread* ptagHashThread, const TCHAR* cpInFileName, WIN32_FIND_DATA* fdFind, DWORD dwHashLine);
BOOL HashThread_ClearFileRecode(HashThread* ptagHashThread, BOOL nCurrentFolderClear = TRUE);
DWORD HashThread_Folder_Init(HashThread* ptagHashThread, const TCHAR* cpInFileName, BOOL nIsAddFile);
DWORD HashThread_Folder_Open(HashThread* ptagHashThread, TCHAR* cpInFileName, size_t nInFileNameLen, BOOL nIsAddFile);
DWORD HashThread_MultiFile_Init(HashThread* ptagHashThread, const TCHAR* cpInFileName, BOOL nIsAddFile);
DWORD HashThread_HashFile_Init(HashThread* ptagHashThread, const TCHAR* cpInFileName, BOOL nIsAddFile, const TCHAR* cpInOption);
DWORD HashThread_GetCharCode(const HashThread* cptagHashThread);
DWORD HashThread_CreateBuffer(HashThread* ptagHashThread, DWORD dwFileCount);
tagHashThread_MultiFile* HashThread_CreateMultiFileBuffer(DWORD dwFileCount);
tagHashThread_FileRecode* HashThread_CreateFileRecodeBuffer(DWORD dwFileCount);
BOOL HashThread_ClearBuffer(HashThread* ptagHashThread);
BOOL HashThread_ClearBuffer_Core(HashThread* ptagHashThread, BOOL nIsMultiFileClear);
BOOL HashThread_DeleteItemBuffer(HashThread* ptagHashThread, DWORD dwItem);
BOOL HashThread_SwapItemBuffer(const HashThread* cptagHashThread, DWORD dwItem1, DWORD dwItem2);
BOOL HashThread_CreateHashFile(const HashThread* cptagHashThread, const TCHAR* cpInFileName, const TCHAR* cpInOption);
BOOL HashThread_FileWriteTime(const TCHAR* cpInFileName, const FILETIME* cpInFileWriteTime);
BOOL HashThread_ReNameFileRecode(HashThread* ptagHashThread, DWORD dwItem, const TCHAR* cpInFileName);

#define HashThread_HashToString HashThread_HashToString_no_sprintf
#define HashThread_HashToHash HashThread_HashToHash_no_strtoul

char* HashThread_HashToString_no_sprintf(char* pOutChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen = HASH_MD5_LEN);
WCHAR* HashThread_HashToString_no_sprintf(WCHAR* pOutWChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_no_scanf(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_no_scanf(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_no_strtoul(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_no_strtoul(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen = HASH_MD5_LEN);
char* HashThread_HashToString_Default(char* pOutChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen = HASH_MD5_LEN);
WCHAR* HashThread_HashToString_Default(WCHAR* pOutWChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_Default(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_Default(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen = HASH_MD5_LEN);
char* HashThread_HashToString_Old(char* pOutChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen = HASH_MD5_LEN);
WCHAR* HashThread_HashToString_Old(WCHAR* pOutWChar, const BYTE* cpInHashByte, BOOL nIsHashUpper, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_Old(BYTE* pOutHashByte, const char* cpInChar, const DWORD dwHashLen = HASH_MD5_LEN);
BYTE* HashThread_HashToHash_Old(BYTE* pOutHashByte, const WCHAR* cpInWChar, const DWORD dwHashLen = HASH_MD5_LEN);
BOOL HashThread_FileNameCheck(TCHAR* pInFileName);
DWORD HashThread_GetOldHashFile(const HashThread* cptagHashThread, const TCHAR* cpInFileName);
DWORD HashThread_GetHashLen(DWORD dwHashType);
DWORD HashThread_GetHashLenToType(DWORD dwHashLen);
DWORD HashThread_GetHashFileType(HashThread* ptagHashThread, const TCHAR* cpInFileName);
DWORD HashThread_GetHashFileType(HashThread* ptagHashThread, const TCHAR* cpInFileName, FILE* pInFilePointer);
BOOL HashThread_DigitCheck(const TCHAR* pInFileName, DWORD dwStringLen = (DWORD)-1);
const TCHAR* HashThread_GetHashName(DWORD dwHashType);
const TCHAR* HashThread_GetHashListName(DWORD dwHashType);
const TCHAR* HashThread_GetHashExt(DWORD dwHashType);
size_t HashThread_GetHashNameLen(DWORD dwHashType);
size_t HashThread_GetHashListNameLen(DWORD dwHashType);
size_t HashThread_GetHashExtLen(DWORD dwHashType);

#endif
