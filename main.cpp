#include"demoSmartPtr.h"
#include "thread.h"
int main()
{
    int choice;
        while(true)
        {
    cout << "\n\n\n Enter:\n1 for Smart Ponter Demo class or \n2 for Matrix operations using threading: \n-1 to exit.\n";
    cin >> choice;

    
    if (choice == 1)
    {
        demoSmartPtrs demo;
        demo.demo();

    }
    else if (choice == 2)
    {    
    MatrixDemo demo(5,5);
    demo.runDemo();

    }
    else if(choice = -1)
    {
        return 0;
    }
    else
    {
        cout << "Invalid choice. Exiting.\n";

    }

    }

}