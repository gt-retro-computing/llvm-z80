; ModuleID = 'ext_load_i8_test.c'
source_filename = "ext_load_i8_test.c"
target datalayout = "e-m:e-p:16:8-i8:8-i16:8-i32:8-i64:8-a:0:8-n8-S8"
target triple = "z80-unknown-unknown"
; Function Attrs: nofree norecurse nounwind
define dso_local zeroext i8 @simple_arith() local_unnamed_addr #0 {
entry:
  %0 = load volatile i16, i16* inttoptr (i16 1 to i16*), align 1, !tbaa !2
  %1 = load volatile i16, i16* inttoptr (i16 1 to i16*), align 1, !tbaa !2
  %add = add i16 %1, %0
  %conv4 = trunc i16 %add to i8
  ret i8 %conv4
}

attributes #0 = { nofree norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{!"clang version 10.0.0 (git@github.com:gt-retro-computing/llvm-z80.git 56506138cb0de35a56f9793d72f83739e12b3c28)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
