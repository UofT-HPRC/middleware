#ifndef __GALAPAGOS_NET_TCP_HPP__   // if x.h hasn't been included yet...
#define __GALAPAGOS_NET_TCP_HPP

#include <cstdlib>
#include <iostream>
#include <thread>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "galapagos_stream.hpp"
#include "galapagos_router.hpp"


#include "galapagos_net_tcp_session.hpp"
#include "galapagos_net_tcp_accept_server.hpp"
#include "galapagos_net_tcp_server_send.hpp"

namespace galapagos{
    namespace net{
        namespace tcp{
            class tcp{
                public:
                    tcp(short port, 
                        std::vector <std::string> kernel_info_table, 
                        std::string my_address, 
                        galapagos::stream * in, 
                        galapagos::stream * out, 
                        std::mutex * _done_mutex, 
                        bool * _done,
                        bool enabled);
                    void stop();
                    void test();
                private:
                    boost::asio::io_context io_context;
                    std::unique_ptr<session_container> sc_ptr;
                    std::unique_ptr<server_send> ss_ptr;
                    std::unique_ptr<accept_server> as_ptr;
                    galapagos::stream * to_sessions; 
                    galapagos::stream * from_sessions; 
    
            };
        }//tcp namespace
    }//net namespace
}//galapagos namespace





#endif
