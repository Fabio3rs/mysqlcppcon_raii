#pragma once

#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>

#include <mutex>
#include <string_view>

/**
 * @brief Definições dos tipos nativos do MySQL Cpp Connector + smart pointers
 *
 */
using unique_conn_t = std::unique_ptr<sql::Connection>;
using shared_conn_t = std::shared_ptr<sql::Connection>;
using unique_statement_t = std::unique_ptr<sql::Statement>;
using unique_prepstatement_t = std::unique_ptr<sql::PreparedStatement>;
using unique_resultset_t = std::unique_ptr<sql::ResultSet>;

/**
 * @brief Dados de conexão com o banco de dados MySQL
 *
 */
struct dbconninfo {
    std::string host, user, pass; // Dados de autenticação com o servidor
    std::string db;               // Schema/database a ser selecionada
};

class MysqlRAII {
    std::mutex sqldrvmtx;

    MysqlRAII() = default;
    MysqlRAII(const MysqlRAII &) = delete;

  public:
    static auto singleton() -> MysqlRAII &;

    /**
     * @brief Cria uma conexão ao banco de dados e devolve a instância da
     * conexão
     *
     * @param info dados para conexão
     * @return unique_conn_t instância da conexão
     */
    auto connect_unique(const dbconninfo &info) -> unique_conn_t;

    /**
     * @brief Cria uma conexão ao banco de dados e devolve a instância da
     * conexão; objeto tipo shared_ptr; OBS: não usar em threads concorrentes
     *
     * @param info dados para conexão
     * @return shared_conn_t instância da conexão
     */
    auto connect_shared(const dbconninfo &info) -> shared_conn_t;

    /**
     * @brief Cria um objeto RAII de um statement
     *
     * @tparam sqlconnection_T tipo do objeto unique_conn_t/shared_conn_t (ou
     * mesmo ponteiros raw...)
     * @param conn objeto de conexão existente
     * @return unique_statement_t objeto raii de um statement
     */
    template <class sqlconnection_T>
    static auto make_statement(sqlconnection_T &conn) -> unique_statement_t {
        return unique_statement_t(conn->createStatement());
    }

    /**
     * @brief Cria um objeto RAII de um prepared statement
     *
     * @tparam sqlconnection_T tipo do objeto unique_conn_t/shared_conn_t (ou
     * mesmo ponteiros raw...)
     * @param conn objeto de conexão existente
     * @param query query para inicializar o prepared statement
     * @return unique_prepstatement_t objeto raii de um prepared statement
     */
    template <class sqlconnection_T>
    static auto make_prepstatement(sqlconnection_T &conn,
                                   std::string_view query)
        -> unique_prepstatement_t {
        return unique_prepstatement_t(conn->prepareStatement(query.data()));
    }

    /**
     * @brief Executa uma query preparada em um sql prepared statement
     *
     * @param stmt instância do prepared statement
     * @return unique_resultset_t instância de resultado da query
     */
    static auto execute_preparedquery(unique_prepstatement_t &stmt)
        -> unique_resultset_t {
        return unique_resultset_t(stmt->executeQuery());
    }

    /**
     * @brief Executa uma query comum
     *
     * @param stmt instância do statement
     * @return unique_resultset_t instância de resultado da query
     */
    static auto execute_query(unique_statement_t &stmt, std::string_view query)
        -> unique_resultset_t {
        return unique_resultset_t(stmt->executeQuery(query.data()));
    }
};
