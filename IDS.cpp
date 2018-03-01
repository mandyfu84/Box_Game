//
//  main.cpp
//  IDS2
//
//  Created by Fu Juo Wen on 2017/4/9.
//  Copyright © 2017年 ntnu. All rights reserved.
//

#include <iostream>
#include <deque>
#include <map>
#include <string>
#include <stdio.h>
#include <limits.h>
#include <time.h>
/*
 These are the numbers that represent different blocks.
 1 ** (ans)
 2 *
 3 **
 4 *
   *
 5 ** (ans)
   **
 */

using namespace std;


struct Point
{
    int x=0, y=0;
};

map<string, int> visitedSet; // for checking duplicated states
int vnodes=0; // count all visited nodes

int M=3;
int N=3;


class State
{
public:
    int state[10][10]={{2,1,1},{0,2,2},{0,2,2}};
    //int state[10][10]={{0,4,1,1},{0,4,2,2},{3,3,2,2}};
    //int state[10][10]={{2,4,5,5},{2,4,5,5},{0,2,3,3},{2,0,2,2}};
    //int state[10][10]={{2,4,3,3,1,1},{2,4,2,2,2,2},{2,2,2,4,2,2},{0,0,2,4,2,2}};
    
    string ss; // change 2d array state to string
    int depth=0;
    
    State()
    {
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; ++j)
            {
                ss.append(to_string(state[i][j]));
            }
        }
    }
    
    State(int _state[10][10], int _depth)
    {
        
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; ++j)
            {
                state[i][j]=_state[i][j];
                ss.append(to_string(state[i][j]));
            }
        }
        depth=_depth;
    }
    
    void print(){
       // cout << "-----------------" << endl;
        printf("Depth=%d\n", depth);
        for(int i=0; i<M; ++i){
            for(int j=0; j<N; ++j)
            {
                cout << state[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "-----------------" << endl;
    }
    
    int findzero(Point p[]){ // find two blank position
        int n=0;
        for (int i=0; i<M; i++){
            for(int j=0; j<N; j++)
                if (state[i][j]==0 )
                {
                    p[n].x=i;
                    p[n].y=j;
                    //printf("i=%d j=%d\n", i, j);
                    n++;
                }
        }
        return 0;
    }
    
    void exch(int i1, int j1, int i2, int j2) // exchange two blocks
    {
        int t=state[i2][j2];
        state[i2][j2]=state[i1][j1];
        state[i1][j1]=t;
        
        ss.clear();
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; ++j)
            {
                ss.append(to_string(state[i][j]));
            }
        }
    }
    
    int goal() // check goal state
    {
        if(state[M-1][0]==1 && state[M-1][1]==1) // goal block is **
        {
            return 1;
        }
        else if (state[M-1][0]==5 && state[M-1][1]==5 && state[M-2][0]==5 && state[M-2][1]==5) // goal block is 2*2
        {
            return 1;
        }
        else return 0;
    }
    
    void setVisited()
    {
        if(visitedSet[ss]==0 || depth<visitedSet[ss])
        {
            //cout << "add " << ss << " depth=" << depth << " visitssd=" << visitedSet[ss] << endl ;
            visitedSet[ss]=depth;
            vnodes+=1;
            //cout << "newvdepth=" << visitedSet[ss] << endl;
        }
        
    }
    
    int canAdd2Frontier()
    {
        auto it=visitedSet.find(ss);
        if(it==visitedSet.end()) //not found in closed set
        {
            return 1;
        }
        else if(it->second>depth)//state in closed set is deeper
        {
            return 1;
        }
        return 0;
    }

    void expand(deque<State> *frontier) // expand new states
    {
        Point p0[2];
        
        findzero(p0);
        
        int x[2]={0},y[2]={0};
        x[0]=p0[0].x;
        y[0]=p0[0].y;
        x[1]=p0[1].x;
        y[1]=p0[1].y;
        
        
        for(int i=0; i<2; ++i) // there are two blanks, so check two times
        {
            if(x[i]!=M-1)//not last row, can move up
            {
               // printf("1\n");
                if(state[x[i]+1][y[i]]!=0) //*
                {
                    State s(state,depth+1);
                    
                    if(state[x[i]+1][y[i]]==2) //*
                    {
                        s.exch(x[i], y[i], x[i]+1, y[i]);
                    }
                    else if(state[x[i]+1][y[i]]==4) //*
                    {                               //*
                        s.exch(x[i], y[i], x[i]+2, y[i]);
                    }
                    else if(state[x[i]+1][y[i]]==1 || state[x[i]+1][y[i]]==3) //**
                    {
                        if(y[i]!=N-1)// not most right
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]+1][y[i]+1]==state[x[i]+1][y[i]])// 0's rhs is also 0
                            {
                                s.exch(x[i], y[i], x[i]+1, y[i]);
                                s.exch(x[i], y[i]+1, x[i]+1, y[i]+1);
                            }
                        }
                    }
                    else if(state[x[i]+1][y[i]]==5) // 2*2
                    {
                        if(y[i]!=N-1 && x[i]<M-2) // not most right and not bottom
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]+1][y[i]+1]==5 && state[x[i]+2][y[i]]==5 && state[x[i]+2][y[i]+1]==5)
                            {
                                s.exch(x[i], y[i], x[i]+2, y[i]);
                                s.exch(x[i], y[i]+1, x[i]+2, y[i]+1);
                            }
                        }
                    }
                    if(s.canAdd2Frontier())
                    {
                        //printf("%d up\n", i);
                        (*frontier).push_back(s);
                        s.setVisited();
                    }

                }

            }
            if(x[i]!=0)// not first row, can move down
            {
                //printf("2\n");
                
                if(state[x[i]-1][y[i]]!=0) //*
                {
                    State s(state,depth+1);
                    
                    if(state[x[i]-1][y[i]]==2) //*
                    {
                        s.exch(x[i], y[i], x[i]-1, y[i]);
                    }
                    else if(state[x[i]-1][y[i]]==4) //*
                    {                               //*
                        s.exch(x[i], y[i], x[i]-2, y[i]);
                    }
                    else if(state[x[i]-1][y[i]]==1 || state[x[i]-1][y[i]]==3) //**
                    {
                        if(y[i]!=N-1)// not most right
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]-1][y[i]+1]==state[x[i]-1][y[i]])// 0's rhs is also 0
                            {
                                s.exch(x[i], y[i], x[i]-1, y[i]);
                                s.exch(x[i], y[i]+1, x[i]-1, y[i]+1);
                            }
                        }
                    }
                    else if(state[x[i]-1][y[i]]==5) //2*2
                    {
                        if(y[i]!=N-1 && x[i]>1)
                        {
                            if(state[x[i]][y[i]+1]==0 && state[x[i]-1][y[i]+1]==5 && state[x[i]-2][y[i]]==5 && state[x[i]-2][y[i]+1])// 0's rhs is also 0
                            {
                                s.exch(x[i], y[i], x[i]-2, y[i]);
                                s.exch(x[i], y[i]+1, x[i]-2, y[i]+1);
                            }
                        }
                    }
                    
                    if(s.canAdd2Frontier())
                    {
                        //printf("%d down\n", i);
                        
                        (*frontier).push_back(s);
                        s.setVisited();
                    }

                }

            }
            if(y[i]!=N-1)// not most right col, can move left
            {
                //printf("3\n");
                
                if(state[x[i]][y[i]+1]!=0)
                {
                    
                    State s(state,depth+1);
                    
                    if(state[x[i]][y[i]+1]==2) //*
                    {
                        s.exch(x[i], y[i], x[i], y[i]+1);
                    }
                    else if (state[x[i]][y[i]+1]== 3 || state[x[i]][y[i]+1] == 1) //**
                    {
                        s.exch(x[i], y[i], x[i], y[i]+2);
                    }
                    else if (state[x[i]][y[i]+1]==4) //*
                    {                                //*
                        if(x[i]!=M-1)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]+1]==state[x[i]][y[i]+1])// below 0 is also 0
                            {
                                s.exch(x[i], y[i], x[i], y[i]+1);
                                s.exch(x[i]+1, y[i], x[i]+1, y[i]+1);
                            }
                        }
                    }
                    else if(state[x[i]][y[i]+1]==5) //2*2
                    {
                        if(x[i]!=M-1 && y[i]<N-2)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]+1]==5 && state[x[i]][y[i]+2]==5 && state[x[i]+1][y[i]+2]==5)// below 0 is also 0
                            {
                                s.exch(x[i], y[i], x[i], y[i]+2);
                                s.exch(x[i]+1, y[i], x[i]+1, y[i]+2);
                            }
                        }
                        
                    }
                    if(s.canAdd2Frontier())
                    {
                        //printf("%d left\n", i);
                        
                        (*frontier).push_back(s);
                        s.setVisited();
                    }

                }

            }
            if(y[i]!=0)//not most left col, can move right
            {
                //printf("4\n");
                if(state[x[i]][y[i]-1]!=0)
                {
                    State s(state,depth+1);
                    
                    if(state[x[i]][y[i]-1]==2) //*
                    {
                        s.exch(x[i], y[i], x[i], y[i]-1);
                    }
                    else if (state[x[i]][y[i]-1]==3 || state[x[i]][y[i]-1]==1) //**
                    {
                        s.exch(x[i], y[i], x[i], y[i]-2);
                    }
                    else if(state[x[i]][y[i]-1]==4) //*
                    {                               //*
                        if(x[i]!=M-1)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]-1]==state[x[i]][y[i]-1])// below 0 is also 0
                            {
                                s.exch(x[i], y[i], x[i], y[i]-1);
                                s.exch(x[i]+1, y[i], x[i]+1, y[i]-1);
                            }
                        }
                    }
                    else if(state[x[i]][y[i]-1]==5) //2*2
                    {
                        if(x[i]!=M-1 && y[i]>1)//not button row
                        {
                            if(state[x[i]+1][y[i]]==0 && state[x[i]+1][y[i]-1]==5 && state[x[i]][y[i]-2]==5 && state[x[i]+1][y[i]-2]==5)// below 0 is also 0[
                            {
                                s.exch(x[i], y[i], x[i], y[i]-2);
                                s.exch(x[i]+1, y[i], x[i]+1, y[i]-2);
                            }
                        }
                    }
                    if(s.canAdd2Frontier())
                    {
                        //printf("%d right\n", i);
                        (*frontier).push_back(s);
                        s.setVisited();
                    }
                }

            }
        }
    }
    
   
    int dfs(int limit)
    {
        deque<State> frontier;
        frontier.push_back(*this);
        setVisited();
        
        while(!frontier.empty())
        {
                if(frontier.back().goal()==1)
                {
                    printf("Solution Found:\n");
                    frontier.back().print();
                    frontier.clear();
                    return 1;
                }
                else
                {
                    if(frontier.back().depth >= limit )
                    {
                        //printf("over limit, pop\n");
                        frontier.pop_back();
                    }
                    else
                    {
                        State tmps;
                        tmps = frontier.back();
                        frontier.pop_back();
                        tmps.print();
                        tmps.expand(&frontier);
                    }
                }
        }
       //if(frontier.empty()) printf("Solution Not Found\n");
        return 0;
    }
    
};

int main(int argc, const char * argv[]) {

    State initS;
    
    FILE *fp = fopen("level1.txt", "r");
    
    fscanf(fp, "%d %d", &M, &N);
    for(int i=0; i<M; ++i)
    {
        for(int j=0; j<N; ++j)
        {
            fscanf(fp, "%d", &initS.state[i][j]);
        }
    }
    
    clock_t t1,t2;
    t1=clock();
    
    for(int i=0;; ++i)
    {
        printf("--Limit=%d--\n",i);
        State s(initS.state, 0);
        if(s.dfs(i)==1)
        {
            printf("found!!\n");
            t2=clock();
            printf("Total time = %f seconds.\n", ((double)t2-(double)t1)/CLOCKS_PER_SEC);
            printf("Visited %d nodes.\n", vnodes);
            fclose(fp);
            return 0;
        }
        visitedSet.clear();
    }
    return 0;
}
