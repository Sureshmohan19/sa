# 1. Compile .sa to LLVM IR
./sac ../examples/hello.sa > hello.ll

# 2. Compile LLVM IR to object file
/opt/homebrew/opt/llvm/bin/llc -filetype=obj -mtriple=arm64-apple-macos15.0 hello.ll -o hello.o

# 3. Link with runtime using native ld
ld hello.o runtime.o -o myprogram \
  -lSystem \
  -syslibroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk \
  -arch arm64 \
  -platform_version macos 15.0 15.0

# 4. Run!
./myprogram