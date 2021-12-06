#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

using namespace rapidjson;

int main() {
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"lamp1\":\"test\",\"stand\":1}";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value& s = d["stand"];
    s.SetInt(s.GetInt() - 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    d.Accept(writer);

    // Output {"Lamp_1":"test","stand":0}
    std::cout << buffer.GetString() << std::endl;


    std::cout << "Output : " << json << std::endl;



    return 0;
}