#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "olcPixelGameEngine.h"
#include "Snake.h"
#include "tetris.h"
#include "test.h"
#include "util.h"
#include <conio.h>
#include <stdlib.h>
#include "PS_CPP_PROJECT.h"
struct Player
{
    int sHi, tHi;
    std::string pass, name;
} typedef Player;

bool Login( Player& CurrentPlayer , int& key )
{
    std::string user, pass;
    int n = 0;
    bool newUser = true;
    std::cout << "Enter your username:\n";
    std::cin >> user;
    std::cout << "Enter password:\n";
    std::cin >> pass;
    if (!file_exists("resources/players.txt"))
    {

        std::ofstream fp;
        fp.open("resources/players.txt", std::ios_base::app);
        fp << user + "," + encode(pass, pass.size() + user.size() % 4) + ",0,0";
        fp.close();
        CurrentPlayer = Player{ 0, 0, pass, user };
        return 1;
    }
    else
    {
        std::ifstream leaderboard;
        leaderboard.open("resources/players.txt",std::ios_base::in);
        std::string line;
        
        while (std::getline(leaderboard, line))
        {
            if (line != "")
            {
                n++;
                std::stringstream liness(line);
                std::vector<std::string> info;
                while (liness.good())
                {
                    std::string sub;
                    std::getline(liness, sub, ',');
                    info.push_back(sub);
                }

                
                if (user == info[0])
                {
                    
                    newUser = false;
                    key = pass.size() + user.size() % 4;
                    if (pass == decode(info[1], key))
                    {
                        CurrentPlayer.name = info[0];
                        CurrentPlayer.pass = pass;
                        CurrentPlayer.sHi = stoi(info[2]);
                        CurrentPlayer.tHi = stoi(info[3]);
                        return 1;
                    }
                    else {
                        std::cout << "Wrong password fool!\n";
                        return 0;
                    }
                }
            }
        }
    }
    if (newUser)
    {
        std::ofstream fp;
        fp.open("resources/players.txt", std::ios_base::app);
        key = pass.size() + user.size() % 4;
        if (n > 0) user = "\n" + user;
        fp << user + "," + encode(pass, key )+ ",0,0";
        fp.close();
        CurrentPlayer = Player{ 0, 0, pass, user };
        return 1;
    }
}
void UpdateLeaderBoard(Player& CurrentPlayer, int newS, int newT, int key)
{
    int oldS = CurrentPlayer.sHi, oldT = CurrentPlayer.tHi;
    if (CurrentPlayer.sHi < newS) CurrentPlayer.sHi = newS;
    if (CurrentPlayer.tHi < newT) CurrentPlayer.tHi = newT;
    std::string up = CurrentPlayer.name + "," + encode(CurrentPlayer.pass, key) + "," + std::to_string(CurrentPlayer.sHi) +","+ std::to_string(CurrentPlayer.tHi);
    std::string op = CurrentPlayer.name + "," + encode(CurrentPlayer.pass, key) + "," + std::to_string(oldS) + "," + std::to_string(oldT);
    std::ifstream infile;
    std::ofstream ofile;
    infile.open("resources/players.txt");
    std::string uFile="";
    std::string line;

    while (std::getline(infile, line))
    {
        if (line == "\n") continue;
        int p1 = line.find_first_of(',');
        if (line.substr(0, p1) == CurrentPlayer.name) uFile += up;
        else uFile += line + "\n";
        
    }
    infile.close();
    ofile.open("resources/players.txt");
    ofile << uFile;
    ofile.close();
    
}
void displayBoard() 
{
    system("cls");
    std::ifstream inFile;
    inFile.open("resources/players.txt");
    std::string line;
    std::cout << "User, Snake Highscore, Tetris Highscore\n";
    while (std::getline(inFile, line))
    {
        if (line != "")
        {
            //alin,parola,10,10
            
            std::string r,l;
            int fc = line.find(",");
            l = line.substr(fc + 1, line.size());
            int sc = l.find(",");
            l = l.substr(sc + 1, line.size());
            r = line.substr(0, fc);
            std::cout << r + "," + l << "\n";
            
        }
    }
    inFile.close();
    system("pause");
}
int main()
{
    int h, v, key;
    GetDesktopResolution(h, v);
   
   
    std::string fps;
    Player CurrentPlayer;
    while (1)
    {
        if (Login(CurrentPlayer,key)) break;
    };
    if (CurrentPlayer.name[0] == '\n') CurrentPlayer.name.erase(0, 1);
    int oldS = CurrentPlayer.sHi, oldT = CurrentPlayer.tHi;
    int ch = 1;
    int loopS = 0, loopT = 0;
    while (ch != 0)
    {
        std::cout << "0. Exit\n1. Snake\n2. Tetris\n3. Show Leaderboard.\n";
        std::cin >> ch;
        if (ch == 1)
        {
            Snake snek;
            if (loopS < CurrentPlayer.sHi) snek.setHi(CurrentPlayer.sHi);
            else snek.setHi(loopS);
            if (snek.Construct(35 * 16, 40 * 16, 2, 2, false, false))
                snek.user = CurrentPlayer.name;
                std::cout << "fps:\n";
            std::cin >> fps;
            snek.setFPS(10);
            if (fps.find_first_not_of("0123456789") == std::string::npos and 0 < stoi(fps) and stoi(fps) < 30)
                snek.setFPS(stoi(fps));
            snek.Start();
            loopS = snek.hi;
        }
        if (ch == 2) 
        {
            tetris game;
            if (game.Construct(16 * 18, 19 * 18, 3, 3))
            {
                game.Start();
                loopT = game.nScore > oldT ? game.nScore : oldT;

            }
        }
        if (ch == 3)
        {
            displayBoard();
        }
        system("cls");
        UpdateLeaderBoard(CurrentPlayer, loopS, loopT,  key);
    }
    
   
    return 0;
}
