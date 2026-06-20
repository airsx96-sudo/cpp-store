#include <iostream>
#include <string>
#include "httplib.h"

int main() {
    httplib::Server svr;
// Force rebuild 1
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("<h1>Welcome to Ala's C++ E-Commerce Store!</h1>", "text/html");
    });

    svr.Get("/products", [](const httplib::Request&, httplib::Response& res) {
        std::string json_response = "[\n"
                                    "  {\"id\": 1, \"name\": \"ThinkPad Mouse\", \"price\": 25},\n"
                                    "  {\"id\": 2, \"name\": \"Mechanical Keyboard\", \"price\": 80}\n"
                                    "]";
        res.set_content(json_response, "application/json");
    });

    std::cout << "Server started at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
