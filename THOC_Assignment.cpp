#include<bits/stdc++.h>
using namespace std;

class State
{
     public:
     int state_no;
     int partition_no; //partition to which the state belongs
     vector<int> transition;//state_no of the states reached on each input symbol
     char status; //Final or non-final state
     void set_details(int);
};
set<int> r;
int states,Partition,symbol;


void MinimizeDFA(State s[])
{
    int reached[states];//stores partition_no of the state reached when a particular input is given
    for(int p=0;p<states-2;p++)
    {
        for(int j=0;j<symbol;j++) //j=0 --> symbol='0' , j=1 --> symbol='1' ...
        {
            //Initialize 'reached' for input symbol j
            for(int i=0;i<states;i++)
            {
                int temp=s[i].transition[j];
                reached[i]=s[temp].partition_no;
            }

            for(int i=0;i<states-1;i++)
            {
                int record[states];//record of the states that form new partition during this iteration
                memset(record,-1,sizeof(record));

                for(int k=i+1;k<states;k++)
                {
                    if(s[i].partition_no==s[k].partition_no && reached[i]!=reached[k])
                    {
                        int q =-1;
                        for(int l=0;l<states;l++)//checking whether s[k] can fall in some other already existing partition or it will form a new partition
                        {
                            if(reached[k]==record[l])
                            {
                                q=l;
                                break;
                            }
                        }

                        if(q==-1)//s[k] forms a new partition
                        {
                            s[k].partition_no=Partition;
                            Partition++;
                            record[k]=reached[k];
                        }
                        else//s[k] will fall in some other partition
                        {
                            s[k].partition_no=s[q].partition_no;
                        }
                    }
                }
            }

        }

        int equalPartition=p;
        for(int i=0;i<states;i++)
        {
            if(s[i].partition_no!=reached[i])
            {
                equalPartition=-1;
                break;
            }
        }

        if(equalPartition!=-1)
        {
            return;
        }

    }
}

//get the minimum equivalence partition
void GetPartition(State s[])
{
    cout<<"\n******************Minimum Equivalence Partition*****************\n";
    for(int i=0;i<Partition;i++)
    {
        for(int j=0;j<states;j++)
        {
            if(s[j].partition_no==i){
                if(s[j].state_no!=0 && r.find(s[j].state_no)==r.end())continue;
                cout<<"q"<<s[j].state_no<<" ";
            }

        }
        cout<<"\n";
    }
}

//Function to represent the states belonging to a particular partition by a single state
void group(State s[])
{
    for(int i=0;i<states-1;i++)
    {
        if(s[i].state_no!=0 && (r.find(s[i].state_no)==r.end())){s[i].state_no=-1;continue;}
        if(s[i].state_no!=-1)
        {
            for(int j=i+1;j<states;j++)
            {
                //If state s[i] and s[j] belong to the same group then s[j] is represented by s[i]
                if(s[i].partition_no ==s[j].partition_no)
                {
                    for(int k=0;k<states;k++)
                    {
                        for(int l=0;l<symbol;l++)
                        {
                            if(s[k].transition[l]==s[j].state_no)
                            s[k].transition[l]=i;
                        }
                    }

                    s[j].state_no=-1; //state_no = -1 denotes this state is represented by some other state

                    if(s[j].status=='F' || s[j].status=='f')
                    s[i].status='F';
                }
            }
        }


    }
}

//transition table for minimized DFA
void transitionTable(State s[])
{
    cout<<"\n\n\n*******************Transition Table******************\n";
    cout<<"\n\n";
    cout<<"States |           Input\n";
    cout<<"       |";
    for(int i=0;i<symbol;i++)
    {
        cout<<"     "<<i;
    }
    cout<<"\n";
    for(int i=0;i<states;i++)
    {
        if((s[i].state_no==-1) || (s[i].state_no==states-1 && (r.find(s[i].state_no)==r.end()))) //if this state is represented by some other state then no need to include it
        continue;

        cout<<"\n   q"<<s[i].state_no<<" |";
        for(int j=0;j<symbol;j++)
        {
            cout<<"     q"<<s[i].transition[j];
        }
        if(s[i].state_no==0)
        cout<<"     Initial state";
        if((s[i].status=='f'  ||   s[i].status=='F')&&(s[i].state_no==0))
        cout<<" and Final state";
        else if(s[i].status=='f'  ||   s[i].status=='F')
        cout<<"     Final state";
    }
}


void State::set_details(int symbol)
{
        transition.resize(symbol);
        cout<<"--------------------------------------------------------\n";
        cout<<"State: q";
        cin>>state_no;
        cout<<"Final or non-final state(f/n): ";
        cin>>status;

        //Initially there are two partition, final and non-final
        if(status=='n' || status=='N')
        partition_no=0;
        else if(status=='f' || status=='F')
        partition_no=1;

        cout<<"\nEnter the transition states for the following symbols: \n";
        for(int i=0;i<symbol;i++)
        {
            cout<<"Symbol "<<i<<": q";
            cin>>transition[i];
            r.insert(transition[i]);

        }

}
int main()
{
    cout<<"********************************************************\n";
    cout<<"Enter number of states: ";
    cin>>states;
    cout<<"Enter number of input symbols: ";
    cin>>symbol;
    State s[states];
    for(int i=0;i<states;i++)
    {
        s[i].set_details(symbol);
    }
    Partition=2; //Initially there are two partitions, final and non-final states
    MinimizeDFA(s);
    GetPartition(s);
    group(s);
    transitionTable(s);

    return 0;
}

