//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <cstring>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }
    char time_buffer[32];
    time_t t_;
    strftime(time_buffer, 32, "%c", localtime(&t_));
    char nick[25];
    std::cout << "Welcome to SuperChat!\n\nPlease enter a nick to begin: ";
    std::cin.getline (nick, 25);

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });
    char formatted_message[chat_message::max_body_length + 1];
    // create a date and time string, copy into formatted_message, concatenate nick, then concatenate msg
    char line[chat_message::max_body_length + 1];
    std::cout << "Welcome to the main lobby! Remember: no profanity, no Smurf attacks, and have fun!\n\nType #h for a list of commands\n\n" << std::endl;
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      if (line[0] == '#')
      {
        switch (line[1])
        {
          case 'h':
            std::cout << "\n#h: help --   prints the list of respective commands and shows how to navigate the application\n\n\
#e: encode -- obfuscates a message based on a Caesar cipher prior to being sent\n\n\
#d: decode -- de-obfuscates a message based on a Caesar cipher at any given time\n\n\
#b: block --  blocks a user from a chatroom; cannot be used in lobby\n\n\
#l: lobby --  takes user back to lobby\n\n\
#mc: make chatroom -- makes a chatroom\n\n\
#dc: delete chatroom -- deletes a chatroom\n\n\
#lo: logout -- logs a user out (basically ^C)\n\n\
#f: file --   sends a file to all users in a chatroom\n\n\
#sc: show chatrooms -- shows available chatrooms in lobby" <<std::endl;
            break;
          case 'e':
            std::cout << "encoding..." << std::endl;
            break;

          case 'd':
            if (line[2] == '\0')
            {
              std::cout << "decoding..." << std::endl;
              break;
            }
            else if (line[2] == 'c' && line[3] == '\0')
            {
              std::cout << "deleting chatroom..." << std::endl;
              break;
            }
            else
            {
              std::cout << line << " is not a command. Please try again." << std::endl;
              break;
            }
          case 'b':
            std::cout << "blocking user..." << std::endl;
            break;

          case 'l':
            if (line[2] == '\0')
            {
              std::cout << "going to lobby..." << std::endl;
              break;
            }
            else if (line[2] == 'o' && line[3] == '\0')
            {
              std::cout << "logging out...\n" << std::endl;
              break;
            }
            else
            {
              std::cout << line << " is not a command. Please try again." << std::endl;
              break;
            }
          case 'm':
            std::cout << "Making chatroom..." << std::endl;
            break;

          case 'f':
            std::cout << "File transfer in progress..." << std::endl;
            break;

          case 's':
            std::cout << "Displaying available chatrooms..." << std::endl;
            break;

          default:
            std::cout << line << " is not a command. Please try again." << std::endl;
            break;
        }
        /*determine if it's a valid command and then execute the command

          #h: help -- prints the list of respective commands and shows
                      how to navigate the application

          #e: encode -- obfuscates a message based on a Caesar cipher prior
                      to being sent

          #d: decode -- de-obfuscates a message based on a Caesar cipher
                      at any given time

          #b: block -- blocks a user from a chatroom; cannot be used in lobby

          #l: lobby -- takes user back to lobby

          #mc: make chatroom -- makes a chatroom

          #dc: delete chatroom -- deletes a chatroom

          #lo: logout -- logs a user out (basically ^C)

          #f: file -- sends a file to all users in a chatroom

          #sc: show chatrooms -- shows available chatrooms in lobby

        */

      }
      else
      {
        t_ = time(NULL);
        strftime(time_buffer, 32, "%c", localtime(&t_));
        sprintf(formatted_message, "%s %s: ", time_buffer, nick);
        std::strcat(formatted_message, line);
        chat_message msg;
        msg.body_length(std::strlen(formatted_message));
        std::memcpy(msg.body(), formatted_message, msg.body_length());
        msg.encode_header();
        c.write(msg);
        //update formatted_message with date and time
        t_ = time(NULL);
        strftime(time_buffer, 32, "%c", localtime(&t_));
        sprintf(formatted_message, "%s %s: ", time_buffer, nick);
      }
      t_ = time(NULL);
      strftime(time_buffer, 32, "%c", localtime(&t_));
      sprintf(formatted_message, "%s %s: ", time_buffer, nick);
      std::strcat(formatted_message, line);
      chat_message msg;
      msg.body_length(std::strlen(formatted_message));
      std::memcpy(msg.body(), formatted_message, msg.body_length());
      msg.encode_header();
      c.write(msg);
      //update formatted_message with date and time
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
