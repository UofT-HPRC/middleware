#ifndef __GALAPAGOS_ROUTER_HPP__   // if x.h hasn't been included yet...
#define __GALAPAGOS_ROUTER_HPP__

#include <map>
#include <assert.h>
#include "galapagos_packet.h"
#include "galapagos_stream.hpp"
#include "galapagos_kernel.hpp"

namespace galapagos{
    class router{
        private:
            std::unique_ptr <std::thread> t;
            std::vector <std::unique_ptr <stream>  >  s_axis;
            std::vector <std::unique_ptr <stream>  >  m_axis;
            //std::vector <std::queue <galapagos::stream_packet>  >  s_axis;
            //std::vector <std::queue <galapagos::stream_packet>   > m_axis;
            //std::vector <std::unique_ptr <std::mutex> > m_axis_mutex;
            //std::vector <std::unique_ptr <std::mutex> > s_axis_mutex;
            void route();
            void route_packet();
            std::map <std::string,  std::vector<short> >  address_map;
    	    std::vector < std::string > kern_info_table;
            std::string my_address;
            bool _done;
            std::mutex  mutex;
    
        public:
            router(std::vector <std::string>  _kern_info_table, std::string _my_address);
            void add_kernel(kernel * _gk);
            void start();
            galapagos::stream_packet read(short id);
            size_t m_size(short id);
            size_t s_size(short id);
            void write(galapagos::stream_packet _gps);
            void end();
            ~router();
    };
}



#endif
