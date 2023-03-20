#include <fstream>
#include <iostream>
#include <httplib.h>
#include <string>
#include <iterator>
#include <nlohmann/json.hpp>
#include <vector>

using nlohmann::json;

class Lohman {

public:

std::vector<json> data_json {};
 
~Lohman() {}

void input() {

  std::ifstream in;
  std::vector<std::string> data;
  std::string line;
  in.open("task3/src/task.txt");
  if (!(in.is_open()))
  {
    printf("File open error");
    
  }
  while(std::getline(in,line)){
  data.push_back(line);
  }
  
  for (long long unsigned int i=0; i<data.size(); i++){

  data_json.push_back(json::parse(data[i]));}
  in.close();
}


void print(long long unsigned int n) {

   std::cout<< "Structure"<< " #"<<n<<": " <<data_json[n-1]<<std::endl;}

void Add(std::string start, std::string end, std::string length){

    json j;
    j["Start"]=start;
    j["End"]=end;
    j["Length"]=length;
    j["Transport"]={"Foot", "Car", "Train", "Plane"};
    data_json.push_back(j);    
}  

void Delete(long long unsigned int n){

   data_json.erase(data_json.begin() + int(n) - 1);}  

std::string Execute(std::string transport){
   std::string s="";
   for (long long unsigned int i=0; i<data_json.size(); i++){
       if  (std::count(std::begin(data_json[i]["Transport"]),std::end(data_json[i]["Transport"]), transport)!=0)
        s+=std::to_string(i+1)+" ";
}
   return s;
}

};




int main(void)
{
  using namespace httplib;
  using nlohmann::json;
  Lohman data;
  std::string s1 = R"({
  "Start" : "Moscow", 
  "End" : "Paris", 
  "Length" : 2836,
  "Transport" : ["Plane","Car"]})";
  std::string s2 = R"({
  "Start" : "Spb", 
  "End" : "Stockholm", 
  "Length" : 902, 
  "Transport" : ["Plane","Ship"]})";
  std::string s3 = R"({
  "Start" : "GZ", 
  "End" : "VMK", 
  "Length" : 300, 
  "Transport" : ["Foot","Car"]})";

  
  data.data_json.push_back(json::parse(s1));
  data.data_json.push_back(json::parse(s2));
  data.data_json.push_back(json::parse(s3));
  
  Server svr;
  
   svr.Post("/size", [&data](const Request& req, Response& res) {

    nlohmann::json j = nlohmann::json::parse(req.body);
    //std::string line = data.Execute(j["transport"]);
    res.set_content(std::to_string(data.data_json.size()), "text/html");
  
  });
   
   svr.Post("/execute", [&data](const Request& req, Response& res) {

    nlohmann::json j = nlohmann::json::parse(req.body);
    std::string line = data.Execute(j["transport"]);
    res.set_content(line, "text/html");

  });
  
  svr.Post("/print", [&data](const Request& req, Response& res) {

    nlohmann::json j = nlohmann::json::parse(req.body);
    
    res.set_content((data.data_json[static_cast<long long unsigned int>(j["number"]) - 1]).dump(), "application/json");

  });
  
  svr.Post("/add", [&data](const Request& req, Response& res) {

    nlohmann::json j = nlohmann::json::parse(req.body);
    data.Add(j["Start"],j["End"],j["Length"]);
    res.set_content("Structure succesfully added", "text/html");

  });
  
  svr.Post("/delete", [&data](const Request& req, Response& res) {

    nlohmann::json j = nlohmann::json::parse(req.body);
    data.Delete(j["number"]);
    res.set_content("Structure succesfully deleted", "text/html");

  });
  
  svr.Get("/stop",[&](const Request& req, Response& res) {
    nlohmann::json j = nlohmann::json::parse(req.body);
    res.set_content("stops", "text/html");
    svr.stop();
      
  });

  svr.listen("127.0.0.1", 8080);
  return 0;
}
