/*�@
*sal.h - markers for documenting the semantics of APIs
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       sal.h provides a set of annotations to describe how a function uses its
*       parameters - the assumptions it makes about them, and the guarantees it makes
*       upon finishing.
*
*       [Public]
*
****/

/*==========================================================================

   The comments in this file are intended to give basic understanding of
   the usage of SAL, the Microsoft Source Code Annotation Language.
   For more details, please see https://go.microsoft.com/fwlink/?LinkID=242134

   The macros are defined in 3 layers, plus the structural set:

   _In_/_Out_/_Ret_ Layer:
   ----------------------
   This layer provides the highest abstraction and its macros should be used
   in most cases. These macros typically start with:
      _In_     : input parameter to a function, unmodified by called function
      _Out_    : output parameter, written to by called function, pointed-to
                 location not expected to be initialized prior to call
      _Outptr_ : like _Out_ when returned variable is a pointer type
                 (so param is pointer-to-pointer type). Called function
                 provides/allocated space.
      _Outref_ : like _Outptr_, except param is reference-to-pointer type.
      _Inout_  : inout parameter, read from and potentially modified by
                 called function.
      _Ret_    : for return values
      _Field_  : class/struct field invariants
   For common usage, this class of SAL provides the most concise annotations.
   Note that _In_/_Out_/_Inout_/_Outptr_ annotations are designed to be used
   with a parameter target. Using them with _At_ to specify non-parameter
   targets may yield unexpected results.

   This layer also includes a number of other properties that can be specified
   to extend the ability of code analysis, most notably:
      -- Designating parameters as format strings for printf/scanf/scanf_s
      -- Requesting stricter type checking for C enum parameters

   _Pre_/_Post_ Layer:
   ------------------
   The macros of this layer only should be used when there is no suitable macro
   in the _In_/_Out_ layer. Its macros start with _Pre_ or _Post_.
   This layer provides the most flexibility for annotations.

   Implementation Abstraction Layer:
   --------------------------------
   Macros from this layer should never be used directly. The layer only exists
   to hide the implementation of the annotation macros.

   Structural Layer:
   ----------------
   These annotations, like _At_ and _When_, are used with annotations from
   any of the other layers as modifiers, indicating exactly when and where
   the annotations apply.


   Common syntactic conventions:
   ----------------------------

   Usage:
   -----
   _In_, _Out_, _Inout_, _Pre_, _Post_, are for formal parameters.
   _Ret_, _Deref_ret_ must be used for return values.

   Nullness:
   --------
   If the parameter can be NULL as a precondition to the function, the
   annotation contains _opt. If the macro does not contain '_opt' the
   parameter cannot be NULL.

   If an out/inout parameter returns a null pointer as a postcondition, this is
   indicated by _Ret_maybenull_ or _result_maybenull_. If the macro is not
   of this form, then the result will not be NULL as a postcondition.
     _Outptr_ - output value is not NULL
     _Outptr_result_maybenull_ - output value might be NULL

   String Type:
   -----------
   _z: NullTerminated string
   for _In_ parameters the buffer must have the specified stringtype before the call
   for _Out_ parameters the buffer must have the specified stringtype after the call
   for _Inout_ parameters both conditions apply

   Extent Syntax:
   -------------
   Buffer sizes are expressed as element counts, unless the macro explicitly
   contains _byte_ or _bytes_. Some annotations specify two buffer sizes, in
   which case the second is used to indicate how much of the buffer is valid
   as a postcondition. This table outlines the precondition buffer allocation
   size, precondition number of valid elements, postcondition allocation size,
   and postcondition number of valid elements for representative buffer size
   annotations:
                                     Pre    |  Pre    |  Post   |  Post
                                     alloc  |  valid  |  alloc  |  valid
      Annotation                     elems  |  elems  |  elems  |  elems
      ----------                     ------------------------------------
      _In_reads_(s)                    s    |   s     |   s     |   s
      _Inout_updates_(s)               s    |   s     |   s     |   s
      _Inout_updates_to_(s,c)          s    |   s     |   s     |   c
      _Out_writes_(s)                  s    |   0     |   s     |   s
      _Out_writes_to_(s,c)             s    |   0     |   s     |   c
      _Outptr_result_buffer_(s)        ?    |   ?     |   s     |   s
      _Outptr_result_buffer_to_(s,c)   ?    |   ?     |   s     |   c

   For the _Outptr_ annotations, the buffer in question is at one level of
   dereference. The called function is responsible for supplying the buffer.

   Success and failure:
   -------------------
   The SAL concept of success allows functions to define expressions that can
   be tested by the caller, which if it evaluates to non-zero, indicates the
   function succeeded, which means that its postconditions are guaranteed to
   hold.  Otherwise, if the expression evaluates to zero, the function is
   considered to have failed, and the postconditions are not guaranteed.

   The success criteria can be specified with the _Success_(expr) annotation:
     _Success_(return != FALSE) BOOL
     PathCanonicalizeA(_Out_writes_(MAX_PATH) LPSTR pszBuf, LPCSTR pszPath) :
        pszBuf is only guaranteed to be NULL-terminated when TRUE is returned,
        and FALSE indicates failure. In common practice, callers check for zero
        vs. non-zero returns, so it is preferable to express the success
        criteria in terms of zero/non-zero, not checked for exactly TRUE.

   Functions can specify that some postconditions will still hold, even when
   the function fails, using _On_failure_(anno-list), or postconditions that
   hold regardless of success or failure using _Always_(anno-list).

   The annotation _Return_type_success_(expr) may be used with a typedef to
   give a default _Success_ criteria to all functions returning that type.
   This is the case for common Windows API status types, including
   HRESULT and NTSTATUS.  This may be overridden on a per-function basis by
   specifying a _Success_ annotation locally.

============================================================================*/
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __SAL_ANNOTATION_H_1C0EABD2_C820_4B5A_9C03_732BC2765984__
#define __SAL_ANNOTATION_H_1C0EABD2_C820_4B5A_9C03_732BC2765984__
#if _MSC_VER > 1600
#include <sal.h>

#else
/*==========================================================================
	�|�C���^�[ �p�����[�^�[

	���̕\�̒��߂ɂ��ẮA�|�C���^�[ �p�����[�^�[�ɒ��߂�t����ƁA
	�|�C���^�[�� null �̏ꍇ�A�A�i���C�U�[�̓G���[��񍐂��܂��B
	���̒��߂́A�|�C���^�[�ƁA�|�C���g����Ă���C�ӂ̃f�[�^���ڂɓK�p����܂��B
============================================================================*/

/*==========================================================================
	_In_ ����

	�X�J���[�A�\���́A�\���̂ւ̃|�C���^�[�A����тȂǂ̓��̓p�����[�^�[��
	���߂���͂��܂��B�P���ȃX�J���[�ł́A�����I�Ɏg�p�ł��܂��B 
	�p�����[�^�[�́A���O��ԂŗL���ł���K�v������A�ύX����܂���B
============================================================================*/
#ifndef _In_
#define _In_
#endif

/*==========================================================================
	_Out_ ����

	�X�J���[�A�\���́A�\���̂ւ̃|�C���^�[�A����тȂǂ̏o�̓p�����[�^�[��
	���߂��L�q���܂��B�l�ɂ���ēn�����X�J���[�ȂǁA�l��Ԃ��Ȃ�
	�I�u�W�F�N�g�ɂ��̒��߂�K�p���Ȃ��ł��������B�p�����[�^�[�́A���O���
	�ł͗L���ł���K�v�͂���܂��񂪁A�����ԂŗL���ł���K�v������܂��B
============================================================================*/
#ifndef _Out_
#define _Out_
#endif

/*==========================================================================
	_Inout_ ����

	�֐��ɂ���ĕύX�����p�����[�^�[�ɒ��߂�ݒ肵�܂��B����́A���O��Ԃ�
	�����Ԃ̗����ŗL���ł���K�v������܂����A�Ăяo���̑O��ɈقȂ�l��
	����ƌ��Ȃ���܂��B�ύX�\�Ȓl�ɓK�p����K�v������܂��B
============================================================================*/
#ifndef _Inout_
#define _Inout_
#endif

/*==========================================================================
	_In_z_ ����

	���͂Ƃ��Ďg�p����� null �ŏI��镶����ւ̃|�C���^�[�B������́A
	���O��ԂŗL���ł���K�v������܂��B�ɂ́A���ɐ��������߂����� PSTR ��
	�o���A���g����������Ă��܂��B
============================================================================*/
#ifndef _In_z_
#define _In_z_
#endif

/*==========================================================================
	_Inout_z_ ����

	�ύX����� null �ŏI��镶���z��ւ̃|�C���^�[�B�Ăяo���̑O��ŗL����
	����K�v������܂����A�l���ύX���ꂽ�ƌ��Ȃ���܂��BNull �I�[�����͈ړ�
	�ł��܂����A���� null �I�[�����܂ł̗v�f�݂̂��A�N�Z�X�����\����
	����܂��B
============================================================================*/
#ifndef _Inout_z_
#define _Inout_z_
#endif

/*==========================================================================
	_In_reads_�A_In_reads_bytes_ ����

	�֐��ɂ���ēǂݎ����z��ւ̃|�C���^�[�B�z��̓T�C�Y s �̗v�f�ł���A
	���ׂėL���ł���K�v������܂��B

	_bytes_ �o���A���g�́A�v�f�̑���ɃT�C�Y���o�C�g�P�ʂŎ����܂��B
	���̃o���A���g�́A�T�C�Y��v�f�Ƃ��ĕ\���ł��Ȃ��ꍇ�ɂ̂ݎg�p���Ă��������B
	���Ƃ��΁Achar ������ł́A���l�̊֐��� wchar_t ���g�p����ꍇ�̂݁A
	_bytes_ �o���A���g���g�p����܂��B
============================================================================*/
#ifndef _In_reads_
#define _In_reads_
#endif
#ifndef _In_reads_bytes_
#define _In_reads_bytes_
#endif

/*==========================================================================
	_In_reads_z_ ����

	Null �ŏI�����A���m�̃T�C�Y�����z��ւ̃|�C���^�[�BNull �I�[�����܂ł�
	�v�f (�܂��� null �I�[�������Ȃ��ꍇ�� s) �́A���O��ԂŗL���ł���K�v��
	����܂��B�T�C�Y���o�C�g�P�ʂł킩���Ă���ꍇ�́A�v�f�̃T�C�Y�� s ��
	�X�P�[�����O���܂��B
============================================================================*/
#ifndef _In_reads_z_
#define _In_reads_z_
#endif

/*==========================================================================
	_In_reads_or_z_ ����

	Null �ŏI��邩�A���m�̃T�C�Y (�܂��͂��̗���) �����z��ւ̃|�C���^�[�B
	Null �I�[�����܂ł̗v�f (�܂��� null �I�[�������Ȃ��ꍇ�� s) �́A���O���
	�ŗL���ł���K�v������܂��B�T�C�Y���o�C�g�P�ʂł킩���Ă���ꍇ�́A
	�v�f�̃T�C�Y�� s ���X�P�[�����O���܂��B(strn �t�@�~���Ŏg�p����܂�)�B
============================================================================*/
#ifndef _In_reads_or_z_
#define _In_reads_or_z_
#endif

/*==========================================================================
	_Out_writes_�A_Out_writes_bytes_ ����

	�֐��ɂ���ď������܂�� s �v�f�̔z�� (resp) �ւ̃|�C���^�[�B
	�z��v�f�́A���O��ԂŗL���ł���K�v�͂Ȃ��A�����ԂŗL���ȗv�f�̐���
	�w�肳��Ă��܂���B�p�����[�^�[�̌^�ɒ��߂�����ꍇ�́A�����ԂœK�p
	����܂��B
============================================================================*/
#ifndef _Out_writes_
#define _Out_writes_
#endif
#ifndef _Out_writes_bytes_
#define _Out_writes_bytes_
#endif

/*==========================================================================
	_Out_writes_z_ ����

	s �v�f�̔z��ւ̃|�C���^�[�B�v�f�́A���O��ԂŗL���ł���K�v�͂���܂���B
	����̏�Ԃł́Anull �I�[���� (���݂���K�v������) ����ėv�f���L����
	����K�v������܂��B�T�C�Y���o�C�g�P�ʂł킩���Ă���ꍇ�́A�v�f��
	�T�C�Y�� s ���X�P�[�����O���܂��B
============================================================================*/
#ifndef _Out_writes_z_
#define _Out_writes_z_
#endif

/*==========================================================================
	_Inout_updates_�A_Inout_updates_bytes_ ����

	�z��ւ̃|�C���^�[�B�֐����œǂݎ��Ə������݂̗������s���܂��B
	����̓T�C�Y s �̗v�f�ł���A���O��ԂƎ����ԂŗL���ł��B

	_bytes_ �o���A���g�́A�v�f�̑���ɃT�C�Y���o�C�g�P�ʂŎ����܂��B
	���̃o���A���g�́A�T�C�Y��v�f�Ƃ��ĕ\���ł��Ȃ��ꍇ�ɂ̂ݎg�p���Ă��������B
	���Ƃ��΁Achar ������ł́A���l�̊֐��� wchar_t ���g�p����ꍇ�̂݁A
	_bytes_ �o���A���g���g�p����܂��B
============================================================================*/
#ifndef _Inout_updates_
#define _Inout_updates_
#endif
#ifndef _Inout_updates_bytes_
#define _Inout_updates_bytes_
#endif

/*==========================================================================
	_Inout_updates_z_ ����

	Null �ŏI�����A���m�̃T�C�Y�����z��ւ̃|�C���^�[�B
	Null �I�[���� (���݂���K�v������) �܂ł̗v�f�́A���O��ԂƎ����Ԃ�
	�����ŗL���ł���K�v������܂��B�����Ԃ̒l�́A�O�̏�Ԃ̒l�Ƃ�
	�قȂ���̂ƌ��Ȃ���܂��Bnull �I�[�L���̈ʒu���܂݂܂��B
	�T�C�Y���o�C�g�P�ʂł킩���Ă���ꍇ�́A�v�f�̃T�C�Y�� s ��
	�X�P�[�����O���܂��B
============================================================================*/
#ifndef _Inout_updates_z_
#define _Inout_updates_z_
#endif

/*==========================================================================
	_Out_writes_to_�A_Out_writes_bytes_to_
	_Out_writes_all_�A_Out_writes_bytes_all_ ����

	s �v�f�̔z��ւ̃|�C���^�[�B�v�f�́A���O��ԂŗL���ł���K�v�͂���܂���B
	����̏�Ԃł́Ac-th �v�f�܂ł̗v�f���L���ł���K�v������܂��B
	���� _bytes_ �o���A���g�́A�T�C�Y���v�f���ł͂Ȃ��o�C�g�P�ʂƂ������Ƃ�
	�킩���Ă���ꍇ�Ɏg�p�ł��܂��B
============================================================================*/
#ifndef _Inout_updates_
#define _Inout_updates_
#endif
#ifndef _Inout_updates_bytes_
#define _Inout_updates_bytes_
#endif
#ifndef _Out_writes_all_
#define _Out_writes_all_
#endif
#ifndef _Inout_updates_bytes_
#define _Inout_updates_bytes_
#endif

/*==========================================================================
	_Inout_updates_to_�A_Inout_updates_bytes_to_ ����

	�z��ւ̃|�C���^�[�B���̔z��́A�֐��ɂ���ēǂݎ���A�������܂�܂��B
	����̓T�C�Y s �̗v�f�ł���A���ׂĂ����O��ԂŗL���ł���K�v������A
	c �v�f�͎����ԂŗL���ł���K�v������܂��B

	_bytes_ �o���A���g�́A�v�f�̑���ɃT�C�Y���o�C�g�P�ʂŎ����܂��B
	���̃o���A���g�́A�T�C�Y��v�f�Ƃ��ĕ\���ł��Ȃ��ꍇ�ɂ̂ݎg�p���Ă��������B
	���Ƃ��΁Achar ������ł́A���l�̊֐��� wchar_t ���g�p����ꍇ�̂݁A
	_bytes_ �o���A���g���g�p����܂��B
============================================================================*/
#ifndef _Inout_updates_to_
#define _Inout_updates_to_
#endif
#ifndef _Inout_updates_bytes_to_
#define _Inout_updates_bytes_to_
#endif

/*==========================================================================
	_Inout_updates_all_�A_Inout_updates_bytes_all_ ����

	�T�C�Y s �v�f�̊֐��ɂ���ēǂݎ���A�������܂��z��ւ̃|�C���^�[�B
	���̓����̂��̂Ƃ��Ē�`����܂��B

	_Inout_updates_to_(_Old_(s), _Old_(s)) _Inout_updates_bytes_to_(_Old_(s), _Old_(s))

	�܂�A�O�̏�Ԃɂ���ő� s �̃o�b�t�@�[�ɑ��݂��邷�ׂĂ̗v�f�́A
	���O��ԂƎ����ԂŗL���ł��B

	_bytes_ �o���A���g�́A�v�f�̑���ɃT�C�Y���o�C�g�P�ʂŎ����܂��B
	���̃o���A���g�́A�T�C�Y��v�f�Ƃ��ĕ\���ł��Ȃ��ꍇ�ɂ̂ݎg�p���Ă��������B
	���Ƃ��΁Achar ������ł́A���l�̊֐��� wchar_t ���g�p����ꍇ�̂݁A
	_bytes_ �o���A���g���g�p����܂��B
============================================================================*/
#ifndef _Inout_updates_all_
#define _Inout_updates_all_
#endif
#ifndef _Inout_updates_bytes_all_
#define _Inout_updates_bytes_all_
#endif

/*==========================================================================
	_In_reads_to_ptr_ ����

	p - _Curr_ (�܂�Ap ���� _Curr_ �����Z) ���L���Ȏ��ł���A
	�z��ւ̃|�C���^�[�Bp ���O�̗v�f�́A���O��ԂŗL������K�v������܂��B
============================================================================*/
#ifndef _In_reads_to_ptr_
#define _In_reads_to_ptr_
#endif

/*==========================================================================
	_In_reads_to_ptr_z_ ����

	�� p - _Curr_ (�܂�Ap ���� _Curr_ �����Z) ���L���Ȏ��ł���ANull ��
	�I���z��ւ̃|�C���^�[�B
	p ���O�̗v�f�́A���O��ԂŗL������K�v������܂��B
============================================================================*/
#ifndef _In_reads_to_ptr_z_
#define _In_reads_to_ptr_z_
#endif

/*==========================================================================
	_Out_writes_to_ptr_ ����

	p - _Curr_ (�܂�Ap ���� _Curr_ �����Z) ���L���Ȏ��ł���A
	�z��ւ̃|�C���^�[�Bp �̑O�̗v�f�́A���O��ԂŗL���ł���K�v�͂Ȃ��A
	�����ԂŗL���ł���K�v������܂��B
============================================================================*/
#ifndef _Out_writes_to_ptr_
#define _Out_writes_to_ptr_
#endif

/*==========================================================================
	_Out_writes_to_ptr_z_ ����

	p - _Curr_ (�܂�Ap ���� _Curr_ �����Z) ���L���Ȏ��ł���ANull ��
	�I���z��ւ̃|�C���^�[�Bp �̑O�̗v�f�́A���O��ԂŗL���ł���K�v�͂Ȃ��A
	�����ԂŗL���ł���K�v������܂��B
============================================================================*/
#ifndef _Out_writes_to_ptr_z_
#define _Out_writes_to_ptr_z_
#endif

/*==========================================================================
	�ȗ��\�ȃ|�C���^�[ �p�����[�^�[

	�|�C���^�[ �p�����[�^�[���߂� _opt_ ���܂܂�Ă���ꍇ�́A�p�����[�^�[��
	null �ł���\�������邱�Ƃ������܂��B����ȊO�̏ꍇ�A���߂́A_opt_ ��
	�܂܂Ȃ��o�[�W�����Ɠ����悤�ɓ��삵�܂��B�|�C���^�[ �p�����[�^�[���߂�
	_opt_ �o���A���g�̈ꗗ�����Ɏ����܂��B
============================================================================*/
#ifndef _In_opt_
#define _In_opt_
#endif
#ifndef _Out_opt_
#define _Out_opt_
#endif
#ifndef _Inout_opt_
#define _Inout_opt_
#endif
#ifndef _In_opt_z_
#define _In_opt_z_
#endif
#ifndef _Inout_opt_z_
#define _Inout_opt_z_
#endif
#ifndef _In_reads_opt_
#define _In_reads_opt_
#endif
#ifndef _In_reads_bytes_opt_
#define _In_reads_bytes_opt_
#endif
#ifndef _In_reads_opt_z_
#define _In_reads_opt_z_
#endif
#ifndef _Out_writes_opt_
#define _Out_writes_opt_
#endif
#ifndef _Out_writes_opt_z_
#define _Out_writes_opt_z_
#endif
#ifndef _Inout_updates_opt_
#define _Inout_updates_opt_
#endif
#ifndef _Inout_updates_bytes_opt_
#define _Inout_updates_bytes_opt_
#endif
#ifndef _Inout_updates_opt_z_
#define _Inout_updates_opt_z_
#endif
#ifndef _Out_writes_to_opt_
#define _Out_writes_to_opt_
#endif
#ifndef _Out_writes_bytes_to_opt_
#define _Out_writes_bytes_to_opt_
#endif
#ifndef _Out_writes_all_opt_
#define _Out_writes_all_opt_
#endif
#ifndef _Out_writes_bytes_all_opt_
#define _Out_writes_bytes_all_opt_
#endif
#ifndef _Inout_updates_to_opt_
#define _Inout_updates_to_opt_
#endif
#ifndef _Inout_updates_bytes_to_opt_
#define _Inout_updates_bytes_to_opt_
#endif
#ifndef _Inout_updates_all_opt_
#define _Inout_updates_all_opt_
#endif
#ifndef _Inout_updates_bytes_all_opt_
#define _Inout_updates_bytes_all_opt_
#endif
#ifndef _In_reads_to_ptr_opt_
#define _In_reads_to_ptr_opt_
#endif
#ifndef _In_reads_to_ptr_opt_z_
#define _In_reads_to_ptr_opt_z_
#endif
#ifndef _Out_writes_to_ptr_opt_
#define _Out_writes_to_ptr_opt_
#endif
#ifndef _Out_writes_to_ptr_opt_z_
#define _Out_writes_to_ptr_opt_z_
#endif

/*==========================================================================
	�o�͂̃|�C���^�[ �p�����[�^�[

	�o�̓|�C���^�[ �p�����[�^�[�ɂ́A�p�����[�^�[�ƃ|�C���g��̈ʒu�𖾊m��
	��ʂ��邽�߂̓��ʂȕ\�L���K�v�ł��B
============================================================================*/

/*==========================================================================
	_Outptr_ ����

	�p�����[�^�[�� null �ɂ��邱�Ƃ͂ł��܂���B�܂��Apost ��Ԃł́A
	�|�C���g��̏ꏊ�� null �ɂ��邱�Ƃ͂ł����A�L���ł���K�v������܂��B
============================================================================*/
#ifndef _Outptr_
#define _Outptr_
#endif

/*==========================================================================
	_Outptr_opt_ ����

	�p�����[�^�[�� null �ɂ��邱�Ƃ��ł��܂����Apost ��Ԃł́A�|�C���g���
	�ꏊ�� null �ɂ��邱�Ƃ͂ł����A�L���ł���K�v������܂��B
============================================================================*/
#ifndef _Outptr_opt_
#define _Outptr_opt_
#endif

/*==========================================================================
	_Outptr_result_maybenull_ ����

	�p�����[�^�[�� null �ɂ��邱�Ƃ͂ł��܂���B
	�܂��A��Ԃ������ʒu�� null �ɂ��邱�Ƃ��ł��܂��B
============================================================================*/
#ifndef _Outptr_result_maybenull_
#define _Outptr_result_maybenull_
#endif

/*==========================================================================
	_Outptr_opt_result_maybenull_ ����

	�p�����[�^�[�� null �ɂ��邱�Ƃ��ł��܂��B
	�܂��A��Ԃ������ʒu�� null �ɂ��邱�Ƃ��ł��܂��B

	���̕\�ł́A���߂̈Ӗ�������ɏC�����邽�߂ɁA���ߖ��ɒǉ��̕���������
	�}������Ă��܂��B���܂��܂ȕ���������� _z�A_COM_�A_buffer_�A
	_bytebuffer_�A����� _to_ �ł��B
============================================================================*/
#ifndef _Outptr_opt_result_maybenull_
#define _Outptr_opt_result_maybenull_
#endif

/*==========================================================================
	�Ԃ����|�C���^�[�ɂ� _Null_terminated_ ���߂�����܂��B
============================================================================*/
#ifndef _Outptr_result_z_
#define _Outptr_result_z_
#endif
#ifndef _Outptr_opt_result_z_
#define _Outptr_opt_result_z_
#endif
#ifndef _Outptr_result_maybenull_z_
#define _Outptr_result_maybenull_z_
#endif
#ifndef _Outptr_opt_result_maybenull_z_
#define _Outptr_opt_result_maybenull_z_
#endif

/*==========================================================================
	�Ԃ����|�C���^�[�ɂ� COM �Z�}���e�B�N�X������܂��B
	����́A�Ԃ��ꂽ�|�C���^�[�� null �ł��� _On_failure_ ���������
	�����R�ł��B
============================================================================*/
#ifndef _COM_Outptr_
#define _COM_Outptr_
#endif
#ifndef _COM_Outptr_opt_
#define _COM_Outptr_opt_
#endif
#ifndef _COM_Outptr_result_maybenull_
#define _COM_Outptr_result_maybenull_
#endif
#ifndef _COM_Outptr_opt_result_maybenull_
#define _COM_Outptr_opt_result_maybenull_
#endif

/*==========================================================================
	�Ԃ����|�C���^�[�́A�T�C�Y s �v�f�܂��̓o�C�g�̗L���ȃo�b�t�@�[��
	�w���܂��B
============================================================================*/
#ifndef _Outptr_result_buffer_
#define _Outptr_result_buffer_
#endif
#ifndef _Outptr_result_bytebuffer_
#define _Outptr_result_bytebuffer_
#endif
#ifndef _Outptr_opt_result_buffer_
#define _Outptr_opt_result_buffer_
#endif
#ifndef _Outptr_opt_result_bytebuffer_
#define _Outptr_opt_result_bytebuffer_
#endif

/*==========================================================================
	�Ԃ����|�C���^�[�́A�T�C�Y s �v�f�܂��̓o�C�g�̃o�b�t�@�[���w���܂��B
	���̃o�b�t�@�[�̍ŏ��� c �͗L���ł��B
============================================================================*/
#ifndef _Outptr_result_buffer_to_
#define _Outptr_result_buffer_to_
#endif
#ifndef _Outptr_result_bytebuffer_to_
#define _Outptr_result_bytebuffer_to_
#endif
#ifndef _Outptr_opt_result_buffer_to_
#define _Outptr_opt_result_buffer_to_
#endif
#ifndef _Outptr_opt_result_bytebuffer_to_
#define _Outptr_opt_result_bytebuffer_to_
#endif

/*==========================================================================
	���̒��߂�ύX���܂��B�֐������s�����ꍇ�A���ʂ� null �ɐݒ肳��܂��B
============================================================================*/
#ifndef _Result_nullonfailure_
#define _Result_nullonfailure_
#endif

/*==========================================================================
	���̒��߂�ύX���܂��B�֐������s�����ꍇ�A���ʂ� 0 �ɐݒ肳��܂��B
============================================================================*/
#ifndef _Result_zeroonfailure_
#define _Result_zeroonfailure_
#endif

/*==========================================================================
	�Ԃ��ꂽ�|�C���^�[�́A�֐������������ꍇ�͗L���ȃo�b�t�@�[���w���A
	�֐������s�����ꍇ�� null ���w���܂��B
	���̒��߂́A�ȗ��\�łȂ��p�����[�^�[�p�ł��B
============================================================================*/
#ifndef _Outptr_result_nullonfailure_
#define _Outptr_result_nullonfailure_
#endif
#ifndef _Outptr_opt_result_nullonfailure_
#define _Outptr_opt_result_nullonfailure_
#endif

/*==========================================================================
	�Ԃ��ꂽ�|�C���^�[�́A�֐������������ꍇ�͗L���ȃo�b�t�@�[���w���A
	�֐������s�����ꍇ�� null ���w���܂��B
	���̒��߂́A�Q�ƃp�����[�^�[�p�ł��B
============================================================================*/
#ifndef _Outref_result_nullonfailure_
#define _Outref_result_nullonfailure_
#endif


#ifndef _Post_writable_byte_size_
#define _Post_writable_byte_size_
#endif

#ifndef _Ret_z_
#define _Ret_z_
#endif

#ifndef _Check_return_
#define _Check_return_
#endif


#endif
#endif
