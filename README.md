# wordl

## Prerequisite

To compile this code, you will need to install make and c++ 17 (for Windows you will need MinGW 11.2, and for MacOS you will need Xcode Command Line Tools). You can install both of these on Windows using [Chocolatey](https://chocolatey.org). The Xcode Command Line Tools on MacOS will install both of them. It is recommended to also have [git](https://git-scm.com) (which is also included in the Xcode Command Line Tools) installed. You can get a copy of this reposity by cloning it using

```bash
git clone https://github.com/gwstaten/wordl
```

or just download it through the [homepage](https://github.com/gwstaten/wordl), click on Code -> Download Zip, or download it through [this link](https://github.com/gwstaten/wordl/archive/refs/heads/main.zip).

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
Ultra hard mode, hard mode, or normal (u, h, n)?
```

Wordl at this point gives you three types of hardmode. Normal is what most people will be using. Hardmode forces wordl to use the information given. Ultra hardmode is similar to hardmode except it forces wordl to place yellow letters in different positions. At this point, you should have something similar to

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)?
```

Each search method uses a different algorithm and can be found [here](#search-modes). The recommended search method is 2, though the three have not been thoroughly tested and you can experiment with each one.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
```

Here, most of you would want to type `g` or `a`, so you can get to having the bot solve the wordl or rate words. `Find best` will try to find the best word out of the entire list. This can take anywhere from 1 minute to several hours. The speed of this can be greatly increased with more threads, though more than 8 threads typically will not increase the speed. If your interested, you can also type `l` to see all the words the code will search through and count as valid words.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess:
```

At this point, you can start inputting the results you get from entering words into your wordle website. You can start with any word you would like to enter. To enter the word you guessed, type the word you guessed and press enter when it prompts with `guess:`. After that, nothing will happen as it is waiting for the result of the guess. The result of the guess is formatted in 0s, 1s, and 2s. 0 represents that the letter is not in the word, 1 represents that the letter is in the word, just not in the correct position, and 2 represents that the letter is in the word and the correct position. 0 for grey, 1 for yellow, and 2 for green. **Parallel Wordle Note: In parallel wordle, you would enter the result of each wordle board one after another.** At this point, you should have something similar to

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: trace
rating: 00000
There are now 246 answers remaining

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
```

At this point, typing `f` would result in the program finding the best guess. The speed of the program will depend on the input you gave. Typing `g` will bypass that step, and give prompt you for another guess you inputted. You can also type `l` to find a list of words that are still valid with the information you inputted. You can also type `a` to receive a rating for a word or word set in the current situation. Typing `w` will find the worst guess for the current scenario (it is only recommended to use the feature while in hard mode). This will repeat until the program has one possible solution left, at this point typing `e` will exit the code or you can type `r` to begin a new game with the current settings. If you encounter any issues running the code submit a [bug report](https://github.com/gwstaten/wordl/issues/new/choose). You can also look at [Example Output](#example-output) to see what the code running would look like.

## Word Lists

| WordList Name | Description | Website |
|---------------|-------------|---------|
| 4-11(2) | For words that are length 4-11 |  [https://hellowordl.net](https://hellowordl.net)|
| 2-25 | For words that are length 2-25 | [https://gwstaten.github.io](https://gwstaten.github.io) |
| nytimes(2) | Used for regular wordle | [https://www.nytimes.com/games/wordle/index.html](https://www.nytimes.com/games/wordle/index.html) |
| nytimesold(2) | Used for wordle clones with the old nytimes list | ... |
| food | Used only for foodle | [https://food-le.co](https://food-le.co) |
| shard | Used only for shardle | [https://shardle.17thshard.com/](https://shardle.17thshard.com/) |
| german | Used only for Wördl | [https://wordle.at/](https://wordle.at/) |
| obscordle | Used for obscordle | [https://wordreaper.github.io/wordle/](https://wordreaper.github.io/wordle/) |
| dordle | Used for dordle | [https://zaratustra.itch.io/dordle](https://zaratustra.itch.io/dordle) |
| quordle | Used for quordle | [https://www.quordle.com](https://www.quordle.com) |

To add your own word list, add a file to the wordlists directory that contains the possible answers for the particular wordle game the wordlist is for. (separated by single spaces or line breaks) If you would like to, you can also add another file whose name is & before the name of the first file that contains the allowed guesses for that particular wordle game (this will only produce noticeably better results if the answer list is significantly more limited than the guess list). If you want to officially add a wordlist from a specific wordle website, create a new issue with template feature request at the [wordl github issue page](https://github.com/gwstaten/wordl/issues/new/choose)

## Search Modes

| Search mode | Description |
|-------------|-------------|
| 1 | Finds the word that narrows down to the least number of remaining possibilities on average (aka n average) |
| 2 | Finds the word that splits the word set up into the most separate groups (aka 1/n sum) |
| 3 | Finds the word with the best chance of getting the word on the guess after the current guess (aka n=1 sum) |
| 4 | Finds the word that narrows down to the least number of remaining possibilities in the worst case scenario (aka max(n)) |
| 5 | Finds the word that gets the most bits of information on average (aka logBASE(n/p,0.5) average) |
| 6 | Finds the word that gets the most greens on average |

## Example Program Run

```bash
$ ./wordle
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: trace
rating: 00000
There are now 246 answers remaining

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f

Best guess: spiny \ slimy - score of 69
Best of answers: spiny \ slimy - score of 69

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: slimy
rating: 10200
There are now 2 answers remaining

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f
Only 2 possibility remaining: using
whisk

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: whisk
00210
There are now 1 answers remaining

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f
Only 1 possibility remaining: using

Find best (f), find worst (w), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? e
$
```

## For Advanced Users

### CMDL

Instead of going through the interface, you can use the one-line version. The one-line version consists of arguments, commands, and filters and always exists on completetion. The arguments always begin with a hyphen (`-`) and is assigned a value using the equal sign. If the argument requires a space, you will need to wrap it in quotation marks. Arguments are the ones you go through first in the interface version. If the inputted argument is invalid, you will need to go through the interface version for that specific argument. Commands are similar to the ones in the interface. Instead of a single character, they are the entire word of the command. Commands do not begin with anything in particular. Filters are guesses and ratings for the findbest, filter, and list commands, and are words for the filter command.

### CMDL Info

#### CMDL Usage

```bash
./wordl [ARGUMENTS] <COMMAND> (FILTERS)
```

#### CMDL Arguments

| Name | Description |
| ---- | ----------- |
| -hardmode | Sets the hardmode |
| -parallel | Sets the number of parallel wordles |
| -searchmode | Sets the searchmode |
| -threads | Sets the number of threads |
| -wordlist | Sets the wordlist |

```bash
-<ARGUMENT>=(VALUE)
```

#### CMDL Commands

| Name | Description |
| ---- | ----------- |
| filter | Shows amount of remaining words based on inputted guesses and ratings. Seperate guesses and ratings with spaces |
| findbest | Finds best words based on inputted guesses and ratings. Seperate guesses with spaces |
| list | Lists remaining possible words based on inputted guesses and ratings. Seperate guesses with spaces |
| rate | Rates the inputted words. Seperate words with spaces |

### CMDL Example Run

```bash
$ ./wordl -threads=4 -wordlist=nytimes2 -parallel=1 -hardmode=n -searchmode=2 findbest trace 00000 jazzy 00001

Wordlist initialized with 2309 answers and an additional 12972 guesses

There are now 34 answers remaining

There are now 12 answers remaining


Best guess: plied \ lownd \ lovie \ lound \ loped \ loids \ lodge \ livor \ lidos \ blond ... and more - score of 12
Best of answers: vinyl \ synod \ nylon \ lying \ dying - score of 10

$
```
