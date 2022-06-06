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

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
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

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

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

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: trace
rating: 00000
There are now 246 answers remaining

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
```

At this point, typing `f` would result in the program finding the best guess. The speed of the program will depend on the input you gave. Typing `g` will bypass that step, and give prompt you for another guess you inputted. You can also type `l` to find a list of words that are still valid with the information you inputted. You can also type `a` to receive a rating for a word or word set in the current situation. This will repeat until the program has one possible solution left, at this point typing `e` will exit the code or you can type `r` to begin a new game with the current settings. If you encounter any issues running the code submit a [bug report](https://github.com/gwstaten/wordl/issues/new/choose). You can also look at [Example Output](#example-output) to see what the code running would look like.

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

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: trace
rating: 00000
There are now 246 answers remaining

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f

Best guess: spiny \ slimy - score of 69
Best of answers: spiny \ slimy - score of 69

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g

guess: slimy
rating: 10200
There are now 2 answers remaining

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f

Best Guess: using / whisk

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? g  

guess: whisk
rating: 00210
There are now 1 answers remaining

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f

Best Guess: using

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? e
$
```

## For Advanced Users

Note: Most of the features below are not recommended for regular users as the way to use CMDL is not explicitly stated. CMDL requires some level of understanding of the current state of the `wordl` code.

### CMDL

Instead of going through the interface, you can use the one-line version. The one-line version consists of arguments, commands, and filters and always exists on completetion. The arguments always begin with a hyphen (`-`) and is assigned a value using the equal sign. If the argument requires a space or a special character, you will need to wrap it in quotation marks. Arguments are the ones you go through first in the interface version. If the inputted argument is invalid, you will need to go through the interface version for that specific argument. You can also leave the end of the equal sign blank or not include the equal sign to leave the argument to the default values. `Prefix`, `uletter`, and `ufilterby` are the only argument that does not have a default value. Commands are similar to the ones in the interface. Instead of a single character, they are the entire word of the command. Commands do not begin with anything in particular. Filters are guesses and ratings for the findbest, filter, and list commands, and are words for the filter command.

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
| -prefix | Sets prefix words. Only for findbest |
| -uletter | Sets the number of unique letters |
| -ufilterby | Sets filter mode. Only for ufilter |

```bash
-<ARGUMENT>=(VALUE)
```

##### Default CMDL Arguments

| Name | Default |
| ---- | ------- |
| -hardmode | n |
| -parallel | 1 |
| -searchmode | 2 |
| -threads | 4 |
| -wordlist | nytimes2 |

#### CMDL Commands

| Name | Description |
| ---- | ----------- |
| filter | Shows amount of remaining words based on inputted guesses and ratings. Seperate guesses and ratings with spaces |
| findbest | Finds best words based on inputted guesses and ratings. Seperate guesses with spaces |
| list | Lists remaining possible words based on inputted guesses and ratings. Seperate guesses with spaces |
| rate | Rates the inputted words. Seperate words with spaces |
| ufilter | Filters the answer list based on the number of unique letters. Requires uletter and ufilterby arguments |

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

## Set finding and advanced searches

To search for sets using Wordl (this example will be set up for finding the best 2 word set for nytimes that only uses answers by the 1/n metric), first enter in the basic settings as described in [#usage](#usage)

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
```

(note that wordlist nytimes was used to make the search answers only) Then type `s` to access a settings editor. You first re-enter the search mode. The options for this can be found [here](#search-modes).

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? s

Search mode (1 - 6)? 2
Use prefex (y / n)?
```

You now tell the program whether you want to add a prefix or not. Prefix is used to find the best followups to a word or wordset, so in this case we will type `n`.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)?
```

You now tell the program if you would like the code to generate a file with a ranked list of all of the words or wordsets it checked, in this case I will type `y` to generate the file.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size?
```

Next you enter the size of the set you will be searching for. In this case, 2.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size? 2
Number of required unique for whole set (put 0 for no requirement)? 
```

Next you will enter a few things that will filter down what sets the code actually checks. These aren't required, but they will make the search a lot faster. The first filter is the number of required unique letters for the entire set. In this case, we know that the best 2 word set will have 10 unique letters, so we will enter 10.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size? 2
Number of required unique for whole set (put 0 for no requirement)? 10
Num of required unique for each step (0 for no requirement)?

```

The next prompt only appears if you entered 2 or greater for the Set size option. It will have you enter (set size - 1) numbers. In this case, only one number, which is the number of required unique letters for the first word of the set. For this we will enter 5, because only sets where the first word has 5 unique letters will it be possible to have 10 unique letters across the whole set, so there is no point in checking other first words. (In the case of a 3 word set, you would enter one number for the number of required unique letters in the first word, and then a number for the number of required unique letters between the first two words).

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size? 2
Number of required unique for whole set (put 0 for no requirement)? 10
Num of required unique for each step (0 for no requirement)? 
5
New best prints (y / n)? 
```

You are then prompted with whether or not you want the code to print new best statements. This is recommended for long searches so you get updates along the way, but is not required.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size? 2
Number of required unique for whole set (put 0 for no requirement)? 10
Num of required unique for each step (0 for no requirement)? 
5
New best prints (y / n)? y
Forced include letters (y / n)?
```

The final setting is whether or not we want to force include letters in the set. In some cases you may want to force include letters such as S, A, E, T, R, etc if you know that they are for sure going to be a part of the best set for the particular search. I won't do this in this case because it is a fairly small search. (If you did want to include this as a filter, you would enter `y` for this option and then enter all letters you want to be included as a single string such as `aestr`)

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size? 2
Number of required unique for whole set (put 0 for no requirement)? 10
Num of required unique for each step (0 for no requirement)? 
5
New best prints (y / n)? y
Forced include letters (y / n)? n

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
```

You now have all of the settings in place, and can now enter `f` to run the search. As the search runs, you will see prints such as the ones shown below if you enabled the new best prints option. (the number shown has been reduced to not clog up this page) It is expected to see a large burst of these messages when the search begins and then fewer and farther apart updates as the search continues. This is because each thread that is running will print one of these messages when it finds a new best for that thread. When the search completes it will print the best result(s) and then return to the main options prompt.

```bash
$ ./wordl
Number of threads? 4
Word list? nytimes
Number of parallel wordls? 1
Ultra hard mode, hard mode, or normal (u, h, n)? n
Search mode (1 - 6)? 2

Wordlist initialized with 2309 answers

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?

Search mode (1 - 6)? 2
Use prefex (y / n)? n
Print full rankings to file (y / n)? y
Set size? 2
Number of required unique for whole set (put 0 for no requirement)? 10
Num of required unique for each step (0 for no requirement)? 
5
New best prints (y / n)? y
Forced include letters (y / n)? n

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)? f
(Thread 4) slain truce 1038
(Thread 3) lunar spite 945
(Thread 2) droit lance 1024
(Thread 1) adult siren 951
(Thread 3) maple shirt 958
(Thread 1) agile snort 953
price slant \ crane spilt - score of 1070

Find best (f), list (l), guess (g), rate (a), restart with same settings (r), or exit (e)?
```

If the print full rankings to file option was enabled, a file will have been generated in the same directory as the executable file. These file(s) will contain every set that the code checked. In this case, a file name answersRating.txt was formed with 195,105 lines. The first and last 10 are shown below.

```bash
price slant 1070
crane spilt 1070
salon trice 1065
close train 1056
crane split 1051
cairn stole 1051
coast liner 1050
cried slant 1049
print scale 1040
slain truce 1038
...
jumpy vodka 323
epoxy quack 323
jumpy vixen 321
inbox quack 321
jumbo wacky 318
epoxy quick 315
buxom gawky 311
gawky squib 305
inbox jumpy 303
gawky jumbo 302
```
