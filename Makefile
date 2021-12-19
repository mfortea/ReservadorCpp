MYSQL_CONCPP_DIR = /usr/local/mysql-connector-c++-8.0.22
CPPFLAGS = -I $(MYSQL_CONCPP_DIR)/include -L $(MYSQL_CONCPP_DIR)/lib64
LDLIBS = -lmysqlcppconn8
CXXFLAGS = -std=c++11
reservador : reservador.cc

