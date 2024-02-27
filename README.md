# Purpose
Multithreaded application that reads lines from input file, reverses each line, then outputs to destination file.

# How to run
Run with Powershell using `./run.ps1`

# Design
First the input file size is determined, then the file is split into equal pieces.
For each section in the file, a thread will read the section line-by-line and output transformed lines in the appropriate output file section.
The first line of each section, except for the first, is discarded and the output position is incremented to move past the rest of the line. This is because that first line of each section will be filled out by the thread processing the section before it.

## Future improvements
Processing line-by-line is most likely less efficient than doing so in chunks. Each processor could read a chunk of data, then reverse all lines in it, then output it, and repeat until its section of the file is completed.
More robust handling of newlines. Currently, we assume a consistent newline structure (Windows). The `reverse_lines` function could be modified to account for mixed types of newline functions in the input file.
CLI arguments could be added instead of hardcoded values.
Both functional tests and end-to-end tests could be implemented to ensure edge cases are not broken with further updates.