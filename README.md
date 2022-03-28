# List
This project is my attempt to develop a well known structure of data: doubly linked list.
## General information
My list able to contain any basic numeric type of the language C you want. Before trying to insert data of some type write its name after key word <typedef> in file List.h. The standard capacity of list is 8 elements but it insreases automatically when needed. There is one warning though. The capacity of list doesn't decreases at all.
## Specific functions
There are some useful functions in the project. Firstly, I've developed a function called List_Dump (). It generates a graph that is a picture of the list. You can see it below. If you use this function, you should have programm Graphviz on your PC. Secondly, there is a function List_Linearisation (). It changes the order of elements in a way that the order starts being equal to the logical one. It means that every two elements that are logicaly neighbours lie in the memory in a row. Lastly, you can find the physical index of an element (the index in array) from its logical index by using function Logical_Num_To_Physical ().
  
Picture 1: List which capacity is miniaml
  ![picture_1](https://github.com/KetchuppOfficial/List/blob/master/Dump.png)

Picture 2: List which capacity has been resized
  ![picture_2](https://github.com/KetchuppOfficial/List/blob/master/Dump%202.png)
