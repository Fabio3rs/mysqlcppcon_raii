#include "MysqlRAII.hpp"
#include <cppconn/driver.h>
#include <mysql_driver.h>

auto MysqlRAII::singleton() -> MysqlRAII&
{
    static MysqlRAII inst;
    return inst;
}

auto MysqlRAII::connect_unique(const dbconninfo &info) -> unique_conn_t {
    std::lock_guard<std::mutex> lck(sqldrvmtx);
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();

    auto conn = unique_conn_t(driver->connect(info.host, info.user, info.pass));
    conn->setSchema(info.db);

    return conn;
}

auto MysqlRAII::connect_shared(const dbconninfo &info) -> shared_conn_t {
    std::lock_guard<std::mutex> lck(sqldrvmtx);
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();

    auto conn = shared_conn_t(driver->connect(info.host, info.user, info.pass));
    conn->setSchema(info.db);

    return conn;
}
