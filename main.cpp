#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>
#include <climits>
#include <utility>

using namespace std;

enum class owner : short
    {
    NONE, P1, P2
    };

ostream& operator<<( ostream& out, const owner& o )
    {
    switch( o )
        {
        case owner::NONE: out << "NONE"; break;
        case owner::P1: out << "P1  "; break;
        case owner::P2: out << "P2  "; break;
        }
    return out;
    }

//gamehex class
//contains coords (x and y) and who owns this gamehex
class gamehex
    {
public:
    gamehex( int x = 0, int y = 0, owner o = owner::NONE ):x(x), y(y), own(o){};
    
    gamehex( const gamehex& h)
        {
        x = h.x;
        y = h.y;
        own = h.own;
        };
    
    friend ostream& operator<<( ostream& out, const gamehex& n )
        {
        out << n.x << "," << n.y << ":" << n.own;
        return out;
        }

    void set_own( owner o )
        {
        this->own = o;
        }
    
    int get_x( )
        {
        return this->x;
        }
    int get_y( )
        {
        return this->y;
        }
    owner get_own( )
        {
        return this->own;
        }

private:
    int x;
    int y;
    owner own;
    };

class board
    {
public:
    
    board( int size ): size(size)//create game board of n hexes
        {
        hexes.resize(size);
        for( int i = 0; i < size; i++ )
            {
            hexes[i].resize(size);
            for( int j = 0; j < size; j++ )
                {
                hexes[i][j] = gamehex(i, j, owner::NONE);
                }
            }
        }
    board(const board& b )
        {
        for( int i = 0; i < b.hexes.size(); i++)
            {
            for( int j = 0; j < b.hexes[i].size(); j++ )
                {
                hexes[i][j] = gamehex(b.hexes[i][j]);
                }
            }
        };
    
    friend ostream& operator<<( ostream& out, const board& b )
        {
        for( int i = 0; i < b.hexes.size(); i++ )
            {
            for(int k = 0; k < i; k++)
                out << "\t";
            for( int j = 0; j < b.hexes[i].size(); j++ )
                {
                out << b.hexes[i][j] << "\t";
                }
            out << endl << endl;
            }
        return out;
        }
    
    int get_size( )
        {
        return this->size;
        }
    
    gamehex get_hex(int x, int y )
        {
        return this->hexes[x][y];
        }
    
    gamehex get_hex(int n)
        {
        return this->hexes[n/size][n%size];
        }

    bool is_adj( int x1, int y1, int x2, int y2 )
        {
        return( x1 - 1 == x2 && y1 == y2 )
                || ( x1 - 1 == x2 && y1 + 1 == y2 )
                || ( x1 == x2 && y1 - 1 == y2 )
                || ( x1 == x2 && y1 + 1 == y2 )
                || ( x1 + 1 == x2 && y1 - 1 == y2 )
                || ( x1 + 1 == x2 && y1 == y2 );
        }
    
    void set_own( gamehex h, owner o )
        {
        hexes[h.get_x()][h.get_y()].set_own(o);
        }

protected:
    vector<vector<gamehex>> hexes;
    int size;
    };

class move_finder
    {
public:

    move_finder( board& b ) 
        {
        real_owners.resize(b.get_size() * b.get_size());
        for(int i = 0; i < real_owners.size(); i++)
            real_owners[i] = b.get_hex(i).get_own();
        };

    generate_game() 
        {
        srand(time(0));
        bool is_p1 = true;
        int r;
        int c = count(real_owners.begin(), real_owners.end(), owner::NONE);
        owners = real_owners;
        //fill board with random moves, alternating players
        for(int i = 0; i < c; i++)
            {
            r = rand()%owners.size();
            while(owners[r] != owner::NONE)
                {
                if( r = owners.size())
                    r = 0;
                else
                    r++;
                }
            is_p1 ? owners[r] = owner::P1 : owners[r] = owner::P2;
            is_p1 = !is_p1;
            }
        };
        
        owner who_wins()
            {
            
            }
            
protected:
    vector<owner> owners;
    vector<owner> real_owners;
    };
    
int main( int argc, char** argv )
    {
    board b = board(4);
    cout << b << endl;
    b.set_own(b.get_hex(0, 1), owner::P1);
    cout << b << endl;
    return 0;
    }

