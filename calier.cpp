#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class schrageOutput
{
public:
    int cmax;
    int pi;
    // int timeTab[];
};

class Task
{
public:
    int r;
    int p;
    int q;
    int endTime;
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

int Load(priority_queue<Task, vector<Task>, N_collection> &N_set, string fileNumber)
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
        N_set.push(v_tasks[i]);
        i++;
    }
    file.close();
    return n_lines;
}

int schragePodzial(string fileNumber)
{
    int t = 0, k = 0, Cmax = 0;
    priority_queue<Task, vector<Task>, G_collection> G_set;
    priority_queue<Task, vector<Task>, N_collection> N_set;
    Task e;
    Task l;
    l.q = 0;
    l.p = 0;
    l.r = 0;

    Load(N_set, fileNumber);

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

int find_B(schrageOutput &schrage, vector<Task> &taskVect)
{
    int b_position = -1;
    for (int i = taskVect.size() - 1; i > 0; i--)
    {
        if (schrage.cmax == taskVect[i].endTime + taskVect[i].q)
        {
            b_position = i;
            break;
        }
    }
    return b_position;
}

int findA(schrageOutput &schrage, vector<Task> &taskVect, int b_position)
{
    int a_position = -1;
    int p_sum = 0;
    for (int i = 0; i < taskVect.size(); i++)
    {
        for (int s = i; s <= b_position; s++)
        {
            p_sum += taskVect[s].p;
        }
        if (schrage.cmax == taskVect[i].r + p_sum + taskVect[b_position].q)
        {
            a_position = i;
            break;
        }
    }
    return a_position;
}

int findC(schrageOutput &schrage, vector<Task> &taskVect, int b_position, int a_position)
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

void Schrage(string fileNumber, schrageOutput &result, vector<Task> &taskVect)
{
    int t = 0, k = 0;
    result.cmax = 0;
    priority_queue<Task, vector<Task>, G_collection> G_set;
    priority_queue<Task, vector<Task>, N_collection> N_set;
    Task e;

    Load(N_set, fileNumber);
    // cout << "Rozmiar tablicy: " << sizeof(timeTab) / sizeof(timeTab[0]) << endl;

    int index = 0;
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
            taskVect.push_back(e);
            G_set.pop();
            k++;
            result.pi = e.q;
            t = t + e.p;
            taskVect[index].endTime = t;
            index++;
            result.cmax = max(result.cmax, t + e.q);
        }
        // cout << "Cmax: " << result.cmax << endl;
        // cout << "Czas zakonczenia zadania : " << t << endl;
    }
    // cout << "Cmax: " << result.cmax << endl;
    // cout << "Permutacja: " << result.pi << endl;

    // return result;
}

// for (int i = 0; i < 6; i++)
// {
//     cout << "Vector: " << taskVect[i] << endl;
// }
int Calier(schrageOutput &schrage, vector<Task> &taskVect, int UB, int fileNumber)
{
    int U, LB;
    int PI_prim;
    int minR = 0;
    int minQ = 0;
    int sumP = 0;
    // int fileNumber = 2;

    Schrage(to_string(fileNumber), schrage, taskVect);
    U = schrage.cmax;
    if (U < UB)
    {
        UB = U;
        PI_prim = schrage.pi;
    }
    int position_B = find_B(schrage, taskVect);
    int position_A = findA(schrage, taskVect, position_B);
    int position_C = findC(schrage, taskVect, position_B, position_A);
    if (position_C == -1)
    {
        return UB;
    }
    for (int i = (position_C + 1); i <= position_B; i++)
    {
        if (taskVect[i].r < minR)
        {
            minR = taskVect[i].r;
        }
        if (taskVect[i].q < minQ)
        {
            minQ = taskVect[i].q;
        }
        sumP += taskVect[i].p;
    }

    int r_temp = taskVect[position_C].r;
    taskVect[position_C].r = max(taskVect[position_C].r, minR + sumP);

    LB = schragePodzial(to_string(fileNumber));

    if (LB < UB)
    {
        UB = Calier(schrage, taskVect, UB, fileNumber);
    }

    //odtwórz rpi(C)
    taskVect[position_C].r = r_temp;

    int q_temp = taskVect[position_C].q;
    taskVect[position_C].q = max(taskVect[position_C].q, minQ + sumP);
    LB = schragePodzial(to_string(fileNumber));
    if (LB < UB)
    {
        UB = Calier(schrage, taskVect, UB, fileNumber);
    }

    //odwtorz qpi(C);
    taskVect[position_C].q = q_temp;

    return UB;
}

int main()
{
    int resultTab[] = {32, 687, 1299, 1399, 3487, 3659, 6918, 6936, 72853};
    schrageOutput result;
    vector<Task> taskVect;
    int UB = 100000;
    // Calier(result, taskVect);
    cout << "Calier: " << Calier(result, taskVect, UB, 7) << endl;
    // result = Schrage(to_string(1));

    for (int i = 1; i < 2; i++)
    {
        cout << "Calier: " << Calier(result, taskVect, UB, i) << endl;
        //     result = Schrage(to_string(i));
        //     if (result.cmax == resultTab[i - 1])
        //     {
        //         cout << "Goood" << endl;
        //     }
        //     else
        //     {
        //         cout << "sth went wrong " << endl;
        //     }
    }
    return 0;
}
