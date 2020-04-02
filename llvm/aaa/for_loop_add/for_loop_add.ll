; ModuleID = 'for_loop_add.ll'
source_filename = "for_loop_add.ll"
target datalayout = "e-m:e-p:16:8-i8:8-i16:8-i32:8-i64:8-a:0:8-n8:16-S8"
target triple = "z80-unknown-unknown"
define dso_local zeroext i8 @main() {
  %1 = alloca i8, align 1
  %2 = alloca i8, align 1
  store i8 0, i8* %1, align 1
  store i8 0, i8* %2, align 1
  br label %3

3:                                                ; preds = %15, %0
  %4 = load i8, i8* %2, align 1
  %5 = zext i8 %4 to i32
  %6 = icmp slt i32 %5, 10
  br i1 %6, label %7, label %18

7:                                                ; preds = %3
  %8 = load i8, i8* %2, align 1
  %9 = add i8 %8, 1
  store i8 %9, i8* %2, align 1
  %10 = zext i8 %9 to i32
  %11 = load i8, i8* %1, align 1
  %12 = zext i8 %11 to i32
  %13 = add nsw i32 %12, %10
  %14 = trunc i32 %13 to i8
  store i8 %14, i8* %1, align 1
  br label %15

15:                                               ; preds = %7
  %16 = load i8, i8* %2, align 1
  %17 = add i8 %16, 1
  store i8 %17, i8* %2, align 1
  br label %3

18:                                               ; preds = %3
  %19 = load i8, i8* %1, align 1
  ret i8 %19
}
