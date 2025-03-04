# Lab Implementation Report

## 3. find Program

### Workflow

I started by figuring out how to traverse directories recursively:

- First, I open the specified directory using `open()` and loop through entries with `read()`
- I made sure to skip `.` and `..` entries (I learned this the hard way after creating an infinite loop in my first version!)
- Then I recursively call `find` on any subdirectories I encounter

For building the paths:
- I linked the current directory path with each entry name (like `a/b` becomes `a/b/c` for entry `c`)
- Used a 512 byte buffer for storing paths, which I think is enough for xv6's filesystem

For name matching:
- Just compared each entry's name to the target filename using `strcmp()`
- When theres a match, I print the full path

### Abandoned Approaches

I initially tried an different approach with a stack to avoid recursion because I was worried about stack overflow, but honestly the recursive approach was so much cleaner and simpler. Since xv6 doesn't have huge directory structures anyway, I decided recursion would be fine.

I also thought about using dynamic buffers for the paths, but then remembered xv6 dosent have `malloc` or `free`.

### Key Challenges

Buffer overflow was a big worry - if paths got longer than 512 bytes, I just shorten them and added a warning message. I don't think this will be a problem in practice since xv6 rarely has super long paths.

I also had a nasty bug where I wasn't closing my directories properly after recursion... this took me a while to figure out! I had to add a bunch of `printf` statements to debug why it was bugging out.

### 3.1. Partial Credit Considerations

- I had that weird recursion bug with `.` and `..` entries at first
- My path truncation solution isn't ideal but seems reasonable enough

## 4. xargs Program

### Workflow

For parsing the input:
- Read stdin character by character until I hit a newline or a deadend
- Built up a line buffer and split it into tokens based on spaces/tabs

For executing commands:
- Combined the command from the user (like `echo bye`) with stdin
- Used a static array of size `MAXARG` to hold all the arguments
- For each line, I created a child process to run the command with `exec`
- Parent waited for the child to finish using `wait()`

### Abandoned Approaches

I initially tried to implement the full Unix `xargs` behavior where it groups multiple lines into a single command execution, but that got complicated fast with managing partial lines and error cases. I simplified it back to the simpler approach of one execution per line.

My first implementation used `read(0, buf, sizeof(buf))` to read entire chunks, but I ran into issues with partial inputs, especially when piping commands together. Switching to character-by-character reading was slower but more reliable.

I thought about supporting quoted arguments (like `"hello world"`) but that seemed beyond what was required for.

### Key Challenges 

I had to limit arguments to `MAXARG` (32) to avoid memory issues. If there were too many tokens on a line, I just threw an error.

Handling edge cases like empty lines was tricky - I decided to just skip them rather than calling `exec` with potentially invalid arguments.

## 4.1 Partial Credit Considerations

- If the input ends without a newline character, I still process the final line which I think is the right behavior
- I didn't implement any signal handling, so if you interrupt commands with Ctrl+C you might get orphaned processes


There are defenitely tradeoffs:
- Almost no error recovery (if `exec` fails, the whole thing's cooked)
- No support for advanced filesystem features 

I ran all the tests in the test suite and they passed, so I'm pretty confident the implementation meets the requirements despite some rough edges.
