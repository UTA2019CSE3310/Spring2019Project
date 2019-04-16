//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define NCURSES_NOMACROS
#include <ncurses.h>
#include <pthread.h>
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
  char currRoom = 'l';
  std::vector<std::string> rooms = {"lobby"};
	
	
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(
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
    asio::post([this]() { socket_.close(); });
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
			 // std::cout << currRoom;
			//std::cout.write(read_msg_.body(), read_msg_.header_length);
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
		 
			  
			if (read_msg_.data()[5] == currRoom){std::cout << "|||||| in cr |||||";}
			if (read_msg_.data()[5] != currRoom){std::cout << "|||||| not in same room ||||| ";}
			std::cout << read_msg_.data()[5];
		 std::cout.write(read_msg_.data(), read_msg_.header_length-1);
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
	
	
	char header[4] = "123";
	int head_buff = 0;

  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve( argv[1], argv[2] );
    chat_client c(io_context, endpoints);
	 
     c.currRoom = '1';
    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
		
		
		
	
	  
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line+head_buff, msg.body_length());
		
	  
		
		
	 
	  msg.head_set(header);
		//std::cout <<msg.head <<"here";
	  //std::memcpy(msg.head,header,msg.header_length+1);
	 //msg.head = head;
	 // std::cout << msg.head;
      msg.encode_header();
		 //std::cout.write(msg.data(), msg.header_length);
      c.write(msg);
		
    }
	
    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  //getch();
  //endwin();
  return 0;
}