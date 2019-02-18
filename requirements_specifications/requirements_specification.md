# Requirements Specification
> For the completion of the fundamentals of software engineering course at the University of Texas at Arlington


## By: Group 2
> Gerardo Rodriguez, Luis Flores, Kai Lee, Samuel Mahofski, and Matthew Schultz

SuperChat, a lightweight terminal application, is being considered as an application for development. The following document specifies three use cases, a list of requirements that need to be implemented for its functionality, a state diagram, illustrations of the graphical user interface, and a list of unknown requirements (otherwise known as not-fully-specified requirements).

## Use Cases

### Stick Figure Use-Case
The stick figure use case specifies the domain of both the user and the system. As illustrated by the diagram, the user and the system have different privileges when they interact with each other through the application.

![Stick Figure Use Case](https://raw.githubusercontent.com/UTA2019CSE3310/Spring2019Project/master/requirements_specifications/Use%20case%20diagram.JPG)

### Message Obfuscation Use-Case

| SuperChat:  | Message Obfuscation |
| ----------- | ----------- |
| Actors      | Two clients |
| Description | Two clients in a chat room will have an agreed-upon number between 1 to 25 to obfuscate their messages. The sender will type and enter **#e** before writing and entering the message they wish to encrypt. The message, once sent, will go through a Caesar cipher and then be presented on the receiving client's screen. The receiver must then type and enter **#d** to decrypt all of the chat's messages. A second entry of **#d** will return the chat to its original composition.  |
| Data        | The modulus to obfuscate the message, the message|
| Stimulus    | Receiving user enters **#d** to decrypt the message |
| Response    | All of the text on the screen is shifted by the Caesar cipher specified by the two parties. |
| Comments    | The receiving user must then type **#d** to shift the text back to its original composition.  |


### User Filtering Use-Case

| SuperChat:  | User Banning/Filtering |
| ----------- | ----------- |
| Actors      | One Client |
| Description | A client (Client A) who desires to filter out another user's messages (Client B) from his terminal may type **#b** to hide all of the messages from the his (Client A's) terminal as they start being sent. Entering **#b** again will show the filtered user's (Client B) messages again as the messages are sent.|
| Data        | Two user's nicks |
| Stimulus    | Client A types and enters **#b** |
| Response    | The messages sent by Client B will be filtered from Client A's terminal as they are sent out. |
| Comments    | The rest of the clients still receive Client B's messages.  |

### The Requirements List

The requirements that are listed will specify a unique identifier (UID), a written requirement, whether the requirement is non-functional or functional (N/F), the source of the data, whether the requirement works with the client, the server, or both, and any miscellaneous notes.

| UID                | (N/F)              | Source             | (C/S/B)            | Written Requirements                                                                                                       |      Notes      |
| :--------------:   | :----------------: | :----------------: | :----------------: | -------------------------------------------------------------------------------------------------------------------------  | --------------- |
| 001                | F                  | \<ctime\>          | C                  | The messages will be tagged with the date they were sent on                                                                | N/A             |
| 002                | N                  | \<ctime\>          | S                  | Use library \<ctime\> to query time and date                                                                               | N/A             |
| 003                | F                  | Client nick        | B                  | The messages will be tagged with the nick                                                                                  | N/A             |
| 004                | N                  | Client input       | S                  | The nicks will be stored in a string vector                                                                                | N/A             |
| 005                | N                  | Client input       | B                  | The nick entered by the user shall be verified by the server to determine any collisions with any retired or active nicks. | N/A             |
| 006                | F                  | \<ctime\>          | C                  | The messages will be tagged with the time they were sent on                                                                | N/A             |
| 007                | N                  | N/A                | S                  | The server should only be established on ports above 1024                                                                  | N/A             |
| 008                | F                  | Server             | S                  | The server should provide users with past messages of a chatroom                                                           | N/A             |
| 009                | N                  | Client input       | B                  | The messages must be sent 1 second after user enters send                                                                  | N/A             |
| 010                | N                  | \<ncurses\>        | C                  | The user interface for the client will be created with ncurses                                                             | N/A             |
| 011                | N                  | N/A                | B                  | The application will be developed in C++11                                                                                 | N/A             |
| 012                | N                  | N/A                | B                  | The application's networking capacities will be carried out by the ASIO library                                            | N/A             |
| 013                | F                  | User input         | B                  | The users will be identified by nicks specified by the user                                                                | N/A             |
| 014                | F                  | User input         | B                  | User input will be spell-checked by an external dictionary                                                                 | N/A             |
| 015                | F                  | User input         | C                  | Tab completion will allow the client to complete words used often by the client                                            | N/A             |
| 016                | F                  | User directory     | B                  | File transfer will be allowed from one user's home directory to another user's home directory.                             | N/A             |
| 017                | N                  | User directory     | B                  | The file transfer feature will be implemented using sftp (safe file transfer protocol)                                     | N/A             |
| 018                | F                  | Server             | B                  | Up to ten chatrooms will be allowed in a specific server           | N/A             |
| 019                | N                  | Server             | S                  | The chatrooms and their information will be stored in a chatroom object vector with error checking to prevent an 11th room from being created. | N/A |
| 020                | F                  | User input         | B                  | Any user can create or remove a chat room if the chatroom is empty                                                         | N/A             |
| 021                | N                  | User input         | B                  | The user can create a chatroom by typing and entering #c in the lobby                                                   | N/A             |
| 022                | N                  | User input         | B                  | The user can remove a chatroom by typing and entering #r in the respective chatroom                                         | N/A             |
| 023                | F                  | User input         | C                  | The user may obfuscate their message           | N/A             |
| 024                | N                  | User input         | C                  | The user may encrypt their message by typing and entering #e. The system will query the user with a question "Caesar cipher: " to specify the encryption method. To remove the encryption feature, the user types and enters #e again.           | N/A             |
| 025                | N                  | User input         | C                  | The user may decrypt any obfuscated messages by typing #d. The system will query the user with a question "Caesar cipher: " to specify the decryption method. To remove the decryption feature, the user types and enters #d again.            | N/A             |
| 026                | F                  | Server             | S                 | The server will give the user an option to logout | N/A |         
| 027                | N                  | User Input         | C                 | The user can enter #help for a help menu showing all the command a user is allowed to use | N/A |
| 028                | F                  | Server             | S                 | The server will automatically start with a default lobby chatroom that cannot be deleted | N/A |
| 029                | F                  | Server             | S                 | The server will support up to 50 users | N/A |
| 030                | N                  | User input         | C                 | The user can block or "ban" another user's messages by using #b | N/A |
| 031                | F                  | Server             | S                 | The server will show how to logout, get to the lobby, and change chatrooms at the top of the server | N/A |
| 032                | B                  | User Input         | S                 | When the user enters #chatrooms a new window will show the user all the available chatrooms | N/A |
| 033                | F                  | User input         | S                 | The chatrooms window will tell the user how many people are in each chatroom | N/A |
| 034                | F                  | Server             | C                 | The chatroom will show when anew user has joined the current chatroom | N/A |
| 035                | F                  | Server             | C                 | The chatroom will show which user created the chatroom | N/A |
| 036                | N                  | Server             | C                 | The user can return to the main lobby chatroom by typing #lobby | N/A |
| 037                | N                  | Server             | C                 | The chatrooms window will tell the user if they've entered incorrect options when chosing a chatroom | N/A |

### The State Diagram

The following state diagram specifies a lower-level view of the functionalities of SuperChat from the specification of the ports used to the input and output characteristics of the user and the server.

![State Diagram](https://raw.githubusercontent.com/UTA2019CSE3310/Spring2019Project/master/requirements_specifications/statediagram.jpg)


### The User Interface

The next three images illustrate an idea of the user interface.

#### The Lobby

![Lobby](https://raw.githubusercontent.com/UTA2019CSE3310/Spring2019Project/master/requirements_specifications/Lobby.jpg)

The lobby will be a nonerasable chatroom equipped with a command to display the available chatrooms.

#### The Chatroom Display

![Chatroom Display](https://raw.githubusercontent.com/UTA2019CSE3310/Spring2019Project/master/requirements_specifications/Chatroom_menu.jpg)

The chatroom display will display all of the available chatrooms and will give users the ability to create chatrooms if there is not an excess of chatrooms. Users will also be capable of deleting any empty chatrooms.

#### The Chatroom

![The Chatroom](https://raw.githubusercontent.com/UTA2019CSE3310/Spring2019Project/master/requirements_specifications/Chatroom_1.jpg)

The chatroom will have its name and the creator's nick displayed on top, will have detailed chronology of the messages sent, and a help command to guide the users through any commands they may wish to carry out.

### Unknown Requirements

The following table specifies any requirements that we are unsure of being able to carry out.

| UID                | (N/F)              | Source                 | (C/S/B)            | Written Requirements                                                                                                       |      Notes      |
| :--------------:   | :----------------: | :----------------:     | :----------------: | -------------------------------------------------------------------------------------------------------------------------  | --------------- |
| 001                | F                  | User Input, dictionary | C                  | A client's messages will be spell-checked by the system's dictionary                                                       | N/A             |
| 002                | N                  | User Input             | C                  | Consistent user input will be queried for quick replies. For example, a user who consistently types \$\{name\} may press tab to finish typing the name                                  | N/A             |
| 003                | N                  | User's LAN             | C                  | The client application will do a port scan to determine which systems have the server's designated port activated          | N/A             |
| 004                | N                  | User Input, nicks      | S                  | Users will be able to filter users. The nonfunctional aspect of this is yet to be determined.                              | N/A             |
| 005                | N                  | User's home directory  | B                  | The user may share files from their home directory with another user. This will probably be implemented with the safe-file-transfer protocol | N/A             |
