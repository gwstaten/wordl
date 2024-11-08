# wordl

A command-line tool that aims to solve wordle and wordle variants as efficiently as possible.

![Image of `wordl` in-use](./resources/wordl.png)

## Prerequisite

To compile this code, you will need to install make and c++ 17 (for Windows you will need MinGW 11.2, and for MacOS you will need Xcode Command Line Tools). You can install both of these on Windows using [Chocolatey](https://chocolatey.org). The Xcode Command Line Tools on MacOS will install both of them. It is recommended to also have [git](https://git-scm.com) (which is also included in the Xcode Command Line Tools) installed. You can get a copy of this repository by cloning it using

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

If you downloaded it through the homepage, you can redownload the repository. Then you can recompile the code using [the instructions above](#compile).

## Usage

For learning how to use `wordl` and extended documentation, see [USAGE.md](./USAGE.md).
