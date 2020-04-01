define dso_local i8 @main() #0 {
  %1 = alloca i8, align 1
  store i8 1, i8* %1, align 1
  ret i8 105
}
