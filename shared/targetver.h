#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __TARGETVER_H_FF68BA53_7FB9_464d_8025_416D277E971D__
#define __TARGETVER_H_FF68BA53_7FB9_464d_8025_416D277E971D__

// 以下のマクロは、最低限必要なプラットフォームを定義します。最低限必要なプラットフォームとは、
// アプリケーションを実行するために必要な機能を備えた最も古いバージョンの Windows や Internet Explorer など
// をいいます。これらのマクロは、指定したバージョンと、それ以前のバージョンのプラットフォーム上で利用できるすべての機能を有効にすることによって 
// 動作します。

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。

#if _MSC_VER > 1600

#ifndef _WIN32_WINNT					// 最低限必要なプラットフォームが Windows 7 であることを指定します。
#define _WIN32_WINNT 0x0A00				// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#include <WinSDKVer.h>

#else

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0400
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#endif
#endif
