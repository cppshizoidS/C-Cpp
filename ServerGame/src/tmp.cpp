// default
#include <iostream>
#include <string>
#include <array>

// conan
#include "restinio/all.hpp"


int main(int argc, char** argv)
{
    restinio::run(
        restinio::on_this_thread()
        .port(80)
        .address("0.0.0.0")
        .request_handler([](auto req) {
            return req->create_response().set_body("Hello, World!").done();
        })
    );

    return 0;
}

