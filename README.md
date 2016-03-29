# Assignnment 1 

* Name : Liyang(Leon) Guan  

* Uni ID : u5684206  

## Summary of program's functionalities  

* Display the help message  
* Print out the width and height of an image  
* Apply a threshold filter  
* Crop the image  


## How to compile or run  

### Compile  

* $ gcc -Wall -o bmpedit bmpedit.c  
OR  
* $ make bmpedit (if there is a Makefile in the same directory)  

### Run  

(The order of the list corresponds to the summary of functionalities)  
* $ ./bmpedit -h  
* $ ./bmpedit [file]  
    e.g. $ ./bmpedit cup.bmp  
* $ ./bmpedit -t [thredhold value] -o [output_file]   
    e.g. $ ./bmpedit -t 0.5 -o cupth.bmp OR $ ./bmpedit -t 0.5 (by default output file is out.bmp)  
* $ ./bmpedit -c [originX] [originY] [width] [height] -o [output_file]    
    e.g. $ ./bmpedit -c 100 100 100 100 -o crop.bmp OR $ ./bmpedit -c 100 100 100 100 (by default output file is out.bmp)  


## Testing  

## What makes my extensions deserve a higher mark  

## Limitations and Improvements  


