#include "galapagos_router_node.hpp"



galapagos::router_node::router_node(std::vector <std::string>  _kern_info_table, 
                                    std::string _my_address)
                                    :galapagos::router::router()
{
  
    _done = false;

    my_address = _my_address;

    int num_ports = 0;

    for(int i=0; i<_kern_info_table.size(); i++){
        kern_info_table.push_back(_kern_info_table[i]);

        if(_kern_info_table[i] == _my_address){
            num_ports++;
        }
    }
    
    galapagos::router::init_ports(num_ports);

    
}


void galapagos::router_node::add_ext_kernel(galapagos::kernel * gk){
    
    s_axis.push_back(std::make_unique <stream> ());
    m_axis.push_back(std::make_unique <stream> ());
   
    ext_kernels.push_back(gk);
    ext_kernels[ext_kernels.size() - 1]->start();

    ext_kernels_indices.push_back(s_axis.size() - 1);

}



void galapagos::router_node::add_kernel(galapagos::kernel * _gk){

    galapagos::router::add_stream( _gk);

}

void galapagos::router_node::start(){

    t=std::make_unique<std::thread>(&galapagos::router::route, this); 
    t->detach();
    
}



void galapagos::router_node::route(){

    galapagos::stream_packet gps;

    while(1){
        
        //done set outside
        {
            std::lock_guard<std::mutex> guard(mutex);
            if(_done)
                break;
        }
        for(int i=0; i<m_axis.size(); i++){
            galapagos::stream * stream_ptr = m_axis[i].get();
            if(stream_ptr->try_read(gps)){
                if (kern_info_table[gps.dest] == my_address)
                {
                    //std::cout << "local route" << std::endl;
                    s_axis[gps.dest]->write(gps);
                }
                else{
                    s_axis[ext_kernels_indices[network]]->write(gps);
                }
            }
        }
    }
}


void galapagos::router_node::end(){

    {
        std::lock_guard<std::mutex> guard(mutex);
        _done = true;
    }


    for(int i=0; i<ext_kernels.size(); i++){
        ext_kernels[i]->barrier();
    }
    

}

galapagos::router_node::~router_node(){;}
