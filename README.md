# wordl

## Prerequisite

To compile this code, you will need to install make and c++ (17).

### Compile

To compile `wordl`, run

```bash
make
```

in the overall directory. This will produce a `bin` folder which is then used to create the executable. The executable is named `wordl`.

## Usage

### Running wordl

To run `wordl` on Unix-like machines, run

```bash
./wordl
```

and on Windows, run

```bash
wordl
```

in the same directory the executable `wordl` file is in.

### Using wordl

When running wordl, you will be greeted with

```bash
$ ./wordl
Number of threads?
```

This is what the program will show each time you run it. This is prompting you to input the number of threads you are willing to allow the program to use. If you don't really understand it, use `4` as it will give the best performance. You can experiment with different numbers. Next, the code will show

```bash
$ ./wordl
Number of threads? 4
Word List?
```

When prompted with this, enter one of the [wordlist](#word-lists). For the next prompt, enter 1 if you are trying to solve for only one wordle, or the number of wordles you are solving at the same time (like [dordle](https://zaratustra.itch.io/dordle), [quordle](https://www.quordle.com), [octordle](https://octordle.com), etc). After you have finished that, you should have something similar to

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n)
```

You can then enter either `y` or `n` to say if you are playing in hard mode or not, saying y will make sure the solver always chooses a word that is still a possible answer, but will on average take a few more guesses to guess the word. At this point, you should have something similar to

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n) n
Find best (y) or guess (n)?

```

Here, most of you would want to type `n`, so you can get to having the bot solve the wordl. But for those of you interested in `Find best`, the code will try to find the best word out of the entire list. This can take anywhere from 1 minute to several hours. The speed of this can be greatly increased with more threads, though more than 8 threads typically will not increase the speed.

```bash
$ ./wordl
Word list? nytimes
Number of threads? 4
Number of parallel wordls? 1
Hard mode? (y / n) n
Find best (y) or guess (n)?
n

guess:
```

At this point, you can start inputting the results you get from entering words into your wordle website. You can start with any word you would like to enter. To enter the word you guessed, type the word you guessed and press enter when it prompts with `guess:`. After that, nothing will happen as it is waiting for the result of the guess. The result of the guess is formatted in 0s, 1s, and 2s. 0 represents that the letter is not in the word, 1 represents that the letter is in the word, just not in the correct position, and 2 represents that the letter is in the word and the correct position. 0 for grey, 1 for yellow, and 2 for green. **Parallel Wordle Note: In parallel wordle, you would enter the result of each wordle board one after another.** At this point, you should have something similar to

```bash
Number of threads? 4
$ ./wordl
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n) n
Find best (y) or guess (n)?
n

guess: adiou
01001
Find best (y) or another guess (n)?
```

At this point, typing `y` would result in the program finding the best guess. The speed of the program will depend on the input you gave. Typing `n` will bypass that step, and give prompt you for another guess you inputted. This will repeat until the program has one possible solution left, in which the program will cleanly exit. Look through [Q&A](#qa) if you encounter any issues running the code. You can also look at [Example Output](#example-output) to see what the code running would look like.

## Word Lists

| WordList Name | Description | Website  |
|---------------|-------------|----------|
| 4-11 | For words that are length 4-11 |  [https://hellowordl.net](https://hellowordl.net)|
| 2-25 | For words that are length 2-25 | [https://gwstaten.github.io](https://gwstaten.github.io) |
| nytimes(2) | Used for regular wordle | [https://www.nytimes.com/games/wordle/index.html](https://www.nytimes.com/games/wordle/index.html) |
| food | Used only for foodle | [https://food-le.co](https://food-le.co) |
| shard | Used only for shardle | [https://shardle.17thshard.com/](https://shardle.17thshard.com/) |
| german | Used only for Wördl | [https://wordle.at/](https://wordle.at/) |
| taylor5-8 | Used for Taylordle | [https://www.taylordle.com](https://www.taylordle.com) |

To add your own word list, add a file to the wordlists directory that contains the possible answers for the particular wordle game the wordlist is for. (separated by single spaces or line breaks) If you would like to, you can also add another file whose name is & before the name of the first file that contains the allowed guesses for that particular wordle game (this will only produce noticeably better results if the answer list is significantly more limited than the guess list). If you want to officially add a wordlist from a specific wordle website, create a new issue with template feature request at the [wordl github issue page](https://github.com/gwstaten/wordl/issues)

## Q&A

### Q1: I entered a wordlist when prompted, but nothing happened

A1: Make sure you have spelled the wordlist correctly, and chosen one from [wordlist](#word-lists). Note that 4-11 represents an integer between 4-11 (inclusive).

### Q2: The code exited when I entered `Number of parallel words`

A2: Ensure you have not inputted anything other than numbers.

### Q3: I received `libc++abi: terminating with uncaught exception of type std::out_of_range: basic_string` when I inputted my guess

A3: When entering your guess, make sure the guess is the correct word length. Also ensure that the result you inputted is also the correct length (it should be the same as the word).

### Q4: When I input my second (or x) guess, I get `segmentation fault` or `core dump`

A4: This will happen if you enter a guess that contradicts any of your previous guesses. Make sure you entered it correctly!

## Example Program Run

```bash
$ ./wordle
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n) n
Find best (y) or guess (y)?
n

guess: adiou
01001
Find best (y) or another guess (n)?
y
Best guess for 34 possibilities: dunce
Narrows down to 2.70588 possibilities on average


guess: dunce
22010
Only 1 possibility remaining: duchy

$
```

## Acknowledgements

- Steffen Schümann 03/20/2022 [filesystem](https://github.com/gulrak/filesystem/tree/e8a17c131952c86563ff0ba7e08896a4f2594216) ([filesystem.hpp](https://github.com/gwstaten/wordl/blob/main/include/filesystem.hpp)) v1.5.10
- Mega Limited 04/03/2022 [mingw-std-threads](https://github.com/meganz/mingw-std-threads/tree/7e2507915900f5589febf0d8972cd5c9c03191f2) ([mingw.thread.h](https://github.com/gwstaten/wordl/blob/threads/include/mingw.thread.h)) ([mingw.invoke.h](https://github.com/gwstaten/wordl/blob/threads/include/mingw.invoke.h)) v1.0.0
