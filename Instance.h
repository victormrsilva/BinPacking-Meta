#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <string>
#include <vector>


class Instance{
public:

    Instance( const std::string &fileName);

    int n() const { return n_; } // number of jobs

    int c() const { return c_;} // number of machines

    int w(int i) const { return pesos [i]; }

    bool conflito( int i, int j ) const { return conflitos[i][j]; }

    int qtdItensConflito(int i) const;

    //int maxTime() const { return timeout; };

    void writeInstance( const std::string fname ) const;

private:
    int n_;
    int c_;

    int timeout; // max time

    std::string instance_name;

    std::vector< int > pesos;

    std::vector< std::vector< bool > > conflitos;
    
    const std::vector<std::string> explode(const std::string& s, const char& c);

};

#endif

