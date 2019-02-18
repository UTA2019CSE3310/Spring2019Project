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
| 027                | N                  | User Input         | C                 | The user can enter #h for a help menu showing all the command a user is allowed to use | N/A |
| 028                | F                  | Server             | S                 | The server will automatically start with a default lobby chatroom that cannot be deleted | N/A | 
| 029                | F                  | Server             | S                 | The server will support up to 50 users | N/A |
| 030                | N                  | User input         | C                 | The user can block or "ban" another user's messages by using #b | N/A | 