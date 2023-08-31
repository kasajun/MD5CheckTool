/*　
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
	ポインター パラメーター

	次の表の注釈については、ポインター パラメーターに注釈を付けると、
	ポインターが null の場合、アナライザーはエラーを報告します。
	この注釈は、ポインターと、ポイントされている任意のデータ項目に適用されます。
============================================================================*/

/*==========================================================================
	_In_ 注釈

	スカラー、構造体、構造体へのポインター、およびなどの入力パラメーターに
	注釈を入力します。単純なスカラーでは、明示的に使用できます。 
	パラメーターは、事前状態で有効である必要があり、変更されません。
============================================================================*/
#ifndef _In_
#define _In_
#endif

/*==========================================================================
	_Out_ 注釈

	スカラー、構造体、構造体へのポインター、およびなどの出力パラメーターに
	注釈を記述します。値によって渡されるスカラーなど、値を返さない
	オブジェクトにこの注釈を適用しないでください。パラメーターは、事前状態
	では有効である必要はありませんが、事後状態で有効である必要があります。
============================================================================*/
#ifndef _Out_
#define _Out_
#endif

/*==========================================================================
	_Inout_ 注釈

	関数によって変更されるパラメーターに注釈を設定します。これは、事前状態と
	事後状態の両方で有効である必要がありますが、呼び出しの前後に異なる値が
	あると見なされます。変更可能な値に適用する必要があります。
============================================================================*/
#ifndef _Inout_
#define _Inout_
#endif

/*==========================================================================
	_In_z_ 注釈

	入力として使用される null で終わる文字列へのポインター。文字列は、
	事前状態で有効である必要があります。には、既に正しい注釈がある PSTR の
	バリアントが推奨されています。
============================================================================*/
#ifndef _In_z_
#define _In_z_
#endif

/*==========================================================================
	_Inout_z_ 注釈

	変更される null で終わる文字配列へのポインター。呼び出しの前後で有効で
	ある必要がありますが、値が変更されたと見なされます。Null 終端文字は移動
	できますが、元の null 終端文字までの要素のみがアクセスされる可能性が
	あります。
============================================================================*/
#ifndef _Inout_z_
#define _Inout_z_
#endif

/*==========================================================================
	_In_reads_、_In_reads_bytes_ 注釈

	関数によって読み取られる配列へのポインター。配列はサイズ s の要素であり、
	すべて有効である必要があります。

	_bytes_ バリアントは、要素の代わりにサイズをバイト単位で示します。
	このバリアントは、サイズを要素として表現できない場合にのみ使用してください。
	たとえば、char 文字列では、同様の関数が wchar_t を使用する場合のみ、
	_bytes_ バリアントが使用されます。
============================================================================*/
#ifndef _In_reads_
#define _In_reads_
#endif
#ifndef _In_reads_bytes_
#define _In_reads_bytes_
#endif

/*==========================================================================
	_In_reads_z_ 注釈

	Null で終了し、既知のサイズを持つ配列へのポインター。Null 終端文字までの
	要素 (または null 終端文字がない場合は s) は、事前状態で有効である必要が
	あります。サイズがバイト単位でわかっている場合は、要素のサイズで s を
	スケーリングします。
============================================================================*/
#ifndef _In_reads_z_
#define _In_reads_z_
#endif

/*==========================================================================
	_In_reads_or_z_ 注釈

	Null で終わるか、既知のサイズ (またはその両方) を持つ配列へのポインター。
	Null 終端文字までの要素 (または null 終端文字がない場合は s) は、事前状態
	で有効である必要があります。サイズがバイト単位でわかっている場合は、
	要素のサイズで s をスケーリングします。(strn ファミリで使用されます)。
============================================================================*/
#ifndef _In_reads_or_z_
#define _In_reads_or_z_
#endif

/*==========================================================================
	_Out_writes_、_Out_writes_bytes_ 注釈

	関数によって書き込まれる s 要素の配列 (resp) へのポインター。
	配列要素は、事前状態で有効である必要はなく、事後状態で有効な要素の数が
	指定されていません。パラメーターの型に注釈がある場合は、事後状態で適用
	されます。
============================================================================*/
#ifndef _Out_writes_
#define _Out_writes_
#endif
#ifndef _Out_writes_bytes_
#define _Out_writes_bytes_
#endif

/*==========================================================================
	_Out_writes_z_ 注釈

	s 要素の配列へのポインター。要素は、事前状態で有効である必要はありません。
	事後の状態では、null 終端文字 (存在する必要がある) を介して要素が有効で
	ある必要があります。サイズがバイト単位でわかっている場合は、要素の
	サイズで s をスケーリングします。
============================================================================*/
#ifndef _Out_writes_z_
#define _Out_writes_z_
#endif

/*==========================================================================
	_Inout_updates_、_Inout_updates_bytes_ 注釈

	配列へのポインター。関数内で読み取りと書き込みの両方を行います。
	これはサイズ s の要素であり、事前状態と事後状態で有効です。

	_bytes_ バリアントは、要素の代わりにサイズをバイト単位で示します。
	このバリアントは、サイズを要素として表現できない場合にのみ使用してください。
	たとえば、char 文字列では、同様の関数が wchar_t を使用する場合のみ、
	_bytes_ バリアントが使用されます。
============================================================================*/
#ifndef _Inout_updates_
#define _Inout_updates_
#endif
#ifndef _Inout_updates_bytes_
#define _Inout_updates_bytes_
#endif

/*==========================================================================
	_Inout_updates_z_ 注釈

	Null で終了し、既知のサイズを持つ配列へのポインター。
	Null 終端文字 (存在する必要がある) までの要素は、事前状態と事後状態の
	両方で有効である必要があります。事後状態の値は、前の状態の値とは
	異なるものと見なされます。null 終端記号の位置を含みます。
	サイズがバイト単位でわかっている場合は、要素のサイズで s を
	スケーリングします。
============================================================================*/
#ifndef _Inout_updates_z_
#define _Inout_updates_z_
#endif

/*==========================================================================
	_Out_writes_to_、_Out_writes_bytes_to_
	_Out_writes_all_、_Out_writes_bytes_all_ 注釈

	s 要素の配列へのポインター。要素は、事前状態で有効である必要はありません。
	事後の状態では、c-th 要素までの要素が有効である必要があります。
	この _bytes_ バリアントは、サイズが要素数ではなくバイト単位ということが
	わかっている場合に使用できます。
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
	_Inout_updates_to_、_Inout_updates_bytes_to_ 注釈

	配列へのポインター。この配列は、関数によって読み取られ、書き込まれます。
	これはサイズ s の要素であり、すべてが事前状態で有効である必要があり、
	c 要素は事後状態で有効である必要があります。

	_bytes_ バリアントは、要素の代わりにサイズをバイト単位で示します。
	このバリアントは、サイズを要素として表現できない場合にのみ使用してください。
	たとえば、char 文字列では、同様の関数が wchar_t を使用する場合のみ、
	_bytes_ バリアントが使用されます。
============================================================================*/
#ifndef _Inout_updates_to_
#define _Inout_updates_to_
#endif
#ifndef _Inout_updates_bytes_to_
#define _Inout_updates_bytes_to_
#endif

/*==========================================================================
	_Inout_updates_all_、_Inout_updates_bytes_all_ 注釈

	サイズ s 要素の関数によって読み取られ、書き込まれる配列へのポインター。
	次の同等のものとして定義されます。

	_Inout_updates_to_(_Old_(s), _Old_(s)) _Inout_updates_bytes_to_(_Old_(s), _Old_(s))

	つまり、前の状態にある最大 s のバッファーに存在するすべての要素は、
	事前状態と事後状態で有効です。

	_bytes_ バリアントは、要素の代わりにサイズをバイト単位で示します。
	このバリアントは、サイズを要素として表現できない場合にのみ使用してください。
	たとえば、char 文字列では、同様の関数が wchar_t を使用する場合のみ、
	_bytes_ バリアントが使用されます。
============================================================================*/
#ifndef _Inout_updates_all_
#define _Inout_updates_all_
#endif
#ifndef _Inout_updates_bytes_all_
#define _Inout_updates_bytes_all_
#endif

/*==========================================================================
	_In_reads_to_ptr_ 注釈

	p - _Curr_ (つまり、p から _Curr_ を減算) が有効な式である、
	配列へのポインター。p より前の要素は、事前状態で有効ある必要があります。
============================================================================*/
#ifndef _In_reads_to_ptr_
#define _In_reads_to_ptr_
#endif

/*==========================================================================
	_In_reads_to_ptr_z_ 注釈

	式 p - _Curr_ (つまり、p から _Curr_ を減算) が有効な式である、Null で
	終わる配列へのポインター。
	p より前の要素は、事前状態で有効ある必要があります。
============================================================================*/
#ifndef _In_reads_to_ptr_z_
#define _In_reads_to_ptr_z_
#endif

/*==========================================================================
	_Out_writes_to_ptr_ 注釈

	p - _Curr_ (つまり、p から _Curr_ を減算) が有効な式である、
	配列へのポインター。p の前の要素は、事前状態で有効である必要はなく、
	事後状態で有効である必要があります。
============================================================================*/
#ifndef _Out_writes_to_ptr_
#define _Out_writes_to_ptr_
#endif

/*==========================================================================
	_Out_writes_to_ptr_z_ 注釈

	p - _Curr_ (つまり、p から _Curr_ を減算) が有効な式である、Null で
	終わる配列へのポインター。p の前の要素は、事前状態で有効である必要はなく、
	事後状態で有効である必要があります。
============================================================================*/
#ifndef _Out_writes_to_ptr_z_
#define _Out_writes_to_ptr_z_
#endif

/*==========================================================================
	省略可能なポインター パラメーター

	ポインター パラメーター注釈に _opt_ が含まれている場合は、パラメーターが
	null である可能性があることを示します。それ以外の場合、注釈は、_opt_ を
	含まないバージョンと同じように動作します。ポインター パラメーター注釈の
	_opt_ バリアントの一覧を次に示します。
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
	出力のポインター パラメーター

	出力ポインター パラメーターには、パラメーターとポイント先の位置を明確に
	区別するための特別な表記が必要です。
============================================================================*/

/*==========================================================================
	_Outptr_ 注釈

	パラメーターを null にすることはできません。また、post 状態では、
	ポイント先の場所を null にすることはできず、有効である必要があります。
============================================================================*/
#ifndef _Outptr_
#define _Outptr_
#endif

/*==========================================================================
	_Outptr_opt_ 注釈

	パラメーターは null にすることができますが、post 状態では、ポイント先の
	場所を null にすることはできず、有効である必要があります。
============================================================================*/
#ifndef _Outptr_opt_
#define _Outptr_opt_
#endif

/*==========================================================================
	_Outptr_result_maybenull_ 注釈

	パラメーターを null にすることはできません。
	また、状態を示す位置を null にすることもできます。
============================================================================*/
#ifndef _Outptr_result_maybenull_
#define _Outptr_result_maybenull_
#endif

/*==========================================================================
	_Outptr_opt_result_maybenull_ 注釈

	パラメーターは null にすることができます。
	また、状態を示す位置を null にすることもできます。

	次の表では、注釈の意味をさらに修飾するために、注釈名に追加の部分文字列が
	挿入されています。さまざまな部分文字列は _z、_COM_、_buffer_、
	_bytebuffer_、および _to_ です。
============================================================================*/
#ifndef _Outptr_opt_result_maybenull_
#define _Outptr_opt_result_maybenull_
#endif

/*==========================================================================
	返されるポインターには _Null_terminated_ 注釈があります。
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
	返されるポインターには COM セマンティクスがあります。
	これは、返されたポインターが null である _On_failure_ 事後条件を
	持つ理由です。
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
	返されるポインターは、サイズ s 要素またはバイトの有効なバッファーを
	指します。
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
	返されるポインターは、サイズ s 要素またはバイトのバッファーを指します。
	このバッファーの最初の c は有効です。
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
	他の注釈を変更します。関数が失敗した場合、結果は null に設定されます。
============================================================================*/
#ifndef _Result_nullonfailure_
#define _Result_nullonfailure_
#endif

/*==========================================================================
	他の注釈を変更します。関数が失敗した場合、結果は 0 に設定されます。
============================================================================*/
#ifndef _Result_zeroonfailure_
#define _Result_zeroonfailure_
#endif

/*==========================================================================
	返されたポインターは、関数が成功した場合は有効なバッファーを指し、
	関数が失敗した場合は null を指します。
	この注釈は、省略可能でないパラメーター用です。
============================================================================*/
#ifndef _Outptr_result_nullonfailure_
#define _Outptr_result_nullonfailure_
#endif
#ifndef _Outptr_opt_result_nullonfailure_
#define _Outptr_opt_result_nullonfailure_
#endif

/*==========================================================================
	返されたポインターは、関数が成功した場合は有効なバッファーを指し、
	関数が失敗した場合は null を指します。
	この注釈は、参照パラメーター用です。
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
