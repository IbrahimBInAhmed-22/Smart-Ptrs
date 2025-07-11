#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<Windows.h>
using namespace std;

class Operation{
    public:
    virtual void execute(vector<int> &row) = 0;
    virtual string getName() = 0;
    virtual ~Operation() = default;
};
class addOperation : public Operation
{
    int value;
    public:
    addOperation(int val):value(val){}
    void execute(vector<int>& row)  override
    {
        for (auto &val: row)
        {
            val += value;
        }
    }
    const string getName() const 
    {
        return "Add(" + to_string(value) + ")\n";
    }

};

class subtractOperation : public Operation{
    int value;
    public:
    void execute(vector<int> &row)  override
    {
        for (auto &val: row)
        {
            val -= value;
        }
    }
    const string getName() const
    {
        return "Subtracting (" + to_string(value) +") \n";
    }
};

class multiplyOperation : public Operation
{
    int value;
    public:
    void execute(vector<int> &row)
    {
        for(auto &val: row)
        {
            val *= value;
        }
    }
    const string getName() const 
    {
        return "Multiplying (" + to_string(value) +")\n";
    }
};

class squareOperation : public Operation{
    public:
    void execute(vector<int>&row)
    {
        for(auto &val :row)
        {
            val *= val;
        }
        
    }
    const string getName() const{
        return "Square Operation";
    }
};

class threadData
{
    vector<vector<int>>* data;
    int row_index;
    unique_ptr<Operation> operation;
    threadData(vector<vector<int>>* mat, int row, unique_ptr<Operation> op):data(mat),row_index(row), operation(move(op)){}
};
class matrix{
    vector<vector<int>> data;
    matrix(int rows, int cols)
    {
        data.resize(rows);
        for(int i = 0; i < rows ; i++)
        {
            data[i].resize(cols, 0);
            for(int j = 0; j < cols ;j++)
            data[i][j] = i*cols +(j+1);
        }
    }
    vector<vector<int>>& getData()
    {
        return data;
    }
    const vector<vector<int>>& getData() const{
        return data;
    }
    void print()
    {
        for (const auto &row: data)
        {
            for(const auto &col: row)
            {
                cout<< col<<" ";
            }
            cout << endl;
        }
    }
};
class mutexOperation : public Operation
{
    ProcThreadAttributeAllApplicationPackagesPolicy
    int sleepTime;
    unique_ptr<Operation> first_op;
    unique_ptr<Operation> second_op;

};
