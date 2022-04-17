# wordl

## Prerequisite

To compile this code, you will need to install make and c++ (17). It is recommended to also have [git](https://git-scm.com) installed. You will also need to clone this entire repository, using

```bash
git clone https://github.com/gwstaten/wordl
```

or just download it through the [homepage](https://github.com/gwstaten/wordl), click on Code -> Download Zip or through [here](https://github.com/gwstaten/wordl/archive/refs/heads/main.zip).

### Compile

To compile `wordl`, run

```bash
make
```

in the overall directory. This will produce a `bin` folder which is then used to create the executable. The executable is named `wordl`.

### Update

If you downloaded this repository using git clone, update your local version of this repository using

```bash
git pull origin
```

If you downloaded it through the homepage, you can redownload the repository. Then you can recompile the code using [#compile](#compile).

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
Search mode (1, 2, 3, 4)?
```

Each search method uses a different algorithm and can be found [here](#search-modes). The recommended search method is 2, though the three have not been thoroughly tested and you can experiment with each one.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n) n
Search mode (1, 2, 3, 4)? 2

Wordlist initialized with 2309 answers and an additional 2309 guesses

Find best (f), list (l), guess (g), or exit (e)?
```

Here, most of you would want to type `g`, so you can get to having the bot solve the wordl. But for those of you interested in `Find best`, the code will try to find the best word out of the entire list. This can take anywhere from 1 minute to several hours. The speed of this can be greatly increased with more threads, though more than 8 threads typically will not increase the speed. If your interested, you can also type `l` to see all the words the code will search through and count as valid words.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n) n
Search mode (1, 2, 3, 4)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), or exit (e)? g

guess:
```

At this point, you can start inputting the results you get from entering words into your wordle website. You can start with any word you would like to enter. To enter the word you guessed, type the word you guessed and press enter when it prompts with `guess:`. After that, nothing will happen as it is waiting for the result of the guess. The result of the guess is formatted in 0s, 1s, and 2s. 0 represents that the letter is not in the word, 1 represents that the letter is in the word, just not in the correct position, and 2 represents that the letter is in the word and the correct position. 0 for grey, 1 for yellow, and 2 for green. **Parallel Wordle Note: In parallel wordle, you would enter the result of each wordle board one after another.** At this point, you should have something similar to

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode? (y / n) n
Search mode (1, 2, 3, 4)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), or exit (e)? g

guess: adieu
01001
There are now 30 answers remaining

Find best (f), list (l) or guess (g)?
```

At this point, typing `f` would result in the program finding the best guess. The speed of the program will depend on the input you gave. Typing `g` will bypass that step, and give prompt you for another guess you inputted. You can also type `l` to find a list of words that are still valid with the information you inputted. You can also type `a` to receive a rating for a word or word set in the current situation. Typing `w` will find the worst guess for the current scenario (it is only recommended to use the feature while in hard mode). This will repeat until the program has one possible solution left, at this point typing `e` will exit the code or you can type `r` to begin a new game with the current settings. Look through [Q&A](#qa) if you encounter any issues running the code. You can also look at [Example Output](#example-output) to see what the code running would look like.

## Word Lists

| WordList Name | Description | Website  |
|---------------|-------------|----------|
| 4-11(2) | For words that are length 4-11 |  [https://hellowordl.net](https://hellowordl.net)|
| 2-25 | For words that are length 2-25 | [https://gwstaten.github.io](https://gwstaten.github.io) |
| nytimes(2) | Used for regular wordle | [https://www.nytimes.com/games/wordle/index.html](https://www.nytimes.com/games/wordle/index.html) |
| nytimesOld(2) | Used for wordle clones with the old nytimes list |     |
| food | Used only for foodle | [https://food-le.co](https://food-le.co) |
| shard | Used only for shardle | [https://shardle.17thshard.com/](https://shardle.17thshard.com/) |
| german | Used only for Wördl | [https://wordle.at/](https://wordle.at/) |
| taylor5-8 | Used for Taylordle | [https://www.taylordle.com](https://www.taylordle.com) |
| obscordle | Used for obscordle | [https://wordreaper.github.io/wordle/](https://wordreaper.github.io/wordle/) |

To add your own word list, add a file to the wordlists directory that contains the possible answers for the particular wordle game the wordlist is for. (separated by single spaces or line breaks) If you would like to, you can also add another file whose name is & before the name of the first file that contains the allowed guesses for that particular wordle game (this will only produce noticeably better results if the answer list is significantly more limited than the guess list). If you want to officially add a wordlist from a specific wordle website, create a new issue with template feature request at the [wordl github issue page](https://github.com/gwstaten/wordl/issues)

## Search Modes

| Search mode | Description |
|---------------|-------------|
| 1 | Finds the word that narrows down to the least number of remaining possibilities on average (aka n average) |
| 2 | Finds the word that splits the word set up into the most separate groups (aka 1/n sum) |
| 3 | Finds the word with the best chance of getting the word on the guess after the current guess (aka n=1 sum) |
| 4 | Finds the word that narrows down to the least number of remaining possibilities in the worst case scenario (aka max(n)) |
| 5 | Finds the word that gets the most bits of information on average (aka logBASE(n/p,0.5) average) |

## Q&A

### Q1: I entered a wordlist when prompted, but nothing happened

A1: Make sure you have spelled the wordlist correctly, and chosen one from [wordlist](#word-lists). Note that 4-11 represents an integer between 4-11 (inclusive).

### Q2: The code exited when I entered `Number of parallel words`

A2: Ensure you have not inputted anything other than numbers.

### Q3: I received `libc++abi: terminating with uncaught exception of type std::out_of_range: basic_string` when I inputted my guess

A3: When entering your guess, make sure the guess is the correct word length. Also ensure that the result you inputted is also the correct length (it should be the same as the word).

### Q4: When I input my second (or x) guess, I get `segmentation fault` or `core dump`

A4: This will happen if you enter a guess that contradicts any of your previous guesses. Make sure you entered it correctly!

### Q5: What does it mean when the code says `Creating log directory x...`

A5: This code creates log files in order to optimize performance of the code, and making this code run faster. These log files are located entirely on your computer, in the directory `log` inside the directory in which you cloned this repository.

## Example Program Run

```bash
$ ./wordle
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Hard mode (y / n)? n
Search mode (1, 2, 3, 4)? 2

Wordlist initialized with 2309 answers and an additional 2309 guesses

Find best (f), list (l), guess (g), or exit (e)? g

guess: trace
01212
There are now 1 answers remaining

Find best (f), list (l), guess (g), or exit (e)? l
scare

Find best (f), list (l), guess (g), or exit (e)? e
$
```
