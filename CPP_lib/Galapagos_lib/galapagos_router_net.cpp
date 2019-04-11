#include "galapagos_router_net.hpp"



galapagos::router_net::router_net(std::vector <std::string>  _kern_info_table, stream * ext){
  
    _done = false;
   
    ext_port = ext;

    std::vector<std::string>::iterator it;
    std::map<std::string, short >::iterator itr;
    
    //s_axis.push_back(std::make_unique <stream> ());
    //m_axis.push_back(std::make_unique <stream> ());

    for(int i=0; i<_kern_info_table.size(); i++){
        kern_info_table.push_back(_kern_info_table[i]);
        it = std::find(ip_addrs.begin(), ip_addrs.end(), _kern_info_table[i]);
        if(it == ip_addrs.end()){
           ip_addrs.push_back(_kern_info_table[i]); 
           //address_map[_kern_info_table[i]] = ip_addrs.size() - 1;
           address_map[_kern_info_table[i]] = ip_addrs.size();
        }

        s_axis.push_back(std::make_unique <stream> (_kern_info_table[i]));
        m_axis.push_back(std::make_unique <stream> (_kern_info_table[i]));

    }

    
}

void galapagos::router_net::add_socket(galapagos::streaming_core * _gsc){
    _gsc->in= s_axis[_gsc->id].get(); 
    _gsc->out= m_axis[_gsc->id].get(); 

//    std::cout << "adding socket " << _gsc->id << std::endl;
}



galapagos::router_net_in::router_net_in(std::vector <std::string>  _kern_info_tables, stream * to_sessions)
    :galapagos::router_net::router_net(_kern_info_tables, to_sessions)
{;}

void galapagos::router_net_in::start(){
    t=std::make_unique<std::thread>(&galapagos::router_net_in::route, this); 
    t->detach();
}

void galapagos::router_net_in::route(){


    int s_axis_index = 0;
    galapagos::stream_packet gps;

    while(1){
        
        //done set outside
        {
            std::lock_guard<std::mutex> guard(mutex);
            if(_done)
                break;
        }
        //galapagos::stream * stream_ptr = m_axis[0].get();


        //if(stream_ptr->try_read(gps)){
        //    std::string ip_addr = kern_info_table[gps.dest];
        //    int slave_index = address_map[ip_addr];
        //    s_axis[slave_index]->write(gps);
        //}
        
        if(ext_port->try_read(gps)){
            std::cout << " 75 ext name: " << ext_port->name << std::endl;
            std::string ip_addr = kern_info_table[gps.dest];
            int slave_index = address_map[ip_addr];
            s_axis[slave_index]->write(gps);
        }



    }
}

galapagos::router_net_out::router_net_out(std::vector <std::string>  _kern_info_tables, stream * from_sessions)
    :galapagos::router_net::router_net(_kern_info_tables, from_sessions)
{;}

void galapagos::router_net_out::start(){
    t=std::make_unique<std::thread>(&galapagos::router_net_out::route, this); 
    t->detach();
}

void galapagos::router_net_out::route(){

    int s_axis_index = 1;
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
                //s_axis[0]->write(gps);
                //s_axis[0]->write(gps);
                //std::cout << "s_axis name: " << s_axis[0]->name << std::endl;
                //std::cout << "s_axis size: " << s_axis[0]->size() << std::endl;
                ext_port->write(gps);
                ext_port->write(gps);
                std::cout << "ext name: " << ext_port->name << std::endl;
                std::cout << "ext size: " << ext_port->size() << std::endl;
            }

        }

    }
}



