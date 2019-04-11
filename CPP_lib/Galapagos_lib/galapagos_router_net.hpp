#ifndef __GALAPAGOS_ROUTER_NET_HPP__   // if x.h hasn't been included yet...
#define __GALAPAGOS_ROUTER_NET_HPP__

#include <map>
#include <assert.h>
#include "galapagos_packet.h"
#include "galapagos_stream.hpp"
#include "galapagos_kernel.hpp"
#include "galapagos_router.hpp"


namespace galapagos{
    class router_net: public router{
        protected:
            std::map <std::string,  int >  address_map;
    	    std::vector < std::string > kern_info_table;
            bool _done;
            std::mutex  mutex;
            std::unique_ptr <std::thread> t;
            stream * ext_port;
        public:
            router_net(std::vector <std::string>  _kern_info_table, stream * ext);
            void add_socket(galapagos::streaming_core * _gsc);
            virtual void route() = 0;
            virtual void start() = 0;
            std::vector <std::string> ip_addrs;
            //void end();
    };
    class router_net_in: public router_net{
        public:
            router_net_in(std::vector <std::string>  _kern_info_table, stream * to_sessions);
            void start();
            void route();
    };
    class router_net_out: public router_net{
        public:
            router_net_out(std::vector <std::string>  _kern_info_table, stream * from_sessions);
            void start();
            void route();
    };
}



#endif
