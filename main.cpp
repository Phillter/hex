#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>
#include <climits>
#include <utility>
#include <chrono>

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
    board(const board& b ):size(b.size) //create a board from an existing board
        {
        hexes.resize(size);
        
        for( int i = 0; i < size; i++)
            {
            hexes[i].resize(size);
            for( int j = 0; j < hexes[i].size(); j++ )
                {
                hexes[i][j] = gamehex(b.hexes[i][j]);
                }
            }
        };
        
    board( const vector<vector<owner>> owners ) : size(owners[0].size()) // create board from 2D list of owned hexes
        {
        hexes.resize(size);
        for( int i = 0; i < size; i++)
            {
            hexes[i].resize(size);
            for( int j = 0; j <hexes[i].size(); j++ )
                {
                hexes[i][j] = gamehex(i, j, owners[i][j]);
                }
            }
        }
    
    board( const vector<owner> owners, int size ) : size(size) // create board from 1D list of owned hexes
        {
        hexes.resize(size);
        for( int i = 0; i < size; i++)
                {
                hexes[i].resize(size);
                }
        for( int i = 0; i < owners.size(); i++)
            {
            hexes[i/size][i%size] = gamehex(i/size, i%size, owners[i]);
            }
        }
    
    friend ostream& operator<<( ostream& out, const board& b )
        {
        cout << "\t\t";
        for(int c = 0; c < b.hexes.size(); c++)
                {
                cout << "P2 start\t";
                }
        cout << endl;
        cout << "\t\t";
        for(int c = 0; c < b.hexes.size(); c++)
            {
            cout << "--------\t";
            }
        cout << endl;
        for( int j = 0; j < b.hexes.size(); j++ )
            {
            cout << endl;
            for(int k = 0; k < j; k++)
                out << "\t";
            cout << "P1 start \\ \t";
            for( int i = 0; i < b.hexes[j].size(); i++ )
                {
                out << j*b.size + i << "-" << b.hexes[i][j] << "\t";
                }
            cout << "\t \\ P1 goal ";
            out << endl;
            }
        cout << endl << "\t";
        for(int c = 0; c < b.hexes.size(); c++)
                {
                cout << "\t";
                }
        for(int c = 0; c < b.hexes.size(); c++)
            {
            cout << "--------\t";
            }
        cout << endl << "\t";
        for(int c = 0; c < b.hexes.size(); c++)
                {
                cout << "\t";
                }
        for(int c = 0; c < b.hexes.size(); c++)
            {
            cout << "P2 goal\t\t";
            }
        cout << endl;
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
    
    int hex_to_num(gamehex& h)
        {
        return h.get_y()*size + h.get_x();
        }
    
    int coords_to_num(int x, int y)
        {
        return y*size + x;
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
    bool get_adj( vector<gamehex> &adj, gamehex h )
        {
        if(h.get_x() < size - 1)
            {
            adj.push_back(hexes[h.get_x()+1][h.get_y()]);
            }
        if(h.get_y() < size - 1)
            {
            adj.push_back(hexes[h.get_x()][h.get_y()+1]);
            }
                
        if( h.get_x() >= 1)
            {
            adj.push_back(hexes[h.get_x()-1][h.get_y()]);
            
            if(h.get_y() < size - 1)
                {
                adj.push_back(hexes[h.get_x()-1][h.get_y()+1]);
                }
            }
        if( h.get_y() >= 1)
            {
            adj.push_back(hexes[h.get_x()][h.get_y()-1]);
            
            if(h.get_x() < size - 1)
                {
                adj.push_back(hexes[h.get_x()+1][h.get_y()-1]);
                }
            }
        }
    
    void set_own( gamehex h, owner o )
        {
        hexes[h.get_x()][h.get_y()].set_own(o);
        }
    
    owner check_for_win()
        {
        //check the board for a win
        vector<int> check;
        owner winner = owner::NONE;
        //check P1 start hexes
        for(gamehex& h : hexes[0])
            {
            if( h.get_own() != owner::NONE)
                {
                winner = win_check(h, check);
                check.clear();
                if(winner != owner::NONE) 
                    return winner;
                }
            }
        //check P2 start hexes
        for( int i = 0; i < size; i++ )
            {
            if(hexes[i][0].get_own() != owner::NONE)
                {
                winner = win_check(hexes[i][0], check);
                check.clear();
                if(winner != owner::NONE) 
                    return winner;
                }
            }
        return winner;
        }

protected:
    vector<vector<gamehex>> hexes;
    int size;
    
    owner win_check(gamehex& h, vector<int> check)
        {
        owner winner = owner::NONE;
        check.push_back(hex_to_num(h));
        //check for win at a given hex
        if((h.get_own() == owner::P1 && h.get_x() == size - 1)
                || ( h.get_own() == owner::P2 && h.get_y() == size - 1 ))
            {
            //we have a winner!
            for( int& n : check)
                {
                cout << n << ", ";
                }
            cout << endl;
            return h.get_own();
            }
        else
            {
            vector<gamehex> adj;
            owner winner = owner::NONE;
            get_adj(adj, h);
            for( gamehex& adj_h : adj)
                {
//                for(int x = 0; x < check.size(); x++) 
//                    cout << "  ";
//                cout << "current check:" ;
//                for(int& n : check)
//                    cout << n << ",";
//                cout << hex_to_num(h) << ". ";
//                cout << " remaining, try " << hex_to_num(adj_h) << "... ";
                
                if( find(check.begin(), check.end(), hex_to_num(adj_h)) == check.end()  && h.get_own() == adj_h.get_own() )
                    {
//                    cout << "Check is now ";
//                    for( int& n : check)
//                        cout << n << "->";
//                    cout << ", Trying " << hex_to_num(adj_h) << endl;
                    winner = win_check(adj_h, check);
                    if( winner != owner::NONE)
                        {
//                        cout << "WINNER FOUND: " << winner << endl;
                        return winner;
                        }
                    }
                else
                    {
//                    cout << "no good" << endl;
                    }
                }
            check.pop_back();
//            cout << h << " is no good..." << endl;
            return winner;
            }
        }
    };

class move_finder
    {
public:

    move_finder( board& b ) 
        {
        board_size = b.get_size();
        real_owners.resize(board_size * board_size);
        for(int i = 0; i < real_owners.size(); i++)
            {
            real_owners[i] = b.get_hex(i).get_own();
            }
        };

    int generate_game() 
        {
        bool is_p1 = true;
        int r, candidate;
        int c = count(real_owners.begin(), real_owners.end(), owner::NONE);
        owners = real_owners;
        //fill board with random moves, alternating players
        for(int i = 0; i < c; i++)
            {
            r = rand()%owners.size();
            while(owners[r] != owner::NONE)
                {
                if( r == owners.size())
                    r = 0;
                else
                    r++;
                }
            is_p1 ? owners[r] = owner::P1 : owners[r] = owner::P2;
            if(i == 0 && is_p1 == false)
                {//save off first move for computer move candidate
                candidate = r;
                }
            is_p1 = !is_p1;
            }
        return candidate;
        };
        
        int find_best_move()
            {
            int current_candidate = 0;
            vector<int> candidates(board_size * board_size - 1, 0);
            board b = board(board_size);
            time_t end_time;
            auto start_time = time(NULL);
            
            while( difftime(end_time, start_time) < 1.0 )
                {//generate a new game
                current_candidate = generate_game();
                b = board(this->get_game(), board_size);
                if(b.check_for_win() == owner::P2)
                    {
                    //computer win, save candidate as potential winning move
                    candidates[current_candidate]++;
                    }
                this->reset_game();
                end_time = time(NULL);
                }
            //select move based on which hex as the first move when randomizing won the most games
            //vector<int>::iterator it = find(candidates.begin(), candidates.end(), max_element(candidates.begin(), candidates.end()));
            int biggest = 0;
            int location = 0;
            for(int i = 0; i < candidates.size(); i++)
                {
                if(candidates[i] > biggest)
                    {
                    biggest = candidates[i];
                    location = i;
                    }
                }
            return location;
            }
        
        reset_game()
            {
            //reset a generated game to the original state
            owners = real_owners;
            }
        
    vector<owner> get_game()
        {
        return owners;
        }
    int get_board_size()
        {
        return board_size;
        }
            
protected:
    vector<owner> owners;
    vector<owner> real_owners;
    int board_size;
    };
    
int main( int argc, char** argv )
    {
    srand(time(NULL));
    board b = board(4);
    b.set_own(b.get_hex(0, 1), owner::P1);
    b.set_own(b.get_hex(0, 3), owner::P1);
    b.set_own(b.get_hex(1, 1), owner::P1);
//    b.set_own(b.get_hex(2, 1), owner::P1);
//    b.set_own(b.get_hex(3, 0), owner::P1);
    
    b.set_own(b.get_hex(1, 0), owner::P2);
    b.set_own(b.get_hex(1, 2), owner::P2);
    b.set_own(b.get_hex(1, 3), owner::P2);
//    
    cout << b << endl;
//    vector<gamehex> adj;
//    gamehex hex;
//    for( int j = 0; j < b.get_size(); j++)
//        {
//        for( int i = 0; i < b.get_size(); i++)
//            {
//            hex = b.get_hex(i, j);
//            b.get_adj(adj, hex);
//            cout << b.hex_to_num(hex) << " - ADJ: ";
//            
//            for(gamehex& h : adj)
//                {
//                cout << b.hex_to_num(h) << "\t";
//                }
//            cout << endl;
//            adj.clear();
//            }
//        }
//    cout << b << endl;
//    vector<gamehex> adj;
//    b.get_adj(adj, b.get_hex(1, 1));
    
//    board b2 = board(b);
//    for(int i = 0; i < b.get_size() * b.get_size(); i++)
//            cout <<  i << b.get_hex(i).get_own() << endl;
    move_finder mf = move_finder(b);
//    board b2 = board(4);
//    int wins [2] = {0, 0};
//    owner winner;
////    cout << b.check_for_win() << endl;
//    for(int x = 0; x < 100; x++)
//        {
//        mf.generate_game();
//        b2 = board(mf.get_game(), mf.get_board_size());
//        winner = b2.check_for_win();
//        if(winner == owner::P1)
//            wins[0]++;
//        else if(winner == owner::P2)
//            wins[1]++;
//        
//        cout << b2 << endl;
//        cout << winner << endl;
//        cout << "Wins: " << wins[0] << ", " << wins[1] << endl;
//        mf.reset_game();
//        }
    return 0;
    }

