; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-unknown-unknown -mattr=+sse2 | FileCheck %s --check-prefix=CHECK --check-prefix=X86
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -mattr=+sse2 | FileCheck %s --check-prefix=CHECK --check-prefix=X64

define i32 @extractelt_undef_insertelt(i32 %x, i32 %y) {
; X86-LABEL: extractelt_undef_insertelt:
; X86:       # %bb.0:
; X86-NEXT:    retl
;
; X64-LABEL: extractelt_undef_insertelt:
; X64:       # %bb.0:
; X64-NEXT:    retq
  %b = insertelement <4 x i32> zeroinitializer, i32 %x, i64 3
  %c = icmp uge i32 %y, %y
  %d = extractelement <4 x i32> %b, i1 %c
  ret i32 %d
}

