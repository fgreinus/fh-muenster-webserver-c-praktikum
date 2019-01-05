#PSE Gruppe 10

##Author
* Steffen Schulze - ss896775@fh-muenster.de
* Alexander Gruhlke - ag257774@fh-muenster.de
* Hakan Batmaz - hb719211@fh-muenster.de
* Florian Greinus - fg523866@fh-muenster.de

##Getting Started
###Prerequisites
 1.cmake  
 2.make

###Installation
1.Go to the Directory where you want to install the Project  
2.Installation:  
        -git clone ssh://git@git.fh-muenster.de:2323/PSE2017/Gruppe10.git  
        -cd Gruppe10  
        -mkdir build  
        -cd build/
        -cmake ../  
        -make  

###Run tests
1. make sure Python3 is installed
2. go to /Gruppe10/tests
3. run Python3 tests.py in your Console

###Generating Documentation out of in-code doxygen annotations
1. Make sure a doxygen executable is installed on your system (e.g. on debian-based system -> apt-get install doxygen)
2. The config file to pass into doxygen is located in the root directory - see doxygen.conf
3. Call doxygen doxygen.conf from within the root directory.
4. The documentation will be exported to the doc-folder.



