#include <httplib.h>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
using nlohmann::json;



std::map<httplib::Error, std::string> errors = {
  {httplib::Error::Success, "Success"},
  {httplib::Error::Unknown, "Unknown"},
  {httplib::Error::Connection, "Connection"},
  {httplib::Error::BindIPAddress, "BindIPAddress"},
  {httplib::Error::Read, "Read"},
  {httplib::Error::Write, "Write"},
  {httplib::Error::ExceedRedirectCount, "ExceedRedirectCount"},
  {httplib::Error::Canceled, "Canceled"},
  {httplib::Error::SSLConnection, "SSLConnection"},
  {httplib::Error::SSLLoadingCerts, "SSLLoadingCerts"},
  {httplib::Error::SSLServerVerification, "SSLServerVerification"},
  {httplib::Error::UnsupportedMultipartBoundaryChars, "UnsupportedMultipartBoundaryChars"}
};

int main(void)
{
  httplib::Client cli("127.0.0.1", 8080);
  std::string Command, key, value, length; 
  long long unsigned int number;  
  nlohmann::json j;
  
  
  std::cout<< "Input command [P/A/D/E/Stop]"<<std::endl;
  std::cin>>Command;
  while (Command!="Stop"){
  
  if (Command == "P"){
  std::cout<< "Enter number of the structure"<<std::endl;
  std::cin>>number;
  j["number"]=number;
  if (auto res = cli.Post("/print", j.dump(), "application/json")) {

    nlohmann::json out = nlohmann::json::parse(res->body);

    std::cout << "Structure: " << out << std::endl;
  }
  else {
    auto err = res.error();

    std::cout << "Error: " << errors.at(err) << std::endl;
  }
  j.clear();
  }
  
  if (Command == "A"){
  std::cout<< "Enter start"<<std::endl;
  std::cin>>key;
  std::cout<< "Enter end"<<std::endl;
  std::cin>>value;
  std::cout<< "Enter length"<<std::endl;
  std::cin>>length;
  j["Start"]=key;
  j["End"]=value;
  j["Length"]=length;
  if (auto res = cli.Post("/add", j.dump(), "application/json")) {

    std::string out = (res->body);

    std::cout << "Structure: " << out << std::endl;
  }
  else {
    auto err = res.error();

    std::cout << "Error: " << errors.at(err) << std::endl;
  }  
  j.clear();
  }
  
  if (Command == "D"){
  std::cout<< "Enter number of the structure"<<std::endl;
  std::cin>>number;
  j["number"]=number;
  if (auto res = cli.Post("/delete", j.dump(), "application/json")) {

    std::string out = (res->body);

    std::cout << "Structure: " << out << std::endl;
  }
  else {
    auto err = res.error();

    std::cout << "Error: " << errors.at(err) << std::endl;
  }
  j.clear();
  }
  
  if (Command == "E"){
  std::cout<< "Enter name of the transport"<<std::endl;
  std::cin>>key;
  j["transport"]=key;
  
  if (auto res = cli.Post("/execute", j.dump(), "application/json")) {

    std::string out = (res->body);

    std::cout << "Structures #: " << out << std::endl;
  }
  else {
    auto err = res.error();

    std::cout << "Error: " << errors.at(err) << std::endl;
  }  
  j.clear();
  }
  std::cin.clear();
  std::cout<< "Input command [P/A/D/E/Stop]"<<std::endl;
  std::cin>>Command;
  }
  auto res = cli.Post("/stop");
  return 0;
}