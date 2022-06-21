![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

# List

This project is a task of the programming course by [Ilya Dedinsky (aka ded32)](https://github.com/ded32).

## General information

The standard capacity of list is 8 elements but it insreases automatically when needed. The capacity of list doesn't decreases at all. My list supprots graphic dump by Graphviz.

## Specific functions

There are some useful functions in the project. Firstly, there is a function **List_Linearisation ()**. It changes the order of elements in a way that the order starts being equal to the logical one. It means that every two elements that are logicaly neighbours located in the memory in a row. Secondly, you can find the physical index of an element (the index in array) from its logical index by using function **Logical_Num_To_Physical ()**.

## Build and run

**List** is released for Linux only.

**Step 1:** Clone this repository.
```bash
git clone git@github.com:KetchuppOfficial/List.git
cd List
```

**Step 2:** Clone submodule.
```bash
git submodule init
git submodule update
```

**Step 3:** Build the project.
```bash
make
```

You'll see something like this:

```bash
username@machine:~/List$ make

```

**Step 4:** Running.
```bash
make run
```

## Dump examples
  
List which capacity is miniaml

![picture_1](/examples/dump.png)

List which capacity has been insreased

![picture_2](/examples/dump_2.png)
