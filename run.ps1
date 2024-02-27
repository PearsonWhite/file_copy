# Script to demonstrate the program.

"==== start input file ===="
Get-Content ./input.txt
"==== end input file ===="
"removing output files"
rm .\output_1.txt ;
rm .\output_2.txt ;
"removing executable"
rm ./a.exe ;
g++ --std=c++23 .\main.cpp 2>&1 | tee build.log ;
"compilation finished" ;
.\a.exe 2>&1 | tee out.log ;
$LASTEXITCODE ;
"input -> reverse -> output_1"
"==== start output_1 file ===="
Get-Content ./output_1.txt
"==== end output_1 file ===="

""
"input -> reverse -> output_1 -> reverse -> output_2"
""

"==== start output_2 file ===="
Get-Content ./output_2.txt
"==== end output_2 file ===="

""
"We expect input.txt and output_2.txt to be the same"
"Comparison result:"
$(Get-FileHash -Algorithm SHA1 -Path .\input.txt).Hash -eq $(Get-FileHash -Algorithm SHA1 -Path .\output_2.txt).Hash

""
"done"