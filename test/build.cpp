#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <arpp/arpp.h>

SCENARIO("build is ok", "[build]") {
  SECTION("build is ok") {
    std::map<std::string, std::string> options = {
        {arpp::Connection::kOptionAdapter, "sqlite3"},
        {arpp::Connection::kOptionDatabase, "db.sqlite3"},
    };

    arpp::Status status;
    auto c = arpp::Connection::connect(options, &status);
    REQUIRE(status.is_ok());
  }
}
