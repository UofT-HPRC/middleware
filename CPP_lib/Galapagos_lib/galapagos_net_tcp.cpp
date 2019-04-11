//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "galapagos_net_tcp.hpp"



galapagos::net::tcp::tcp::tcp(short port, 
              std::vector <std::string> kernel_info_table, 
              std::string my_address, 
              galapagos::stream * _to_sessions, 
              galapagos::stream * _from_sessions, 
              std::mutex * _done_mutex, 
              bool * _done,
              bool enabled=true)

{
  
    to_sessions = _to_sessions;
    from_sessions = _from_sessions;
    sc_ptr = std::make_unique<galapagos::net::tcp::session_container>(to_sessions, from_sessions, kernel_info_table, my_address);
    ss_ptr = std::make_unique<galapagos::net::tcp::server_send>(port, &io_context, sc_ptr.get(), _done, _done_mutex, to_sessions); 
    if(enabled){
        as_ptr = std::make_unique<galapagos::net::tcp::accept_server>(&io_context, port, sc_ptr.get());
    }
    std::thread t(&galapagos::net::tcp::tcp::test, this); 
    t.detach();
    io_context.run();


}



void galapagos::net::tcp::tcp::test(){

    std::vector<ap_uint<PACKET_DATA_LENGTH> > vect;

    int dest;
    while(1){
        std::cout << "TEST: reading stream: " << from_sessions->name << std::endl;
        while(from_sessions->size() == 0);
        vect = from_sessions->read(&dest);
        std::cout << "size of test is " << vect.size() << std::endl;
        std::string test = (char *)vect.data();
        std::cout << "test is " << test << std::endl;

    }




}




void galapagos::net::tcp::tcp::stop(){
    io_context.stop();

}
