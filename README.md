# SuperChat 
> A Super Way to Chat

This application was made to meet the requirements for the Fundamentals of Software Engineering course at the University of Texas at Arlington. 

The dependencies for this application are: 
- the Boost library
- the Asio library


## Build

The current method of building on MacOS is as follows (assuming you have downloaded the Asio library from sourceforge and your Downloads directory is in your current working directory):

`#!/bin/bash

brew install boost
brew install asio

cd Downloads/asio-1.12.2/src/examples/cpp11/chat

g++ -std=c++11 chat_client.cpp -o chat_client
g++ -std=c++11 chat_server.cpp -o chat_server `

The same build method will work by replacing "brew" with your favorite package manager.
