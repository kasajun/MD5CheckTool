#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __TARGETVER_H_FF68BA53_7FB9_464d_8025_416D277E971D__
#define __TARGETVER_H_FF68BA53_7FB9_464d_8025_416D277E971D__

// �ȉ��̃}�N���́A�Œ���K�v�ȃv���b�g�t�H�[�����`���܂��B�Œ���K�v�ȃv���b�g�t�H�[���Ƃ́A
// �A�v���P�[�V���������s���邽�߂ɕK�v�ȋ@�\��������ł��Â��o�[�W������ Windows �� Internet Explorer �Ȃ�
// �������܂��B�����̃}�N���́A�w�肵���o�[�W�����ƁA����ȑO�̃o�[�W�����̃v���b�g�t�H�[����ŗ��p�ł��邷�ׂĂ̋@�\��L���ɂ��邱�Ƃɂ���� 
// ���삵�܂��B

// ���Ŏw�肳�ꂽ��`�̑O�ɑΏۃv���b�g�t�H�[�����w�肵�Ȃ���΂Ȃ�Ȃ��ꍇ�A�ȉ��̒�`��ύX���Ă��������B
// �قȂ�v���b�g�t�H�[���ɑΉ�����l�Ɋւ���ŐV���ɂ��ẮAMSDN ���Q�Ƃ��Ă��������B

#if _MSC_VER > 1600

#ifndef _WIN32_WINNT					// �Œ���K�v�ȃv���b�g�t�H�[���� Windows 7 �ł��邱�Ƃ��w�肵�܂��B
#define _WIN32_WINNT 0x0A00				// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
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
