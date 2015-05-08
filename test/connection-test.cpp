#include <catch.hpp>
#include <arpp/arpp.h>

SCENARIO("check basic connection features", "[connection]") {
  SECTION("has_connected") {
    std::map<std::string, std::string> options = {
        {arpp::Connection::kOptionAdapter, "sqlite3"},
        {arpp::Connection::kOptionDatabase, "db.sqlite3"},
    };

    arpp::Status status;
    auto c = arpp::Connection::connect(options, &status);
    REQUIRE(status.is_ok());
    REQUIRE(c->has_connected());
  }

  SECTION("equals shared_connection() and connect/re-connect") {
    std::map<std::string, std::string> options = {
        {arpp::Connection::kOptionAdapter, "sqlite3"},
        {arpp::Connection::kOptionDatabase, "db.sqlite3"},
    };

    arpp::Status status;
    auto c = arpp::Connection::connect(options, &status);
    REQUIRE(status.is_ok());
    REQUIRE(c == arpp::Connection::shared_connection());
    auto c2 = arpp::Connection::connect(options, &status);
    REQUIRE(status.is_ok());
    REQUIRE(c2 == arpp::Connection::shared_connection());
  }
}
