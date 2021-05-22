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

    friend istream &operator>>(istream &input, Task &ToDoTask);
    friend ostream &operator<<(ostream &output, Task &ToDoTask);
};

istream &operator>>(istream &input, Task &ToDoTask)
{
    input >> ToDoTask.r >> ToDoTask.p >> ToDoTask.q;
    return input;
}

ostream &operator<<(ostream &output, Task &ToDoTask)
{
    output << "r:" << ToDoTask.r << " p:" << ToDoTask.p << " q:" << ToDoTask.q;
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

int Load(priority_queue<Task, vector<Task>, N_collection> &N_set, string linesNumber)
{
    int i = 0;
    int n_lines;
    ifstream file;
    file.open("SCHRAGE" + linesNumber + ".DAT");
    file >> n_lines;
    // cout << "n_lines: " << n_lines << endl;
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

int Schrage(string linesNumber)
{
    int t = 0, k = 0, Cmax = 0;
    priority_queue<Task, vector<Task>, G_collection> G_set;
    priority_queue<Task, vector<Task>, N_collection> N_set;
    Task e;

    Load(N_set, linesNumber);

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
            Cmax = max(Cmax, t + e.q);
        }
        // cout << "Cmax: " << Cmax << endl;
        // cout << "t : " << t << endl;
    }
    cout << "Cmax: " << Cmax << endl;
    return Cmax;
}

int main()
{
    int resultTab[] = {32, 687, 1299, 1399, 3487, 3659, 6918, 6936, 72853};

    for (int i = 1; i < 10; i++)
    {
        if (Schrage(to_string(i)) == resultTab[i - 1])
        {
            cout << "Goood" << endl;
        }
        else
        {
            cout << "sth went wrong " << endl;
        }
    }
    return 0;
}
