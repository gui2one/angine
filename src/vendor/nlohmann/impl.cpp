#include "json.hpp"
using json = nlohmann::json;

int main(){
	
	json j;
	j["property_1"] = true;
	std::string s;
	s = j.dump(4);
	printf("%s\n", s.c_str());
	
	return 1;
}

