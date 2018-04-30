# cs342project03

Bilkent University Spring 2018 CS342: Operating Systems Project 3

Assignment Link: https://drive.google.com/open?id=1nRTLgqZ8MSoQJfR089m4zYxsTL5Tuq-h

Course Page: http://www.cs.bilkent.edu.tr/~korpe/courses/cs342spring2018/index.html

-------------------------------------------------------------------------------------------------------------------------------

Group Members:

Berat Biçer, 21503050

Ecem İlgün, 21502157

-------------------------------------------------------------------------------------------------------------------------------

Notes:

1 - For heap test, make sure heap grows gradually. Otherwise, results are not accurate. For example, do not allocate 10^24 size integer array at once; allocate in steps of length 10^4 integers.

2 - For stack test, make sure you don't overflow the stack; since default maximum size of stack is 8 MB.

3 - User might have noticed that growth in heap is not equal to the amount of memory malloc allocates. This is equal to sizeof(struct test) * (length of linked list) = 1040 bytes * length. The reason might be the following: since each process has a default heap size, kernel first allocates space from this existing heap. When the heap is full, kernel allocates more heap area to the process and user allocation contiues from this area. Therefore, this proves heap growth occurs only when existing heap is full.

4 - For stack, user might have noticed that in some cases, same parameter to the function "sum" does not result in exact same results. This problem can only be explained if and only if stack growth implementation of Linux kernel is known. Therefore, at this point, it is impossible to know for sure.

5- By default, heap and stack size of the process are equal, 135168 bytes in decimal.
