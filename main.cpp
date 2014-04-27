/*
 *  File: main.cpp
 *  Authors-Sprint1: Joseph Lillo, Dan Nix, Lisa Woody
 *  Authors-Sprint2: Ryan Brown, Kelsey Bellew, Ryan Feather
 *  Date: 2/19/14
 *  Class: Software Engineering
 *  Professor: Dr. Logar
 *  Description: Main entry point for software test suite program.
 *               This program allows the user to automatically compile
 *               a c++ program and test it with known input/output cases.
 *               The program is run from the command line with the name of the
 *               c++ file to test. This program will then compile the c++ file
 *               and recursively search child directories for .tst and .ans
 *               files. The program will then save the output from the tested
 *               program in a timestamped directory. This program will also
 *               output a timestamped log file that outlines the results of the
 *               test.
 */

#include "testsuite.h"

void presentationMenu(TestSuite &t)
{
    string presentationOpt = "", presentationType = "";
    bool validOpt = false;
        while(presentationOpt != "y" && presentationOpt != "n")
        {
            cout << "Do you want to ignore presentation errors? (y/n): ";
            cin >> presentationOpt;
        }

        if(presentationOpt == "y")
            t.presentationErrors = true;
        else
            t.presentationErrors = false;

    if(presentationOpt == "y")
    {
        validOpt = false;
        while(!validOpt)
        {
            cout << "What datatype presentation errors do you want to ignore?";
            cout << " (1 for floats, 2 for string): ";
            cin >> presentationType;

            if(presentationType == "1")
            {
                t.stringPresentationErrors = false;
                break;
            }
            else if(presentationType == "2")
            {
                t.stringPresentationErrors = true;
                break;
            }
        }

        t.presentationErrors = true;
    }
    else
        t.presentationErrors = false;
}


int main(int argc, char ** argv)
{
    int i;
    vector<string> cpps;
    string class_dir = "", presentationOpt = "";
    TestSuite t;
    vector<string> spec;
    bool menuTesting;
    //If they did not provide the minimum set of args
    if( argc < 3 )
    {
        cout << "Usage: ./tester [-g|-r] [directory] | [-p]" << endl;
        cout << "       -g: Generate test cases from \"golden\" .cpp" << endl;
        cout << "       -r: Run tests on student programs." << endl;
        cout << " 		-p: Profile student code." << endl;
        return -1;
    }

    class_dir = argv[2];
    if(chdir(argv[2]))
    {
        cout << "Failed to change to directory: " << class_dir;
    }
	
	
	
    string profile_flag = "";
    if(argc == 4)
    {
        profile_flag = argv[3];
        if(profile_flag == "-p")
            t.profiling = true;
    }



    //Choose one of two modes. [R]unning tests or [G]enerating tests.
    string flag = argv[1];
    if(flag == "-g")
    {
        t.dirCrawl(".spec", ".",spec);

        //if spec file exists, do menu driven testing
        if ((int)spec.size() != 0)
            menuTesting = t.menu_tests(spec[0]);
        //Call test generation function
        
        if(!menuTesting)
            t.helper_func();
    }
    else if(flag == "-r")
    {
        cout << "What is the maximum amount of time you would like a program to run";
        cout << " before it is considered an infinite loop (in seconds)?  ";
            cout << endl;
        cin >> t.allowed_time;
        
        presentationMenu(t);

        //fill "cpps" with the name of every .cpp to be ran
        t.dirCrawl(".cpp", ".", cpps);


        //loop through every .cpp and run it
        for(i=0; i<(int)cpps.size(); i++)
        {
            //Excludes the "golden" .cpp from being evaluated
            if(count(cpps.at(i).begin(), cpps.at(i).end(), '/') > 1)
            {
				cout << cpps.at(i) << endl;
                //t.initTest(argv[1],".tst",".ans");
                t.initTest( cpps.at(i) ,".tst",".ans");
                t.runTests();
                t.outputLogFile();
            }
        }
        //end for loop


        system ("rm dummy.out");
        system ("rm test_out.klein");
        t.createSummary();
    }
    else
    {
        cout << "Unrecognized command line option. [-g|-r]" << endl;
        return -2;
    }
    return 0;
}



