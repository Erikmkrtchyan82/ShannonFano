#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class shannon_fano {

public:

    shannon_fano( const string &str ) : string_( str ) {
        map< char, unsigned > sym;
        double total = str.size();
        for ( auto &x:str ) {
            ++sym[ x ];
        }

        for ( auto&[s, p]:sym ) {
            symbols.push_back( {
                                       s, p / total
                               } );
        }

        sort_by_probability();
    }

    string decode( const string codes ) {

        ostringstream oss;
        istringstream iss( codes );
        string code{};

        while ( !iss.eof()) {
            code += ( char ) iss.get();
            auto x = find_if( symbols.begin(), symbols.end(),
                              [&]( sym_prob &symProb ) {
                                  return symProb.code == code;
                              } );
            if ( x != symbols.end()) {
                oss << x->sym;
                code.clear();
            }
        }
        return oss.str();
    }

    string decode( const vector< string > &codes ) {
        string str;
        for ( auto &x:codes ) {
            for ( auto &y:symbols ) {
                if ( x == y.code ) {
                    str += y.sym;
                    break;
                }
            }
        }
        return str;
    };

    void encode() {
        encode( 0, symbols.size() - 1 );
    }

    void print() {
        for ( auto &[s, p, c]:symbols ) {
            cout << s << ':' << p << '\t' << c << endl;
        }
        cout << endl;
        for ( auto &s:string_ ) {
            auto x = find_if( symbols.begin(), symbols.end(), [&]( sym_prob &symProb ) {
                return symProb.sym == s;
            } );
            cout << x->code;
        }
        cout << endl;
    }

private:

    void encode( int first, int last ) {

        if ( first == last )
            return;

        if ( last - first == 1 ) {
            symbols[ first ].code += '0';
            symbols[ last ].code += '1';
            return;
        }

        double half = 0, prev = 0, total = 0;

        for ( int j = first; j <= last; ++j )
            total += symbols[ j ].prob;

        int i;

        for ( i = first; i <= last; ++i ) {
            prev = half;
            half += symbols[ i ].prob;
            if ( half >= total / 2 )
                break;
        }

        if ( half > total / 2 ) {
            if ( prev != 0 && ( total - prev ) - prev < half - ( total - half )) {
                --i;
                half = prev;
            }
        }

        for ( int j = first; j <= i; ++j ) {
            symbols[ j ].code += '0';
        }

        for ( int j = i + 1; j <= last; ++j ) {
            symbols[ j ].code += '1';
        }

        encode( first, i );
        encode( i + 1, last );
    };

    void sort_by_probability() {
        std::sort( symbols.begin(), symbols.end(), []( sym_prob &a, sym_prob &b ) {
            return a.prob > b.prob;
        } );
    }

private:

    struct sym_prob {
        char sym;
        double prob;
        string code{};
    };

    vector< sym_prob > symbols;
    string string_;

};

int main() {

    string str = "AABCDAABCA";
//    cin >> str;

    shannon_fano sh( str );
    sh.encode();
    sh.print();

    cout << sh.decode( "001011011100101100" ) << endl << str << endl;

    return 0;
}
