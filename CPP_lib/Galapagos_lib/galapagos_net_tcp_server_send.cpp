//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "galapagos_net_tcp_server_send.hpp"

using namespace galapagos::net::tcp;

server_send::server_send(short _port, 
                        boost::asio::io_context * _io_context, 
                        session_container * _sessions,
                        bool * _done,
                        std::mutex * _done_mutex,
                        galapagos::stream * to_sessions
        
):galapagos::streaming_core(0, nullptr, to_sessions)

{

    done = _done;
    mutex = _done_mutex;
    port = _port;
    io_context = _io_context;
    sessions = _sessions;
    std::thread t_send(&server_send::send_loop,this);
    t_send.detach();
}



void server_send::send_loop(){

    int dest;
    while(1){
        {
            std::lock_guard<std::mutex> guard(*mutex);
            if(*done)
                break;
        }

        //std::cout << "before send socket" << std::endl;
        std::vector<ap_uint<PACKET_DATA_LENGTH> > data_vect = out->read(&dest);
        //std::cout << "got something in the socket" << std::endl;
        send((char *)data_vect.data(), data_vect.size() * 8, dest);        
        //std::cout << "sent something in the socket socket" << std::endl;

    }

}

  
void server_send::send(char * data, int size, short dest){
    
      
    std::string ip_addr = sessions->get_ip_addr(dest);
    bool found = sessions->send(ip_addr, data, size, dest);
      
      
      if(!found){
          std::cout << "making new socket " << std::endl; 
          boost::asio::ip::tcp::socket s(*io_context);
          boost::asio::ip::tcp::resolver resolver(*io_context);
          std::ostringstream convert;
          convert << port;
          std::string port_str = convert.str();
          std::cout << "trying to connect to ip addr " << ip_addr << std::endl;
          boost::asio::connect(s, resolver.resolve((char *)ip_addr.c_str(), (char *)port_str.c_str()));
   
          //std::string test = "hello123hello123";
          //boost::asio::write(s, boost::asio::buffer((char *)test.c_str(), 16));

          //exit(0);
          std::cout << "after connect " << ip_addr << std::endl;
          sessions->add_session(std::move(s));
          sessions->send(ip_addr, data, size, dest);
      }

}

