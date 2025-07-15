#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<thread>


class  Operation{
    public:
    virtual void execute(std::vector<int> &row) = 0;
    virtual const std::string getName() const = 0;
    virtual ~Operation() = default;
};
class addOperation : public Operation
{
    int value;
    public:
    addOperation(int val):value(val){}
    void execute(std::vector<int>& row)  override
    {
        for (auto &val: row)
        {
            val += value;
        }
    }
    const std::string getName() const override
    {
        return "Add(" + std::to_string(value)+")";
    }

};

class subtractOperation : public Operation{
    int value;
    public:
    subtractOperation(int v):value(v){}
    void execute(std::vector<int> &row)  override
    {
        for (auto &val: row)
        {
            val -= value;
        }
    }
    const std::string getName() const    override
    {
        return "Subtracting (" + std::to_string(value) +")";
    }
};

class multiplyOperation : public Operation
{
    int value;
    public:
    multiplyOperation(int v):value(v){}
    void execute(std::vector<int> &row)
    {
        for(auto &val: row)
        {
            val *= value;
        }
    }
    const std::string getName() const  override
    {
        return "Multiplying (" + std::to_string(value) +")";
    }
};

class squareOperation : public Operation{
    public:
    squareOperation(int v){}
    void execute(std::vector<int>&row)
    {
        for(auto &val :row)
        {
            val *= val;
        }
        
    }
    const std::string getName() const    override
    {
        return "Square Operation";
    }
};

class matrix
{
    std::vector<std::vector<int>> data;
    int row,col;
    public:
    matrix(int rows, int cols):row(rows), col(cols)
    {
        data.resize(rows);
        for(int i = 0; i < rows ; i++)
        {
            data[i].resize(cols, 0);
            for(int j = 0; j < cols ;j++)
            data[i][j] = i*cols +(j+1);
        }
    }
    std::vector<std::vector<int>>& getData()
    {
        return data;
    }
    const std::vector<std::vector<int>>& getData() const{
        return data;
    }
    void print()
    {
        for(int i = 0; i < row ; i++)
        {
            
            for(int j = 0; j < col ;j++)
            std::cout << data[i][j] <<" ";

            std::cout << std::endl;
        }
    }
           
    
    int getRows()
    {
        return row;
    }
};


struct threadData
{
    std::vector<std::vector<int>>* mat;
    int row_index;
    std::unique_ptr<Operation> operation;
    threadData(std::vector<std::vector<int>>* matr, int row, std::unique_ptr<Operation> op):mat(matr),row_index(row), operation(move(op)){}
    ~threadData() {
    std::cout << "threadData destroyed for row " << row_index << std::endl;
}

};

class threadManager{
    public:
    static void * threadFunction(void* arg)
    {
        std::unique_ptr< threadData> data(static_cast<threadData*>(arg));
        pthread_t threadID = pthread_self();
        std::cout << " Starting " << " Thread: " << threadID << data->operation->getName() << " On row " << data->row_index << endl;
        data->operation->execute((*data->mat)[data->row_index]);
        std::cout << " finished "  << " Thread: " << threadID << data->operation->getName() << " On row " << data->row_index << endl;
        return nullptr;
    }
    static void processMatrix(matrix& mat, std::vector<std::unique_ptr<Operation>>& oper)
    {
        std::vector<pthread_t> threads(oper.size());

        for (int i = 0; i < oper.size(); i++)
        {
            std::unique_ptr<threadData> data = std::make_unique<threadData>( &mat.getData(), i, move(oper[i]));

            if(pthread_create(&threads[i], nullptr, threadFunction, data.release()) != 0)
            std::cerr << "Error creating thread for row " << i;
        }
        for (auto &t: threads)
        {
        pthread_join(t, nullptr);
        }

        std::cout<<"All operations complete."<< std::endl;
    }
};


class MatrixDemo{
    matrix Mat;
    int row, col;
    threadManager thread_manager;
    public:
    MatrixDemo(int rows, int cols): Mat(rows, cols), row(rows), col(cols){

    }
    void runDemo(){

        std::cout << "Initial Matrix: "<<std::endl;
        Mat.print();
       
        std::vector<std::unique_ptr<Operation>> operations;
        operations.push_back(std::make_unique<addOperation>(5));
        operations.push_back(std::make_unique<subtractOperation>(1));
        operations.push_back(std::make_unique<multiplyOperation>(2));
        operations.push_back(std::make_unique<squareOperation>(3));
        operations.push_back(std::make_unique<squareOperation>(3));
  
        thread_manager.processMatrix(Mat, operations);

        std::cout <<"Matrix after operatons: "<<std::endl;
        {Mat.print();}
      


    }
};

