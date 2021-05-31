#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


class Task
{
public:
    int r;
    int p;
    int q;
    int endTime;
};

class schrageOutput
{
public:
    int cmax;
    vector<Task> PI;
};


istream &operator>>(istream &input, Task &ToDoTask)
{
    input >> ToDoTask.r >> ToDoTask.p >> ToDoTask.q;
    return input;
}

ostream &operator<<(ostream &output, Task &ToDoTask)
{
    output << "r:" << ToDoTask.r << " p:" << ToDoTask.p << " q:" << ToDoTask.q << " endTime: " << ToDoTask.endTime;
    return output;
}

struct N_collection
{
    bool operator()(Task &x, Task &y)
    {
        return x.r > y.r;
    }
};

struct G_collection
{
    bool operator()(Task &x, Task &y)
    {
        return x.q < y.q;
    }
};

vector<Task> Load(vector<Task> taskVect, string fileNumber)
{
    int i = 0;
    int n_lines;
    ifstream file;
    file.open("SCHRAGE" + fileNumber + ".DAT");
    file >> n_lines;
    vector<Task> v_tasks(n_lines);
    while (!file.eof())
    {
        file >> v_tasks[i];
        
        // N_set.push(v_tasks[i]);
        i++;
    }
    file.close();
    taskVect = v_tasks;
    return taskVect;
}

int schragePodzial(vector<Task> &permutation)
{
    int t = 0, k = 0, Cmax = 0;
    priority_queue<Task, vector<Task>, G_collection> G_set;
    priority_queue<Task, vector<Task>, N_collection> N_set;
    Task e;
    Task l;
    l.q = 0;
    l.p = 0;
    l.r = 0;

    for(int i = 0; i < permutation.size(); i++){
        Task temp = permutation[i];
        N_set.push(temp);
    }

    while ((!G_set.empty()) || (!N_set.empty()))
    {
        while ((!N_set.empty()) && (N_set.top().r <= t))
        {
            e = N_set.top();
            G_set.push(e);
            N_set.pop();
            if (e.q > l.q)
            {
                l.p = t - e.r;
                t = e.r;
                if (l.p > 0)
                {
                    G_set.push(l);
                }
            }
        }
        if (G_set.empty())
        {
            t = N_set.top().r;
        }
        else
        {
            e = G_set.top();
            G_set.pop();
            l = e;
            k++;
            t = t + e.p;
            Cmax = max(Cmax, t + e.q);
        }
    }
    // cout << "Cmax: " << Cmax << endl;
    return Cmax;
}

int find_B(int Cmax, vector<Task> &taskVect)
{
    int b_position = -1;
    for (int i = taskVect.size() - 1; i > 0; i--)
    {
        if (Cmax == taskVect[i].endTime + taskVect[i].q) 
        {
            b_position = i;
            break;
        }
    }
    return b_position;
}

int findA(int Cmax, vector<Task> &taskVect, int b_position)
{
    int a_position = -1;
    
    
    for (int i = 0 ; i < taskVect.size(); i++)
    {
        int p_sum = 0;
    
        for (int s = i; s <= b_position; s++)
        {
            p_sum += taskVect[s].p;            
        }
        if (Cmax == taskVect[i].r + p_sum + taskVect[b_position].q)
        {
            a_position = i;
            break;
        }
    }
    
    return a_position;
}

int findC(vector<Task> &taskVect, int b_position, int a_position)
{
    int c_position = -1;
    for (int i = b_position; i >= a_position; i--)
    {
        // cout << "taskVect[i].q: " << taskVect[i].q << endl;
        // cout << "taskVect[b_position].q: " << taskVect[b_position].q << endl;
        if (taskVect[i].q < taskVect[b_position].q)
        {
            c_position = i;
            break;
        }
    }
    return c_position;
}

schrageOutput Schrage(vector<Task> &taskVect)
{
    schrageOutput result;
    int t = 0, k = 0, Cmax = 0;
    int index = 0;
    Task e;
    priority_queue<Task, vector<Task>, G_collection> G_set;
    priority_queue<Task, vector<Task>, N_collection> N_set;
    
    for(int i = 0; i < taskVect.size(); i++){
        Task temp = taskVect[i];
        N_set.push(temp);
    }
   
    while ((!G_set.empty()) || (!N_set.empty()))
    {
        while ((!N_set.empty()) && (N_set.top().r <= t))
        {
            e = N_set.top();
            G_set.push(e);
            N_set.pop();
        }
        if (G_set.empty())
        {
            t = N_set.top().r;
        }
        else
        {
            e = G_set.top();
            G_set.pop();
            k++;
            t = t + e.p;
            e.endTime = t;
            result.PI.push_back(e);    
            Cmax = max(Cmax, t + e.q);
        }
    }
    result.cmax = Cmax;
    return result;
}

int UB = 100000;

int Calier(vector<Task> &taskVect)
{
    int U, LB;
    vector <Task> PI_prim;
    vector<Task> pi;
    pi = Schrage(taskVect).PI;
      
    int minR = 100000;
    int minQ = 100000;
    int sumP = 0;
    
    int x;
    
    U = Schrage(taskVect).cmax;
    // cout<<"U: "<<U <<" UB: "<<UB<<endl;
    
    if (U < UB)
    {
        UB = U;
        PI_prim = Schrage(taskVect).PI;
    }
    int position_B = find_B(U, pi);
    int position_A = findA(U, pi, position_B);
    int position_C = findC(pi, position_B, position_A);

// cout<<"Position_A: "<<position_A<<" Position_B: "<<position_B<<" Position_C: "<<position_C<<endl;

    if (position_C == -1)
    {
        return UB;
    }
    for (int i = (position_C + 1); i <= position_B; i++)
    {
        // cout<<"Czy tutaj wchodzi"<<endl;
        // cout << pi[i]<<endl;
        // cin>>x;
        if (pi[i].r < minR)
        {
            minR = pi[i].r;
            // cout<<"minR: "<<minR<<endl;
        }
        if (pi[i].q < minQ)
        {
            minQ = pi[i].q;
            // cout<<"minQ: "<<minQ<<endl;
        }
        sumP += pi[i].p;
    }

    int r_temp = pi[position_C].r;
    pi[position_C].r = max(pi[position_C].r, minR + sumP);

    LB = schragePodzial(pi);

    if (LB < UB)
    {
        // cout<<"1. LB: "<<LB << " UB: " << UB<<" U: "<<U<<endl;
        Calier(pi);
    }

    //odtwórz rpi(C)
    pi[position_C].r = r_temp;

    int q_temp = pi[position_C].q;
    pi[position_C].q = max(pi[position_C].q, minQ + sumP);
    
    LB = schragePodzial(pi);
    if (LB < UB)
    {
        // cout<<"2. LB: "<<LB << " UB: " << UB<<" U: "<<U<<endl;
        Calier(pi);
    }

    //odwtorz qpi(C);
    pi[position_C].q = q_temp;

    return UB;
}


int main()
{
    int resultTab[] = {32, 641, 1267, 1386, 3472, 3617, 6885, 6904, 72852};
               //Wynik 32, 641, 1267, 1386, 3472, 3617, 6885, 6904, 72852
    vector<Task> taskVect;
    
    taskVect = Load(taskVect, to_string(2)); 
    cout << Calier(taskVect)<<endl;

// for(int i = 1 ; i < 10; i++){
//     taskVect = Load(taskVect, to_string(i)); 
//     if(Calier(taskVect) == resultTab[i-1]){
//         cout<<i<<". "<<Calier(taskVect) << "  ==  "<<resultTab[i-1];
//         cout<<"   --> Good"<<endl;
//     }else{
//         cout<<i<<". "<<Calier(taskVect) << "  !=  "<<resultTab[i-1];
//         cout <<"  --> Bad"<<endl;
//     }
// }
      return 0;
}
