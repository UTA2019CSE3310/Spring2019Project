| SuperChat:  | Message Obfuscation |
| ----------- | ----------- |
| Actors      | Two clients |
| Description | Two clients in a chat room will have an agreed-upon number between 1 to 25 to obfuscate their messages. The sender will type and enter **#e** before writing and entering the message they wish to encrypt. The message, once sent, will go through a Caesar cipher and then be presented on the receiving client's screen. The receiver must then type and enter **#d** to decrypt all of the chat's messages. A second entry of **#d** will return the chat to its original composition.  |
| Data        | The modulus to obfuscate the message, the message|
| Stimulus    | Receiving user enters **#d** to decrypt the message |
| Response    | All of the text on the screen is shifted by the Caesar cipher specified by the two parties. |
| Comments    | The receiving user must then type **#d** to shift the text back to its original composition.  |
