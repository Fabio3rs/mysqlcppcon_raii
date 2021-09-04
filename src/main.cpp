/**
 * @file main.cpp
 * @author Fabio R Sluzala ()
 * @brief Demonstração do MysqlCppcon com RAII usando smart pointers
 * @version 0.1
 * @date 2021-09-04
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "CConfig.hpp"
#include "MysqlRAII.hpp"

auto main(int /*ununsed*/, const char * /*ununsed*/[], const char **envp)
    -> int {
    auto &config = CConfig::config();
    config.load_from_envp(envp);

    dbconninfo conninfo;
    conninfo.host = config["MYSQL_HOST"];
    conninfo.user = config["MYSQL_USER"];
    conninfo.pass = config["MYSQL_PASSWORD"];
    conninfo.db = config["MYSQL_DATABASE"];

    auto &msqlinst = MysqlRAII::singleton();

    auto dbconn = msqlinst.connect_unique(conninfo);

    /**
     * @brief Example with prepared sql statement
     *
     */
    {
        uint64_t id = 1;

        auto prepared_query = MysqlRAII::make_prepstatement(
            dbconn, "SELECT * FROM test WHERE id = ?");

        prepared_query->setUInt64(1, id);

        auto result = MysqlRAII::execute_preparedquery(prepared_query);

        if (result->rowsCount() == 0) {
            std::cout << "Empty query" << std::endl;
        }

        sql::ResultSetMetaData *metadata = result->getMetaData();

        for (unsigned int i = 1; i < metadata->getColumnCount(); i++) {
            std::cout << "|\t" << metadata->getColumnLabel(i);
        }

        std::cout << std::endl;

        while (result->next()) {
            for (unsigned int i = 1; i < metadata->getColumnCount(); i++) {
                std::cout << "|\t" << result->getString(i);
            }
        }

        std::cout << std::endl;
    }

    /**
     * @brief Prepared insert query
     *
     */
    {
        auto prepared_insert = MysqlRAII::make_prepstatement(
            dbconn, "INSERT INTO test (name, created_at, updated_at) VALUES "
                    "(?, NOW(), NOW());");

        prepared_insert->setString(1, "some name"); // Sets the first '?' value

        prepared_insert->execute();
    }

    return 0;
}
