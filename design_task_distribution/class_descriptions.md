# Class Descriptions

## SuperChat

The SuperChat class is merely an interface class meant to show the structural design of the software. This class will not be implemented but will serve the developers in appropriately identifying the relationships within the system.


## Server

The server class serves up to 50 clients and is meant to be run on a single machine, a single IP address, and a single port. The server —in conjunction with the clients— will work in a fat-client architecture to optimize server speed for quick message delivery. Considering the fact that the graphical user interface is text-based and the system's language, a fat-client should not be an issue.

The server will be able to log users out, establish connections, and print the past few messages of any room.


## Nicks

The client has an array of functions alongside its networking capabilities (not limited to connecting into a server). The client can send and receive files to and from all users, respectively, send and receive messages to and from all users, respectively, encrypt and decrypt messages according to a Caeser cipher, and create and delete chatrooms.


## GNU Wrapper

The GNU Wrapper calls functions to store constant user information and to spell check messages before being sent out to the chat. The GNU Wrapper wraps around the GNU readline library which contains functions that will allow the program to spell check messages and aid in storing constant information.


## NCurses_Wrapper

The NCurses_Wrapper wraps around the NCURSES library. The attribute <windows> is a vector of windows of size 10 to appropriately limit the number of chatroooms available. The <panels> attribute is a vector of panels that can dynamically change if a user requires help or if they wish to obfuscate a message. The respective methods initialize NCURSES, the panels, and modify any visuals.


## ASIO Wrapper

The ASIO Wrapper is an interface that initializes a TCP session between a server and a nick. The relevant attribute for Nicks is the endpoints attribute while the start_session() method is relevant to both the server and the nick.
