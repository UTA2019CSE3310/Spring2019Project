/*
  Message obfuscation software written by Gerardo Rodriguez on March 3, 2019.
  This software was created to obfuscate messages by their alphabetic content only and to meet the message
  obfuscation requirements of SuperChat: Messaging Like It's 2005.
  The functionalities of this software have only been tested on MacOS Mojave 10.14.2 with the GNU C++ compiler.

  Author's note: This software was created to meet the ends of a group project in the Fundamentals of Software Engineering course
                 at the University of Texas at Arlington.
*/
#include <iostream>
#include <cstring>
using namespace std;
#ifndef message_obfuscation_
#define message_obfuscation_

void encrypt(char* message, int key)
{
  int i;
  for (i = 0; i < strlen(message); i++)
 	{
 	 if (message[i] <  65 || (message[i] > 90 && message[i] < 97) || message[i] > 122) //handles non-alphabetic input
 	  message[i] = message[i];
 	 else if ((message[i] + key) > 90 && (message[i] <= 90 && message[i] >= 65))       //handles upper-case letters that overflow to
 		{                                                                                //non-alphabetic characters
 		 message[i] = message[i] + key - 26;
 		}
     else if ((message[i] + key) > 122 && (message[i] <= 122 && message[i] >= 97))   //handles lower-case letters that overflow to
     {                                                                               //non-alphabetic characters
       message[i] = message[i] + key - 26;
     }
 	 else//handles alphabetic characters that don't overflow to non-alphabetic characters
 	  message[i] = message[i] + key;
 	}
}

void decrypt(char *message, int key)
{
  int i;
  for (i = 0; i < strlen(message); i++)
 	{
     if (message[i] <  65 || (message[i] > 90 && message[i] < 97) || message[i] > 122) //handles non-alphabetic input
      message[i] = message[i];
     else if ((message[i] - key) < 65 && message[i] >= 65)                             //handles upper-case letters that overflow to
  		{                                                                                //non-alphabetic characters
  		 message[i] = message[i] - key + 26;
  		}
      else if ((message[i] - key) < 97 && message[i] >= 97)                            //handles lower-case letters that overflow to
      {                                                                                //non-alphabetic characters
        message[i] = message[i] - key + 26;
      }
 	 else//handles alphabetic characters that don't overflow to non-alphabetic characters
 	  message[i] = message[i] - key;
 	}
}

#endif
