#ifndef __GALAPAGOS_ROUTER_NODE_HPP__   // if x.h hasn't been included yet...
#define __GALAPAGOS_ROUTER_NODE_HPP__

#include "galapagos_router.hpp"


enum ext_port{network};


namespace galapagos{
    class router_node: public router{
        private:
            std::map <std::string,  std::vector<short> >  address_map;
    	    std::vector < std::string > kern_info_table;
            std::string my_address;
            bool _done;
            std::mutex  mutex;

            //external kernels;
            std::vector <galapagos::kernel * > ext_kernels;
            std::vector <int > ext_kernels_indices;
            std::unique_ptr <std::thread> t;
        public:
            router_node(std::vector <std::string>  _kern_info_table, std::string _my_address);
            void route();
            void add_kernel(kernel * _gk);
            void add_ext_kernel(galapagos::kernel * gk);
            void start();
            void end();
            ~router_node();
    };
}



#endif
